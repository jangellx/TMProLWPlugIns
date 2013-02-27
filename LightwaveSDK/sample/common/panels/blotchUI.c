// BlotchUI.c -- LW Panel Services example
// by Arnie Cachelin Copyright 1996 NewTek, inc.

#include <splug.h>
#include <lwbase.h>
#include <lwran.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lwpanel.h"
#include "gui_help.h"

static LWPanControlDesc   desc;	 							// required by macros in lwpanel.h
static LWValue ival={LWT_INTEGER},ivecval={LWT_VINT},		// required by macros in lwpanel.h
  fval={LWT_FLOAT},fvecval={LWT_VFLOAT},sval={LWT_STRING};

// Blotch instance data (from blotch.c)
typedef struct st_Blotch {
	double		 color[3];
	double		 center[3];
	double		 radius;
	double		 softness;
	double		 r2, piOverR;
	char		   desc[20];
} Blotch;

XCALL_(int)BlotchInterface (
	long			 version,
	GlobalFunc		*global,
	Blotch			*inst,
	void			*serverData)
{
	LWPanelFuncs	*panl;
	LWPanelID		panID;
	LWControl		*col,*cen,*sof,*rad;
	MessageFuncs	*message;
	int	x=200,y=0,w=400,h=300;

	XCALL_INIT;
	if (version != 1)
		return AFUNC_BADVERSION;
	message = (*global) ("Info Messages", GFUSE_TRANSIENT);
	if (!message )
		return AFUNC_BADGLOBAL;
	
panl = (*global) (PANEL_SERVICES_NAME, GFUSE_TRANSIENT);
	if(!panl)
	{
		(*message->error)("Unable to activate global "PANEL_SERVICES_NAME, "     please add plugin lwpanels.p" );
		return AFUNC_BADGLOBAL;
	}

	if( panID=PAN_CREATE(panl,"Blotch") )
	{
		LWValue	val = {LWT_INTEGER}; 
		int lw;
#ifdef RGB
		int rgb[3];
		if(!(col = MINIRGB_CTL(panl,panID,"Color"))) 
			goto controlError;
#else
		if(!(col = MINIHSV_CTL(panl,panID,"Color"))) 
			goto controlError;
#endif
		if(!(cen = FVEC_CTL(panl,panID,"Center"))) 
			goto controlError;
		if(!(rad = FLOAT_CTL(panl,panID,"Radius"))) 
			goto controlError;
		if(!(sof = FLOAT_CTL(panl,panID,"Softness"))) 
			goto controlError;
		lw = maxLabel(panl,panID);
		ctlVAlign(lw,col);
		ctlVAlign(lw,cen);
		ctlVAlign(lw,rad);
		ctlOneLine(rad,sof,NULL,NULL,NULL,NULL);
#ifdef RGB
		rgb[0] = (int)(255.0*inst->color[0]);
		rgb[1] = (int)(255.0*inst->color[1]);
		rgb[2] = (int)(255.0*inst->color[2]);
		SETV_IVEC(col,rgb);
#else
		SETV_FVEC(col,inst->color);
#endif
		SETV_FVEC(cen,inst->center);
		SET_FLOAT(rad,inst->radius);
		SET_FLOAT(sof,inst->softness);

		if(PAN_POST(panl,panID))
		{
#ifdef RGB
			GETV_IVEC(col,rgb); 
			inst->color[0] = ((double)rgb[0])/255.0;
			inst->color[1] = ((double)rgb[1])/255.0;
			inst->color[2] = ((double)rgb[2])/255.0;
#else
			GETV_FVEC(col,inst->color);
#endif
			GETV_FVEC(cen,inst->center);
			GET_FLOAT(rad,inst->radius);
			GET_FLOAT(sof,inst->softness);
		}
		PAN_KILL(panl,panID);
		return (AFUNC_OK);
controlError:
		PAN_KILL(panl,panID);
		(*message->error)("Duoooh!:","Unable to create panel");
	}
	else 
	{
		(*message->error)("Duoooh!:","Unable to create panel");
	}
	return AFUNC_BADGLOBAL;
}
