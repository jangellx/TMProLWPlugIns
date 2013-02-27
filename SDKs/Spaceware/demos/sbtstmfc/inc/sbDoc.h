// sbtestmfcDoc.h : interface of the CSbtestmfcDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SBTESTMFCDOC_H__EC60F75B_86A7_11D1_A7B6_0060979D0B2E__INCLUDED_)
#define AFX_SBTESTMFCDOC_H__EC60F75B_86A7_11D1_A7B6_0060979D0B2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CSbtestmfcDoc : public CDocument
{
protected: // create from serialization only
	CSbtestmfcDoc();
	DECLARE_DYNCREATE(CSbtestmfcDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSbtestmfcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSbtestmfcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSbtestmfcDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SBTESTMFCDOC_H__EC60F75B_86A7_11D1_A7B6_0060979D0B2E__INCLUDED_)
