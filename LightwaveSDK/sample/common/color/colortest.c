/*
 * Example plugin that uses the gColorPicker global
 *
 * Matt Craig
 */

#include <stdio.h>

#include <splug.h>
#include <lwran.h>
#include <lwpanel.h>
#include <gui_help.h>
#include <gcolorpicker.h>


static LWPanControlDesc   desc;	 							// required by macros in lwpanel.h
static LWValue ival={LWT_INTEGER},ivecval={LWT_VINT},		// required by macros in lwpanel.h
  fval={LWT_FLOAT},fvecval={LWT_VFLOAT},sval={LWT_STRING};
  
/*
 * A Bunch of globals cause this plugin is trivial and I'm lazy 
 */
  
MessageFuncs	*message;
LWPanelFuncs	*panl;
LWPanelID		panID;
int				swatchX, swatchY;
gColor_Picker	*myPicker;
LWControl		*colorButton, *colorString, *colorCanvas;

#define COLOR_W 18		// width of the color swatch
#define COLOR_H 18		// height of the color swatch
#define CIRCLE_W 10		// diameter of circle

int	circle[CIRCLE_W][CIRCLE_W] = {					// cheap way to draw circles!!!
				  {0,0,0,1,1,1,1,0,0,0},
				  {0,1,1,1,1,1,1,1,1,0},
				  {0,1,1,1,1,1,1,1,1,0},
				  {1,1,1,1,1,1,1,1,1,1},
				  {1,1,1,1,1,1,1,1,1,1},
				  {1,1,1,1,1,1,1,1,1,1},
				  {1,1,1,1,1,1,1,1,1,1},
				  {0,1,1,1,1,1,1,1,1,0},
				  {0,1,1,1,1,1,1,1,1,0},
				  {0,0,0,1,1,1,1,0,0,0}};


void buttonEvent(LWControl *ctl, void *data);		// callback for color button
void myDraw(LWControl *ctl,void *data);				// panel custom draw callback

XCALL_(int)colorTester (
	long			 version,
	GlobalFunc		*global,
	void			*inst,
	void			*serverData)
{
	int				rgb[3], x;
	

	XCALL_INIT;
	message = (MessageFuncs	*)(*global) ("Info Messages", GFUSE_TRANSIENT);
	if (!message )
		return AFUNC_BADGLOBAL;
		
	panl = (LWPanelFuncs *)(*global) (PANEL_SERVICES_NAME, GFUSE_TRANSIENT);
	if(!panl)
	{
		(*message->error)("Unable to activate global "PANEL_SERVICES_NAME, "     please add plugin lwpanels.p" );
		return AFUNC_BADGLOBAL;
	}
	
	/*
	 * Now get the gColorRequester global
	 */
	 
	myPicker = (gColor_Picker *)(*global) (GLOB_COLOR_PICKER_NAME, GFUSE_TRANSIENT);
	if(!myPicker)
	{
		(*message->error)("Unable to activate global "GLOB_COLOR_PICKER_NAME, "     please add plugin gColorPicker.p" );
		return AFUNC_BADGLOBAL;
	}
	
	if( panID=PAN_CREATE(panl,"Color Thang") )
	{	
		colorButton = BUTTON_CTL(panl, panID, "Color");						// add button
		if (colorButton == NULL) {
			(*message->error)("ERROR: Button creation", "Gotta motor!");
			return;
		}
		
		colorString = STRRO_CTL(panl, panID, "", 26);						// add display
		if (colorString == NULL) {
			(*message->error)("ERROR: stringRO creation", "Gotta motor!");
			return;
		}
		
		colorCanvas = CANVAS_CTL(panl, panID, "", COLOR_W, COLOR_H);		// add canvas to draw on
		if (colorCanvas == NULL) {
			(*message->error)("ERROR: Canvas creation", "Gotta motor!");
			return;
		}

		ctlOneLine(colorButton, colorString, colorCanvas, NULL, NULL, NULL);
		
		swatchX = CON_HOTX(colorCanvas);
		swatchY = CON_HOTY(colorCanvas);
		
		/*
		 * Set up initial values
		 */
		 		
		SET_STR(colorString, "R:000    G:000    B:000", 26);
		rgb[0] = 0; 
		rgb[1] = 0;
		rgb[2] = 0;
		
		CON_SETEVENT(colorButton, &buttonEvent, rgb);		// set event callback for button
		(*panl->set)(panID,PAN_USERDRAW, myDraw);			// set our own drawing function
		(*panl->set)(panID,PAN_USERDATA, rgb);				// set our data for drawfunc to access

		
		if (!(*panl->open)(panID, PANF_BLOCKING | PANF_CANCEL|PANF_FRAME | PANF_MOUSETRAP))
		{
			(*message->error)("Drag Nabbit!","You canceled");
		}
		PAN_KILL(panl,panID);
		
	}
	else {
		(*message->error)("Zoinks!:","Unable to create panel");
		return AFUNC_BADGLOBAL;
	}

	return AFUNC_OK;
}


/*
 * This callback is called whenever the user clicks on the button.
 * It should present the color picker and set appropriate interface
 * values
 */
 
void buttonEvent(LWControl *ctl, void *data)
{
	char 			*hail = "Color Picker";
	ColorPick_Local	pick;
	int				*rgb = (int *)data;
	char			tmp[30];

	pick.title = "Color";					// set window title
	pick.result = 0;						// set OK/Cancel to cancel
	pick.red = rgb[0];						// set init color
	pick.green = rgb[1];
	pick.blue = rgb[2];
	
	(*myPicker->Multi_Picker)(&pick);		// open color picker window
	if (pick.result) {						// if user didn't cancel, set new colors
		rgb[0] = pick.red;
		rgb[1] = pick.green;
		rgb[2] = pick.blue;
	}
	
	/*
	 * set all appropriate UI elements
	 */
	 
	sprintf(tmp, "R:%03d    G:%03d    B:%03d", rgb[0], rgb[1], rgb[2]);
	SET_STR(colorString, tmp, 26);
	myDraw(NULL, rgb);						// call draw func to update swatch manually
}

/*
 * This callback is called anytime the panel needs to draw itself as well as 
 * explicitly from the button function.
 */
 
void myDraw(LWControl *ctl,void *data)
{
	int	*rgb = (int*)data;
	int x,y;
	
	panl->drawFuncs->drawRGBBox(panID, 0, 0, 0, swatchX, swatchY, COLOR_W, COLOR_H);
	for (y = 0;y<CIRCLE_W;y++)
		for (x=0;x<CIRCLE_W;x++)
			if (circle[x][y])
				panl->drawFuncs->drawRGBPixel(panID, rgb[0], rgb[1], rgb[2], 
					swatchX + (COLOR_W-CIRCLE_W)/2 + x,swatchY + (COLOR_W-CIRCLE_W)/2 + y);
	panl->drawFuncs->drawBorder(panID, 1 , swatchX, swatchY, COLOR_W, COLOR_H);
}
	

ServerRecord	ServerDesc[] = {
	{ "LayoutGeneric", "Color_Test", colorTester},
	{ NULL }
};

