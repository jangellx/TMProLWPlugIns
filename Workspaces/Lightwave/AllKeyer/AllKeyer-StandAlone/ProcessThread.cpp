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

CProcessThread::CProcessThread()
  : comparer(NULL), scene(NULL), gui_data(NULL) {
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
  // MFC UI processing
	if( CWinThread::OnIdle(lCount) )
    return TRUE;

  // LWS Processing
  return TRUE;  // return TRUE until STEP_ALL_DONE is returned by DoStep()
}

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

  case STEP_SCENE_ERROR:    // Got an Error; we don't do anything with this, either
    return STEP_SCENE_ERROR;

  case STEP_FATAL_ERROR:    // Got an Error; we don't do anything with this, either
    return STEP_FATAL_ERROR;

  // Setup
  case STEP_INITIAL:          // Open the compare file and store it's contents
    return OpenLog();

  case STEP_LOAD_COMPARE:     // Open the compare file and store it's contents
    return LoadCompare();

  case STEP_GET_SOURCE_LIST:
    return GetSourceList();   // Get a list of all the source files

  // Scene Processing
  case STEP_OPEN_SOURCE:       // Open the next source file found
    return OpenSource();

  case STEP_OPEN_DESTINATION:  // Open the output file
    return OpenDestination();

  case STEP_FIND_HIGH_FRAME:   // Last Key/Last Frame Test.  Also counts the
    return FindHighFrame();    //  number of objects in a scene.

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


// OpenLog():  Open the loh file
int CProcessThread::OpenLog() {
  return STEP_GET_SOURCE_LIST;
}

// LoadCompare():  Load the compare file
int CProcessThread::LoadCompare() {
  return STEP_GET_SOURCE_LIST;
}

// GetSourceList():  Obtain a list of all the files matching the
//                   source pattern.  Returns STEP_OPEN_SOURCE if
//                   any files match the pattern.  Otherwise,
//                   STEP_FATAL_ERROR is reported and processing
//                   stops.
int CProcessThread::GetSourceList() {
  if( 1 ) // At least 1 scene was found
    return STEP_OPEN_SOURCE;
  else
    return STEP_FATAL_ERROR;
}

// OpenSource():  Open the input file.  Returns STEP_ALL_DONE if no
//                more matches are found (finished processing
//                all scenes).  Otherwise, STEP_OPEN_DESTINATION
//                is called and the new scene is processed.
//                Returns STEP_SCENE_ERROR if there was an error
//                opening the source file.
int CProcessThread::OpenSource() {
  if( 1 ) // Scene found
    if( 1 ) // Scene opened successfully
      return STEP_OPEN_DESTINATION;
    else
      return STEP_SCENE_ERROR;
  else
    return STEP_ALL_DONE;
}

// OpenDestination():  Open the output file.  Returns STEP_FIND_HIGH_FRAME
//                     if the output file was opened without probelms.
//                     Returns STEP_SCENE_ERROR if there was an error
//                     opening the source file.
int CProcessThread::OpenDestination() {
  if( 1 ) // Output File Opened Sucessfully
    return STEP_FIND_HIGH_FRAME;
  else
    return STEP_SCENE_ERROR;
}

// FindHighFrame():  Find the highest keyframe in the scene.
//                   Also obtains a count of the objects in the scene.
int CProcessThread::FindHighFrame() {
  return STEP_FIND_NEXT_OBJECT;
}

// FindNextObject():  Find the next object in the scene.
//  This function will return either STEP_OPEN_SOURCE if no more objects
//   are found in the scene file (ie:  the scene is now completely
//   all-keyed), or STEP_LOAD_MOTION if one is found and needs to be
//   processed.
int CProcessThread::FindNextObject() {
  if( 0 ) // Error searching scene
    return STEP_SCENE_ERROR;
  else {
    if( 1 ) // Object found
      return STEP_LOAD_MOTION;
    else
      return STEP_OPEN_SOURCE;
  }
}

// LoadMotion():  Load the found object's motion.
int CProcessThread::LoadMotion() {
  if( 0 ) // Error loading motion
    return STEP_SCENE_ERROR;
  else
    return STEP_ALL_KEY_MOTION;
}

// AllKeyMotion():  All-key the loaded motion.
int CProcessThread::AllKeyMotion() {
  return STEP_LINEAR_APROXIMATE_MOTION;
}

// LinearAproximateMotion():  Linearly Aproximate the all-keyed motion.
int CProcessThread::LinearAproximateMotion() {
    return STEP_SAVE_MOTION;
}

// SaveMotion():  Save the All-Keyed/Linearly Aproximated motion.
//  Returns STEP_FIND_NEXT_OBJECT, thus looping until the last object
//  is found and processed.
int CProcessThread::SaveMotion() {
  if( 0 ) // Error saving motion
    return STEP_SCENE_ERROR;
  else
    return STEP_OPEN_SOURCE;
}

