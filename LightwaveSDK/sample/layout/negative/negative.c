/*
 * NEGATIVE.C -- Layout Plugin Post-Process Filter
 *		 Invert color of image after rendering
 *
 * by Allen Hastings and Arnie Cachelin
 * revised by Stuart Ferguson
 * last revision  6/9/95
 */
#include <splug.h>
#include <moni.h>
#include <lwran.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/*
 * HANDLER -- Instance Methods
 *
 * An instance is a particular set of user-set options for a plug-in
 * operation.  Normally the host would call these callback functions
 * to manage instances, but since the Negative image filter has no
 * options, only a dummy instance needs to be used.  In this case the
 * pointer for all instances will just be a pointer to a string and 
 * the instance handler functions will do nothing.
 */
static char		 dummyInst[] = "NIL";


// NilCreate()
XCALL_(LWInstance) NilCreate( LWError	*err) {
	return ((LWInstance) dummyInst);
}

// NilDestroy()
XCALL_(void) NilDestroy ( LWInstance inst) {
}

// NilCopy()
XCALL_(LWError) NilCopy ( LWInstance from, LWInstance to ) {
  return ((LWInstance) dummyInst);
}

// NilLoad()
XCALL_(LWError) NilLoad ( LWInstance inst, const LWLoadState *lState) {
	return (NULL);
}

// NilSave()
XCALL_(LWError) NilSave ( LWInstance inst, const LWSaveState	*sState ) {
	return (NULL);
}



/*
 * PROCESSING
 *
 * The host calls this function to process each rendered frame.  The
 * server has to process the whole frame, line-by-line.  In this case
 * the red, green and blue buffers are read and inverted before being
 * set back, and the alpha buffer is simply copied.  All pixels must
 * be touched even if only a few or none are changed.
 *
 * The monitor in the filter access is used to track the progress of
 * the filter for the user and to check for user abort.  If this is
 * used with the pre-release, the monitor is not present in the
 * access struct and is nulled.
 */

XCALL_(static void) NegProcess ( LWInstance inst, const FilterAccess *fa) {
	Monitor     *mon;
	BufferValue  out[4], *r, *g, *b, *a;
	int          i, j;

	mon = fa->monitor;

  // Scan through the lines in the image and get image buffer
  // line pointers for the RGB and alpha buffers of the original
  // image.
  //
	MON_INIT (mon, fa->height);
	for (i = 0; i < fa->height; i++) {
		r = (*fa->bufLine) (LWBUF_RED,   i);
		g = (*fa->bufLine) (LWBUF_GREEN, i);
		b = (*fa->bufLine) (LWBUF_BLUE,  i);
		a = (*fa->bufLine) (LWBUF_ALPHA, i);

		//
  	// For each pixel in the current line, compute the
  	// negative color value and leave the alpha alone.
		//
		for (j = 0; j < fa->width; j++) {
			out[0] = 255 - r[j];
			out[1] = 255 - g[j];
			out[2] = 255 - b[j];
			out[3] = a[j];

			(*fa->setRGB)   (j, i, out);
			(*fa->setAlpha) (j, i, out[3]);
		}

		//
		// Step the monitor and check for abort on each line.
		// This is a good compromise of responsiveness and
		// input-checking overhead.
		//
		if (MON_STEP (mon))
			return;
	}

	MON_DONE (mon);
}



/*
 * FLAGS
 *
 * The host calls this function to determine what options are needed by
 * this instance of filter.  Specifically, which buffers other than the
 * normal RGBA buffers need to be computed for this filter.  Since the
 * negative operation olny operates on colors, no additional buffers
 * are needed.
 */
XCALL_(static unsigned int) NegFlags ( LWInstance inst) {
	return (0);
}



/*
 * ACTIVATE -- Main entry point
 *
 * The host calls this function with an uninitialized image filter handler
 * and the server has to fill in the callback fields for the handler
 * functions for this handler class.  Since we don't care about the descln
 * method (this handler has no instance parameters), we can just act like
 * an old version-1 server.
 */
XCALL_(int) Activate ( long version, GlobalFunc *global, ImageFilterHandler	*local,
                       void *serverData) {
	XCALL_INIT;
	if (version != 1)
		return (AFUNC_BADVERSION);

	local->create  = NilCreate;
	local->destroy = NilDestroy;
	local->load    = NilLoad;
	local->save    = NilSave;
	local->copy    = NilCopy;

	local->process = NegProcess;
	local->flags   = NegFlags;

	return (AFUNC_OK);
}


/*
 * Globals necessary to declare the class and name of this plugin server.
 */
char		ServerClass[] = "ImageFilterHandler";
char		ServerName[]  = "Demo_Negative";
