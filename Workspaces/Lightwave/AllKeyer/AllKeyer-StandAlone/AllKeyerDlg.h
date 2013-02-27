// AllKeyerDlg.h : header file
//

#if !defined(AFX_ALLKEYERDLG_H__8D32B512_AE08_11D2_B82B_0040054088A2__INCLUDED_)
#define AFX_ALLKEYERDLG_H__8D32B512_AE08_11D2_B82B_0040054088A2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Includes
#include "ProcessScene.h"

/////////////////////////////////////////////////////////////////////////////
// CAllKeyerDlg dialog

class CAllKeyerDlg : public CDialog {
// Construction
public:
	void DoAboutBox();
	GUIData * gui_data;
	CBitmap open_file_bitmap;
	CAllKeyerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAllKeyerDlg)
	enum { IDD = IDD_ALLKEYER_MAIN };
	CButton	   log_append;
	CButton    open_source;
	CButton    open_log;
	CButton    open_destination;
	CButton    open_compare;
	CEdit	     compare_name;
	CStatic	   status_line;
	CEdit  	   source_name;
	CEdit	     log_name;
	CEdit	     destination_name;
	CComboBox  key_mode;
	CComboBox	 log_mode;
	CComboBox	 compare_mode;
	CButton	   use_frame_0;
	CButton	   pause_on_error;
	CButton	   last_key;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllKeyerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAllKeyerDlg)
	virtual BOOL     OnInitDialog();
	afx_msg void     OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void     OnDestroy();
	afx_msg void     OnPaint();
	afx_msg HCURSOR  OnQueryDragIcon();
	afx_msg void     OnOpenCompare();
	afx_msg void     OnOpenDestination();
	afx_msg void     OnOpenLogfile();
	afx_msg void     OnOpenSource();
	virtual void     OnCancel();
	virtual void     OnOK();
	afx_msg void OnAboutBox();
	afx_msg void OnSaveSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALLKEYERDLG_H__8D32B512_AE08_11D2_B82B_0040054088A2__INCLUDED_)
