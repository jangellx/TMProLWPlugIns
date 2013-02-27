// ------------  Gradient Test ------------------- //

#include <splug.h>
#include <moni.h>
#include <lwbase.h>
#include <lwran.h>
#include <lwpanel.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include	"gradient.h"

#define		TRUE	1
#define		FALSE	0

MessageFuncs		*message;
GlobalFunc			*gGlobal;
LWPanelFuncs		*panl;
LWGradientHandler	*grad_handler;

gradInterfaceData	testGradient = {"Test Gradient"
					,{{"User parameter",0.0f,100.0f}
					,{NULL,0,0}} };

LWControl			*slider,*grad,*field2;
LWPanelID			*panID;

static LWPanControlDesc   desc;	 // required by macros in lwpanel.h
static LWValue ival={LWT_INTEGER},ivecval={LWT_VINT},		// required by macros in lwpanel.h
  fval={LWT_FLOAT},fvecval={LWT_VFLOAT},sval={LWT_STRING};

typedef	struct{
	LWGradient	*grad;
} testGrad;


// ----------------------------------------------- //

void	gradFunc(LWControl	*ctl,testGrad	*test)
{
	int		status;

	if (!test->grad)
	{
		test->grad = (*grad_handler->create)(0,LWGRADIENT_SCALAR,testGradient.pDesc);
		if(!test->grad )
		{
			(*message->error)("On a un probleme les gars!","Unable to create gradient editor");
			return;
		}
	}

	status = (*grad_handler->Interface)(test->grad,(char *)testGradient.title);
	if (status != -1)
		SET_INT(ctl,TRUE);
	else
	{
		(*grad_handler->destroy)(test->grad);
		test->grad = NULL;
		SET_INT(ctl,FALSE);
	}
}

void	sliderFunc(LWControl	*ctl,testGrad	*test)
{
	int		in;
	double	out;

	GET_INT(ctl,in);
	if (test->grad)
		(*grad_handler->evaluate)(test->grad,in,&out);
	else
		out = (double) in;
	SET_FLOAT(field2,out);
}


XCALL_(int)gradientTest (
	long			 version,
	GlobalFunc		*global,
	void			*inst,
	void			*serverData)
{
	testGrad		test;
	int				x,y;

	gGlobal	 = global;	

	message = (*gGlobal) ("Info Messages", GFUSE_TRANSIENT);
	if (!message )
		return AFUNC_BADGLOBAL;

	panl = (*gGlobal) (PANEL_SERVICES_NAME, GFUSE_TRANSIENT);
	if(!panl)
	{
		(*message->error)("Unable to activate global "PANEL_SERVICES_NAME, "please add plugin lwpanels.p" );
		return AFUNC_BADGLOBAL;
	}
	grad_handler = (*gGlobal) (GRADIENT_ACCESS_NAME, GFUSE_TRANSIENT);
	if(!grad_handler)
	{
		(*message->error)("Unable to activate global "GRADIENT_SERVICES_NAME,"Please add plugin Gradient.p");
		return AFUNC_BADGLOBAL;
	}

	panID=PAN_CREATE(panl,"Gradient Test");
	slider = MINISLIDER_CTL(panl,panID,"Input",30,0,100);
	grad = BOOLBUTTON_CTL(panl,panID," --> Gradient --> ");
	x = CON_X(slider) + (int)(1.2*CON_W(slider)); y = CON_Y(slider);	MOVE_CON(grad,x,y);
	field2 = FLOATRO_CTL(panl,panID,"Output");
	x = CON_X(grad) + (int)(1.2*CON_W(grad)); y = CON_Y(grad);	MOVE_CON(field2,x,y);

	CON_SETEVENT(grad,(int) gradFunc,(int) &test);
	CON_SETEVENT(slider,(int) sliderFunc,(int) &test);

	test.grad = NULL;
	(*panl->open)(panID,PANF_BLOCKING|PANF_CANCEL|PANF_FRAME);
	if (panID)
		PAN_KILL(panl,panID);

	if (test.grad)
		(*grad_handler->destroy)(test.grad);
	(*grad_handler->freeHandler)();
	return AFUNC_OK	;
}



// ------------------------------------------------------ //

ServerRecord	ServerDesc[] = {
	{ "LayoutGeneric", "testGradient", gradientTest},
	{ NULL }
} ;
