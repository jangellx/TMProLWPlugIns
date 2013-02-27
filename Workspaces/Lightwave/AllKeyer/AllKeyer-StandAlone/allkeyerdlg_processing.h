#if !defined(AFX_CAllKeyerDlg_PROCESSING_H__8D32B51E_AE08_11D2_B82B_0040054088A2__INCLUDED_)
#define AFX_CAllKeyerDlg_PROCESSING_H__8D32B51E_AE08_11D2_B82B_0040054088A2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AllKeyerDlg_Processing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAllKeyerDlg_Processing dialog

class CAllKeyerDlg_Processing : public CDialog
{
// Construction
public:
	CAllKeyerDlg_Processing(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAllKeyerDlg_Processing)
	enum { IDD = IDD_ALLKEYER_PROCESSING };
	CProgressCtrl	 object_progress;
	CProgressCtrl	 scene_progress;
	CStatic	       object_status;
	CStatic	       to_name;
	CStatic	       from_name;
	CStatic	       compare_name;
	CString	       scene_status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllKeyerDlg_Processing)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAllKeyerDlg_Processing)
	afx_msg void OnAbort();
	afx_msg void OnPause();
	afx_msg void OnSkip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAllKeyerDlg_PROCESSING_H__8D32B51E_AE08_11D2_B82B_0040054088A2__INCLUDED_)
