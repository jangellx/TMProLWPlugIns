/*
 *  winmain.c - NT specific routines for Widget World demo
 *
 *  Written:     March 1996
 *  Author:      Jeff LeBlanc
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
 *
 ----------------------------------------------------------------------------- */

static char rcsid[] = "$Id: winmain.c,v 1.9.2.1.2.1 1998/05/26 11:04:26 equerze Exp $";

/* Include files */

#include <stdio.h>
#include <math.h>

#include <windows.h>
#include <float.h>
#include <stdlib.h>
#include "spwmacro.h"
#include "si.h"
#include "spwslim.h"

#include "widget.h"

/*
 *  Typdefs and macros to aid in windowing-system independence (NT version)
 */

typedef int SPW_WINPARAM;
typedef HWND SPW_WINDOW;
typedef int SPW_RETURN;

/* Arrays for object data */
extern PointStr   *Spw_Points; 
extern EdgeStr    *Spw_Edges;
extern PolygonStr *Spw_Polygons;

extern int Spw_NPoints,                /* Total number of Points   */
           Spw_NEdges,                 /* Total number of Edges    */
           Spw_NPolygons;              /* Total number of Polygons */

extern PointStr *Spw_DrawPoints;        /* Scratch array of points */
extern int       Spw_WindowWidth, 
                 Spw_WindowHeight;      /* Size of window          */

/*
 *  Windowing-system specific globals (those I couldn't easily avoid)
 */

extern HPEN        Spw_Black, Spw_White;
extern int         ncmdshow;
extern HWND        hWndMain;
extern HDC         hdc;
extern PAINTSTRUCT ps;
extern HPEN        Spw_Foreground,
                   Spw_Background;

int DispatchLoopNT(SiSpwHandlers *DspHandlers);
void GetNTFileName( HWND hWnd, char *filename );
void InitializeSpaceball(SPW_WINPARAM dpy, 
                         SPW_WINDOW win,
                         SiSpwHandlers *DspHandlers,
                         SpaceballControlStruct *scs);

/*-----------------------------------------------------------------------------

  Function:  WinMain

  Description:
    This is the main Windows routine.

  Args:   
    hInstance     (r/o) 
    hPrevInstance (r/o)
    lpszCmdLine   (r/o)  String with the command line
    nCmdShow      (r/o)

-----------------------------------------------------------------------------*/

int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpszCmdLine, int nCmdShow )
{  

  SpaceballControlStruct scs;
  SpaceballAppDataStruct app;
  PointStr World[2];
  char fname[256];
  float CTM[4][4];             /* Current transformation matrix */ 
  SiSpwHandlers DspHandlers;
  LONG addr = (long)&scs;
  HWND wndScreen;
  RECT screenRect;
  int size;
  DWORD dSizeNeeded;
  char appDir[256];
  //int ssize;
  //char DirName[256];
  ncmdshow = nCmdShow;
  //FILE *fp;
  /*
   *   Figure out how big the screen is so that we can make our
   *   window an appropriate size.
   */
  wndScreen = GetDesktopWindow();
  GetWindowRect ( wndScreen, &screenRect );
  size = (int)(.8 * SPW_MIN( (screenRect.bottom - screenRect.top),
                   (screenRect.right - screenRect.left) ));
  Spw_WindowWidth = size;   
  Spw_WindowHeight = size;
   
  /* Add code to parse command line for alternate file --here-- */

  /* Code that finds the default data file in its usual spot */
  
 // if (0 != (dSizeNeeded = GetCurrentDirectory(256, appDir)))
  if (0 != (dSizeNeeded = GetModuleFileName(NULL, appDir, 256 )))
     {
     char widgetUStr[] = "WIDGET\0";
     char widgetLStr[] = "widget\0";
     char DirName[256];
     char *pdest;
     int  result;
     FILE *fp;
     

     /* find "widget" in directory string */
     pdest = strstr( appDir, widgetUStr );
	 if (pdest == NULL)
	    {
        pdest = strstr( appDir, widgetLStr );
        }
	 if (pdest != NULL)
	    {
        /* get the index of the end of "widget" */
        result = pdest - appDir + strlen(widgetUStr);
        /* snip everything after "widget" */
        appDir[result] = '\0'; 
        /* build the file name */
        strcpy(DirName, appDir);
        strcat(DirName, "\\dat\\widget.dat");
        }
     /* see if we can open the file */
     if (NULL != (fp=fopen(DirName, "r")))
        {
        fclose(fp);              /* close it back up */
        strcpy(fname, DirName);  /* save the file name */
        }
     else /* file is in a non standard location; let user find it */
        {
        strcpy(fname, "*.dat");
        GetNTFileName( NULL, fname );
        }
     }

  /*
   *  Initialize the Spaceball data structures
   */

  /* Spaceball tuning values */

  scs.sbScaleFactor[0] = (float)0.05;
  scs.sbScaleFactor[1] = (float)0.05;
  scs.sbScaleFactor[2] = (float)0.005;
  scs.sbScaleFactor[3] = (float)0.0005;
  scs.sbScaleFactor[4] = (float)0.0005;
  scs.sbScaleFactor[5] = (float)0.0005;

  /* Initialize platform-independent data */

  MainInitialize (fname, &scs, &app, (float *)CTM, World);
  MoveWindow(hWndMain, 300, 0, Spw_WindowWidth, Spw_WindowHeight, TRUE);

  SetClassLong(hWndMain, 0, (LONG)(&scs));
 
  /*
   *  Initialize the Spaceball, render initial scene
   */

  InitializeSpaceball(0, hWndMain, &DspHandlers, &scs);
  ResetWidgetView( (float(*)[4])app.CTM, app.center );
  app.redraw = TRUE;
  InvalidateRect(hWndMain, NULL, FALSE);

  return(DispatchLoopNT(&DspHandlers));

} /* end of WinMain */

/*-----------------------------------------------------------------------------

  Function:  DispatchLoopNT

  Description:
    Main loop for the application.  This gets and translates all event
    and/or messages coming into the app.

  Args:
    spw_dispatch (r/w)  Pointer to the InputDispatch structure

-----------------------------------------------------------------------------*/

int DispatchLoopNT(SiSpwHandlers *DspHandlers)
{

   SiGetEventData EData;
   SiSpwEvent Event;
   MSG msg;
   BOOL handledEvent = FALSE;
   SpaceballControlStruct *scs = (SpaceballControlStruct *)
                                 (DspHandlers->motion.data);
  SpaceballAppDataStruct *ads = (SpaceballAppDataStruct *)(scs->appData);

  while ( GetMessage( &msg, NULL, 0, 0 ) )
    {
      handledEvent = FALSE;
      if (scs->devHdl)
         {
         SiGetEventWinInit (&EData, msg.message, msg.wParam, msg.lParam);
         if (SiGetEvent (scs->devHdl, 0, &EData, &Event) == SI_IS_EVENT)
            {
            SiDispatch (scs->devHdl, &EData, &Event, DspHandlers);
            handledEvent = TRUE;
            }
         }
      if (handledEvent == FALSE)
        {
          TranslateMessage( &msg );
          DispatchMessage( &msg );
        }

      if (ads->redraw == 1) 
         InvalidateRect(hWndMain, NULL, FALSE);
    }
  return( msg.wParam );
  
} /* end of DispatchLoopNT */

/*-----------------------------------------------------------------------------

  Function:  HandleNTEvent

  Description:
    Handle any windowing system events.  Control data is passed in using
    an address in the two extra words retrieved by GetClassLong (rather
    than just using a global variable, which is generally bad programming
    practice).  I really wish I could pass this in as a simple parameter...

  Args:
    hWnd   (r/o)  Handle to the window
    msg    (r/o)  
    wParam (r/o)  
    lParam (r/o)  

-----------------------------------------------------------------------------*/

long WINAPI HandleNTEvent ( HWND hWnd, unsigned msg, WPARAM wParam,
                           LPARAM lParam )

{

  LONG addr = GetClassLong(hWnd, 0);
  SpaceballControlStruct *scs;
  SpaceballAppDataStruct *ads;
  float *appCTM; 

  /*
   *  Make sure the SpaceballControlStruct has been initialized
   *  before we try to access it, since WindowCreate will cause this
   *  routine to get called before we set the correct address.
   */

  if (addr)
    {
    scs = (SpaceballControlStruct *)addr;
    ads = (SpaceballAppDataStruct *)(scs->appData);
    appCTM = (ads->CTM); 
    }

  switch ( msg )
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
      if (wParam == VK_ESCAPE)
        SendMessage ( hWndMain, WM_CLOSE, 0, 0l );
      break;
      
    case WM_PAINT:
      if (addr)
         {
         hdc = BeginPaint ( hWndMain, &ps ); 
         Render( (float(*)[4])appCTM, ads->center);
         EndPaint ( hWndMain, &ps );
         ads->redraw = FALSE;
         }

      break;
      
    case WM_CLOSE:
      /* cleanup the object info created */
      DeleteObject ( Spw_White );
      DeleteObject ( Spw_Black  );
      break;
      
    case WM_DESTROY :
      PostQuitMessage (0);
      return (0);
    }
  return DefWindowProc ( hWnd, msg, wParam, lParam );

} /* end of HandleNTEvent */

