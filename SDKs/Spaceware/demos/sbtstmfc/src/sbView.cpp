/*----------------------------------------------------------------------
 *  sbtestmfcview.c  -- Basic MFC Program to initinalize a spaceball, read 
 *                  its data and print it out.
 *
 *  $Id: sbView.cpp,v 1.1.2.1.2.1 1998/05/26 18:12:39 equerze Exp $
 *
 *  Written By Elio Querze
 *
 *  NOTE: MUST LINK WITH SIAPPST.LIB and WINMM.LIB
 *
 *----------------------------------------------------------------------
 *
 *  (C) 1998 Spacetec IMC Corporation.  All rights reserved.
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
static char CvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: sbView.cpp,v 1.1.2.1.2.1 1998/05/26 18:12:39 equerze Exp $";
 
#include "stdafx.h"
#include "sbtstmfc.h"

#include "sbDoc.h"
#include "sbView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* SpaceWare Specific Includes */

#include "spwmacro.h"  /* Common macros used by SpaceWare functions. */
#include "si.h"        /* Required for any SpaceWare support within an app.*/

/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcView

IMPLEMENT_DYNCREATE(CSbtestmfcView, CView)

BEGIN_MESSAGE_MAP(CSbtestmfcView, CView)
	//{{AFX_MSG_MAP(CSbtestmfcView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcView construction/destruction

CSbtestmfcView::CSbtestmfcView()
{

}

CSbtestmfcView::~CSbtestmfcView()
{
 
}


/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcView drawing

void CSbtestmfcView::OnDraw(CDC* pDC)
{

 /* When app loses and regains focus print this (also init) */   
 pDC->TextOut( 0 , 0, "Spaceball Ready", 15);
 pDC->TextOut( 15 , 60, "TX: 0", 5);
 pDC->TextOut( 15 , 80, "TY: 0", 5);
 pDC->TextOut( 15 , 100, "TZ: 0", 5);
 pDC->TextOut( 15 , 120, "RX: 0", 5);
 pDC->TextOut( 15 , 140, "RY: 0", 5);
 pDC->TextOut( 15 , 160, "RZ: 0", 5);

}

/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcView diagnostics

#ifdef _DEBUG
void CSbtestmfcView::AssertValid() const
{
	CView::AssertValid();
}

void CSbtestmfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSbtestmfcDoc* CSbtestmfcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSbtestmfcDoc)));
	return (CSbtestmfcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcView Spaceball Functions

/*--------------------------------------------------------------------------
 * Function: CSbtestmfcView::SbInit()
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
CSbtestmfcView::SbInit()
{
  int res;                                    //result of SiOpen, to be returned  
  SiOpenData oData;                           //OS Independent data to open ball  
  
  if (SiInitialize() == SPW_DLL_LOAD_ERROR)   //init the SpaceWare input library
     {
	  MessageBox("Error: Could not load SiAppDll dll files",
		         NULL, MB_ICONEXCLAMATION);
	 }
  SiOpenWinInit (&oData, m_hWnd);             //init Win. platform specific data  
  SiSetUiMode(&m_DevHdl, SI_UI_ALL_CONTROLS); //Config SoftButton Win Display 

  //open data, which will check for device type and return the device handle
  // to be used by this function  
  if ( (m_DevHdl = SiOpen ("TestSP", SI_ANY_DEVICE, SI_NO_MASK,  
                         SI_EVENT, &oData)) == NULL )
     {
     SiTerminate();  //called to shut down the SpaceWare input library 
     res = 0;        //could not open device
     return res; 
     }
  else
     {
     res = 1;        //opened device succesfully
     return res;  
     }  
}


/*----------------------------------------------------------------------
 * Function: SbMotionEvent()
 *
 * Description:
 *    This function receives motion information and prints out the info 
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
CSbtestmfcView::SbMotionEvent(SiSpwEvent *pEvent)
{	
   CDC* pDC;
   
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
	pDC = GetDC();        
   
   /* print buffers */
   pDC->TextOut( 0 , 0, "Motion Event                ", 28);
   pDC->TextOut( 15 , 60, buff0, len0);
	pDC->TextOut( 15 , 80, buff1, len1);
	pDC->TextOut( 15 , 100, buff2, len2);
	pDC->TextOut( 15 , 120, buff3, len3);
	pDC->TextOut( 15 , 140, buff4, len4);
	pDC->TextOut( 15 , 160, buff5, len5);
   
   /*release our window handle */ 
	ReleaseDC(pDC);
}



/*----------------------------------------------------------------------
 * Function: CSbtestmfcView::SbZeroEvent()
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
CSbtestmfcView::SbZeroEvent()
{	
   /* get handle of our window to draw on */
   CDC* pDC;
  
   pDC = GetDC();  

   /* print null data */   
   pDC->TextOut( 0 , 0, "Zero Event                  ", 28);
   pDC->TextOut( 15 , 60, "TX: 0          ", 15);
	pDC->TextOut( 15 , 80, "TY: 0          ", 15);
   pDC->TextOut( 15 , 100, "TZ: 0          ", 15);
   pDC->TextOut( 15 , 120, "RX: 0          ", 15);
   pDC->TextOut( 15 , 140, "RY: 0          ", 15);
   pDC->TextOut( 15 , 160, "RZ: 0          ", 15);

   /*release our window handle */ 
   ReleaseDC(pDC);
}



/*----------------------------------------------------------------------
 * Function: CSbtestmfcView::SbButtonPressEvent()
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
CSbtestmfcView::SbButtonPressEvent(int buttonnumber)
{
   /* get handle of our window to draw on */
   CDC* pDC;
 
   pDC = GetDC();  
  
   /* print button pressed(does not include rezero button) */
   switch (buttonnumber) 
      {
      case SI_PICK_BUTTON:           /* #31 defined in si.h*/
         pDC->TextOut( 0 , 0, "Pick Button Pressed ", 20); 
         break;
      case 1:
         pDC->TextOut( 0 , 0, "Button 1 Pressed ", 17); 
         break;
      case 2:
         pDC->TextOut( 0 , 0, "Button 2 Pressed ", 17); 
         break;
      case 3:
         pDC->TextOut( 0 , 0, "Button 3 Pressed ", 17); 
         break;
      case 4:
         pDC->TextOut( 0 , 0, "Button 4 Pressed ", 17); 
         break;
      case 5:
         pDC->TextOut( 0 , 0, "Button 5 Pressed ", 17); 
         break;
      case 6:
         pDC->TextOut( 0 , 0, "Button 6 Pressed ", 17); 
         break;
      case 7:
         pDC->TextOut( 0 , 0, "Button 7 Pressed ", 17); 
         break;
      case 8:
         pDC->TextOut( 0 , 0, "Button 8 Pressed ", 17); 
         break; 
      default: 
         pDC->TextOut( 0 , 0, "Button ? Pressed ", 17);  
         break;
      } 

    /*release our window handle */ 
    ReleaseDC(pDC);
}	

/*----------------------------------------------------------------------
 * Function: CSbtestmfcView::SbButtonReleaseEvent()
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
CSbtestmfcView::SbButtonReleaseEvent(int buttonnumber)
{
   /* get handle of our window to draw on */
   CDC* pDC;
 
   pDC = GetDC();  
  
   /* print button pressed(does not include rezero button) */
   switch (buttonnumber) 
      {
      case SI_PICK_BUTTON:           /* #31 defined in si.h*/
         pDC->TextOut( 0 , 0, "Pick Button Released", 20); 
         break;
      case 1:
         pDC->TextOut( 0 , 0, "Button 1 Released", 17); 
         break;
      case 2:
         pDC->TextOut( 0 , 0, "Button 2 Released", 17); 
         break;
      case 3:
         pDC->TextOut( 0 , 0, "Button 3 Released", 17); 
         break;
      case 4:
         pDC->TextOut( 0 , 0, "Button 4 Released", 17); 
         break;
      case 5:
         pDC->TextOut( 0 , 0, "Button 5 Released", 17); 
         break;
      case 6:
         pDC->TextOut( 0 , 0, "Button 6 Released", 17); 
         break;
      case 7:
         pDC->TextOut( 0 , 0, "Button 7 Released", 17); 
         break;
      case 8:
         pDC->TextOut( 0 , 0, "Button 8 Released", 17); 
         break; 
      default: 
         pDC->TextOut( 0 , 0, "Button ? Released", 17);  
         break;
      } 

    /*release our window handle */ 
    ReleaseDC(pDC);
}	
/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcView message handlers

/*----------------------------------------------------------------------
 * Function: CSbtestmfcView::PreTranslateMessage() 
 *
 * Description:
 *    This function recieves Spaceball button information and prints out the 
 *    info on screen.
 *    
 *
 * Args:
 *    MSG*  pMsg    //pointer to the message that we trap and check
 *
 * Return Value:
 *    BOOL  handled //was the message handled?
 *
 *----------------------------------------------------------------------*/
BOOL CSbtestmfcView::PreTranslateMessage(MSG* pMsg) 
{
   int            num;      /* number of button returned */
   BOOL           handled;  /* is message handled yet */ 
   SiSpwEvent     Event;    /* SpaceWare Event */ 
   SiGetEventData EData;    /* SpaceWare Event Data */
   
   handled = SPW_FALSE;         /* init handled */

   /* init Window platform specific data for a call to SiGetEvent */
   SiGetEventWinInit(&EData, pMsg->message, pMsg->wParam, pMsg->lParam);
  
  /* check whether msg was a Spaceball event and process it */
  if (SiGetEvent (m_DevHdl, 0, &EData, &Event) == SI_IS_EVENT)
     {
     /* figure out what kind of Spaceball event it is */
     if (Event.type == SI_MOTION_EVENT)
        {
        SbMotionEvent(&Event);  /* process Spaceball motion event */     
	     }
     if (Event.type == SI_ZERO_EVENT)
        {
        SbZeroEvent();          /* process Spaceball zero event */     
    	  }
     if (Event.type == SI_BUTTON_EVENT)
        {
        if ((num = SiButtonPressed (&Event)) != SI_NO_BUTTON)	
           {
           SbButtonPressEvent(num);     /* process Spaceball button event */
           }
		if ((num = SiButtonReleased (&Event)) != SI_NO_BUTTON)	
           {
           SbButtonReleaseEvent(num);     /* process Spaceball button event */
           }
     }
     handled = SPW_TRUE;            /* spaceball event handled */ 
  }

  /* if its not a Spaceball event, let windows handle it */
  if (handled == SPW_FALSE)
	  {
     return CView::PreTranslateMessage(pMsg);
     }
  else
     {
     return handled;
     }  
}


/*----------------------------------------------------------------------
 * Function: CSbtestmfcView::OnCreate() 
 *
 * Description:
 *    This function recieves Spaceball button information and prints out the 
 *    info on screen.
 *    
 *
 * Args: LPCREATESTRUCT lpCreateStruct  //info about the CWnd being created.
 *    
 * Return Value: 
 *    must be 0 to create window, -1 to destroy
 *
 *----------------------------------------------------------------------*/
int CSbtestmfcView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  
	if (CView::OnCreate(lpCreateStruct) == -1)
      {
		return -1;
	   }

	int res;  /* Is Spaceball detected? */
   
   /* intitialize spaceball */
   res = SbInit();

   // if SpaceBall was not detected then print error, close win., exit prog. 
   if (res < 1)
      {
       MessageBox( "Sorry - No supported Spacetec IMC device available.\n",
		            NULL, MB_OK);
       if (m_hWnd != NULL)
          {
          DestroyWindow();    // destroy window   
          }
       ExitProcess(1);        // exit program 
       }

	return 0;
}

