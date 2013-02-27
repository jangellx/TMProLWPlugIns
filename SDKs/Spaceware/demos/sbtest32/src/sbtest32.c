/*----------------------------------------------------------------------
 *  sbtest32.c  -- Basic Win32 Program to initinalize a spaceball, read 
 *                  its data and print it out.
 *
 *  $Id: sbtest32.c,v 1.8.2.1.2.1 1998/05/26 18:13:04 equerze Exp $
 *
 *  Written By Elio Querze
 *
 *  NOTE: MUST LINK WITH SIAPPST.LIB 
 *
 *----------------------------------------------------------------------
 *
 *  (C) 1997 Spacetec IMC Corporation.  All rights reserved.
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
 */

static char cvsId[]="(C) 1997 Spacetec IMC Corporation: $Id: sbtest32.c,v 1.8.2.1.2.1 1998/05/26 18:13:04 equerze Exp $";
 

/* Standard Win32 Includes */
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <float.h>
#include <stdlib.h>


/* SpaceWare Specific Includes */

#include "spwmacro.h"  /* Common macros used by SpaceWare functions. */
#include "si.h"        /* Required for any SpaceWare support within an app.*/



/* Program Specific Includes */

#include "sbtest32.h"


/* Function Definitions */

/*----------------------------------------------------------------------
 * Function: WinMain()
 *
 * Description:
 *    This is the main window function and we use it to initialize data
 *    and then call our loop function. This is a std. Win32 function.
 *
 * Args:
 *    HINSTANCE hInstance         // handle to current instance 
 *    HINSTANCE hPrevInstance     // handle to previous instance 
 *    LPSTR     lpszCmdLine       // pointer to command line
 *    int       nCmdShow          // show state of window
 *
 * Return Value:
 *    int       Returns the return value of DispatchLoopNT
 *
 *----------------------------------------------------------------------*/
int 
WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpszCmdLine, int nCmdShow )
{   
  int  res;            /* SbInits result..if>0 it worked, if=0 it didnt work   */ 
  int  hsize, vsize;   /* size of window to be created,for each Dimension */
 
  /* Set Window Size */
  hsize = 280;
  vsize = 215;

  /* create our apps window */
  CreateSPWindow(0, 0, hsize, vsize, "SbTest32");

  /* update screen */
  InvalidateRect(hWndMain, NULL, FALSE);

  /* intitialize spaceball */
  res = SbInit();

  /* if SpaceBall was not detected then print error, close win., exit prog. */
  if (res < 1)
    {
    MessageBox(hWndMain, 
               "Sorry - No supported Spacetec IMC device available.\n",
		         NULL, MB_OK);
    if (hWndMain != NULL)
       {
       DestroyWindow(hWndMain);    /* destroy window */  
       }
 
    ExitProcess(1);                /* exit program */
    }

  /* Function To be Repeated */
  return(DispatchLoopNT());

} 


/*--------------------------------------------------------------------------
 * Function: SbInit()
 *
 * Description:
 *    This function initializes the Spaceball and opens ball for use.
 *    
 *
 * Args: None
 *    
 *
 * Return Value:
 *    int  res         result of SiOpen, =0 if Fail =1 if it Works
 *
 *--------------------------------------------------------------------------*/
int
SbInit()
{
  int res;                             /* result of SiOpen, to be returned  */
  SiOpenData oData;                    /* OS Independent data to open ball  */ 
  
  /*init the SpaceWare input library */
  if (SiInitialize() == SPW_DLL_LOAD_ERROR)  
     {
	  MessageBox(hWndMain,"Error: Could not load SiAppDll dll files",
		         NULL, MB_ICONEXCLAMATION);
	 }

  SiOpenWinInit (&oData, hWndMain);    /* init Win. platform specific data  */
  SiSetUiMode(devHdl, SI_UI_ALL_CONTROLS); /* Config SoftButton Win Display */

  /* open data, which will check for device type and return the device handle
     to be used by this function */ 
  if ( (devHdl = SiOpen ("sbtest32", SI_ANY_DEVICE, SI_NO_MASK,  
                         SI_EVENT, &oData)) == NULL) 
     {
     SiTerminate();  /* called to shut down the SpaceWare input library */
     res = 0;        /* could not open device */
     return res; 
     }
  else
     {
     res = 1;        /* opened device succesfully */ 
     return res;
     }  
}


/*--------------------------------------------------------------------------
 * Function: DispatchLoopNT()
 *
 * Description:
 *    This function contains the main message loop which constantly checks for 
 *    SpaceBall Events and handles them apropriately. 
 *
 * Args: None
 *    
 *
 * Return Value:
 *    int  msg.wparam                  // event passed to window
 *
 *--------------------------------------------------------------------------*/
int 
DispatchLoopNT()
{
   int            num;      /* number of button pressed */
   MSG            msg;      /* incoming message to be evaluated */
   BOOL           handled;  /* is message handled yet */ 
   SiSpwEvent     Event;    /* SpaceWare Event */ 
   SiGetEventData EData;    /* SpaceWare Event Data */

   handled = SPW_FALSE;     /* init handled */

   /* start message loop */ 
   while ( GetMessage( &msg, NULL, 0, 0 ) )
      {
	   handled = SPW_FALSE;
      
      /* init Window platform specific data for a call to SiGetEvent */
      SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);
      
      /* check whether msg was a Spaceball event and process it */
      if (SiGetEvent (devHdl, 0, &EData, &Event) == SI_IS_EVENT)
         {
         if (Event.type == SI_MOTION_EVENT)
            {
            SbMotionEvent(&Event);        /* process Spaceball motion event */     
			   }
         if (Event.type == SI_ZERO_EVENT)
            {
            SbZeroEvent();                /* process Spaceball zero event */     
			   }
         if (Event.type == SI_BUTTON_EVENT)
            {
            if ((num = SiButtonPressed (&Event)) != SI_NO_BUTTON)	
               {
               SbButtonPressEvent(num);        /* process Spaceball button event */
               }
            if ((num = SiButtonReleased (&Event)) != SI_NO_BUTTON)	
               {
               SbButtonReleaseEvent(num);        /* process Spaceball button event */
               }
		    }
         
		   handled = SPW_TRUE;              /* spaceball event handled */ 
         }

      /* not a Spaceball event, let windows handle it */
      if (handled == SPW_FALSE)
	      {
	      TranslateMessage( &msg );
         DispatchMessage( &msg );
         }
      }

   return( msg.wParam );
} 


/*--------------------------------------------------------------------------
 * Function: HandleNTEvent
 *
 * Description:  This is a std. Win32 function to handle various window events
 *     
 *    
 *
 * Args: HWND hWnd                    // handle to window   
 *       unsigned msg                 // message to process
 *       WPARAM wParam                // 32 bit msg parameter
 *       LPARAM lParam                // 32 bit msg parameter
 *
 * Return Value:
 *    int  msg.wparam                 // program done
 *
 *--------------------------------------------------------------------------*/
long 
WINAPI HandleNTEvent ( HWND hWnd, unsigned msg, WPARAM wParam,
                           LPARAM lParam )

{
  PAINTSTRUCT ps;           /* used to paint the client area of a window */ 
  LONG addr;                /* address of our window */
   
  addr = GetClassLong(hWnd, 0);  /* get address */
  
  switch ( msg )
     {
     case WM_ACTIVATEAPP:
       hdc = GetDC(hWnd);       
       /* print buffers */
       TextOut(hdc, 0 , 0, "Spaceball Ready", 15);
       TextOut(hdc, 15 , 100, "TX: 0", 5);
       TextOut(hdc, 15 , 120, "TY: 0", 5);
       TextOut(hdc, 15 , 140, "TZ: 0", 5);
       TextOut(hdc, 15 , 160, "RX: 0", 5);
       TextOut(hdc, 15 , 180, "RY: 0", 5);
       TextOut(hdc, 15 , 200, "RZ: 0", 5);
   
   /*release our window handle */ 
   ReleaseDC(hWnd,hdc);
     case WM_KEYDOWN:
     case WM_KEYUP:
       /* user hit a key to close program */
       if (wParam == VK_ESCAPE)
         {                  
         SendMessage ( hWndMain, WM_CLOSE, 0, 0l );
         }
       break;
      
     case WM_PAINT:
       /* time to paint the window */
       if (addr)
          {
          hdc = BeginPaint ( hWndMain, &ps ); 
          EndPaint ( hWndMain, &ps );
          }

       break;
      
     case WM_CLOSE:
       /* cleanup the object info created */
      
       break;
      
     case WM_DESTROY :
       PostQuitMessage (0);
       return (0);
     }
  return DefWindowProc ( hWnd, msg, wParam, lParam );

} 


/*--------------------------------------------------------------------------
 * Function: CreateSPWindow
 *
 * Description:  This creates the window for our app
 *     
 *    
 *
 * Args:  int  atx        // horiz. start point to put window
 *        int  aty        // vert.  start point to put window
 *        int  hi         // hight of window
 *        int  wid        // width of window
 *        char *string    // window caption
 *
 * Return Value:
 *    NONE
 *
 *--------------------------------------------------------------------------*/
void 
CreateSPWindow (int atx, int aty, int hi, int wid, char *string)

{
   WNDCLASS wndclass;        /* our own instance of the window class */
   HANDLE      hInst;        /* handle to our instance */

   hInst = NULL;             /* init handle */
   
   /* Register display window class */
   wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
   wndclass.lpfnWndProc   = HandleNTEvent ;
   wndclass.cbClsExtra    = 8 ;
   wndclass.cbWndExtra    = 0 ;
   wndclass.hInstance     = hInst;
   wndclass.hIcon         = NULL;
   wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
   wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
   wndclass.lpszMenuName  = NULL ;
   wndclass.lpszClassName = "sbtest32";

   RegisterClass (&wndclass) ;

   /* create the window */
   hWndMain = CreateWindow ( "sbtest32",           /*Window class name*/
                              string,              /*Window caption*/
			                     WS_OVERLAPPEDWINDOW, /*Window Style*/
                              atx,aty,wid,hi,
                              NULL,	               /*parent window handle*/
			                     NULL,                /*window menu handle*/
			                     hInst,		         /*program instance handle*/
			                     NULL);               /*creation parameters*/

   
   /* display the window */
   ShowWindow ( hWndMain, SW_SHOW );
  
   /* get handle of our window to draw on */
   hdc = GetDC(hWndMain);        
   
   /* print buffers */
   TextOut(hdc, 0 , 0, "Spaceball Ready", 15);
   TextOut(hdc, 15 , 100, "TX: 0", 5);
   TextOut(hdc, 15 , 120, "TY: 0", 5);
   TextOut(hdc, 15 , 140, "TZ: 0", 5);
   TextOut(hdc, 15 , 160, "RX: 0", 5);
   TextOut(hdc, 15 , 180, "RY: 0", 5);
   TextOut(hdc, 15 , 200, "RZ: 0", 5);
   
   /*release our window handle */ 
   ReleaseDC(hWndMain,hdc);
   
   UpdateWindow ( hWndMain );

} /* end of CreateWindow */


/*----------------------------------------------------------------------
 * Function: SbMotionEvent()
 *
 * Description:
 *    This function recieves motion information and prints out the info 
 *    on screen.
 *    
 *
 * Args:
 *    SiSpwEvent *pEvent   Containts Data from a Spaceball Event
 *
 * Return Value:
 *    NONE
 *
 *----------------------------------------------------------------------*/
void 
SbMotionEvent(SiSpwEvent *pEvent)
{	

   
   char buff0[20];                            /* text buffer for TX */
   char buff1[20];                            /* text buffer for TY */
	char buff2[20];                            /* text buffer for TZ */
	char buff3[20];                            /* text buffer for RX */
	char buff4[20];                            /* text buffer for RY */
	char buff5[20];                            /* text buffer for RZ */ 

   int len0,len1,len2,len3,len4,len5;	      /* length of each buffer */
   

   /* put the actual ball data into the buffers */
	len0= sprintf( buff0, "TX: %d         ", pEvent->u.spwData.mData[SI_TX] );
	len1= sprintf( buff1, "TY: %d         ", pEvent->u.spwData.mData[SI_TY] );
	len2= sprintf( buff2, "TZ: %d         ", pEvent->u.spwData.mData[SI_TZ] );
	len3= sprintf( buff3, "RX: %d         ", pEvent->u.spwData.mData[SI_RX] );
	len4= sprintf( buff4, "RY: %d         ", pEvent->u.spwData.mData[SI_RY] );
	len5= sprintf( buff5, "RZ: %d         ", pEvent->u.spwData.mData[SI_RZ] );

   
   /* get handle of our window to draw on */
	hdc = GetDC(hWndMain);        
   
   /* print buffers */
   TextOut(hdc, 0 , 0, "Motion Event                ", 28);
   TextOut(hdc, 15 , 100, buff0, len0);
	TextOut(hdc, 15 , 120, buff1, len1);
	TextOut(hdc, 15 , 140, buff2, len2);
	TextOut(hdc, 15 , 160, buff3, len3);
	TextOut(hdc, 15 , 180, buff4, len4);
	TextOut(hdc, 15 , 200, buff5, len5);
   
   /*release our window handle */ 
	ReleaseDC(hWndMain,hdc);
}


/*----------------------------------------------------------------------
 * Function: SbZeroEvent()
 *
 * Description:
 *    This function clears the previous data, no motion data was recieved
 *    
 *    
 *
 * Args:
 *    NONE
 *
 * Return Value:
 *    NONE
 *
 *----------------------------------------------------------------------*/
void 
SbZeroEvent()
{	
   /* get handle of our window to draw on */
   hdc = GetDC(hWndMain);

   /* print null data */   
   TextOut(hdc, 0 , 0, "Zero Event                  ", 28);
   TextOut(hdc, 15 , 100, "TX: 0          ", 15);
	TextOut(hdc, 15 , 120, "TY: 0          ", 15);
   TextOut(hdc, 15 , 140, "TZ: 0          ", 15);
   TextOut(hdc, 15 , 160, "RX: 0          ", 15);
   TextOut(hdc, 15 , 180, "RY: 0          ", 15);
   TextOut(hdc, 15 , 200, "RZ: 0          ", 15);

   /*release our window handle */ 
   ReleaseDC(hWndMain,hdc);
}



/*----------------------------------------------------------------------
 * Function: SbButtonPressEvent()
 *
 * Description:
 *    This function recieves Spaceball button information and prints out the 
 *    info on screen.
 *    
 *
 * Args:
 *    int     buttonnumber   //Containts number of button pressed 
 *
 * Return Value:
 *    NONE
 *
 *----------------------------------------------------------------------*/
void
SbButtonPressEvent(int buttonnumber)
{
   /* get handle of our window to draw on */
   hdc = GetDC(hWndMain);
  
   /* print button pressed(does not include rezero button) */
   switch (buttonnumber) 
      {
      case SI_PICK_BUTTON:           /* #31 defined in si.h*/
         TextOut(hdc, 0 , 0, "Pick Button Pressed ", 20); 
         break;
      case 1:
         TextOut(hdc, 0 , 0, "Button 1 Pressed ", 17); 
         break;
      case 2:
         TextOut(hdc, 0 , 0, "Button 2 Pressed ", 17); 
         break;
      case 3:
         TextOut(hdc, 0 , 0, "Button 3 Pressed ", 17); 
         break;
      case 4:
         TextOut(hdc, 0 , 0, "Button 4 Pressed ", 17); 
         break;
      case 5:
         TextOut(hdc, 0 , 0, "Button 5 Pressed ", 17); 
         break;
      case 6:
         TextOut(hdc, 0 , 0, "Button 6 Pressed ", 17); 
         break;
      case 7:
         TextOut(hdc, 0 , 0, "Button 7 Pressed ", 17); 
         break;
      case 8:
         TextOut(hdc, 0 , 0, "Button 8 Pressed ", 17);  
         break;
 
      default:
         TextOut(hdc, 0 , 0, "Button ? Pressed ", 17);  
         break;
         
      } 

    /*release our window handle */ 
    ReleaseDC(hWndMain,hdc);
}	

/*----------------------------------------------------------------------
 * Function: SbButtonReleaseEvent()
 *
 * Description:
 *    This function recieves Spaceball button information and prints out the 
 *    info on screen.
 *    
 *
 * Args:
 *    int     buttonnumber   //Containts number of button pressed 
 *
 * Return Value:
 *    NONE
 *
 *----------------------------------------------------------------------*/
void
SbButtonReleaseEvent(int buttonnumber)
{
   /* get handle of our window to draw on */
   hdc = GetDC(hWndMain);
  
   /* print button pressed(does not include rezero button) */
   switch (buttonnumber) 
      {
      case SI_PICK_BUTTON:           /* #31 defined in si.h*/
         TextOut(hdc, 0 , 0, "Pick Button Released", 20); 
         break;
      case 1:
         TextOut(hdc, 0 , 0, "Button 1 Released", 17); 
         break;
      case 2:
         TextOut(hdc, 0 , 0, "Button 2 Released", 17); 
         break;
      case 3:
         TextOut(hdc, 0 , 0, "Button 3 Released", 17); 
         break;
      case 4:
         TextOut(hdc, 0 , 0, "Button 4 Released", 17); 
         break;
      case 5:
         TextOut(hdc, 0 , 0, "Button 5 Released", 17); 
         break;
      case 6:
         TextOut(hdc, 0 , 0, "Button 6 Released", 17); 
         break;
      case 7:
         TextOut(hdc, 0 , 0, "Button 7 Released", 17); 
         break;
      case 8:
         TextOut(hdc, 0 , 0, "Button 8 Released", 17);  
         break;
 
      default:
         TextOut(hdc, 0 , 0, "Button ? Released", 17);  
         break;
         
      } 

    /*release our window handle */ 
    ReleaseDC(hWndMain,hdc);
}	