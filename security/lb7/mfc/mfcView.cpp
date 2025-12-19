
// mfcView.cpp : implementation of the CmfcView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "mfc.h"
#endif

#include "mfcDoc.h"
#include "mfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmfcView

IMPLEMENT_DYNCREATE(CmfcView, CView)

BEGIN_MESSAGE_MAP(CmfcView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CmfcView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CmfcView construction/destruction

CmfcView::CmfcView() noexcept
{
	// TODO: add construction code here

}

CmfcView::~CmfcView()
{
}

BOOL CmfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CmfcView drawing

void CmfcView::OnDraw(CDC* /*pDC*/)
{
	CmfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CmfcView printing


void CmfcView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CmfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CmfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CmfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CmfcView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CmfcView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CmfcView diagnostics

#ifdef _DEBUG
void CmfcView::AssertValid() const
{
	CView::AssertValid();
}

void CmfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmfcDoc* CmfcView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmfcDoc)));
	return (CmfcDoc*)m_pDocument;
}
#endif //_DEBUG


// CmfcView message handlers
