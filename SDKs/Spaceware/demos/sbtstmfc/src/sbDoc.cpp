// sbtestmfcDoc.cpp : implementation of the CSbtestmfcDoc class
//

#include "stdafx.h"
#include "sbtstmfc.h"

#include "sbDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcDoc

IMPLEMENT_DYNCREATE(CSbtestmfcDoc, CDocument)

BEGIN_MESSAGE_MAP(CSbtestmfcDoc, CDocument)
	//{{AFX_MSG_MAP(CSbtestmfcDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcDoc construction/destruction

CSbtestmfcDoc::CSbtestmfcDoc()
{
	// TODO: add one-time construction code here

}

CSbtestmfcDoc::~CSbtestmfcDoc()
{
}

BOOL CSbtestmfcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
   
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
   SetTitle("SpaceWare");
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcDoc serialization

void CSbtestmfcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcDoc diagnostics

#ifdef _DEBUG
void CSbtestmfcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSbtestmfcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSbtestmfcDoc commands
