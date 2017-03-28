// ..\..\common\LKComponent\GDI\LKTreeCtrl.cpp : 实现文件
//

#include "LKTreeCtrl.h"
#include "LKImageMgr.h"


// 提示信息触发定时器标识
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

// 获取项的层次//6225 7500 0348 4017
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

// 获取指定项(遍历)
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

// 更新节点的下一个节点值
void _LKSkinTreeCtrlItem::SetNextItem(_LKSkinTreeCtrlItem *pNextItemA)
{
	pNextItem = pNextItemA;
	int nCount = arrItem.GetCount();
	if(nCount > 0)
	{
		arrItem[nCount - 1]->SetNextItem(pNextItemA);
	}
}

// 设置状态
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

// 增加状态
int _LKSkinTreeCtrlItem::AddState(int nImage, int nState, int nStateCount)
{
	LKSkinTreeCtrlItemState st;
	st.nIconIndex = nImage;
	st.nState = nState;
	st.nStateCount = nStateCount;
	arrState.Add(st);
	return arrState.GetCount();
}
// 设置状态
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

// 获取状态图片
int _LKSkinTreeCtrlItem::GetStateImageIndex(int nIndex)
{
	int nImageIndex(-1);
	if(nIndex >= 0 && nIndex < arrState.GetCount())
	{
		nImageIndex = arrState[nIndex].nIconIndex;
	}
	return nImageIndex;
}

// 改变某一个状态
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
// 获取此索引对应的按钮左边位置离项最右边的距离
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
        // 按钮背景图片
        CLKImage *pImgButton = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_BUTTON_BEGIN + 1);
        if(pImgButton && m_nCurIndex == i)
        {
            // 画按钮背景
            int nIndex = m_bMouseDown ? 1 : 0;
            pImgButton->DrawToDC(pDC, rtBtn, nIndex, LKSKINBASE_DRAWBGSTYLE_ALL);
        }
        if(pBtn->pImg)
        {
            // 画按钮图标
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
            // 画按钮标签
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
        // 更新
        return true;
    }
    return false;
}

// CLKTreeCtrl

IMPLEMENT_DYNAMIC(CLKTreeCtrl, CLKCtrlBaseEx)

CLKTreeCtrl::CLKTreeCtrl(int nImageIndex)
: CLKCtrlBaseEx(nImageIndex)
// 右边字符的颜色
, m_crRightStrCor(RGB(160, 160, 160))
// 触发HINT消息的时间(秒)
, m_nHintTime(1)
// 是否保存到内存或从内存加载过
, m_bIsLoadedORSave(false)
// 客户区域是否需要透过父的内存前景
, m_bTransparent(true)
// 内存缓冲是否是自己创建的
, m_bImgIsOwner(false)
// 背景常态图片(内存缓冲)
, m_pNormalImg(0)
// 高亮选中前景图片
, m_pSelectImg(0)
// 缩进
, m_nIndent(2)
// 顶部距离
, m_nTopOffset(1)
// 可见元素总个数
, m_nVisibleItemCount(0)
// 展开元素总个数
, m_nExpandItemCount(0)
// 可见元素起始索引
, m_nVisibleItemBeginIndex(0)
// 每一项的高度
, m_nItemHeight(22)
// 选中结点
, m_pSelectItem(0)
// 高亮结点(正处于鼠标移动下的)
, m_pCurItem(0)
// 项之间的间距
, m_nItemSplit(0)
// 项状态之间的间距
, m_nItemStateSplit(2)
// 项的左边距
, m_nItemLeftMargin(4)
// 可见元素起始项指针
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

// 添加状态图标
bool CLKTreeCtrl::AddStateImg(HRSRC hImg, CLKImage *pImage)
{
	m_arrImage.Add(pImage);
	return false;
}

// 插入状态图标
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
		// 更新节点的下一个节点值
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
	// 展开收拢图片索引
	pItem->nExpandIndex = nImage;
	pItem->strCaption = lpszItem;
    pItem->pData = pData;
	// 计算展开总数
	if(pItem->pParentItem == m_pRootItem)
	{
		m_nExpandItemCount++;
	}else if(pItem->pParentItem && pItem->pParentItem->bExpand)
	{
		m_nExpandItemCount++;
	}
	// 重新刷新滚动条滑块大小
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
		// 更新节点的下一个节点值
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
	// 展开收拢图片索引
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

	// 计算展开总数
	if(pItem->pParentItem == m_pRootItem)
	{
		m_nExpandItemCount++;
	}else if(pItem->pParentItem && pItem->pParentItem->bExpand)
	{
		m_nExpandItemCount++;
	}
	// 重新刷新滚动条滑块大小
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

// 重新刷新滚动条滑块大小（为了提高效率，手动调用此函数刷新）
void CLKTreeCtrl::ReSetScrollbarSliderPos()
{
    // 计算可见元素的个素
    CalcVisibleCount();
	// 计算展开项的总数
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

// 设置根结点
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
	// 展开收拢图片索引
	pItem->nExpandIndex = nImage;
	pItem->strCaption = lpszItem;
	// 计算展开总数
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
// 描述：
//   添加一个项的状态
// 参数：
//   hItem       - 项的指针
//   nImage      - 项的状态对应的图片列表的索引
//   nState	     - 项的初始状态（默认状态）对应的图片位置（组合图片中某一个图片）
//   nStateCount - 项的状态对应的图片列表中的组合图片共有多少个图片组成（此类状态共含多少种状态）
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

// 节点展开或是收拢
void CLKTreeCtrl::OnExpandOrCollage(PLKTREEITEM pItem)
{
	if(pItem)
	{
		pItem->OnExpandIconLButtonDown();
		// 计算展开项的总数
		CalcExpandItemCount();
		// 重新刷新滚动条滑块大小
		//ReSetScrollbarSliderPos();
        int nHeight = m_nExpandItemCount * (m_nItemHeight + m_nItemSplit) - m_nItemSplit;
		SetVScrollbarRang(m_nExpandItemCount, nHeight/*GetItemsHeight()*/);
	}
}

// 设置共用前景图片
void CLKTreeCtrl::SetMemImage(CLKImage *pImg)
{
	SetMemImg(pImg);
	if(GetSafeHwnd() && IsWindowVisible())
	{
		Invalidate();
	}
}

// 设置当前选中项为指定项
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

// 设置当前选中项为第一项
void CLKTreeCtrl::SetFirstItemToSelected()
{
	m_pSelectItem = GetNextExpandItem(m_pRootItem);
	if(m_pSelectItem && GetSafeHwnd()) Invalidate();
}

// 获取当前鼠标下的项的区域
bool CLKTreeCtrl::GetCurItemRect(LPRECT lpRect)
{
    if(m_pCurItem)
    {
        return GetItemRectByItem(m_pCurItem, lpRect);
    }
    return false;
}

// 获取当前鼠标下的项的数据
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
	    // 第一个根结点(不显示)
	    p->pRootItem = m_pRootItem;
	    // 选中结点
	    p->pSelectItem = m_pSelectItem;
	    // 高亮结点(正处于鼠标移动下的)
	    //p->pCurItem;
	    // 每一级与上一级之间的缩进
	    p->nIndent = m_nIndent;
	    // 标题
	    p->strTitle = m_strTitle;
	    // 控件第一个显示的元素在顶部的偏移
	    p->nTopOffset = m_nTopOffset;
	    // 可见元素总个数
	    //p->nVisibleItemCount = m_nVisibleItemCount;
	    // 可见元素起始索引
	    p->nVisibleItemBeginIndex = m_nVisibleItemBeginIndex;
	    // 可见元素起始项指针
	    p->pVisibleItemBeginItem = m_pVisibleItemBeginItem;
	    // 展开元素总个数
	    p->nExpandItemCount = m_nExpandItemCount;
	    // 每一项的高度
	    p->nItemHeight = m_nItemHeight;
	    // 项之间的间距
	    p->nItemSplit = m_nItemSplit;
	    // 项状态之间的间距
	    p->nItemStateSplit = m_nItemStateSplit;
	    // 项的左边距
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

        // 加载数据
	    // 第一个根结点(不显示)
	    m_pRootItem = p->pRootItem;
	    // 选中结点
	    m_pSelectItem = p->pSelectItem;
	    // 高亮结点(正处于鼠标移动下的)
	    //= p->pCurItem;
	    // 每一级与上一级之间的缩进
	    m_nIndent = p->nIndent;
	    // 标题
	    m_strTitle = p->strTitle;
	    // 控件第一个显示的元素在顶部的偏移
	    m_nTopOffset = p->nTopOffset;
	    // 可见元素总个数
	    //m_nVisibleItemCount= p->nVisibleItemCount;
	    // 可见元素起始索引
	    m_nVisibleItemBeginIndex = p->nVisibleItemBeginIndex;
	    // 可见元素起始项指针
	    m_pVisibleItemBeginItem = p->pVisibleItemBeginItem;
	    // 展开元素总个数
	    m_nExpandItemCount = p->nExpandItemCount;
	    // 每一项的高度
	    m_nItemHeight = p->nItemHeight;
	    // 项之间的间距
	    m_nItemSplit = p->nItemSplit;
	    // 项状态之间的间距
	    m_nItemStateSplit = p->nItemStateSplit;
	    // 项的左边距
	    m_nItemLeftMargin = p->nItemLeftMargin;
        // 重新刷新滚动条滑块大小
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



// CLKTreeCtrl 消息处理程序
LRESULT CLKTreeCtrl::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR == wParam)
	{
		// 滚动
		LKSCROLLINFO *pInfo = (LKSCROLLINFO *)lParam;
		// 滚动消息
		m_nVisibleItemBeginIndex = pInfo->nPos;
        m_pVisibleItemBeginItem = GetItemByIndex(m_nVisibleItemBeginIndex);
		//CalcVisibleCount();
		Invalidate();
	}
	return CLKCtrlBaseEx::OnCommonMsg(wParam, lParam);
}



// 创建控件后初始化
void CLKTreeCtrl::InitCtrlAfterCreate()
{
	ParseImageToMem();
	CLKCtrlBaseEx::InitCtrlAfterCreate();
}

// 刷新前景缓存
void CLKTreeCtrl::ReSetMem()
{
	ParseImageToMem();

	if(GetSafeHwnd())
	{
		// 倒圆角
		CRect rc;
		GetWindowRect(&rc);
        GetClientRect(&m_rtClient);
		// 只有当存在非客户区才进行倒角
		if(rc.Height() - m_rtClient.Height() > 2)
		{
			CRgn rgn, rgn2;
			rc.MoveToXY(0, 0);
			rgn.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //创建一个圆角矩形
			CRect rc2(rc);
			rc2.top = rc.bottom - 4;
			rgn2.CreateRectRgn(rc2.left, rc2.top, rc2.right, rc2.bottom);
			rgn.CombineRgn(&rgn, &rgn2, RGN_OR);
			SetWindowRgn(rgn,TRUE); 
		}
		// 计算可见元素的个素
		CalcVisibleCount();
		//// 调整滚动条位置
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
// 解析出相关图片到内存缓存中
void CLKTreeCtrl::ParseImageToMem()
{
	m_bImgIsOwner = true;
	// 获取需要解析的图片
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
            // 画标题
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

// 初始化图片列表
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

// 画print背景
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

// 在缓存上绘图
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
// CLKTreeCtrl 私有函数	====开始====

// 获取待画项的宽度
int CLKTreeCtrl::GetItemWidth()
{
	int nWidth = m_rtClient.Width();//GetWidth();
	if(IsVScrollbarValid())
	{
		nWidth -= GetVScrollbarWidth();
	}/**/
	return nWidth;
}

// 获取待画项的深度
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


// 画项
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
		// 画高亮背景
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
		// 先画左边的展开收扰按钮
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
		// 画各个状态
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
        // 画右边按钮
        if(m_pCurItem == pItem)
        {
            pItem->DrawButtons(pDC, rtItem);
        }

		// 画文本

		// 先画右边文本
		// 计算左边按钮总宽度
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
		// 再画左边文本
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

// 计算可见元素的个素
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

// 计算展开项的总数
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

// 计算展开项的总数
void CLKTreeCtrl::CalcExpandItemCount()
{
	m_nExpandItemCount = 0;
	PLKTREEITEM pItem = m_pRootItem;
	for(int i = 0; i < pItem->arrItem.GetCount(); i++)
	{
		m_nExpandItemCount += CalcExpandItemCount(pItem->arrItem[i]);
	}
}

// 计算项的总数
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
// 描述：
//   从指定的索引获取指定的扩展项指针
// 参数：
//   nIndex      - 基于指定项的索引偏移个数
//   pBaseItem   - 指定项指针
// 返回值：
//   PLKTREEITEM - 结果项指针
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
// 描述：
//   从索引获取指定的项指针
// 参数：
//   nIndex      - 基于第一个项的索引偏移个数
// 返回值：
//   PLKTREEITEM - 指定项指针
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
// 描述：
//   从可见的索引获取指定的项指针
// 参数：
//   nIndex      - 基于第一个可见项的索引偏移个数
// 返回值：
//   PLKTREEITEM - 指定项指针
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

// 根据鼠标指针的位置获取项指针
PLKTREEITEM CLKTreeCtrl::GetItemByPoint(CPoint point)
{
	int nIndex = GetItemIndexByPoint(point);
	if(nIndex >= 0)
	{
		return GetItemByVisibleIndex(nIndex);
	}
	return 0;
}

// 获取下一个可见的项指针
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


// 根据鼠标指针的位置获取项索引　－　相对于起始可见索引
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

// 根据可见索引获取指定项的区域
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
// 根据可见项获取指定项的区域
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

// CLKTreeCtrl 私有虚函数	====结束====
//=================================================================================================

void CLKTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!PtInScrollbar(point)) 
	{
	    // 选中
	    if(m_pCurItem != m_pSelectItem)
	    {
            // 当前选中项发生改变
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
            // 鼠标按在项的按钮上
            m_pSelectItem->OnLButtonDown(nFlags, point);
            InvalidateRect(&rtItem);
        }else
        {
	        // 按键操作
	        if(m_pSelectItem && m_pSelectItem->IsDir())
	        {
		        OnExpandOrCollage(m_pSelectItem);
	        }
        }
    }
    // 消息
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
        // 消息 nIndex - 按钮索引（从右往左）
        if(nIndex >= 0)
        {
            // 注意异步消息，当处理此消息时可能当前项已发生变化。（注意异常保护，业务影响基本可以忽略）
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
        // 离开高亮的项
        if(m_pCurItem)
        {
            m_pCurItem->OnMouseLeave();
        }
		InvalidateRect(&rtItem);
	}
	m_pCurItem = 0;
    // 消息
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
                // 离开高亮的项
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
			    tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
			    ::TrackMouseEvent(&tme);

                //
                if(m_nHintTime > 0)
                {
                    // 消息
                    SetTimer(LKTREECTRL_TIMERID_HINT, m_nHintTime * 1000 / 2, 0);
                }
            }else
            {
                KillTimer(LKTREECTRL_TIMERID_HINT);
                // 消息
                ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTREECTRL_MSG_WPARAM_ITEMCANCELHINT, 0);
		    }

	    }else if(m_pCurItem)
        {
	        if(GetItemRectByItem(m_pCurItem, &rtItem))
	        {
                // 在高亮的项上移动
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
    // TODO: 在此添加消息处理程序代码和/或调用默认值
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
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if(m_pSelectItem)
    {
        ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTREECTRL_MSG_WPARAM_ITEMDBCLICKED, (LPARAM)m_pSelectItem);
    }
    CLKCtrlBaseEx::OnLButtonDblClk(nFlags, point);
}
