/*  ntgraph.c - Library of NT graphics wrapper functions
 *
 *  Written:     February 1996
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
 *  Library of graphics routines to separate an application developer from
 *  the platform-specific graphics calls.  The routines in this file
 *  perform the Windows (NT or 95) specific calls.
 *
 ----------------------------------------------------------------------------- */

/*
 *  Evil Globals
 */
#include <windows.h>
#include <float.h>
#include <stdlib.h>
#include "spwmacro.h"
#include "si.h"
#include "spwslim.h"


HANDLE hInst;
HWND hWndMain;
HDC hdc;
PAINTSTRUCT ps;
int ncmdshow;
HPEN Spw_Black, Spw_White;

long WINAPI HandleNTEvent ( HWND hWnd, unsigned msg, WPARAM wParam,
			                LPARAM lParam );

/*-----------------------------------------------------------------------------

 Function: CreateWindow

 Description:
   Routine to open a graphics window and set up the colormap.
 
 Args:
   atx       (r/o)  X-coord origin of window
   aty       (r/o)  Y-coord origin of window
   wid       (r/o)  Width of window
   hi        (r/o)  Height of window
   string    (r/o)  Text quote at top of window

-----------------------------------------------------------------------------*/

void CreateWidgetWindow (int atx, int aty, int hi, int wid, char *string)
/* 
int atx, aty;        Screen coordinates to open window at
int wid, hi;         width and height of window
*/
{
   WNDCLASS wndclass;

   /* Register display window class */
   wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
   wndclass.lpfnWndProc   = HandleNTEvent ;
   wndclass.cbClsExtra    = 8 ;
   wndclass.cbWndExtra    = 0 ;
   wndclass.hInstance     = hInst;
   wndclass.hIcon         = LoadIcon( wndclass.hInstance, "WidgetIcon") ;
   wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
   wndclass.hbrBackground = GetStockObject(BLACK_BRUSH) ;
   wndclass.lpszMenuName  = NULL ;
   wndclass.lpszClassName = "WidgetWorld";

   RegisterClass (&wndclass) ;

   /* create the window */
   hWndMain = CreateWindow ( "WidgetWorld",          // Window class name
                            string,                  // Window caption
			                   WS_OVERLAPPEDWINDOW ,    // Window Style
                            atx,aty,wid,hi,
                            NULL,	             // parent window handle
			                   NULL,		     // window menu handle
			                   hInst,		     // program instance handle
			                   NULL) ;		     // creation parameters

   /*
    *   Create colored pens
    */
   Spw_White = CreatePen ( PS_SOLID, 1, RGB(255, 255, 255) );
   Spw_Black = CreatePen ( PS_SOLID, 1, RGB(0, 0, 0) );

   /* display the window */
   ShowWindow ( hWndMain, ncmdshow );
   UpdateWindow ( hWndMain );

} /* end of CreateWindow */

/*-----------------------------------------------------------------------------

 Function: SetColor

 Description:
   Set the current foreground drawing color

 Args:
   color   (r/o)  Color to set to

-----------------------------------------------------------------------------*/

void SetColor(HPEN color)
{

  SelectObject(hdc, color);

} /* end of SetColor */

/*-----------------------------------------------------------------------------

 Function: DrawLine

 Description:
   Routine to draw a line from (x1, y1) to (x2, y2) in the current forground
   color.

 Args:
   x1    (r/o) X-Coord of first point
   y1    (r/o) Y-Coord of first point
   x2    (r/o) X-Coord of second point
   y2    (r/o) Y-Coord of second point

-----------------------------------------------------------------------------*/

void DrawLine(int x1, int y1, int x2, int y2)
{

  MoveToEx(hdc, x1, y1,NULL);
  LineTo(hdc, x2, y2);

} /* end of DrawLine */

/*-----------------------------------------------------------------------------

 Function:  DrawLineColor

 Description:
   Routine to draw a line from (x1, y1) to (x2, y2) in a given color

 Args:
   x1    (r/o) X-Coord of first point
   y1    (r/o) Y-Coord of first point
   x2    (r/o) X-Coord of second point
   y2    (r/o) Y-Coord of second point
   color (r/o) Color index to draw in

-----------------------------------------------------------------------------*/

void DrawLineColor(int x1, int y1, int x2, int y2, HPEN color)
{

  HPEN hpen;

  hpen = SelectObject ( hdc, color );
  MoveToEx(hdc, x1, y1,NULL);
  LineTo(hdc, x2, y2);
  SelectObject(hdc, hpen);

} /* end of DrawLineColor */

/*---------------------------------------------------------------------------

    Function:  GetNTFileName
    
    The purpose of this routine is to read a filename from the user.
    Ideally this comes up as one of those touchy feely windows window.

    Args:  hWnd     (r/o)  The handle of our main window
           filename (r/w)  the place to put the filename

    Returns: nothing.  The string is returned in filename.
-----------------------------------------------------------------------------*/
void
GetNTFileName( HWND hWnd, char *filename )
{
   OPENFILENAME ofn;
   static char *szFilter[] = { "Datafiles (*.dat)", "*.dat",
                               "All files (*.*)",    "*.*",
                               "" };

   ofn.lStructSize       = sizeof( OPENFILENAME );
   ofn.hwndOwner         = hWnd;
   ofn.hInstance         = NULL;
   ofn.lpstrFilter       = szFilter[0];
   ofn.lpstrCustomFilter = NULL;
   ofn.nMaxCustFilter    = 0;
   ofn.nFilterIndex      = 0;
   ofn.lpstrFile         = filename;
   ofn.nMaxFile          = _MAX_PATH;
   ofn.lpstrFileTitle    = "Open File";
   ofn.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT;
   ofn.lpstrInitialDir   = "..\\dat";
   ofn.lpstrTitle        = NULL;
   ofn.Flags             = OFN_CREATEPROMPT;
   ofn.nFileOffset       = 0;
   ofn.nFileExtension    = 0;
   ofn.lpstrDefExt       = "dat";
   ofn.lCustData         = 0L;
   ofn.lpfnHook          = NULL;
   ofn.lpTemplateName    = NULL;
   GetOpenFileName( &ofn );
}

/*----------------------------------------------------------------------------

   Function:  DrawFilledBox

   Description:
     Routine to draw a filled rectangle

   Args:
    x      (r/o)  X-Coord origin of box
    y      (r/o)  Y-Coord origin of box
    width  (r/o)  Width of box
    height (r/o)  Height of box
    color  (r/o)  Color to fill in

-----------------------------------------------------------------------------*/

void DrawFilledBox(int x, int y, int width, int height, HPEN color)
{

    HPEN old;
    old = SelectObject ( hdc, GetStockObject( (int)color) );
    Rectangle( hdc, x, y, width, height);
    SelectObject(hdc,old);

}
