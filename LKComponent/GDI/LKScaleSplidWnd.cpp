// ..\..\..\common\LKComponent\gdi\LKScaleSplidWnd.cpp : 实现文件
//

#include "LKScaleSplidWnd.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"


// CLKScaleSplidWnd

IMPLEMENT_DYNAMIC(CLKScaleSplidWnd, CLKCtrlBase)

CLKScaleSplidWnd::CLKScaleSplidWnd(int nImageIndex, bool bLeft)
: CLKCtrlBase(nImageIndex)
, m_pLeftWnd(0)
, m_pRightWnd(0)
, m_bLeft(bLeft)
, m_emsCommon(esb_nohere)
{

}

CLKScaleSplidWnd::~CLKScaleSplidWnd()
{
}

BOOL CLKScaleSplidWnd::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
    //CalcItemSize();
	return bRet;
}

// 是否展开
bool CLKScaleSplidWnd::GetIsOpen()
{
    CWnd *pHideWnd = 0;
    if(m_pLeftWnd && !m_pLeftWnd->IsWindowVisible())
    {
        pHideWnd = m_pLeftWnd;
    }else if(m_pRightWnd && !m_pRightWnd->IsWindowVisible())
    {
        pHideWnd = m_pRightWnd;
    }

    return !pHideWnd;
}

BEGIN_MESSAGE_MAP(CLKScaleSplidWnd, CLKCtrlBase)
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CLKScaleSplidWnd 消息处理程序
void CLKScaleSplidWnd::OnMouseLeave()
{
	if(m_emsCommon != esb_nohere)
	{
		m_emsCommon = esb_nohere;
		Invalidate();
	}
	CLKCtrlBase::OnMouseLeave();
}

void CLKScaleSplidWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_emsCommon == esb_nohere)
	{
		m_emsCommon = esb_hover;
		TRACKMOUSEEVENT   tme;   
		tme.cbSize		= sizeof(tme);   
		tme.dwFlags		= TME_LEAVE; 
		tme.hwndTrack	= this->GetSafeHwnd();   
		tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
		::TrackMouseEvent(&tme);

		Invalidate();
	}
	CLKCtrlBase::OnMouseMove(nFlags, point);
}


void CLKScaleSplidWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_emsCommon != esb_down)
	{
		m_emsCommon = esb_down;
		Invalidate();
	}
	CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKScaleSplidWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_emsCommon == esb_down)
	{
        m_emsCommon = esb_nohere;
		OpenOrClose();
		::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	    Invalidate();
	}
	CLKCtrlBase::OnLButtonUp(nFlags, point);
}

// 创建控件后初始化
void CLKScaleSplidWnd::InitCtrlAfterCreate()
{
	ParseImageToMem();
}

// 刷新前景缓存
void CLKScaleSplidWnd::ReSetMem()
{
    //CalcItemSize();
	ParseImageToMem();
	if(IsWindowVisible())
	{
		Invalidate();
	}
}

// 解析出相关图片到内存缓存中
void CLKScaleSplidWnd::ParseImageToMem()
{
	//if(m_pBGImage)
	//{
	//	BGParam prm;
	//	prm.rt = CRect(0, 0, 0, 0);
	//	GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
	//	CRect rt;
	//	GetRectInParent(&rt);
	//	prm.rt.OffsetRect(rt.left, rt.top);

	//	CLKImage *pImg = GetMemImg();
	//	if(pImg) delete pImg;
	//	pImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
 //       m_pBGImage->DrawToImage(pImg, 0, &prm);
	//	SetMemImg(pImg);
	//}
}

// 初始化图片列表
void CLKScaleSplidWnd::InitImageList()
{
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN, LKIMAGELISTINDEX_SCALESPLIDWND_END);
	CLKImage *pImage = GetParseImg();
	if(pImage)
	{
		m_nItemWidth = pImage->GetWidth();
		m_nItemHeight = pImage->GetHeight() / pImage->GetImageNumber();
	}
}

// 在缓存上绘图
void CLKScaleSplidWnd::OnPaint(CDC *pDC)
{
    // 
    int nIndex = GetImageIndex();
    CLKImage *pImg;
    if(m_bLeft)
    {
        // 在左边
        pImg = m_arrImage[nIndex + 1];
    }else
    {
        // 在右边
        pImg = m_arrImage[nIndex];
    }
    if(pImg)
    {
        // 鼠标状态
        int nIndex = (esb_hover == m_emsCommon ? 1 : (esb_down == m_emsCommon ? 2 : 0));
        CRect rtDest(m_rtClient);
        rtDest.top += (m_rtClient.Height() - m_nItemHeight) / 2;
        rtDest.bottom = rtDest.top + m_nItemHeight;
        rtDest.left += (m_rtClient.Width() - m_nItemWidth) / 2;
        rtDest.right = rtDest.left + m_nItemWidth;
        pImg->DrawToDC(pDC, rtDest, nIndex);
    }
}

// 将前景画在指定的CDC设置上
void CLKScaleSplidWnd::OnDrawFGToDC(BGParam *prm)
{
}

// 打开或关闭窗口
void CLKScaleSplidWnd::OpenOrClose()
{
    m_bLeft = !m_bLeft;
    CWnd *pHideWnd = 0;
    if(m_pLeftWnd && !m_pLeftWnd->IsWindowVisible())
    {
        pHideWnd = m_pLeftWnd;
    }else if(m_pRightWnd && !m_pRightWnd->IsWindowVisible())
    {
        pHideWnd = m_pRightWnd;
    }

    CRect rtWnd(0, 0, 0, 0);
    if(pHideWnd)
    {
        // 展开
        //OpenWnd();
        if(m_bLeft)
        {
            // 向左展开
            if(m_pRightWnd)
            {
                // 向左
                m_pRightWnd->AnimateWindow(100, AW_HOR_NEGATIVE | AW_SLIDE);
                //m_pRightWnd->GetWindowRect(&rtWnd);
                //GetParent()->ScreenToClient(&rtWnd);
                //SetWindowPos(0, rtWnd.left - m_rtClient.Width(), rtWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                //if(m_pLeftWnd)
                //{
                //    int nWidth(rtWnd.Width());
                //    m_pLeftWnd->GetWindowRect(&rtWnd);
                //    m_pLeftWnd->SetWindowPos(0, 0, 0, rtWnd.Width() - nWidth, rtWnd.Height(), SWP_NOMOVE | SWP_NOZORDER);
                //} 
                ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKSCALESPLIDWND_MSG_WPARAM_OPEN, (LPARAM)this);
            }
        }else
        {
            // 向右展开
            if(m_pLeftWnd)
            {
                m_pLeftWnd->AnimateWindow(100, AW_HOR_POSITIVE | AW_SLIDE);
                //m_pLeftWnd->ShowWindow(SW_SHOW);
                m_pLeftWnd->GetWindowRect(&rtWnd);
                GetParent()->ScreenToClient(&rtWnd);
                SetWindowPos(0, rtWnd.right, rtWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                if(m_pRightWnd)
                {
                    int nWidth(rtWnd.Width());
                    m_pRightWnd->GetWindowRect(&rtWnd);
                    GetParent()->ScreenToClient(&rtWnd);
                    m_pRightWnd->SetWindowPos(0, rtWnd.left + nWidth, rtWnd.top, rtWnd.Width() - nWidth, rtWnd.Height(), SWP_NOZORDER | SWP_NOREDRAW/**/);
                }            
                ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKSCALESPLIDWND_MSG_WPARAM_OPEN, (LPARAM)this);
            }
        }
    }else
    {
        // 关闭
        //CloseWnd();
        if(m_bLeft)
        {
            // 向左关闭
            if(m_pLeftWnd)
            {
                ShowWindow(SW_HIDE);
                m_pLeftWnd->AnimateWindow(100, AW_HOR_NEGATIVE | AW_SLIDE | AW_HIDE);
                //m_pLeftWnd->ShowWindow(SW_HIDE);
                m_pLeftWnd->GetWindowRect(&rtWnd);
                GetParent()->ScreenToClient(&rtWnd);
                SetWindowPos(0, rtWnd.left, rtWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
                if(m_pRightWnd)
                {
                    int nWidth(rtWnd.Width());
                    m_pRightWnd->GetWindowRect(&rtWnd);
                    GetParent()->ScreenToClient(&rtWnd);
                    m_pRightWnd->SetWindowPos(0, rtWnd.left - nWidth, rtWnd.top, rtWnd.Width() + nWidth, rtWnd.Height(), SWP_NOZORDER | SWP_NOREDRAW);
                }            
                ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKSCALESPLIDWND_MSG_WPARAM_CLOSE, (LPARAM)this);
            }
        }else
        {
            // 向右关闭
            if(m_pRightWnd)
            {
                ShowWindow(SW_HIDE);
                m_pRightWnd->AnimateWindow(100, AW_HOR_POSITIVE | AW_SLIDE | AW_HIDE);
                //m_pRightWnd->ShowWindow(SW_HIDE);
                m_pRightWnd->GetWindowRect(&rtWnd);
                GetParent()->ScreenToClient(&rtWnd);
                SetWindowPos(0, rtWnd.right - m_rtClient.Width(), rtWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
                if(m_pLeftWnd)
                {
                    int nWidth(rtWnd.Width());
                    m_pLeftWnd->GetWindowRect(&rtWnd);
                    m_pLeftWnd->SetWindowPos(0, 0, 0, rtWnd.Width() + nWidth, rtWnd.Height(), SWP_NOMOVE | SWP_NOZORDER);
                }            
                ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKSCALESPLIDWND_MSG_WPARAM_CLOSE, (LPARAM)this);
            }
        }
    }
}
