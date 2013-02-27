/*
 * SUPERBLOTCH.C --  Layout Plugin Shader
 *	Applies a gradient to a surface
 *
 * Code based on 'Blotch', uses preview and gradient services.
 */
#include <splug.h>
#include <moni.h>
#include <lwran.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include	<lwpanel.h>
#include	<preview.h>
#include	<gradient.h>

// preview window resolution
#define		PREVIEW_RX	256
#define		PREVIEW_RY	200

/*
 * HANDLER -- Instance Methods
 *
 * An instance is a particular set of user-set options for a plug-in
 * operation.  The options for a blotch are the color tint, the center
 * and radius, and a number for the softness of the blotch edges.
 */
typedef struct st_Blotch {
	LWGradient	*grad;
	prevBuffer	*prev;
	LWTime		time;
	char		 desc[32];

	LWPanelFuncs*	Gpan_funcs;
	LWPanelID		pan_id;
	GlobalFunc*		global;
} Blotch;


static	LWGradientHandler	*gradient_handler;
static	PrevBufferHandler	*preview_handler;
static	LWSceneInfo			*scene_info;
static	MessageFuncs		*message;
static	GlobalFunc			*gf = NULL;

// This is the gradient descriptor:
// you can modify this to create your own gradient
static	gradInterfaceData	shaderGradData = {"Shader gradient"
									,{{"Time",0.0f,1.0f}
									,{"Position(y)",0.0f,1.0f}
									,{"Slope",0.0f,90.0f}
									,{"Cosine",0.0f,90.0f}	
									,{NULL,0,0}} };
#define	TIME	0
#define	YPOS	1
#define	SLOPE	2
#define	COSINE	3

#define	RAD_TO_DEG	57.2958 //  = 180/PI

#define NORM(v)		{double length, norm;				\
			 length = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);	\
			 norm = 1.0 / length;				\
			 v[0] *= norm;					\
			 v[1] *= norm;					\
			 v[2] *= norm; }

// required by macros in lwpanel.h
static LWPanControlDesc   desc;								
static LWValue ival={LWT_INTEGER},ivecval={LWT_VINT},
  fval={LWT_FLOAT},fvecval={LWT_VFLOAT},sval={LWT_STRING};

// Function prototypes
// -------------------------------------------------------------- //

void	my_evaluate(Blotch		*inst,ShaderAccess		*sa);
void	gradientCallBack(LWControl	*ctl,void	*data);
void	makePreview(Blotch			*inst);
void	userDraw(LWControl	*ctl,void	*data);
 
// -------------------------------------------------------------- //

/*
 * The create function allocates a blotch struct and returns its
 * pointer as the instance.  Note that "Blotch *" is used throughout
 * instead of "LWInstance".  This works since a LWInstance type is
 * a generic pointer and can be safely replaced with any specific
 * pointer type.  Instance variables are initialized to some default
 * values.
 */
	XCALL_(static LWInstance)
Create (
	LWError			*err)
{
	Blotch			*inst;
	short			type;
	int				x,y;
	gParamDesc		*pDesc;
	unsigned char	rgb[3];

	XCALL_INIT;

	inst = malloc (sizeof (Blotch));
	if (!inst) {
		*err = "Memory allocation problem!";
		return NULL;
	}

	if (gf)
		inst->global = gf;	// save global function pointer
	else
		return NULL;

	type = LWGRADIENT_COLOR;		// we want a color gradient	
	pDesc = shaderGradData.pDesc;	// the descriptor must be passed at creation
	inst->grad = (*gradient_handler->create)(0,type,pDesc);

	// to create the buffer we must specify the resolution and the type of data buffers we want
	inst->prev = (*preview_handler->createPrev)(PREVIEW_RX,PREVIEW_RY,LWPBF_SHADER);

	// comment this if you want to have access to all layers in preview UI
	for (y=1;y<inst->prev->nLayers;y++)
		inst->prev->layerList[y].desc.visible = 0;

	rgb[0]=0;	rgb[1]=0;	rgb[2]=0;
	for (y=0;y<PREVIEW_RY;y++)
		for (x=0;x<PREVIEW_RY;x++) 
			(*preview_handler->renderPixelOffscreen)(inst->prev, x,y, rgb);
	
	inst->time = 0;

	return inst;
}


/*
 * Destroy frees the blotch instance memory.
 */
	XCALL_(static void)
Destroy (
	Blotch			*inst)
{
	XCALL_INIT;

	if (inst->prev)
		(*preview_handler->destroy)(inst->prev);
	if (inst->grad)
		(*gradient_handler->destroy)(inst->grad);

	free (inst);
	(*gradient_handler->freeHandler)();
}


/*
 * Copy simply moves the data settings from one blotch struct to
 * another.
 */
	XCALL_(static LWError)
Copy (
	Blotch			*from,
	Blotch			*to)
{
	XCALL_INIT;

	//just copy the gradient
	if (to->grad)
		(*gradient_handler->copy)(from->grad,to->grad);

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

	sprintf (inst->desc, "Gradient shader");
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
	static void
Swap4 (
	char                    *d,
	int                      n)
{
	char                     t;

	while (n--) {
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
	static void
SaveValue (
	const LWSaveState	*sState,
	double			 dval)
{
	if (sState->ioMode == LWIO_OBJECT) {
		float		 val = (float) dval;

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


	static double
LoadValue (
	const LWLoadState	*lState)
{
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
	XCALL_(static LWError)
Save (
	Blotch			*inst,
	const LWSaveState	*sState)
{
	XCALL_INIT;

	if (!inst->grad)
		SaveValue(sState,0);	// gradient off
	else
	{
		SaveValue(sState,1);	// gradient on
		(gradient_handler->save_lw)(inst->grad,sState);	
	}

	return (NULL);
}

	XCALL_(static LWError)
Load (
	Blotch			*inst,
	const LWLoadState	*lState)
{
	int		on;

	XCALL_INIT;

	on = (short) LoadValue(lState); // gradient flag
	if (on && inst->grad)
	{
		(gradient_handler->load_lw)(inst->grad,lState);	
	}

	return (NULL);
}


/*
 * INIT and CLEANUP
 *
 * The host calls these callbacks for each shader instance just before
 * and just after rendering.  This is the place to do any precalculation
 * to speed up the rendering of an instance.
 */
	XCALL_(static LWError)
Init (
	Blotch			*inst)
{
	XCALL_INIT;

	// update the scene information we have
	// things may have changed like frameWidth and frameHeight
	scene_info = (*inst->global) ("LW Scene Info", GFUSE_TRANSIENT);
	if (!scene_info )
		return "Bad Global";

	// the preview must be initialised here to make sure it matches the screen resolution.
	(*preview_handler->init)(inst->prev,inst->time,scene_info->frameWidth,scene_info->frameHeight);
	return (NULL);
}

	XCALL_(static void)
Cleanup (
	Blotch			*inst)
{
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
	XCALL_(static LWError)
NewTime (
	Blotch			*inst,
	LWFrame			 f,
	LWTime			 t)
{
	XCALL_INIT;
	
	inst->time = t;
	return (NULL);
}


/*
 * FLAGS
 *
 * The host calls the flags function to find out some information about
 * this shader instance.  Since all blotches will alter the color of
 * a surface but nothing else, the shader color bit is returned.
 */
	XCALL_(static unsigned int)
Flags (
	Blotch			*inst)
{
	XCALL_INIT;
	// we are just changing color
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
 * The gradient is called here. It modifies the color value given the input parameter
 */
	XCALL_(static void)
Evaluate (
	Blotch			*inst,
	ShaderAccess		*sa)
{
	prevBufferAccess	pba;

	XCALL_INIT;

	// first save the shader structure for the preview	
	pba.sx = sa->sx;
	pba.sy = sa->sy;	
	pba.surfID = (short)inst;	// convert the instance pointer to a short to get a surf ID
	pba.color[0] = (float) sa->color[0];
	pba.color[1] = (float) sa->color[1];
	pba.color[2] = (float) sa->color[2];
	pba.pos[0] = (float) sa->oPos[0];
	pba.pos[1] = (float) sa->oPos[1];
	pba.pos[2] = (float) sa->oPos[2];
	pba.norm[0] = (float) sa->wNorm[0];
	pba.norm[1] = (float) sa->wNorm[1];
	pba.norm[2] = (float) sa->wNorm[2];
	pba.lum = (float) sa->luminous;
	pba.dif = (float) sa->diffuse;
	pba.spe = (float) sa->specular;
	pba.ref = (float) sa->mirror;
	pba.tra = (float) sa->transparency;
	pba.eta = (float) sa->eta;
	pba.rough = (float) sa->roughness;
	pba.rayLength = (float) sa->rayLength;

	(*preview_handler->savePreviewSt)(inst->prev, &pba, LWPBF_SHADER);

	// second, evaluate the gradient passing the correct parameter
	my_evaluate(inst,sa);
}

XCALL_(static void)
my_evaluate(Blotch		*inst,ShaderAccess		*sa)
{
	double	param;

	// Evaluate the gradient passing the correct parameter
	switch (inst->grad->paramNb)
	{
	case TIME:
		param = inst->time;
		break;
	case SLOPE:
		param = acos(sa->wNorm[1])*RAD_TO_DEG;
		break;
	case YPOS:
		param = sa->oPos[1];
		break;
	case COSINE:
		param = acos(sa->cosine)*RAD_TO_DEG;
		break;
	default:
		param = 0;
	}

	// the gradient returns the value in sa->color[]
	(gradient_handler->evaluate)(inst->grad,param,sa->color);
}

/*
 * ACTIVATE
 *
 * The activation function for shader handlers just fills in the shader
 * handler fields with callbacks for this blotch shader.  This accepts
 * both version 1 and version 2 handlers, but it only sets the descln
 * function for version 2.
 */
	XCALL_(static int)
BlotchActivate (
	long			 version,
	GlobalFunc		*global,
	ShaderHandler	*local,
	void			*serverData)
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

	message = (*global) ("Info Messages", GFUSE_TRANSIENT);
	if (!message)
		return (AFUNC_BADGLOBAL);

	gradient_handler = (*global) (GRADIENT_ACCESS_NAME, GFUSE_TRANSIENT);
	if (!gradient_handler)
	{
		(*message->error)("Gradient services could not be found","Please add 'Gradient.p'");
		return AFUNC_BADGLOBAL;	
	}

	preview_handler = (*global) (PREVIEW_ACCESS_NAME, GFUSE_TRANSIENT);
	if (!preview_handler)
	{
		(*message->error)("Preview services could not be found","Please add 'Preview.p'");
		return AFUNC_BADGLOBAL;	
	}

	scene_info = (*global) ("LW Scene Info", GFUSE_TRANSIENT);
	if (!scene_info )
		return AFUNC_BADGLOBAL;
	

	gf = global;	

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
	XCALL_(static int)
Interface (
	long			 version,
	GlobalFunc		*global,
	Blotch			*inst,
	void			*serverData)
{
	LWControl 		*gradient,*preview;

	XCALL_INIT;

	// get panel funcs
	inst->Gpan_funcs = (LWPanelFuncs*)(*global) (PANEL_SERVICES_NAME, GFUSE_TRANSIENT);
	// create panel and save its id in instance data
	inst->pan_id= (*inst->Gpan_funcs->create) ("SuperBlotch",inst->Gpan_funcs);

	// now create the controls
	gradient = BUTTON_CTL(inst->Gpan_funcs, inst->pan_id,"Color gradient");	
	preview  = PREV_CTL(preview_handler,inst->prev,inst->pan_id);

	// set userdata and  callbacks
	(*inst->Gpan_funcs->set)(inst->pan_id,PAN_USERDATA,(void *)inst);
	(*inst->Gpan_funcs->set)(inst->pan_id,PAN_USERDRAW,(void *)userDraw);
	CON_SETEVENT(gradient, (int)gradientCallBack,(int)inst);

    // Finalise the preview window control (places the controls, etc.)
   (*preview_handler->finalizePrev)(inst->prev);
	makePreview(inst);

	// open the panel
	if((*inst->Gpan_funcs->open)(inst->pan_id,PANF_BLOCKING|PANF_FRAME|PANF_CANCEL))	
	{
		// do something
	}

	(*inst->Gpan_funcs->destroy)(inst->pan_id);	   

	return (AFUNC_OK);
}

void	gradientCallBack(LWControl	*ctl,void	*data)
{
	Blotch	*inst = (Blotch *) data;

	(*gradient_handler->Interface)(inst->grad,"Color gradient");
	makePreview(inst);
}

void	userDraw(LWControl	*ctl,void	*data)
{
	Blotch	*inst = (Blotch *) data;
	// blit preview to screen.. this is the place where preview is actually displayed
	(*preview_handler->blitToScreen)(inst->prev);
}

void	makePreview(Blotch			*inst) 
{
	int					x, y;
	unsigned char		rgb[3];
	double				v[3];
	ShaderAccess		sa;
	prevBufferAccess	pba;
	
	// check that preview has been initialised....
	if (!inst->prev->initialised)
	{
		(*message->error) ("Preview buffer unavailable !","Please return to layout and make a test render");
		(*inst->Gpan_funcs->set)(inst->pan_id, PAN_TO_FRONT, inst);
		return;
	}

	// render preview
	for (x=0;x<inst->prev->rx;x++)
	{
		for (y=0;y<inst->prev->ry;y++)
		{
			// get shader access data saved into the buffer:
			// fill in the sx and sy value of pba, then get data from buffer
			pba.sx = x;	pba.sy = y;
			(*preview_handler->getPreviewSt)(inst->prev,&pba,LWPBF_SHADER);
			if (pba.surfID == (short) inst)
			{
				// translate to shader access struct
				sa.sx = x;	sa.sy = y;
				sa.oPos[0] = pba.pos[0];
				sa.oPos[1] = pba.pos[1];
				sa.oPos[2] = pba.pos[2];
				sa.wNorm[0] = pba.norm[0];
				sa.wNorm[1] = pba.norm[1];
				sa.wNorm[2] = pba.norm[2];
				// re-evaluate the cosine  = v.n (not saved in buffer)
				v[0] = -sa.oPos[0] + inst->prev->raySource[0];
				v[1] = -sa.oPos[1] + inst->prev->raySource[1];
				v[2] = -sa.oPos[2] + inst->prev->raySource[2];
				NORM(v);
				sa.cosine = v[0]*sa.wNorm[0] + v[1]*sa.wNorm[1] + v[2]*sa.wNorm[2];

				// call the evaluate function
				my_evaluate(inst,&sa);
				rgb[0] = (unsigned char) (255*sa.color[0]);
				rgb[1] = (unsigned char) (255*sa.color[1]);
				rgb[2] = (unsigned char) (255*sa.color[2]);
				// render pixel off screen
				(*preview_handler->renderPixelOffscreen)(inst->prev,x,y,rgb);
			}
		}
	}
}

/*
 * This plug-in module contains two servers -- one for the shader handler
 * and one for the interface.  The blotch instance will be passed to
 * this interface server since it has the same name as the handler.  These
 * servers do not have to be in the same module if a render-only shader
 * plug-in is required.
 */
ServerRecord ServerDesc[] = {
	{ "ShaderHandler",	"SuperBlotch",	BlotchActivate },
	{ "ShaderInterface","SuperBlotch",	Interface },
	{ NULL }
};
