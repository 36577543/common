// ..\..\..\common\LKComponent\gdi\LKDateTimeCtrl.cpp : 实现文件
//

#include "LKDateTimeCtrl.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// 日历控件ID
#define MONTHCALCTRL_ID     10000

// CLKDateTimeCtrl

IMPLEMENT_DYNAMIC(CLKDateTimeCtrl, CLKCtrlBase)


LRESULT CALLBACK CLKDateTimeCtrl::EditWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	CLKDateTimeCtrl *pThis = (CLKDateTimeCtrl *)GetWindowLong(hWnd, GWL_USERDATA);
	if(msg == WM_MOUSELEAVE)
	{
        pThis->m_bEnterEdit = false;
        pThis->OnMouseLeave();
	}else if(msg == WM_MOUSEMOVE)
	{
		if(!pThis->m_bEnterEdit)
		{
            pThis->m_bEnterEdit = true;
			TRACKMOUSEEVENT etMouse;
			etMouse.cbSize = sizeof(TRACKMOUSEEVENT);   
			etMouse.dwFlags = TME_LEAVE | TME_HOVER;   
			etMouse.dwHoverTime = NULL;   
			etMouse.hwndTrack = hWnd;  
			_TrackMouseEvent(&etMouse);   
		}
	//}else if(WM_SETFOCUS == msg)
	//{
	//	if(!pThis->m_strHint.IsEmpty())
	//	{
	//		CString strText;
 //           pThis->m_bGetPass = true;
	//		pThis->m_ctlEdit.GetWindowText(strText);
 //           pThis->m_bGetPass = false;
	//		if(strText.IsEmpty())
	//		{
	//			pThis->m_ctlEdit.SetWindowText(strText);
	//		}
	//	}
	//}else if(WM_KILLFOCUS == msg)
	//{
	//	if(!pThis->m_strHint.IsEmpty())
	//	{
	//		CString strText;
 //           pThis->m_bGetPass = true;
	//		pThis->m_ctlEdit.GetWindowText(strText);
 //           pThis->m_bGetPass = false;
	//		if(strText.IsEmpty())
	//		{
	//			pThis->m_ctlEdit.SetWindowText(pThis->m_strHint);
	//		}
	//	}
	}else if(EM_SHOWBALLOONTIP == msg)
	{
		::PostMessage(pThis->GetSafeHwnd(), LKCOMMON_MSG, LKEDIT_MSG_WPARAM_CAPSLK, (LPARAM)pThis);
		return 1;
	}
	return ::CallWindowProc(pThis->m_pOldEditProc, hWnd, msg, wparam, lparam);
}


CLKDateTimeCtrl::CLKDateTimeCtrl()
: CLKCtrlBase(0)
// 状态（0 - 常态； 1 - 激活状态）
, m_nState(0)
// 鼠标处于什么之上(0 - 未知； 1 - 清空； 2 - 弹出日历)
, m_nMouseWhere(0)
// 鼠标按下是的位置(0 - 未知； 1 - 清空； 2 - 弹出日历)
, m_nMouseDown(0)
// 清空按钮位置
, m_rtClear(0, 0, 0, 0)
// 弹出日历按钮位置
, m_rtPop(0, 0, 0, 0)
// 是否进入编辑框
, m_bEnterEdit(false)
// 日历控件
, m_pDate(0)
{
    m_pImgArr[0] = 0;
    m_pImgArr[1] = 0;
}

CLKDateTimeCtrl::~CLKDateTimeCtrl()
{
	if(m_pImgArr[0]) delete m_pImgArr[0];
	if(m_pImgArr[1]) delete m_pImgArr[1];
}

BOOL CLKDateTimeCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_TABSTOP, rect, pParentWnd, nID); 
    //CalcItemSize();
	return bRet;
}


BEGIN_MESSAGE_MAP(CLKDateTimeCtrl, CLKCtrlBase)
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CLKDateTimeCtrl 消息处理程序


void CLKDateTimeCtrl::ParseImageToMem()
{
    CLKImage *pBGImg = GetParseImg();
	if(pBGImg)
	{
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
        GetParentImage(&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);

		if(m_pImgArr[0]) delete m_pImgArr[0];
		m_pImgArr[0] = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
	    pBGImg->DrawToImage(m_pImgArr[0], 0, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
		if(m_pImgArr[1]) delete m_pImgArr[1];
		m_pImgArr[1] = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
	    pBGImg->DrawToImage(m_pImgArr[1], 1, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
		SetMemImg(m_pImgArr[0]);
	}

    // 计算出按钮位置
    CLKImage *pImg = m_arrImage[1];
    CRect rtClear(m_rtClient);
    rtClear.DeflateRect(6, 4, 4, 4);
    rtClear.left = rtClear.right;
    if(pImg)
    {
        rtClear.left -= pImg->GetWidth();
        m_rtClear = rtClear;
    }
    pImg = m_arrImage[2];
    CRect rtDate(rtClear);
    rtDate.right = rtDate.left;
    if(pImg)
    {
        rtDate.left -= pImg->GetWidth();
        m_rtPop = rtDate;
    }
}

// 刷新前景缓存
void CLKDateTimeCtrl::ReSetMem()
{
	ParseImageToMem();
	if(IsWindowVisible())
	{
		Invalidate();
	}
}

// 初始化图片列表
void CLKDateTimeCtrl::InitImageList()
{
    // 初始化图片列表(0 - 按钮背景， 1 - 控件背景， 2 - 箭头) 
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_DTAETIME_BEGIN, LKIMAGELISTINDEX_DTAETIME_END);
    m_arrImage[0] = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_EDIT_BEGIN + 2);
}

// 创建控件后初始化
void CLKDateTimeCtrl::InitCtrlAfterCreate()
{
	ParseImageToMem();

    CRect rtEdit(m_rtClient);
    int nRight = 6;
    if(m_arrImage[1])
    {
        nRight += m_arrImage[1]->GetWidth();
    }
    if(m_arrImage[2])
    {
        nRight += m_arrImage[2]->GetWidth();
    }
    rtEdit.DeflateRect(6, 7, nRight, 5);
	m_ctlEdit.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL, rtEdit, this, LKEDIT_CHILDID_EDIT);
	SetWindowLong(m_ctlEdit.GetSafeHwnd(), GWL_USERDATA,(LONG)this);
	m_pOldEditProc = (WNDPROC)SetWindowLong(m_ctlEdit.GetSafeHwnd(), GWL_WNDPROC,(LONG)EditWndProc);
	CFont *pFtText = CLKFontMgr::GetST90();
	m_ctlEdit.SetFont(pFtText, true);
}

// 在缓存上绘图
void CLKDateTimeCtrl::OnPaint(CDC *pDC)
{
    CLKImage *pImg = m_arrImage[1];
    if(pImg)
    {
        int nIndex = (1 == m_nMouseDown ? 2 : (1 == m_nMouseWhere ? 1 : 0));
        pImg->DrawToDC(pDC, m_rtClear, nIndex);
    }
    pImg = m_arrImage[2];
    if(pImg)
    {
        int nIndex = (2 == m_nMouseDown ? 2 : (2 == m_nMouseWhere ? 1 : 0));
        pImg->DrawToDC(pDC, m_rtPop, nIndex);
    }
}

// 将前景画在指定的CDC设置上
void CLKDateTimeCtrl::OnDrawFGToDC(BGParam *prm)
{
}


void CLKDateTimeCtrl::OnMouseLeave()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    m_nMouseWhere = 0;
    m_nMouseDown = 0;

    CPoint pt;
    GetCursorPos(&pt);
	ScreenToClient(&pt);
    static int n(0);
    if(!m_rtClient.PtInRect(pt))
    {
        m_nState = 0;
		SetMemImg(m_pImgArr[0]);
        Invalidate();
        //TRACE(_T("MOUSE LEAVE: %d\r\n"), n++);
    }
    CLKCtrlBase::OnMouseLeave();
}

void CLKDateTimeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if(m_rtClear.PtInRect(point))
    {
        // 是否在清空按钮上
        if(m_nMouseDown == 2) 
        {
            m_nMouseDown = 0;
            InvalidateRect(&m_rtPop);
        }
        if(m_nMouseWhere != 1)
        {
            if(m_nMouseWhere == 2) 
            {
                InvalidateRect(&m_rtPop);
            }
            m_nMouseWhere = 1;
            InvalidateRect(&m_rtClear);
        }

    }else if(m_rtPop.PtInRect(point))
    {
        // 是否在弹出按钮上
        if(m_nMouseDown == 1) 
        {
            m_nMouseDown = 0;
            InvalidateRect(&m_rtClear);
        }
        if(m_nMouseWhere != 2)
        {
            if(m_nMouseWhere == 1) 
            {
                InvalidateRect(&m_rtClear);
            }
            m_nMouseWhere = 2;
            InvalidateRect(&m_rtPop);
        }

    }else if(m_nMouseWhere != 0)
    {
        m_nMouseWhere = 0;
        m_nMouseDown = 0;
        Invalidate();
    }

    if(1 != m_nState || m_bEnterEdit)
    {
        m_nState = 1;
		SetMemImg(m_pImgArr[1]);
        Invalidate();
		TRACKMOUSEEVENT   tme;   
		tme.cbSize		= sizeof(tme);   
		tme.dwFlags		= TME_LEAVE; 
		tme.hwndTrack	= GetSafeHwnd();   
		tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
		::TrackMouseEvent(&tme);
    }
    CLKCtrlBase::OnMouseMove(nFlags, point);
}

void CLKDateTimeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(m_rtClear.PtInRect(point))
    {
        // 是否在清空按钮上
        if(m_nMouseDown != 1)
        {
            m_nMouseDown = 1;
            InvalidateRect(&m_rtClear);
        }

    }else if(m_rtPop.PtInRect(point))
    {
        // 是否在弹出按钮上
        if(m_nMouseDown != 2)
        {
            m_nMouseDown = 2;
            InvalidateRect(&m_rtPop);
        }

    }else if(m_nMouseDown != 0)
    {
        m_nMouseDown = 0;
        Invalidate();
    }
    CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKDateTimeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(m_rtClear.PtInRect(point))
    {
        // 是否在清空按钮上
        if(m_nMouseDown == 1)
        {
            m_nMouseDown = 0;
            InvalidateRect(&m_rtClear);
        }

    }else if(m_rtPop.PtInRect(point))
    {
        // 是否在弹出按钮上
        if(m_nMouseDown == 2)
        {
            m_nMouseDown = 0;
            InvalidateRect(&m_rtPop);
            
            // 弹出日历控件
            PopMonthCalCtrl();
        }

    }else if(m_nMouseDown != 0)
    {
        m_nMouseDown = 0;
        Invalidate();
    }
    CLKCtrlBase::OnLButtonUp(nFlags, point);
}

// 弹出日历控件
void CLKDateTimeCtrl::PopMonthCalCtrl()
{
    if(!m_pDate)
    {
        m_pDate = new CLKMonthCalCtrl;
        CRect rt;
        GetRectInParent(&rt);
        rt.top = rt.bottom;
        m_pDate->Create(rt, GetParent(), MONTHCALCTRL_ID);
        //m_pDate->SetOwner(this);
    }
}

void CLKDateTimeCtrl::OnSetFocus(CWnd* pOldWnd)
{
    CLKCtrlBase::OnSetFocus(pOldWnd);
	m_ctlEdit.SetFocus();
}
