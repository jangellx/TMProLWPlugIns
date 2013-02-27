/*
 * LAYERSET.C -- Modeler Plugin Command
 *		 Two operations to change layers
 *
 * written by Stuart Ferguson
 * last revision  12/6/94
 */
#include <splug.h>
#include <lwmod.h>
#include <stdio.h>


/*
 * If the DEBUG symbol is defined, a debugging output window will be
 * opened as the server data for this module.  Unfortunately this only
 * works on the Amiga where we can open console windows as simple files.
 */
/* #define DEBUG */

#ifndef _AMIGA
 #ifdef  DEBUG
  #undef DEBUG
 #endif
#endif

#ifdef DEBUG
 #define DPRT(a)	fprintf a
 #define DFLUSH		fflush (debugPort)
#else
 #define DPRT(a)
 #define DFLUSH
#endif


/*
 * Send a command to select the next empty layer after this one.
 */
	XCALL_(static int)
NextEmpty (
	long			 version,
	GlobalFunc		*global,
	ModCommand		*local,
	FILE			*debugPort)
{
	StateQueryFuncs		*query;
	unsigned int		 empty, fg;
	CommandCode		 ccSetLayer;
	DynaValue		 dval;
	int			 num, i, max, l, lset;

	/*
	 * Standard version check.
	 */
	XCALL_INIT;
	if (version != 1)
		return AFUNC_BADVERSION;

	/*
	 * Get the state query functions and the command code for the
	 * set layer command.
	 */
	query = (*global) ("LWM: State Query", GFUSE_TRANSIENT);
	if (!query)
		return AFUNC_BADGLOBAL;

	ccSetLayer = (*local->lookup) (local->data, "setlayer");
	if (!ccSetLayer)
		return AFUNC_BADLOCAL;

	DPRT ((debugPort, "Next Empty...\n"));

	/*
	 * Get the total number of layers and the mask for the empty layers
	 * and current layers.  Then scan through the fg layer mask and find
	 * the maximum layer number that is selected.
	 */
	num   = (*query->numLayers) ();
	fg    = (*query->layerMask) (OPLYR_FG);
	empty = (*query->layerMask) (OPLYR_EMPTY);

	DPRT ((debugPort, "num %d, fg %d, empty %d\n", num, fg, empty));

	max = -1;
	for (i = 0; i < num; i++)
		if ((fg & (1 << i)) && i > max)
			max = i;

	DPRT ((debugPort, "max current layer %d\n", max));

	/*
	 * Starting from the last layer selected and scanning through the
	 * rest of the layers (wrapping around to the start layers), stop
	 * on the first empty layer that is not selected.
	 */
	lset = 0;
	for (i = 0; i < num - 1; i++) {
		l = 1 << ((max + i + 1) % num);
		if ((fg & l) == 0 && (empty & l)) {
			lset = l;
			break;
		}
	}

	/*
	 * Send the "setlayer" command with the chosen layer as argument.
	 * The selected data for the command is irrelevant since nothing
	 * will change except display state.
	 */
	if (lset) {
		DPRT ((debugPort, "Next empty layer set (mask=%d).\n", lset));
		dval.type = DY_LAYERS;
		dval.intv.value = lset;
		(*local->execute) (local->data, ccSetLayer, 1,
				   &dval, OPSEL_GLOBAL, NULL);
	} else
		DPRT ((debugPort, "No next empty layer found.\n"));

	DFLUSH;
	return AFUNC_OK;
}


/*
 * Attempt to add layer number k to the BG layer mask.  Ignore this layer
 * if it is out of range, if it is not empty, if it is currently displayed
 * or if it is already in the BG mask.  If the layer can be added, also
 * attempt to add the adjacent layers recursively.
 */
	static unsigned int
AddBGLayer (
	unsigned int		 bg,
	unsigned int		 fg,
	unsigned int		 empty,
	int			 num,
	int			 k)
{
	unsigned int		 l;

	if (k < 0 || k >= num)
		return bg;

	l = 1 << k;
	if ((l & empty) || (l & fg) || (l & bg))
		return bg;

	bg = bg | l;
	bg = AddBGLayer (bg, fg, empty, num, k - 1);
	return AddBGLayer (bg, fg, empty, num, k + 1);
}


/*
 * Select all background layers near currently selected ones.
 */
	XCALL_(static int)
AllBG (
	long			 version,
	GlobalFunc		*global,
	ModCommand		*local,
	FILE			*debugPort)
{
	StateQueryFuncs		*query;
	unsigned int		 empty, fg, bg;
	CommandCode		 ccSetBLayer;
	DynaValue		 dval;
	int			 num, i;

	/*
	 * Standard version check.
	 */
	XCALL_INIT;
	if (version != 1)
		return AFUNC_BADVERSION;

	/*
	 * Get the state query functions and the command code for the
	 * set bg layer command.
	 */
	query = (*global) ("LWM: State Query", GFUSE_TRANSIENT);
	if (!query)
		return AFUNC_BADGLOBAL;

	ccSetBLayer = (*local->lookup) (local->data, "setblayer");
	if (!ccSetBLayer)
		return AFUNC_BADLOCAL;

	DPRT ((debugPort, "All BG Layers...\n"));

	/*
	 * Get the total number of layers and the mask for the empty layers
	 * and current layers.
	 */
	num   = (*query->numLayers) ();
	fg    = (*query->layerMask) (OPLYR_FG);
	empty = (*query->layerMask) (OPLYR_EMPTY);

	DPRT ((debugPort, "num %d, fg %d, empty %d\n", num, fg, empty));

	/*
	 * Recursively add all layers adjacent to fg layers to the background
	 * set.  Non-empty layers will be ignored.
	 */
	bg = 0;
	for (i = 0; i < num; i++) {
		if ((fg & (1 << i)) == 0)
			continue;

		bg = AddBGLayer (bg, fg, empty, num, i - 1);
		bg = AddBGLayer (bg, fg, empty, num, i + 1);
	}

	/*
	 * If we get a valid set of BG layers, set the background.
	 */
	if (bg) {
		DPRT ((debugPort, "setting new background %d\n", bg));

		dval.type = DY_LAYERS;
		dval.intv.value = bg;
		(*local->execute) (local->data, ccSetBLayer, 1,
				   &dval, OPSEL_GLOBAL, NULL);
	}

	DFLUSH;
	return AFUNC_OK;
}


/*
 * Startup and shutdown are called to load and unload this plugin module.
 * The return value from Startup is passed to the activation functions as
 * serverData, and must be non-null if the load was successful.  Since
 * we're only opening the debug window, these functions are optional.
 */
#ifdef DEBUG
	void *
Startup (void)
{
	FILE			*debugPort;

	debugPort = fopen ("con:10/10/300/180/XYZDebug", "w+");
	if (debugPort)
		fprintf (debugPort, "Layer-Set Server\n");

	return debugPort;
}

	void
Shutdown (
	FILE			*debugPort)
{
	fclose (debugPort);
}
#endif


/*
 * ServerDesc records for each entry point in this multi-server module.
 */
const char		class[] = "CommandSequence";
ServerRecord		ServerDesc[] = {
	{ class, "Demo_NextEmptyLayer",		NextEmpty },
	{ class, "Demo_AllBGLayers",		AllBG },
	{ NULL }
};
