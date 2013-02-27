/* winmain.c - Win32 specific routines for cityfly
 *
 *  Written:     March 1996 (moved from sbXcityfly.c)
 *  Author:      Jeff LeBlanc
 *
 * (C) 1998 Spacetec IMC Corporation (Spacetec). All rights reserved. 
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby grated provided that this copyright
 * notice appears in all copies.  Permission to modify this software is granted
 * and Spacetec will support such modifications only is said modifications are
 * approved by Spacetec.
 *
 *
--------------------------- BEGINNING OF NOTES -------------------------------

Win32 specific routines for cityfly demo, first port to Si/UAPI

-----------------------------------------------------------------------------*/

static char rcsid[] = "$Id: winmain.c,v 1.17.2.1.2.1 1998/05/26 18:22:33 equerze Exp $";

/*-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*
 *  NT specific includes and definions
 */

#include <windows.h>
#include <float.h>
#include <stdlib.h>

#include "spwmacro.h"

#include "si.h"

#include "spwslim.h"
#include "resource.h"
#include "cityfly.h"

/* One API mistake */
#define SPW_InputRezero(dpy)  SPW_InputReZero(dpy)

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/* The number of positions in the scrollbar for sensitivity */
#define NUM_SCROLL_POS 9

/*
 *  Typedefs to aid in windowing-system independence
 */

typedef int SPW_WINPARAM;
typedef int SPW_RETURN;

/*
 *  NT windowing globals
 */

extern HPEN  Spw_white, Spw_black, Spw_grey, Spw_green;
extern HWND  Spw_hWndMain;
extern HWND  Spw_hWndButton;
extern HDC   Spw_hDCScreen;
extern HDC   Spw_hDC;
extern HDC Spw_hDCBitmap;
extern HBITMAP Spw_hBitmap;
extern short Spw_DeviceHandle;

/*
 *  Windowing and drawing
 */

extern Matrix Spw_Persp, Spw_World_to_eye;
extern int    Spw_WindowSize;
extern int    Spw_WindowWidth, Spw_WindowHeight;      /* Window geometry */

extern int    Spw_SizeBldg, Spw_SizeLand;
extern data3d bldg[], land[];
extern uniq_st buniq[], luniq[];

/*
 * Spaceball flags
 */

extern int   Spw_Redraw;
extern int   Spw_SingleAxisFilter,
             Spw_TransAllowed,
             Spw_RotAllowed,
             Spw_RollAllowed;
extern float Spw_Sensitivity;
extern int Spw_DoubleBuffer;

void  SbMotionEvent(SiSpwEvent *pEvent);
void  SbButtonEvent(SiSpwEvent *pEvent);

SiHdl devHdl = SI_NO_HANDLE;

/*---------------------------------------------------------------------------

  Function:  ComputeScrollPos

  Description:
    This function computes the scroll bar position based on the floating
    point sensitivity value and the number of scroll bar positions.  It
    assumes that the scrollbar positions start at zero and that each
    sensitivity value is 2* or /2 from each other. The number of positions
    must also be odd so that you have an even number of sensitivities on
    either side of 1.0.

  Args:
    sensitivity  (r/o)  The current sensitivity.
    numPos       (r/o)  How many positions are on the scrollbar

  Returns:  A number between 0 and numPos-1, representing the location for
            for the scroll bar.

-----------------------------------------------------------------------------*/

int
ComputeScrollPos( float sensitivity, int numPos )
{
   int posOne = numPos/2;  /* The postion of 1.0 in the scroll bar range */
   float testVal;          /* The current value to test against */
   int pos;                /* The current scroll position to check */

   if ( sensitivity < 1.0 )
      {
      pos = posOne;
      testVal = 1.0f;
      while ( ( sensitivity < testVal ) && ( pos > 0 ) )
         {
         testVal /= 2.0f;
         pos--;
         }
      }
   else
      {
      pos = posOne;
      testVal = 1.0f;
      while ( ( sensitivity > testVal ) && ( pos < (numPos-1) ) )
         {
         testVal *= 2.0f;
         pos++;
         } 
      }
   
   return( pos );
   
} /* end of ComputeScrollPos */

/*-----------------------------------------------------------------------------

  Function:  SpwUpdateButtons

  Description:
    This function updates the buttons, checkboxes and scroll bars
    in the button window using the current global flags.

  Args:  none

-----------------------------------------------------------------------------*/

void
SpwUpdateButtons()
{
    /* set value for the translation box */
   CheckDlgButton ( Spw_hWndButton, IDTRANS, Spw_TransAllowed );

   /* set value for the rotation box */
   CheckDlgButton ( Spw_hWndButton, IDROTAT, Spw_RotAllowed );

   /* set value for the single axis filter box */
   CheckDlgButton ( Spw_hWndButton, IDSAXIS, Spw_SingleAxisFilter );

   /* set value for the Keep in View box */
   CheckDlgButton ( Spw_hWndButton, IDROLL, Spw_RollAllowed );

   /* Setup the Scroll Bar */
   SetScrollRange ( GetDlgItem( Spw_hWndButton, SENSCRL ), SB_CTL,
                     0, NUM_SCROLL_POS-1, FALSE );
   SetScrollPos ( GetDlgItem( Spw_hWndButton, SENSCRL ), SB_CTL,
                 ComputeScrollPos( Spw_Sensitivity, NUM_SCROLL_POS ), TRUE );

} /* end of SpwUpdateButtons */

/*-------------------------------------------------------------------------

  Function:  ShowEventError

  Description:
    This takes the a return value from a SiGetEvent and checks it for
    any common errors.  If it finds any it displays an error message
    to the user.

  Args:
    SpwRetVal  ret  (r/o)  SpaceWare error code to display

-------------------------------------------------------------------------*/

void ShowEventError(enum SpwRetVal ret)
{
   if (ret != SI_SKIP_EVENT)
      {
      MessageBox (Spw_hWndMain, SpwErrorString (ret), NULL, MB_OK);
      }
}

/*-----------------------------------------------------------------------------

  Function:  DispatchLoop

  Description:
    This routine reads messages and dispatches them to the appropriate
    routines.  This is the WindowsNT version.

  Args:   None, gets messages from the queue.

  Returns: The message parameter that breaks the loop.

-----------------------------------------------------------------------------*/

int
DispatchLoop(void)
{
   MSG msg;
   int handledEvent;
   SiGetEventData EData;
   SiSpwEvent Event;

   /* Get and process messages */
   while ( GetMessage( &msg, NULL, 0, 0 ) )
      {
      enum SpwRetVal ret;
      
      handledEvent = FALSE;

      if (devHdl)
         {
         SiGetEventWinInit (&EData, msg.message, msg.wParam, msg.lParam);
         if ((ret = SiGetEvent (devHdl, 0, &EData, &Event)) == SI_IS_EVENT)
            {
            if (Event.type == SI_MOTION_EVENT)
               {
               SbMotionEvent(&Event);
               }
            if (Event.type == SI_BUTTON_EVENT)
               {
               SbButtonEvent(&Event);
               }
            handledEvent = TRUE;
            }

         /* If it is a SpaceWare event, check for common errors */

         else if (ret != SI_NOT_EVENT)
            {
            ShowEventError(ret);
            handledEvent = TRUE;
            }
         }

      /* If it wasn't a Spaceball event, handle it through
       * the normal channels.
       */

      if ( handledEvent == FALSE )
         {

         /*
          * do some special key processing so keys have effects
          * in the button window
          */

         if (msg.message == WM_KEYUP ||
             msg.message == WM_KEYDOWN)
            {
            if ( ( msg.wParam == VK_ESCAPE )  )
               {
               SendMessage ( Spw_hWndMain, WM_CLOSE, 0, 0l );
               }
            }

         /*
          * If the button window was created, check to see if this message is
          * intended for the button window, only if it is for the main window
          * can we process it. (IsDialogMessage takes care of passing messages
          * to the button window
          */

         if ( ( Spw_hWndButton == NULL ) ||
              ( !IsDialogMessage( Spw_hWndButton, &msg ) ) )
            {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
            }
         } 

      /* if the redraw flag is set, for the screen to repaint. */

      if ( Spw_Redraw )
         InvalidateRect( Spw_hWndMain, NULL, FALSE );

      } 

   return( msg.wParam );

} /* end of DispatchLoop */

/*-----------------------------------------------------------------------------

   This function processes an event send to the button window dialog box.

   Args:  hWnd   (r/o)  The handle to the button window.
          msg    (r/o)  The message ID sent to that window.
          wParam (r/o)  Parameters sent with the message.
          lParam (r/o)  Parameters sent with the message.

   Returns: TRUE if we processed the message, FALSE if we did not.

-----------------------------------------------------------------------------*/
long WINAPI
HandleDialogEvent ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   switch ( msg )
      {
      case WM_INITDIALOG:
         SpwUpdateButtons();
         break;

      case WM_COMMAND:
         switch ( wParam )
            {
            case IDTRANS: /* Translation check box */

               if ( Spw_TransAllowed = !Spw_TransAllowed )
                  SiBeep(devHdl,"cC");

               if( ( Spw_TransAllowed == FALSE ) && 
                   ( Spw_RotAllowed == FALSE ) )
                  Spw_RotAllowed = TRUE;
               break;

            case IDROTAT: /* Rotations check box */

               if ( Spw_RotAllowed = !Spw_RotAllowed )
                  SiBeep(devHdl,"cC");

               if ( ( Spw_TransAllowed == FALSE ) && 
                    ( Spw_RotAllowed == FALSE ) )
                  Spw_TransAllowed = TRUE;
               break;

            case IDSAXIS: /* Single Axis Filter check box */
               if ( Spw_SingleAxisFilter = !Spw_SingleAxisFilter )
                  SiBeep(devHdl,"cC");

               break;

            case IDROLL:  /* Keep in View check box */
               if ( Spw_RollAllowed = !Spw_RollAllowed )
                  SiBeep(devHdl,"cC");

               break;

            case IDDEF:   /* Restore Defaults Button */
               Spw_TransAllowed = TRUE;
               Spw_RotAllowed = TRUE;
               Spw_SingleAxisFilter = FALSE;
               Spw_RollAllowed = FALSE;
               Spw_Sensitivity = 1.0f;
               SiBeep(devHdl,"cC");

               Spw_Redraw = TRUE;
               break;

            case IDRVIEW: /* Reset View Button */
               SPW_MakeIdentityMatrix(Spw_World_to_eye);
               Spw_World_to_eye[3][0] = XTRAN;
               Spw_World_to_eye[3][1] = YTRAN;
               Spw_World_to_eye[3][2] = ZTRAN;
               SiBeep(devHdl,"cC");

               Spw_Redraw = TRUE;
               break;

            case IDRZERO: /* Rezero Button */
               SiBeep(devHdl,"cC");
               SiRezero(devHdl);
               break;

            case IDQUIT:  /* Quit Button */
               SendMessage ( Spw_hWndMain, WM_CLOSE, 0, 0l );
               break;

            case IDHELP:  /* Help Button */
               {
             
			 	   
			   char filename[256];
               char *helpName = "\\..\\..\\..\\hlp\\cityfly.hlp";
               char *ptr;
			   char *backslash = "\\";        

               GetModuleFileName( NULL, filename, 256 );
             
			   ptr = strrchr( filename, backslash[0]);
             
			   strcpy( ptr, helpName );
               WinHelp( hWnd, filename, HELP_CONTENTS, 0 );
               } 
               break;

            default: /* Unknown command */
               break;
            } /* end switch wParam (ie, which command ) */
         SpwUpdateButtons();
         break;

      case WM_HSCROLL:
         switch ( LOWORD( wParam ) )
            {
            case SB_PAGEDOWN:  /* Increasing our sensitivity */
            case SB_LINEDOWN:
               if ( (Spw_Sensitivity*2) < 32.0 )
                  {
                  Spw_Sensitivity *= 2;
                  SiBeep(devHdl,"cC");

                  }
               break;

            case SB_PAGEUP:  /* Decreasing our sensitivity */
            case SB_LINEUP:
               if ( (Spw_Sensitivity/2) > (1.0/32.0) )
                  {
                  Spw_Sensitivity /= 2;
                  SiBeep(devHdl,"cC");
                  }
               break;

            case SB_THUMBPOSITION:  /* Moving "Thumb" by hand */
               {
               int currPos;

               currPos = ComputeScrollPos( Spw_Sensitivity, NUM_SCROLL_POS );
               currPos -= HIWORD( wParam );
               if ( currPos < 0 )
                  {
                  Spw_Sensitivity *= (float)(1<<(-currPos));
                  SiBeep(devHdl,"cC");
                  } 
               else if ( currPos > 0 )
                  {
                  Spw_Sensitivity /= (float)(1<<(currPos));
                  SiBeep(devHdl,"cC");
                  } 
               } 
               break;

            default: /* Unknown Scroll command, or one we don't care about. */
               break;
            }
         SpwUpdateButtons();
         break;

      case WM_CLOSE:
         SendMessage ( Spw_hWndMain, WM_CLOSE, 0, 0l );
         break;

      default:
         return( FALSE );
      }  

   return( TRUE );
} /* end of HandleDialogEvent */

/*-----------------------------------------------------------------------------

  Function:  HandleNTEvent

  Description:
    Handle any windowing system events.

  Args:
    hWnd   (r/o)  Handle to the window which recieved the message.
    msg    (r/o)  The message type.
    wParam (r/o)  Parameters for the particular message type.
    lParam (r/o)  Parameters for the particular message type.

  Returns: The result of the default message handler, or 0 if we want
           to kill the window.

-----------------------------------------------------------------------------*/

long WINAPI
HandleNTEvent ( HWND hWnd, unsigned msg, WPARAM wParam, LPARAM lParam )
{
   int done;
   PAINTSTRUCT ps;
   enum SpwRetVal ret;
   SiGetEventData EData;
   SiSpwEvent Event;

   switch ( msg )
      {
      case WM_KEYDOWN:
         switch( wParam )
            {
            case 'd':
            case 'D':
               Spw_DoubleBuffer = !Spw_DoubleBuffer;
               break;
            default:
               break;
            }
         break;

      case WM_KEYUP:
         switch( wParam )
            {
            case VK_ESCAPE:
               SendMessage ( Spw_hWndMain, WM_CLOSE, 0, 0l );
               break;
            default:
               break;
            }

      case WM_PAINT:
         Spw_hDC = BeginPaint ( Spw_hWndMain, &ps );
         Spw_hDCScreen = Spw_hDC;
         if (Spw_hDCBitmap == NULL)
           {
             HDC old;
             Spw_hDCBitmap = CreateCompatibleDC(NULL);
             Spw_hBitmap = CreateCompatibleBitmap(Spw_hDC,
                                                  Spw_WindowWidth+1,
                                                  Spw_WindowHeight+1);
             old = SelectObject(Spw_hDCBitmap, Spw_hBitmap);
           }
         if (Spw_DoubleBuffer)
            {
            Spw_hDC = Spw_hDCBitmap;
            }
         Spw_Redraw = FALSE;
         UpdateScreen();
         EndPaint ( Spw_hWndMain, &ps );
         break;

      case WM_SIZE:
        Spw_WindowWidth = LOWORD(lParam);
        Spw_WindowHeight = HIWORD(lParam);
        Spw_WindowSize = SPW_MIN( Spw_WindowWidth, Spw_WindowHeight );
        DeleteObject( Spw_hDCBitmap );
        Spw_hDCBitmap = NULL;
        DeleteObject( Spw_hBitmap );
        Spw_hBitmap = NULL;
        Spw_Redraw = TRUE;
        break;

      case WM_CLOSE:
         /* cleanup the object info created */
         DeleteObject ( Spw_white );
         DeleteObject ( Spw_black  );
         DeleteObject ( Spw_grey );
         DeleteObject ( Spw_green );
         break;

      case WM_DESTROY:
         PostQuitMessage (0);
         done = 1;
         return (0);

      default:
         if (devHdl)
            {
            SiGetEventWinInit (&EData, msg, wParam, lParam);
            if ((ret = SiGetEvent (devHdl, 0, &EData, &Event)) == SI_IS_EVENT)
               {
               if (Event.type == SI_MOTION_EVENT)
                  {
                  SbMotionEvent(&Event);
                  } 
               if (Event.type == SI_BUTTON_EVENT)
                  {
                  SbButtonEvent(&Event);
                  }
              return (0);
               }

            /* If it is a SpaceWare event, check for common errors */

            else if (ret != SI_NOT_EVENT)
               {
               ShowEventError(ret);
               }
            }
         break;
      }

   return DefWindowProc ( hWnd, msg, wParam, lParam );

} /* end of HandleNTEvent */

/*-----------------------------------------------------------------------------

  Function:  WinMain

  Description:
    This is the main Windows NT routine.

  Args:   hInstance     (r/o) The handle to the instance of this program.
          hPrevInstance (r/o) The handle to the previous instance?
          lpszCmdLine   (r/o) String with the command line
          nCmdShow      (r/o)

  Returns: The results of the dispatch loop

-----------------------------------------------------------------------------*/

int WINAPI
WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine,
          int nCmdShow )
{
   float D;                       /* Perspective constant */
   int midx,midy;                 /* Window size determination variables */
   char *window_name = "Cityfly"; /* The string for window name */
   WNDCLASS wndclass;             /* A variable to hold our class definition */
   HWND wndScreen;                /* Hold a handle to the screen window */
   RECT screenRect;               /* Find the exents of the screen */
   SiOpenData oData;
   int stat;

   /*
    *   Create colored pens
    */
   Spw_white = CreatePen ( PS_SOLID, 1, RGB(255, 255, 255) );
   Spw_black = CreatePen ( PS_SOLID, 1, RGB(0, 0, 0) );
   Spw_grey =  CreatePen ( PS_SOLID, 1, RGB(255, 255, 255) );
   Spw_green = CreatePen ( PS_SOLID, 1, RGB(0, 255, 0) );

   /*
    * Register display window class
    */
   wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
   wndclass.lpfnWndProc   = HandleNTEvent ;
   wndclass.cbClsExtra    = 0 ;
   wndclass.cbWndExtra    = 0 ;
   wndclass.hInstance     = hInstance;
   wndclass.hIcon         = NULL;
   wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
   wndclass.hbrBackground = GetStockObject(BLACK_BRUSH);
   wndclass.lpszMenuName  = NULL ;
   wndclass.lpszClassName = window_name;
   RegisterClass (&wndclass);

   /*
    *   Figure out how big the screen is so that we can make our
    *   window an appropriate size.
    */
   wndScreen = GetDesktopWindow();
   GetWindowRect ( wndScreen, &screenRect );
   Spw_WindowSize = (int) (0.8f * SPW_MIN( (screenRect.bottom - screenRect.top),
                    (screenRect.right - screenRect.left) ));
   Spw_WindowWidth = Spw_WindowSize;
   Spw_WindowHeight = Spw_WindowSize;
   midx = Spw_WindowWidth/2;
   midy = Spw_WindowHeight/2;

   /* create the window */
   Spw_hWndMain = CreateWindow ( window_name,     /* Window class name       */
                             window_name,         /* Window caption          */
                             WS_OVERLAPPEDWINDOW, /* Window Style            */
                             0,0,
                             Spw_WindowWidth,
                             Spw_WindowHeight,    /* Window geometry         */
                             NULL,                /* parent window handle    */
                             NULL,                /* window menu handle      */
                             hInstance,           /* program instance handle */
                             NULL);               /* creation parameters     */
   /*
    *  Set the icon
    */
   SetClassLong( Spw_hWndMain, GCL_HICON,
                 (LONG)LoadIcon( hInstance, (char *)IDI_ICON2 ) );

   /*
    *  Initialize the spaceball
    */
   if (SiInitialize() == SPW_DLL_LOAD_ERROR)  
     {
	  MessageBox(Spw_hWndMain,"Error: Could not load SiAppDll dll files",
		         NULL, MB_ICONEXCLAMATION);
	 }

   SiOpenWinInit (&oData, Spw_hWndMain);
   devHdl = SiOpen (window_name, SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData);

   if (devHdl == SI_NO_HANDLE)
      {
      SiTerminate ();
      MessageBox (Spw_hWndMain,
         "Sorry - No Spacetec IMC device found.", NULL, MB_OK);
      if (Spw_hWndMain)
         DestroyWindow (Spw_hWndMain);
      ExitProcess (1);
      }

   SiSetUiMode (devHdl, SI_UI_FUNC_BUTTONS | SI_UI_DIALOG_POPUP);

   /* Initialize the perspective matrix */
   
   D = (float) (1.0/tan( (double) ((SPW_PI/180.0)*(FOV/2.0))));
   SPW_InitializeMatrix(Spw_Persp, 1.0f, 0.0f,  0.0f,  0.0f,
                                   0.0f, 1.0f,  0.0f,  0.0f,
                                   0.0f, 0.0f, -1.0f, -1.0f/D,
                                   0.0f, 0.0f,  0.0f,  0.0f);

   /*  Get the geometery */
   
   GetUniqPoints(bldg, buniq, Spw_SizeBldg);
   GetUniqPoints(land, luniq, Spw_SizeLand);

   /* 
    * Set up our button window, and move main window to make sure
    * it's in view
    */

   Spw_hWndButton = CreateDialog( hInstance, (char *)IDD_SETTINGSDLG,
                                 Spw_hWndMain,
                                 HandleDialogEvent );
   GetWindowRect ( Spw_hWndButton, &screenRect );
   MoveWindow( Spw_hWndMain, screenRect.right, 0, Spw_WindowWidth,
              Spw_WindowHeight, TRUE );

   /* display the window */

   ShowWindow ( Spw_hWndMain, nCmdShow );
   UpdateWindow ( Spw_hWndMain );
   SpwUpdateButtons();

   stat = DispatchLoop ();

   if (Spw_hWndButton)
      DestroyWindow (Spw_hWndButton);

   if (Spw_hWndMain)
      DestroyWindow (Spw_hWndMain);

   SiClose (devHdl);
   SiTerminate ();

   return (stat);
}
