/*----------------------------------------------------------------------
 *  sbtestmfc.h  -- Basic Program to init spaceball, read its data and 
 *                  print it out.  The most important function is the 
 *                  Pre Translate Message function because we trap 
 *                  messages there.
 *
 *  $Id: sbView.h,v 1.1.2.1 1998/05/18 17:27:33 equerze Exp $
 *
 *  Written By Elio Querze
 *  
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

static char SbTestMfcViewCvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: sbView.h,v 1.1.2.1 1998/05/18 17:27:33 equerze Exp $";
 
#include "si.h"        /* Required for any SpaceWare support within an app.*/

#if !defined(AFX_SBTESTMFCVIEW_H__EC60F75D_86A7_11D1_A7B6_0060979D0B2E__INCLUDED_)
#define AFX_SBTESTMFCVIEW_H__EC60F75D_86A7_11D1_A7B6_0060979D0B2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSbtestmfcView : public CView
{
protected: // create from serialization only
	CSbtestmfcView();
	DECLARE_DYNCREATE(CSbtestmfcView)

// Attributes
public:
	CSbtestmfcDoc* GetDocument();
   
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSbtestmfcView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreTranslateMessage(MSG* pMsg);  //This where we trap the msgs
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSbtestmfcView();

   


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

   /* SpaceBall Variables&Functions */
   
   /* Members */
   SiHdl       m_DevHdl;       /* Handle to Spaceball Device */
   
   /* SpaceBall Functions --See CPP File for Additional Details */
   int   SbInit();
   void  SbMotionEvent(SiSpwEvent *pEvent);
   void  SbZeroEvent();
   void  SbButtonPressEvent(int buttonnumber);
   void  SbButtonReleaseEvent(int buttonnumber);

// Generated message map functions
protected:
	//{{AFX_MSG(CSbtestmfcView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in sbtestmfcView.cpp
inline CSbtestmfcDoc* CSbtestmfcView::GetDocument()
   { return (CSbtestmfcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SBTESTMFCVIEW_H__EC60F75D_86A7_11D1_A7B6_0060979D0B2E__INCLUDED_)
