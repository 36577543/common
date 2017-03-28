// ..\..\common\LKComponent\GDI\LKAdWnd.cpp : 实现文件
//

#include "LKAdWnd.h"


// CLKAdWnd

IMPLEMENT_DYNAMIC(CLKAdWnd, CLKCtrlBase)

CLKAdWnd::CLKAdWnd()
: CLKCtrlBase(-1)
, m_pImg(0)
, m_bUrlIsValid(false)
, m_nIDTemplate(0)
, m_pDlgAd(0)
{

}

CLKAdWnd::~CLKAdWnd()
{
	if(m_pDlgAd)
	{
		delete m_pDlgAd;
	}
}

BOOL CLKAdWnd::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, CLKImage *pImg)
{
	m_pImg = pImg;
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN/**/, rect, pParentWnd, nID); 
	return bRet;
}

BEGIN_MESSAGE_MAP(CLKAdWnd, CLKCtrlBase)
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CLKAdWnd 消息处理程序
LRESULT CLKAdWnd::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKADDLG_MSG_WPARAM_SUCCESS == wParam)
	{
		m_bUrlIsValid = true;
		m_pDlgAd->ShowWindow(SW_SHOW);
		Invalidate(false);
		return 1;
	}
	return CLKCtrlBase::OnCommonMsg(wParam, lParam);
}

// 在缓存上绘图
void CLKAdWnd::OnPaint(CDC *pDC)
{
	if(m_pImg && m_pImg->IsValid() && !m_bUrlIsValid)
	{
		if(m_pImg->GetWidth() >= m_rtClient.Width() && m_pImg->GetHeight() >= m_rtClient.Height())
		{
			m_pImg->DrawToDC(pDC, m_rtClient, 0);
		}else
		{
			CRect rt(m_rtClient);
			if(m_pImg->GetWidth() < m_rtClient.Width())
			{
				rt.left = (m_rtClient.Width() - m_pImg->GetWidth()) / 2;
				rt.right = rt.left + m_pImg->GetWidth();
			}
			if(m_pImg->GetHeight() < m_rtClient.Height())
			{
				rt.top = (m_rtClient.Height() - m_pImg->GetHeight()) / 2;
				rt.bottom = rt.top + m_pImg->GetHeight();
			}
			m_pImg->DrawToDC(pDC, rt, 0);
		}
	}
}

// 创建控件后初始化
void CLKAdWnd::InitCtrlAfterCreate()
{
	//InitHtmlDlg();
}

// 刷新前景缓存
void CLKAdWnd::ReSetMem()
{
	if(GetSafeHwnd() /*&& IsWindowVisible()*/)
	{
		if(m_pDlgAd && m_pDlgAd->GetSafeHwnd() /*&& m_pDlgAd->IsWindowVisible()*/)
		{
			int nL = (m_rtClient.Width() - 262) / 2;
            int nT = (m_rtClient.Height() - 79) / 2;
			Invalidate();
			m_pDlgAd->SetWindowPos(0, nL, nT, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}else
		{
		    Invalidate();
		}
	}
}

LRESULT CLKAdWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_PAINT)
	{
		PAINTSTRUCT paint;
		CDC *pDC = BeginPaint(&paint);
		DWORD dw = ::GetTickCount();
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDC, m_rtClient.Width(), m_rtClient.Height());
		CBitmap *pOldBmp = memDC.SelectObject(&bmp);
		memDC.IntersectClipRect(&paint.rcPaint);
		if(m_bEraseBG)
		{
			m_bEraseBG = false;
			BGParam prm;
			prm.rt = CRect(0, 0, 0, 0);

			GetParentImage(&prm);
			CRect rt;
			GetRectInParent(&rt);
			prm.rt.OffsetRect(rt.left, rt.top);
			CLKImage *pImg = (CLKImage *)prm.pBG;
			if(pImg)
			{
				pImg->BitBlt(memDC.GetSafeHdc(), 0, 0, m_rtClient.Width(), m_rtClient.Height(), prm.rt.left, prm.rt.top, SRCCOPY);
			}
		}else
		{
			memDC.BitBlt(0, 0, m_rtClient.Width(), m_rtClient.Height(), pDC, 0, 0, SRCCOPY);
		}
		static int n = 0;
		int nWidth = paint.rcPaint.right - paint.rcPaint.left;
		int nHeight = paint.rcPaint.bottom - paint.rcPaint.top;
		int nLeft = paint.rcPaint.left + m_rtClientOffset.left;
		int nTop = paint.rcPaint.top + m_rtClientOffset.top;
		OnPaint(&memDC);
		pDC->BitBlt(0, 0, m_rtClient.Width(), m_rtClient.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBmp);
		bmp.DeleteObject();
		memDC.DeleteDC();
		EndPaint(&paint);
		return 1;
	}else if(message == WM_ERASEBKGND)
	{
		m_bEraseBG = true;
		return 1;
	}else if(WM_MOVE == message)
	{
		ReSetMem();
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CLKAdWnd::InitHtmlDlg()
{
	if(!m_pDlgAd && !m_strUrl.IsEmpty() && GetSafeHwnd() && m_nIDTemplate)
	{
		m_pDlgAd = new CLKAdDialog;
		m_pDlgAd->SetUrlString(m_strUrl);
		m_pDlgAd->Create(m_nIDTemplate, this);
		m_pDlgAd->ShowWindow(SW_HIDE);
		CRect rt(0, 0, 262, 79);
		int nL = (m_rtClient.Width() - rt.Width()) / 2;
        int nT = (m_rtClient.Height() - rt.Height()) / 2;
		rt.OffsetRect(nL, nT);
		m_pDlgAd->MoveWindow(&rt);
		//m_pDlgAd->Navigate(m_strUrl);
	}
}

void CLKAdWnd::OnSize(UINT nType, int cx, int cy)
{
	//GetClientRect(&m_rtClient);
	CLKCtrlBase::OnSize(nType, cx, cy);
	//CWnd::OnSize(nType, cx, cy);

}
