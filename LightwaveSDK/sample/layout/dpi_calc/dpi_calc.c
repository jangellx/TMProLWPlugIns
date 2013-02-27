// DPI_Calc.c -- Calculate Image DPI from Size
// by Arnie Cachelin  Copyright 1996, 1997  NewTek Inc.

#include <splug.h>
#include <lwran.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lwpanel.h>
#include <gui_help.h>

static LWPanControlDesc   desc;	 							// required by macros in lwpanel.h
static LWValue ival={LWT_INTEGER},ivecval={LWT_VINT},		
  fval={LWT_FLOAT},fvecval={LWT_VFLOAT},sval={LWT_STRING};

// Local copy of the values from the UI
#define MAX_TAGS	10
typedef struct st_dpidat {
	int w,h, aspLock;
	double	xSize, ySize, xDPI, yDPI,aspect;
	GlobalFunc		*global;
	LWControl *res,*xres,*yres,*xsiz,*ysiz,*xdpi,*ydpi,*asp,*aspPre;
} dpidata;

// Resolution Pop-Up Enumeration
enum { RES_SUPLO, RES_LO, RES_MED, RES_HI, RES_PRINT, RES_CUSTOM };

// Aspect Ratio Pop-Up Enumeration
enum { ASP_SQ,  ASP_D1NTSC, ASP_D2NTSC,ASP_D1PAL, ASP_D2PAL, ASP_CUSTOM };

// Resolution Names
static char *resNames[] = {"Super-Low Resolution", "Low Resolution", "Medium Resolution", "High Resolution", "Print Resolution","Custom Resolution", NULL},
            *aspNames[] = {"Square","D1 NTSC","D2 NTSC","D1 PAL","D2 PAL","Custom",NULL};

// Resolution Sizes
int resSizesSQ[] = 	  {160,120,  320,240,  640,480,  1280,960,  2560,1920,   256,256,  0,0},
	resSizesD1PAL[] = {180,144,  360,288,  720,576,  1440,1152,  2880,2304,   256,256,  0,0},
	resSizesD2PAL[] = {188,144,  376,288,  752,576,  1504,1152,  3008,2304,   256,256,  0,0},
	resSizesD1NTSC[] = {180,121, 360,243,  720,486,  1440,972,  2880,1944,   256,256,  0,0},
	resSizesD2NTSC[] = {188,120, 376,240,  752,480,  1504,960,  3008,1920,   256,256,  0,0};

// Resolution Aspect Ratios
int *resSizeAspects[] = {resSizesSQ,resSizesD1NTSC,resSizesD2NTSC,resSizesD1PAL,resSizesD2PAL,resSizesSQ,NULL};
double resAspects[] = {1.0, 1.067, 1.019, 0.900, 0.859, 1.0};

// RenderSize():  Used to set the default values in the panel
//                when it's first opened, getting it's values
//                from the current render size.  This sets
//                the width and height and returns the aspect.
// fill size (w,h), return fp aspect
double renderSize(GlobalFunc *global, int *w, int *h) {
	LWSceneInfo *si;
	*w = 0; *h = 0;
	if(si=(*global)("LW Scene Info",GFUSE_TRANSIENT))
	{
		*w = si->frameWidth;	
		*h = si->frameHeight;
		return si->pixelAspect;	
	}
	return 0.0;
}

// setRes():  Set Resolution
int setRes(dpidata *dat) {
	int res,a,*resSizes = resSizeAspects[0]; 
	
	GET_INT(dat->aspPre, a);
	if(a<ASP_CUSTOM)
		resSizes = resSizeAspects[a];	
	for(res=0;res<RES_CUSTOM; res++)
	{
		if( (dat->w == resSizes[res*2])	&& (dat->h == resSizes[1+res*2]) )
			break;  
	}
	if(res<RES_CUSTOM)
	{
		GHOST_CON(dat->xres);
		GHOST_CON(dat->yres);
		SET_INT(dat->res, res);	
	}
	else
	{
		UNGHOST_CON(dat->xres);
		UNGHOST_CON(dat->yres);
		SET_INT(dat->res, RES_CUSTOM);	
	}
	return res;
}

// alockEvent():  Handle the Aspect Ration Lock button
void alockEvent(LWControl *ctl, dpidata *dat) {
	GET_INT(ctl, dat->aspLock);
}

// dpiEvent():  Handle a change in the DPI
void dpiEvent(LWControl *ctl, dpidata *dat) {
	double x=1.0,y=1.0; 
	if(ctl==dat->ydpi)
	{
		GET_FLOAT(dat->ydpi,y);	
		if(y)
		{
			dat->yDPI = y;
			dat->ySize = dat->h/dat->yDPI;
			SET_FLOAT(dat->ysiz, dat->ySize);
		}
		else
			SET_FLOAT(dat->ydpi,dat->yDPI);	
	}
	else
	{
		GET_FLOAT(dat->xdpi,x);
		if(x)
		{
			dat->xDPI = x;
			dat->xSize =	dat->w/dat->xDPI;
			SET_FLOAT(dat->xsiz, dat->xSize);
		/*	if(dat->aspLock)
			{
				dat->yDPI = x;
				SET_FLOAT(dat->ydpi,dat->yDPI);	
				dat->ySize =	dat->h/dat->yDPI;
				SET_FLOAT(dat->ysiz, dat->ySize);
			}	   */
		}
		else
			SET_FLOAT(dat->xdpi,dat->xDPI);	
	}
}

// sizeEvent():  Handle a change in the resolution, numeric entry
void sizEvent(LWControl *ctl, dpidata *dat) {
	double x,y; 
	if(ctl==dat->ysiz)
	{
		GET_FLOAT(dat->ysiz,y);
		if(y) 
		{
			dat->h = (int)(y*dat->yDPI);
			if(dat->aspLock)
			{
				x = ((y*dat->w/dat->h));
				SET_FLOAT(dat->xsiz,x);
				dat->w = (int)(x*dat->xDPI);
				SET_INT(dat->xres, dat->w);
			}
			SET_FLOAT(dat->ydpi,y);
			SET_INT(dat->yres, dat->h);
		}
		else
		{
			y = dat->h/dat->yDPI; 
			SET_FLOAT(dat->ysiz,y);
		}
	}
	else
	{
		GET_FLOAT(dat->xsiz,x);
		if(x) 
		{
			if(dat->aspLock)
			{
				y = ((x*dat->h/dat->w));
				SET_FLOAT(dat->ysiz,y);
				dat->h = (int)(y*dat->yDPI);
				SET_INT(dat->yres, dat->h);
			}
			dat->w = (int)(x*dat->xDPI);
			SET_INT(dat->xres, dat->w);
		}
		else
		{
			x = dat->w/dat->xDPI; 
			SET_FLOAT(dat->xsiz,x);
		}
	}
	setRes(dat);
}

// resEvent:  Handle a change in the resolution, pop-up
void resEvent(LWControl *ctl, dpidata *dat) {
	int res,x,y,*resSizes = resSizeAspects[0]; 
	
	GET_INT(dat->aspPre, y);
	if(y<ASP_CUSTOM)
		resSizes = resSizeAspects[y];	
	if(ctl==dat->yres)
	{
		GET_INT(dat->yres, y);
		if(y!=dat->h)
		{
			if(dat->aspLock)
			{
				if(y) 
					dat->h = y;	
				dat->w = (int)(0.5+(dat->aspect*dat->h));
				SET_INT(dat->xres, dat->w);
			}
			else if(y)
			{
				 dat->h = y;
				 dat->aspect = dat->w/dat->h;
				 SET_FLOAT(dat->asp,dat->aspect);
			}
			dat->xSize = dat->w/dat->xDPI;
			dat->ySize = dat->h/dat->yDPI;
			SET_FLOAT(dat->xsiz,dat->xSize);
			SET_FLOAT(dat->ysiz,dat->ySize);
		}
	}
	else if(ctl==dat->xres)
	{
		GET_INT(dat->xres, x);
		if(x!=dat->w)
		{
			if(dat->aspLock)
			{
				if(x) 
					dat->w = x;	
				dat->h = (int)(0.5+(dat->w/dat->aspect));
				SET_INT(dat->yres, dat->h);
			}
			else if(x)
			{	 
				 dat->w = x;
				 dat->aspect = dat->w/dat->h;
				 SET_FLOAT(dat->asp,dat->aspect);
			}
			dat->xSize = dat->w/dat->xDPI;
			dat->ySize = dat->h/dat->yDPI;
			SET_FLOAT(dat->xsiz,dat->xSize);
			SET_FLOAT(dat->ysiz,dat->ySize);
		}
	} 
	else
	{
		GET_INT(dat->res, res);	
		if(res<RES_CUSTOM)
		{
			GHOST_CON(dat->xres);
			GHOST_CON(dat->yres);
		}
		else
		{
			UNGHOST_CON(dat->xres);
			UNGHOST_CON(dat->yres);
		}
		dat->w = resSizes[res*2];  
		dat->h = resSizes[1+res*2];  
		SET_INT(dat->xres, dat->w);
		SET_INT(dat->yres, dat->h);
		dat->xSize = dat->w/dat->xDPI;
		dat->ySize = dat->h/dat->yDPI;
		SET_FLOAT(dat->xsiz,dat->xSize);
		SET_FLOAT(dat->ysiz,dat->ySize);
	}
}

// aspPreEvent():  Handle Aspect Ratio Preset pop-up changes
void aspPreEvent(LWControl *ctl, dpidata *dat)   // A callback for item changes
{
	int a;
	GET_INT(ctl, a); 
	dat->aspect = resAspects[a];
	SET_FLOAT(dat->asp,dat->aspect);
	if(a<ASP_CUSTOM)
	{
		resEvent(dat->res,dat);
		GHOST_CON(dat->asp);
	}
	else
 		UNGHOST_CON(dat->asp);


}

// getRes():  Get the resolution that most closely matches the values input?
int getRes(int w, int h, int *asp) {
	int i,a,*resSizes;	  
	for(a=ASP_SQ; a<ASP_CUSTOM; a++)   // Go from the First to Last entry in the Aspect Pop-Up
	{
		*asp = a;
		resSizes = resSizeAspects[a];	
		for(i=RES_SUPLO; i<RES_CUSTOM; i++)
			if((w==resSizes[i*2]) && (h==resSizes[1+(i*2)]))  
				return i;
	}
	return i;
}

// More Globals
LWItemID  item=0;
dpidata	  dpdat = {640,480,1,1.0,1.0,100.0,100.0,1.0};
int res	= RES_MED;
int asp = ASP_SQ;

// DPI_Calc():  Activate Function
XCALL_(static int) DPI_Calc( long version, GlobalFunc *global,
                             LayoutGeneric *dat, void	*serverData ) {
	LWPanelFuncs	*panl;     // Pointer to the LW Panels Functions
	LWPanelID		   panID;    // ID of the panel created
	MessageFuncs	*message;  // Quick Requester Functions

	XCALL_INIT;              // Standard plug-in initialization

  // Get the Message Functions
	message = (*global) ("Info Messages", GFUSE_TRANSIENT);
	if (!message )
		return AFUNC_BADGLOBAL;

  // Set up some default variables
	dpdat.global=global;
	dpdat.aspLock = 1;
	dpdat.aspect = renderSize(global,&dpdat.w,&dpdat.h);
	res = getRes(dpdat.w,dpdat.h, &asp);

	if(dpdat.xDPI<=0)
    dpdat.xDPI = 100.0;	 
	dpdat.xSize = dpdat.w/dpdat.xDPI;

	if(dpdat.yDPI<=0)
    dpdat.yDPI = 100.0;	 
	dpdat.ySize = dpdat.h/dpdat.yDPI;

  // Get the Panel Service functions
	panl = (*global) (PANEL_SERVICES_NAME, GFUSE_TRANSIENT);
	if(!panl)
	{
		(*message->error)("Is the plugin lwpanels.p around?",
                      "I need "PANEL_SERVICES_NAME);  // Useful error info
		return AFUNC_BADGLOBAL;
	}

  // Create the new Panel
	if( panID=(*panl->create)("Image DPI Calulator",panl) ) { // Your Name Here
    // Set some initial controls
		LWValue	val = {LWT_INTEGER}; 
		LWControl	*alock;
		int x,y,w;

		if(!(dpdat.res = WPOPUP_CTL(panl,panID,"Preset Frame Size",resNames,120)))  // Resolution pop-up 
			goto controlError;

		if(!(dpdat.xres = INT_CTL(panl,panID,"   Width")))                          // Width text entry
			goto controlError;

		ctlOneLine(dpdat.res, dpdat.xres , NULL,NULL,NULL,NULL);                    // Put Res and X-Res on the same line
 		if(!(dpdat.yres = INT_CTL(panl,panID,"Height")))                            // Height text entry
			goto controlError;

		ctlOneLine(dpdat.res, dpdat.xres ,dpdat.yres ,NULL,NULL,NULL);              // Put Res, X-Res and Y-Res on the same line (so why is that other ctlOneLine() call up there?)
		x = CON_X(dpdat.res);                                                       
 		x += CON_LW(dpdat.res);
		w = x;

		if(!(dpdat.aspPre = WPOPUP_CTL(panl,panID,"Standard Aspect", aspNames,80))) // Aspect Ratio pop-up
			goto controlError;
 		y = CON_Y(dpdat.aspPre);
		x -= CON_LW(dpdat.aspPre);
		MOVE_CON(dpdat.aspPre,x,y);                                                 // Move the new control under the Resolution pop-up

		if(!(dpdat.asp = FLOAT_CTL(panl,panID,"   Pixel Aspect Ratio")))            // Pixel Aspect Ration text entry
			goto controlError;
 		ctlOneLine(  dpdat.aspPre,dpdat.asp,NULL,NULL,NULL,NULL);                   // Put it on the same line as the pop-up

		if(!(dpdat.xdpi = FLOAT_CTL(panl,panID,"Density(dpi)  X")))                 // X DPI
			goto controlError;
 		if(!(dpdat.ydpi = FLOAT_CTL(panl,panID,"  Y")))                             // Y DPI
			goto controlError;
 		y = CON_Y(dpdat.xdpi);
		x = w-CON_LW(dpdat.xdpi);
		MOVE_CON(dpdat.xdpi,x,y);                                                  // Move the X DPI under the Aspect pop-up
		ctlOneLine(dpdat.xdpi, dpdat.ydpi ,NULL,NULL,NULL,NULL);                   // Put the DPI controls on the same line

		if(!(dpdat.xsiz = DIST_CTL(panl,panID,"Output Width")))                    // Output Width text entry
			goto controlError;
 		if(!(dpdat.ysiz = DIST_CTL(panl,panID,"Height")))                          // Output Height text entry
			goto controlError;
  		y = CON_Y(dpdat.xsiz);
		x = w-CON_LW(dpdat.xsiz);
		MOVE_CON(dpdat.xsiz,x,y);                                                  // Put the controls under the X DPI control
		ctlOneLine(dpdat.xsiz, dpdat.ysiz ,NULL,NULL,NULL,NULL);                   // and put them on the same line
 		if(!(alock = BOOL_CTL(panl,panID,"Lock Aspect")))                          // Aspect Lock
			goto controlError;
		ctlOneLine(dpdat.xsiz, dpdat.ysiz ,alock,NULL,NULL,NULL);                  // put the X DPI, Y DPI and Aspect Lock on the same line

    // Default Text Entry Values
		SET_INT(dpdat.res,res);
		SET_INT(dpdat.xres,dpdat.w);
		SET_INT(dpdat.yres,dpdat.h);

		if(res<RES_CUSTOM) {
			GHOST_CON(dpdat.xres);
			GHOST_CON(dpdat.yres);
		}

		SET_FLOAT(dpdat.asp,dpdat.aspect);
		SET_INT(dpdat.aspPre,asp);

		if(asp<RES_CUSTOM)
			GHOST_CON(dpdat.asp);

		SET_FLOAT(dpdat.xsiz,dpdat.xSize);
		SET_FLOAT(dpdat.ysiz,dpdat.ySize);
		SET_FLOAT(dpdat.xdpi,dpdat.xDPI);
		SET_FLOAT(dpdat.ydpi,dpdat.yDPI);
		SET_INT(alock,dpdat.aspLock);
		val.intv.value = (int)&dpdat;
		(*dpdat.res->set)(dpdat.res,CTL_USERDATA,&val);	
		(*dpdat.res->set)(dpdat.aspPre,CTL_USERDATA,&val);	
		(*dpdat.xres->set)(dpdat.xres,CTL_USERDATA,&val);	
		(*dpdat.yres->set)(dpdat.yres,CTL_USERDATA,&val);	
		(*dpdat.xsiz->set)(dpdat.xsiz,CTL_USERDATA,&val);	
		(*dpdat.ysiz->set)(dpdat.ysiz,CTL_USERDATA,&val);	
		(*dpdat.xdpi->set)(dpdat.xdpi,CTL_USERDATA,&val);	
		(*dpdat.ydpi->set)(dpdat.ydpi,CTL_USERDATA,&val);	
		(*alock->set)(alock,CTL_USERDATA,&val);	

    // Set the Event Handlers for the UI elements
		val.intv.value = (int)alockEvent;
		(*alock->set)(alock,CTL_USEREVENT,&val);
		val.intv.value = (int)aspPreEvent;
		(*dpdat.aspPre->set)(dpdat.aspPre,CTL_USEREVENT,&val);	
		val.intv.value = (int)resEvent;
		(*dpdat.res->set)(dpdat.res,CTL_USEREVENT,&val);	
		(*dpdat.xres->set)(dpdat.res,CTL_USEREVENT,&val);	
		(*dpdat.yres->set)(dpdat.res,CTL_USEREVENT,&val);	
		val.intv.value = (int)sizEvent;
		(*dpdat.xsiz->set)(dpdat.xsiz,CTL_USEREVENT,&val);	
		(*dpdat.ysiz->set)(dpdat.ysiz,CTL_USEREVENT,&val);	
		val.intv.value = (int)dpiEvent;
		(*dpdat.xdpi->set)(dpdat.xdpi,CTL_USEREVENT,&val);	
		(*dpdat.ydpi->set)(dpdat.ydpi,CTL_USEREVENT,&val);	
    
    // Center the panel in the display
		y = PAN_GETH(panl,panID);		
		y -= CON_H(dpdat.xdpi);
		PAN_SETH(panl,panID,y);		
		if((*panl->open)(panID,PANF_BLOCKING)) {				  
			item = 0;
		}
		(*panl->destroy)(panID);
		return (AFUNC_OK);
controlError:
		(*message->error)("drat!","Can't add Control");	  // my kinda error message
		(*panl->destroy)(panID);
	}
	else 
	{
		(*message->error)("Duoooh!:","Unable to create panel");	  //   ^
		return AFUNC_BADGLOBAL;
	}
	return (AFUNC_OK);
}


// Tell LW what we are
ServerRecord		ServerDesc[] = {
	{ "LayoutGeneric", "DPI_Calc", DPI_Calc },	
	{ NULL }
};

