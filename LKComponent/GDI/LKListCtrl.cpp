// ..\..\common\LKComponent\GDI\LKListCtrl.cpp : ʵ���ļ�
//

#include "LKListCtrl.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// ���ؼ���ʶ
// �б�ͷ
#define LKLISTCTRL_HEADERID	100
// ������
#define LKLISTCTRL_LEFTID	101

// CLKListCtrl

IMPLEMENT_DYNAMIC(CLKListCtrl, CLKCtrlBaseEx)

CLKListCtrl::CLKListCtrl(int nImageIndex)
: CLKCtrlBaseEx(nImageIndex)
// �Ƿ���Ҫ��ʾ������
, m_bShowHeader(true)
// �Ƿ���Ҫ��ʾ�������
, m_bShowLeft(false)
//�ڴ滺���Ƿ����Լ�������
, m_bImgIsOwner(false)
// ������̬ͼƬ(�ڴ滺��)
, m_pNormalImg(0)
// ��ͷ
, m_pHeader(0)
// ������
, m_pLeft(0)
// �ɼ���ʼ��
, m_nBeginIndex(0)
// �ɼ�������
, m_nEndIndex(0)
// ��ǰ������(����ƶ�)����ڿɼ���ʼ��ƫ��
, m_nCurIndex(-1)
// ѡ�������� �������ʼ��ƫ��
, m_nSelectIndex(-1)
// ����ѡ��ǰ��ͼƬ
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

// ����һ��
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

// ����һ��
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
			// �������������
			CalcShowEndIndex();
		}
	}
	return nRet;
}

// ���������
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

// ��ȡָ������ı�
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



// CLKListCtrl ��Ϣ�������
LRESULT CLKListCtrl::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	// �б�ͷ��ȸı�
	if(LKLISTCTRL_MSG_WPARAM_HEADWIDTHCHANGED == wParam)
	{
		// ��������������
		CalcDataRect();
		CRect rt(m_rtData);
		rt.right = m_rtClient.right;
		InvalidateRect(&rt);
		//Invalidate();
	}else if(LKCTRLBASEEX_MSG_WPARAM_HSCROLLBAR == wParam)
	{
		// ˮƽ����
		LKSCROLLINFO *pInfo = (LKSCROLLINFO *)lParam;
		// ������Ϣ
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

	// TODO: �ڴ˴������Ϣ����������
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

	// �������������
	CalcShowEndIndex();
}


// �����ؼ����ʼ��
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

// ˢ��ǰ������
void CLKListCtrl::ReSetMem()
{
	if(m_bImgIsOwner)
	{
		ParseImageToMem();
	}
}

// ���������Ϣ
bool CLKListCtrl::OnEraseBG(CDC *pDC)
{
	bool b = CLKCtrlBaseEx::OnEraseBG(pDC);
	//SetEraseBG(false);
	return b;
}

// ���������ͼƬ���ڴ滺����
void CLKListCtrl::ParseImageToMem()
{
	m_bImgIsOwner = true;
	// ��ȡ��Ҫ������ͼƬ
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
			tme.dwHoverTime = HOVER_DEFAULT; //ֻ��HOVER��Ч
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

// �ڻ����ϻ�ͼ
void CLKListCtrl::OnPaint(CDC *pDC)
{
	// ��ѡ����
	DWORD dw = ::GetTickCount();
	DrawSelectedItem(pDC);
	if(m_nCurIndex != m_nSelectIndex - m_nBeginIndex)
	{
		// ����ǰ��
		DrawCurItem(pDC);
		//TRACE(_T("DrawCurItem:\t%dms\r\n"), ::GetTickCount() - dw);
	}
	// ���ߺ��ı�
	DrawLineText(pDC);
}

// ��ʼ��ͼƬ�б�
void CLKListCtrl::InitImageList()
{
	// ��ʼ��ͼƬ�б�
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_COMMON_BEGIN + 1, LKIMAGELISTINDEX_COMMON_BEGIN + 1);
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_LISTCTRL_BEGIN, LKIMAGELISTINDEX_LISTCTRL_END);
	m_pSelectImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_SELECTANDFOCUS);
	SetScrollbarImg(CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR), CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_HSCROLLBAR));
}


// �ͷŻ���ǰ��
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

// �ͷ�����
void CLKListCtrl::FreeData()
{
	for(int i = 0; i < m_arrData.GetCount(); i++)
	{
		delete m_arrData[i];
	}
	m_arrData.RemoveAll();
	// �ɼ���ʼ��
	m_nBeginIndex = 0;
	// �ɼ�������
	m_nEndIndex = 0;
	// ��ǰ������(����ƶ�)����ڿɼ���ʼ��ƫ��
	m_nCurIndex = -1;
	// ѡ�������� �������ʼ��ƫ��
	m_nSelectIndex = -1;
}

// �������������
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

// ��ѡ����
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

// ����ǰ��
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

// ���ߺ��ı�
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
	// �Ȼ�γ��
	for(int i = m_nBeginIndex; i < m_nEndIndex; i++)
	{
		CLKListCtrlRow *pItem = m_arrData[i];
		CRect rtText(m_rtData.left, nBottom, 0, nBottom + pItem->m_nHeight);
		nBottom += pItem->m_nHeight;
		pDC->MoveTo(m_rtData.left, nBottom);
		pDC->LineTo(nRight, nBottom);
		// ���ı�
		for(int j = 0; j < pItem->m_arrRow.GetCount(); j++)
		{
			rtText.right = rtText.left + m_pHeader->GetItemWidth(j);
			CString strText = pItem->m_arrRow[j];
			pDC->DrawText(strText, rtText, pItem->m_nFormat);
			rtText.left = rtText.right;
		}
		
	}
	// ������
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

// �������ָ���λ�û�ȡ�������������������ʼ�ɼ�����
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

// ���ݿɼ�������ȡָ���������
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

// ��������������
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
