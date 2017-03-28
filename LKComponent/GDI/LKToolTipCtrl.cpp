// ..\..\common\LKComponent\GDI\LKToolTipCtrl.cpp : 实现文件
//

//#include "stdafx.h"
//#include "Trip.h"
#include "LKToolTipCtrl.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// CLKToolTipCtrl

IMPLEMENT_DYNAMIC(CLKToolTipCtrl, CToolTipCtrl)
void CLKToolTipCtrl::RegeditToolTip()
{
    UINT nStyle = ::GetClassLong(GetSafeHwnd(), GCL_STYLE);
    nStyle &= ~CS_DROPSHADOW;
    
    if(::SetClassLong(GetSafeHwnd(), GCL_STYLE, nStyle) == 0)
    {
        ::AfxMessageBox(_T(""));
    }
}

CLKToolTipCtrl::CLKToolTipCtrl()
: m_pImgBG(0)
, m_pImgOldBG(0)
, m_rtWnd(0, 0, 0, 0)
, m_rtOldWnd(0, 0, 0, 0)
{
}

CLKToolTipCtrl::~CLKToolTipCtrl()
{
    if(m_pImgOldBG)
    {
        delete m_pImgOldBG;
    }
    if(m_pImgBG)
    {
        delete m_pImgBG;
    }
}


BEGIN_MESSAGE_MAP(CLKToolTipCtrl, CToolTipCtrl)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_MOVE()
    ON_WM_SHOWWINDOW()
    ON_WM_NCCALCSIZE()
//    ON_WM_SIZING()
END_MESSAGE_MAP()



// CLKToolTipCtrl 消息处理程序



void CLKToolTipCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    if(!m_rtWnd.IsRectEmpty() && m_pImgBG && m_pImgOldBG)
    {
        CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_LABELBG);
        //pImg->DrawToImage(m_pImgBG, 0, 0, LKSKINBASE_DRAWBGSTYLE_ALL);
        m_pImgBG->DrawToDC(&dc, 0, 0, 0);
        CRect rtDest(0, 0, m_pImgBG->GetWidth(), m_pImgBG->GetHeight());
        pImg->DrawToDC(&dc, rtDest, 0, LKSKINBASE_DRAWBGSTYLE_ALL);
		TOOLINFO tInfo = {0};
		tInfo.cbSize = sizeof(TOOLINFO);
    	//CToolInfo ti;
        //ti.cbSize = sizeof(TTTOOLINFOW);
        CString strText;
        if(GetCurrentTool(&tInfo))
        {
            //strText = tInfo.lpszText;
			CWnd *pWnd = GetParent();
			//while(!pWnd->IsKindOf(RUNTIME_CLASS(CDialog)) && pWnd)
			//{
			//	pWnd = pWnd->GetParent();
			//}
            if(tInfo.hwnd)
            {
				pWnd = CWnd::FromHandlePermanent(tInfo.hwnd);
                GetText(strText, pWnd, tInfo.uId);
            }else
            {
                GetText(strText, pWnd);
            }
        }
        if(!strText.IsEmpty())
        {
            dc.SetBkMode(TRANSPARENT);
            CRect rtText(rtDest);
            rtText.right -= 2;
            rtText.bottom -= 2;
            CFont *pFont = dc.SelectObject(CLKFontMgr::GetST90());
            dc.DrawText(strText, rtText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            dc.SelectObject(pFont);
        }
    }
}

BOOL CLKToolTipCtrl::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    //static int n(0);
    //TRACE(_T("CLKToolTipCtrl::OnEraseBkgnd: %d\r\n"), n++);
    return true;
    //return CToolTipCtrl::OnEraseBkgnd(pDC);
}

LRESULT CLKToolTipCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加专用代码和/或调用基类
    if(WM_WINDOWPOSCHANGING == message)
    {
        if(CSystemInfo::GetSystemVer() > WINDOWS_XP)
        {
        }else
        {
            // XP系统
            //将提示框大小改小
            LPWINDOWPOS   lpPos   =   (LPWINDOWPOS)lParam;
            if(!(SWP_NOSIZE & lpPos->flags))
            {
                //static int n(0);
                //TRACE(_T("CLKToolTipCtrl::WM_WINDOWPOSCHANGING: %d\r\n"), n++);
                lpPos->cx += 6; 
                lpPos->cy += 6; 
            }
            if(!(SWP_NOMOVE & lpPos->flags))
            {
                CPoint pt;
                ::GetCursorPos(&pt);
                CRect rtWnd;
                CWnd *pWnd = GetParent();
                pWnd->GetWindowRect(rtWnd);
                if(lpPos->y <= pt.y)
                {
                    //lpPos->y = rtWnd.top - 22/**/;
                    lpPos->y = pt.y - 22 - 5;
                }
            }
        }
    }
    return CToolTipCtrl::WindowProc(message, wParam, lParam);
}

BOOL CLKToolTipCtrl::Create(CWnd* pParentWnd, DWORD dwStyle)
{
    // TODO: 在此添加专用代码和/或调用基类

    BOOL bRet = CToolTipCtrl::Create(pParentWnd, dwStyle);
    RegeditToolTip();
    //int nTime = GetDelayTime(TTDT_AUTOMATIC); 
    //nTime = GetDelayTime(TTDT_RESHOW); 
    //nTime = GetDelayTime(TTDT_AUTOPOP); 
    //nTime = GetDelayTime(TTDT_INITIAL); 
    //nTime = GetDelayTime(TTDT_INITIAL); 
    //SetWindowLong(, );
    //SetDelayTime(800); 
    //SetDelayTime(TTDT_AUTOPOP, 4000); 
    return bRet;
}

void CLKToolTipCtrl::OnSize(UINT nType, int cx, int cy)
{
    CToolTipCtrl::OnSize(nType, cx, cy);
    CRect rtClient;
    GetWindowRect(&m_rtWnd);
    rtClient = m_rtWnd;
    rtClient.MoveToXY(0, 0);
    if(!rtClient.IsRectEmpty())
    {
        //static int n(0);
        //TRACE(_T("OnSize: left-%d, top-%d, right-%d, bottom-%d\r\n"), m_rtWnd.left, m_rtWnd.top, m_rtWnd.right, m_rtWnd.bottom);
        if(m_pImgOldBG)
        {
            delete m_pImgOldBG;
        }
        if(m_pImgBG)
        {
            delete m_pImgBG;
        }
        m_pImgOldBG = new CLKImage(rtClient.Width(), rtClient.Height());
        m_pImgBG = new CLKImage(rtClient.Width(), rtClient.Height());
        DrawDeskBGToImg(m_pImgOldBG, m_rtWnd);
        DrawDeskBGToImg(m_pImgBG, m_rtWnd);
        m_rtOldWnd = m_rtWnd;
    }
}

// 画桌面背景到CLKImage中
void CLKToolTipCtrl::DrawDeskBGToImg(CLKImage *pImg, CRect rt)
{
    //获取桌面窗口句柄
    HWND hWnd = ::GetDesktopWindow();
    //获取桌面窗口DC
    HDC hDC = ::GetWindowDC(hWnd);
    if(hDC)
    {
        pImg->DrawFromDC(CDC::FromHandle(hDC), rt);
        ::ReleaseDC(hWnd, hDC);
    }
}


void CLKToolTipCtrl::OnMove(int x, int y)
{
    CToolTipCtrl::OnMove(x, y);

    GetWindowRect(&m_rtWnd);
    if(!m_rtWnd.IsRectEmpty() && m_pImgBG && m_pImgOldBG && !(m_rtWnd.left == m_rtOldWnd.left && m_rtWnd.top == m_rtOldWnd.top))
    {
        if(GetSafeHwnd())
        {
            DrawDeskBGToImg(m_pImgBG, m_rtWnd);
            // 取新旧区域的交集
            CRect rt(0, 0, 0, 0);
            bool b = !!rt.IntersectRect(&m_rtWnd, &m_rtOldWnd);
            if(!rt.IsRectEmpty() && b)
            {
                //TRACE(_T("OnMove: left-%d, top-%d, right-%d, bottom-%d\r\n"), rt.left, rt.top, rt.right, rt.bottom);
                CRect rtDest(rt), rtSrc(rt);
                rtDest.OffsetRect(-m_rtWnd.left, -m_rtWnd.top);
                rtSrc.OffsetRect(-m_rtOldWnd.left, -m_rtOldWnd.top);
                m_pImgOldBG->DrawToImage(m_pImgBG, rtDest, rtSrc);
            }
        }
        m_pImgBG->DrawToImage(m_pImgOldBG);
    }
    m_rtOldWnd = m_rtWnd;
}

void CLKToolTipCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CToolTipCtrl::OnShowWindow(bShow, nStatus);
    if(!bShow)
    {
        m_rtOldWnd = CRect(0, 0, 0, 0);
    }
    // TODO: 在此处添加消息处理程序代码
}

void CLKToolTipCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    lpncsp->rgrc[0].top    += 0;		// 默认标题栏高度为0
    lpncsp->rgrc[0].left   += 0;	// 默认左边框宽度为0
    lpncsp->rgrc[0].right  -= 0;	// 默认右边框宽度为0 
    lpncsp->rgrc[0].bottom -= 0;	// 默认底边框高度为0
    //CToolTipCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}

//void CLKToolTipCtrl::OnSizing(UINT fwSide, LPRECT pRect)
//{
//    pRect->bottom = pRect->top + 28;
//    CToolTipCtrl::OnSizing(fwSide, pRect);
//
//    // TODO: 在此处添加消息处理程序代码
//}
