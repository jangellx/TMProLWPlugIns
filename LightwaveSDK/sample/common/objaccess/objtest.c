// ObjTest.c -- ObjectAccess  AND LW Panel Services example.
// By Arnie Cachelin Copyright 1996 NewTek, Inc.

#include <splug.h>
#include <lwbase.h>
#include <lwran.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lwpanel.h"
#include "gui_help.h"
#include "lwobjacc.h"

static LWPanControlDesc   desc;  /* required by macros in lwpanel.h */
static LWValue ival={LWT_INTEGER},ivecval={LWT_VINT},       /* required by macros in lwpanel.h */
  fval={LWT_FLOAT},fvecval={LWT_VFLOAT},sval={LWT_STRING};
/*
static DyReqControlDesc   desc;	 // required by macros in lwpanel.h
static DynaValue ival={DY_INTEGER},ivecval={DY_VINT},		// required by macros in lwpanel.h
  fval={DY_FLOAT},fvecval={DY_VFLOAT},sval={DY_STRING};
*/
typedef struct st_OListData {
	LWObjectInfo	*obinfo;
	ObjectAccess	*oa;
	LWControl 		*obj,*pnts,*polys,*srfs,*min,*max,*rad,*snam,*ssiz;
} OListData;

#define 	MAX_SURFNAME	54
char srNames[15][MAX_SURFNAME];
char *SurfList[16] = {"<          none          >",NULL};
int SurfSize[16] = {0};

// just update size when current surface is changed
void	slistEvent(LWControl *obj, OListData *od)
{
	int i;
	GET_INT(od->snam,i);
	SET_INT(od->ssiz,SurfSize[i]);
}

// when object is changed, load object, update display and surface strings, free object  
void	olistEvent(LWControl *obj, OListData *od)
{
	LWItemID	id;
	const char *name;
	LWObjectID	lwo;
	LWSurfaceID	sur;
	double	vec[3]={0.0,0.0,0.0},box[6];
	int srfs,i;
	GET_INT(od->obj,(int)id);
	SET_INT(od->snam,0);
	SET_INT(od->ssiz,0);
	if(id)
	{
		name = (*od->obinfo->filename)(id);
		if(name && (lwo = (*od->oa->create)(name)) )
		{
			srfs=(*od->oa->surfCount)(lwo);
			SET_INT(od->srfs,srfs);
			for(i=0; i<srfs && i<14; i++)
			{		  
				sur = (*od->oa->surfGet)(lwo,i);
				if(sur)
				{
					strncpy(srNames[i],sur->name,MAX_SURFNAME-1);
					SurfList[i+1] = srNames[i];		 // Note: SurfList[0] == " none " ALWAYS ==> fill SurfList[i+1]
					SurfSize[i+1] = sur->size;
				}
			}				  
			SurfList[i+1] = NULL;
			RENDER_CON(od->snam);  // may not be necessary
			SET_FLOAT(od->rad,(*od->oa->boundBox)(lwo,box));
			vec[0] = box[0]; vec[1] = box[2]; vec[2] = box[4];
			SETV_FVEC(od->min,vec);
			vec[0] = box[1]; vec[1] = box[3]; vec[2] = box[5];
			SETV_FVEC(od->max,vec);
			SET_INT(od->pnts,(*od->oa->pointCount)(lwo));
			SET_INT(od->polys,(*od->oa->polyCount)(lwo));
			(*od->oa->destroy)(lwo);	  // probably should cache loaded objects, rather than create&free on each event
			return;
		}
	}
	else
		SurfList[1] = NULL;
	SETV_FVEC(od->min,vec);
	SETV_FVEC(od->max,vec);
	SET_FLOAT(od->rad,0.0);
	SET_INT(od->pnts,0);
	SET_INT(od->polys,0);
	SET_INT(od->srfs,0);
}

XCALL_(int) ObjectList (
	long			 version,
	GlobalFunc		*global,
	void			*inst,
	void			*serverData)
{
	LWPanelFuncs	*panl;
	LWPanelID		panID;
	MessageFuncs	*message;
	OListData		od;
	char 			*hail = "Object List";

	XCALL_INIT;	 
	message = (*global) ("Info Messages", GFUSE_TRANSIENT);
	if (!message )
		return AFUNC_BADGLOBAL;
	if(!(od.obinfo=(*global)("LW Object Info",GFUSE_TRANSIENT)))
	{
		(*message->error)("Duoooh!:","Unable to get 'LW Object Info'!");
		return AFUNC_BADGLOBAL;
	}	  
	od.oa = (*global) (OBJECT_ACCESS_NAME, GFUSE_TRANSIENT);
	if(!od.oa)
	{
		(*message->error)("Duoooh!:","Unable to activate global '"OBJECT_ACCESS_NAME"'!");
		return AFUNC_BADGLOBAL;
	}	  
	panl = (*global) (PANEL_SERVICES_NAME, GFUSE_TRANSIENT);
	if(!panl)
	{
		(*message->error)("Duoooh!:","Unable to activate global '"PANEL_SERVICES_NAME"'!");
		return AFUNC_BADGLOBAL;
	}
	if( panID=PAN_CREATE(panl,hail) )
	{
		DynaValue	val = {DY_INTEGER}; 
		if(!(od.obj = ITEM_CTL(panl,panID,"                          Object",global,LWI_OBJECT) ))
			goto controlError;
		if(!(od.pnts = INTRO_CTL(panl,panID,"Points") ))
			goto controlError;
		if(!(od.polys = INTRO_CTL(panl,panID,"Polygons") ))
			goto controlError;
		if(!(od.srfs = INTRO_CTL(panl,panID,"Surfaces") ))
			goto controlError;
		ctlOneLine(od.pnts,od.polys,od.srfs,NULL,NULL,NULL);
		SurfList[1] = NULL;
		if(!(od.snam = POPUP_CTL(panl,panID,"Current  Surface:  ",SurfList) ))
			goto controlError;
		if(!(od.ssiz = INTRO_CTL(panl,panID,"Surface Size  ") ))
			goto controlError;
		ctlOneLine(od.snam,od.ssiz,NULL,NULL,NULL,NULL);
		if(!(od.min = FVECRO_CTL(panl,panID,"Min. Corner") ))
			goto controlError;
		if(!(od.max = FVECRO_CTL(panl,panID,"Max. Corner") ))
			goto controlError;
		if(!(od.rad = FLOATRO_CTL(panl,panID,"Max. Radius") ))
			goto controlError;
		val.intv.value = (int)olistEvent;
		(*od.obj->set)(od.obj,CTL_USEREVENT,&val);	
		val.intv.value = (int)slistEvent;
		(*od.snam->set)(od.snam,CTL_USEREVENT,&val);	
		val.intv.value = (int)&od;
		(*od.obj->set)(od.obj,CTL_USERDATA,&val);
		(*od.snam->set)(od.snam,CTL_USERDATA,&val);
		if(PAN_POST(panl,panID))
		{
			(*message->info)("Fun","huh?");
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

ServerRecord	ServerDesc[] = {
	{ "LayoutGeneric",	"ObjectList",	ObjectList },
	{ NULL }
} ;
