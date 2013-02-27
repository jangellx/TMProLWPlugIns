/*
 * BLOTCH.C --  Layout Plugin Shader
 *		Stick a colored spot on a surface
 *
 * by Allen Hastings and Arnie Cachelin
 * revised by Stuart Ferguson
 * last revision  2/27/96
 */
#include <splug.h>
#include <moni.h>
#include <lwran.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>



/*
 * HANDLER -- Instance Methods
 *
 * An instance is a particular set of user-set options for a plug-in
 * operation.  The options for a blotch are the color tint, the center
 * and radius, and a number for the softness of the blotch edges.
 */
typedef struct st_Blotch {
	double		 color[3];
	double		 center[3];
	double		 radius;
	double		 softness;

	double		 r2;
  double     piOverR;

	char		   desc[20];
} Blotch;


/*
 * The create function allocates a blotch struct and returns its
 * pointer as the instance.  Note that "Blotch *" is used throughout
 * instead of "LWInstance".  This works since a LWInstance type is
 * a generic pointer and can be safely replaced with any specific
 * pointer type.  Instance variables are initialized to some default
 * values.
 */
XCALL_(static LWInstance) Create ( LWError *err) {
	Blotch			*inst;

	XCALL_INIT;

	inst = malloc (sizeof (Blotch));
	if (!inst) {
		*err = "No memory for new blotch shader.";
		return NULL;
	}

	inst->color[0] = 0.4;
	inst->color[1] = 0.0;
	inst->color[2] = 0.8;

	inst->center[0] = inst->center[1] = inst->center[2] = 0.0;
	inst->radius    = 1.0;
	inst->softness  = 0.5;

	return inst;
}


/*
 * Destroy frees the blotch instance memory.
 */
XCALL_(static void) Destroy ( Blotch *inst) {
	XCALL_INIT;
	free (inst);
}


/*
 * Copy simply moves the data settings from one blotch struct to
 * another.
 */
XCALL_(static LWError) Copy (
	Blotch			*from,
	Blotch			*to)
{
	XCALL_INIT;

	*to = *from;
	return (NULL);
}


/*
 * DescLN returns a single line which describes the instance.  In this
 * case it just returns the color as a triple from 0-255 to make the
 * string short.  Since the string must persist, it is part of the
 * instance.
 */
	XCALL_(static const char *)
DescLn (
	Blotch			*inst)
{
	XCALL_INIT;

	sprintf (inst->desc, "%3d %3d %3d",
		 (int)(inst->color[0] * 255.0),
		 (int)(inst->color[1] * 255.0),
		 (int)(inst->color[2] * 255.0));

	return inst->desc;
}


/*
 * SAVE and LOAD
 *
 * Any instance can theoretically be written to either scenes or objects,
 * so this code deals with both cases, although in practice shaders are
 * really only written to objects.
 */


/*
 * The SwapM4 function (or macro) takes a pointer to a long value in
 * Motorola byte-order and swaps in place into local byte order.  Likewise,
 * it takes a value in local order and swaps to Motorola order.  This only
 * does any swapping at all on Windows systems.
 */
#ifdef _WIN32
static void Swap4 ( char *d, int n) {
	char t;

	while( n-- ) {
		t = d[0];
		d[0] = d[3];
		d[3] = t;

		t = d[1];
		d[1] = d[2];
		d[2] = t;

		d += 4;
	}
}

#define SwapM4(p)	Swap4 ((char *)p, 1)
#else
#define SwapM4(p)
#endif


/*
 * This pair of routines saves and loads a single-precision floating-point
 * value given a context.  On a binary file it is saved as four bytes with
 * optional swapping to Motorola order.  On a text file it is saved as a
 * simple string.
 */
static void SaveValue( const LWSaveState *sState, double dval ) {
	if (sState->ioMode == LWIO_OBJECT) {
		float		 val = dval;

		/*
		 * Binary data can be saved directly, but only after we
		 * have done appropriate swapping.
		 */
		SwapM4 (&val);
		(*sState->write) (sState->writeData, (char *) &val, 4);
	} else {
		/*
		 * Text data can be saved only after conversion.  Each
		 * number will be stored on a separate line.
		 */
		char		 buf[50];

		sprintf (buf, " %f", dval);
		(*sState->write) (sState->writeData, buf, strlen (buf) + 1);
	}
}


static double LoadValue ( const LWLoadState	*lState) {
	if (lState->ioMode == LWIO_OBJECT) {
		float		 val;

		(*lState->read) (lState->readData, (char *) &val, 4);
		SwapM4 (&val);
		return (double) val;
	} else {
		char		 buf[50];
		double		 val;

		(*lState->read) (lState->readData, buf, 50);
		sscanf (buf, "%f", &val);
		return val;
	}
}


/*
 * The main save and load functions just use the above functions to
 * dump their parameters and re-load them.  Ideally there would be
 * some sort of versioning for later changes to the disk format.
 */
XCALL_(static LWError) Save ( Blotch *inst, const LWSaveState *sState) {
	XCALL_INIT;

	SaveValue (sState, inst->color[0]);
	SaveValue (sState, inst->color[1]);
	SaveValue (sState, inst->color[2]);
	SaveValue (sState, inst->center[0]);
	SaveValue (sState, inst->center[1]);
	SaveValue (sState, inst->center[2]);
	SaveValue (sState, inst->radius);
	SaveValue (sState, inst->softness);

	return (NULL);
}

XCALL_(static LWError) Load ( Blotch *inst, const LWLoadState *lState) {
	XCALL_INIT;

	inst->color[0]  = LoadValue (lState);
	inst->color[1]  = LoadValue (lState);
	inst->color[2]  = LoadValue (lState);
	inst->center[0] = LoadValue (lState);
	inst->center[1] = LoadValue (lState);
	inst->center[2] = LoadValue (lState);
	inst->radius    = LoadValue (lState);
	inst->softness  = LoadValue (lState);

	return (NULL);
}


/*
 * INIT and CLEANUP
 *
 * The host calls these callbacks for each shader instance just before
 * and just after rendering.  This is the place to do any precalculation
 * to speed up the rendering of an instance.
 */
XCALL_(static LWError) Init ( Blotch *inst) {
	XCALL_INIT;

	inst->r2      = inst->radius * inst->radius;
	inst->piOverR = 3.1416 / inst->radius;

	return (NULL);
}

XCALL_(static void) Cleanup ( Blotch *inst) {
	XCALL_INIT;
}


/*
 * NEWTIME
 *
 * The host calls this callback whenever a different timestep will be
 * being rendered.  It is called once at the start of a frame and more
 * times during the frame if the frame requires includes multiple
 * times (like in motion blur or field-rendering).
 */
XCALL_(static LWError) NewTime ( Blotch *inst, LWFrame f, LWTime t) {
	XCALL_INIT;
	return (NULL);
}


/*
 * FLAGS
 *
 * The host calls the flags function to find out some information about
 * this shader instance.  Since all blotches will alter the color of
 * a surface but nothing else, the shader color bit is returned.
 */
XCALL_(static unsigned int) Flags ( Blotch *inst) {
	XCALL_INIT;
	return (LWSHF_COLOR);
}


/*
 * EVALUATE
 *
 * This is the guts of the shader.  It is called for each spot that needs
 * to be colored with this shader in the final image.  The shaderAccess
 * struct contains info about the spot and some values may be modified
 * by the shader according to what bits were returned in the flags
 * callback.
 *
 * The blotch shader will compute the distance of this point to the center
 * of the blotch and blend some of the blotch color with the color already
 * computed for that spot.
 */
XCALL_(static void) Evaluate( Blotch *inst, ShaderAccess *sa) {
	double	 d;
  double   r2;
  double   a;
	int			 i;

	XCALL_INIT;

	/*
	 * Compute the distance from the center of the blotch to the spot
	 * in object coordinates.  Exit early if the spot is clearly
	 * outside the blotch radius.
	 */
	r2 = 0;
	for( i = 0; i < 3; i++ ) {
		d = sa->oPos[i] - inst->center[i];
		d = d * d;
		if (d > inst->r2)
			return;

		r2 += d;
	}

	if (r2 > inst->r2)
		return;

	d = sqrt (r2);

	/*
	 * Using the distance in 'd', compute where this spot falls in
	 * blotch's soft edge.  The blotch is given by a cosine density
	 * function scaled by the softness factor.  Where the density is
	 * greater than 1.0, it clips.
	 */
	d = cos (d * inst->piOverR) * inst->softness;
	if (d > 1.0)
		d = 1.0;

	/*
	 * Finally, blend the blotch color into the existing color using
	 * the computed density.
	 */
	a = 1.0 - d;
	for (i = 0; i < 3; i++)
		sa->color[i] = sa->color[i] * a + inst->color[i] * d;
}



/*
 * ACTIVATE
 *
 * The activation function for shader handlers just fills in the shader
 * handler fields with callbacks for this blotch shader.
 */
XCALL_(static int) BlotchActivate ( long version, GlobalFunc *global,
                                    ShaderHandler *local, void *serverData)
{
	XCALL_INIT;
	if (version < 1 || version > 2)
		return (AFUNC_BADVERSION);

	local->create   = Create;
	local->destroy  = Destroy;
	local->load     = Load;
	local->save     = Save;
	local->copy     = Copy;
	if (version > 1)
		local->descln = DescLn;

	local->init     = Init;
	local->cleanup  = Cleanup;
	local->newTime  = NewTime;
	local->evaluate = Evaluate;
	local->flags    = Flags;

	return (AFUNC_OK);
}



/*
 * INTERFACE
 *
 * The interface function for a shader is actually the activation function
 * of the ShaderInterface class server for this instance type.  The instance
 * to be edited is passed as the local data pointer to the interface
 * activation function.  This function allows the user to edit the instance.
 */
XCALL_(static int) Interface ( long version, GlobalFunc *global,
                               Blotch *inst, void *serverData) {
	MessageFuncs		*msg;
	char			       s1[100];
  char             s2[100];

	XCALL_INIT;
	if (version != 1)
		return (AFUNC_BADVERSION);

	msg = (*global) ("Info Messages", GFUSE_TRANSIENT);
	if (!msg)
		return (AFUNC_BADGLOBAL);

	/*
	 * Since a full user interface is beyond the scope of this sample
	 * plug-in, this function will just display the current settings
	 * of the shader.
	 */
	sprintf (s1, "BLOTCH: center (%f, %f, %f), radius %f",
		 inst->center[0], inst->center[1], inst->center[2],
		 inst->radius);
	sprintf (s2, "color (%5.3f, %5.3f, %5.3f), softness %f",
		 inst->color[0], inst->color[1], inst->color[2],
		 inst->softness);
	(*msg->info) (s1, s2);

	return (AFUNC_OK);
}



/*
 * This plug-in module contains two servers -- one for the shader handler
 * and one for the interface.  The blotch instance will be passed to
 * this interface server since it has the same name as the handler.  These
 * servers do not have to be in the same module if a render-only shader
 * plug-in is required.
 */
ServerRecord ServerDesc[] = {
	{ "ShaderHandler",    "Demo_Blotch",	BlotchActivate },
	{ "ShaderInterface",  "Demo_Blotch",	Interface },
	{ NULL }
};
