/*  sbXcityfly.c  -- Spaceball fly-through program
 *
 *  Written:     September 1992
 *  Author:      Jim Wick
 *
 * $Id: cityfly.c,v 1.13 1998/03/23 19:37:05 mfarr Exp $
 *
 * (C) 1998 Spacetec IMC Corporation (Spacetec). All rights reserved. 
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby grated provided that this copyright
 * notice appears in all copies.  Permission to modify this software is granted
 * and Spacetec will support such modifications only is said modifications are
 * approved by Spacetec.
 *
--------------------------- BEGINNING OF NOTES -------------------------------

 Simple vector drawing program that allows the user to fly around a cityscape.
 All drawing output through Xlib so it can run on any platform.

-----------------------------------------------------------------------------*/
 
static char rcsid[] = "$Id: cityfly.c,v 1.13 1998/03/23 19:37:05 mfarr Exp $";

/*-------------------------- BEGINNING OF CODE ------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cityfly.h"

#define SPW_PICKBUTTON   SI_PICK_BUTTON
#define SPW_TRANS        1
#define SPW_SINGLE_AXIS  3

#if defined(OS_WIN32)

/*
 *  Win32 specific includes and definions
 */

#include <windows.h>
#include <float.h>
#include <stdlib.h>

#include "spwmacro.h"
#include "si.h"
#include "spwslim.h"
#include "resource.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/* The number of positions in the scrollbar for sensitivity */
#define NUM_SCROLL_POS 9

/*
 *  Typedefs to aid in windowing-system independence
 */

typedef short SPW_WINPARAM;
typedef int SPW_RETURN;

/*
 *  Win32 windowing globals
 */

HPEN  Spw_white, Spw_black, Spw_grey, Spw_green;
HWND  Spw_hWndMain;
HWND  Spw_hWndButton;
HDC   Spw_hDCBitmap = NULL;
HBITMAP Spw_hBitmap = NULL;
HDC   Spw_hDCScreen = NULL;
HDC   Spw_hDC;
short Spw_DeviceHandle;

#else /* !WIN_NT so default to X11 */

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
#include "spwslim.h"

#include "icon.h"
#include "iconmask.h"

/*
 *  Typedefs to aid in windowing-system independence
 */

typedef Display * SPW_WINPARAM;
typedef void      SPW_RETURN;

/*
 *   X11 Windowing globals
 */

GC             Spw_Gc;
Display       *Spw_Dpy;
Window         Spw_mainWindow;
Pixmap         Spw_mainBuffer;
SPW_BwPtr      Spw_ButtonWindow;
XSegment       Spw_drawSegments[5000];
unsigned long  Spw_white, Spw_black, Spw_grey, Spw_green;

int  HandleXEvent(XEvent *, int *);

#endif /* end else 0S_WIN32 */

/*
 *   General defines, macros, and global variables
 */

/* 
 * Spaceball flags 
 */

int   Spw_SingleAxisFilter = FALSE,
      Spw_TransAllowed = TRUE,
      Spw_RotAllowed = TRUE,
      Spw_RollAllowed = FALSE;
int   Spw_Redraw = FALSE;
float Spw_Sensitivity = 1.0f;

/*
 *  Drawing and display globals
 */

int Spw_WindowWidth, Spw_WindowHeight;          /* Window geometry */
int Spw_WindowSize;                 
int Spw_DoubleBuffer = TRUE;
Matrix Spw_World_to_eye={{1.0f,0.0f,0.0f,0.0f},             /* Transformation matrices */
                         {0.0f,1.0f,0.0f,0.0f},
                         {0.0f,0.0f,1.0f,0.0f},
                         {XTRAN,YTRAN,ZTRAN,1.0f}},
       Spw_Persp;

/* 
 *  Include the object data; the geometry for the cityscape is
 *  compiled into the code directly.
 */

#define MOVE FALSE
#define DRAW TRUE
#define MOVEI(x,y,z) {MOVE,{x,y,z}}
#define DRAWI(x,y,z) {DRAW,{x,y,z}}

data3d  bldg[]= {
#include "bldgs.dat"   /* Compiled-in data sets */
};

data3d land[]={
#include "land.dat"
};

int Spw_SizeBldg = NUM_ELEMENTS(bldg);  /* Size of data sets */
int Spw_SizeLand = NUM_ELEMENTS(land);

uniq_st buniq[NUM_ELEMENTS(bldg)]={{NULL,{0.0f,0.0f,0.0f}}},  /* model's  uniq points */
        luniq[NUM_ELEMENTS(land)]={{NULL,{0.0f,0.0f,0.0f}}};  /* land's  uniq points  */

/*
 *  Platform independent function definitions
 */

void SpwButtonPressEvent(SPW_WINPARAM, int, void *);
void SpwButtonReleaseEvent(SPW_WINPARAM, int, void *);

void SpwUpdateButtons(void);
void DrawObject(data3d *data, uniq_st *uniq, int num_elements, 
                Matrix world2eye, Matrix eye2screen);
void GetUniqPoints( data3d *local_data, uniq_st *local_uniq, 
                    int cnt );

extern SiHdl devHdl;

/*-----------------------------------------------------------------------------

  Function:  UpdateScreen

  Description:
    Redraw the screen according to the current transformation matrix.
    Uses single-buffered drawing scheme.

  Args:
    None.

-----------------------------------------------------------------------------*/

void UpdateScreen()

{
  float tmp[3];
  Matrix eye_to_screen, mTmp;
  static Matrix mirrory = {{ 1.0f,  0.0f,  0.0f,  0.0f},
                           { 0.0f, -1.0f,  0.0f,  0.0f},
                           { 0.0f,  0.0f,  1.0f,  0.0f},
                           { 0.0f,  0.0f,  0.0f,  1.0f}};

  /*
   *  Clear the drawing area for redisplay
   */

#if defined(OS_WIN32)
  HPEN old;
  old = SelectObject ( Spw_hDC, GetStockObject(BLACK_BRUSH) );
  Rectangle( Spw_hDC, -1, -1, Spw_WindowWidth+1, Spw_WindowHeight+1);
  SelectObject(Spw_hDC,old);
#else
  if (Spw_DoubleBuffer) {
    XSetForeground(Spw_Dpy,Spw_Gc,Spw_black);
    XFillRectangle(Spw_Dpy, Spw_mainBuffer, Spw_Gc, 0, 0, 
                   Spw_WindowWidth, Spw_WindowHeight);
  }
  else
    XClearWindow(Spw_Dpy, Spw_mainWindow);
#endif

  /*
   *  Calculate the current view
   */

  SPW_MakeIdentityMatrix(eye_to_screen);
  tmp[0] = tmp[1] = Spw_WindowSize/2.0f; 
  tmp[2]=0.0f;

  SPW_MakeIdentityMatrix (mTmp);
  mTmp[3][0] = tmp[0];
  mTmp[3][1] = tmp[1];
  mTmp[3][2] = tmp[2];

  SPW_Mult44x44(eye_to_screen,Spw_Persp,mirrory); 
  SPW_Mult44x44(eye_to_screen, eye_to_screen, mTmp);

  /*
   * Set the color for the buildings
   */

#if defined(OS_WIN32)
  old = SelectObject ( Spw_hDC, Spw_grey );
#else
  XSetForeground(Spw_Dpy,Spw_Gc,Spw_grey);
#endif

  /*
   *  Render the building
   */
  DrawObject(bldg, buniq, NUM_ELEMENTS(bldg), 
             Spw_World_to_eye, eye_to_screen);


  /*
   *  Set the color for the land grid
   */
#if defined(OS_WIN32)
   SelectObject ( Spw_hDC, old );
   old = SelectObject ( Spw_hDC, Spw_green );
#else
  XSetForeground(Spw_Dpy,Spw_Gc,Spw_green);
#endif

  /*
   *  Render the land
   */

  DrawObject(land, luniq, NUM_ELEMENTS(land), 
             Spw_World_to_eye, eye_to_screen);

#if defined(OS_WIN32)
  if (Spw_DoubleBuffer) 
    {
      BitBlt(Spw_hDCScreen, 0, 0, Spw_WindowWidth, Spw_WindowHeight,
             Spw_hDCBitmap, 0, 0, SRCCOPY);
    }
  else
    SelectObject ( Spw_hDC, old );
#else
  if (Spw_DoubleBuffer)
    XCopyArea(Spw_Dpy, Spw_mainBuffer, Spw_mainWindow, Spw_Gc, 0, 0,
              Spw_WindowWidth, Spw_WindowHeight, 0, 0);

#endif


} /* end of UpdateScreen */

/*-----------------------------------------------------------------------------

 int SPW_ClipZ( float p0[], float p1[], float frontplane, float backplane)

 Args: 
   p0          (r/w)  -one end point of the line segment
   p1          (r/w)  -the other end point
   frontplane  (r/o)  -the Z coordinates of the nearest plane to clip
                       against.  A line segment in front of this
                       plane is removed.  Frontplane is assumed to
                       be a negative Z value that is greater than
                       backplane.
   backplane   (r/o)  -the Z coordinate of the farthest plane to clip
                       against.  A line segment in back of this plane
                       is removed.  Backplane is assumed to be a
                       negative Z value that is less than frontplane.

 Return Value:
   SPW_TRUE  if there is some usable geometry that survived the clip
   SPW_FALSE if everything was clipped away

 Description:
   Clips 3D lines to front and back clipping planes.  Does not clip sides.

---------------------------------------------------------------------------*/
int
SPW_ClipZ( float p0[], float p1[], float frontplane, float backplane )
{
#define X(p) p[0]
#define Y(p) p[1]
#define Z(p) p[2]
#define INFRONTOF >
#define INBACKOF  <

   /*
    * CHECK TRIVIAL CASES
    */

   /*
    * Swap front and back planes if they are specified in the reverse order.
    * This means they are clipping from the other direction.
    */
   if (frontplane INBACKOF backplane)
      {
      float t = frontplane;
      frontplane = backplane;
      backplane = t;
      }

   /*
    * Are both endpoints between the front and back clipping planes?
    */
   if ( (Z(p0) INBACKOF frontplane) && (Z(p0) INFRONTOF backplane) &&
        (Z(p1) INBACKOF frontplane) && (Z(p1) INFRONTOF backplane) ) 
      {
      return SPW_TRUE;
      }
  
   /*
    * Are both endpoints in front of the front clipping plane?
    */
   if ( (Z(p0) INFRONTOF frontplane) && (Z(p1) INFRONTOF frontplane) ) 
      {
      return SPW_FALSE;
      }
  
   /*
    * Are both endpoints in back of the back clipping plane?
    */
   if ( (Z(p0) INBACKOF backplane) && (Z(p1) INBACKOF backplane) ) 
      {
      return SPW_FALSE;
      }

  /*
   * The line crosses at least one plane
   */

   /*
    * Make sure p1 is behind p0
    */
   if (Z(p0) INBACKOF Z(p1))
      {
      float *tmp = p0;
      p0 = p1;
      p1 = tmp;
      }
   
   if ( (Z(p0) INFRONTOF frontplane) && (Z(p0) != Z(p1)) )
      {
      float t = (frontplane - Z(p0)) / (Z(p1) - Z(p0));
      X(p0) = X(p0) + t * (X(p1) - X(p0));
      Y(p0) = Y(p0) + t * (Y(p1) - Y(p0));
      Z(p0) = Z(p0) + t * (Z(p1) - Z(p0));
      }
  
   if ( (Z(p1) INBACKOF backplane) && (Z(p0) != Z(p1)) )
      {
      float t = (backplane - Z(p0)) / (Z(p1) - Z(p0));
      X(p1) = X(p0) + t * (X(p1) - X(p0));
      Y(p1) = Y(p0) + t * (Y(p1) - Y(p0));
      Z(p1) = Z(p0) + t * (Z(p1) - Z(p0));
      }
  
   return SPW_TRUE;
} /* end of SPW_ClipZ */

/*-----------------------------------------------------------------------------

  Function:  DrawObject

  Description:
    Render an object to the screen.

  Args:
    data          (r/o)  List of full data points
    uniq          (r/o)  Indecies of unique points
    num_elements  (r/o)  Number of points to draw
    world2eye     (r/o)  World-to-eye matrix
    eye2screen    (r/o)  Eye-to-screen matrix

-----------------------------------------------------------------------------*/

void DrawObject(data3d *data, uniq_st *uniq, int num_elements, 
           Matrix world2eye, Matrix eye2screen)
{

  int    i,                     /* Temporary and counter variables */
         ixyz[3], 
         last[3],
         pCount,
         drawit = 0;
   float tmp[4], 
         tmp2[4];

   float  front = -40.0f,           /* Known dimensions in Eye Space */
          back  = -3500.0f;
   data3d  *pdata = data; /* Pointers to data for indexing */
   uniq_st *puniq = uniq;

   /* 
    * Transform all vertices from world_space to eye_space 
    */

   for(puniq=uniq;puniq->porigxyz!=NULL; puniq++)
     {
       tmp[0] = (float) puniq->porigxyz[0];
       tmp[1] = (float) puniq->porigxyz[1];
       tmp[2] = (float) puniq->porigxyz[2];
       tmp[3] = 1.0f;
       SPW_MultFull14x44(puniq->xyz,tmp,world2eye);
    }

   tmp2[0] = pdata->puniq->xyz[0];
   tmp2[1] = pdata->puniq->xyz[1];
   tmp2[2] = pdata->puniq->xyz[2];
   tmp2[3] = pdata->puniq->xyz[3];
   pdata++;

   pCount = 0;
   for(i=num_elements-1;i>0;i--,pdata++)
     {
       tmp[0] = pdata->puniq->xyz[0];
       tmp[1] = pdata->puniq->xyz[1];
       tmp[2] = pdata->puniq->xyz[2];
       tmp[3] = pdata->puniq->xyz[3];

       drawit = 0;

       /* 
        * If point is to be drawn, clip it within the viewing pyramid 
        */

        if(pdata->draw) 
          drawit = SPW_ClipZ(tmp, tmp2, front, back);
         
       /* 
        *  Transform from eye_space to screen coordinates 
        */

       if(drawit) 
         {
           
           SPW_MultFull14x44(tmp,tmp,eye2screen);
           ixyz[0] = (int)tmp[0];
           ixyz[1] = (int)tmp[1];
           ixyz[2] = (int)tmp[2];
           
           SPW_MultFull14x44(tmp2,tmp2,eye2screen);
           last[0] = (int)tmp2[0];
           last[1] = (int)tmp2[1];
           last[2] = (int)tmp2[2];
           
#if defined(OS_WIN32)
           MoveToEx( Spw_hDC, last[0], last[1], NULL );
                   LineTo( Spw_hDC, ixyz[0], ixyz[1] );
#else
           Spw_drawSegments[pCount].x1 = last[0];
           Spw_drawSegments[pCount].y1 = last[1];
           Spw_drawSegments[pCount].x2 = ixyz[0];
           Spw_drawSegments[pCount++].y2 = ixyz[1];
#endif
         }

       tmp2[0] = pdata->puniq->xyz[0];
       tmp2[1] = pdata->puniq->xyz[1];
       tmp2[2] = pdata->puniq->xyz[2];
       tmp2[3] = pdata->puniq->xyz[3];

     } /* end for */

#if !defined(OS_WIN32)
  if (Spw_DoubleBuffer)
   XDrawSegments(Spw_Dpy, Spw_mainBuffer, Spw_Gc, Spw_drawSegments, pCount);
  else
   XDrawSegments(Spw_Dpy, Spw_mainWindow, Spw_Gc, Spw_drawSegments, pCount);

#endif

} /* end of DrawObject */

/*---------------------------------------------------------------------------

  Function:  SpwButtonMotionEvent

  Description:
     Called when the Spaceball generated a motion event

  Args:
     pEvent    (r/o)  Pointer to an SiSpwEvent struct

-----------------------------------------------------------------------------*/

void
SbMotionEvent(SiSpwEvent *pEvent)
{
  Matrix eye_to_world;    /* Eyespace to worldspace matrix */
  Matrix Rd;              /* Scratch rotation matrix       */
  float matrix_scale;     /* value to scale matrix by      */

  float sbdata[7];

  sbdata[0] = (float) pEvent->u.spwData.mData[SI_TX] / 4000.f;
  sbdata[1] = (float) pEvent->u.spwData.mData[SI_TY] / 4000.f;
  sbdata[2] = (float) pEvent->u.spwData.mData[SI_TZ] / 4000.f;
  sbdata[3] = (float) pEvent->u.spwData.mData[SI_RX] / 2000.f;
  sbdata[4] = (float) pEvent->u.spwData.mData[SI_RY] / 2000.f;
  sbdata[5] = (float) pEvent->u.spwData.mData[SI_RZ] / 2000.f;
  sbdata[6] = (float) pEvent->u.spwData.period;

  /*
   *  Filter the raw Spaceball data
   */
  if (Spw_SingleAxisFilter) 
    SPW_HighValuePassFilter(sbdata, 6);

  if (!Spw_TransAllowed)  
    sbdata[0] = sbdata[1] = sbdata[2] = 0.0f;
  if (!Spw_RotAllowed)
    sbdata[3] = sbdata[4] = sbdata[5] = 0.0f;
  if (!Spw_RollAllowed)
    sbdata[3] = sbdata[5] = 0.0f;
  
  /*
   * Scaling for good feel
   */

  sbdata[0] *= Spw_Sensitivity * 2e7f;
  sbdata[1] *= Spw_Sensitivity * 2e7f;
  sbdata[2] *= Spw_Sensitivity * 2e7f;
  sbdata[3] *= Spw_Sensitivity * 4e5f;
  sbdata[4] *= Spw_Sensitivity * 4e5f;
  sbdata[5] *= Spw_Sensitivity * 4e5f;

   matrix_scale = SPW_GetMatrixScale (Spw_World_to_eye);

   /* Rotations */
   sbdata[5] = -sbdata[5];
   SPW_ArbitraryAxisToMatrix (Rd, sbdata+3,
                              (float)(sbdata[6] * 1e-7 * matrix_scale));
   SPW_Mult44x44 (Spw_World_to_eye, Spw_World_to_eye,Rd);

   /* Translations */
   Spw_World_to_eye[3][0] -= (float)(sbdata[0]*sbdata[6] * 3e-7 * matrix_scale);
   Spw_World_to_eye[3][1] -= (float)(sbdata[1]*sbdata[6] * 3e-7 * matrix_scale);
   Spw_World_to_eye[3][2] += (float)(sbdata[2]*sbdata[6] * 3e-7 * matrix_scale);

  /* Invert world_to_eye */
  SPW_InvertMatrix(eye_to_world,Spw_World_to_eye);
  
  /* Check to see if we have tried to drive through ground */
  if (eye_to_world[3][1] < 1.0)
    {
    eye_to_world[3][1] = 1.0f;
    SPW_InvertMatrix(Spw_World_to_eye,eye_to_world); /* reinvert */
    }

  Spw_Redraw = SPW_TRUE;
  
} /* end of SpwMotionEvent */

/*-----------------------------------------------------------------------------

  Function:  SpwButtonPressEvent

  Description:
     Called when a Spaceball button is pressed

  Args:
    dpy         (r/o)  XDisplay pointer, or NULL
    button      (r/o)  Spaceball putton pressed
    pclientdata (r/w)  Application specific data

-----------------------------------------------------------------------------*/

void
SpwButtonPressEvent(SPW_WINPARAM dpy, int button, void *pclientdata)
{

  switch(button)
    {
    case SPW_TRANS: 
      if (Spw_TransAllowed = !Spw_TransAllowed) 
        SiBeep(devHdl,"cC");
      if (Spw_TransAllowed == FALSE && Spw_RotAllowed == FALSE)
        {
        Spw_RotAllowed = TRUE;
        SiBeep(devHdl,"cC");
        }
      break;
      
    case 2: 
      if (Spw_RotAllowed = !Spw_RotAllowed) 
        SiBeep(devHdl,"cC");

      if (Spw_TransAllowed == FALSE && Spw_RotAllowed == FALSE)
        {
        Spw_TransAllowed = TRUE;
        SiBeep(devHdl,"cC");

        }
      break;
      
    case SPW_SINGLE_AXIS: 
      if (Spw_SingleAxisFilter = !Spw_SingleAxisFilter) 
        SiBeep(devHdl,"cC");

      break;
      
    case 4: 
      if (Spw_RollAllowed = !Spw_RollAllowed) 
        SiBeep(devHdl,"cC");

      break;
      
    case 5: 
      if ((Spw_Sensitivity/2)>1.0/32.0) 
        {
          Spw_Sensitivity/=2; 
           SiBeep(devHdl,"cC");
        } 
      break;
      
    case 6: 
      if ((Spw_Sensitivity*2)<32.0)
        {
        Spw_Sensitivity*=2; 
        SiBeep(devHdl,"cC");

        } 
      break;
      
    case 7: 
      break;
      
    case 8: 
      SiBeep(devHdl,"cC");
      SiRezero(devHdl);
      break;
      
    case SPW_PICKBUTTON: 
      break;
      
    default:
      break;
    }

  SpwUpdateButtons();
  
} /* end of SpwButtonPressEvent */

/*-----------------------------------------------------------------------------

  Function:  SpwButtonReleaseEvent

  Description:
     Called when a Spaceball button is released

  Args:
    dpy         (r/o)  XDisplay pointer, or NULL
    button      (r/o)  Spacebal button numbert
    pclientdata (r/w)  Application specific data

-----------------------------------------------------------------------------*/

void
SpwButtonReleaseEvent(SPW_WINPARAM dpy, int button, void *pclientdata)
{

  switch(button)
    {
    case SPW_TRANS: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
        break;
    case 5: 
      break;
    case 6: 
      break;
    case 7: 
      break;
    case 8: 
      break;
    case SPW_PICKBUTTON: 
      SPW_MakeIdentityMatrix(Spw_World_to_eye); 
      Spw_World_to_eye[3][0] = XTRAN;
      Spw_World_to_eye[3][1] = YTRAN;
      Spw_World_to_eye[3][2] = ZTRAN;
      SpwUpdateButtons();
      SiBeep(devHdl,"cC");

      Spw_Redraw = TRUE;
      break;

    default:
      break;
      
    }

} /* end of SpwButtonReleaseEvent */

/*------------------------------------------------------------------------- 

   Function:  SbButtonEvent

   Called whenever one of the Spaceball buttons is pressed or released.
   This then calles the correct function for press or release (done this
   way so this can still compile under UNIX)

   Args:
     pEvent      (r/o)  Pointer to an SiSpwEvent struct

-----------------------------------------------------------------------------*/

void
SbButtonEvent(SiSpwEvent *pEvent)
{
  int button;

  if ((button = SiButtonReleased (pEvent)) != SI_NO_BUTTON)
    SpwButtonReleaseEvent((short)NULL, button, NULL);

  if ((button = SiButtonPressed (pEvent)) != SI_NO_BUTTON)
    SpwButtonPressEvent((short)NULL, button, NULL);

}

/*-----------------------------------------------------------------------------

  Function:  GetUniqPoints

  Description:
   Make an array of the unique entries in the data set.

  Args:
    local_data  (r/w)  Full list of data points
    local_uniq  (r/w)  Processed list of unique data points
    cnt         (r/o)  Number of entries

-----------------------------------------------------------------------------*/

void GetUniqPoints( data3d *local_data, uniq_st *local_uniq, 
                   int cnt )
{
  data3d  *pdata;
  uniq_st *puniq;
  int i;
  
  /* Gather together the unique entries */
  for(pdata=local_data,i=cnt;i>0;--i,++pdata)
    {
      
      /* Check for earlier occurrence of this point */
      for(puniq=local_uniq; puniq->porigxyz!=NULL; ++puniq)
        {
          if (pdata->xyz[0] == puniq->porigxyz[0]
              && pdata->xyz[1] == puniq->porigxyz[1]
              && pdata->xyz[2] == puniq->porigxyz[2])
            break;
        }
      if (puniq->porigxyz == NULL) /* not in uniq, add ptr */
        puniq->porigxyz = pdata->xyz;
      pdata->puniq = puniq; /* pts to new or an earlier occurrence */
    }
} /* end GetUniqPoints */

/* end of sbXcityfly.c */
