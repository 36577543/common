// ..\..\..\common\LKComponent\GDI\LKContainer.cpp : ʵ���ļ�
//

#include "LKContainer.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// CLKContainer


IMPLEMENT_DYNAMIC(CLKContainer, CLKCtrlBase)

CLKContainer::CLKContainer(int nImageIndex)
: CLKCtrlBaseEx(nImageIndex)
, m_emsCommon(esb_nohere)
{

}

CLKContainer::~CLKContainer()
{
	if (m_pDialog != NULL) {
		if (m_pDialog->GetSafeHwnd() != NULL) {
			m_pDialog->DestroyWindow();
		}
		delete m_pDialog;
		m_pDialog = NULL;
	}
	if (m_pNormalImg) delete m_pNormalImg;
	DeleteAllItem();
}
// ɾ�����пؼ�����
void CLKContainer::DeleteAllItem()
{
	for (list<CLKContainerItem *>::iterator it = m_ltCtrl.begin(); it != m_ltCtrl.end(); it++)
	{
		CLKContainerItem * pItem = *it;
		delete pItem;
	}
	m_ltCurCtrl.clear();
	m_ltCtrl.clear();
	// �ܵ�ҳ��
	m_nPageCount = 0;
	// ��ǰҳ������
	m_nCurPageIndex = 0;
	// ��ǰѡ�е���
	m_pCurItem = 0;
}

BOOL CLKContainer::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBaseEx::Create(_T(""), WS_CHILD | WS_VISIBLE | /**/WS_CLIPCHILDREN, rect, pParentWnd, nID);
	m_pDialog = new CDynDialogEx(this);		//create the dynamic dialog, using this as parent window
	m_pDialog->SetUseSystemButtons(FALSE);
	m_pDialog->SetUseModeless();
	m_pDialog->DoModal();
	m_pDialog->SetWindowPos(0, -m_nItemWidth, -m_nItemHeight, m_nItemWidth, m_nItemHeight, SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOACTIVATE/* | SWP_NOMOVE*/);
	return bRet;
}
BOOL CLKContainer::Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	int nItemDlgID = 200;
	BOOL bRet = CLKCtrlBaseEx::Create(strCaption, WS_CHILD | WS_VISIBLE | /**/WS_CLIPCHILDREN, rect, pParentWnd, nID);
	m_pDialog = new CDynDialogEx(this);		//create the dynamic dialog, using this as parent window
	m_pDialog->SetWidthAndHeight(m_nItemWidth, m_nItemHeight);
	m_pDialog->SetUseSystemButtons(FALSE);
	m_pDialog->SetUseModeless();
	m_pDialog->DoModal();
	m_pDialog->SetWindowPos(0, -m_nItemWidth, -m_nItemHeight, m_nItemWidth, m_nItemHeight, SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOACTIVATE/* | SWP_NOMOVE*/);
	return bRet;
}

// ���ؽ����
void CLKContainer::HideDlg()
{
	if (m_pDialog)
	{
		m_pDialog->SetWindowPos(0, -m_nItemWidth, -m_nItemHeight, m_nItemWidth, m_nItemHeight, SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOACTIVATE/* | SWP_NOMOVE*/);
	}
}

// ����һ���ؼ�����
bool CLKContainer::InsertItem(int nId, CString strItem, CString strFlag, CLKImage *pImg, void *pData, bool bCalcVisible)
{
	CLKContainerItem *pItem = new CLKContainerItem;
	pItem->nId = nId;
	pItem->pImg = pImg;
	pItem->strText = strItem;
	pItem->strFlag = strFlag;
	pItem->pData = pData;
	m_ltCtrl.push_back(pItem);
	//m_ltCurCtrl.push_back(pItem);
	if (bCalcVisible)
	{
		CalcVisibleCtrl();
	}
	return true;
}

BEGIN_MESSAGE_MAP(CLKContainer, CLKCtrlBaseEx)
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



// CLKContainer ��Ϣ�������
LRESULT CLKContainer::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if (LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR == wParam)
	{
		//// ����
		//LKSCROLLINFO *pInfo = (LKSCROLLINFO *)lParam;
		//// ������Ϣ
		//m_nVisibleItemBeginIndex = pInfo->nPos;
		//m_pVisibleItemBeginItem = GetItemByIndex(m_nVisibleItemBeginIndex);
		//Invalidate();
	}
	return CLKCtrlBaseEx::OnCommonMsg(wParam, lParam);
}

// �����ؼ����ʼ��
void CLKContainer::InitCtrlAfterCreate()
{
	ParseImageToMem();
	CLKCtrlBaseEx::InitCtrlAfterCreate();
}

// ˢ��ǰ������
void CLKContainer::ReSetMem()
{
	// ����ɼ��ؼ��б�
	CalcVisibleCtrl();
	ParseImageToMem();

	if(IsWindowVisible())
	{
		Invalidate();
	}
	// �����ӿؼ�
	CWnd *pWnd = GetWindow(GW_CHILD);
	while(pWnd)
	{
		pWnd->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_RESETMEM);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

// ���������ͼƬ���ڴ滺����
void CLKContainer::ParseImageToMem()
{
	//m_bImgIsOwner = true;
	// ��ȡ��Ҫ������ͼƬ
	CLKImage *pImg = GetParseImg();
	if (pImg)
	{
		// 
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParentImage(&prm);
		//GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);
		int nNumber = pImg->GetImageNumber();
		if (nNumber > 0)
		{
			CRect rtWnd;
			GetWindowRect(&rtWnd);
			rtWnd.MoveToXY(0, 0);
			if (m_pNormalImg) delete m_pNormalImg;
			m_pNormalImg = new CLKImage(rtWnd.Width(), rtWnd.Height());
			AngleOffset angleOffset(4, 0, 4);
			pImg->DrawToImage(m_pNormalImg, &prm, LKSKINBASE_DRAWBGSTYLE_FRAME, angleOffset);
			CRect rtDest(rtWnd);
			rtDest.DeflateRect(m_rtClientOffset.left, m_rtClientOffset.top, m_rtClientOffset.right, m_rtClientOffset.bottom);

			BGParam pM;
			pM.rt = CRect(0, 0, 0, 0);
			GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&pM);
			pM.rt.OffsetRect(rt.left + m_rtClientOffset.left, rt.top + m_rtClientOffset.top);
			CLKImage *pI = (CLKImage *)pM.pBG;
			if (pI)
			{
				pM.rt.right = pM.rt.left + rtDest.Width();
				pM.rt.bottom = pM.rt.top + rtDest.Height();
				pI->DrawToImage(m_pNormalImg, rtDest, pM.rt);
			}
			// ������
			CString strText;
			GetWindowText(strText);
			if (!strText.IsEmpty())
			{
				CRect rtText(0, 0, rtWnd.Width(), m_rtClientOffset.top);
				m_pNormalImg->DrawText(strText, rtText, CLKFontMgr::GetSTB90());
			}
			SetMemImg(m_pNormalImg);
		}
	}
}

// ��ʼ��ͼƬ�б� 
void CLKContainer::InitImageList()
{
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_COMMON_BEGIN + 1, LKIMAGELISTINDEX_COMMON_BEGIN + 1);
	m_pItemStateImag = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_SKINFRAME);
	m_pItemLeftArrStateImag = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_TABEXLEFTARR);
	m_pItemRightArrStateImag = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_TABEXRIGHTARR);
	//CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_TREECTRL_BEGIN, LKIMAGELISTINDEX_TREECTRL_END);
	//m_arrImage[3] = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_ICONBUSINESS);

	CLKImage *pImg = GetParseImg();
	if (pImg)
	{
		m_rtClientOffset = pImg->GetFrameRect();
	}
	SetScrollbarImg(CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR), 0);
}

int CLKContainer::OnPrint(WPARAM wParam, LPARAM lParam)
{
	BGParam rm;
	GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&rm);
	CRect rt;
	GetRectInParent(&rt);
	rm.rt.OffsetRect(rt.left, rt.top);
	CDC *pDC = CDC::FromHandle((HDC)wParam);
	CLKImage *pImg = (CLKImage *)rm.pBG;
	pImg->Draw(pDC->GetSafeHdc(), 0, 0, rt.Width(), rt.Height(), rm.rt.left, rm.rt.top, rt.Width(), rt.Height());
	if (m_rtClientOffset.IsRectNull())
	{
		OnPaintEx(0, pDC);
	}
	else
	{
		DrawNCArea(pDC);
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDC, m_rtClient.Width(), m_rtClient.Height());
		CBitmap *pOldBmp = memDC.SelectObject(&bmp);
		memDC.BitBlt(0, 0, m_rtClient.Width(), m_rtClient.Height(), pDC, m_rtClientOffset.left, m_rtClientOffset.top, SRCCOPY);
		OnPaintEx(0, &memDC);
		pDC->BitBlt(m_rtClientOffset.left, m_rtClientOffset.top, m_rtClient.Width(), m_rtClient.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBmp);
		bmp.DeleteObject();
		memDC.DeleteDC();
	}
	return 0;
}

// �ڻ����ϻ�ͼ
void CLKContainer::OnPaint(CDC *pDC)
{
	int nIndex = 0;
	CRect rt(m_rtClient);
	rt.DeflateRect(m_rtSubItemsOffest);
	CRect rtCtrl(rt.left, rt.top, rt.left + m_nItemWidth, rt.top + m_nItemHeight);
	for (list<CLKContainerItem *>::iterator it = m_ltCurCtrl.begin(); it != m_ltCurCtrl.end(); it++)
	{
		CLKContainerItem *pItem = *it;
		DrawItem(pDC, pItem, rtCtrl);
		//pItem = GetNextExpandItem(pItem);
		if (rtCtrl.right + m_nItemSplit + m_nItemWidth < rt.right)
		{
			rtCtrl.OffsetRect(m_nItemWidth + m_nItemSplit, 0);
		}
		else if (rtCtrl.bottom + m_nItemSplit + m_nItemHeight < rt.bottom)
		{
			rtCtrl.OffsetRect(0, m_nItemHeight + m_nItemSplit);
			rtCtrl.MoveToX(rt.left);
		}
		else
		{
			break;
		}
	}

	// ����ҳ��ť
	DrawButton(pDC);
}

void DrawMultiLine(CDC *pDC, const CString &csText, CRect rcText, int iLineSpace)
{
	int iLastStartPos = 0;
	int iTextLen = static_cast<int>(csText.GetLength());
	int iLineCount = rcText.Height() / iLineSpace;
	CRect rcSubText(rcText);

	// ����ÿһ���ı�
	for (int iIndexLine = 1; iIndexLine <= iLineCount; ++iIndexLine)
	{
		// �ж��ı��Ƿ��Ѿ�������
		if (iLastStartPos >= iTextLen)
		{
			break;
		}

		rcSubText.top = rcText.top + iLineSpace*(iIndexLine - 1);
		rcSubText.bottom = rcSubText.top + iLineSpace;

		CString csSubText = csText.Mid(iLastStartPos);
		if (iIndexLine != iLineCount)
		{
			if (iIndexLine == 1)
			{
				//rcSubText.left = rcSubText.left + m_iIndentText;
				rcSubText.left = rcSubText.left + 2;
			}

			// ʹ��drawParams��ȡһ�л��Ƶ��ַ���
			DRAWTEXTPARAMS drawParams;
			ZeroMemory(&drawParams, sizeof(DRAWTEXTPARAMS));
			drawParams.cbSize = sizeof(DRAWTEXTPARAMS);
			//pDC->DrawTextEx(csSubText.GetBuffer(), static_cast<int>(csSubText.GetLength()), rcSubText, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK | DT_EXTERNALLEADING | DT_NOPREFIX, &drawParams);
			pDC->DrawTextEx(csSubText.GetBuffer(), static_cast<int>(csSubText.GetLength()), rcSubText, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK | DT_EXTERNALLEADING | DT_NOPREFIX, &drawParams);
			csSubText.ReleaseBuffer();

			if (drawParams.uiLengthDrawn > 0)
			{
				// �����Ѿ����Ƶ��ַ�
				iLastStartPos += drawParams.uiLengthDrawn;
			}

			if (iIndexLine == 1)
			{
				//rcSubText.left = rcSubText.left - m_iIndentText;
				rcSubText.left = rcSubText.left - 2;
			}
		}
		else
		{
			// �������һ��
			//pDC->DrawText(csSubText, rcSubText, DT_LEFT | DT_END_ELLIPSIS | DT_EDITCONTROL | DT_EXTERNALLEADING | DT_NOPREFIX);
			pDC->DrawText(csSubText, rcSubText, DT_CENTER | DT_END_ELLIPSIS | DT_EDITCONTROL | DT_EXTERNALLEADING | DT_NOPREFIX);
		}
	}
}

// ��������
void CLKContainer::DrawItem(CDC *pDC, PLKContainerItem pItem, CRect rtCtrl)
{
	if (pItem->pImg && pItem->pImg->IsValid())
	{
		int nWidth = pItem->pImg->GetWidth() > m_nItemWidth - 4 ? m_nItemWidth - 4 : pItem->pImg->GetWidth();
		int nHeight = pItem->pImg->GetHeight() > m_nItemHeight - 4 ? m_nItemHeight - 4 : pItem->pImg->GetHeight();
		int nLeft = rtCtrl.left + (rtCtrl.Width() - nWidth) / 2;
		int nTop = rtCtrl.top + (rtCtrl.Height() - nHeight - 20) / 2;
		CRect r(nLeft, nTop, nLeft + nWidth, nTop + nHeight);

		pItem->pImg->DrawToDC(pDC, r, 0);
	}
	if (pItem->strFlag.Compare(L"1") == 0)
	{
		CRect rtWnd(0, 0, m_nItemWidth, m_nItemHeight);
		rtWnd = rtCtrl;
		CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_FOCUS);
		if (pImg)
		{
			AngleOffset angleOffset(0);
			pImg->DrawToDC(pDC, rtWnd, 0, LKSKINBASE_DRAWBGSTYLE_ALL, 0, angleOffset);
		}
	}
	if (!pItem->strText.IsEmpty())
	{
		rtCtrl.bottom -= 2;
		rtCtrl.top = rtCtrl.bottom - 20;
		rtCtrl.left += 2;
		rtCtrl.right -= 2;
		//rtCtrl.top += m_nItemWidth;
		//rtCtrl.left -= 5;
		//rtCtrl.right += 5;
		pDC->SetBkMode(TRANSPARENT);
		if (!m_pFtText)
		{
			m_pFtText = CLKFontMgr::GetST90();
		}
		CFont *pOldFont = pDC->SelectObject(m_pFtText);
		pDC->SetTextColor(m_crText);
		//pDC->DrawText(pItem->strText, &rtCtrl, DT_CENTER | DT_VCENTER /*| DT_SINGLELINE*/);
		DrawMultiLine(pDC, pItem->strText, rtCtrl, 20);
		pDC->SelectObject(pOldFont);
	}
}

// ����ҳ��ť
void CLKContainer::DrawButton(CDC *pDC)
{
	if (m_ltCurCtrl.empty()) return;
	CRect rt(m_rtClient);
	rt.top = m_rtClient.bottom - m_rtSubItemsOffest.bottom;
	CRect rtText(rt);

	CRect rtArr(rt);
	rtArr.top += 2;
	rtArr.left += (rtArr.Width() / 2 - 30);
	// ���ͷ״̬ͼƬ
	if (m_pItemLeftArrStateImag)
	{
		rtText.left = rtArr.left + m_pItemLeftArrStateImag->GetWidth();
		m_pItemLeftArrStateImag->DrawToDC(pDC, rtArr.left, rtArr.top, m_nLeftArrState == 0 ? 0 : (m_emsCommon != esb_down ? m_nLeftArrState : m_nLeftArrState + 1));
	}

	// �Ҽ�ͷ״̬ͼƬ
	if (m_pItemRightArrStateImag)
	{
		rtArr.MoveToX(rtArr.left + 60 - m_pItemRightArrStateImag->GetWidth());
		rtText.right = rtArr.left;
		m_pItemRightArrStateImag->DrawToDC(pDC, rtArr.left, rtArr.top, m_nRightArrState == 0 ? 0 : (m_emsCommon != esb_down ? m_nRightArrState : m_nRightArrState + 1));
	}

	// ҳ��
	CString strText;
	strText.Format(L"%d/%d", m_nCurPageIndex + 1, m_nPageCount);
	pDC->DrawText(strText, &rtText, DT_CENTER | DT_TOP | DT_SINGLELINE/**/);
}

void CLKContainer::OnMouseLeave()
{
	if (m_emsCommon != esb_nohere)
	{
		m_emsCommon = esb_nohere;
		ProcPageBtnState(CPoint(0,0));
		Invalidate();
	}
	CLKCtrlBase::OnMouseLeave();
}

// ����������Ǹ�������
bool CLKContainer::GetRectByPoint(CRect &r, CPoint &point, int &nIndex)
{
	bool b(false);
	CRect rt(m_rtClient);
	rt.DeflateRect(m_rtSubItemsOffest);
	if (!rt.PtInRect(point))
	{
		return false;
	}
	CRect rtCtrl(rt.left, rt.top, rt.left + m_nItemWidth, rt.top + m_nItemHeight);
	int i = 0;
	for (list<CLKContainerItem *>::iterator it = m_ltCurCtrl.begin(); it != m_ltCurCtrl.end(); it++,i++)
	{
		CLKContainerItem *pItem = *it;
		if (rtCtrl.PtInRect(point))
		{
			m_pCurItem = pItem;
			nIndex = i;
			r = rtCtrl;
			b = true;
			break;
		}
		if (rtCtrl.right + m_nItemSplit + m_nItemWidth < rt.right)
		{
			rtCtrl.OffsetRect(m_nItemWidth + m_nItemSplit, 0);
		}
		else if (rtCtrl.bottom + m_nItemSplit + m_nItemHeight < rt.bottom)
		{
			rtCtrl.OffsetRect(0, m_nItemHeight + m_nItemSplit);
			rtCtrl.MoveToX(rt.left);
		}
		else
		{
			break;
		}
	}
	return b;
}

// ��ҳ��ť״̬�Ƿ�ı�
bool CLKContainer::IsPageBtnStateChange(CPoint &point)
{
	bool b(false);
	CRect rt(m_rtClient);
	rt.top = m_rtClient.bottom - m_rtSubItemsOffest.bottom;

	CRect rtArr(rt);
	rtArr.top += 2;
	rtArr.left += (rtArr.Width() / 2 - 30);
	// ���ͷ״̬ͼƬ
	if (m_pItemLeftArrStateImag)
	{
		rtArr.bottom = rtArr.top + m_pItemLeftArrStateImag->GetHeight();
		rtArr.right = rtArr.left + m_pItemLeftArrStateImag->GetWidth();
		if (rtArr.PtInRect(point))
		{
			if (m_nLeftArrState == 0)
			{
				return true;
			}
		}
		else
		{
			if (m_nLeftArrState != 0)
			{
				return true;
			}
		}
	}

	// �Ҽ�ͷ״̬ͼƬ
	if (m_pItemRightArrStateImag)
	{
		rtArr.MoveToX(rtArr.left + 60 - m_pItemRightArrStateImag->GetWidth());
		rtArr.bottom = rtArr.top + m_pItemRightArrStateImag->GetHeight();
		rtArr.right = rtArr.left + m_pItemRightArrStateImag->GetWidth();
		if (rtArr.PtInRect(point))
		{
			if (m_nRightArrState == 0)
			{
				return true;
			}
		}
		else
		{
			if (m_nRightArrState != 0)
			{
				return true;
			}
		}
	}
	return false;
}

// ��������ҳ��ť��
bool CLKContainer::PointInLeftPageBtn(CPoint &point)
{
	CRect rt(m_rtClient);
	rt.top = m_rtClient.bottom - m_rtSubItemsOffest.bottom;

	CRect rtArr(rt);
	rtArr.top += 2;
	rtArr.left += (rtArr.Width() / 2 - 30);
	// ���ͷ״̬ͼƬ
	if (m_pItemLeftArrStateImag)
	{
		rtArr.bottom = rtArr.top + m_pItemLeftArrStateImag->GetHeight();
		rtArr.right = rtArr.left + m_pItemLeftArrStateImag->GetWidth();
		if (rtArr.PtInRect(point))
		{
			return true;
		}
	}
	return false;
}
// ������ҷ�ҳ��ť��
bool CLKContainer::PointInRightPageBtn(CPoint &point)
{
	CRect rt(m_rtClient);
	rt.top = m_rtClient.bottom - m_rtSubItemsOffest.bottom;

	CRect rtArr(rt);
	rtArr.top += 2;
	rtArr.left += (rtArr.Width() / 2 + 30);
	// �Ҽ�ͷ״̬ͼƬ
	if (m_pItemRightArrStateImag)
	{
		rtArr.left -= m_pItemRightArrStateImag->GetWidth();
		rtArr.bottom = rtArr.top + m_pItemRightArrStateImag->GetHeight();
		rtArr.right = rtArr.left + m_pItemRightArrStateImag->GetWidth();
		if (rtArr.PtInRect(point))
		{
			return true;
		}
	}
	return false;
}

// ����ɼ��ؼ��б�
void CLKContainer::CalcVisibleCtrl()
{
	CRect rt(m_rtClient);
	rt.DeflateRect(m_rtSubItemsOffest);
	CRect rtCtrl(rt.left, rt.top, rt.left + m_nItemWidth, rt.top + m_nItemHeight);
	int nCount = 0;
	int nRow = 0;
	int nCol = 1;
	// ����ɼ��ؼ�������i
	for (; 1; nCount++)
	{
		if (rtCtrl.right + m_nItemSplit + m_nItemWidth < rt.right)
		{
			rtCtrl.OffsetRect(m_nItemWidth + m_nItemSplit, 0);
			if (nRow == 0)
			{
				nCol++;
			}
		}
		else if (rtCtrl.bottom + m_nItemSplit + m_nItemHeight < rt.bottom)
		{
			rtCtrl.OffsetRect(0, m_nItemHeight + m_nItemSplit);
			rtCtrl.MoveToX(rt.left);
			nRow++;
		}
		else
		{
			break;
		}
	}
	if (nCount > 0)
	{
		m_nPageCount = m_ltCtrl.size() / nCount + (m_ltCtrl.size() % nCount ? 1 : 0);
	}
	m_ltCurCtrl.clear();
	int i = 0;
	for (list<CLKContainerItem *>::iterator it = m_ltCtrl.begin(); it != m_ltCtrl.end(); it++, i++)
	{
		CLKContainerItem *pItem = *it;
		if ((i >= nCount * m_nCurPageIndex) && (i <= nCount + nCount * m_nCurPageIndex))
		{
			m_ltCurCtrl.push_back(pItem);
		}
	}

	nRow = m_ltCtrl.size() / nCol + (m_ltCtrl.size() % nCol ? 1 : 0);
	int nHeight = nRow * (m_nItemHeight + m_nItemSplit) - m_nItemSplit;
	if (nRow > 0)
	{
		SetVScrollbarRang(nRow, nHeight, true);
		SetVScrollbarVisible();
	}
}

void CLKContainer::ProcPageBtnState(CPoint point)
{
	if (IsPageBtnStateChange(point))
	{
		if (PointInLeftPageBtn(point))
		{
			if (m_nLeftArrState != 1)
			{
				m_nLeftArrState = 1;
			}
		}
		else
		{
			if (m_nLeftArrState != 0)
			{
				m_nLeftArrState = 0;
			}
		}

		if (PointInRightPageBtn(point))
		{
			if (m_nRightArrState != 1)
			{
				m_nRightArrState = 1;
			}
		}
		else
		{
			if (m_nRightArrState != 0)
			{
				m_nRightArrState = 0;
			}
		}
		// ����ڷ�ҳ��ť��
		Invalidate();
	}
}

// ��ǰ��ҳ
void CLKContainer::PagePrev()
{
	if (m_nCurPageIndex > 0)
	{
		m_nCurPageIndex--;
		CalcVisibleCtrl();
	}
}
// ���ҳ
void CLKContainer::PageNext()
{
	if (m_nCurPageIndex < m_nPageCount - 1)
	{
		m_nCurPageIndex++;
		CalcVisibleCtrl();
	}
}

void CLKContainer::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_emsCommon == esb_nohere)
	{
		m_emsCommon = esb_hover;
		TRACKMOUSEEVENT   tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = this->GetSafeHwnd();
		tme.dwHoverTime = HOVER_DEFAULT; //ֻ��HOVER��Ч
		::TrackMouseEvent(&tme);
	}
	// ����������Ǹ�������
	CRect rt(0, 0, 0, 0);
	int nIndex = -1;
	if (GetRectByPoint(rt, point, nIndex))
	{
		if (nIndex != m_nCurItemIndex)
		{
			m_nCurItemIndex = nIndex;
			// ������
			ClientToScreen(&rt);
			m_pDialog->SetWindowPos(0, rt.left, rt.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
			//m_pDialog->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_pCurItem = 0;
		m_nCurItemIndex = -1;
		// ���ظ���
		m_pDialog->ShowWindow(SW_HIDE);
		ProcPageBtnState(point);
	}
	CLKCtrlBase::OnMouseMove(nFlags, point);
}


void CLKContainer::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_emsCommon != esb_down)
	{
		m_emsCommon = esb_down;
		if (m_pCurItem)
		{
			//::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKCONTAINER_MSG_WPARAM_ITEMBTNCLICKED, (LPARAM)m_pCurItem->pData);
		}
		else
		{
			ProcPageBtnState(point);
			Invalidate();
		}
	}
	CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKContainer::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_emsCommon == esb_down)
	{
		if (m_pCurItem)
		{
			::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKCONTAINER_MSG_WPARAM_ITEMBTNCLICKED, (LPARAM)m_pCurItem->pData);
		}
		//InitLClick();
		//::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	}
	if (m_emsCommon != esb_hover)
	{
		m_emsCommon = esb_hover;
		// ��ֹ˫�����뿪��Ϣ�ò�������
		TRACKMOUSEEVENT   tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = this->GetSafeHwnd();
		tme.dwHoverTime = HOVER_DEFAULT; //ֻ��HOVER��Ч
		::TrackMouseEvent(&tme);
		ProcPageBtnState(point);
		if (PointInLeftPageBtn(point))
		{
			PagePrev();
		}
		else if (PointInRightPageBtn(point))
		{
			PageNext();
		}
		Invalidate();
	}
	CLKCtrlBase::OnLButtonUp(nFlags, point);
}


void CLKContainer::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CLKCtrlBaseEx::OnShowWindow(bShow, nStatus);

	// TODO:  �ڴ˴������Ϣ����������
}
