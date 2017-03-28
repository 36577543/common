// ..\..\common\LKComponent\GDI\LKListCtrl.cpp : 实现文件
//

#include "LKListCtrl.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// 各控件标识
// 列表头
#define LKLISTCTRL_HEADERID	100
// 左序列
#define LKLISTCTRL_LEFTID	101

// CLKListCtrl

IMPLEMENT_DYNAMIC(CLKListCtrl, CLKCtrlBaseEx)

CLKListCtrl::CLKListCtrl(int nImageIndex)
: CLKCtrlBaseEx(nImageIndex)
// 是否需要显示顶部项
, m_bShowHeader(true)
// 是否需要显示左边序列
, m_bShowLeft(false)
//内存缓冲是否是自己创建的
, m_bImgIsOwner(false)
// 背景常态图片(内存缓冲)
, m_pNormalImg(0)
// 列头
, m_pHeader(0)
// 左序列
, m_pLeft(0)
// 可见起始项
, m_nBeginIndex(0)
// 可见结束项
, m_nEndIndex(0)
// 当前项索引(鼠标移动)相对于可见起始项偏移
, m_nCurIndex(-1)
// 选中项索引 相对于起始项偏移
, m_nSelectIndex(-1)
// 高亮选中前景图片
, m_pSelectImg(0)
{
	m_rtClientOffset = CRect(1, 1, 1, 1);
}

CLKListCtrl::~CLKListCtrl()
{
	FreeData();
	FreeMemImg();
	if(m_pHeader)
	{
		delete m_pHeader;
	}
	if(m_pLeft)
	{
		delete m_pLeft;
	}
}

BOOL CLKListCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBaseEx::Create(_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKListCtrl::Create(CString strCatpion, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBaseEx::Create(strCatpion, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}

// 增加一列
int CLKListCtrl::AddColumn(CString strCaption, int nWidth, UINT uStyle, int nFormat, CLKImage *pImg)
{
	int nRet = -1;
	if(m_pHeader)
	{
		nRet = m_pHeader->AddItem(strCaption, nWidth, uStyle, nFormat, pImg);
		//m_rtData = m_rtClient;
		if(m_pHeader && m_pHeader->GetSafeHwnd())
		{
			int nRight = m_rtData.left + m_pHeader->GetItemsWidth();
			if(nRight < m_rtClient.right) 
			{
				m_rtData.right = nRight;
			}else
			{
				//m_rtData.right = m_rtClient.right;
				m_rtData.right = nRight;
				SetHScrollbarVisible();
				SetHScrollbarRang(m_pHeader->GetItemsWidth());
			}
		}
	}
	return nRet;
}

// 增加一项
int CLKListCtrl::AddItem(CString strCaption)
{
	int nRet(-1);
	if(m_pHeader)
	{
		int nCount = m_pHeader->GetCount();
		if(nCount > 0)
		{
			CLKListCtrlRow *pItem = new CLKListCtrlRow(nCount);
			pItem->m_arrRow.SetAt(0, strCaption);
			nRet = m_arrData.Add(pItem);
			// 计算结束项索引
			CalcShowEndIndex();
		}
	}
	return nRet;
}

// 设置项标题
void CLKListCtrl::SetItemText(int nIndex, int nChildIndex, CString strCaption)
{
	if(nIndex >= 0 && nIndex < m_arrData.GetCount())
	{
		CLKListCtrlRow *pItem = m_arrData[nIndex];
		if(nChildIndex >= 0 && nChildIndex < pItem->m_arrRow.GetCount())
		{
			pItem->m_arrRow.SetAt(nChildIndex, strCaption);
		}
	}
}

// 获取指定项的文本
CString CLKListCtrl::GetItemText(int nIndex, int nChildIndex)
{
	CString strText(_T(""));
	if(nIndex >= 0 && nIndex < m_arrData.GetCount())
	{
		CLKListCtrlRow *pItem = m_arrData[nIndex];
		if(nChildIndex >= 0 && nChildIndex < pItem->m_arrRow.GetCount())
		{
			strText = pItem->m_arrRow[nChildIndex];
		}
	}
	return strText;
}

BEGIN_MESSAGE_MAP(CLKListCtrl, CLKCtrlBaseEx)
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
END_MESSAGE_MAP()



// CLKListCtrl 消息处理程序
LRESULT CLKListCtrl::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	// 列表头宽度改变
	if(LKLISTCTRL_MSG_WPARAM_HEADWIDTHCHANGED == wParam)
	{
		// 计算数据区域宽度
		CalcDataRect();
		CRect rt(m_rtData);
		rt.right = m_rtClient.right;
		InvalidateRect(&rt);
		//Invalidate();
	}else if(LKCTRLBASEEX_MSG_WPARAM_HSCROLLBAR == wParam)
	{
		// 水平滚动
		LKSCROLLINFO *pInfo = (LKSCROLLINFO *)lParam;
		// 滚动消息
		m_pHeader->SendMessage(LKCOMMON_MSG, wParam, (LPARAM)pInfo->nPos);
		//m_rtData.left = -pInfo->nPos;
		InvalidateRect(&m_rtData);
		return 1;
	}
	return CLKCtrlBaseEx::OnCommonMsg(wParam, lParam);
}

void CLKListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CLKCtrlBaseEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_rtData = m_rtClient;
	if(m_pHeader && m_pHeader->GetSafeHwnd())
	{
		int nRight = m_rtData.left + m_pHeader->GetItemsWidth();
		if(nRight < m_rtData.right) m_rtData.right = nRight; 
		if(m_pHeader->IsWindowVisible())
		{
			CRect rtHeader;
			m_pHeader->GetWindowRect(&rtHeader);
			m_rtData.top += rtHeader.Height();
		}
	}
	if(m_pLeft && m_pLeft->GetSafeHwnd() && m_pLeft->IsWindowVisible())
	{
		CRect rtLeft;
		m_pHeader->GetWindowRect(&rtLeft);
		m_rtData.left += rtLeft.Width();
	}

	// 计算结束项索引
	CalcShowEndIndex();
}


// 创建控件后初始化
void CLKListCtrl::InitCtrlAfterCreate()
{
	ParseImageToMem();
	if(m_bShowHeader)
	{
		m_pHeader = new CLKListCtrlHeader;
		m_pHeader->Create(this, LKLISTCTRL_HEADERID);
		m_rtData = m_rtClient;
		CRect rtHeader;
		m_pHeader->GetWindowRect(&rtHeader);
		m_rtData.top += rtHeader.Height();
	}
	CLKCtrlBaseEx::InitCtrlAfterCreate();
}

// 刷新前景缓存
void CLKListCtrl::ReSetMem()
{
	if(m_bImgIsOwner)
	{
		ParseImageToMem();
	}
}

// 察除背景消息
bool CLKListCtrl::OnEraseBG(CDC *pDC)
{
	bool b = CLKCtrlBaseEx::OnEraseBG(pDC);
	//SetEraseBG(false);
	return b;
}

// 解析出相关图片到内存缓存中
void CLKListCtrl::ParseImageToMem()
{
	m_bImgIsOwner = true;
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		// 
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		//GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETBGIMG, (LPARAM)&prm);
		GetParentImage(&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);
		int nNumber = pImg->GetImageNumber();
		if(nNumber > 0)
		{
			CRect rtWnd;
			GetWindowRect(&rtWnd);
			rtWnd.MoveToXY(0, 0);
			if(m_pNormalImg) delete m_pNormalImg;
			m_pNormalImg = new CLKImage(rtWnd.Width(), rtWnd.Height());
			pImg->DrawToImage(m_pNormalImg, 0, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
			SetMemImg(m_pNormalImg);
		}
	}
}

void CLKListCtrl::OnMouseLeave()
{
	CRect rtItem(0, 0, 0, 0);
	if(GetItemRectByIndex(m_nCurIndex, &rtItem))
	{
		InvalidateRect(&rtItem, false);
	}
	m_nCurIndex = -1;
	CLKCtrlBaseEx::OnMouseLeave();
}

void CLKListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int nIndex = GetItemIndexByPoint(point);
	if(nIndex != m_nCurIndex)
	{
		CRect rtItem(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
			InvalidateRect(&rtItem, false);
		}
		m_nCurIndex = nIndex;
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
			InvalidateRect(&rtItem, false);
		}
		if(nIndex >= 0)
		{
			TRACKMOUSEEVENT   tme;   
			tme.cbSize		= sizeof(tme);   
			tme.dwFlags		= TME_LEAVE; 
			tme.hwndTrack	= this->GetSafeHwnd();   
			tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
			::TrackMouseEvent(&tme);
		}
	}
	CLKCtrlBaseEx::OnMouseMove(nFlags, point);
}


void CLKListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_nCurIndex != m_nSelectIndex - m_nBeginIndex && !PtInScrollbar(point))
	{
		CRect rtItem(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nSelectIndex - m_nBeginIndex, &rtItem))
		{
			InvalidateRect(&rtItem, false);
		}
		m_nSelectIndex = m_nCurIndex + m_nBeginIndex;
		rtItem = CRect(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
			InvalidateRect(&rtItem, false);
		}
		::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKLISTCTRL_MSG_WPARAM_ITEMCHANGED, (LPARAM)GetSafeHwnd());
	}
	CLKCtrlBaseEx::OnLButtonDown(nFlags, point);
}

void CLKListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	InitLClick();
	::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	CLKCtrlBaseEx::OnLButtonUp(nFlags, point);
}

// 在缓存上绘图
void CLKListCtrl::OnPaint(CDC *pDC)
{
	// 画选中项
	DWORD dw = ::GetTickCount();
	DrawSelectedItem(pDC);
	if(m_nCurIndex != m_nSelectIndex - m_nBeginIndex)
	{
		// 画当前项
		DrawCurItem(pDC);
		//TRACE(_T("DrawCurItem:\t%dms\r\n"), ::GetTickCount() - dw);
	}
	// 画线和文本
	DrawLineText(pDC);
}

// 初始化图片列表
void CLKListCtrl::InitImageList()
{
	// 初始化图片列表
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_COMMON_BEGIN + 1, LKIMAGELISTINDEX_COMMON_BEGIN + 1);
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_LISTCTRL_BEGIN, LKIMAGELISTINDEX_LISTCTRL_END);
	m_pSelectImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_SELECTANDFOCUS);
	SetScrollbarImg(CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR), CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_HSCROLLBAR));
}


// 释放缓存前景
void CLKListCtrl::FreeMemImg()
{
	if(m_bImgIsOwner)
	{
		if(m_pNormalImg) 
		{
			delete m_pNormalImg;
			m_pNormalImg = 0;
		}
	}
}

// 释放数据
void CLKListCtrl::FreeData()
{
	for(int i = 0; i < m_arrData.GetCount(); i++)
	{
		delete m_arrData[i];
	}
	m_arrData.RemoveAll();
	// 可见起始项
	m_nBeginIndex = 0;
	// 可见结束项
	m_nEndIndex = 0;
	// 当前项索引(鼠标移动)相对于可见起始项偏移
	m_nCurIndex = -1;
	// 选中项索引 相对于起始项偏移
	m_nSelectIndex = -1;
}

// 计算结束项索引
void CLKListCtrl::CalcShowEndIndex()
{
	m_rtClient;
	int nHeight(0);
	for(m_nEndIndex = m_nBeginIndex; m_nEndIndex < m_arrData.GetCount(); m_nEndIndex++)
	{
		CLKListCtrlRow *pItem = m_arrData[m_nEndIndex];
		if(nHeight >= m_rtData.Height()) break;
		nHeight += pItem->m_nHeight;
	}
}

// 画选中项
void CLKListCtrl::DrawSelectedItem(CDC *pDC)
{
	if(m_nSelectIndex - m_nBeginIndex >= 0 && m_nSelectIndex < m_nEndIndex)
	{
		CRect rtDest(m_rtData);
		rtDest.left = 0;
		rtDest.right = rtDest.right < m_rtClient.right ? rtDest.right : m_rtClient.right;
		rtDest.top += LKLISTCTRLROW_DEFAULT_HIGHT * (m_nSelectIndex - m_nBeginIndex) + 1;
		rtDest.bottom = rtDest.top + LKLISTCTRLROW_DEFAULT_HIGHT;
		m_pSelectImg->DrawToDC(pDC, rtDest, 1, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
		//int nL = 5;
		//int nR = 5;
		//int nHeight = m_pSelectImg->GetHeight() / m_pSelectImg->GetImageNumber();
		//m_pSelectImg->DrawLRToDC(pDC, rtDest, nL, nR, nHeight);
		//m_pSelectImg->DrawLRToDCEx(pDC, rtDest, nL, nR, nHeight);
	}
}

// 画当前项
void CLKListCtrl::DrawCurItem(CDC *pDC)
{
	if(m_nCurIndex >= 0)
	{
		CRect rtDest(m_rtData);
		rtDest.left = 0;
		rtDest.right = rtDest.right < m_rtClient.right ? rtDest.right : m_rtClient.right;
		rtDest.top += LKLISTCTRLROW_DEFAULT_HIGHT * m_nCurIndex;
		rtDest.bottom = rtDest.top + LKLISTCTRLROW_DEFAULT_HIGHT;
		m_pSelectImg->DrawToDC(pDC, rtDest, 0, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
		//int nL = 4;
		//int nR = 4;
		//m_pSelectImg->DrawLRToDC(pDC, rtDest, nL, nR, 0);
	}
}

// 画线和文本
void CLKListCtrl::DrawLineText(CDC *pDC)
{
	CPen linePen(PS_SOLID, 1, RGB(117, 165, 206));
	CPen *pOldPen =  pDC->SelectObject(&linePen);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crText);
	CFont *pOldFont = pDC->SelectObject(CLKFontMgr::GetST90());
	int nRight = m_rtData.left + m_pHeader->GetItemsWidth();
	int nBottom(m_rtData.top);
	int nTop = m_rtData.top; 
	// 先画纬线
	for(int i = m_nBeginIndex; i < m_nEndIndex; i++)
	{
		CLKListCtrlRow *pItem = m_arrData[i];
		CRect rtText(m_rtData.left, nBottom, 0, nBottom + pItem->m_nHeight);
		nBottom += pItem->m_nHeight;
		pDC->MoveTo(m_rtData.left, nBottom);
		pDC->LineTo(nRight, nBottom);
		// 画文本
		for(int j = 0; j < pItem->m_arrRow.GetCount(); j++)
		{
			rtText.right = rtText.left + m_pHeader->GetItemWidth(j);
			CString strText = pItem->m_arrRow[j];
			pDC->DrawText(strText, rtText, pItem->m_nFormat);
			rtText.left = rtText.right;
		}
		
	}
	// 画经线
	nRight = m_rtData.left;
	for(int i = 0; i < m_pHeader->GetCount(); i++)
	{
		nRight += m_pHeader->GetItemWidth(i);
		pDC->MoveTo(nRight, m_rtData.top);
		pDC->LineTo(nRight, nBottom);
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}

// 根据鼠标指针的位置获取项索引　－　相对于起始可见索引
int CLKListCtrl::GetItemIndexByPoint(CPoint point)
{
	//int nBase = point.y / LKLISTCTRLROW_DEFAULT_HIGHT;
	CRect rt(m_rtData);
	for(int i = m_nBeginIndex; i < m_nEndIndex; i++)
	{
		CLKListCtrlRow *pItem = m_arrData[i];
		rt.bottom = rt.top + pItem->m_nHeight;
		if(rt.PtInRect(point))
		{
			return i - m_nBeginIndex;
		}
		rt.top = rt.bottom;
	}
	return -1;
}

// 根据可见索引获取指定项的区域
bool CLKListCtrl::GetItemRectByIndex(int nIndex, LPRECT lpRect)
{
	CRect rt(m_rtData);
	for(int i = m_nBeginIndex; i < m_nEndIndex; i++)
	{
		CLKListCtrlRow *pItem = m_arrData[i];
		if(i == nIndex)
		{
			rt.bottom = rt.top + pItem->m_nHeight;
			*lpRect = rt;
			return true;
		}
		rt.top += pItem->m_nHeight;
	}
	return false;
}

// 计算数据区域宽度
void CLKListCtrl::CalcDataRect()
{
	m_pHeader->GetLeftAndRight((int *)&m_rtData.left, (int *)&m_rtData.right);
	if(m_rtData.Width() > m_rtClient.Width())
	{
		SetHScrollbarVisible();
		SetHScrollbarRang(m_rtData.Width());
	}else
	{
		SetHScrollbarVisible(false);
	}
}
