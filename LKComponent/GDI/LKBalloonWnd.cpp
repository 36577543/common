// ..\..\common\LKComponent\GDI\LKBalloonWnd.cpp : 实现文件
//

#include "LKBalloonWnd.h"
#include "LKFontMgr.h"
#include "LKImageMgr.h"

// 注销定时器
#define	LKBALLOONWND_TIMER_DESTROY	1

// LKBalloonWnd

IMPLEMENT_DYNAMIC(CLKBalloonWnd, CWnd)

void CLKBalloonWnd::Show(CWnd *pParentWnd, CRect rt, CString strText, UINT nSecs, BOOL bBalloonUp)
{
	CLKBalloonWnd *pWnd = new CLKBalloonWnd;
	pWnd->m_strText = strText;
	pWnd->m_uDestroyTime = nSecs;
	pWnd->m_ptDlgPos = rt.TopLeft();
	//pWnd->Create(strText, rt, GetDesktopWindow());
	if(pParentWnd)
	{
		pParentWnd->ScreenToClient(&rt);
	}
	pWnd->Create(strText, rt, pParentWnd);
}

void CLKBalloonWnd::Hide()
{
	CWnd *pWnd = CWnd::FromHandle(::GetCapture());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		if(pWnd->IsKindOf(RUNTIME_CLASS(CLKBalloonWnd)))
		{
			ReleaseCapture();
		}
	}
}

CLKBalloonWnd::CLKBalloonWnd()
: m_pImgBG(0)
, m_uDestroyTime(0)
, m_colText(RGB(43, 85, 128))
, m_nLeftMargin(2)
, m_ptDlgPos(0, 0)
{
	m_pImgBG = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_DIALOGBALLOON);
}

CLKBalloonWnd::~CLKBalloonWnd()
{
}

BOOL CLKBalloonWnd::Create(LPCTSTR lpszText, const RECT& rect, CWnd* pParentWnd)
{
	CRect rt(rect);
	int nLen = GetTextExtent();
	int nHeight = GetTextHeight();
	rt.right = rt.left + nLen + 11;
	rt.bottom = rt.top + nHeight + 15 + 3;
	BOOL bRet = CWnd::Create(AfxRegisterWndClass(NULL), _T("CLKBalloonWnd"), WS_CHILD | WS_VISIBLE, rt, pParentWnd, 65535);
	return bRet;
}

BEGIN_MESSAGE_MAP(CLKBalloonWnd, CWnd)
    ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
	ON_WM_PAINT()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
//	ON_WM_NCDESTROY()
END_MESSAGE_MAP()



// LKBalloonWnd 消息处理程序
LRESULT CLKBalloonWnd::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKBALLOONWND_MSG_WPARAM_CLOSE == wParam)
	{
		DestroyWindow();
	}
	return 1;
}


void CLKBalloonWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	rt;
	GetClientRect(&rt);
	//dc.SetBkMode(TRANSPARENT);
    CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap Screen;
	Screen.CreateCompatibleBitmap(&dc, rt.Width(), rt.Height());
	CBitmap *pOld = dcMem.SelectObject(&Screen);
	dcMem.SetBkMode(TRANSPARENT);
	// 画背景框架
	DrawDlgBG(&dcMem, rt);
	// 画文本
	DrawInfoText(&dcMem, rt);
	dc.BitBlt(0, 0, rt.Width(), rt.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOld);
	Screen.DeleteObject();
	dcMem.DeleteDC();
}

void CLKBalloonWnd::OnCaptureChanged(CWnd *pWnd)
{
	// TODO: 在此处添加消息处理程序代码
	PostMessage(LKCOMMON_MSG, LKBALLOONWND_MSG_WPARAM_CLOSE, 0);
	CWnd::OnCaptureChanged(pWnd);
}

void CLKBalloonWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CWnd::OnLButtonDown(nFlags, point);
	ClientToScreen(&point);
	::mouse_event(MOUSEEVENTF_LEFTDOWN, point.x, point.y, 0, 0);
}

void CLKBalloonWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CWnd::OnRButtonDown(nFlags, point);
	ClientToScreen(&point);
	::mouse_event(MOUSEEVENTF_RIGHTDOWN, point.x, point.y, 0, 0);
}

void CLKBalloonWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if(bShow)
	{
		SetCapture();
		if(m_uDestroyTime)
		{
			SetTimer(LKBALLOONWND_TIMER_DESTROY, m_uDestroyTime * 1000, 0);
		}
	}
}

void CLKBalloonWnd::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch(nIDEvent)
	{
	// 注销
	case LKBALLOONWND_TIMER_DESTROY:
		ReleaseCapture();
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CLKBalloonWnd::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd::PostNcDestroy();
	delete this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 私有函数
///////////////////////////////////////////////////////////////////////////////////////////////////

// 画窗体背景
void CLKBalloonWnd::DrawDlgBG(CDC *pDC, CRect rtClient)
{
	if(m_pImgBG->IsValid())
	{
		// 屏幕ＤＣ
		CWnd *pWnd = GetDesktopWindow();
		CDC *pWndDC = pWnd->GetDC();
		if(pWndDC && pWndDC->GetSafeHdc())
		{
			pDC->BitBlt(0, 0, rtClient.Width(), rtClient.Height(), pWndDC, m_ptDlgPos.x, m_ptDlgPos.y, SRCCOPY);
			pWnd->ReleaseDC(pWndDC);
		}
		m_pImgBG->DrawToDC(pDC, rtClient, 0, LKSKINBASE_DRAWBGSTYLE_ALL);
	}
}

// 画文本
void CLKBalloonWnd::DrawInfoText(CDC *pDC, CRect rtClient)
{
	if(m_strText.IsEmpty()) return;
	CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST90());
	pDC->SetTextColor(m_colText);
	rtClient.left += m_nLeftMargin;
	if(m_pImgBG->IsValid())
	{
		CRect rtImg = m_pImgBG->GetFrameRect();
		rtClient.top += rtImg.top;
		rtClient.bottom -= rtImg.bottom - 2;
	}
	pDC->DrawText(m_strText, rtClient, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	pDC->SelectObject(pOld);
}

// 获取提示字符所占长度
int CLKBalloonWnd::GetTextExtent()
{
	CWnd *pWnd = GetDesktopWindow();
	CDC *pDC = pWnd->GetDC();
	if(pDC && pDC->GetSafeHdc())
	{
		CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST90());
		CSize sz = pDC->GetOutputTextExtent(m_strText);
		pDC->SelectObject(pOld);
		pWnd->ReleaseDC(pDC);
		return sz.cx;
	}
	return 0;
}

// 获取提示字符所占高度
int CLKBalloonWnd::GetTextHeight()
{
	CWnd *pWnd = GetDesktopWindow();
	CDC *pDC = pWnd->GetDC();
	if(pDC && pDC->GetSafeHdc())
	{
		CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST90());
		CSize sz = pDC->GetOutputTextExtent(m_strText);
		pDC->SelectObject(pOld);
		pWnd->ReleaseDC(pDC);
		return sz.cy;
	}
	return 0;
}
