/*  widget.c - main block of code for Widget World demo
 *
 *  Written:     January 1996
 *  Author:      Jeff LeBlanc
 *
 * $Id: widget.c,v 1.12.2.1.2.1 1998/05/26 18:23:42 equerze Exp $
 *
 * (C) 1998 Spacetec IMC Corporation (Spacetec). All rights reserved. 
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby grated provided that this copyright
 * notice appears in all copies.  Permission to modify this software is granted
 * and Spacetec will support such modifications only is said modifications are
 * approved by Spacetec.
 *
 -----------------------------------------------------------------------------
 *  Program to illustrate some basic commands for
 *  graphics using XWindows or Windows, and how to
 *  integrate a Spaceball into such an application.  This program 
 *  reads in the object data file and renders a wireframe.
 *  All the OS independent routines are in this file.
 *
 ----------------------------------------------------------------------------- */

static char rcsid[] = "$Id: widget.c,v 1.12.2.1.2.1 1998/05/26 18:23:42 equerze Exp $";

/* Include files */

#include <stdio.h>
#include <math.h>

#ifdef OS_WIN32

#include <windows.h>
#include <float.h>
#include <stdlib.h>
#include "spwmacro.h"
#include "si.h"
#include "spwslim.h"

/*
 *  Typdefs and macros to aid in windowing-system independence (NT version)
 */

typedef int SPW_WINPARAM;
typedef HWND SPW_WINDOW;
typedef int SPW_RETURN;

#define SPW_InputRezero(PARAMS) SPW_InputReZero(PARAMS)

#else

/*
 *  X11 specific includes and definitions
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <keysym.h>
#include <x11input.h>
#include <spwslim.h>

#endif

#include "widget.h"

/*
 *  Typedefs to aid in windowing-system independence 
 */

#ifdef OS_WIN32

typedef int SPW_WINPARAM;
typedef HWND SPW_WINDOW;
typedef int SPW_RETURN;

#else
typedef Display *  SPW_WINPARAM;
typedef void       SPW_RETURN;
typedef Window     SPW_WINDOW;

#endif

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

/*
 *  Function prototypes
 */

void CreateWidgetWindow(int, int, int, int, char *);

/*
 *  Evil global variables
 *
 *  The following data pieces are left global since we don't want to make
 *  any assumptions about how the applications programmer is going to
 *  interface their data with the Spaceball routines.  The current
 *  transformation matrix (CTM) is passed around with the Spaceball
 *  specific data in a pointer to a non-global structure; this data could
 *  just as easily be passed around in a similar fashion.
 */

/* Arrays for object data */
PointStr   *Spw_Points; 
EdgeStr    *Spw_Edges;
PolygonStr *Spw_Polygons;

int Spw_NPoints,                /* Total number of Points   */
    Spw_NEdges,                 /* Total number of Edges    */
    Spw_NPolygons;              /* Total number of Polygons */

PointStr *Spw_DrawPoints;        /* Scratch array of points */
int       Spw_WindowWidth, 
          Spw_WindowHeight;      /* Size of window          */

/*
 *  Windowing-system specific globals (those I couldn't easily avoid)
 */

#ifdef OS_WIN32

HPEN        Spw_Black, Spw_White;
HWND        hWndMain;
HDC         hdc;
PAINTSTRUCT ps;
HPEN        Spw_Foreground,
            Spw_Background;

#else /* X Windows */

extern Display *Spw_Dpy;
extern Window   Spw_MainWin;
extern int      Spw_White, Spw_Black;

int             Spw_Foreground,
                Spw_Background;
#endif

/*-----------------------------------------------------------------------------
 *
 * void SPW_MakeTranslationMatrix( Matrix result, float trans[3] )
 *
 *  Args: 
 *    result (w/o) -matrix that represents a translation
 *    trans  (r/o) -translation vector matrix is to be made with
 *
 *  Return Value:
 *    None
 *
 *  Description:
 *    Make a standard right handed translation matrix.
 *       1  0  0  0
 *       0  1  0  0
 *       0  0  1  0
 *       X  Y  Z  1
 *
 *---------------------------------------------------------------------------*/
void
SPW_MakeTranslationMatrix( Matrix result, float trans[3] )
{
   SPW_MakeIdentityMatrix (result);
   result[3][0] = trans[0];
   result[3][1] = trans[1];
   result[3][2] = trans[2];
} /* end of SPW_MakeTranslationMatrix */
       
/*----------------------------------------------------------------------------

  Function:  ReadDataFile

  Description:
    Routine to read in an file containing the points, edges, and polygon 
    data to define the objects(s) to be rendered.

  Args:  
    fname     (r/o)  Name of file to read scene from
    Points    (r/w)  List of points
    Edges     (r/w)  List of edges
    Polygons  (r/w)  List of polygons
    NPoints   (r/w)  Number of points
    NEdges    (r/w)  Number of edges
    NPolygons (r/w)  Number of polygons
    World     (r/w)  Boundaries of your world

-----------------------------------------------------------------------------*/

void ReadDataFile(char *fname, PointStr **Points, EdgeStr **Edges,
              PolygonStr **Polygons, int *NPoints, int *NEdges,
              int *NPolygons, PointStr World[2])
{
  FILE *fp;       /* File pointer */
  int i, j;       /* Counter      */
  
  /*
   *  Open file for reading 
   */
  
  if ((fp = fopen(fname, "r")) == NULL) 
    {
      printf("error opening file %s.  program terminated.\n", fname);
      exit(-1);
    }
  
  /* 
   * Read in the World Boundaries
   */

  World[0].Coord[0] = World[0].Coord[1] = (float)0.0;
  World[1].Coord[0] = World[1].Coord[1] = (float)0.0;
  
  /*
   * Read in the size of the arrays 
   */

  fscanf (fp, "%d %d %d", NPoints, NEdges, NPolygons);
  
  /*
   * Dynamically allocate data storage space 
   */

  if ((*Points = (PointStr *) malloc (*NPoints * sizeof(PointStr))) == NULL) 
    {
      printf("Couldn't malloc Point array to size %d\n", *NPoints);
      exit(-1);
    }
  
  if ((*Edges = (EdgeStr *) malloc (*NEdges * sizeof(EdgeStr))) == NULL) 
    {
      printf("Couldn't malloc Edge array to size %d\n", *NEdges);
      exit(-1);
    }

  if ((*Polygons = (PolygonStr *) malloc 
       (*NPolygons * sizeof(PolygonStr))) == NULL) 
    {
      printf("Couldn't malloc Polygon array to size %d\n", *NPolygons);
      exit(-1);
    }
  
  /*
   * Read in the list of Points 
   */

  for (i=0; i < *NPoints; i++) 
    {
      fscanf(fp, "%f %f %f",  &((*Points)[i].Coord[0]), 
             &((*Points)[i].Coord[1]),
             &((*Points)[i].Coord[2]) );

      if ( (*Points)[i].Coord[0] < World[0].Coord[0])
        World[0].Coord[0] = (*Points)[i].Coord[0];
      if ( (*Points)[i].Coord[1] < World[0].Coord[1])
        World[0].Coord[1] = (*Points)[i].Coord[1];

      if ( (*Points)[i].Coord[0] > World[1].Coord[0])
        World[1].Coord[0] = (*Points)[i].Coord[0];
      if ( (*Points)[i].Coord[1] > World[1].Coord[1])
        World[1].Coord[1] = (*Points)[i].Coord[1];

    }
  
  /*
   * Read in the list of Edges 
   */

  for (i=0; i < *NEdges; i++) {
    fscanf (fp, "%d %d", &((*Edges)[i].Start), &((*Edges)[i].End) );
  }
  
  /*
   * Read in the list of Polygons 
   */

  for (i=0; i < *NPolygons; i++) 
    { 
      fscanf (fp, "%d", &((*Polygons)[i].Edges));
      fscanf (fp, "%d", &((*Polygons)[i].Color));  /* Not currently used */
      
      for (j=0; j < (*Polygons)[i].Edges; j++)
        fscanf(fp, "%d", &((*Polygons)[i].EList[j]));
    }
  
  /*
   * When done, close the file 
   */

  fclose(fp);
  
} /* end ReadDataFile */

/*-----------------------------------------------------------------------------

  Function:  SbMotionEvent

  Description:
    Take the Spaceball motion data and create the new transformation matrix
    to display the geometry with.  This is a callback from SiDispatch.

  Args:
     SiOpenData     pOData (r/w)  Good question.....
     SiGetEventData pEData (r/w)  Good question.....
     SiSpwEvent     pEvent (r/w)  SpaceWare event
     void           pUData (r/w)  User data

-----------------------------------------------------------------------------*/

int SbMotionEvent(SiOpenData *pOData,
  SiGetEventData *pEData, SiSpwEvent *pEvent, void *pUData)
{

   Matrix tmpCTM, tmp2;
   int i;
   float tmpTrans[4];
   float trans[4];
   float zoom, tZoom;
   float data[6];
  
   SpaceballControlStruct *scs = (SpaceballControlStruct *)pUData;
   SpaceballAppDataStruct *ads = (SpaceballAppDataStruct *)(scs->appData);
   float *appCTM = (ads->CTM);
   
   data[0] = (float) pEvent->u.spwData.mData[SI_TX];
   data[1] = (float) pEvent->u.spwData.mData[SI_TY];
   data[2] = (float) pEvent->u.spwData.mData[SI_TZ];
   data[3] = (float) pEvent->u.spwData.mData[SI_RX];
   data[4] = (float) pEvent->u.spwData.mData[SI_RY];
   data[5] = (float) pEvent->u.spwData.mData[SI_RZ];

  /*
   *  Apply Spaceball filtering to raw data
   */

  if (scs->sbDominant)
    SPW_HighValuePassFilter(data, 6);
  
  if (!scs->sbAllowTranslation)
    {
      data[0] = (float)0.0;
      data[1] = (float)0.0;
      data[2] = (float)0.0;
    }

  if (!scs->sbAllowRotation)
    {
      data[3] = (float)0.0;
      data[4] = (float)0.0;
      data[5] = (float)0.0;
    }

  /*
   *  Scale Spaceball data to get a good feel with the app
   */

  for (i=0; i < 6; i++)
    data[i] *= (scs->sbScale * scs->sbScaleFactor[i]);

  /*
   *  Put Spaceball data into app coordinate space
   */

  data[1] *= -1;
  data[4] *= -1;
  data[5] *= -1;

  /*
   *  Translate back from center of rotation to origin, storing 
   *  initial location for later use
   */

  tmpTrans[0] = ads->centerRot.Coord[0];
  tmpTrans[1] = ads->centerRot.Coord[1];
  tmpTrans[2] = (float)0.0;
  trans[0] = -1 * tmpTrans[0];
  trans[1] = -1 * tmpTrans[1];
  trans[2] = (float)0.0;
  trans[3] = (float)1.0;

  SPW_MakeTranslationMatrix(tmpCTM, trans);

  /*
   *  Perform Spaceball rotation
   */

  SPW_ArbitraryAxisToMatrix(tmp2, data+3, (float)1.0);
  SPW_Mult44x44(tmpCTM, tmpCTM, tmp2);

  /*
   *  Perform zoom, but keep in view
   */

  tZoom = SPW_GetMatrixScale((float(*)[4])appCTM);
  if (data[2] > 0.0)
    {
      zoom = (float)pow((double)(1.0/(1.0+ 0.01)), (double)(data[2]));
      if (tZoom > 0.05)
        SPW_ScaleMatrix(tmpCTM, tmpCTM, zoom);
    }
  else
    {
      zoom = (float)pow((double)(1.0+0.01), (double)(-data[2]));
      if (tZoom < 6.0)
        SPW_ScaleMatrix(tmpCTM, tmpCTM, zoom);
    }


  /*
   * Translate back to original place
   */

  SPW_MakeTranslationMatrix(tmp2, tmpTrans);
  SPW_Mult44x44( tmpCTM, tmpCTM, tmp2);

  /*
   *  Do Spaceball translation
   */
  trans[0] = data[0];
  trans[1] = data[1];
  trans[2] = (float)0.0;
  SPW_MakeTranslationMatrix( tmp2, trans);
  SPW_Mult44x44( tmpCTM, tmpCTM, tmp2);
  
  SPW_Mult44x44((float(*)[4])appCTM, (float(*)[4])appCTM, tmpCTM);

  /*
   *  Update the center of rotation
   */

  ads->centerRot.Coord[0] += data[0];
  ads->centerRot.Coord[1] += data[1];

  /*
   *  Set flag to redraw
   */

    ads->redraw = TRUE;
    return(0);
}

/*-----------------------------------------------------------------------------

 Function: SbButtonPressEvent

 Description:
   Routine to handle presses of the Spaceball buttons.  Called from
   SPW_InputDispatch.

 Args:  
   button  (r/o)  Spaceball button number
   appdata (r/w)  Application specific data

-----------------------------------------------------------------------------*/

void
SbButtonPressEvent(int button, void *appdata)
{
  SpaceballControlStruct *scs = (SpaceballControlStruct *)appdata;

  /*
   *  C - 3/16 sec delay
   *  c - 3/16 sec tone
   *  This is done in case the server should bunch our beep events
   *  together, we'll still get our beeps.
   */

  static char *BeepString = "CcC";

  switch(button) 
    {
    case 1:
      scs->sbAllowTranslation = !scs->sbAllowTranslation;
      if (scs->sbAllowTranslation == TRUE)
         {
         SiBeep(scs->devHdl, BeepString);
         }
      /* We don't want both trans and rotate off at the same time! */
      if (scs->sbAllowRotation == FALSE && scs->sbAllowRotation == FALSE)
         {
         scs->sbAllowRotation = TRUE;
         }
      break;

    case 2:
      scs->sbAllowRotation = !scs->sbAllowRotation;
      if (scs->sbAllowRotation == TRUE)
         {
         SiBeep(scs->devHdl, BeepString);
         }
      /* We don't want both trans and rotate off at the same time! */
      if (scs->sbAllowRotation == FALSE && scs->sbAllowRotation == FALSE)
         {
         scs->sbAllowTranslation = TRUE;
         }
      break;

    case 3:
      scs->sbDominant = !scs->sbDominant;
      if (scs->sbDominant == TRUE)
         SiBeep(scs->devHdl, BeepString);
      break;

    case 5:
      scs->sbScale /= (float)2.0;
      SiBeep(scs->devHdl, BeepString);
      break;

    case 6:
      scs->sbScale *= (float)2.0;
      SiBeep(scs->devHdl, BeepString);
      break;

    case 7:
      scs->sbScale = (float)1.0;
      scs->sbDominant = FALSE;
      scs->sbAllowTranslation = TRUE;
      scs->sbAllowRotation = TRUE;
      SiBeep(scs->devHdl, BeepString);
      break;

    case 8:
        SiRezero(scs->devHdl);
        SiBeep(scs->devHdl, BeepString);
      break;

    default:
      break;
    }
}

/*-----------------------------------------------------------------------------

 Function: SbButtonReleaseEvent

 Description:
   Routine to handle release of the Spaceball buttons.  Called from
   SPW_InputDispatch

 Args:  
   button  (r/o)  Spaceball button number
   appdata (r/w)  Application specific data

-----------------------------------------------------------------------------*/

void SbButtonReleaseEvent(int button, void *appdata)
{

  SpaceballControlStruct *scs = (SpaceballControlStruct *)appdata;
  SpaceballAppDataStruct *ads = (SpaceballAppDataStruct *)(scs->appData);
  static char *BeepString = "CcC";

  switch(button) 
    {
    case SI_PICK_BUTTON:
      {
        /*
         *  Reset view and center of rotation.
         *
         *  We do the "pick" button reset here, on release, so we
         *  get less motion artifact, since the button is on the 
         *  ball itself for the Spaceball 2003.
         */

        float *appCTM = (ads->CTM);
        ResetWidgetView( (float(*)[4])appCTM, ads->center);
        SiBeep(scs->devHdl, BeepString);
        ads->redraw = TRUE;

        ads->centerRot.Coord[0] = Spw_WindowWidth / (float)2.0;
        ads->centerRot.Coord[1] = Spw_WindowHeight / (float)2.0;
      }
      break;

    default:
      break;
    }

} /* end of SbButtonReleaseEvent */

/*-----------------------------------------------------------------------------

   Function: SbButtonEvent

   Description:
     Called when a Spaceball button is pressed of release.  We parse the
     event and call a helper routine for a press or a release

   Args:
     SiOpenData     pOData (r/w)  Good question.....
     SiGetEventData pEData (r/w)  Good question.....
     SiSpwEvent     pEvent (r/w)  SpaceWare event
     void           pUData (r/w)  User data

-----------------------------------------------------------------------------*/

int
SbButtonEvent(SiOpenData *pOData, SiGetEventData *pEData,
              SiSpwEvent *pEvent, void *pUData)
{

  int button;

  if ((button = SiButtonReleased (pEvent)) != SI_NO_BUTTON)
    SbButtonReleaseEvent(button, pUData);

  if ((button = SiButtonPressed (pEvent)) != SI_NO_BUTTON)
    SbButtonPressEvent(button, pUData);

  return (0);

}

/*-----------------------------------------------------------------------------

  Function: InitializeSpaceball

  Description:
   Open connection to the Spaceball and initialize the callback structures
   used to get Spaceball events.

   Args:  
    dpy           (r/o)  X Display
    spw_dispatch  (r/w)  Spaceball callback structure
    scs           (r/o)  App-specific Spaceball data

-----------------------------------------------------------------------------*/

void InitializeSpaceball(SPW_WINPARAM dpy, 
                         SPW_WINDOW win,
                         SiSpwHandlers *DspHandlers,
                         SpaceballControlStruct *scs)
{
  
#ifdef OS_WIN32
   SiOpenData oData;
   if (SiInitialize() == SPW_DLL_LOAD_ERROR)  
     {
	  MessageBox(NULL,"Error: Could not load SiAppDll dll files",
		         NULL, MB_ICONEXCLAMATION);
	 }
   SiOpenWinInit (&oData, win);

   if (!(scs->devHdl = SiOpen ("Widget World", SI_ANY_DEVICE, SI_NO_MASK,
                             SI_EVENT, &oData)))

      {
      SiTerminate();
      MessageBox( win, "Sorry - No supported Spacetec IMC device available.\n",
                 NULL, MB_OK );
      if (win)
         DestroyWindow(win);
      
      ExitProcess(1);
   }
   SiSetUiMode(scs->devHdl, SI_UI_ALL_CONTROLS);

#else /* X11 */
  if (!SPW_InputCheckForSpaceball(dpy, win, "Widget World"))
    {
      printf ("Sorry - No Spaceball found.\n");
      exit(0);
    }
#endif

   DspHandlers->button.func    = SbButtonEvent;
   DspHandlers->motion.func    = SbMotionEvent;
   DspHandlers->zero.func      = SbMotionEvent;
   DspHandlers->exception.func = NULL;

   DspHandlers->button.data    = (void *)(scs);
   DspHandlers->motion.data    = (void *)(scs);
   DspHandlers->zero.data      = (void *)(scs);
   DspHandlers->exception.data = NULL;

#ifndef OS_WIN32
  spw_dispatch->display = dpy;
  spw_dispatch->spwbw   = NULL;
#endif

} /* end of InitializeSpaceball */

/*-----------------------------------------------------------------------------

  Function:  ResetWidgetView

  Description:
    This routine will reset the current transformation matrix to
    center the view.

    Args:  
      CTM    (r/o)  Current Transformation Matrix
      Center (r/o)  The center of the object.

-----------------------------------------------------------------------------*/

void
ResetWidgetView(Matrix CTM, PointStr Center)
{
  float tcent[4];
  tcent[0] = (Spw_WindowWidth / (float)2.0) - Center.Coord[0];
  tcent[1] = (Spw_WindowHeight / (float)2.0) - Center.Coord[1];
  tcent[2] = (float)0.0;
  tcent[3] = (float)1.0;

  SPW_MakeIdentityMatrix( (float(*)[4]) CTM);
  SPW_MakeTranslationMatrix((float(*)[4]) CTM, Center.Coord);
  
  SPW_MakeTranslationMatrix((float(*)[4]) CTM, tcent);

} /* end of ResetWidgetView */

/*-----------------------------------------------------------------------------

  Function:  Render

  Description:
    This routine will draw the wireframes of all object in the "world".  It
    does this by looping through the polygons and drawing each edge.  

    Args:  CTM  (r/o)  Current Transformation Matrix

-----------------------------------------------------------------------------*/

void Render(Matrix CTM, PointStr center)
{

  int i, j;
  int x = 0;
  int y = 1;
  float tvec[4];

  /* 
   *  Draw the initial blank screen 
   */

#ifdef OS_WIN32 
  DrawFilledBox(0, 0, Spw_WindowWidth, Spw_WindowHeight, 
                GetStockObject( WHITE_BRUSH ) );
#else
  DrawFilledBox(0, 0, Spw_WindowWidth, Spw_WindowHeight, Spw_Background);
#endif

  /*
   *  Transform and draw the center of rotation
   */

  tvec[0]=center.Coord[0];
  tvec[1]=center.Coord[1];
  tvec[2]=center.Coord[2];
  tvec[3]=(float)1.0;
  SPW_Mult14x44(tvec, tvec, CTM);
  Spw_DrawPoints[0].Coord[0] = tvec[0];
  Spw_DrawPoints[0].Coord[1] = tvec[1];
  Spw_DrawPoints[0].Coord[2] = tvec[2];

  SetColor(Spw_Foreground);
  DrawLine((int)(Spw_DrawPoints[0].Coord[0])-10,
           (int)(Spw_DrawPoints[0].Coord[1]),
           (int)(Spw_DrawPoints[0].Coord[0])+10,
           (int)(Spw_DrawPoints[0].Coord[1]));
  DrawLine((int)(Spw_DrawPoints[0].Coord[0]),
           (int)(Spw_DrawPoints[0].Coord[1])-10,
           (int)(Spw_DrawPoints[0].Coord[0]),
           (int)(Spw_DrawPoints[0].Coord[1])+10 );

  /*
   *  Store transformed points into drawing array
   */

  for (i=0; i < Spw_NPoints; i++)
    {
    tvec[0]=Spw_Points[i].Coord[0];
    tvec[1]=Spw_Points[i].Coord[1];
    tvec[2]=Spw_Points[i].Coord[2];
    tvec[3]=(float)1.0;
    SPW_Mult14x44(tvec, tvec, CTM);
    Spw_DrawPoints[i].Coord[0] = tvec[0];
    Spw_DrawPoints[i].Coord[1] = tvec[1];
    Spw_DrawPoints[i].Coord[2] = tvec[2];
  }

  /*
   * Draw the wireframe object 
   */

  SetColor(Spw_Foreground);
  for (j=0; j < Spw_NPolygons; j++)            /* Loop through the polygons */
    for (i=0; i < Spw_Polygons[j].Edges; i++){  /* Loop throught the edges   */
      /* Draw the points */
      DrawLine((int)Spw_DrawPoints[Spw_Edges[Spw_Polygons[j].EList[i]].Start].Coord[x], 
               (int)Spw_DrawPoints[Spw_Edges[Spw_Polygons[j].EList[i]].Start].Coord[y],
               (int)Spw_DrawPoints[Spw_Edges[Spw_Polygons[j].EList[i]].End].Coord[x], 
               (int)Spw_DrawPoints[Spw_Edges[Spw_Polygons[j].EList[i]].End].Coord[y]);
    }

} /* end of Render */

/*-----------------------------------------------------------------------------

  Function:  MainInitialize

  Description:
    Set up all the platform-independent data structures.

  Args:
    fname   (r/o)  File to read data from
    scs     (r/w)  Spaceball control flags
    app     (r/w)  App-specific data the Spaceball callbacks will use
    CTM     (r/o)  Pointer to viewing transformation matrix
    World   (r/w)  Points defining size of viewing scene

-----------------------------------------------------------------------------*/

void MainInitialize(char *fname, 
                    SpaceballControlStruct *scs,  
                    SpaceballAppDataStruct *app,
                    float *CTM, PointStr *World)
{

  /*
   *  Initialize the Spaceball data structures
   */

  scs->sbDominant = FALSE;
  scs->sbAllowTranslation = TRUE;
  scs->sbAllowRotation = TRUE;
  scs->sbScale = (float)1.0;
  scs->appData = (void *)(app);
  app->CTM = (float *)CTM; 
  app->redraw = TRUE;

  /* 
   * Read in the data file containing the objects 
   */

  ReadDataFile(fname, &Spw_Points, &Spw_Edges, &Spw_Polygons, 
               &Spw_NPoints, &Spw_NEdges, &Spw_NPolygons, World);

  /* 
   *  Create scratch array of drawing points 
   */

  if ((Spw_DrawPoints = (PointStr *) malloc 
       (Spw_NPoints * sizeof(PointStr))) == NULL)
 {
    printf ("Couldn't alloc Spw_DrawPoints arrar\n");
    exit(1);
  }
  
  /* 
   *  Translate the scene to the center of the window, set center
   *  of rotation
   */

  app->center.Coord[0] = 
    ((World[0].Coord[0] + World[1].Coord[0]) / (float)2.0);
   app->centerRot.Coord[1] = app->center.Coord[1] = 
    ((World[0].Coord[1] + World[1].Coord[1]) / (float)2.0);
   app->centerRot.Coord[2] = app->center.Coord[2] = (float)0.0;

  app->centerRot.Coord[0] = (Spw_WindowWidth / (float)2.0);
  app->centerRot.Coord[1] = (Spw_WindowHeight / (float)2.0);

  /* 
   *  Initialize the transformation matrix 
   */

  SPW_MakeIdentityMatrix((float(*)[4])CTM);
  SPW_MakeTranslationMatrix((float(*)[4])CTM, app->center.Coord);

  /* 
   *  Open the main drawing window, map window
   */
  
  CreateWidgetWindow(0, 0, Spw_WindowHeight, Spw_WindowWidth, "Widget World");
  Spw_Foreground = Spw_Black;
  Spw_Background = Spw_White;

} /* end of MainInitialize */

/* end of sbwidget.c */

