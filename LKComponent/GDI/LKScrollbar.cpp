// ..\..\..\common\LKComponent\GDI\LKScrollbar.cpp : 实现文件
//

#include "LKScrollbar.h"
#include "LKFontMgr.h"
#include "LKImageMgr.h"


// CLKScrollbar

IMPLEMENT_DYNAMIC(CLKScrollbar, CLKCtrlBaseEx)


LRESULT CALLBACK CLKScrollbar::HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	CLKScrollbar *pScrollbar=(CLKScrollbar*)GetWindowLong(hwnd,GWL_USERDATA);
	if(msg==WM_DESTROY)
	{
		WNDPROC procOld=pScrollbar->m_funOldProc;
		SetWindowLong(hwnd,GWL_WNDPROC,(LONG)pScrollbar->m_funOldProc);
		SetWindowLong(hwnd,GWL_USERDATA,0);
		return ::CallWindowProc(procOld,hwnd,msg,wp,lp);
	}
	
	LRESULT lr=::CallWindowProc(pScrollbar->m_funOldProc,hwnd,msg,wp,lp);
	//return lr;

	if(msg==WM_ERASEBKGND)
	{
	}else if(msg==WM_NCCALCSIZE && wp)
	{
		LPNCCALCSIZE_PARAMS pNcCalcSizeParam=(LPNCCALCSIZE_PARAMS)lp;
		DWORD dwStyle=::GetWindowLong(hwnd,GWL_STYLE);
		DWORD dwExStyle=::GetWindowLong(hwnd,GWL_EXSTYLE);
		BOOL  bLeftScroll=dwExStyle&WS_EX_LEFTSCROLLBAR;
		int nWid=::GetSystemMetrics(SM_CXVSCROLL);
		if(dwStyle&WS_VSCROLL) 
		{
			pNcCalcSizeParam->rgrc[0].right+=4;
		}
		
		RECT rc,rcVert,rcHorz;
		::GetWindowRect(hwnd,&rc);
		::OffsetRect(&rc,-rc.left,-rc.top);
		
		//nWid=pScrollbar->m_nScrollWid;
		{
			int nRight=pNcCalcSizeParam->rgrc[0].right;
			int nBottom=pNcCalcSizeParam->rgrc[0].bottom;
			rcVert.left=nRight;
			rcVert.right=nRight+nWid;
			rcVert.top=0;
			rcVert.bottom=nBottom;
			rcHorz.left=0;
			rcHorz.right=nRight;
			rcHorz.top=nBottom;
			rcHorz.bottom=nBottom + nWid;
		}
		if(dwStyle & WS_VSCROLL)
		{
			if(!pScrollbar->IsWindowVisible())
			{
				pScrollbar->PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_SHOWSCROLLBAR, SW_SHOW);
			}
		}else
		{
			if(pScrollbar->IsWindowVisible())
			{
				pScrollbar->PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_SHOWSCROLLBAR, SW_HIDE);
			}
		}

	}else if(WM_SIZE == msg)
	{
		DWORD dwStyle=::GetWindowLong(hwnd,GWL_STYLE);
		DWORD dwExStyle=::GetWindowLong(hwnd,GWL_EXSTYLE);
		BOOL  bLeftScroll=dwExStyle&WS_EX_LEFTSCROLLBAR;
		int nWid=::GetSystemMetrics(SM_CXVSCROLL);
		if(dwStyle&WS_VSCROLL)
		{
			pScrollbar->PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_SHOWSCROLLBAR, SW_SHOW);
		}else
		{
			if(pScrollbar->IsWindowVisible())
			{
				pScrollbar->PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_SHOWSCROLLBAR, SW_HIDE);
			}
		}
	}
	return lr;
}


CLKScrollbar::CLKScrollbar()
: CLKCtrlBaseEx(-1)
, m_pOwner(0)
, m_funOldProc(0)
{
	//SetVScrollbarVisible();
}

CLKScrollbar::~CLKScrollbar()
{
}

BOOL CLKScrollbar::Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID, bool bVisible)
{
	m_pOwner = pParentWnd;
	CRect rtWnd(rect);
	m_pOwner->ClientToScreen(&rtWnd);
	m_pOwner->GetParent()->ScreenToClient(&rtWnd);
	DWORD dwType = WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	if(bVisible)
	{
		dwType |= WS_VISIBLE;
	}
	BOOL bRet = CLKCtrlBase::Create(strCaption, dwType, rtWnd, m_pOwner->GetParent(), nID); 
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW);
	SetWindowLong(m_pOwner->m_hWnd, GWL_USERDATA, (LONG)this);
	m_funOldProc=(WNDPROC)SetWindowLong(m_pOwner->m_hWnd,GWL_WNDPROC,(LONG)HookWndProc);
	return bRet;
}

// 获取默认宽度
int CLKScrollbar::GetDefaultWidth()
{
	int nWidth = GetVScrollbarWidth();
	if(0 == nWidth)
	{
		CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR);
		nWidth = pImg->GetWidth();
	}
	return nWidth;
}

BEGIN_MESSAGE_MAP(CLKScrollbar, CLKCtrlBaseEx)
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
END_MESSAGE_MAP()



// CLKScrollbar 消息处理程序
LRESULT CLKScrollbar::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARUP == wParam || LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARDOWN == wParam)
	{
		GetParent()->PostMessage(LKCOMMON_MSG, wParam, lParam);


		TRACE(_T("Scrollbar width1: %d\r\n"), GetWidth());
		int nMin(0), nMax(0), nPos(0);
		m_pOwner->GetScrollRange(SB_VERT, &nMin, &nMax);
		nPos = GetVScrollbarSliderPos() + nMin;
		if(nPos <= nMin)
		{
			m_pOwner->PostMessage(WM_VSCROLL , MAKEWPARAM(SB_TOP, nPos));
		}else if(nPos < nMax)
		{
			m_pOwner->PostMessage(WM_VSCROLL , MAKEWPARAM(SB_THUMBTRACK, nPos));
		}else
		{
			m_pOwner->PostMessage(WM_VSCROLL , MAKEWPARAM(SB_BOTTOM, nPos));
		}


		//// 滚动
		//LKSCROLLINFO *pInfo = (LKSCROLLINFO *)lParam;
		//// 滚动消息
		//m_nVisibleItemBeginIndex = pInfo->nPos;
  //      m_pVisibleItemBeginItem = GetItemByIndex(m_nVisibleItemBeginIndex);
		////CalcVisibleCount();
		//Invalidate();
	}else if(LKCTRLBASEEX_MSG_WPARAM_SHOWSCROLLBAR == wParam)
	{
		// 垂直滚动条
		if(lParam == SW_SHOW)
		{
			CRect rtWnd;
			m_pOwner->GetWindowRect(&rtWnd);
			rtWnd.MoveToXY(0, 0);
			CRect rtClip(rtWnd);
			rtClip.left = rtClip.right - GetWidth();
			CRgn rgn,rgn2;
			rgn.CreateRectRgnIndirect(&rtWnd);
			rgn2.CreateRectRgnIndirect(&rtClip);
			rgn.CombineRgn(&rgn, &rgn2, RGN_DIFF);
			m_pOwner->ClientToScreen(&rtClip);
			GetParent()->ScreenToClient(&rtClip);
			MoveWindow(&rtClip);
			ShowWindow(SW_SHOW);
			m_pOwner->SetWindowRgn(rgn, true);
		}else if(lParam == SW_HIDE)
		{
			CRect rtWnd;
			m_pOwner->GetWindowRect(&rtWnd);
			rtWnd.MoveToXY(0, 0);
			CRgn rgn;
			rgn.CreateRectRgnIndirect(&rtWnd);
			ShowWindow(SW_HIDE);
			m_pOwner->SetWindowRgn(rgn, true);
		}

		// 水平滚动条
	}
	return CLKCtrlBaseEx::OnCommonMsg(wParam, lParam);
}


// 刷新前景缓存
void CLKScrollbar::ReSetMem()
{
	ParseImageToMem();

	if(IsWindowVisible())
	{
		Invalidate();
	}
}
// 解析出相关图片到内存缓存中
void CLKScrollbar::ParseImageToMem()
{
}

// 初始化图片列表
void CLKScrollbar::InitImageList()
{
	SetScrollbarImg(CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR), 0);
}

// 创建控件后初始化
void CLKScrollbar::InitCtrlAfterCreate()
{
	ParseImageToMem();
	CLKCtrlBaseEx::InitCtrlAfterCreate();
}

// 没有取得背景
void CLKScrollbar::OnDrawNoParentBG(CDC *pDC)
{
	pDC->FillSolidRect(&m_rtClient, RGB(255, 255, 255));
}
