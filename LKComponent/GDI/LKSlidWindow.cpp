// ..\..\..\common\LKComponent\gdi\LKSlidWindow.cpp : 实现文件
//

#include "LKSlidWindow.h"
//#include "LKImage.h"


// CLKSlidWindow 对话框

IMPLEMENT_DYNAMIC(CLKSlidWindow, CDialog)

CLKSlidWindow::CLKSlidWindow(CWnd* pParent /*=NULL*/)
	: CDialog()
{
    m_pParent = pParent;
}

CLKSlidWindow::~CLKSlidWindow()
{
}

bool CLKSlidWindow::Create(CRect rtWnd)
{
    char pTemp[1024] = {0};
    DLGTEMPLATE *pDlgTemplate = (DLGTEMPLATE *)pTemp;
    pDlgTemplate->style = WS_POPUPWINDOW | DS_MODALFRAME | WS_DLGFRAME | WS_DISABLED/* | WS_VISIBLE*/;
    pDlgTemplate->dwExtendedStyle	= WS_EX_NOACTIVATE;
    //pDlgTemplate->cdit = 0;

    //pDlgTemplate->x = rtWnd.left;
    //pDlgTemplate->y = rtWnd.top;
    //pDlgTemplate->cx = rtWnd.Width();
    //pDlgTemplate->cy = rtWnd.Height();
    pDlgTemplate->x = 100;

    if(!CreateIndirect(pDlgTemplate))
    {
        MessageBox(_T(""));
        return false;
    }
    SetWindowPos(&CWnd::wndTopMost, rtWnd.left, rtWnd.top, rtWnd.Width(), rtWnd.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
    //MoveWindow(&rtWnd, false);
    DoUpdateDummyDialog();
    return true;
}

BOOL CLKSlidWindow::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
    return CDialog::Create(nIDTemplate, pParentWnd);
}

// 滑上
void CLKSlidWindow::SlidUp(CWnd *pWnd, CWnd *pCur, CWnd *pPre)
{
    pPre->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    pCur->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    pWnd->ShowWindow(SW_HIDE);
    pWnd->GetParent()->RedrawWindow();

    CRect rtCur;
    pCur->GetWindowRect(&rtCur);
    int nOffset = rtCur.Height() / 10 + rtCur.Width() / 60;
    CRect rtPre(rtCur);
    CRect rtNext(rtCur);
    rtNext.OffsetRect(0, rtCur.Height());

    int nH(nOffset);
    while(rtNext.top > rtCur.top)
    {
        nH = rtNext.top - rtCur.top;
        nH = nH > nOffset ? nOffset : nH;
        rtNext.OffsetRect(0, -nH);
        rtPre.OffsetRect(0, -nH);

		CRgn rgn1, rgn2;
		CRect rc(rtCur);
		rc.IntersectRect(&rtCur, &rtPre);
        //rc -= rc.TopLeft();
        rc.MoveToXY(0, rtCur.top - rtPre.top);
        //rgn1.CreateRectRgnIndirect(&rc);
		rgn1.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //创建一个圆角矩形
		pPre->SetWindowRgn(rgn1,false); 
        //((CLKSlidWindow *)pPre)->DoUpdateDummyDialog();

		rc.IntersectRect(&rtCur, &rtNext);
		rc -= rc.TopLeft();
		rgn2.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //创建一个圆角矩形
		if(!pCur->SetWindowRgn(rgn2,false))
        {
        }
        //((CLKSlidWindow *)pCur)->DoUpdateDummyDialog();

        //pPre->MoveWindow(&rtPre, false);
        //pCur->MoveWindow(&rtNext, false);
        pPre->SetWindowPos(&CWnd::wndTopMost, rtPre.left, rtPre.top, 0, 0, SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        pCur->SetWindowPos(&CWnd::wndTopMost, rtNext.left, rtNext.top, 0, 0, SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        Sleep(10);
    }
    pWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    pCur->ShowWindow(SW_HIDE);
}

// 滑下
void CLKSlidWindow::SlidDown(CWnd *pWnd, CWnd *pCur, CWnd *pNext)
{
    pNext->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    pCur->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    pWnd->ShowWindow(SW_HIDE);
    pWnd->GetParent()->RedrawWindow();

    CRect rtCur;
    pCur->GetWindowRect(&rtCur);
    int nOffset = rtCur.Height() / 10 + rtCur.Width() / 60;
    CRect rtPre(rtCur);
    CRect rtNext(rtCur);
    rtPre.OffsetRect(0, -rtCur.Height());

    int nH(nOffset);
    while(rtPre.top < rtCur.top)
    {
        nH = rtCur.top - rtPre.top;
        nH = nH > nOffset ? nOffset : nH;
        rtNext.OffsetRect(0, nH);
        rtPre.OffsetRect(0, nH);

		CRgn rgn1, rgn2;
		CRect rc(rtCur);
		rc.IntersectRect(&rtCur, &rtNext);
		rc -= rc.TopLeft();
		rgn1.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //创建一个圆角矩形
		//pNext->SetWindowRgn(rgn1,false); 
		if(!pNext->SetWindowRgn(rgn2,false))
        {
        }

		rc.IntersectRect(&rtCur, &rtPre);
        rc.MoveToXY(0, rtCur.top - rtPre.top);
		rgn2.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //创建一个圆角矩形
		//pCur->SetWindowRgn(rgn2,false); 
		if(!pCur->SetWindowRgn(rgn2,false))
        {
        }

        //pNext->MoveWindow(&rtNext, false);
        //pCur->MoveWindow(&rtPre, false);
        pNext->SetWindowPos(&CWnd::wndTopMost, rtNext.left, rtNext.top, 0, 0, SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        pCur->SetWindowPos(&CWnd::wndTopMost, rtPre.left, rtPre.top, 0, 0, SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        Sleep(10);
    }
    pWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    pCur->ShowWindow(SW_HIDE);
}

BEGIN_MESSAGE_MAP(CLKSlidWindow, CDialog)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CLKSlidWindow 消息处理程序

BOOL CLKSlidWindow::OnInitDialog()
{
    CDialog::OnInitDialog();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CLKSlidWindow::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    //if(GetSafeHwnd())
    //{
    //    CRect rtWnd;
    //    GetWindowRect(&rtWnd);
    //    if(!rtWnd.IsRectEmpty())
    //    {
    //        DoUpdateDummyDialog();
    //    }
    //}
}

bool CLKSlidWindow::DoUpdateDummyDialog()
{
    CRect rtWnd;
    GetWindowRect(&rtWnd);
    BYTE SourceConstantAlpha = 255;
	// make sure the window has the WS_EX_LAYERED style
	ModifyStyleEx(0, WS_EX_LAYERED);

	// Create/setup the DC's
	CDC dcScreen;
	CDC dcMemory;

	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);
    CBitmap bmpx;
    bmpx.CreateCompatibleBitmap(&dcScreen, rtWnd.Width(), rtWnd.Height()); 

	CBitmap *pOldBitmap= dcMemory.SelectObject(&bmpx);

    OnDraw(&dcMemory);
    //pImg->Draw(dcMemory.GetSafeHdc(), 0, 0);

	// get the window rectangule (we are only interested in the top left position)
	CRect rectDlg(rtWnd);
	//GetWindowRect(rectDlg);

	// calculate the new window position/size based on the bitmap size
	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(rtWnd.Width(), rtWnd.Height());


	// setup the blend function
	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0,0); // start point of the copy from dcMemory to dcScreen

	// perform the alpha blend
    BOOL bRet= ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
    if(bRet == 0)
    {
        TRACE(_T("UpdateLayeredWindow %d\r\n"), ::GetLastError());
        return false;
    }

	//ASSERT(bRet); // ops... something bad just occured....
	// clean up
	dcMemory.SelectObject(pOldBitmap);
    return true;
}

void CLKSlidWindow::OnDraw(CDC *pDC)
{
    if(m_pParent)
    {
        BGParam rm;
        rm.pBG = pDC;
        GetClientRect(&rm.rt);
        ::SendMessage(m_pParent->GetSafeHwnd(), LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_DRAWDCNOBG, (LPARAM)&rm);
    }
}

BOOL CLKSlidWindow::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CDialog::PreCreateWindow(cs);
}
