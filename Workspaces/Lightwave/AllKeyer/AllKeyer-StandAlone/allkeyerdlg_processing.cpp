// AllKeyerDlg_Processing.cpp : implementation file
//

#include "stdafx.h"
#include "AllKeyer.h"
#include "AllKeyerDlg_Processing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllKeyerDlg_Processing dialog


CAllKeyerDlg_Processing::CAllKeyerDlg_Processing(CWnd* pParent /*=NULL*/)
	: CDialog(CAllKeyerDlg_Processing::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAllKeyerDlg_Processing)
	scene_status = _T("");
	//}}AFX_DATA_INIT
}


void CAllKeyerDlg_Processing::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAllKeyerDlg_Processing)
	DDX_Control(pDX, IDC_PROGRESS_OBJECT, object_progress);
	DDX_Control(pDX, IDC_PROGRESS_SCENE, scene_progress);
	DDX_Control(pDX, IDC_RO_OBJECT, object_status);
	DDX_Control(pDX, IDC_RO_TO, to_name);
	DDX_Control(pDX, IDC_RO_FROM, from_name);
	DDX_Control(pDX, IDC_RO_COMPARE, compare_name);
	DDX_Text(pDX, IDC_RO_SCENE, scene_status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAllKeyerDlg_Processing, CDialog)
	//{{AFX_MSG_MAP(CAllKeyerDlg_Processing)
	ON_BN_CLICKED(ID_ABORT, OnAbort)
	ON_BN_CLICKED(ID_PAUSE, OnPause)
	ON_BN_CLICKED(ID_SKIP,  OnSkip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllKeyerDlg_Processing message handlers

void CAllKeyerDlg_Processing::OnAbort() 
{
	// TODO: Add your control notification handler code here
	
}

void CAllKeyerDlg_Processing::OnPause() 
{
	// TODO: Add your control notification handler code here
	
}

void CAllKeyerDlg_Processing::OnSkip() 
{
	// TODO: Add your control notification handler code here
	
}
