// ..\..\common\LKComponent\GDI\LKMessageBox.cpp : 实现文件
//

#include "LKImageMgr.h"
#include "LKMessageBox.h"
#include "LKFontMgr.h"


// CLKMessageBox 对话框
POINT CLKMessageBox::m_ptSize = CPoint(0, 0);

IMPLEMENT_DYNAMIC(CLKMessageBox, CLKMaskDialog)

int CLKMessageBox::Show(UINT nIDTemplate, CString lpszText, CString lpszCaption, CLKImage *pIcon, UINT nType, CWnd *pDlg)
{
	static bool bShow = false;
	if(bShow) return IDCANCEL;
	bShow = true;
	if(!pDlg) pDlg = CWnd::GetForegroundWindow();
    CLKMessageBox dlg(nIDTemplate, pDlg, lpszText, lpszCaption, pIcon, nType);
	int nRet = dlg.DoModal();
	bShow = false;
	CLKMessageBox::m_ptSize = CPoint(0, 0);
    return nRet;
}

CLKMessageBox::CLKMessageBox(UINT nIDTemplate, CWnd* pParent, CString strText, CString strCaption, CLKImage *pIcon, UINT nType /*=NULL*/)
: CLKMaskDialog(nIDTemplate, pParent)
, m_strText(strText)
, m_strCaption(strCaption)
, m_pIcon(pIcon)
, m_nType(nType)
, m_btnOK(2)
, m_btnCancel(2)
{
	InitImageList();
    //
    if(!m_pIcon)
    {
        m_pIcon = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_MESSAGEHINT);
    }
    //this->CreateDlgIndirect
    //::CreateWindow
}

CLKMessageBox::~CLKMessageBox()
{
}
 //   HINSTANCE hInst = ::AfxGetApp()->m_hInstance;
 //   DLGTEMPLATE dlgP;
 //   dlgP.cdit = 0;
 //   dlgP.x = 0;
 //   dlgP.y = 0;
 //   dlgP.cx = 110;
 //   dlgP.cy = 110;
 //   dlgP.dwExtendedStyle = 0;
 //   dlgP.style = WS_VISIBLE | DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU;
	//LPCDLGTEMPLATE lpDialogTemplate = &dlgP;

//#define _AFX_NO_OCC_SUPPORT

//INT_PTR CLKMessageBox::DoModal()
//{
//	// can be constructed with a resource template or InitModalIndirect
//	ASSERT(m_lpszTemplateName != NULL || m_hDialogTemplate != NULL ||
//		m_lpDialogTemplate != NULL);
//
//     //load resource as necessary
//	LPCDLGTEMPLATE lpDialogTemplate = m_lpDialogTemplate;
//	HGLOBAL hDialogTemplate = m_hDialogTemplate;
//	HINSTANCE hInst = AfxGetResourceHandle();
//	if (m_lpszTemplateName != NULL)
//	{
//		hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);
//		HRSRC hResource = ::FindResource(hInst, m_lpszTemplateName, RT_DIALOG);
//		hDialogTemplate = LoadResource(hInst, hResource);
//	}
//	if (hDialogTemplate != NULL)
//		lpDialogTemplate = (LPCDLGTEMPLATE)LockResource(hDialogTemplate);
//
//	// return -1 in case of failure to load the dialog template resource
//	if (lpDialogTemplate == NULL)
//		return -1;
//
//	// disable parent (before creating dialog)
//	HWND hWndParent = PreModal();
//	AfxUnhookWindowCreate();
//	BOOL bEnableParent = FALSE;
//#ifndef _AFX_NO_OLE_SUPPORT
//	CWnd* pMainWnd = NULL;
//	BOOL bEnableMainWnd = FALSE;
//#endif
//	if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
//	{
//		::EnableWindow(hWndParent, FALSE);
//		bEnableParent = TRUE;
//#ifndef _AFX_NO_OLE_SUPPORT
//		pMainWnd = AfxGetMainWnd();
//		if (pMainWnd && pMainWnd->IsFrameWnd() && pMainWnd->IsWindowEnabled())
//		{
//			//
//			// We are hosted by non-MFC container
//			// 
//			pMainWnd->EnableWindow(FALSE);
//			bEnableMainWnd = TRUE;
//		}
//#endif
//	}
//
//	TRY
//	{
//		// create modeless dialog
//		AfxHookWindowCreate(this);
//		if (CreateDlgIndirect(lpDialogTemplate,
//						CWnd::FromHandle(hWndParent), hInst))
//		{
//			if (m_nFlags & WF_CONTINUEMODAL)
//			{
//				// enter modal loop
//				DWORD dwFlags = MLF_SHOWONIDLE;
//				if (GetStyle() & DS_NOIDLEMSG)
//					dwFlags |= MLF_NOIDLEMSG;
//				VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
//			}
//
//			// hide the window before enabling the parent, etc.
//			if (m_hWnd != NULL)
//				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
//					SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
//		}
//	}
//	CATCH_ALL(e)
//	{
//		e->Delete();
//		m_nModalResult = -1;
//	}
//	END_CATCH_ALL
//
//#ifndef _AFX_NO_OLE_SUPPORT
//	if (bEnableMainWnd)
//		pMainWnd->EnableWindow(TRUE);
//#endif
//	if (bEnableParent)
//		::EnableWindow(hWndParent, TRUE);
//	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
//		::SetActiveWindow(hWndParent);
//
//	// destroy modal window
//	DestroyWindow();
//	PostModal();
//
//	 //unlock/free resources as necessary
//	if (m_lpszTemplateName != NULL || m_hDialogTemplate != NULL)
//		UnlockResource(hDialogTemplate);
//	if (m_lpszTemplateName != NULL)
//		FreeResource(hDialogTemplate);
//
//	return m_nModalResult;
//}
//
BEGIN_MESSAGE_MAP(CLKMessageBox, CLKMaskDialog)
END_MESSAGE_MAP()


// CLKMessageBox 消息处理程序

BOOL CLKMessageBox::OnInitDialog()
{
	if(CLKMessageBox::m_ptSize.x > 0 && CLKMessageBox::m_ptSize.y > 0)
	{
		CRect rtWnd(0, 0, 0, 0);
		GetWindowRect(&rtWnd);
		rtWnd.right = rtWnd.left + CLKMessageBox::m_ptSize.x;
		rtWnd.bottom = rtWnd.top + CLKMessageBox::m_ptSize.y;
		MoveWindow(&rtWnd);
	}
    CLKMaskDialog::OnInitDialog();
    SetWindowText(m_strCaption);
	
    // TODO:  在此添加额外的初始化
    CRect rtBtn;
    GetClientRect(&rtBtn);
    rtBtn.top = rtBtn.bottom - DEFAULT_BUTTONHEIGHT;
    if(MB_OKCANCEL == m_nType)
    {
        rtBtn.left = (rtBtn.Width() - 2 * DEFAULT_BUTTONWIDTH - 10) / 2;
        rtBtn.right = rtBtn.left + DEFAULT_BUTTONWIDTH;
        m_btnOK.Create(_T("确定"), rtBtn, this, IDOK);
        rtBtn.OffsetRect(DEFAULT_BUTTONWIDTH + 10, 0);
        m_btnCancel.Create(_T("取消"), rtBtn, this, IDCANCEL);
    }else
    {
        rtBtn.left = (rtBtn.Width() - DEFAULT_BUTTONWIDTH) / 2;
        rtBtn.right = rtBtn.left + DEFAULT_BUTTONWIDTH;
        m_btnOK.Create(_T("确定"), rtBtn, this, IDOK);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

// 初始化窗口前景图片
void CLKMessageBox::OnInitMemImg()
{
    CLKMaskDialog::OnInitMemImg();
    CRect rtText;
    GetClientRect(&rtText);
    rtText.bottom -= DEFAULT_BUTTONHEIGHT;
	CLKImage *pImg = GetMemImage();
    if(pImg && pImg->IsValid() && m_pIcon && m_pIcon->IsValid())
    {
        CRect rtIcon(rtText);
        rtIcon.top = (rtIcon.Height() - m_pIcon->GetHeight()) / 2;
        rtIcon.bottom = rtIcon.top + m_pIcon->GetHeight();
        rtIcon.left = 15;
        rtIcon.right = rtIcon.left + m_pIcon->GetWidth();
        rtIcon.OffsetRect(GetLeftFrameWidth(), GetTitleBarHeight());
        m_pIcon->DrawToImage(pImg, rtIcon);
        rtText.left = rtIcon.right;
    }
    rtText.left = 80;
    if(pImg && pImg->IsValid() && !m_strText.IsEmpty())
	{
        rtText.bottom += 10;
		UINT uFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
        rtText.OffsetRect(GetLeftFrameWidth(), GetTitleBarHeight());
		pImg->DrawText(m_strText, rtText, CLKFontMgr::GetST90(), uFormat);
	}
}
