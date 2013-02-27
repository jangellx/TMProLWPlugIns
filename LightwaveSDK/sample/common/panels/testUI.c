 /*	 LW Panel Services test/example: Controls
 *  by Arnie Cachelin 8/15/96 Copyright Newtek, Inc. 
 */
#include <math.h>
#include <splug.h>
#include <lwran.h>
#include "lwpanel.h"
#include "gui_help.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#define NO_INTERRUPT
static LWPanControlDesc   desc;	 // required by macros in lwpanel.h
static LWValue ival={LWT_INTEGER},ivecval={LWT_VINT},		// required by macros in lwpanel.h
  fval={LWT_FLOAT},fvecval={LWT_VFLOAT},sval={LWT_STRING};

//#define	MODELER_VERSION
#define	MAX_NAMES		256
typedef struct st_nameList {
	int num;
	char *buf[MAX_NAMES];
} nameList;

typedef struct st_UIData { 
	int 			     panVersion;
	GlobalFunc		*global;
	LWPanelFuncs	*pfunc;
	LWPanelID		   panID;
	LWControl		  *c1,*c2,*c3,*c4,*c5,*c6,*c7,*c8;
	nameList		   names,names2;
	int				     viewLen;
} UIData;

void fillInfo(UIData *dat) {
	int iw=0,ih=0,d=0;
	char buf[64];
	sprintf(buf,"LWPanels API Version %d",dat->panVersion);
	SET_STR(dat->c1,buf,strlen(buf));
}

void	dragEvent(LWControl *ctl, UIData *dat) {
	int vec[3]={0,0,0};
	GETV_IVEC(ctl, vec);  
	SET_INT(dat->c2,vec[0]);
	SET_INT(dat->c3,vec[1]);
	SET_INT(dat->c4,vec[2]);
}	 


static char *PhilosophyDudes[] = {
	"Plato",
	"Augustine",
	"Spinoza",
	"Eckhardt",
	"Descartes",
	"Locke",
	"Kant",
	"Schopenhauer",
	"Proust",
	"Nietzsche",
	"Sartre",
	"Wittgenstein",
	"Russel",
	"Popper",
	"James",
	"Foucault",
	NULL
};

int nameNum2(UIData *dat) {
		return dat->names2.num;
}

char *nameFun2(UIData *dat, int n) {
		if(n>=dat->names2.num)
			return NULL;
		return dat->names2.buf[n];
}

char *nameFun(UIData *dat, int n) {
		if(n>=dat->names.num)
			return NULL;
		return dat->names.buf[n];
}

int nameNum(UIData *dat) {
		return dat->names.num;
}

void	list1Event(LWControl *ctl, UIData *dat) {
	int i;
	GET_INT(ctl,i);
	if(i>=0)
	{
		dat->names2.buf[dat->names2.num++] = dat->names.buf[i];
		REDRAW_CON(dat->c1);
	}
}	 

void	list2Event(LWControl *ctl, UIData *dat) {
	int i,n;
	GET_INT(ctl,n);
	if(n>=0) // if none is selected, value<0
	{
		if(dat->names2.num)
			dat->names2.num--;
		for(i=n; i<dat->names2.num; i++)
			dat->names2.buf[i] = dat->names2.buf[i+1];
		dat->names2.buf[dat->names2.num] = NULL;
		REDRAW_CON(ctl);
	}
}	 

void palDrawEvent(LWControl *pal, UIData *dat, DrMode mode)  {
	int x,y,w,dy,i;
	y = CON_HOTY(pal);
	x = CON_HOTX(pal);
	w = CON_HOTW(pal);
	dy = CON_HOTH(pal);// - dat->viewLen;
	dy /= dat->viewLen;
	for(i=0;i<dat->viewLen;i++)
	{
		(*dat->pfunc->drawFuncs->drawRGBBox)(dat->panID,i*40,(5-i)*40,3*i*(i+1),x+1,y,w-3,dy-1);
		y += dy;
	}

}

static char *TabNames[]={"Objects","Lights","Fog","  Particles  ","Cows",NULL};

XCALL_(int)UITest ( long version, GlobalFunc *global, void *inst, void *serverData) {
	LWPanelFuncs	*panl;
	LWPanelID		   panID;
	UIData 	       MyUI;
	MessageFuncs	*message;

	XCALL_INIT;

	message = (*global) ("Info Messages", GFUSE_TRANSIENT);
	if (!message )
		return AFUNC_BADGLOBAL;

	panl = (*global) (PANEL_SERVICES_NAME, GFUSE_TRANSIENT);
	if(!panl)	{
		(*message->error)("Unable to activate global "PANEL_SERVICES_NAME, "     please add plugin lwpanels.p" );
		return AFUNC_BADGLOBAL;
	}

	MyUI.pfunc = panl;
	if( panID=PAN_CREATE(panl,"This Is A Test: This Is Only A Test") ) 	{
		int ver=0,i;
		MyUI.panID=panID;
		MyUI.viewLen=6;
		MyUI.names.num = 15;
		MyUI.names2.num = 0;
		for(i=0; i<MyUI.names.num; i++)
			MyUI.names.buf[i] = PhilosophyDudes[i];
		if( !(MyUI.c5 = INTRO_CTL(panl,panID,"LWPanels API Version",)) )
			goto controlError;
		ver = PAN_GETVERSION(panl,panID);
		SET_INT(MyUI.c5,ver);

		if(ver>=10) {
			int x,y,w,h;
			if( !(MyUI.c8 = TABCHOICE_CTL(panl,panID," ",TabNames)) )
				goto controlError;
			if( !(MyUI.c7 = LISTBOX_CTL(panl,panID,"Here we go..",100,MyUI.viewLen,nameFun,nameNum)) )
				goto controlError;
			i = CON_HOTH(MyUI.c7);
			x = CON_X(MyUI.c7);
			if( !(MyUI.c6 = DRAGAREA_CTL(panl,panID,"",20,i)) )
				goto controlError;
			y = CON_Y(MyUI.c6);
			y -= CON_HOTY(MyUI.c6);
			y += CON_HOTY(MyUI.c7) + 10;
			MOVE_CON(MyUI.c6,x,y);
			w = CON_W(MyUI.c6);
			y = CON_Y(MyUI.c7) + 10;
			x += w;
			MOVE_CON(MyUI.c7,x,y);
			if( !(MyUI.c1 = LISTBOX_CTL(panl,panID,".. there we are",100,MyUI.viewLen,nameFun2,nameNum2)) )
				goto controlError;
//			ctlOneLine(MyUI.c7,MyUI.c1,NULL,NULL,NULL,NULL);
      x = CON_X( MyUI.c7 );
      w = CON_W( MyUI.c7 );
      x += w;
      y = CON_Y( MyUI.c1 );
      MOVE_CON( MyUI.c1, x, y );
			w = CON_W(MyUI.c7)+4;
			h = CON_H(MyUI.c7)+4;
			if( !(MyUI.c2 = BORDER_CTL(panl,panID,"Ha Ha Ha",w,h)) )
				goto controlError;
			MOVE_CON(MyUI.c2,x-2,y-2);
			if( !(MyUI.c2 = INT_CTL(panl,panID,"X",)) )
				goto controlError;
			if( !(MyUI.c3 = INT_CTL(panl,panID,"Y",)) )
				goto controlError;
			if( !(MyUI.c4 = INT_CTL(panl,panID,"N",)) )
				goto controlError;
			CON_SETEVENT(MyUI.c7,list1Event,(int)&MyUI);
			CON_SETEVENT(MyUI.c1,(int)list2Event,(int)&MyUI);
			CON_SETEVENT(MyUI.c6,(int)dragEvent,(int)&MyUI);
			ival.intv.value = (int)&palDrawEvent;
			(*MyUI.c6->set)(MyUI.c6,CTL_USERDRAW,&ival);	
			if( !(MyUI.c5 = PERCENT_CTL(panl,panID,"Yumminess",)) )
				goto controlError;
		}

		if((*panl->open)(panID,PANF_CANCEL|PANF_ABORT|PANF_BLOCKING)) {
			int i=2;
		}

		PAN_KILL(panl,panID);
		return AFUNC_OK;

controlError:
		(*message->error)("Duoooh!:","Unable to create control");
		PAN_KILL(panl,panID);
	} else {
		(*message->error)("Duoooh!:","Unable to create panel");
	}

	return AFUNC_OK	;
}
/*
ServerRecord	ServerDesc[] = {
#ifdef	MODELER_VERSION
	{ "CommandSequence", "TestUI",UITest },
#endif
	{ "LayoutGeneric", "TestUI",UITest },
	{ NULL }
} ;
*/
