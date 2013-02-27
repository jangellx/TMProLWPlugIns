// ProcessThread.cpp : implementation file
//

#include "stdafx.h"
#include "AllKeyer.h"
#include "ProcessThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessThread

IMPLEMENT_DYNCREATE(CProcessThread, CWinThread)

CProcessThread::CProcessThread() {
}

CProcessThread::~CProcessThread() {
}

BOOL CProcessThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CProcessThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProcessThread, CWinThread)
	//{{AFX_MSG_MAP(CProcessThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessThread message handlers

BOOL CProcessThread::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinThread::OnIdle(lCount);
}

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

// DoStep( int )
//   This allows MFC to handle UI messages, and should be called from the OnIdle().
//   function of a CWinThread class.  The only argument is which
//    step of the process to perform.  The first time this function is
//    called, STEP_INITIAL should be passed in.
//   The return codes of each function called are returned to the calling
//    function:
//    - STEP_ERROR: an error occured, and the process should
//      be aborted, or at the very least the current scene should be skipped
//      and the incomplete file removed.
//    - STEP_ALL_DONE: Finished all-keying, return to the main UI.
//    Any other return code should be passed back into DoStep().
int CProcessThread::DoStep(int step) {
  switch( step ) {
  case STEP_ALL_DONE: // Got an All-Done; we don't do anything with this
    return STEP_ALL_DONE;

  case STEP_ERROR:    // Got an Error; we don't do anything with this, either
    return STEP_ERROR;

  case STEP_INITIAL:           // Open the compare file and store it's contents
    return LoadCompare();

  case STEP_NEXT_SCENE:        // Find the next scene that matches the source path
    return FindNextMatch();

  case STEP_OPEN_SOURCE:       // Open the source file
    scene = new SceneAllKey
    return OpenSource();

  case STEP_OPEN_DESTINATION:  // Open the output file
    return OpenDestination();

  case STEP_FIND_HIGH_FRAME:   // Last Key/Last Frame Test
    return FindHighFrame();

  case STEP_FIND_NEXT_OBJECT:  // Find the next object in the source file
    return FindNextObject();

  case STEP_LOAD_MOTION:       // Load the object's motion
    return LoadMotion();

  case STEP_ALL_KEY_MOTION:    // All-Key the object's motion
    return AllKeyMotion();

  case STEP_LINEAR_APROXIMATE_MOTION:  // Load the object's motion
    return LinearAproximateMotion();

  case STEP_SAVE_MOTION:       // Save the object's all-keyed/linear-aproximated motion
    return SaveMotion();

  default:
    return( STEP_UNKNOWN );
  }
}
