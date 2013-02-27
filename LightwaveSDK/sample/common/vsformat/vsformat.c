/*
 * VSFORMAT.C -- LightWave Plugin Object Loader
 *		 Loads the old VideoScape format, text only
 *
 * written by Stuart Ferguson
 * last revision  12/6/94
 */
#include <splug.h>
#include <lwmod.h>
#include <string.h>
#include <stdio.h>
#include <math.h>



/*
 * Read a two byte integer.  Returns true if the read was successful.
 */
	static int
ReadShort (
	FILE			*file,
	short			*val)
{
	int			 n, i;

	n = fscanf (file, "%d", &i);
	*val = i;
	return (n == 1);
}


/*
 * Read a floating point format value.  Returns true if the read was
 * successful.
 */
	static int
ReadFloat (
	FILE			*file,
	float			*val)
{
	double			 d;
	int			 n;

	n = fscanf (file, "%lf", &d);
	*val = d;
	return (n == 1);
}


/*
 * For the surface number we look up the index and if this is the
 * first time the surface is added, we set the parameters.
 */
	static int
SurfIndexFromColor (
	ObjectImport		*local,
	int			 color)
{
	unsigned char		*dp;
	int			 isurf, on, first;
	static char		 name[28], buf[64];

	sprintf (name, "VideoScape Color %d", color);
	isurf = (*local->surfIndex) (local->data, name, &first);

	if (!first || color >= 256)
		return isurf;

	/*
	 * A surface parameter block is just what we would expect
	 * to find in a SURF chunk, minus the surface name.  This
	 * macro inserts a small color chunk header on the buffer.
	 */
	dp = buf;
#define SURF_HDR(p,a,b,c,d,n)	p[0]=a; p[1]=b; p[2]=c; p[3]=d;\
				p[4]=0; p[5]=n; p+=6;

	/*
	 * The COLR chunk contains a crude RGB based on the low
	 * 4 bits of the color number.
	 */
	SURF_HDR (dp, 'C','O','L','R', 4);
	on = (color & 8 ? 240 : 140);
	*dp++ = (color & 4 ? on : 0);
	*dp++ = (color & 2 ? on : 0);
	*dp++ = (color & 1 ? on : 0);
	*dp++ = 0;

	/*
	 * The FLAG chunk contains a bit for smoothing if the
	 * smoothing bit is set in the color number.
	 */
	SURF_HDR (dp, 'F','L','A','G', 2);
	*dp++ = 0;
	*dp++ = (color & 128 ? (1<<2) : 0);

	/*
	 * The DIFF chunk contains a diffuse value of 255 for non-
	 * luminous surfaces, or 0 if luminous.
	 */
	SURF_HDR (dp, 'D','I','F','F', 2);
	*dp++ = 0;
	*dp++ = (color & 32 ? 0 : 255);

	/*
	 * If the color is luminous and diffuse is zero, we need
	 * a luminous sub-chunk with a 255 (100%) setting.
	 */
	if (color & 32) {
		SURF_HDR (dp, 'L','U','M','I', 2);
		*dp++ = 0;
		*dp++ = 255;
	}

	/*
	 * If the transparent bit is set, the TRAN chunk will have
	 * a 50% transparency value.
	 */
	if (color & 64) {
		SURF_HDR (dp, 'T','R','A','N', 2);
		*dp++ = 0;
		*dp++ = 127;
	}

	/*
	 * If the glossy bit is set, the color will have a specular
	 * setting of 255 and glossiness of 64.
	 */
	if (color & 64) {
		SURF_HDR (dp, 'S','P','E','C', 2);
		*dp++ = 0;
		*dp++ = 255;

		SURF_HDR (dp, 'G','L','O','S', 2);
		*dp++ = 0;
		*dp++ = 64;
	}

	(*local->surfData) (local->data, name, (int)(dp - buf), buf);
	return isurf;
}


/*
 * Read a polygon description from file.  Returns zero if this was the last
 * polygon detected, or if there was an error in which case the result
 * field will be set.
 */
	static int
ReadPolygon (
	ObjectImport		*local,
	FILE			*file,
	int			 isDetail)
{
	short			 color, nvert, numDet;
	int			 hasDetails, i, isurf, flags;
	static unsigned short	 verts[200];
	const char		*errmsg = NULL;

	/*
	 * Read number of vertices in the next polygon.  Deal with end of
	 * file and other cases.
	 */
	if (!ReadShort (file, &nvert))
		return 0;

	if (!nvert)
		return 0;

	if (nvert > 200) {
		errmsg = "Geo format: Polygon too large";
		goto failed;
	}

	/*
	 * Read polygon vertex list given by number of vertices expected
	 * as indicies into the already read vertex list.
	 */
	for (i = 0; i < nvert; i++)
		if (!ReadShort (file, (short *) &verts[i]))
			goto failed;

	/*
	 * Next value must be VideoScape surface number.
	 */
	if (!ReadShort (file, &color))
		goto failed;

	/*
	 * If negative, this polygon has some detail polygons.
	 */
	hasDetails = (color < 0);
	if (hasDetails)
		color = -color;

	/*
	 * Create the polygon given the number of vertices, the vertex list
	 * as point indices, the flags (a face which might be a detail) and
	 * the surface index from above.
	 */
	isurf = SurfIndexFromColor (local, color);
	flags = OBJPOLF_FACE | (isDetail ? OBJPOLF_DETAIL : 0);
	(*local->polygon) (local->data, nvert, isurf, flags, verts);

	/*
	 * If there are no details, this polygon is read successfully.
	 */
	if (!hasDetails)
		return 1;

	/*
	 * Read the details recursively for the given number of polygon.
	 */
	if (isDetail || !ReadShort (file, &numDet))
		goto failed;

	for (i = 0; i < numDet; i++)
		if (!ReadPolygon (local, file, 1))
			goto failed;

	return 1;

 failed:
	local->result = OBJSTAT_FAILED;
	if (!errmsg)
		errmsg = "Geo format: EOF in polygon definition";
	if (local->failedLen > 0)
		strncpy (local->failedBuf, errmsg, local->failedLen);

	return 0;
}


/*
 * Object file loading entry point.  We get a filename and have the option
 * of attempting to load it or passing on it altogether.
 */
	XCALL_(int)
Activate (
	long			 version,
	GlobalFunc		*global,
	ObjectImport		*local,
	void			*serverData)
{
	FILE			*file;
	float			 pos[3];
	char			 str[6];
	short			 npts;
	int			 i, ok;
	const char		*errmsg;

	/*
	 * Check interface version.
	 */
	XCALL_INIT;
	if (version != 1)
		return AFUNC_BADVERSION;

	/*
	 * Open the file for read, text mode.  If it fails, we exit with
	 * a BADFILE result.
	 */
	file = fopen (local->filename, "r");
	if (!file) {
		local->result = OBJSTAT_BADFILE;
		return AFUNC_OK;
	}

	/*
	 * Test sync pattern.  If we don't get what we expect, we exit
	 * with a NOREC result.
	 */
	fread (str, (size_t)1, (size_t)4, file);
	if (strncmp (str, "3DG1", (size_t)4)) {
		local->result = OBJSTAT_NOREC;
		fclose (file);
		return AFUNC_OK;
	}

	/*
	 * Read number of points which must be non-zero.
	 */
	if (!ReadShort (file, &npts) || !npts) {
		errmsg = "Geo format: No point count";
		goto failed;
	}

	/*
	 * Begin sending object data by sending number of points.  Result
	 * code will be left alone unless there is a read failure.
	 */
	local->result = OBJSTAT_OK;
	(*local->begin) (local->data, NULL);
	(*local->numPoints) (local->data, (int)npts);

	/*
	 * Read all the points one number at a time.
	 */
	for (i = 0; i < npts; i++) {
		ok  = ReadFloat (file, &pos[0]);
		ok &= ReadFloat (file, &pos[1]);
		ok &= ReadFloat (file, &pos[2]);
		if (!ok) {
			errmsg = "Geo format: Not enough points";
			goto failed;
		}

		(*local->points) (local->data, 1, pos);
	}

	/*
	 * Scan polygons until ReadPolygon returns False, which signals the
	 * end of file or an error in which case the result value is set.
	 */
	while (ReadPolygon (local, file, 0))
		;

	/*
	 * Normal exit.  The `done' callback is called to end sending data
	 * to the host and the file is closed.
	 */
	(*local->done) (local->data);
	fclose (file);
	return AFUNC_OK;


	/*
	 * Abnormal exit.  This sets the result code to failure and closes
	 * the file and exits.
	 */
 failed:
	fclose (file);
	local->result = OBJSTAT_FAILED;
	if (errmsg && local->failedLen > 0)
		strncpy (local->failedBuf, errmsg, local->failedLen);

	return AFUNC_OK;
}


/*
 * Globals necessary to declare the class and name of this plugin server.
 */
char		ServerClass[] = "ObjectLoader";
char		ServerName[]  = "Demo_3DG1";
