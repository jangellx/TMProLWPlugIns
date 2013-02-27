#if !defined(AFX_PROCESSTHREAD_H__6001A895_B1D6_11D2_B82D_0040054088A2__INCLUDED_)
#define AFX_PROCESSTHREAD_H__6001A895_B1D6_11D2_B82D_0040054088A2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProcessThread.h : header file
//
#include "ProcessScene.h"

/////////////////////////////////////////////////////////////////////////////
// CProcessThread thread
class CProcessThread : public CWinThread {
	DECLARE_DYNCREATE(CProcessThread)
protected:
	CProcessThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	int DoStep( int step );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CProcessThread();
	int OpenLog();
	int LoadCompare();
	int GetSourceList();
	int OpenSource();
 	int OpenDestination();
  int FindHighFrame();
  int FindNextObject();
  int LoadMotion();
  int AllKeyMotion();
  int LinearAproximateMotion();
  int SaveMotion();

  ObjectComparer *comparer;
  SceneAllKey    *scene;
  GUIData        *gui_data;

	// Generated message map functions
	//{{AFX_MSG(CProcessThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSTHREAD_H__6001A895_B1D6_11D2_B82D_0040054088A2__INCLUDED_)
