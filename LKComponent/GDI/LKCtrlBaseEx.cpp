// ..\..\common\LKComponent\GDI\LKCtrlBaseEx.cpp : 实现文件
//

#include "LKCtrlBaseEx.h"


// CLKCtrlBaseEx

IMPLEMENT_DYNAMIC(CLKCtrlBaseEx, CLKCtrlBase)

CLKCtrlBaseEx::CLKCtrlBaseEx(int nImageIndex)
: CLKCtrlBase(nImageIndex)
// 滚动条图片
, m_pVScrollbarImg(0)
, m_pHScrollbarImg(0)
// 垂直滚动条当前图片内索引
, m_nCurVScrollbarImgIndex(0)
// 水平滚动条当前图片内索引
, m_nCurHScrollbarImgIndex(0)
// 垂直滚动条是否可见
, m_bVScrollbarVisible(false)
// 水平滚动条是否可见
, m_bHScrollbarVisible(false)
// 垂直滚动条放置区域
, m_rtVScrollbar(0, 0, 0, 0)
// 水平滚动条放置区域
, m_rtHScrollbar(0, 0, 0, 0)
// 鼠标在滚动条上按下时的位置
, m_ptOld(0, 0)
, m_bCapture(false)
, m_bWndCapture(false)
// 正在拖动滚动条
, m_bScrollbarDraging(false)
{

	memset(&m_siVScrollbar, 0, sizeof(m_siVScrollbar));
	memset(&m_siHScrollbar, 0, sizeof(m_siHScrollbar));
}

CLKCtrlBaseEx::~CLKCtrlBaseEx()
{
}
// 设置垂直滚动条滑块范围
void CLKCtrlBaseEx::SetVScrollbarRang(int nMax, int nMax2, bool bReDraw)
{
	if(m_siVScrollbar.nMax != nMax || m_siVScrollbar.nMax2 != nMax2)
	{
		m_siVScrollbar.nMax = nMax;
		m_siVScrollbar.nMax2 = nMax2;
		CalcVSliderSize();
		if(bReDraw)
		{
			//InvalidateRect(&m_rtVScrollbar, false);
            InvalidateVSlider(true);
		}
	}
}
void CLKCtrlBaseEx::SetVScrollbarRang(int nMax)
{
	if(m_siVScrollbar.nMax != nMax)
	{
		m_siVScrollbar.nMax = nMax;
		m_siVScrollbar.nMax2 = nMax;
		CalcVSliderSize();
	}
}

// 创建控件后初始化
void CLKCtrlBaseEx::InitCtrlAfterCreate()
{
	CLKCtrlBase::InitCtrlAfterCreate();
	CalcScrollbarRect();
}

// 鼠标指针是否在滚动条上
bool CLKCtrlBaseEx::PtInScrollbar(CPoint &point)
{
	if(m_bVScrollbarVisible && m_pVScrollbarImg)
	{
		if(m_rtVScrollbar.PtInRect(point))
			return true;
	}
	if(m_bHScrollbarVisible && m_pHScrollbarImg)
	{
		if(m_rtHScrollbar.PtInRect(point))
			return true;
	}
	return false;
}

BEGIN_MESSAGE_MAP(CLKCtrlBaseEx, CLKCtrlBase)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
//	ON_WM_MOUSEHWHEEL()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CLKCtrlBaseEx 消息处理程序

void CLKCtrlBaseEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(IsVScrollbarValid())
	{
		// 垂直滚动条
		if(m_bCapture && (MK_LBUTTON == (MK_LBUTTON & nFlags)) && m_bScrollbarDraging)
		{
			// 拖动
			//InvalidateVSlider();
			int nOldPos = m_siVScrollbar.nPos;
			m_siVScrollbar.nPos2 += point.y - m_ptOld.y;
			m_siVScrollbar.nPos2 = m_siVScrollbar.nPos2 > 0 ? m_siVScrollbar.nPos2 : 0;
			m_siVScrollbar.nPos2 = m_siVScrollbar.nPos2 < m_rtVScrollbar.Height() - m_siVScrollbar.nSliderSize ? m_siVScrollbar.nPos2 : m_rtVScrollbar.Height() - m_siVScrollbar.nSliderSize;
			m_nCurVScrollbarImgIndex = 2;
			CalcVSliderPos();
			if(m_siVScrollbar.nPos > GetVScrollbarMaxPos())
			{
				m_siVScrollbar.nPos = GetVScrollbarMaxPos();
			}
			if(m_siVScrollbar.nPos != nOldPos)
			{
				// 滚动消息
				SendMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR, (LPARAM)&m_siVScrollbar);
				if(m_siVScrollbar.nPos > nOldPos)
				{
					// 向下滚动
					PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARDOWN, (LPARAM)&m_siVScrollbar);
				}else
				{
					// 向上滚动
					PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARUP, (LPARAM)&m_siVScrollbar);
				}
			}
			InvalidateVSlider(true);
			m_ptOld = point;
		}else if(!m_bWndCapture)
		{
			CRect rtInvalid(m_rtVScrollbar);
			rtInvalid.top = m_siVScrollbar.nPos2;
			rtInvalid.bottom = m_siVScrollbar.nPos2 + m_siVScrollbar.nSliderSize;
			if(m_nCurVScrollbarImgIndex != 1 && rtInvalid.PtInRect(point))
			{
				m_nCurVScrollbarImgIndex = 1;
				TRACKMOUSEEVENT   tme;   
				tme.cbSize		= sizeof(tme);   
				tme.dwFlags		= TME_LEAVE; 
				tme.hwndTrack	= this->GetSafeHwnd();   
				tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
				::TrackMouseEvent(&tme);
				InvalidateVSlider(true);
			}else if(!rtInvalid.PtInRect(point))
			{
				if(0 != m_nCurVScrollbarImgIndex)
				{
					m_nCurVScrollbarImgIndex = 0;
					InvalidateVSlider(true);
				}
			}
		}else
		{
			CRect rtInvalid(m_rtVScrollbar);
			rtInvalid.top += m_siVScrollbar.nPos2;
			rtInvalid.bottom = rtInvalid.top + m_siVScrollbar.nSliderSize;
			if(m_nCurVScrollbarImgIndex != 1 && rtInvalid.PtInRect(point))
			{
				m_nCurVScrollbarImgIndex = 1;
				InvalidateVSlider(true);
			}else if(!rtInvalid.PtInRect(point))
			{
				m_nCurVScrollbarImgIndex = 0;
				InvalidateVSlider(true);
			}
		}
	}else if(m_bHScrollbarVisible && m_pHScrollbarImg)
	{
		// 水平滚动条
		if(m_bCapture)
		{
			// 拖动
			InvalidateHSlider();
			m_siHScrollbar.nPos2 += point.x - m_ptOld.x;
			m_siHScrollbar.nPos2 = m_siHScrollbar.nPos2 > m_rtHScrollbar.left ? m_siHScrollbar.nPos2 : m_rtHScrollbar.left;
			m_siHScrollbar.nPos2 = m_siHScrollbar.nPos2 < m_rtHScrollbar.right - m_siHScrollbar.nSliderSize ? m_siHScrollbar.nPos2 : m_rtHScrollbar.right - m_siHScrollbar.nSliderSize;
			m_nCurHScrollbarImgIndex = 2;
			CalcHSliderPos();
			// 滚动消息
			SendMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_HSCROLLBAR, (LPARAM)&m_siHScrollbar);
			InvalidateHSlider();
			m_ptOld = point;
		}else
		{
			CRect rtInvalid(m_rtHScrollbar);
			rtInvalid.left = m_siHScrollbar.nPos2;
			rtInvalid.right = m_siHScrollbar.nPos2 + m_siHScrollbar.nSliderSize;
			if(m_nCurHScrollbarImgIndex != 1 && rtInvalid.PtInRect(point))
			{
				m_nCurHScrollbarImgIndex = 1;
				TRACKMOUSEEVENT   tme;   
				tme.cbSize		= sizeof(tme);   
				tme.dwFlags		= TME_LEAVE; 
				tme.hwndTrack	= this->GetSafeHwnd();   
				tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
				::TrackMouseEvent(&tme);
				InvalidateHSlider();
			}else if(!rtInvalid.PtInRect(point))
			{
				m_nCurHScrollbarImgIndex = 0;
				InvalidateHSlider();
			}
		}
	}
	CLKCtrlBase::OnMouseMove(nFlags, point);
}

void CLKCtrlBaseEx::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_bCapture)
	{
		if(m_bVScrollbarVisible && m_pVScrollbarImg)
		{
			if(m_nCurVScrollbarImgIndex != 0)
			{
				m_nCurVScrollbarImgIndex = 0;
                InvalidateVSlider(true);
				//InvalidateRect(m_rtVScrollbar, false);
			}
		}
		if(m_bHScrollbarVisible && m_pHScrollbarImg)
		{
			if(m_nCurHScrollbarImgIndex != 0)
			{
				m_nCurHScrollbarImgIndex = 0;
				InvalidateRect(m_rtHScrollbar, false);
			}
		}
	}
	CLKCtrlBase::OnMouseLeave();
}

void CLKCtrlBaseEx::OnCaptureChanged(CWnd *pWnd)
{
	// TODO: 在此处添加消息处理程序代码
	if(pWnd != this)
	{
		// 正在拖动滚动条
		m_bScrollbarDraging = false;
		m_ptOld = CPoint(0, 0);
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		if(m_bVScrollbarVisible && m_pVScrollbarImg)
		{
			// 垂直滚动条
			if(m_rtVScrollbar.PtInRect(point))
			{
				m_nCurVScrollbarImgIndex = 1;
			}else
			{
				m_nCurVScrollbarImgIndex = 0;
			}
			InvalidateVSlider(true);
		}else if(m_bHScrollbarVisible && m_pHScrollbarImg)
		{
			// 水平滚动条
			if(m_rtHScrollbar.PtInRect(point))
			{
				m_nCurHScrollbarImgIndex = 1;
			}else
			{
				m_nCurHScrollbarImgIndex = 0;
			}
			InvalidateHSlider();
		}
	}

	CLKCtrlBase::OnCaptureChanged(pWnd);
}

void CLKCtrlBaseEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bCapture)
	{
		if((m_bVScrollbarVisible && m_pVScrollbarImg) || (m_bHScrollbarVisible && m_pHScrollbarImg))
		{
			if(m_rtVScrollbar.PtInRect(point) || m_rtHScrollbar.PtInRect(point))
			{
				if(m_rtVScrollbar.PtInRect(point))
				{
                    if(point.y < m_siVScrollbar.nPos2 + m_rtVScrollbar.top)
                    {
                        // 滑块向上滚动
			            if(m_siVScrollbar.nPos > 0)
			            {
                            m_siVScrollbar.nPos--;
                            CalcVSliderPos2();
				            // 滚动消息
				            SendMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR, (LPARAM)&m_siVScrollbar);
							// 向下滚动
							PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARUP, (LPARAM)&m_siVScrollbar);
			            }
                    }else if(point.y > m_siVScrollbar.nPos2 + m_rtVScrollbar.top + m_siVScrollbar.nSliderSize)
                    {
                         // 滑块向下滚动
                        if(m_siVScrollbar.nPos < m_siVScrollbar.nMax - m_siVScrollbar.nPosPerPage)
			            {
                            m_siVScrollbar.nPos++;
                            CalcVSliderPos2();
				            // 滚动消息
				            SendMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR, (LPARAM)&m_siVScrollbar);
							PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARDOWN, (LPARAM)&m_siVScrollbar);
			            }
                    }else
                    {
				        // 正在拖动滚动条
				        m_bScrollbarDraging = true;
				        SetCapture();
				        m_ptOld = point;
				        m_bCapture = true;
					    m_nCurVScrollbarImgIndex = 2;
                    }
				    InvalidateVSlider(true);
                }
				if(m_rtHScrollbar.PtInRect(point))
				{
				    // 正在拖动滚动条
				    m_bScrollbarDraging = true;
				    SetCapture();
				    m_ptOld = point;
				    m_bCapture = true;
					m_nCurHScrollbarImgIndex = 2;
					InvalidateHSlider();
				}
			}
		}
	}else
	{
		// 窗口已经捕获鼠标
		if((m_bVScrollbarVisible && m_pVScrollbarImg) || (m_bHScrollbarVisible && m_pHScrollbarImg))
		{
			if(m_rtVScrollbar.PtInRect(point) || m_rtHScrollbar.PtInRect(point))
			{
				if(m_rtVScrollbar.PtInRect(point))
				{
                    if(point.y < m_siVScrollbar.nPos2 + m_rtVScrollbar.top)
                    {
                        // 滑块向上滚动
			            if(m_siVScrollbar.nPos > 0)
			            {
                            m_siVScrollbar.nPos--;
                            CalcVSliderPos2();
				            // 滚动消息
				            SendMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR, (LPARAM)&m_siVScrollbar);
							// 向下滚动
							PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARUP, (LPARAM)&m_siVScrollbar);
			            }
                    }else if(point.y > m_siVScrollbar.nPos2 + m_rtVScrollbar.top + m_siVScrollbar.nSliderSize)
                    {
                         // 滑块向下滚动
                        if(m_siVScrollbar.nPos < m_siVScrollbar.nMax - m_siVScrollbar.nPosPerPage)
			            {
                            m_siVScrollbar.nPos++;
                            CalcVSliderPos2();
				            // 滚动消息
				            SendMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR, (LPARAM)&m_siVScrollbar);
							// 向下滚动
							PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARDOWN, (LPARAM)&m_siVScrollbar);
			            }
                    }else
                    {
				        // 正在拖动滚动条
				        m_bScrollbarDraging = true;
				        m_ptOld = point;
					    m_nCurVScrollbarImgIndex = 2;
					    InvalidateVSlider(true);
                    }
				}
				if(m_rtHScrollbar.PtInRect(point))
				{
			        // 正在拖动滚动条
			        m_bScrollbarDraging = true;
			        m_ptOld = point;
					m_nCurHScrollbarImgIndex = 2;
					InvalidateHSlider();
				}
			}
		}
	}

	CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKCtrlBaseEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bScrollbarDraging = false;
	if(m_bCapture && !m_bWndCapture)
	{
		m_bCapture = false;
		ReleaseCapture();
	}
	CLKCtrlBase::OnLButtonUp(nFlags, point);
}

//BOOL CLKCtrlBaseEx::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// 此功能要求 Windows Vista 或更高版本。
//	// _WIN32_WINNT 符号必须 >= 0x0600。
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	
//	return CLKCtrlBase::OnMouseHWheel(nFlags, zDelta, pt);
//}

BOOL CLKCtrlBaseEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 滚动一行
	if(IsVScrollbarValid())
	{
		if(zDelta > 0)
		{
			// 滚动条向上滚动
			InvalidateVSlider(true);
			m_siVScrollbar.nPos--;
			if(m_siVScrollbar.nPos < 0)
			{
				m_siVScrollbar.nPos = 0;
			}else
			{
				CalcVSliderPos2();
				// 滚动消息
				SendMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR, (LPARAM)&m_siVScrollbar);
				PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARUP, (LPARAM)&m_siVScrollbar);
				InvalidateVSlider(true);
			}
		}else
		{
			// 滚动条向下滚动
			InvalidateVSlider(true);
			m_siVScrollbar.nPos++;
			if(m_siVScrollbar.nPos > GetVScrollbarMaxPos())
			{
				m_siVScrollbar.nPos = GetVScrollbarMaxPos();
				m_siVScrollbar.nPos2 = m_rtVScrollbar.Height() - m_siVScrollbar.nSliderSize;
			}else
			{
				CalcVSliderPos2();
			}
			// 滚动消息
			SendMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR, (LPARAM)&m_siVScrollbar);
			// 向下滚动
			PostMessage(LKCOMMON_MSG, LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARDOWN, (LPARAM)&m_siVScrollbar);
			InvalidateVSlider(true);
			//CalcVSliderPos2();
			//m_siVScrollbar.nPos2 = m_siVScrollbar.nPos2 < m_rtVScrollbar.Height() - m_siVScrollbar.nSliderSize ? m_siVScrollbar.nPos2 : m_rtVScrollbar.Height() - m_siVScrollbar.nSliderSize;
			//CalcVSliderPos();
		}
	}
	return CLKCtrlBase::OnMouseWheel(nFlags, zDelta, pt);
}

void CLKCtrlBaseEx::OnSize(UINT nType, int cx, int cy)
{
	CLKCtrlBase::OnSize(nType, cx, cy);

	// 计算滚动条位置
	CalcScrollbarRect();
	// 计算垂直滚动条滑块大小
	CalcVSliderSize();
	// 计算水平滚动条滑块大小
	CalcHSliderSize();
}

// 在缓存上绘图
void CLKCtrlBaseEx::OnPaintEx(CDC *pDC, CDC *pMemDC)
{
	if((m_bVScrollbarVisible && m_pVScrollbarImg && !m_rtVScrollbar.IsRectEmpty()) || (m_bHScrollbarVisible && m_pHScrollbarImg && !m_rtHScrollbar.IsRectEmpty()))
	{
		DrawScrollbar(pMemDC);
	}else
	{
		OnPaint(pMemDC);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// 画滚动条及客户区
void CLKCtrlBaseEx::DrawScrollbar(CDC *pDC)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, m_rtClient.Width(), m_rtClient.Height());
	CBitmap *pOldBmp = memDC.SelectObject(&bmp);
	//CalcScrollbarRect();
	if(m_bVScrollbarVisible)
	{
		memDC.BitBlt(m_rtVScrollbar.left, m_rtVScrollbar.top, m_rtVScrollbar.Width(), m_rtVScrollbar.Height(), pDC, m_rtVScrollbar.left, m_rtVScrollbar.top, SRCCOPY);
		DrawVScrollbar(&memDC);
	}
	if(m_bHScrollbarVisible)
	{
		memDC.BitBlt(m_rtHScrollbar.left, m_rtHScrollbar.top, m_rtClient.Width(), m_rtHScrollbar.Height(), pDC, m_rtHScrollbar.left, m_rtHScrollbar.top, SRCCOPY);
		DrawHScrollbar(&memDC);
	}
	OnPaint(pDC);
	if(m_bVScrollbarVisible)
	{
		pDC->BitBlt(m_rtVScrollbar.left, m_rtVScrollbar.top, m_rtVScrollbar.Width(), m_rtVScrollbar.Height(), &memDC, m_rtVScrollbar.left, m_rtVScrollbar.top, SRCCOPY);
	}
	if(m_bHScrollbarVisible)
	{
		pDC->BitBlt(m_rtHScrollbar.left, m_rtHScrollbar.top, m_rtHScrollbar.Width(), m_rtHScrollbar.Height(), &memDC, m_rtHScrollbar.left, m_rtHScrollbar.top, SRCCOPY);
	}
	if(m_bVScrollbarVisible && m_bHScrollbarVisible)
	{
		pDC->BitBlt(m_rtHScrollbar.right, m_rtHScrollbar.top, m_rtClient.right - m_rtHScrollbar.right, m_rtHScrollbar.Height(), &memDC, m_rtHScrollbar.right, m_rtHScrollbar.top, SRCCOPY);
	}
	memDC.SelectObject(pOldBmp);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

// 画垂直滚动条
void CLKCtrlBaseEx::DrawVScrollbar(CDC *pDC)
{
	// 画滑道
	m_pVScrollbarImg->DrawToDC(pDC, m_rtVScrollbar, 3, LKSKINBASE_DRAWBGSTYLE_TOPMIDBOTTOM);
	// 画滑块 
	if(m_siVScrollbar.nSliderSize < m_pVScrollbarImg->GetHeight() / m_pVScrollbarImg->GetImageNumber())
	{
		m_siVScrollbar.nSliderSize = m_pVScrollbarImg->GetHeight() / m_pVScrollbarImg->GetImageNumber();
	}
	CRect rtSlider(m_rtVScrollbar);
	rtSlider.top += m_siVScrollbar.nPos2;
	rtSlider.bottom = rtSlider.top + m_siVScrollbar.nSliderSize;
	m_pVScrollbarImg->DrawToDC(pDC, rtSlider, m_nCurVScrollbarImgIndex, LKSKINBASE_DRAWBGSTYLE_TOPMIDBOTTOM);
}

// 画水平滚动条
void CLKCtrlBaseEx::DrawHScrollbar(CDC *pDC)
{
	// 画滑道
	m_pHScrollbarImg->DrawToDC(pDC, m_rtHScrollbar, 3, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
	// 画滑块 
	if(m_siHScrollbar.nSliderSize < m_pHScrollbarImg->GetWidth())
	{
		m_siHScrollbar.nSliderSize = m_pHScrollbarImg->GetWidth();
	}
	CRect rtSlider(m_rtHScrollbar);
	rtSlider.left += m_siHScrollbar.nPos2;
	rtSlider.right = rtSlider.left + m_siHScrollbar.nSliderSize;
	m_pHScrollbarImg->DrawToDC(pDC, rtSlider, m_nCurHScrollbarImgIndex, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
}

// 计算滚动条位置
void CLKCtrlBaseEx::CalcScrollbarRect()
{
	if(GetSafeHwnd())
	{
		if(m_pVScrollbarImg)
		{
			m_rtVScrollbar = m_rtClient;
			m_rtVScrollbar.left = m_rtClient.right - m_pVScrollbarImg->GetWidth();
		}
		if(m_pHScrollbarImg)
		{
			m_rtHScrollbar = m_rtClient;
			m_rtHScrollbar.top = m_rtClient.bottom - m_pVScrollbarImg->GetWidth();
		}
		if(m_bVScrollbarVisible && m_bHScrollbarVisible && m_rtVScrollbar && m_pHScrollbarImg)
		{
			m_rtVScrollbar.bottom = m_rtClient.bottom - m_pVScrollbarImg->GetWidth();
			m_rtHScrollbar.left = m_rtClient.right - m_pHScrollbarImg->GetWidth();
		}
	}
}

// 更新滑块
void CLKCtrlBaseEx::InvalidateSlider()
{
	// 更新垂直滚动条滑块
	InvalidateVSlider(true);
	// 更新水平滚动条滑块
	InvalidateHSlider();
}

// 更新垂直滚动条滑块
void CLKCtrlBaseEx::InvalidateVSlider(bool bErase)
{
	//CRect rtInvalid(m_rtVScrollbar);
	//rtInvalid.top = m_siVScrollbar.nPos2;
	//rtInvalid.bottom = m_siVScrollbar.nPos2 + m_siVScrollbar.nSliderSize;
	//InvalidateRect(rtInvalid, false);
	InvalidateRect(m_rtVScrollbar, bErase);
}
// 更新水平滚动条滑块
void CLKCtrlBaseEx::InvalidateHSlider()
{
	CRect rtInvalid(m_rtHScrollbar);
	rtInvalid.left = m_siHScrollbar.nPos2;
	rtInvalid.right = m_siHScrollbar.nPos2 + m_siHScrollbar.nSliderSize;
	InvalidateRect(rtInvalid, false);
}

// 设置垂直滚动条滑块位置
void CLKCtrlBaseEx::SetVScrollbarSliderPos(int nPos)
{
	if(m_siVScrollbar.nPos != nPos)
	{
		m_siVScrollbar.nPos = nPos;
		CalcVSliderPos2();
		InvalidateVSlider(true);
	}
}

// 设置水平滚动条滑块位置
void CLKCtrlBaseEx::SetHScrollbarSliderPos(int nPos)
{
	if(m_siHScrollbar.nPos != nPos)
	{
		m_siHScrollbar.nPos = nPos;
		CalcHSliderPos2();
		InvalidateHSlider();
	}
}

// 根据pos计算垂直滚动条滑块位置pos2
void CLKCtrlBaseEx::CalcVSliderPos2()
{
    if(m_siVScrollbar.nPos >= m_siVScrollbar.nMax - m_siVScrollbar.nPosPerPage)
    {
        m_siVScrollbar.nPos = m_siVScrollbar.nMax - m_siVScrollbar.nPosPerPage;
        m_siVScrollbar.nPos2 = m_rtVScrollbar.Height() - m_siVScrollbar.nSliderSize;
    }else
    {
	    m_siVScrollbar.nPos2 = m_rtVScrollbar.Height() * m_siVScrollbar.nPos / (m_siVScrollbar.nMax ? m_siVScrollbar.nMax : 1);
	    int nH = m_rtVScrollbar.Height()/* - m_siVScrollbar.nSliderSize*/;
	    if(m_siVScrollbar.nPos2 > nH - m_siVScrollbar.nSliderSize)
	    {
		    m_siVScrollbar.nPos2 = nH - m_siVScrollbar.nSliderSize;
	    }
    }
}

// 根据pos2计算垂直滚动条滑块位置pos
void CLKCtrlBaseEx::CalcVSliderPos()
{
	//int nH = m_rtVScrollbar.Height() ? m_rtVScrollbar.Height() ; 1;
	//m_siVScrollbar.nPos = m_siVScrollbar.nPos2 * m_siVScrollbar.nMax / nH;
	int nH = m_rtVScrollbar.Height()/* - m_siVScrollbar.nSliderSize*/;
	m_siVScrollbar.nPos = m_siVScrollbar.nMax * m_siVScrollbar.nPos2 / nH; 
	if((m_siVScrollbar.nMax * m_siVScrollbar.nPos2) % nH > 0.9 * nH)
	{
		m_siVScrollbar.nPos++;
	}
	//if(nH == m_siVScrollbar.nPos2 + m_siVScrollbar.nSliderSize)
	//{
	//	m_siVScrollbar.nPos++;
	//	return;
	//}
}

// 根据pos计算水平滚动条滑块位置pos2
void CLKCtrlBaseEx::CalcHSliderPos2()
{
	m_siHScrollbar.nPos2 = m_rtHScrollbar.Width() * m_siHScrollbar.nPos / (m_siHScrollbar.nMax ? m_siHScrollbar.nMax : 1);
}

// 根据pos2计算水平滚动条滑块位置pos
void CLKCtrlBaseEx::CalcHSliderPos()
{
	m_siHScrollbar.nPos = m_siHScrollbar.nMax * m_siHScrollbar.nPos2 / m_rtHScrollbar.Width(); 
}

// 计算垂直滚动条滑块大小
void CLKCtrlBaseEx::CalcVSliderSize()
{
	if(GetSafeHwnd() && m_pVScrollbarImg)
	{
		m_siVScrollbar.nSliderSize = m_rtVScrollbar.Height() * m_rtVScrollbar.Height() / (m_siVScrollbar.nMax2 ? m_siVScrollbar.nMax2 : 1);
		if(m_siVScrollbar.nSliderSize > m_rtVScrollbar.Height())
		{
			m_siVScrollbar.nSliderSize = m_rtVScrollbar.Height();
			m_siVScrollbar.nPos = 0;
			m_siVScrollbar.nPos2 = 0;
		}
		if(m_siVScrollbar.nPos2 > m_rtVScrollbar.Height() - m_siVScrollbar.nSliderSize)
		{
			m_siVScrollbar.nPos2 = m_rtVScrollbar.Height() - m_siVScrollbar.nSliderSize;
		}
        int nCountPerPage = 0;
        if(m_siVScrollbar.nMax2 > 0 && m_rtVScrollbar.Height() <= m_siVScrollbar.nMax2)
        {
            nCountPerPage = m_siVScrollbar.nMax * m_rtVScrollbar.Height() / m_siVScrollbar.nMax2;
            if(m_siVScrollbar.nMax * m_rtVScrollbar.Height() % m_siVScrollbar.nMax2 > m_rtVScrollbar.Height())
            {
                nCountPerPage++;
            }
        }
        m_siVScrollbar.nPosPerPage = nCountPerPage;
        // TRACE(_T("nPosPerPage: %d; nCountPerPage: %d\r\n"), m_siVScrollbar.nPosPerPage, nCountPerPage);
	}
}

// 计算水平滚动条滑块大小
void CLKCtrlBaseEx::CalcHSliderSize()
{
	if(GetSafeHwnd() && m_pHScrollbarImg)
	{
		m_siHScrollbar.nSliderSize = m_rtHScrollbar.Width() * m_rtHScrollbar.Width() / (m_siHScrollbar.nMax2 ? m_siHScrollbar.nMax2 : 1);
		if(m_siHScrollbar.nSliderSize > m_rtHScrollbar.Width())
		{
			m_siHScrollbar.nSliderSize = m_rtHScrollbar.Width();
			m_siHScrollbar.nPos = 0;
			m_siHScrollbar.nPos2 = 0;
		}
	}
}

// 获取垂直滚动条滑块最下边的位置
int CLKCtrlBaseEx::GetVScrollbarMaxPos()
{
	if(m_siVScrollbar.nMax2 <= 0) return 0;
	int nCount = m_siVScrollbar.nMax * m_rtVScrollbar.Height() / m_siVScrollbar.nMax2;
	return m_siVScrollbar.nMax - nCount;
}

// 获取水平滚动条滑块最右边的位置
int CLKCtrlBaseEx::GetHScrollbarMaxPos()
{
	if(m_siHScrollbar.nMax2 <= 0) return 0;
	int nCount = m_siHScrollbar.nMax * m_rtHScrollbar.Width() / m_siHScrollbar.nMax2;
	return m_siHScrollbar.nMax - nCount;
}
