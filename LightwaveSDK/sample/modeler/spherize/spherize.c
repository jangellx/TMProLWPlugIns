/*
 * SPHERIZE.C -- Modeler Plugin Mesh Edit
 *		 Conform selected points to unit sphere
 *
 * written by Stuart Ferguson
 * last revision  12/6/94
 */
#include <splug.h>
#include <lwmod.h>
#include <string.h>
#include <math.h>


/*
 * Local information packet.  This includes the mesh edit context and the
 * monitor, if any.
 */
typedef struct st_MyData {
	MeshEditOp		*op;
	Monitor			*mon;
} MyData;


/*
 * Point scan callback for spherizing.  This is called for every point in
 * the primary layer dataset, so we must filter for selection mode.
 */
	static EDError
Spherize (
	MyData			*dat,
	const PointInfo		*info)
{
	MeshEditOp		*op = dat->op;
	double			 new[3];
	double			 d;
	int			 i;

	/*
	 * Skip this point if it is not selected.
	 */
	if (!(info->flags & PPDF_SELECT))
		return EDERR_NONE;

	/*
	 * Count this selected point in the aggregate for the monitor.  The
	 * step function returns True if the user has requested an abort,
	 * which we can propogate by returning the appropriate code.
	 */
	if (dat->mon && (*dat->mon->step) (dat->mon->data, 1))
		return EDERR_USERABORT;

	/*
	 * Compute distance of point to origin.  If zero, skip this point.
	 */
	d = 0.0;
	for (i = 0; i < 3; i++)
		d += info->position[i] * info->position[i];

	d = sqrt (d);
	if (d <= 0.0)
		return EDERR_NONE;

	/*
	 * Compute normalized coordinates for the point and attempt to
	 * move it.  Any error that might result will halt the scan and
	 * be returned to the activation function.
	 */
	for (i = 0; i < 3; i++)
		new[i] = info->position[i] / d;

	return ((*op->pntMove) (op->state, info->pnt, new));
}


/*
 * Entry point for mesh edit plugin.
 */
	XCALL_(int)
Activate (
	long			 version,
	GlobalFunc		*global,
	MeshEditBegin		*local,
	void			*serverData)
{
	MeshEditOp		*op;
	DynaMonitorFuncs	*mfunc;
	MyData			 dat;
	unsigned int		 count;
	EDError			 err;

	/*
	 * Check interface version.
	 */
	XCALL_INIT;
	if (version != 1)
		return AFUNC_BADVERSION;

	/*
	 * Get global data -- monitor functions.
	 */
	mfunc = (*global) ("LWM: Dynamic Monitor", GFUSE_TRANSIENT);
	if (!mfunc)
		return AFUNC_BADGLOBAL;

	/*
	 * Attempt to begin edit operation.  If the startup fails we still
	 * return OK, but just don't do anything.
	 */
	op = (*local) (0, 0, OPSEL_USER);
	if (!op)
		return AFUNC_OK;

	/*
	 * Start the monitor.  If we can create a monitor, count the points
	 * that will be affected by the operation and init the monitor with
	 * that value.
	 */
	dat.mon = (*mfunc->create) ("Spherize", NULL);
	if (dat.mon) {
		count = (*op->pointCount) (op->state, OPLYR_PRIMARY,
					   EDCOUNT_SELECT);
		(*dat.mon->init) (dat.mon->data, count);
	}

	/*
	 * Scan points and apply transformation.  The `scan' function will
	 * traverse the database of points for the primary edit layer and
	 * call the client's function with the client's data pointer and
	 * info for the point.  Errors (or any non-zero value) returned by
	 * the client's function will be returned here.  If the scan
	 * completes, EDERR_NONE is returned.
	 */
	dat.op = op;
	err = (*op->pointScan) (op->state, Spherize, &dat, OPLYR_PRIMARY);

	/*
	 * End the monitor whether we completed the process or not.
	 */
	if (dat.mon) {
		(*dat.mon->done) (dat.mon->data);
		(*mfunc->destroy) (dat.mon);
	}

	/*
	 * Complete the operation by calling `done.'  We will pass an error
	 * code if there was one.
	 */
	(*op->done) (op->state, err, 0);
	return AFUNC_OK;
}


/*
 * Globals necessary to declare the class and name of this plugin server.
 */
char		ServerClass[] = "MeshDataEdit";
char		ServerName[]  = "Demo_Spherize";
