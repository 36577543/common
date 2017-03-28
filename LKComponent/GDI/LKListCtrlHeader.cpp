// ..\..\common\LKComponent\GDI\LKListCtrlHeader.cpp : 实现文件
//

#include "LKListCtrlHeader.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// 最小的列表头长度
#define MIN_HEADERITEM_WIDTH	10

// 各控件标识
// 编辑框
#define LKLISTCTRL_EDITID	200

// CLKListCtrlHeader

IMPLEMENT_DYNAMIC(CLKListCtrlHeader, CLKCtrlBase)

CLKListCtrlHeader::CLKListCtrlHeader(int nImageIndex)
: CLKCtrlBase(nImageIndex)
, m_nHeight(20)
, m_nOffset(0)
, m_ptOld(0, 0)
, m_nCurCol(-1)
, m_bIsDrag(false)
{

}

CLKListCtrlHeader::~CLKListCtrlHeader()
{
	FreeItems();
}

BOOL CLKListCtrlHeader::Create(CWnd* pParentWnd, UINT nID)
{
	CRect rt(0, 0, 0, 0);
	pParentWnd->GetClientRect(&rt);
	rt.bottom = rt.top + m_nHeight;
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE, rt, pParentWnd, nID); 
	return bRet;
}

// 获取列总宽度
int CLKListCtrlHeader::GetItemsWidth()
{
	int nWidth(0);
	for(int i = 0; i < m_arrItem.GetCount(); i++)
	{
		CLKListCtrlHeaderItem  *p = m_arrItem[i];
		nWidth += p->m_nWidth;
	}
	return nWidth;
}

// 获取列宽度
int CLKListCtrlHeader::GetItemWidth(int nIndex)
{
	int nWidth(0);
	if(nIndex >= 0 && nIndex < m_arrItem.GetCount())
	{
		nWidth = m_arrItem[nIndex]->m_nWidth;
	}
	return nWidth;
}
// 获取所有列左右端位置
void CLKListCtrlHeader::GetLeftAndRight(int *pLeft, int *pRight)
{
	*pLeft = -m_nOffset;
	*pRight = *pLeft + GetItemsWidth();
}

// 增加一项
int CLKListCtrlHeader::AddItem(CString strCaption, int nWidth, UINT uStyle, int	nFormat, CLKImage *pImg)
{
	CLKListCtrlHeaderItem *pItem = new CLKListCtrlHeaderItem(strCaption, nWidth, uStyle, nFormat, pImg);
	if(LKLISTCTRL_INNERCTRL_STYLE_EDIT == uStyle)
	{
		CEdit *p = new CEdit;
		CRect rt(0, 0, 0, 0);
		p->Create(WS_CHILD, rt, GetParent(), LKLISTCTRL_EDITID);
		pItem->m_pWnd = p;
	}
	return AddItem(pItem);
}

BEGIN_MESSAGE_MAP(CLKListCtrlHeader, CLKCtrlBase)
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()



// CLKListCtrlHeader 消息处理程序
LRESULT CLKListCtrlHeader::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	// 列表头宽度改变
	if(LKCTRLBASEEX_MSG_WPARAM_HSCROLLBAR == wParam)
	{
		// 水平滚动
		m_nOffset = (int)lParam;
		RedrawWindow();
		//Invalidate();
		::SendMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKLISTCTRL_MSG_WPARAM_HEADWIDTHCHANGED, (LPARAM)this);
		return 1;
	}
	return CLKCtrlBase::OnCommonMsg(wParam, lParam);
}



// 初始化图片列表
void CLKListCtrlHeader::InitImageList()
{
	// 初始化图片列表
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_LISTCTRLHEADER_BEGIN, LKIMAGELISTINDEX_LISTCTRLHEADER_END);
}


// 创建控件后初始化
void CLKListCtrlHeader::InitCtrlAfterCreate()
{
	ParseImageToMem();
}

// 刷新前景缓存
void CLKListCtrlHeader::ReSetMem()
{
	ParseImageToMem();
}

// 在缓存上绘图
void CLKListCtrlHeader::OnPaint(CDC *pDC)
{
	int nLeft(-m_nOffset);
	for(int i = 0; i < m_arrItem.GetCount(); i++)
	{
		CLKListCtrlHeaderItem  *p = m_arrItem[i];
		CPen linePen(PS_SOLID, 1, RGB(117, 165, 206));
		CPen *pOldPen =  pDC->SelectObject(&linePen);
		pDC->MoveTo(nLeft + p->m_nWidth, m_rtClient.top);
		pDC->LineTo(nLeft + p->m_nWidth, m_rtClient.bottom);
		if(!p->m_strCaption.IsEmpty())
		{
			CRect rtText(nLeft, m_rtClient.top, nLeft + p->m_nWidth, m_rtClient.bottom);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_crText);
			CFont *pOldFont = pDC->SelectObject(CLKFontMgr::GetST100());
			pDC->DrawText(p->m_strCaption, &rtText, p->m_nFormat);
			pDC->SelectObject(pOldFont);
		}
		pDC->SelectObject(pOldPen);
		nLeft += p->m_nWidth;
	}
}

// 增加一项
int CLKListCtrlHeader::AddItem(CLKListCtrlHeaderItem *pItem)
{
	return m_arrItem.Add(pItem);
}

// 释放所有项
void CLKListCtrlHeader::FreeItems()
{
	for(int i = 0; i < m_arrItem.GetCount(); i++)
	{
		CLKListCtrlHeaderItem  *p = m_arrItem[i];
		if(LKLISTCTRL_INNERCTRL_STYLE_NONE != p->m_nCtlStyle && p->m_pWnd)
		{
			delete p->m_pWnd;
		}
		delete p;
	}
	m_arrItem.RemoveAll();
}

void CLKListCtrlHeader::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(::LoadCursor(NULL, IDC_SIZEWE) == GetCursor())
	{
		m_ptOld = point;
		m_bIsDrag = true;
	}
	CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKListCtrlHeader::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bIsDrag = false;
	CLKCtrlBase::OnLButtonUp(nFlags, point);
}

void CLKListCtrlHeader::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CLKCtrlBase::OnLButtonDblClk(nFlags, point);
}

void CLKListCtrlHeader::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bIsDrag = false;
	CLKCtrlBase::OnMouseLeave();
}

void CLKListCtrlHeader::OnMouseMove(UINT nFlags, CPoint point)
{
	if(MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		if(m_bIsDrag)
		{
			int nOffset = point.x - m_ptOld.x;
			for(int nCol = 0; nCol < m_arrItem.GetCount(); nCol++)
			{
				CLKListCtrlHeaderItem *p = m_arrItem[nCol];
				if(nCol == m_nCurCol)
				{
					if(p->m_nWidth + nOffset < MIN_HEADERITEM_WIDTH) return;
					p->m_nWidth += nOffset;
					m_ptOld = point;
					//Invalidate();
					RedrawWindow();
					// 发送消息给父窗体
					::SendMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKLISTCTRL_MSG_WPARAM_HEADWIDTHCHANGED, (LPARAM)this);
					break;
				}
			}
		}
	}

	if(m_bIsDrag)
	{
		TRACKMOUSEEVENT   tme;   
		tme.cbSize		= sizeof(tme);   
		tme.dwFlags		= TME_LEAVE; 
		tme.hwndTrack	= this->GetSafeHwnd();   
		tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
		::TrackMouseEvent(&tme);
	}
	CLKCtrlBase::OnMouseMove(nFlags, point);
}

BOOL CLKListCtrlHeader::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	CRect rt(-m_nOffset - 2, 0, -m_nOffset + 2, m_rtClient.bottom);
	for(int i = 0; i < m_arrItem.GetCount(); i++)
	{
		CLKListCtrlHeaderItem  *p = m_arrItem[i];
		rt.OffsetRect(p->m_nWidth, 0);
		if(rt.PtInRect(point))
		{
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			m_nCurCol = i;
			return true;
			//return CWnd::OnSetCursor(pWnd, nHitTest, message);
		}
	}
	if(m_bIsDrag)
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		return true;
	}
	return CLKCtrlBase::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CLKListCtrlHeader::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//int nWidth(0);
	//CPoint pt(point);
	//ScreenToClient(&pt);
	//CRect rt(m_nOffset - 2, 0, m_nOffset + 2, m_rtClient.bottom);
	//for(int i = 0; i < m_arrItem.GetCount(); i++)
	//{
	//	CLKListCtrlHeaderItem  *p = m_arrItem[i];
	//	rt.OffsetRect(p->m_nWidth, 0);
	//	if(rt.PtInRect(pt))
	//	{
	//		return HTNOWHERE;
	//	}
	//}
	return CLKCtrlBase::OnNcHitTest(point);
}
