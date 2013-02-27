/* main.c - UNIX specific routines for sbXcityfly
 *  
 *  Written:     March 1996 (moved from sbXcityfly.c)
 *  Author:      Jeff LeBlanc
 *
 * $Id: main.c,v 1.2 1996/12/19 22:29:12 jeffl Exp $
 *
 *  (C) 1996-1997 Spacetec IMC Corporation.  All rights reserved.
 *
 *  The computer codes included in this file, including source code and 
 *  object code, constitutes the proprietary and confidential information of
 *  Spacetec IMC Corporation, and are provided pursuant to a license 
 *  agreement.  These computer codes are protected by international, federal
 *  and state law, including United States Copyright Law and international
 *  treaty provisions.  Except as expressly authorized by the license
 *  agreement, or as expressly permitted under applicable laws of member
 *  states of the European Union and then only to the extent so permitted,
 *  no part of these computer codes may be reproduced or transmitted in any
 *  form or by any means, electronic or mechanical, modified, decompiled,
 *  disassembled, reverse engineered, sold, transferred, rented or utilized
 *  for any unauthorized purpose without the express written permission of 
 *  Spacetec IMC Corporation.
 *
 *
-----------------------------------------------------------------------------*/

static char rcsid[] = "$Id: main.c,v 1.2 1996/12/19 22:29:12 jeffl Exp $";

/*-------------------------- BEGINNING OF CODE ------------------------------*/

#include "cpyright.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 *  X11/UNIX specific includes and definitions
 */

#include <types.h>

#include <Xlib.h>
#include <Xutil.h>
#include <Xproto.h>
#include <keysym.h>
#include <extensions/XI.h>
#include <extensions/XInput.h>

#include "platform.h"
#include "spwmacro.h"

#include "spw_bw.h"
#include "x11input.h"
#include "spw_slim.h"

#include "icon.h"
#include "iconmask.h"
#include "cityfly.h"

/*
 *  Typedefs to aid in windowing-system independence
 */

typedef Display * SPW_WINPARAM;
typedef void      SPW_RETURN;

/*
 *   X11 Windowing globals
 */

extern GC             Spw_Gc;
extern Display       *Spw_Dpy;
extern Window         Spw_mainWindow;
extern Pixmap         Spw_mainBuffer;
extern SPW_BwPtr      Spw_ButtonWindow;
extern unsigned long  Spw_white, Spw_black, Spw_grey, Spw_green;

/*
 *  Windowing and drawing
 */

extern Matrix Spw_Persp;
extern int    Spw_WindowSize;
extern int    Spw_WindowWidth, Spw_WindowHeight;          /* Window geometry */

extern int    Spw_SizeBldg, Spw_SizeLand;
extern data3d bldg[], land[];
extern uniq_st buniq[], luniq[];
extern int Spw_DoubleBuffer;

/* 
 * Spaceball flags 
 */

extern int   Spw_Redraw;
extern int   Spw_SingleAxisFilter,
             Spw_TransAllowed,
             Spw_RotAllowed,
             Spw_RollAllowed;
extern float Spw_Sensitivity;

SPW_RETURN SpwMotionEvent(SPW_WINPARAM, float *, void *);
SPW_RETURN SpwButtonPressEvent(SPW_WINPARAM, int, void *);
SPW_RETURN SpwButtonReleaseEvent(SPW_WINPARAM, int, void *);

/*---------------------------------------------------------------------------

  Function:  AllocPublicColor

  Description:
    Allocate the requested color cell from the RGB description

  Args:
    red    (r/o)  Red component
    green  (r/o)  Green component
    blue   (r/o)  Blue component

  Returns:  index into color map

-----------------------------------------------------------------------------*/

unsigned long 
AllocPublicColor(unsigned short red, 
                 unsigned short green,
                 unsigned short blue)
{
  XColor color;
  int    screen;
  
  color.red = red;
  color.green = green;
  color.blue = blue;
  screen=DefaultScreen(Spw_Dpy);
  
  XAllocColor(Spw_Dpy,DefaultColormap(Spw_Dpy,screen),&color);
  return(color.pixel);
}

/*-----------------------------------------------------------------------------

  Function:  DispatchLoop

  Description:
    This routine reads messages and dispatches them to the appropriate
    routines.  This is the UNIX version.

  Args:  None

  Returns:  status of done variable

-----------------------------------------------------------------------------*/

int 
DispatchLoop()
{
  SPW_InputDispatchStruct dispatch_data;
  XEvent                  xevent;
  SPW_InputEvent          spwevent;
  int done = FALSE;

  /*
   *  Set up the callback struct
   */

  SPW_InputInitDispatchStruct(&dispatch_data);
  dispatch_data.display = Spw_Dpy;
  dispatch_data.spwbw   = Spw_ButtonWindow;
  dispatch_data.xevent  = &xevent;
  dispatch_data.spwevent  = &spwevent;
  dispatch_data.handle_motion = SpwMotionEvent;
  dispatch_data.handle_bpress = SpwButtonPressEvent;
  dispatch_data.handle_brelease = SpwButtonReleaseEvent;
  dispatch_data.delay = 1;
  
  /*
   *  Loop, process events as possible Button Window, Spaceball, or app
   */

  while(!done)
    {
      if(!XPending(Spw_Dpy) && Spw_Redraw)
        {
          Spw_Redraw=FALSE;
          UpdateScreen();
        }
      XNextEvent(Spw_Dpy, &xevent);
      
      SPW_BwHandleEvent(Spw_ButtonWindow, &xevent);
      
      if(SPW_InputIsSpaceballEvent(Spw_Dpy, &xevent, &spwevent))
        {
          SPW_InputDispatch(&dispatch_data); 
        }
      else
        {
          Spw_Redraw|=HandleXEvent(&xevent, &done);
        }
    }  

  return done;

} /* end of DispatchLoop */

/*-----------------------------------------------------------------------------

  Function:  HandleXEvent

  Descritption:
    Process the X Windows events specific to the app.

  Args:
    event  (r/o)  Pointer to an XEvent structure

-----------------------------------------------------------------------------*/

int 
HandleXEvent(XEvent *event, int *done)
{
  int       redraw = FALSE;
  KeySym    sym;
  
  switch(event->type)
    {
    case KeyRelease:
      {
        /* THESE ARE X KEYBOARD KEY PRESSES */
        sym = XKeycodeToKeysym(Spw_Dpy, event->xkey.keycode, 0);
        if (sym == XK_Escape) {
          *done = TRUE;
        }

         if (sym == 'd')
           Spw_DoubleBuffer = !Spw_DoubleBuffer;
  
      }
      break;
      
    case Expose:
      {
        if (event->xexpose.window == Spw_mainWindow) 
          redraw = TRUE;
      }
      break;

    case DestroyNotify:
      *done = TRUE;
      break;
      
    default:
      break;
    }
  
  return(redraw);

} /* end of HandleXEvent */

/*---------------------------------------------------------------------------

  Function:  SpwUpdateButtons

  Description:
    Change any labels in the Button Window according to what Spaceball
    buttons were pressed/released.

  Args:
    none

-----------------------------------------------------------------------------*/

void 
SpwUpdateButtons()
{
  char buffer[256];
  int  scaleTop, scaleBottom;

  sprintf(buffer, "Translation %s", Spw_TransAllowed?"ON ":"OFF");
  SPW_BwChangeLabel(Spw_ButtonWindow, "1", buffer);

  sprintf(buffer, "Rotation %s", Spw_RotAllowed?"ON ":"OFF");
  SPW_BwChangeLabel(Spw_ButtonWindow, "2", buffer);

  sprintf(buffer, "Single Axis Filter %s", Spw_SingleAxisFilter?"ON ":"OFF");
  SPW_BwChangeLabel(Spw_ButtonWindow, "3", buffer);

  sprintf(buffer, "Roll %s", Spw_RollAllowed?"ON ":"OFF");
  SPW_BwChangeLabel(Spw_ButtonWindow, "4", buffer);

  if(Spw_Sensitivity<1.0)
    {
    scaleTop=1;
    scaleBottom=1/Spw_Sensitivity;
    sprintf(buffer, "Sensitivity: %2d/%2d", scaleTop, scaleBottom);
    }
  else
    {
    scaleTop = Spw_Sensitivity;
    sprintf(buffer, "Sensitivity:   %2d ", scaleTop);
    }
  
  SPW_BwChangeLabel(Spw_ButtonWindow, "Sensi", buffer);
}

/*-----------------------------------------------------------------------------

  Function:  setWindowHints

  Description:
    Set all the X-specific window flags to be a good X client

  Args:
    win         (r/o)  X-Window to configure
    argv        (r/o) Arguement list
    argc        (r/o) Number of arguements in argv
    window_name (r/o) Name of the window
    icon_name   (r/o) Name of the icon
    res_name    (r/o) Name of the XResource
    res_class   (r/o) Name of the XResource class to use

-----------------------------------------------------------------------------*/

void setWindowHints(Window win, char **argv, int argc, char *window_name,
                    char *icon_name, char *res_name, char *res_class)
{
  XSizeHints *size_hints;
  XWMHints *wm_hints;
  XClassHint *class_hints;
  XTextProperty windowName, iconName;

  XSelectInput(Spw_Dpy, win,
               ButtonPressMask | ButtonReleaseMask
               | KeyPressMask | KeyReleaseMask
               | ExposureMask 
               );

  /*
   * Allocate and set the the Size hints in a friendly way
   */

  if ( !(size_hints = XAllocSizeHints()) )
     {
     fprintf(stderr,"Not enough memory\n");
     exit(-1);
     }
  /*
   *  What things we will be setting.  Position and size refer to
   *  if the user set the values (via a resource file for example)
   *  or if the program set the values.
   */
  size_hints->flags = 
                      PPosition   | /* Program specified position      */
                      PSize       | /* Program specified size          */
                      PBaseSize   | /* Program specified base size     */
                      0;

  /*
   *  Now set the values.
   */
  size_hints->base_width = Spw_WindowWidth;
  size_hints->base_height = Spw_WindowHeight;
   
  /*
   * Allocate and set the the Window Manager hints in a friendly way.
   * This is probably the most critical to set correctly as some
   * WMs (olwm) are REALLY picky about setting them correctly and
   * hose you if you don't.  Like, for example, not sending you any
   * keypresses.
   */
  if ( !(wm_hints = XAllocWMHints()) )
     {
     fprintf(stderr,"Not enough memory\n");
     exit(-1);
     }

  /*
   *  What hints we will be setting
   */
  wm_hints->flags = InputHint        | /* Setting if we expect input  */
                    StateHint        | /* What initial state to be in */
                    IconPixmapHint   | /* Set the Icon pixmap         */
                    IconMaskHint     | /* Set the Icon Shape Mask     */
                    0;

  wm_hints->input = TRUE;                /* InputHint */
  wm_hints->initial_state = NormalState  /* or IconicState */; /* StateHint */

  wm_hints->icon_pixmap = XCreateBitmapFromData(Spw_Dpy,
                                                Spw_mainWindow,
                                                (char *)sbXcityfly_icon_bits,
                                                sbXcityfly_icon_width,
                                                sbXcityfly_icon_height);
  wm_hints->icon_mask = XCreateBitmapFromData(Spw_Dpy,
                                              Spw_mainWindow,
                                              (char *)sbXcityfly_iconMask_bits,
                                              sbXcityfly_iconMask_width,
                                              sbXcityfly_iconMask_height);
  /*
   * Allocate and set the the Class hints in a friendly way
   */
  if ( !(class_hints = XAllocClassHint()) )
     {
     fprintf(stderr,"Not enough memory\n");
     exit(-1);
     }
   /*
    *  Not much to say this is about it for class hints....
    */
  class_hints->res_name = res_name;
  class_hints->res_class = res_class;
  
  /*
   * Convert names into X strings
   */

  if ( XStringListToTextProperty( &window_name, 1, &windowName) == 0 )
     {
     fprintf(stderr,"Couldn't create string\n");
     exit(-1);
     }
  if ( XStringListToTextProperty( &icon_name, 1, &iconName) == 0 )
     {
     fprintf(stderr,"Couldn't create string\n");
     exit(-1);
     }

  /*
   * Finally we can set the properities.
   * This is supposedly the "correct" call.  Most of the
   * others like SetWMHints are obsolete.
   */

  XSetWMProperties(Spw_Dpy, win, &windowName, &iconName,
                   argv, argc, size_hints, wm_hints, class_hints);

}

/*-----------------------------------------------------------------------------

  Function:  main

  Description:
    This is the main entry point into the code (UNIX)

-----------------------------------------------------------------------------*/

void main(int argc, char **argv)
{
  int   i,j,k,
        x_return, y_return;
  float D;
  unsigned int width_return, height_return,
               border_width_return, depth_return;
  Window root_return;  
  char *display_name;
  int screen;
  XSetWindowAttributes winAttributes;
  char *window_name = "sbXcityfly";
  char *icon_name = "sbXcityfly";

  static ButtonRec buttonDefs[]={
    {&buttonDefs[1],  "1",     "Translation ON ",         0, 
     SPW_BwFlagRoundButton},
    {&buttonDefs[2],  "2",     "Rotation ON ",            0, 
     SPW_BwFlagRoundButton},
    {&buttonDefs[3],  "3",     "Single Axis Filter OFF",  0, 
     SPW_BwFlagRoundButton},
    {&buttonDefs[4],  "4",     "Roll ON ",                0, 
     SPW_BwFlagRoundButton},
    {&buttonDefs[5],  "5",     "Decrease sensitivity",    0, 
     SPW_BwFlagRoundButton},
    {&buttonDefs[6], "Sensi",  "Sensitivity:    1 ",      0, 
     SPW_BwFlagNoButton},
    {&buttonDefs[7],  "6",     "Increase sensitivity",    0, 
     SPW_BwFlagRoundButton},
    {&buttonDefs[8],  "7",     " ",                       0, 
     SPW_BwFlagRoundButton},
    {&buttonDefs[9],  "8",     "Rezero",                  0, 
     SPW_BwFlagRoundButton},
    {&buttonDefs[10],  "P",    "Reset View",              0, 
     SPW_BwFlagRoundButton},
    {&buttonDefs[11], NULL,    NULL,                      0, 
     SPW_BwFlagSeparator},
    {&buttonDefs[12], "ESC",   "Quit",            XK_Escape, 
     SPW_BwFlagKey},
    {NULL,                NULL,    NULL,                      0, 
             SPW_BwFlagSeparator},
  };


  /*
   *  Connect to the X Server
   */

  if ((display_name=getenv("DISPLAY")) == NULL)
    display_name=":0";
  
  if ((Spw_Dpy=XOpenDisplay(display_name)) == NULL)
    {
      fprintf(stderr,"%s: Could not open X Display\n",argv[0]);
      exit(1);
    }
  
  screen=DefaultScreen(Spw_Dpy);
  Spw_Gc=DefaultGC(Spw_Dpy,screen);
  XSetBackground(Spw_Dpy,Spw_Gc,BlackPixel(Spw_Dpy,screen));
  XSetForeground(Spw_Dpy,Spw_Gc,WhitePixel(Spw_Dpy,screen));
  winAttributes.background_pixel=BlackPixel(Spw_Dpy,screen);

  /*
   *  Create the main window proportional to the root window
   */
  XGetGeometry(Spw_Dpy, DefaultRootWindow(Spw_Dpy), &root_return,
               &x_return,&y_return,
               &width_return, &height_return,& border_width_return,
               &depth_return);

  Spw_WindowSize = .8 * MIN(height_return,width_return);
  Spw_WindowWidth = Spw_WindowSize;
  Spw_WindowHeight = Spw_WindowSize;

  Spw_mainWindow = XCreateWindow(Spw_Dpy,DefaultRootWindow(Spw_Dpy),
                                 250, 0, Spw_WindowWidth, Spw_WindowHeight,1,
                                 CopyFromParent,CopyFromParent,CopyFromParent,
                                 CWBackPixel,&winAttributes);
  Spw_mainBuffer= XCreatePixmap(Spw_Dpy, Spw_mainWindow, 
                                Spw_WindowWidth, Spw_WindowHeight,
                                XDefaultDepthOfScreen(XDefaultScreenOfDisplay(Spw_Dpy)));

  /*
   *  Select the colors to draw in
   */
  Spw_white   = AllocPublicColor(0xffff,0xffff,0xffff);
  Spw_black   = AllocPublicColor(0x0000,0x0000,0x0000);
  Spw_grey    = AllocPublicColor(0xffff,0xffff,0xffff);
  Spw_green   = AllocPublicColor(0x0000,0xffff,0x0000);
  
  /*
   * Set up and map the window
   */
  XStoreName(Spw_Dpy, Spw_mainWindow, argv[0]);
  setWindowHints(Spw_mainWindow, argv, argc, window_name, icon_name,
                 "sbXcityfly", "SBxcityfly");
  XMapWindow(Spw_Dpy, Spw_mainWindow);

  /*
   *  Initialize the Spaceball and Button Window
   */

  if (!(SPW_InputCheckForSpaceball(Spw_Dpy, Spw_mainWindow, "sbXcityfly")) )
    {
      fprintf (stderr, "Sorry, no Spaceball found!\n");
      exit(0);
    }
  
  Spw_ButtonWindow=SPW_BwInit(Spw_Dpy, DefaultScreenOfDisplay(Spw_Dpy), 
                   "sbXcityfly", buttonDefs);
  
  if(!Spw_ButtonWindow)
    {
      fprintf(stderr, "Error: BwInit returned NULL\n");
      exit(-1);
    }
  
   SpwUpdateButtons();

  /*
   *  Set up the geometry to be displayed
   */

   D = 1.0/tan( (double) ((M_PI/180.0)*(FOV/2.0)) );
   SPW_InitializeMatrix(Spw_Persp, 1.0, 0.0,  0.0,  0.0,
                               0.0, 1.0,  0.0,  0.0,
                                      0.0, 0.0, -1.0, -1.0/D,
                               0.0, 0.0,  0.0,  0.0);

   GetUniqPoints(bldg, buniq, Spw_SizeBldg );
   GetUniqPoints(land, luniq, Spw_SizeLand );

  /*
   *  Go into main loop, clean up if we ever exit cleanly
   */

   DispatchLoop();
   XDestroyWindow(Spw_Dpy, Spw_mainWindow);
   XCloseDisplay(Spw_Dpy);

} /* end of main */

