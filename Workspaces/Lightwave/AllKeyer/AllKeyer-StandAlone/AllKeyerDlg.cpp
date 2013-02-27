// AllKeyerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AllKeyer.h"
#include "AllKeyerDlg.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllKeyerDlg dialog

CAllKeyerDlg::CAllKeyerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAllKeyerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAllKeyerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon( IDI_MAINFRAME );
}

void CAllKeyerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAllKeyerDlg)
	DDX_Control(pDX, IDC_COMBO_KEY_MODE, key_mode);
	DDX_Control(pDX, IDC_CHECK_APPEND, log_append);
	DDX_Control(pDX, IDC_OPEN_SOURCE, open_source);
	DDX_Control(pDX, IDC_OPEN_LOGFILE, open_log);
	DDX_Control(pDX, IDC_OPEN_DESTINATION, open_destination);
	DDX_Control(pDX, IDC_OPEN_COMPARE, open_compare);
	DDX_Control(pDX, IDC_EDIT_COMPARE, compare_name);
	DDX_Control(pDX, IDC_RO_STATUS, status_line);
	DDX_Control(pDX, IDC_EDIT_SOURCE, source_name);
	DDX_Control(pDX, IDC_EDIT_LOGFILE, log_name);
	DDX_Control(pDX, IDC_EDIT_DESTINATION, destination_name);
	DDX_Control(pDX, IDC_COMBO_LOG_MODE, log_mode);
	DDX_Control(pDX, IDC_COMBO_COMPARE_MODE, compare_mode);
	DDX_Control(pDX, IDC_CHECK_USEFRAME0, use_frame_0);
	DDX_Control(pDX, IDC_CHECK_PAUSEONERRORS, pause_on_error);
	DDX_Control(pDX, IDC_CHECK_LASTKEY, last_key);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAllKeyerDlg, CDialog)
	//{{AFX_MSG_MAP(CAllKeyerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_COMPARE, OnOpenCompare)
	ON_BN_CLICKED(IDC_OPEN_DESTINATION, OnOpenDestination)
	ON_BN_CLICKED(IDC_OPEN_LOGFILE, OnOpenLogfile)
	ON_BN_CLICKED(IDC_OPEN_SOURCE, OnOpenSource)
	ON_COMMAND(IDM_ABOUTBOX, OnAboutBox)
	ON_COMMAND(IDM_SAVE_SETTINGS, OnSaveSettings)
	ON_BN_CLICKED(ID_ABOUTBOX, OnAboutBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllKeyerDlg message handlers

BOOL CAllKeyerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
  
  // Set the Combo Boxes to their default indices
  compare_mode.SetCurSel( gui_data->compare_mode );
  key_mode.SetCurSel(     gui_data->key_mode );
  log_mode.SetCurSel(     gui_data->log_mode );
  
  // Set the default Status Text
  CTime now = CTime::GetCurrentTime();
  CString status_text;
  status_text.LoadString( IDS_DEFAULT_STATUS_TEXT );
  status_text += now.Format( "%A, %B %d, %Y, %I:%M %p" );
  status_line.SetWindowText( status_text );

  // Default filenames
  compare_name.SetWindowText(     gui_data->compare_name     );
  source_name.SetWindowText(      gui_data->source_name      );
  destination_name.SetWindowText( gui_data->destination_name );
  log_name.SetWindowText(         gui_data->log_name         );

  // Default checks
  log_append.SetCheck(     gui_data->log_append     );
  pause_on_error.SetCheck( gui_data->pause_on_error );
  last_key.SetCheck(       gui_data->last_key       );
  use_frame_0.SetCheck(    gui_data->use_frame_0    );

  // Set the bitmaps for the Load Buttons
  open_file_bitmap.LoadBitmap( IDB_OPEN_FILE );
  open_compare.SetBitmap( (HBITMAP) open_file_bitmap );
  open_source.SetBitmap( (HBITMAP) open_file_bitmap );
  open_destination.SetBitmap( (HBITMAP) open_file_bitmap );
  open_log.SetBitmap( (HBITMAP) open_file_bitmap );
	return TRUE;  // return TRUE  unless you set the focus to a control

  char teststring[1024];
  sprintf( teststring, "Icon Pointer:  %d", m_hIcon );
  AfxMessageBox( teststring, MB_OK | MB_ICONEXCLAMATION );
}

void CAllKeyerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
    OnAboutBox();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CAllKeyerDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();

  // Delete the bitmap for the Open File buttons
  open_file_bitmap.DeleteObject();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAllKeyerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAllKeyerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// OnOpenCompare():  Open a file requester and set the compare filename
void CAllKeyerDlg::OnOpenCompare() {
  CFileDialog open( true );

  char compare_buffer[ FILENAME_LENGTH ];
  compare_name.GetWindowText( compare_buffer, FILENAME_LENGTH );

  open.m_ofn.lpstrTitle   = "Select a Compare File";
  open.m_ofn.lpstrFile    = compare_buffer;
  open.m_ofn.lpstrFilter  = "All Files\0*.*\0Lightwave Scene\0*.LWS\0\0";
  open.m_ofn.nFilterIndex = 1;
  open.m_ofn.Flags        = open.m_ofn.Flags | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
  
  if( open.DoModal() == IDOK ) {  // Open the requester and see if the user hit OK
    compare_name.SetWindowText( (LPCTSTR) open.GetPathName() );
  }
}

// OnOpenDestination():  Open a file requester and set the destination filename
void CAllKeyerDlg::OnOpenDestination() 
{
  CFileDialog open( true );

  char dest_buffer[ FILENAME_LENGTH ];
  destination_name.GetWindowText( dest_buffer, FILENAME_LENGTH );
  if( dest_buffer[ strlen( dest_buffer )-1 ] == '\\' )
    strcat( dest_buffer, "(none)" );

  open.m_ofn.lpstrTitle   = "Select a Destination Path";
  open.m_ofn.lpstrFile    = dest_buffer;
  open.m_ofn.lpstrFilter  = "All Files\0*.*\0\0";
  open.m_ofn.nFilterIndex = 1;
  open.m_ofn.Flags        = open.m_ofn.Flags | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
  
  if( open.DoModal() == IDOK ) {  // Open the requester and see if the user hit OK
    CString path = open.GetPathName();
    path.GetBufferSetLength( path.GetLength() - open.GetFileName().GetLength() );
    destination_name.SetWindowText( (LPCTSTR) path );
  }	
}

// OnOpenLog():  Open a file requester and set the log filename
void CAllKeyerDlg::OnOpenLogfile() {
  CFileDialog open( false );

  char log_buffer[ FILENAME_LENGTH ];
  log_name.GetWindowText( log_buffer, FILENAME_LENGTH );

  open.m_ofn.lpstrTitle   = "Select a Log File";
  open.m_ofn.lpstrFile    = log_buffer;
  open.m_ofn.lpstrFilter  = "All Files\0*.*\0\0";
  open.m_ofn.nFilterIndex = 1;
  open.m_ofn.Flags        = open.m_ofn.Flags | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
  
  if( open.DoModal() == IDOK ) {  // Open the requester and see if the user hit OK
    log_name.SetWindowText( (LPCTSTR) open.GetPathName() );
  }
}

// OnOpenSource():  Open a file requester and set the source filename
void CAllKeyerDlg::OnOpenSource() 
{
  CFileDialog open( true );

  char source_buffer[ FILENAME_LENGTH ];
  source_name.GetWindowText( source_buffer, FILENAME_LENGTH );

  open.m_ofn.lpstrTitle   = "Select a Source File";
  open.m_ofn.lpstrFile    = source_buffer;
  open.m_ofn.lpstrFilter  = "All Files\0*.*\0Lightwave Scene\0*.LWS\0\0";
  open.m_ofn.nFilterIndex = 1;
  open.m_ofn.Flags        = open.m_ofn.Flags | OFN_NOVALIDATE | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
  
  if( open.DoModal() == IDOK ) {  // Open the requester and see if the user hit OK
    source_name.SetWindowText( (LPCTSTR) open.GetPathName() );
  }	
}

void CAllKeyerDlg::OnCancel() 
{
  if( AfxMessageBox( "Are you sure you want to quit?", MB_YESNO ) == IDYES )
    CDialog::OnCancel();
}

void CAllKeyerDlg::OnOK() {
  // Get the Combo Box indices
  gui_data->compare_mode = compare_mode.GetCurSel();
  gui_data->key_mode     = key_mode.GetCurSel();
  gui_data->log_mode     = log_mode.GetCurSel();
  
  // Default filenames
  compare_name.GetWindowText(     gui_data->compare_name,     FILENAME_LENGTH );
  source_name.GetWindowText(      gui_data->source_name,      FILENAME_LENGTH );
  destination_name.GetWindowText( gui_data->destination_name, FILENAME_LENGTH );
  log_name.GetWindowText(         gui_data->log_name,         FILENAME_LENGTH );

  // Default checks
  gui_data->log_append     = log_append.GetCheck();
  gui_data->pause_on_error = pause_on_error.GetCheck();
  gui_data->last_key       = last_key.GetCheck();
  gui_data->use_frame_0    = use_frame_0.GetCheck();
  
	// Make sure filenames are valid before allowing processing 
  bool do_process = true;
  
  if( gui_data->compare_mode != CMODE_NONE ) {          // Test the Compare Name
    if( access( gui_data->compare_name, 0 ) == -1 ) {
      AfxMessageBox( IDS_BAD_COMPARE_NAME, MB_OK | MB_ICONEXCLAMATION );
      do_process = false;
    }
  }

  if( access( gui_data->destination_name, 0 ) == -1 ) { // Test the Destination Name
    AfxMessageBox( IDS_BAD_DESTINATION_NAME, MB_OK | MB_ICONEXCLAMATION );
    do_process = false;
  }

  WIN32_FIND_DATA source_data;  // Test the Source String
  HANDLE search_handle = FindFirstFile( gui_data->source_name, &source_data );
  if( search_handle == INVALID_HANDLE_VALUE ) {
    AfxMessageBox( IDS_BAD_SOURCE_NAME, MB_OK | MB_ICONEXCLAMATION );
    do_process = false;
  } else
    FindClose( search_handle );

  if( do_process == true )
    CDialog::OnOK();
}


// Pop open the About Box
void CAllKeyerDlg::OnAboutBox() 
{
  CAboutDlg dlgAbout;
  dlgAbout.DoModal();
}

void CAllKeyerDlg::OnSaveSettings() 
{
	// TODO: Add your command handler code here
	
}
