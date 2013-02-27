// EnView.c -- a Global Envelope Handler Example  
// by Arnie Cachelin  Copyright 1997  NewTek Inc.
// 5/10/97
// 6/4/97 -- more stuff

#include <splug.h>
#include <lwran.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lwpanel.h>
#include <gui_help.h>
#include <lwenvlp.h>

static LWPanControlDesc   desc;	 							// required by macros in lwpanel.h
static LWValue ival={LWT_INTEGER},ivecval={LWT_VINT},		
  fval={LWT_FLOAT},fvecval={LWT_VFLOAT},sval={LWT_STRING};

static MessageFuncs	*message;
static LWError MyError = NULL;

ehInterfaceData myIfaceData = { NULL, "Test Envelope", 
				{{"Intensity",0.0f, 360.0f},	{NULL} }}; 
						/*
							{"X Position",0.0f, 1.0f}, {"Y Position",0.0f, 1.0f}, {"Z Position",0.0f, 1.0f},
							{"Heading Angle",0.0f, 1.0f}, {"Pitch Angle",0.0f, 1.0f}, {"Bank Angle",0.0f, 1.0f},
							{"X Scale",0.0f, 1.0f}, {"Y Scale",0.0f, 1.0f}, {"Z Scale",0.0f, 1.0f},
							{NULL}
						}};	  */


typedef struct st_envData {
	LWPanelFuncs		*panl;
	LWPanelID			panID;
	EnvelopeHandler		*envHand;
	ehInterfaceData		*iface;
	Envelope			*env;
	LWControl 			*val, *ebutton,*t;
	int 				flags;  // bit flags	  
} envData;

void envEvent(LWControl *ctl, envData *edata)
{
	int b; 
	Envelope *bakup=NULL;
	GET_INT(edata->ebutton,b);
	
	MyError = NULL;
	if(!edata->env)
		edata->env = (*edata->envHand->create)(&MyError);
	else
	{
		if(edata->env)
			if(bakup = (*edata->envHand->create)(&MyError))
			{
				(*edata->envHand->copy)(edata->env,bakup);	
			}
	}	
	if(edata->env )
	{
		int result;
		GHOST_CON(edata->val);
		result=(*edata->envHand->Interface)(edata->env,edata->iface);

		if(result==1) // OK
		{
			if(!b) SET_INT(edata->ebutton,1);
			SET_FLOAT(edata->val,edata->env->keylist[0].cv[0]);
		}
		else if(result==-1)	  	// 'Remove Envelope'
		{	  
			(*edata->envHand->destroy)(edata->env); 
			edata->env = NULL;
			UNGHOST_CON(edata->val);
			if(b) SET_INT(edata->ebutton,0);
		}
		else //result==0
		{	  		//  replace changed inst with saved copy
			if(bakup)
			{
				(*edata->envHand->copy)(bakup,edata->env);	
				(*edata->envHand->destroy)(bakup); 
			}
			if(!b) SET_INT(edata->ebutton,1);
			SET_FLOAT(edata->val,edata->env->keylist[0].cv[0]);
		}
		(*edata->panl->draw)(edata->panID, DR_REFRESH);
	}
	else
	{
		UNGHOST_CON(edata->val);
		if(b) SET_INT(edata->ebutton,0);
	}
}

ChanVec	cVec;
void tEvent(LWControl *ctl, envData *edata)
{
	if(edata->env )
	{
		double time=0.0;
		GET_FLOAT(ctl,time);
		(*edata->envHand->evaluate)(edata->env, time, cVec);
		SET_FLOAT(edata->val,cVec[0]);	
	}
}


XCALL_(static int) envTest (
	long			 version,
	GlobalFunc		*global,
	LayoutGeneric 	*laydat, 
	void			*serverData)
{
	envData edata;
	double val=-666.0;
	XCALL_INIT;
	message = (*global) ("Info Messages", GFUSE_TRANSIENT);
	if (!message )
		return AFUNC_BADGLOBAL;
	edata.panl = (*global) (PANEL_SERVICES_NAME, GFUSE_TRANSIENT);
	if(!edata.panl)
	{
		(*message->error)("Looking for "PANEL_SERVICES_NAME, "Got lwpanels.p?");
		return AFUNC_BADGLOBAL;
	}
	edata.envHand = (*global) (ENVELOPE_ACCESS_NAME, GFUSE_TRANSIENT);
	edata.env = NULL;
	edata.iface = &myIfaceData;
	edata.iface->pfunc = edata.panl;
	if(!edata.envHand)
	{
		(*message->error)("Looking for "ENVELOPE_ACCESS_NAME, "Got envacces.p?");
		return AFUNC_BADGLOBAL;
	}
	if( edata.panID=(*edata.panl->create)("Envelope Handler Example",&edata) )
	{			   
		if(!(edata.val = FLOAT_CTL(edata.panl,edata.panID,"Animated Value:")))
			goto controlError;	  
		if(!(edata.ebutton = BOOLBUTTON_CTL(edata.panl,edata.panID,"E")))
			goto controlError;	  
		ctlOneLine(edata.val,edata.ebutton,NULL,NULL,NULL,NULL);
		if(!(edata.t = FLOAT_CTL(edata.panl,edata.panID,"  Current Time:")))
			goto controlError;	  
		SET_INT(edata.ebutton, 0);
		SET_FLOAT(edata.val,0.0);
		SET_FLOAT(edata.t,0.0);
		CON_SETEVENT(edata.ebutton,(int)envEvent,(int)&(edata));
		CON_SETEVENT(edata.t,(int)tEvent,(int)&(edata));
		if((*edata.panl->open)(edata.panID,PANF_BLOCKING|PANF_FRAME|PANF_CANCEL) )
		{				   
			if(edata.env)	
			{
				val=edata.env->keylist[0].cv[0];
				SET_FLOAT(edata.val,val);
			}
			else
			{
				GET_FLOAT(edata.val,val);
			}
		}
		else
			GET_FLOAT(edata.val,val);
		if(edata.env) (*edata.envHand->destroy)(edata.env);	
		(*edata.panl->destroy)(edata.panID);
		return (AFUNC_OK);
controlError:
		(*message->error)("drat!","Can't add Control");	  // my kinda error message
		(*edata.panl->destroy)(edata.panID);
	}
	else 
	{
		(*message->error)("Duoooh!:","Unable to create panel");	  //   ^
		return AFUNC_BADGLOBAL;
	}
	return (AFUNC_OK);
}



ServerRecord		ServerDesc[] = {
//	{ "CommandSequence", "EnvelopeView", envTest },	
	{ "LayoutGeneric", "EnvelopeView", envTest },	
	{ NULL }
};

