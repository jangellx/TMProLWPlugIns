/*
 * BENDER.C -- Another test interactive mesh-edit tool.
 *
 * Copyright 1999, NewTek, Inc.
 * written by Stuart Ferguson
 * last revision  8/30/99
 */
#include <lwsdk/lwserver.h>
#include <lwsdk/lwmodtool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


/*
 * Eventually this could be something really useful, but for now it only
 * demonstrates some of the handle interaction methods.
 */
typedef struct st_BenderTool {
	LWFVector	 v[2];
	int		 active;
	int		 dirty;
} BenderTool;


	static void
Bender_Draw (
	BenderTool		*tool,
	LWWireDrawAccess	*draw)
{
	if (!tool->active)
		return;

	(*draw->moveTo) (draw->data, tool->v[0], 0);
	(*draw->lineTo) (draw->data, tool->v[1], 0);
	tool->dirty = 0;
}


	static int
Bender_Dirty (
	BenderTool		*tool)
{
	return (tool->dirty ? LWT_DIRTY_WIREFRAME : 0);
}


	static int
Bender_Count (
	BenderTool		*tool,
	LWToolEvent		*event)
{
	return (tool->active ? 2 : 0);
}


	static int
Bender_Handle (
	BenderTool		*tool,
	LWToolEvent		*event,
	int			 i,
	LWDVector		 pos)
{
	pos[0] = tool->v[i][0];
	pos[1] = tool->v[i][1];
	pos[2] = tool->v[i][2];
	return 1;
}


	static int
Bender_Adjust (
	BenderTool		*tool,
	LWToolEvent		*event,
	int			 i)
{
	tool->v[i][0] = event->posSnap[0];
	tool->v[i][1] = event->posSnap[1];
	tool->v[i][2] = event->posSnap[2];
	tool->dirty = 1;
	return i;
}


	static void
Bender_Destroy (
	BenderTool		*tool)
{
	free (tool);
}


/*
 * Main entry point for mesh tool initialization.
 */
	XCALL_(int)
Activate (
	long			 version,
	GlobalFunc		*global,
	LWMeshEditTool		*local,
	void			*serverData)
{
	BenderTool		*tool;

	if (version != LWMESHEDITTOOL_VERSION)
		return AFUNC_BADVERSION;

	tool = malloc (sizeof (BenderTool));
	if (!tool)
		return AFUNC_OK;

	memset (tool, 0, sizeof (*tool));
	tool->active = 1;
	tool->v[0][0] = -1.0;
	tool->v[0][1] = -1.0;
	tool->v[0][2] = -1.0;
	tool->v[1][0] =  1.0;
	tool->v[1][1] =  1.0;
	tool->v[1][2] =  1.0;

	local->instance     = tool;
	local->tool->done   = Bender_Destroy;
	local->tool->draw   = Bender_Draw;
	local->tool->count  = Bender_Count;
	local->tool->handle = Bender_Handle;
	local->tool->adjust = Bender_Adjust;
	local->tool->dirty  = Bender_Dirty;

	return AFUNC_OK;
}


/*
 * Globals necessary to declare the class and name of this plugin server.
 */
char		ServerClass[] = LWMESHEDITTOOL_CLASS;
char		ServerName[]  = "LWBenderTool";
