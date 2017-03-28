// ..\..\..\common\LKComponent\gdi\LKTabEx.cpp : ʵ���ļ�
//

#include "LKTabEx.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// ��������
// ������Ŀ��
#define const_max_tabitem_width     120

// CLKTabEx

IMPLEMENT_DYNAMIC(CLKTabEx, CLKCtrlBase)

CLKTabEx::CLKTabEx(int nImageIndex)
: CLKCtrlBase(nImageIndex)
, m_pBGImage(0)
, m_nMargin(2)
, m_nItemSplit(2)
, m_nItemWidth(61)
, m_nItemHeight(23)
, m_nSelectIndex(-1)
, m_nCurIndex(-1)
// ��ʾ�ɼ��������
, m_nVisibleBeginIndex(-1)
// �Ƿ��Զ�������ߴ�
, m_bIsAdjustItmeSize(false)
, m_bInCloseBtn(false)
, m_bMouseDown(false)
// �Ƿ���Ҫ�رհ�ť
, m_bMustCloseButton(false)
// �رհ�ťͼƬ
, m_pCloseImg(0)
, m_rtArr(0, 0, 0, 0)
// ���ͷ��ť
, m_pLeftArrImg(0)
// �Ҽ�ͷ��ť
, m_pRightArrImg(0)
, m_nCurArrIndex(-1)
// ���ͷ�ɼ���
, m_bVisibleLArr(false)
// �Ҽ�ͷ�ɼ���
, m_bVisibleRArr(false)
{

}

CLKTabEx::~CLKTabEx()
{
    for(int i = 0; i < m_arrItems.GetCount(); i++)
    {
        LKTabItem *pItem = m_arrItems[i];
        if(pItem) delete pItem;
    }
    m_arrItems.RemoveAll();
}

BOOL CLKTabEx::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
    ReCalcItemSize();
	return bRet;
}


// ����һ��
void CLKTabEx::AddItem(CString strText, int nImgIndex, CWnd *pWnd, void *pData)
{
	AddItem(strText, CLKImageMgr::GetImageS(nImgIndex), pWnd, false, pData);
}

void CLKTabEx::AddItem(CLKImage *pImg, CWnd *pWnd, void *pData)
{
	AddItem(_T(""), pImg, pWnd, false, pData);
}
void CLKTabEx::AddItem(CString strText, CLKImage *pImg, CWnd *pWnd, bool bMustFree, void *pData)
{
	LKTabItem *pItem = new LKTabItem;
	pItem->strText = strText;
	pItem->pImg = pImg;
	pItem->pWnd = pWnd;
	pItem->bMustFree = bMustFree;
    pItem->pData = pData;
	m_arrItems.Add(pItem);
    if(m_bIsAdjustItmeSize)
    {
        ReCalcItemSize();
    }else
    {
        CalcItemSize(pItem);
    }
    if(m_nVisibleBeginIndex < 0)
    {
        m_nVisibleBeginIndex = 0;
    }
    CalcArrVisible();
}

void CLKTabEx::SetItem(int nIndex, CString strText, CLKImage *pImg, CWnd *pWnd, void *pData)
{
    if(nIndex >= 0 && nIndex < m_arrItems.GetCount())
    {
	    LKTabItem *pItem = m_arrItems[nIndex];
	    pItem->strText = strText;
	    pItem->pImg = pImg;
	    pItem->pWnd = pWnd;
	    pItem->bMustFree = false;
        pItem->pData = pData;
    }
}

void CLKTabEx::SetItem(int nIndex, CLKImage *pImg, CWnd *pWnd, void *pData)
{
    SetItem(nIndex, _T(""), pImg, pWnd, pData);
}

// ɾ��һ��(����ʼ������ʼ)
void CLKTabEx::DeleteItem(int nIndex)
{
    if(nIndex >= 0 && nIndex < m_arrItems.GetCount())
    {
        LKTabItem *pItem = m_arrItems[nIndex];
        m_arrItems.RemoveAt(nIndex);
        if(pItem) delete pItem;
        if(nIndex < m_nSelectIndex) m_nSelectIndex--;
        if(m_nSelectIndex > m_arrItems.GetCount() - 1) m_nSelectIndex = m_arrItems.GetCount() - 1;
        if(m_nCurIndex > m_arrItems.GetCount() - 1 - m_nVisibleBeginIndex) m_nCurIndex = m_arrItems.GetCount() - m_nVisibleBeginIndex - 1;
        //m_bInCloseBtn = true;
        CalcArrVisible();
        Invalidate();
		::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTAP_MSG_WPARAM_ITEMCLOSED, (LPARAM)GetSafeHwnd());
    }
}

// ��ȡָ��������
void *CLKTabEx::GetItemData(int nIndex)
{
    for(int i = 0; i < m_arrItems.GetCount(); i++)
    {
        if(nIndex == i) 
        {
            LKTabItem *pItem = m_arrItems[i];
            return pItem->pData;
        }
    }
    return 0;
}

// ��ȡ�ؼ��ɷ�������
CRect CLKTabEx::GetCtrlClientRect()
{
    CRect rt(m_rtClient);
    if(m_pBGImage)
    {
        CRect rtWnd = m_pBGImage->GetFrameRect();
        rt.DeflateRect(rtWnd.left, rtWnd.top, rtWnd.right, rtWnd.bottom);
    }
    return rt;
}


BEGIN_MESSAGE_MAP(CLKTabEx, CLKCtrlBase)
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CLKTabEx ��Ϣ�������
void CLKTabEx::OnMouseLeave()
{
    m_bMouseDown = false;
    if(m_bMustCloseButton)
    {
        m_bInCloseBtn = false;
    }
	CRect rtItem(0, 0, 0, 0);
	if(GetItemRectByIndex(m_nCurIndex, &rtItem))
	{
		InvalidateRect(&rtItem);
	}
	m_nCurIndex = -1;

    if(m_nCurArrIndex != -1)
    {
        m_nCurArrIndex = -1;
        InvalidateRect(&m_rtArr);
    }
	CLKCtrlBase::OnMouseLeave();
}

void CLKTabEx::OnMouseMove(UINT nFlags, CPoint point)
{
	int nIndex = GetItemIndexByPoint(point);
	if(nIndex != m_nCurIndex)
	{
		CRect rtItem(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
			InvalidateRect(&rtItem);
		}
		m_nCurIndex = nIndex;
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
			InvalidateRect(&rtItem);
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
	}else if(m_bMustCloseButton && nIndex >= 0)
    {
        // ������ť
		CRect rtIcon(0, 0, 0, 0);
        if(GetItemRectByIndex(nIndex, &rtIcon))
        {
            //rtIcon.DeflateRect(m_nMargin, m_nMargin, 0, 0);
 		    CLKImage *pImg = m_pCloseImg;
		    if(pImg && pImg->IsValid())
		    {
			    int nImgHeight = pImg->GetHeight() / pImg->GetImageNumber();
                rtIcon.right -= 2;
			    rtIcon.left = rtIcon.right - pImg->GetWidth();
			    rtIcon.top += (rtIcon.Height() - nImgHeight) / 2;
			    rtIcon.bottom = rtIcon.top + nImgHeight;
                bool bOld = m_bInCloseBtn;
                m_bInCloseBtn = rtIcon.PtInRect(point) ? true : false;
                if(bOld != m_bInCloseBtn)
                {
			        InvalidateRect(&rtIcon);
                }
            }
       }
    }else if(nIndex < 0 && !m_bIsAdjustItmeSize)
    {
        CRect rtL(m_rtArr), rtR(m_rtArr);
        rtL.right = rtL.left + m_rtArr.Width() / 2;
        rtR.left = rtL.right;
        if(rtL.PtInRect(point) && m_nCurArrIndex != 0)
        {
            // ˢ�����ͷ
            m_nCurArrIndex = 0;
            InvalidateRect(&m_rtArr);
        }else if(rtR.PtInRect(point) && m_nCurArrIndex != 1)
        {
            // ˢ���Ҽ�ͷ
            m_nCurArrIndex = 1;
            InvalidateRect(&m_rtArr);
        }else if(!m_rtArr.PtInRect(point) && m_nCurArrIndex != -1)
        {
            m_nCurArrIndex = -1;
            InvalidateRect(&m_rtArr);
        }
    }
	CLKCtrlBase::OnMouseMove(nFlags, point);
}


void CLKTabEx::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bMouseDown = true;
    if(m_bInCloseBtn && m_bMustCloseButton)
    {
		CRect rtItem(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
		    if(::SendMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTAP_MSG_WPARAM_ITEMCLOSING, (LPARAM)(m_nCurIndex + m_nVisibleBeginIndex)))
            {
                DeleteItem(m_nCurIndex + m_nVisibleBeginIndex);
				InvalidateRect(&rtItem);
            }
		}
    }else if(m_nCurIndex != m_nSelectIndex - m_nVisibleBeginIndex && m_nCurIndex >= 0)
	{
		CRect rtItem(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nSelectIndex - m_nVisibleBeginIndex, &rtItem))
		{
			InvalidateRect(&rtItem);
		}
		m_nSelectIndex = m_nCurIndex + m_nVisibleBeginIndex;
		rtItem = CRect(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
			InvalidateRect(&rtItem);
		}
		::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTAP_MSG_WPARAM_ITEMCHANGED, (LPARAM)GetSafeHwnd());
	}else if(m_nCurArrIndex >= 0)
    {
        CRect rtL(m_rtArr), rtR(m_rtArr);
        rtL.right = rtL.left + m_rtArr.Width() / 2;
        rtR.left = rtL.right;
        if(rtL.PtInRect(point) && m_bVisibleLArr)
        {
            m_nVisibleBeginIndex--;
            CalcArrVisible();
		    ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTAP_MSG_WPARAM_ITEMLSCROLL, (LPARAM)GetSafeHwnd());
            InvalidateRect(&m_rtArr);
        }else if(rtR.PtInRect(point) && m_bVisibleRArr)
        {
            m_nVisibleBeginIndex++;
            CalcArrVisible();
		    ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTAP_MSG_WPARAM_ITEMRSCROLL, (LPARAM)GetSafeHwnd());
            InvalidateRect(&m_rtArr);
        }
    }
	CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKTabEx::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bMouseDown = false;
    if(m_bMustCloseButton)
    {
        Invalidate();
    }

	if(m_nCurArrIndex >= 0)
    {
		InvalidateRect(&m_rtArr);
    }
	CLKCtrlBase::OnLButtonUp(nFlags, point);
}



// �����ؼ����ʼ��
void CLKTabEx::InitCtrlAfterCreate()
{
	ParseImageToMem();
}

// ˢ��ǰ������
void CLKTabEx::ReSetMem()
{
    ReCalcItemSize();
	ParseImageToMem();
	// ��Բ��
	CRect rc;
	GetWindowRect(&rc);
    GetClientRect(&m_rtClient);
    CalcArrVisible();
    if(!m_bIsAdjustItmeSize)
    {
        // �������Ҽ�ͷ������
        m_rtArr = m_rtClient;
        m_rtArr.right -= m_nMargin;
        m_rtArr.left = m_rtArr.right - m_pLeftArrImg->GetWidth() - m_pRightArrImg->GetWidth();
        m_rtArr.top = m_nMargin + (m_nItemHeight - m_pLeftArrImg->GetHeight() / m_pLeftArrImg->GetImageNumber()) / 2;
        m_rtArr.bottom = m_rtArr.top + m_pLeftArrImg->GetHeight() / m_pLeftArrImg->GetImageNumber();
    }
	CRgn rgn, rgn2;
	rc.MoveToXY(0, 0);
	rgn.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //����һ��Բ�Ǿ���
	CRect rc2(rc);
	rc2.top = rc.bottom - 4;
	rgn2.CreateRectRgn(rc2.left, rc2.top, rc2.right, rc2.bottom);
	rgn.CombineRgn(&rgn, &rgn2, RGN_OR);
	SetWindowRgn(rgn,TRUE); 
    if(m_pBGImage)
    {
        CRect rtOffset = m_pBGImage->GetFrameRect();
        ClipClientRect(rtOffset);
    }
	if(IsWindowVisible())
	{
        Invalidate(false);
	}
}
// ���������ͼƬ���ڴ滺����
void CLKTabEx::ParseImageToMem()
{
	//if(m_pBGImage)
	//{
	//	BGParam prm;
	//	prm.rt = CRect(0, 0, 0, 0);
	//	GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETBGIMG, (LPARAM)&prm);
	//	CRect rt;
	//	GetRectInParent(&rt);
	//	prm.rt.OffsetRect(rt.left, rt.top);

	//	CLKImage *pImg = GetMemImg();
	//	if(pImg) delete pImg;
	//	pImg = new CLKImage(rt.Width(), rt.Height());
 //       AngleOffset angle(2, 0, 2);
 //       m_pBGImage->DrawToImage(pImg, &prm, LKSKINBASE_DRAWBGSTYLE_FRAME, angle);
	//	SetMemImg(pImg);
	//}
}

// ��ʼ��ͼƬ�б�
void CLKTabEx::InitImageList()
{
    // �б���2����ͨ�ñ߿��23���� ����
    m_pBGImage = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_CTRLFRAME1);
    if(m_pBGImage)
    {
        CRect rtWnd = m_pBGImage->GetFrameRect();
        //SetClientOffset(rtWnd);
    }
    // 
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_TAB_BEGIN, LKIMAGELISTINDEX_TAB_END);
	CLKImage *pImage = GetParseImg();
	if(pImage)
	{
		m_nItemWidth = pImage->GetWidth();
        m_nItemHeight = pImage->GetHeight() / pImage->GetImageNumber();
	}
    // �رհ�ťͼƬ
    m_pCloseImg = CLKImageMgr::GetImageS(LKIMAGEINDEX_COMMON_ICONDELETE3_15);
    // ���ͷ��ť
    m_pLeftArrImg = m_arrImage[3];
    // �Ҽ�ͷ��ť
    m_pRightArrImg = m_arrImage[4];

}
//
//// ���ǿͻ�����
//void CLKTabEx::DrawNCArea(CDC *pDC)
//{
//	//CRect rtDest;
//	//GetWindowRect(&rtDest);
//	//rtDest.MoveToXY(0, 0);
//	//if(m_pBGImage)
//	//{
//	//	BGParam prm;
//	//	prm.rt = CRect(0, 0, 0, 0);
//	//	GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETBGIMG, (LPARAM)&prm);
//	//	CRect rt;
//	//	GetRectInParent(&rt);
//	//	prm.rt.OffsetRect(rt.left, rt.top);
//
// //       CRect rtOffset = m_pBGImage->GetFrameRect();
// //       AngleOffset angle(2, 0, 2);
// //       m_pBGImage->DrawToDC(pDC, rtDest, rtOffset, 0, LKSKINBASE_DRAWBGSTYLE_FRAME, &prm, angle);
//	//}
//	if(GetParseImg())
//	{
//  //      CLKImage *pImg = GetMemImg();
//		//if(pImg) 
//  //      {
//		//    CRect rtDest;
//		//    GetWindowRect(&rtDest);
//  //          rtDest.MoveToXY(0, 0);
//  //          pImg->DrawToDC(pDC, rtDest, 0);
//  //      }
//	}
//}

// �ڻ����ϻ�ͼ
void CLKTabEx::OnPaint(CDC *pDC)
{
	CRect rtDest, rtWnd;
	GetWindowRect(&rtWnd);
	rtWnd.MoveToXY(0, 0);
    rtDest = rtWnd;
	if(m_pBGImage)
	{
        CRect rtOffset = m_pBGImage->GetFrameRect();
		pDC->ExcludeClipRect(rtOffset.left, rtOffset.top, rtDest.Width() - rtOffset.right, rtDest.Height() - rtOffset.bottom);
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETBGIMG, (LPARAM)&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);

        AngleOffset angle(2, 0, 2);
        m_pBGImage->DrawToDC(pDC, rtDest, rtOffset, 0, LKSKINBASE_DRAWBGSTYLE_FRAME, &prm, angle);
	}

    // ��ǰ��
    CLKImage *pImg = GetParseImg();
    if(!pImg) return;
    rtDest.DeflateRect(m_nMargin, m_nMargin, 0, 0);
	rtDest.left = m_nMargin;
    rtDest.right = rtDest.left;
    int nOffset(0);
    bool bDrawArr(false);
    for(int i = 0; i < m_arrItems.GetCount() - m_nVisibleBeginIndex; i++)
	{
        rtDest.left += nOffset;
		rtDest.right = rtDest.left + m_arrItems[i + m_nVisibleBeginIndex]->nWidth;
        int nRight = (m_bVisibleLArr || m_bVisibleRArr) ? m_rtArr.left : m_rtClient.right; 
        if(rtDest.right > nRight - m_nMargin && !m_bIsAdjustItmeSize) 
        {
            break;
        }
        nOffset = rtDest.Width() + m_nItemSplit;
        rtDest.bottom = rtDest.top + m_nItemHeight;
		int nIndex = (m_nSelectIndex - m_nVisibleBeginIndex == i ? 2 : (m_nCurIndex == i ? 0 : 1));
        pImg->DrawToDC(pDC, rtDest, nIndex, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);

        // �Ȼ��رհ�ť(���Ҿ���)
        CRect rtClose(rtDest);
        
        if(m_bMustCloseButton)
        {
			int nImgHeight = m_pCloseImg->GetHeight() / m_pCloseImg->GetImageNumber();
            rtClose.right = rtDest.right - 2;
			rtClose.left = rtClose.right - m_pCloseImg->GetWidth();
			rtClose.top = rtDest.top + (rtDest.Height() - nImgHeight) / 2;
			rtClose.bottom = rtClose.top + nImgHeight;
            int nIconIndex = 0;
            if(i == m_nCurIndex)
            {
                if(m_bInCloseBtn)
                {
                    nIconIndex = m_bMouseDown ? 2 : 1;
                    //TRACE(L"m_bInCloseBtn:%d\r\n", m_bInCloseBtn);
                }
            }
			m_pCloseImg->DrawToDC(pDC, rtClose, nIconIndex);
        }else{rtClose.left = rtClose.right;}
		// ��ͼ��
		CRect rtIcon(rtDest);
		CLKImage *pImg = m_arrItems[i + m_nVisibleBeginIndex]->pImg;
		if(pImg && pImg->IsValid())
		{
			int nImgHeight = pImg->GetHeight() / pImg->GetImageNumber();
			rtIcon.left += 2;
            rtIcon.right = rtIcon.left + pImg->GetWidth();
			rtIcon.top = rtDest.top + (rtDest.Height() - nImgHeight) / 2;
			rtIcon.bottom = rtIcon.top + nImgHeight;
			pImg->DrawToDC(pDC, rtIcon, 0);
		}else{ rtIcon.right = rtIcon.left; }
        CRect rtText(rtDest);
        rtText.left = rtIcon.right;
        rtText.right = rtClose.left;
        if(!m_arrItems[i + m_nVisibleBeginIndex]->strText.IsEmpty())
		{			
			// ���ı�
            rtText.left += 2;
            rtText.right -= 2;
            if(rtText.Width() >= 6)
            {
			    CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST90());
                CString strText(m_arrItems[i + m_nVisibleBeginIndex]->strText);
                CRect rtText2(0, 0, 0, 0);
                pDC->DrawText(strText, strText.GetLength(), &rtText2, DT_CALCRECT);
                if(rtText2.Width() > rtText.Width())
                {
                    strText = strText.Left(8);
                    strText += _T("...");
                }
			    pDC->SetBkMode(TRANSPARENT);
			    pDC->SetTextColor(m_crText);
			    pDC->DrawText(strText, rtText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			    pDC->SelectObject(pOld);
            }
		}
	}

    // �����Ҽ�ͷ
    if(!m_bIsAdjustItmeSize)
    {
		// �����ͷ
		CRect rtIcon(m_rtArr);
        rtIcon.right = rtIcon.left + rtIcon.Width() / 2;
		int nLIndex = (m_bMouseDown ? (m_nCurArrIndex == 0 ? 2 : 0) : (m_nCurArrIndex == 0 ? 1 : 0));
		int nRIndex = (m_bMouseDown ? (m_nCurArrIndex == 1 ? 2 : 0) : (m_nCurArrIndex == 1 ? 1 : 0));
		if(m_pLeftArrImg && m_pLeftArrImg->IsValid() && m_bVisibleLArr)
		{
            //m_bVisibleLArr = true;
			m_pLeftArrImg->DrawToDC(pDC, rtIcon, nLIndex);
		}else m_bVisibleLArr = false;
        rtIcon.left = rtIcon.right;
        rtIcon.right = m_rtArr.right;
		if(m_pRightArrImg && m_pRightArrImg->IsValid() && m_bVisibleRArr)
		{
            //m_bVisibleRArr = true;
			m_pRightArrImg->DrawToDC(pDC, rtIcon, nRIndex);
		}else m_bVisibleRArr = false;
    }
}

// ��ǰ������ָ����CDC������
void CLKTabEx::OnDrawFGToDC(BGParam *prm)
{
}

// �������ָ���λ�û�ȡ�������������������ʼ�ɼ�����
int CLKTabEx::GetItemIndexByPoint(CPoint point)
{
    int nWidth(m_nMargin);
    for(int i = m_nVisibleBeginIndex; i < m_arrItems.GetCount(); i++)
    {
        if(nWidth + m_arrItems[i]->nWidth > m_rtArr.left && !m_bIsAdjustItmeSize) break;
        if(point.x >= nWidth && point.x <= nWidth + m_arrItems[i]->nWidth)
        {
            return i - m_nVisibleBeginIndex;
        }
        nWidth += m_arrItems[i]->nWidth + m_nItemSplit;
        
        if(nWidth >= m_rtClient.Width()) break;        
    }
    return -1;


	//int m = (point.x - m_nMargin + m_nItemSplit) % (m_nItemWidth + m_nItemSplit);
	//if(m > 0 && m <= m_nItemWidth)
	//{
	//	int n = (point.x - m_nMargin + m_nItemSplit) / (m_nItemWidth + m_nItemSplit);
	//	if(n < m_arrItems.GetCount())
	//	{
	//		return n;
	//	}
	//}
	//return -1;
}

// ���ݿɼ�������ȡָ���������
bool CLKTabEx::GetItemRectByIndex(int nIndex, LPRECT lpRect)
{
    *lpRect = m_rtClient;
    lpRect->top += m_nMargin;
    lpRect->bottom = lpRect->top + m_nItemHeight;
    lpRect->left = m_nMargin;
    lpRect->right = lpRect->left;
    for(int i = 0; i <= nIndex; i++)
    {
        lpRect->left = lpRect->right;
        lpRect->right = lpRect->left + m_arrItems[i + m_nVisibleBeginIndex]->nWidth;
        if(nIndex == i)
        {
            return true;
        }
        lpRect->right += m_nItemSplit;
    }

	//if(nIndex >= 0 && nIndex < m_arrItems.GetCount())
	//{
	//	*lpRect = m_rtClient;
	//	lpRect->left = m_nMargin + (m_nItemWidth + m_nItemSplit) * nIndex;
	//	lpRect->right = lpRect->left + m_nItemWidth;
	//	return true;
	//}
	return false;
}


// ���¼������С
void CLKTabEx::ReCalcItemSize()
{
    if(m_bIsAdjustItmeSize && GetSafeHwnd())
    {
        // ƽ��������
        m_nItemWidth = (m_rtClient.Width() - 2 * m_nMargin + m_nItemSplit) / m_arrItems.GetCount() - m_nItemSplit;
        for(int i = 0; i < m_arrItems.GetCount(); i++)
        {
            m_arrItems[i]->nWidth = m_nItemWidth;
        }
    }
}

// ����ָ�����С
void CLKTabEx::CalcItemSize(LKTabItem *pItem)
{
    // ���ַ���ȣ����ó��������
    pItem->nWidth = 60;
    if(!pItem->strText.IsEmpty())
    {
        pItem->nWidth = GetStringWidth(pItem->strText, CLKFontMgr::GetST90());
    }
    // ������ڹرհ�ť 
    if(m_bMustCloseButton)
    {
        pItem->nWidth += m_pCloseImg->GetWidth();
        if(!pItem->strText.IsEmpty())
        {
            // �ַ�����ͼ��֮����2�Ŀհ�
            pItem->nWidth += 2;
        }
    }
    // �������ͼ��
    if(pItem->pImg)
    {
        pItem->nWidth += pItem->pImg->GetWidth();
        if(!pItem->strText.IsEmpty())
        {
            // �ַ�����ͼ��֮����2�Ŀհ�
            pItem->nWidth += 2;
        }
    }
    // ���Ҹ���2�Ŀհ�
    pItem->nWidth += 4;

    // ���ó��������
    pItem->nWidth = pItem->nWidth > const_max_tabitem_width ? const_max_tabitem_width : pItem->nWidth;
}

// �����ͷ�Ŀɼ���
void CLKTabEx::CalcArrVisible()
{
    if(m_nVisibleBeginIndex < 0)
    {
        return;
    }
    CRect rt = m_rtClient;
    rt.top += m_nMargin;
    rt.bottom = rt.top + m_nItemHeight;
    rt.left = m_nMargin;
    rt.right = rt.left;
    m_bVisibleRArr = false;
    for(int i = m_nVisibleBeginIndex; i < m_arrItems.GetCount(); i++)
    {
        rt.left = rt.right;
        rt.right = rt.left + m_arrItems[i]->nWidth;
        if(rt.right > m_rtClient.right - m_nMargin)
        {
            m_bVisibleRArr = true;
            break;
        }
        rt.right += m_nItemSplit;
    }

    if(m_nVisibleBeginIndex > 0)
    {
        m_bVisibleLArr = true;
    }else
    {
        m_bVisibleLArr = false;
    }

}
