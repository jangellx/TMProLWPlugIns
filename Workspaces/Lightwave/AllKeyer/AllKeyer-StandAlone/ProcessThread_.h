#if !defined(AFX_PROCESSTHREAD_H__6001A894_B1D6_11D2_B82D_0040054088A2__INCLUDED_)
#define AFX_PROCESSTHREAD_H__6001A894_B1D6_11D2_B82D_0040054088A2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProcessThread.h : header file
//

// Processing Step Enumeration
//  These enums are used by DoStep() in CProcessThread() and various
//  in SceneAllKey and ObjectComparer functions.
enum StepEnum {
  STEP_UNKNOWN    = -2,
  STEP_ALL_DONE   =  1,  // Returned when finished processing
  STEP_ERROR      =  0,  // Error
  STEP_INITIAL    =  1,  // First time DoStep is called (does 
  STEP_NEXT_SCENE =  2,  // Each successive time DoStep is called
  STEP_OPEN_SOURCE,
  STEP_OPEN_DESTINATION,
  STEP_FIND_HIGH_FRAME,
  STEP_FIND_NEXT_OBJECT,
  STEP_LOAD_MOTION,
  STEP_ALL_KEY_MOTION,
  STEP_LINEAR_APROXIMATE_MOTION,
  STEP_SAVE_MOTION
};

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
	virtual int  ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CProcessThread();

	// Generated message map functions
	//{{AFX_MSG(CProcessThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSTHREAD_H__6001A894_B1D6_11D2_B82D_0040054088A2__INCLUDED_)
