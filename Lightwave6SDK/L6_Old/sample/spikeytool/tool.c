/*
 * TOOL.C -- Spikey operation as an interactive mesh-edit tool.
 *
 * Copyright 1999, NewTek, Inc.
 * written by Stuart Ferguson
 * last revision  8/30/99
 */
#include <lwsdk/lwserver.h>
#include <lwsdk/lwmodtool.h>
#include "spikey.h"


/*
 * The Spikey tool is the successor to the venerable 'make spikey' command.
 * The spikey tool employs the same mesh edit code to generate the new
 * geometry, while also allowing the parameters of the operation to be varied
 * interactively with a tool.  The tool state-machine is similar to bevel.
 */
typedef struct st_SpikeyTool {
	double		  spike, s0;
	int		  update;
} SpikeyTool;


/*
 * The 'build' function is called, on the request of the tool, to recompute
 * the new geometry whenever the parameters of the operation have changed.
 * Every time it runs, it resets the update state for the tool.
 */
	static LWError
Spikey_Build (
	SpikeyTool		*tool,
	MeshEditOp		*op)
{
	EDError			 err;

	err = Spikey (op, tool->spike);
	tool->update = LWT_TEST_NOTHING;
	return (err ? "Failed" : NULL);
}


/*
 * The test function returns the mesh update action for the tool.  Mostly
 * this will be NOTHING, but sometimes there will be a need to advance to a
 * new state.  This will always result in a call to 'build' which will
 * reset the update state to NOTHING again.
 */
	static int
Spikey_Test (
	SpikeyTool		*tool)
{
	return tool->update;
}


/*
 * End is called when the interactive build is complete, either by the
 * direct request of the tool itself, or by the implicit action of some
 * other aspect of the application.  In every case we reset the spikey
 * percentage for the next operation and reset the update state.
 */
	static void
Spikey_End (
	SpikeyTool		*tool,
	int			 keep)
{
	tool->spike  = 0.0;
	tool->update = LWT_TEST_NOTHING;
}


/*
 * This function is called to get the help text for the tool which will
 * be displayed in the status line while the tool is active.  If the text
 * were variable, the 'dirty' function would be required to update it.
 */
	static const char *
Spikey_Help (
	SpikeyTool		*tool,
	LWToolEvent		*event)
{
	return "Drag left or right to adjust the spikey percentage";
}


/*
 * Raw mouse actions come in the form of 'down', 'move' and 'up' events.
 * On the down event we record the current spike factor which we will
 * use as the base vale to vary on mouse moves.  If the user presses the
 * alternate mouse button, we complete the last action and start a new
 * one with zero spikeyness.  This is consistent with the bevel tool.
 */
	static int
Spikey_Down (
	SpikeyTool		*tool,
	LWToolEvent		*event)
{
	if (event->flags & LWTOOLF_ALT_BUTTON) {
		tool->spike  = 0.0;
		tool->update = LWT_TEST_CLONE;
	} else
		tool->update = LWT_TEST_UPDATE;

	tool->s0 = tool->spike;
	return 1;
}


/*
 * On the mouse move event, we set the new spikey factor to be the value
 * at initial mouse down, plus 1% for each pixel the user has offset to
 * right of the initial position.  The update state has to be set since
 * we need to compute the mesh for the new spike factor.
 */
	static void
Spikey_Move (
	SpikeyTool		*tool,
	LWToolEvent		*event)
{
	tool->spike  = tool->s0 + 0.01 * event->dx;
	tool->update = LWT_TEST_UPDATE;
}


/*
 * The 'drop' action is caused when the user clicks in the blank area of
 * the display or uses the keyboard equivalent.  Our response is to force
 * a rejection of any interactive action partly complete.
 */
	static void
Spikey_Drop (
	SpikeyTool		*tool)
{
	tool->update = LWT_TEST_REJECT;
}


/*
 * The options for the spikey tool consist of an xPanel that will be
 * placed into the "Numeric" window in Modeler.  This is a VIEW panel
 * which displays the properties of the spikey instance.  In this case
 * there is only one property: the spikiness percentage.  When this is
 * altered from the panel, the update state is set to compute the new
 * geometry.
 */
#define XID_SPIK	LWID_('s','p','i','k')

	static void *
Spikey_Get (
	SpikeyTool		*tool,
	unsigned long		 vid)
{
	if (vid == XID_SPIK)
		return &tool->spike;

	return NULL;
}

	static int
Spikey_Set (
	SpikeyTool		*tool,
	unsigned long		 vid,
	double			*value)
{
	if (vid != XID_SPIK)
		return 0;

	tool->spike  = value[0];
	tool->update = LWT_TEST_UPDATE;
	return 1;
}

	static LWXPanelID
Spikey_Panel (
	SpikeyTool		*tool)
{
	static LWXPanelDataDesc	 def[] = {
		{ XID_SPIK,  "Spike Factor",	"percent" },
		{ 0 }
	};
	LWXPanelID		 pan;

	pan = (*globFun_pan->create) (LWXP_VIEW, (LWXPanelControl *) def);
	if (!pan)
		return NULL;

	(*globFun_pan->describe) (pan, def, Spikey_Get, Spikey_Set);
	return pan;
}


/*
 * Free the tool instance when done.
 */
	static void
Spikey_Done (
	SpikeyTool		*tool)
{
	free (tool);
}


/*
 * Main entry point for mesh operation.
 */
	XCALL_(int)
Activate (
	long			 version,
	GlobalFunc		*global,
	LWMeshEditTool		*local,
	void			*serverData)
{
	SpikeyTool		*tool;

	if (version != LWMESHEDITTOOL_VERSION)
		return AFUNC_BADVERSION;

	/*
	 * Get global data -- monitor and xPanels functions.
	 */
	globFun_mon = (*global) ("LWM: Dynamic Monitor", GFUSE_TRANSIENT);
	if (!globFun_mon)
		return AFUNC_BADGLOBAL;

	globFun_pan = (*global) (LWXPANELFUNCS_GLOBAL, GFUSE_TRANSIENT);
	if (!globFun_pan)
		return AFUNC_BADGLOBAL;

	/*
	 * Allocate and init the tool instance.
	 */
	tool = malloc (sizeof (SpikeyTool));
	if (!tool)
		return AFUNC_OK;

	memset (tool, 0, sizeof (*tool));
	tool->spike  = 0.0;
	tool->update = LWT_TEST_NOTHING;

	/*
	 * Initialize the local tool struct with instance and handler
	 * callback functions.
	 */
	local->instance    = tool;
	local->tool->done  = Spikey_Done;
	local->tool->help  = Spikey_Help;
	local->tool->down  = Spikey_Down;
	local->tool->move  = Spikey_Move;
	local->tool->drop  = Spikey_Drop;
	local->tool->panel = Spikey_Panel;
	local->build       = Spikey_Build;
	local->test        = Spikey_Test;
	local->end         = Spikey_End;

	return AFUNC_OK;
}


/*
 * Globals necessary to declare the class and name of this plugin server.
 */
char		ServerClass[] = LWMESHEDITTOOL_CLASS;
char		ServerName[]  = "LWSpikeyTool";
