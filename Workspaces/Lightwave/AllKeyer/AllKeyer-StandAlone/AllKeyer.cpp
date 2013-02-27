// AllKeyer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AllKeyer.h"
#include "AllKeyerDlg.h"
#include "AllKeyerDlg_Processing.h"
#include "ProcessScene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllKeyerApp

BEGIN_MESSAGE_MAP(CAllKeyerApp, CWinApp)
	//{{AFX_MSG_MAP(CAllKeyerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllKeyerApp construction

CAllKeyerApp::CAllKeyerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAllKeyerApp object

CAllKeyerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAllKeyerApp initialization

BOOL CAllKeyerApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CAllKeyerDlg            main_dlg;   // Main Window
  CAllKeyerDlg_Processing proc_dlg;   // Progress Window

  // Create the GUIData stuff
  GUIData gui_data;
  main_dlg.gui_data = &gui_data;

  // Loop handles opening the Main and Proccessing windows as needed.
  //  Program exits on anything except an IDOK (ie:  IDCANCEL or an error
  //  code forces a quit).
  // Note that the Processing function is called from proc_dlg, not from
  //  this function.
  int retval;
  do {
    retval = main_dlg.DoModal();
    if( retval == IDOK ) {  // Process Scenes button pressed; switch panels and process
      proc_dlg.DoModal();
    }
  } while( retval == IDOK );
  
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
