// ..\..\common\LKComponent\GDI\LKTreeCtrl.cpp : ʵ���ļ�
//

#include "LKTreeCtrl.h"
#include "LKImageMgr.h"


// ��ʾ��Ϣ������ʱ����ʶ
#define LKTREECTRL_TIMERID_HINT     1000

// _LKSkinTreeCtrlItem
_LKSkinTreeCtrlItem::_LKSkinTreeCtrlItem()
{
    m_nCurIndex     = -1;
    m_bMouseDown    = false;
	nExpandIndex	= -1;
	bExpand			= false;
	pParentItem		= 0;
	pNextItem		= 0;
	pData			= 0;
}
_LKSkinTreeCtrlItem::~_LKSkinTreeCtrlItem()
{
    for(int i = 0; i < arrBtn.GetCount(); i++)
    {
        delete arrBtn[i];
    }
    arrBtn.RemoveAll();
}

// ��ȡ��Ĳ��//6225 7500 0348 4017
int _LKSkinTreeCtrlItem::GetLevel()
{
    int nLevel(0);
    PLKTREEITEM pItem(pParentItem);
    while(pItem)
    {
        nLevel++;
        pItem = pItem->pParentItem;
    }
    return nLevel;
}

// ��ȡָ����(����)
_LKSkinTreeCtrlItem *_LKSkinTreeCtrlItem::GetItemByData(void *pDataA)
{
	_LKSkinTreeCtrlItem *pItem(0);
	if(pData == pDataA) return this;
	for(int i = 0; i < arrItem.GetCount(); i++)
	{
		pItem = arrItem[i]->GetItemByData(pDataA);
		if(pItem)
		{
			return pItem;
		}
	}
	return 0;
}

// ���½ڵ����һ���ڵ�ֵ
void _LKSkinTreeCtrlItem::SetNextItem(_LKSkinTreeCtrlItem *pNextItemA)
{
	pNextItem = pNextItemA;
	int nCount = arrItem.GetCount();
	if(nCount > 0)
	{
		arrItem[nCount - 1]->SetNextItem(pNextItemA);
	}
}

// ����״̬
bool _LKSkinTreeCtrlItem::SetStateByData(void *pDataA, int nStateIndex, int nImage, int nState, int nStateCount)
{
	bool bRet(false);
	_LKSkinTreeCtrlItem *pItem(0);
	if(pData == pDataA) 
	{
		SetState(bRet, nStateIndex, nImage, nState, nStateCount);
	}
	for(int i = 0; i < arrItem.GetCount(); i++)
	{
		if(arrItem[i]->SetStateByData(pDataA, nStateIndex, nImage, nState, nStateCount))
		{
			bRet = true;
		}
	}
	return bRet;
}

// ����״̬
int _LKSkinTreeCtrlItem::AddState(int nImage, int nState, int nStateCount)
{
	LKSkinTreeCtrlItemState st;
	st.nIconIndex = nImage;
	st.nState = nState;
	st.nStateCount = nStateCount;
	arrState.Add(st);
	return arrState.GetCount();
}
// ����״̬
int _LKSkinTreeCtrlItem::SetState(bool &bChange, int nStateIndex, int nImage, int nState, int nStateCount)
{
	if(nStateIndex < 0 || nStateIndex >= arrState.GetCount()) return -1;
	LKSkinTreeCtrlItemState &st = arrState[nStateIndex];
	if(st.nIconIndex == nImage && st.nState == nState)
	{
		bChange = true;
	}
	st.nIconIndex = nImage;
	st.nState = nState;
	st.nStateCount = nStateCount;
	return arrState.GetCount();
}

// ��ȡ״̬ͼƬ
int _LKSkinTreeCtrlItem::GetStateImageIndex(int nIndex)
{
	int nImageIndex(-1);
	if(nIndex >= 0 && nIndex < arrState.GetCount())
	{
		nImageIndex = arrState[nIndex].nIconIndex;
	}
	return nImageIndex;
}

// �ı�ĳһ��״̬
bool _LKSkinTreeCtrlItem::ModifyState(int nStateIndex, int nState)
{
	for(int i = 0; i < arrState.GetCount(); i++)
	{
		LKSkinTreeCtrlItemState &st = arrState[i];
		if(i == nStateIndex)
		{
			st.nState = nState;
			return true;
		}
	}
	return false;
}
// ��ȡ��������Ӧ�İ�ť���λ���������ұߵľ���
int _LKSkinTreeCtrlItem::GetButtonLeftFromItemRight(int nIndex)
{
	int nLeft = 0;
	if(nIndex >= 0)
	{
		for(int i = nIndex; i < arrBtn.GetCount(); i++)
		{
			nLeft += arrBtn[i]->nWidth;
		}
	}
	return nLeft;
}

void _LKSkinTreeCtrlItem::AddButton(int nIndex, CLKImage *pImg, CString strCaption)
{
    PTreeItemButton pBtn = new TreeItemButton;
    pBtn->nId = nIndex;
    pBtn->pImg = pImg;
    pBtn->nWidth = 4;
    pBtn->strCaption = strCaption;
    if(!pBtn->strCaption.IsEmpty())
    {
        pBtn->nWidth += CLKCtrlBase::GetStringWidth(pBtn->strCaption, CLKFontMgr::GetST90());
    }
    if(pBtn->pImg)
    {
        pBtn->nWidth += pBtn->pImg->GetWidth();
    }
    arrBtn.Add(pBtn);
}
//
void _LKSkinTreeCtrlItem::DrawButtons(CDC *pDC, CRect rtItem)
{
    CRect rtBtn(rtItem);
    rtBtn.top++;
    rtBtn.bottom--;
    for(int i = 0; i < arrBtn.GetCount(); i++)
    {
        PTreeItemButton pBtn = arrBtn[i];
        rtBtn.left = rtBtn.right - pBtn->nWidth;
        CRect rtBtnText(rtBtn);
        // ��ť����ͼƬ
        CLKImage *pImgButton = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_BUTTON_BEGIN + 1);
        if(pImgButton && m_nCurIndex == i)
        {
            // ����ť����
            int nIndex = m_bMouseDown ? 1 : 0;
            pImgButton->DrawToDC(pDC, rtBtn, nIndex, LKSKINBASE_DRAWBGSTYLE_ALL);
        }
        if(pBtn->pImg)
        {
            // ����ťͼ��
            CRect r(rtBtn);
            r.left += 2;
            r.right = r.left + pBtn->pImg->GetWidth();
            r.top += (r.Height() - pBtn->pImg->GetHeight()) / 2;
            r.bottom = r.top + pBtn->pImg->GetHeight();
            if(m_bMouseDown && m_nCurIndex == i)
            {
                r.OffsetRect(1, 1);
            }
            pBtn->pImg->DrawToDC(pDC, r, 0);
            rtBtnText.left = r.right;
        }
        if(!pBtn->strCaption.IsEmpty())
        {
            // ����ť��ǩ
		    pDC->DrawText(pBtn->strCaption, &rtBtnText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }

        rtBtn.right = rtBtn.left;
    }
}

bool _LKSkinTreeCtrlItem::IsButtonRect(CPoint point, CRect rtItem)
{
    int nWidth(0);
    for(int i = 0; i < arrBtn.GetCount(); i++)
    {
        nWidth += arrBtn[i]->nWidth;
    }
    rtItem.left = rtItem.right - nWidth;
    if(rtItem.PtInRect(point))
    {
        return true;
    }
    return false;
}

inline void _LKSkinTreeCtrlItem::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bMouseDown = true;
}

inline int _LKSkinTreeCtrlItem::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(m_bMouseDown)
    {
        m_bMouseDown = false;
        return m_nCurIndex;
    }
    return -1;
}

inline void _LKSkinTreeCtrlItem::OnMouseLeave()
{
    m_bMouseDown = false;
    m_nCurIndex  = -1;
}

bool _LKSkinTreeCtrlItem::OnMouseMove(UINT nFlags, CPoint point, CRect rtItem)
{
    int nWidth(0);
    int nIndex(-1);
    for(int i = 0; i < arrBtn.GetCount(); i++)
    {
        nWidth += arrBtn[i]->nWidth;
        rtItem.left = rtItem.right - nWidth;
        if(rtItem.PtInRect(point))
        {
            nIndex = i;
            break;
        }
        rtItem.right = rtItem.left;
    }
    if(m_nCurIndex != nIndex)
    {
        m_bMouseDown = false;
        m_nCurIndex = nIndex;
        // ����
        return true;
    }
    return false;
}

// CLKTreeCtrl

IMPLEMENT_DYNAMIC(CLKTreeCtrl, CLKCtrlBaseEx)

CLKTreeCtrl::CLKTreeCtrl(int nImageIndex)
: CLKCtrlBaseEx(nImageIndex)
// �ұ��ַ�����ɫ
, m_crRightStrCor(RGB(160, 160, 160))
// ����HINT��Ϣ��ʱ��(��)
, m_nHintTime(1)
// �Ƿ񱣴浽�ڴ����ڴ���ع�
, m_bIsLoadedORSave(false)
// �ͻ������Ƿ���Ҫ͸�������ڴ�ǰ��
, m_bTransparent(true)
// �ڴ滺���Ƿ����Լ�������
, m_bImgIsOwner(false)
// ������̬ͼƬ(�ڴ滺��)
, m_pNormalImg(0)
// ����ѡ��ǰ��ͼƬ
, m_pSelectImg(0)
// ����
, m_nIndent(2)
// ��������
, m_nTopOffset(1)
// �ɼ�Ԫ���ܸ���
, m_nVisibleItemCount(0)
// չ��Ԫ���ܸ���
, m_nExpandItemCount(0)
// �ɼ�Ԫ����ʼ����
, m_nVisibleItemBeginIndex(0)
// ÿһ��ĸ߶�
, m_nItemHeight(22)
// ѡ�н��
, m_pSelectItem(0)
// �������(����������ƶ��µ�)
, m_pCurItem(0)
// ��֮��ļ��
, m_nItemSplit(0)
// ��״̬֮��ļ��
, m_nItemStateSplit(2)
// �����߾�
, m_nItemLeftMargin(4)
// �ɼ�Ԫ����ʼ��ָ��
, m_pVisibleItemBeginItem(0)
//, m_pVScrollbar(0)
{
	//m_rtClientOffset = CRect(1, 1, 1, 1);
    m_pRootItem = new LKTREEITEM;
	m_pRootItem->bExpand = true;
}

CLKTreeCtrl::~CLKTreeCtrl()
{
	if(m_pNormalImg) delete m_pNormalImg;
    if(!m_bIsLoadedORSave)
    {
	    DeleteAllItems();
        delete m_pRootItem;
    }
}

BOOL CLKTreeCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBaseEx::Create(CS_DBLCLKS, /**/_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKTreeCtrl::Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBaseEx::Create(CS_DBLCLKS, /**/strCaption, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKTreeCtrl::CreateHide(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBaseEx::Create(CS_DBLCLKS, /**/_T(""), WS_CHILD, rect, pParentWnd, nID); 
	return bRet;
}

// ���״̬ͼ��
bool CLKTreeCtrl::AddStateImg(HRSRC hImg, CLKImage *pImage)
{
	m_arrImage.Add(pImage);
	return false;
}

// ����״̬ͼ��
bool CLKTreeCtrl::InsertStateImg(int nIndex, CLKImage *pImage)
{
	if(nIndex >= 0 && nIndex < m_arrImage.GetCount())
	{
		m_arrImage.InsertAt(nIndex, pImage);
	}else
	{
		m_arrImage.Add(pImage);
	}
	return true;
}

// Inserts a new item to the control.
//	_In_ int nSelectedImage, _In_ UINT nState, _In_ UINT nStateMask, _In_ LPARAM lParam,
//	_In_ PLKTREEITEM hParent, _In_ PLKTREEITEM hInsertAfter);
PLKTREEITEM CLKTreeCtrl::InsertItem(_In_z_ LPCTSTR lpszItem, _In_ PLKTREEITEM hParent, _In_ PLKTREEITEM hInsertAfter, void *pData)
{
	return InsertItem(lpszItem, -1, hParent, hInsertAfter, pData);
}

PLKTREEITEM CLKTreeCtrl::InsertItem(_In_z_ LPCTSTR lpszItem, _In_ int nImage, _In_ PLKTREEITEM hParent, _In_ PLKTREEITEM hInsertAfter, void *pData)
{
	PLKTREEITEM pItem = new LKTREEITEM;
	if(0 == hParent)
	{
		hParent = m_pRootItem;
	}
	int nIndex = hParent->arrItem.GetCount();
	if(hInsertAfter != 0)
	{
		for(int i = 0; i < hParent->arrItem.GetCount(); i++)
		{
			if(hInsertAfter == hParent->arrItem[i])
			{
				nIndex = i + 1;
				break;
			}
		}
	}
	if(nIndex > 0)
	{
		pItem->pNextItem = hParent->arrItem[nIndex - 1]->pNextItem;
		// ���½ڵ����һ���ڵ�ֵ
	    //hParent->arrItem[nIndex - 1]->pNextItem = pItem;
		hParent->arrItem[nIndex - 1]->SetNextItem(pItem);
	}else if(hParent->arrItem.GetCount() > 0)
    {
        pItem->pNextItem = hParent->arrItem[1];
    }else
	{
		//pItem->pNextItem = 0;
		pItem->pNextItem = hParent->pNextItem;
	}
	hParent->arrItem.InsertAt(nIndex, pItem);
	pItem->pParentItem = hParent;
	// չ����£ͼƬ����
	pItem->nExpandIndex = nImage;
	pItem->strCaption = lpszItem;
    pItem->pData = pData;
	// ����չ������
	if(pItem->pParentItem == m_pRootItem)
	{
		m_nExpandItemCount++;
	}else if(pItem->pParentItem && pItem->pParentItem->bExpand)
	{
		m_nExpandItemCount++;
	}
	// ����ˢ�¹����������С
    int nHeight = m_nExpandItemCount * (m_nItemHeight + m_nItemSplit) - m_nItemSplit;
    SetVScrollbarRang(m_nExpandItemCount, nHeight);
	//SetVScrollbarVisible();
	//
	return pItem;
}
PLKTREEITEM CLKTreeCtrl::InsertItem(PLKTREEITEM pInsertItem, PLKTREEITEM hParent, PLKTREEITEM hInsertAfter)
{
	PLKTREEITEM pItem = new LKTREEITEM;
	if(0 == hParent)
	{
		hParent = m_pRootItem;
	}
	int nIndex = hParent->arrItem.GetCount();
	if(hInsertAfter != 0)
	{
		for(int i = 0; i < hParent->arrItem.GetCount(); i++)
		{
			if(hInsertAfter == hParent->arrItem[i])
			{
				nIndex = i + 1;
				break;
			}
		}
	}
	if(nIndex > 0)
	{
		pItem->pNextItem = hParent->arrItem[nIndex - 1]->pNextItem;
		// ���½ڵ����һ���ڵ�ֵ
	    //hParent->arrItem[nIndex - 1]->pNextItem = pItem;
		hParent->arrItem[nIndex - 1]->SetNextItem(pItem);
	}else if(hParent->arrItem.GetCount() > 0)
    {
        pItem->pNextItem = hParent->arrItem[1];
    }else
	{
		//pItem->pNextItem = 0;
		pItem->pNextItem = hParent->pNextItem;
	}
	hParent->arrItem.InsertAt(nIndex, pItem);
	pItem->pParentItem = hParent;
	// չ����£ͼƬ����
	pItem->nExpandIndex = pInsertItem->nExpandIndex;
	pItem->strCaption = pInsertItem->strCaption;
	pItem->pData = pInsertItem->pData;
	pItem->arrState.Copy(pInsertItem->arrState);
	for(int i = 0; i < pInsertItem->arrBtn.GetCount(); i++)
	{
		//
		PTreeItemButton pBtn = new TreeItemButton(*(pInsertItem->arrBtn[i]));
		pItem->arrBtn.Add(pBtn);
	}

	// ����չ������
	if(pItem->pParentItem == m_pRootItem)
	{
		m_nExpandItemCount++;
	}else if(pItem->pParentItem && pItem->pParentItem->bExpand)
	{
		m_nExpandItemCount++;
	}
	// ����ˢ�¹����������С
    int nHeight = m_nExpandItemCount * (m_nItemHeight + m_nItemSplit) - m_nItemSplit;
    SetVScrollbarRang(m_nExpandItemCount, nHeight);
	//

	hInsertAfter = 0;
	for(int i = 0; i < pInsertItem->arrItem.GetCount(); i++)
	{
		hInsertAfter = InsertItem(pInsertItem->arrItem[i], pItem, hInsertAfter);
	}

	return pItem;
}

// ����ˢ�¹����������С��Ϊ�����Ч�ʣ��ֶ����ô˺���ˢ�£�
void CLKTreeCtrl::ReSetScrollbarSliderPos()
{
    // ����ɼ�Ԫ�صĸ���
    CalcVisibleCount();
	// ����չ���������
	CalcExpandItemCount();
    if(m_nVisibleItemCount < m_nExpandItemCount)
    {
	    SetVScrollbarVisible();
        int nHeight = m_nExpandItemCount * (m_nItemHeight + m_nItemSplit) - m_nItemSplit;
	    SetVScrollbarRang(m_nExpandItemCount, nHeight);
    }else
    {
	    SetVScrollbarVisible(false);
    }
}

// ���ø����
void CLKTreeCtrl::SetRootItem(PLKTREEITEM pItem)
{
	m_pRootItem->arrItem.RemoveAll();
	m_pRootItem->arrBtn.RemoveAll();
	m_pRootItem->arrState.RemoveAll();
	m_pRootItem->pData = 0;
	m_pRootItem->strCaption.Empty();
	m_pRootItem->strCaption2.Empty();
	if(pItem)
	{
		m_pRootItem->arrItem.Append(pItem->arrItem);
		m_pRootItem->arrBtn.Append(pItem->arrBtn);
		m_pRootItem->arrState.Append(pItem->arrState);
		m_pRootItem->pData = pItem->pData;
		m_pRootItem->strCaption = pItem->strCaption;
		m_pRootItem->strCaption2 = pItem->strCaption2;
		//m_pRootItem->
	}
}

PLKTREEITEM CLKTreeCtrl::SetItem(LPCTSTR lpszItem, int nImage, PLKTREEITEM hParent, int nIndex)
{
	PLKTREEITEM pItem = new LKTREEITEM;
	if(0 == hParent)
	{
		hParent = m_pRootItem;
	}
	if(nIndex < hParent->arrItem.GetCount() && nIndex >= 0)
	{
		hParent->arrItem.SetAt(nIndex, pItem);
	}else
	{
		hParent->arrItem.Add(pItem);
	}
	if(nIndex > 0)
	{
		pItem->pNextItem = hParent->arrItem[nIndex - 1]->pNextItem;
		hParent->arrItem[nIndex - 1]->pNextItem = pItem;
	}else
	{
		pItem->pNextItem = hParent->pNextItem;
		//hParent->pNextItem = pItem;
	}
	pItem->pParentItem = hParent;
	// չ����£ͼƬ����
	pItem->nExpandIndex = nImage;
	pItem->strCaption = lpszItem;
	// ����չ������
	if(pItem->pParentItem == m_pRootItem)
	{
		m_nExpandItemCount++;
	}else if(pItem->pParentItem && pItem->pParentItem->bExpand)
	{
		m_nExpandItemCount++;
	}
	//
	return pItem;
}

//// Removes the specified item from the control.
BOOL CLKTreeCtrl::DeleteItem(_In_ PLKTREEITEM pItem)
{
	BOOL bRet(true);
	if(pItem)
	{
		for(int i = 0; i < pItem->arrItem.GetCount(); i++)
		{
			DeleteItem(pItem->arrItem[i]);
		}
		pItem->arrItem.RemoveAll();
		if(pItem != m_pRootItem)
		{
			delete pItem;
		}
	}
	return bRet;
}

//// Removes all items from the control.
BOOL CLKTreeCtrl::DeleteAllItems()
{
	return DeleteItem(m_pRootItem);
}

//-------------------------------------------------------------------------------------------------
// ������
//   ���һ�����״̬
// ������
//   hItem       - ���ָ��
//   nImage      - ���״̬��Ӧ��ͼƬ�б������
//   nState	     - ��ĳ�ʼ״̬��Ĭ��״̬����Ӧ��ͼƬλ�ã����ͼƬ��ĳһ��ͼƬ��
//   nStateCount - ���״̬��Ӧ��ͼƬ�б��е����ͼƬ���ж��ٸ�ͼƬ��ɣ�����״̬����������״̬��
//-------------------------------------------------------------------------------------------------
bool CLKTreeCtrl::AddItemState(PLKTREEITEM hItem, int nImage, int nState, int nStateCount)
{
	if(hItem)
	{
		LKSkinTreeCtrlItemState st;
		st.nState = nState;
		st.nIconIndex = nImage;
		st.nStateCount = nStateCount;
		hItem->arrState.Add(st);
		return true;
	}
	return false;
}

// �ڵ�չ��������£
void CLKTreeCtrl::OnExpandOrCollage(PLKTREEITEM pItem)
{
	if(pItem)
	{
		pItem->OnExpandIconLButtonDown();
		// ����չ���������
		CalcExpandItemCount();
		// ����ˢ�¹����������С
		//ReSetScrollbarSliderPos();
        int nHeight = m_nExpandItemCount * (m_nItemHeight + m_nItemSplit) - m_nItemSplit;
		SetVScrollbarRang(m_nExpandItemCount, nHeight/*GetItemsHeight()*/);
	}
}

// ���ù���ǰ��ͼƬ
void CLKTreeCtrl::SetMemImage(CLKImage *pImg)
{
	SetMemImg(pImg);
	if(GetSafeHwnd() && IsWindowVisible())
	{
		Invalidate();
	}
}

// ���õ�ǰѡ����Ϊָ����
void CLKTreeCtrl::SetCurSelected(PLKTREEITEM pItem)
{
	PLKTREEITEM pParent(pItem);
	while(pParent)
	{
		//pParent->bExpand = true;
		if(!pParent->bExpand)
		{
			OnExpandOrCollage(pParent);
		}
		pParent = pParent->pParentItem;
	}
	m_pSelectItem = pItem;
	if(m_pSelectItem && GetSafeHwnd()) Invalidate();
}

// ���õ�ǰѡ����Ϊ��һ��
void CLKTreeCtrl::SetFirstItemToSelected()
{
	m_pSelectItem = GetNextExpandItem(m_pRootItem);
	if(m_pSelectItem && GetSafeHwnd()) Invalidate();
}

// ��ȡ��ǰ����µ��������
bool CLKTreeCtrl::GetCurItemRect(LPRECT lpRect)
{
    if(m_pCurItem)
    {
        return GetItemRectByItem(m_pCurItem, lpRect);
    }
    return false;
}

// ��ȡ��ǰ����µ��������
void *CLKTreeCtrl::GetCurItemData()
{
    if(m_pCurItem)
    {
        return m_pCurItem->pData;
    }
    return 0;
}

void CLKTreeCtrl::SaveDataToMem(CtrlDataBase *pData)
{
    if(pData)
    {
        m_pData = pData;
        TreeCtrlData *p = (TreeCtrlData *)pData;
        if(!m_bIsLoadedORSave)
        {
            m_bIsLoadedORSave = true;
        }
	    // ��һ�������(����ʾ)
	    p->pRootItem = m_pRootItem;
	    // ѡ�н��
	    p->pSelectItem = m_pSelectItem;
	    // �������(����������ƶ��µ�)
	    //p->pCurItem;
	    // ÿһ������һ��֮�������
	    p->nIndent = m_nIndent;
	    // ����
	    p->strTitle = m_strTitle;
	    // �ؼ���һ����ʾ��Ԫ���ڶ�����ƫ��
	    p->nTopOffset = m_nTopOffset;
	    // �ɼ�Ԫ���ܸ���
	    //p->nVisibleItemCount = m_nVisibleItemCount;
	    // �ɼ�Ԫ����ʼ����
	    p->nVisibleItemBeginIndex = m_nVisibleItemBeginIndex;
	    // �ɼ�Ԫ����ʼ��ָ��
	    p->pVisibleItemBeginItem = m_pVisibleItemBeginItem;
	    // չ��Ԫ���ܸ���
	    p->nExpandItemCount = m_nExpandItemCount;
	    // ÿһ��ĸ߶�
	    p->nItemHeight = m_nItemHeight;
	    // ��֮��ļ��
	    p->nItemSplit = m_nItemSplit;
	    // ��״̬֮��ļ��
	    p->nItemStateSplit = m_nItemStateSplit;
	    // �����߾�
	    p->nItemLeftMargin = m_nItemLeftMargin;
    }
    CLKCtrlBaseEx::SaveDataToMem(pData);
}

void CLKTreeCtrl::LoadDataFromMem(CtrlDataBase *pData)
{
    if(pData)
    {
        m_pData = pData;
        TreeCtrlData *p = (TreeCtrlData *)pData;
        if(!m_bIsLoadedORSave)
        {
            m_bIsLoadedORSave = true;
	        DeleteAllItems();
            delete m_pRootItem;
        }

        // ��������
	    // ��һ�������(����ʾ)
	    m_pRootItem = p->pRootItem;
	    // ѡ�н��
	    m_pSelectItem = p->pSelectItem;
	    // �������(����������ƶ��µ�)
	    //= p->pCurItem;
	    // ÿһ������һ��֮�������
	    m_nIndent = p->nIndent;
	    // ����
	    m_strTitle = p->strTitle;
	    // �ؼ���һ����ʾ��Ԫ���ڶ�����ƫ��
	    m_nTopOffset = p->nTopOffset;
	    // �ɼ�Ԫ���ܸ���
	    //m_nVisibleItemCount= p->nVisibleItemCount;
	    // �ɼ�Ԫ����ʼ����
	    m_nVisibleItemBeginIndex = p->nVisibleItemBeginIndex;
	    // �ɼ�Ԫ����ʼ��ָ��
	    m_pVisibleItemBeginItem = p->pVisibleItemBeginItem;
	    // չ��Ԫ���ܸ���
	    m_nExpandItemCount = p->nExpandItemCount;
	    // ÿһ��ĸ߶�
	    m_nItemHeight = p->nItemHeight;
	    // ��֮��ļ��
	    m_nItemSplit = p->nItemSplit;
	    // ��״̬֮��ļ��
	    m_nItemStateSplit = p->nItemStateSplit;
	    // �����߾�
	    m_nItemLeftMargin = p->nItemLeftMargin;
        // ����ˢ�¹����������С
        if(m_nVisibleItemCount < m_nExpandItemCount)
        {
	        SetVScrollbarVisible();
            int nHeight = m_nExpandItemCount * (m_nItemHeight + m_nItemSplit) - m_nItemSplit;
	        SetVScrollbarRang(m_nExpandItemCount, nHeight);
        }else
        {
	        SetVScrollbarVisible(false);
        }
    }
    CLKCtrlBaseEx::LoadDataFromMem(pData);
}

BEGIN_MESSAGE_MAP(CLKTreeCtrl, CLKCtrlBaseEx)
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CLKTreeCtrl ��Ϣ�������
LRESULT CLKTreeCtrl::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR == wParam)
	{
		// ����
		LKSCROLLINFO *pInfo = (LKSCROLLINFO *)lParam;
		// ������Ϣ
		m_nVisibleItemBeginIndex = pInfo->nPos;
        m_pVisibleItemBeginItem = GetItemByIndex(m_nVisibleItemBeginIndex);
		//CalcVisibleCount();
		Invalidate();
	}
	return CLKCtrlBaseEx::OnCommonMsg(wParam, lParam);
}



// �����ؼ����ʼ��
void CLKTreeCtrl::InitCtrlAfterCreate()
{
	ParseImageToMem();
	CLKCtrlBaseEx::InitCtrlAfterCreate();
}

// ˢ��ǰ������
void CLKTreeCtrl::ReSetMem()
{
	ParseImageToMem();

	if(GetSafeHwnd())
	{
		// ��Բ��
		CRect rc;
		GetWindowRect(&rc);
        GetClientRect(&m_rtClient);
		// ֻ�е����ڷǿͻ����Ž��е���
		if(rc.Height() - m_rtClient.Height() > 2)
		{
			CRgn rgn, rgn2;
			rc.MoveToXY(0, 0);
			rgn.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //����һ��Բ�Ǿ���
			CRect rc2(rc);
			rc2.top = rc.bottom - 4;
			rgn2.CreateRectRgn(rc2.left, rc2.top, rc2.right, rc2.bottom);
			rgn.CombineRgn(&rgn, &rgn2, RGN_OR);
			SetWindowRgn(rgn,TRUE); 
		}
		// ����ɼ�Ԫ�صĸ���
		CalcVisibleCount();
		//// ����������λ��
		//ReSetScrollbarPos();
	    if(m_nVisibleItemCount < m_nExpandItemCount)
	    {
		    SetVScrollbarVisible();
	    }else
	    {
		    SetVScrollbarVisible(false);
	    }
	}
	if(IsWindowVisible())
	{
		Invalidate();
	}
}
// ���������ͼƬ���ڴ滺����
void CLKTreeCtrl::ParseImageToMem()
{
	m_bImgIsOwner = true;
	// ��ȡ��Ҫ������ͼƬ
	CLKImage *pImg = GetParseImg();
	if(pImg)
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
		if(nNumber > 0)
		{
			CRect rtWnd;
			GetWindowRect(&rtWnd);
			rtWnd.MoveToXY(0, 0);
			if(m_pNormalImg) delete m_pNormalImg;
			m_pNormalImg = new CLKImage(rtWnd.Width(), rtWnd.Height()); 
			AngleOffset angleOffset(4, 0, 4);
			if(m_bTransparent)
			{
				//if (pImg->GetBPP() == 32)
				//{
				//	pImg->DrawToImage(m_pNormalImg, &prm, LKSKINBASE_DRAWBGSTYLE_ALL, angleOffset);
				//}
				//else
				//{
				//	pImg->DrawToImage(m_pNormalImg, &prm, LKSKINBASE_DRAWBGSTYLE_FRAME, angleOffset);
				//}
				CRect rtDest(rtWnd);
				rtDest.DeflateRect(m_rtClientOffset.left, m_rtClientOffset.top, m_rtClientOffset.right, m_rtClientOffset.bottom);

				BGParam pM;
				pM.rt = CRect(0, 0, 0, 0);
				GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&pM);
				pM.rt.OffsetRect(rt.left + m_rtClientOffset.left, rt.top + m_rtClientOffset.top);
				CLKImage *pI = (CLKImage *)pM.pBG;
				if(pI)
				{
					pM.rt.right = pM.rt.left + rtDest.Width();
					pM.rt.bottom = pM.rt.top + rtDest.Height();
					pI->DrawToImage(m_pNormalImg, rtDest, pM.rt);
				}

				if (pImg->GetBPP() == 32)
				{
					pImg->DrawToImage(m_pNormalImg, &prm, LKSKINBASE_DRAWBGSTYLE_ALL, angleOffset);
				}
				else
				{
					pImg->DrawToImage(m_pNormalImg, &prm, LKSKINBASE_DRAWBGSTYLE_FRAME, angleOffset);
				}
			}
			else
			{
				pImg->DrawToImage(m_pNormalImg, &prm, LKSKINBASE_DRAWBGSTYLE_ALL, angleOffset);
			}
			//CRect rtSrc(6, 28, pImg->GetWidth() - 6, pImg->GetHeight() - 6);
			//pImg->DrawToImage(m_pNormalImg, rtDest, rtSrc);
            // ������
            CString strText;
            GetWindowText(strText);
            if(!strText.IsEmpty())
            {
                CRect rtText(0, 0, rtWnd.Width(), m_rtClientOffset.top);
                m_pNormalImg->DrawText(strText, rtText, CLKFontMgr::GetSTB90());
            }
			SetMemImg(m_pNormalImg);
		}
	}
}

// ��ʼ��ͼƬ�б�
void CLKTreeCtrl::InitImageList()
{
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_COMMON_BEGIN + 1, LKIMAGELISTINDEX_COMMON_BEGIN + 1);
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_TREECTRL_BEGIN, LKIMAGELISTINDEX_TREECTRL_END);
	m_arrImage[3] = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_ICONBUSINESS);

	//m_arrImage[4] = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_BUTTON_LASTCHECK);
	//m_arrImage[5] = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_BUTTON_MONEYCHECK);
	//m_arrImage[6] = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_BUTTON_DEPCHECK);
	//m_arrImage[7] = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_BUTTON_TEAMSTATE);

	m_pSelectImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_SELECTANDFOCUS);
	if(m_pSelectImg)
	{
		m_nItemHeight = m_pSelectImg->GetHeight() / m_pSelectImg->GetImageNumber();
	}
    CLKImage *pImg = GetParseImg();
    if(pImg)
    {
        m_rtClientOffset = pImg->GetFrameRect();
    }
	//SetScrollbarImg(CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR), 0);
	SetScrollbarImg(CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_VSCROLLBAR), 0);
}

// ��print����
int CLKTreeCtrl::OnPrint(WPARAM wParam, LPARAM lParam)
{
    BGParam rm;
    GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&rm);
	CRect rt;
	GetRectInParent(&rt);
	rm.rt.OffsetRect(rt.left, rt.top);
    CDC *pDC = CDC::FromHandle((HDC)wParam);
    CLKImage *pImg = (CLKImage *)rm.pBG;
    //pImg->Draw(pDC->GetSafeHdc(), 0, 0, m_rtClient.Width(), m_rtClient.Height(), rm.rt.left, rm.rt.top, m_rtClient.Width(), m_rtClient.Height());
    pImg->Draw(pDC->GetSafeHdc(), 0, 0, rt.Width(), rt.Height(), rm.rt.left, rm.rt.top, rt.Width(), rt.Height());
    if(m_rtClientOffset.IsRectNull())
    {
        OnPaintEx(0, pDC);
    }else
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
    //OnPaint(pDC);
    return 0;
}

// �ڻ����ϻ�ͼ
void CLKTreeCtrl::OnPaint(CDC *pDC)
{
	int nIndex = 0;
    if(0 == m_pVisibleItemBeginItem) m_pVisibleItemBeginItem = GetNextExpandItem(m_pRootItem);
	PLKTREEITEM pItem = m_pVisibleItemBeginItem;//GetItemByVisibleIndex(m_nVisibleItemBeginIndex);
	CRect rt(0, m_nTopOffset, GetItemWidth(), m_nItemHeight + m_nTopOffset);
	for(int nIndex = 0; pItem && nIndex < m_nVisibleItemCount; nIndex++)
	{
		DrawItem(pDC, pItem, rt);
		pItem = GetNextExpandItem(pItem);
		rt.OffsetRect(0, m_nItemHeight + m_nItemSplit);
	}
}



//=================================================================================================
// CLKTreeCtrl ˽�к���	====��ʼ====

// ��ȡ������Ŀ��
int CLKTreeCtrl::GetItemWidth()
{
	int nWidth = m_rtClient.Width();//GetWidth();
	if(IsVScrollbarValid())
	{
		nWidth -= GetVScrollbarWidth();
	}/**/
	return nWidth;
}

// ��ȡ����������
int CLKTreeCtrl::GetItemLevel(PLKTREEITEM pItem)
{
	if(pItem->pParentItem == m_pRootItem)
	{
		return 0;
	}
	int nL(0);
	for(;pItem;pItem = pItem->pParentItem)
	{
		if(pItem->pParentItem == m_pRootItem)
		{
			break;
		}
		nL++;
	}
	return nL;
}


// ����
void CLKTreeCtrl::DrawItem(CDC *pDC, PLKTREEITEM pItem, CRect rtItem)
{
	if(pItem)
	{
		pDC->SetBkMode(TRANSPARENT);
		if (!m_pFtText)
		{
			m_pFtText = CLKFontMgr::GetST90();
		}
		CFont *pOldFont = pDC->SelectObject(m_pFtText);
		pDC->SetTextColor(m_crText);

        rtItem.DeflateRect(1, 0);
		int nIndent(0);
		if(m_nIndent > 0)
		{
			nIndent = m_nIndent * GetItemLevel(pItem);
		}
		// ����������
		if((m_pSelectItem == pItem || m_pCurItem == pItem) && m_pSelectImg)
		{
			CRect rtSelect(rtItem);
			
			if(m_pSelectItem == pItem)
			{
				m_pSelectImg->DrawLRToDC(pDC, rtSelect, 6, 6, m_pSelectImg->GetHeight() / m_pSelectImg->GetImageNumber());
			}else
			{
				m_pSelectImg->DrawLRToDC(pDC, rtSelect, 6, 6, 0);
			}
		}
		// �Ȼ���ߵ�չ�����Ű�ť
		CRect rtExpand(0, 0, 0, 0);
		if(pItem->nExpandIndex >= 0)
		{
			int nIconWidth = m_arrImage[pItem->nExpandIndex]->GetWidth();
			int nIconHeight = m_arrImage[pItem->nExpandIndex]->GetHeight() / 2;
			rtExpand = rtItem;
			rtExpand.top += (rtExpand.Height() - nIconHeight) / 2;
			rtExpand.bottom = rtExpand.top + nIconHeight;
			rtExpand.right = rtItem.Height();
			rtExpand.left  = (rtExpand.Width() - nIconWidth) / 2;
			rtExpand.OffsetRect(nIndent, 0);
			if(pItem->IsDir())
			{
				CLKImage *pImg = m_arrImage[pItem->nExpandIndex];
				if(pItem->bExpand)
				{
					//pImg->Draw(pDC->GetSafeHdc(), rtIcon, 1);
					pImg->DrawToDC(pDC, rtExpand.left, rtExpand.top, 1);
				}else
				{
					pImg->DrawToDC(pDC, rtExpand.left, rtExpand.top, 0);
				}
			}
		}else
		{
			rtExpand.OffsetRect(m_nItemLeftMargin + nIndent, 0);
		}
		// ������״̬
		CRect rtState(rtExpand.right, rtItem.top, rtExpand.right, rtItem.bottom);
		for(int i = 0; i < pItem->arrState.GetCount(); i++)
		{
			LKSkinTreeCtrlItemState &itemState = pItem->arrState[i];
			if(itemState.nIconIndex >= 0 && itemState.nIconIndex < m_arrImage.GetCount())
			{
				CLKImage *pImg = m_arrImage[itemState.nIconIndex];
				rtState.right = rtState.left + (pImg->GetWidth() > 16 ? 16 : pImg->GetWidth());
				int nH = pImg->GetHeight() / itemState.nStateCount;
				if(nH > 16) nH = 16;
				rtState.top   += (rtState.Height() - nH) / 2;
				rtState.bottom = rtState.top + nH;
                pImg->DrawToDC(pDC, rtState, itemState.nState);
			}
			if(i + 1 < pItem->arrState.GetCount())
			{
				rtState.OffsetRect(rtState.Width() + m_nItemStateSplit, 0);
			}else
			{
				rtState.OffsetRect(m_nItemStateSplit, 0);
			}
		}
        // ���ұ߰�ť
        if(m_pCurItem == pItem)
        {
            pItem->DrawButtons(pDC, rtItem);
        }

		// ���ı�

		// �Ȼ��ұ��ı�
		// ������߰�ť�ܿ��
		int nWidth = 0;
		if(m_pCurItem == pItem)
		{
			nWidth = pItem->GetButtonLeftFromItemRight(0);
		}
		if(!pItem->strCaption2.IsEmpty())
		{
			CRect rtRText(rtItem);
			rtRText.right -= nWidth + m_nItemLeftMargin;
			rtRText.left = rtState.right;
			if(rtRText.right > rtRText.left)
			{
                COLORREF corOld = pDC->SetTextColor(m_crRightStrCor);
				pDC->DrawText(pItem->strCaption2, &rtRText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
                pDC->SetTextColor(corOld);
			}
		}
		// �ٻ�����ı�
		if(!pItem->strCaption.IsEmpty())
		{
			CRect rtText(rtItem);
			CSize szText = pDC->GetTextExtent(pItem->strCaption2);
			rtText.right -= nWidth + m_nItemLeftMargin + szText.cx;
			rtText.left = rtState.right;
			if(rtText.right > rtText.left)
			{
				pDC->DrawText(pItem->strCaption, &rtText, m_uDrawItemCaptionType/*DT_LEFT | DT_VCENTER | DT_SINGLELINE*/);
			}
		}
		pDC->SelectObject(pOldFont);
	}
}

// ����ɼ�Ԫ�صĸ���
void CLKTreeCtrl::CalcVisibleCount()
{
	int nHeight = GetHeight() + m_nItemSplit;
	if(nHeight % (m_nItemHeight + m_nItemSplit)> 0)
	{
		m_nVisibleItemCount = nHeight / (m_nItemHeight + m_nItemSplit) + m_nItemSplit;
	}else
	{
		m_nVisibleItemCount = nHeight / (m_nItemHeight + m_nItemSplit);
	}
}

// ����չ���������
int CLKTreeCtrl::CalcExpandItemCount(PLKTREEITEM pItem)
{
	int nRet(1);
	if(pItem->bExpand)
	{
		for(int i = 0; i < pItem->arrItem.GetCount(); i++)
		{
			nRet += CalcExpandItemCount(pItem->arrItem[i]);
		}
	}
	return nRet;
}

// ����չ���������
void CLKTreeCtrl::CalcExpandItemCount()
{
	m_nExpandItemCount = 0;
	PLKTREEITEM pItem = m_pRootItem;
	for(int i = 0; i < pItem->arrItem.GetCount(); i++)
	{
		m_nExpandItemCount += CalcExpandItemCount(pItem->arrItem[i]);
	}
}

// �����������
int CLKTreeCtrl::GetItemCount(PLKTREEITEM pItem)
{
	int nRet(0);
	for(int i = 0; i < pItem->arrItem.GetCount(); i++)
	{
		nRet += CalcExpandItemCount(pItem->arrItem[i]);
	}
	return nRet;
}
int CLKTreeCtrl::GetItemCount()
{
	int nRet(0);
	PLKTREEITEM pItem = m_pRootItem;
	for(int i = 0; i < pItem->arrItem.GetCount(); i++)
	{
		nRet += GetItemCount(pItem->arrItem[i]) + 1;
	}
	return nRet;
}

//-------------------------------------------------------------------------------------------------
// ������
//   ��ָ����������ȡָ������չ��ָ��
// ������
//   nIndex      - ����ָ���������ƫ�Ƹ���
//   pBaseItem   - ָ����ָ��
// ����ֵ��
//   PLKTREEITEM - �����ָ��
//-------------------------------------------------------------------------------------------------
PLKTREEITEM CLKTreeCtrl::GetExpandItemByIndex(int nIndex, PLKTREEITEM pBaseItem)
{
	PLKTREEITEM pItem = pBaseItem;
	
	for(int n = 0; pItem; pItem = GetNextExpandItem(pItem))
	{
		if (nIndex == n) 
		{
			return pItem;
		}
		n++;
	}
	return pItem;
}

//-------------------------------------------------------------------------------------------------
// ������
//   ��������ȡָ������ָ��
// ������
//   nIndex      - ���ڵ�һ���������ƫ�Ƹ���
// ����ֵ��
//   PLKTREEITEM - ָ����ָ��
//-------------------------------------------------------------------------------------------------
PLKTREEITEM CLKTreeCtrl::GetItemByIndex(int nIndex)
{
	PLKTREEITEM pItem = m_pRootItem;
	int n = 0;
	while(pItem)
	{
		pItem = GetNextExpandItem(pItem);
		if (nIndex == n) 
		{
			return pItem;
		}
		n++;
	}
	return pItem;
}

//-------------------------------------------------------------------------------------------------
// ������
//   �ӿɼ���������ȡָ������ָ��
// ������
//   nIndex      - ���ڵ�һ���ɼ��������ƫ�Ƹ���
// ����ֵ��
//   PLKTREEITEM - ָ����ָ��
//-------------------------------------------------------------------------------------------------
PLKTREEITEM CLKTreeCtrl::GetItemByVisibleIndex(int nIndex)
{
    if(0 == m_pVisibleItemBeginItem) m_pVisibleItemBeginItem = GetNextExpandItem(m_pRootItem);
	PLKTREEITEM pItem = m_pVisibleItemBeginItem;
	int n = 0;
	while(pItem)
	{
		if(pItem)
		{
			if (nIndex == n) 
			{
				return pItem;
			}
			n++;
		}
		pItem = GetNextExpandItem(pItem);
	}
	return pItem;
}

// �������ָ���λ�û�ȡ��ָ��
PLKTREEITEM CLKTreeCtrl::GetItemByPoint(CPoint point)
{
	int nIndex = GetItemIndexByPoint(point);
	if(nIndex >= 0)
	{
		return GetItemByVisibleIndex(nIndex);
	}
	return 0;
}

// ��ȡ��һ���ɼ�����ָ��
PLKTREEITEM CLKTreeCtrl::GetNextExpandItem(PLKTREEITEM pItem)
{
	if(pItem)
	{
		if(pItem->bExpand && pItem->arrItem.GetCount() > 0)
		{
			return pItem->arrItem[0];
		}else
		{
			if(pItem->pNextItem)
			{
				return pItem->pNextItem;
			}else
			{
				while(pItem->pParentItem)
				{
					if(pItem->pParentItem->pNextItem)
					{
						return pItem->pParentItem->pNextItem;
					}else
					{
						pItem = pItem->pParentItem;
					}
				}
			}
		}
	}
	return 0;
}


// �������ָ���λ�û�ȡ�������������������ʼ�ɼ�����
int CLKTreeCtrl::GetItemIndexByPoint(CPoint point)
{
	CRect rtItem(0, m_nTopOffset, GetItemWidth(), m_nItemHeight + m_nTopOffset);
	
	for(int nIndex = 0; nIndex < m_nVisibleItemCount; nIndex++)
	{
		if(rtItem.PtInRect(point))
		{
			return nIndex;
		}
		rtItem.OffsetRect(0, m_nItemHeight + m_nItemSplit);
	}
	return -1;
}

// ���ݿɼ�������ȡָ���������
bool CLKTreeCtrl::GetItemRectByIndex(int nIndex, LPRECT lpRect)
{
	CRect rtItem(0, m_nTopOffset, GetItemWidth(), m_nItemHeight + m_nTopOffset);
	
	if(nIndex >= 0 && nIndex < m_nVisibleItemCount)
	{
		rtItem.OffsetRect(0, (m_nItemHeight + m_nItemSplit) * nIndex);
		return true;
	}
	return false;
}
// ���ݿɼ����ȡָ���������
bool CLKTreeCtrl::GetItemRectByItem(PLKTREEITEM pItem, LPRECT lpRect)
{
    //if(pItem) return false;
	CRect rtItem(0, m_nTopOffset, GetItemWidth(), m_nItemHeight + m_nTopOffset);
	
	for(int nIndex = 0; nIndex < m_nVisibleItemCount; nIndex++)
	{
		PLKTREEITEM pI = GetItemByVisibleIndex(nIndex);
		if(pItem == pI)
		{
			*lpRect = rtItem;
			return true;
		}
		rtItem.OffsetRect(0, m_nItemHeight + m_nItemSplit);
	}
	return false;
}

// CLKTreeCtrl ˽���麯��	====����====
//=================================================================================================

void CLKTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!PtInScrollbar(point)) 
	{
	    // ѡ��
	    if(m_pCurItem != m_pSelectItem)
	    {
            // ��ǰѡ������ı�
            ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTREECTRL_MSG_WPARAM_ITEMCHANGED, (LPARAM)m_pSelectItem);
		    m_pSelectItem = m_pCurItem;
		    Invalidate();
	    }else if(m_pSelectItem && m_pSelectItem->IsDir())
	    {
		    Invalidate();
	    }
        CRect rtItem(0, 0, 0, 0);
        GetItemRectByItem(m_pSelectItem, rtItem);
        if(m_pSelectItem && m_pSelectItem->IsButtonRect(point, &rtItem))
        {
            // ��갴����İ�ť��
            m_pSelectItem->OnLButtonDown(nFlags, point);
            InvalidateRect(&rtItem);
        }else
        {
	        // ��������
	        if(m_pSelectItem && m_pSelectItem->IsDir())
	        {
		        OnExpandOrCollage(m_pSelectItem);
	        }
        }
    }
    // ��Ϣ
    ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTREECTRL_MSG_WPARAM_ITEMCANCELHINT, 0);
    KillTimer(LKTREECTRL_TIMERID_HINT);
	CLKCtrlBaseEx::OnLButtonDown(nFlags, point);
}

void CLKTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(m_pCurItem)
    {
        int nIndex = m_pCurItem->OnLButtonUp(nFlags, point);
	    CRect rtItem(0, 0, 0, 0);
	    if(GetItemRectByItem(m_pCurItem, &rtItem))
	    {
		    InvalidateRect(&rtItem);
	    }
        // ��Ϣ nIndex - ��ť��������������
        if(nIndex >= 0)
        {
            // ע���첽��Ϣ�����������Ϣʱ���ܵ�ǰ���ѷ����仯����ע���쳣������ҵ��Ӱ��������Ժ��ԣ�
            ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTREECTRL_MSG_WPARAM_ITEMBTNCLICKED, nIndex);
        }
    }
	CLKCtrlBaseEx::OnLButtonUp(nFlags, point);
}

void CLKTreeCtrl::OnMouseLeave()
{
	CRect rtItem(0, 0, 0, 0);
	if(GetItemRectByItem(m_pCurItem, &rtItem))
	{
        // �뿪��������
        if(m_pCurItem)
        {
            m_pCurItem->OnMouseLeave();
        }
		InvalidateRect(&rtItem);
	}
	m_pCurItem = 0;
    // ��Ϣ
    ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTREECTRL_MSG_WPARAM_MOUSELEAVE, 0);
	CLKCtrlBaseEx::OnMouseLeave();
}

void CLKTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!GetScrollbarDraging() && !PtInScrollbar(point)) 
	{
	    CRect rtItem(0, 0, 0, 0);
	    PLKTREEITEM pItem = GetItemByPoint(point);
	    if(pItem != m_pCurItem)
	    {
            if(m_pCurItem)
            {
                // �뿪��������
                m_pCurItem->OnMouseLeave();
            }
		    if(GetItemRectByItem(m_pCurItem, &rtItem))
		    {
			    InvalidateRect(&rtItem);
		    }
		    m_pCurItem = pItem;
		    if(GetItemRectByItem(m_pCurItem, &rtItem))
		    {
			    InvalidateRect(&rtItem);
		    }
		    if(m_pCurItem)
		    {
			    TRACKMOUSEEVENT   tme;   
			    tme.cbSize		= sizeof(tme);   
			    tme.dwFlags		= TME_LEAVE; 
			    tme.hwndTrack	= this->GetSafeHwnd();   
			    tme.dwHoverTime = HOVER_DEFAULT; //ֻ��HOVER��Ч
			    ::TrackMouseEvent(&tme);

                //
                if(m_nHintTime > 0)
                {
                    // ��Ϣ
                    SetTimer(LKTREECTRL_TIMERID_HINT, m_nHintTime * 1000 / 2, 0);
                }
            }else
            {
                KillTimer(LKTREECTRL_TIMERID_HINT);
                // ��Ϣ
                ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTREECTRL_MSG_WPARAM_ITEMCANCELHINT, 0);
		    }

	    }else if(m_pCurItem)
        {
	        if(GetItemRectByItem(m_pCurItem, &rtItem))
	        {
                // �ڸ����������ƶ�
                if(m_pCurItem->OnMouseMove(nFlags, point, rtItem))
                {
			        InvalidateRect(&rtItem);
		        }
            }
        }
    }else
    {
        OnMouseLeave();
    }
	CLKCtrlBaseEx::OnMouseMove(nFlags, point);
}


void CLKTreeCtrl::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    switch(nIDEvent)
    {
    case LKTREECTRL_TIMERID_HINT:
        {
            KillTimer(LKTREECTRL_TIMERID_HINT);
            if(m_pCurItem)
            {
                ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTREECTRL_MSG_WPARAM_ITEMHINT, (LPARAM)m_pSelectItem);
            }
        }
        break;
    }
    CLKCtrlBaseEx::OnTimer(nIDEvent);
}

void CLKTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if(m_pSelectItem)
    {
        ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTREECTRL_MSG_WPARAM_ITEMDBCLICKED, (LPARAM)m_pSelectItem);
    }
    CLKCtrlBaseEx::OnLButtonDblClk(nFlags, point);
}
