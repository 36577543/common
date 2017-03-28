#pragma once
#include "LKButton.h"
#include "LKCtrlBaseEx.h"
#include "LKFontMgr.h"

typedef struct _TreeItemButton
{
    // 标识
    int     nId;
    // 图标
    CLKImage *pImg;
    // 标题
    CString strCaption;
    // 宽度
    int     nWidth;
    _TreeItemButton()
    {
        nId         = 0;
        pImg        = 0;
        strCaption  = _T("");
        nWidth       = 20;
    }
    _TreeItemButton(_TreeItemButton &obj)
    {
        nId         = obj.nId;
        pImg        = obj.pImg;
        strCaption  = obj.strCaption;
        nWidth      = obj.nWidth;
    }
}TreeItemButton, *PTreeItemButton;
typedef CArray<PTreeItemButton, PTreeItemButton> PTIBArray;


// 项的状态
typedef struct _LKSkinTreeCtrlItemState
{
	// 状态(从0开始递增)
	int nState;
	// 状态总个数(表示这种状态最多有多少种，对应的图标就会有多少个)
	int	nStateCount;
	// 状态对应的图标索引（某一个索引对应的图标可能会有多个）
	int	nIconIndex;
	_LKSkinTreeCtrlItemState()
	{
		nState = 0;
		nStateCount = 1;
		nIconIndex  = 0;
	}
}LKSkinTreeCtrlItemState;
typedef CArray<LKSkinTreeCtrlItemState, LKSkinTreeCtrlItemState &> LKSkinTreeCtrlItemStateArray;

// 项的结构
typedef struct _LKSkinTreeCtrlItem
{
    // 鼠标指向的当前按钮索引（-1 - 不在当前项按钮上， 0 - 在第一个按钮上。。。。。。）
    int         m_nCurIndex;
    // 鼠标已按下
    bool        m_bMouseDown;
	// 标题
	CString		strCaption;
	// 标题2(靠右显示)
	CString		strCaption2;
	// 展开收起图标索引
	int			nExpandIndex;
	// 展开收起状态
	bool		bExpand;
	// 右边按钮列表
	PTIBArray   arrBtn;
	// 左边状态列表
	LKSkinTreeCtrlItemStateArray		arrState;
	// 父结点
	_LKSkinTreeCtrlItem *pParentItem;
	// 兄结点
	// 弟结点
	// 子结点
	CArray<_LKSkinTreeCtrlItem *, _LKSkinTreeCtrlItem *&> arrItem;
	// 下一个结点(不包括子结点)
	_LKSkinTreeCtrlItem *pNextItem;
	// 数据
	void		*pData;
	_LKSkinTreeCtrlItem();
	~_LKSkinTreeCtrlItem();
	// 获取状态图片索引
	int GetStateImageIndex(int nIndex);
    // 获取项的层次
    int GetLevel();
    // 获取指定项
    _LKSkinTreeCtrlItem *GetItemByData(void *pDataA);
	// 更新节点的下一个节点值
	void SetNextItem(_LKSkinTreeCtrlItem *pNextItemA);
	// 设置状态
	bool SetStateByData(void *pDataA, int nStateIndex, int nImage, int nState = 0, int nStateCount = 3);
	// 是否为目录
	bool IsDir(){ return (arrItem.GetCount() > 0 || nExpandIndex >= 0); }
	// 鼠标左键按下
	void OnExpandIconLButtonDown(){ bExpand = !bExpand; }
	// 增加状态
    int AddState(int nImage, int nState = 0, int nStateCount = 3);
	// 设置状态
    int SetState(bool &bChange, int nStateIndex, int nImage, int nState = 0, int nStateCount = 3);
	// 改变某一个状态
	bool ModifyState(int nStateIndex, int nState = 0);
	// 获取此索引对应的按钮左边位置离项最右边的距离
	int GetButtonLeftFromItemRight(int nIndex);
    // 设置右边字符串
    void SetRightCaption(CString strCaption){strCaption2 = strCaption;}
	// 插入右边按钮
    void AddButton(CLKImage *pImg = 0, CString strCaption = _T("")){ int nIndex = arrBtn.GetCount();AddButton(nIndex, pImg, strCaption); }
    void AddButton(int nIndex, CLKImage *pImg = 0, CString strCaption = _T(""));
    void DrawButtons(CDC *pDC, CRect rtItem);
    bool IsButtonRect(CPoint point, CRect rtItem);
	void OnLButtonDown(UINT nFlags, CPoint point);
	int OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseLeave();
	bool OnMouseMove(UINT nFlags, CPoint point, CRect rtItem);
}LKTREEITEM, *PLKTREEITEM;

// 数据
struct TreeCtrlData : public CtrlDataBase
{
	// 第一个根结点(不显示)
	PLKTREEITEM			pRootItem;
	// 选中结点
	PLKTREEITEM			pSelectItem;
	// 高亮结点(正处于鼠标移动下的)
	//PLKTREEITEM			pCurItem;
	// 每一级与上一级之间的缩进
	UINT				nIndent;
	// 标题
	CString				strTitle;
	//// 右边按钮列表
	//PTIBArray           arrBtn;
	// 控件第一个显示的元素在顶部的偏移
	int			        nTopOffset;
	// 可见元素总个数
	//int			    nVisibleItemCount;
	// 可见元素起始索引
	int			        nVisibleItemBeginIndex;
	// 可见元素起始项指针
	PLKTREEITEM	        pVisibleItemBeginItem;
	// 展开元素总个数
	int			        nExpandItemCount;
	// 每一项的高度
	int			        nItemHeight;
	// 项之间的间距
	int			        nItemSplit;
	// 项状态之间的间距
	int			        nItemStateSplit;
	// 项的左边距
	int			        nItemLeftMargin;
    TreeCtrlData()
    {
        pRootItem = 0;
	    pSelectItem = 0;
	    nIndent = 2;
	    nTopOffset = 0;
	    //nVisibleItemCount = 0;
	    nVisibleItemBeginIndex = 0;
	    pVisibleItemBeginItem = 0;
	    nExpandItemCount = 0;
	    nItemHeight = 22;
	    nItemSplit = 0;
	    nItemStateSplit = 2;
	    nItemLeftMargin = 4;
    }
    ~TreeCtrlData()
    {
        if(pRootItem)
        {
            DeleteItem(pRootItem);
            delete pRootItem;
        }
    }
    BOOL DeleteItem(PLKTREEITEM pItem)
    {
	    BOOL bRet(true);
	    if(pItem)
	    {
		    for(int i = 0; i < pItem->arrItem.GetCount(); i++)
		    {
			    DeleteItem(pItem->arrItem[i]);
		    }
		    pItem->arrItem.RemoveAll();
		    if(pItem != pRootItem)
		    {
			    delete pItem;
		    }
	    }
	    return bRet;
    }

	PLKTREEITEM InsertItem(_In_z_ LPCTSTR lpszItem, _In_ PLKTREEITEM hParent = 0, _In_ PLKTREEITEM hInsertAfter = 0, void *pData = 0)
    {
	    return InsertItem(lpszItem, -1, hParent, hInsertAfter, pData);
    }
	PLKTREEITEM InsertItem(_In_z_ LPCTSTR lpszItem, _In_ int nImage, _In_ PLKTREEITEM hParent = 0, _In_ PLKTREEITEM hInsertAfter = 0, void *pData = 0)
    {
	    PLKTREEITEM pItem = new LKTREEITEM;
	    if(0 == hParent)
	    {
		    hParent = pRootItem;
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
	    }else
	    {
		    pItem->pNextItem = hParent->pNextItem;
		    //hParent->pNextItem = pItem;
	    }
	    hParent->arrItem.InsertAt(nIndex, pItem);
	    pItem->pParentItem = hParent;
	    // 展开收拢图片索引
	    pItem->nExpandIndex = nImage;
	    pItem->strCaption = lpszItem;
        pItem->pData = pData;
	    // 计算展开总数
	    if(pItem->pParentItem == pRootItem)
	    {
		    nExpandItemCount++;
	    }else if(pItem->pParentItem && pItem->pParentItem->bExpand)
	    {
		    nExpandItemCount++;
	    }
	    //
	    return pItem;
    }
    // 获取指定项
	PLKTREEITEM GetItemByData(void *pDataA){return pRootItem ? pRootItem->GetItemByData(pDataA) : 0;}
	// 设置状态
	bool SetStateByData(void *pData, int nStateIndex, int nImage, int nState = 0, int nStateCount = 3){if(pRootItem) {return pRootItem->SetStateByData(pData, nStateIndex, nImage, nState, nStateCount);} return false;}
};

// CLKTreeCtrl
class CLKTreeCtrl : public CLKCtrlBaseEx
{
	DECLARE_DYNAMIC(CLKTreeCtrl)

public:
	CLKTreeCtrl(int nImageIndex = 1);
	virtual ~CLKTreeCtrl();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateHide(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// 计算项的总数
	int GetItemCount(PLKTREEITEM pItem);
	int GetItemCount();
	// 设置顶部距离
	void SetTopOffset(int nOffset){ m_nTopOffset = nOffset;}
    // 设置状态间距
    void SetItemStateSplit(int nItemStateSplit){m_nItemStateSplit = nItemStateSplit;}
	// 插入状态图标
	bool AddStateImg(HRSRC hImg, CLKImage *pImage);
	// 插入状态图标
	bool InsertStateImg(int nIndex, CLKImage *pImage);
	// 添加一个项的状态
	bool AddItemState(PLKTREEITEM hItem, int nImage, int nState = 0, int nStateCount = 3);

	// Gets the count of items in the control.
	//UINT GetCount() const;
	// Returns the level-to-level indentation (in pixels).
	UINT GetIndent() const{return m_nIndent;}
	// Sets the level-to-level indentation (in pixels).
	void SetIndent(_In_ UINT nIndent){m_nIndent = nIndent;}
	// Inserts a new item to the control.
	//PLKTREEITEM InsertItem(_In_ UINT nMask, _In_z_ LPCTSTR lpszItem, _In_ int nImage,
	//	_In_ int nSelectedImage, _In_ UINT nState, _In_ UINT nStateMask, _In_ LPARAM lParam,
	//	_In_ PLKTREEITEM hParent, _In_ PLKTREEITEM hInsertAfter);
	PLKTREEITEM InsertItem(_In_z_ LPCTSTR lpszItem, _In_ PLKTREEITEM hParent = 0, _In_ PLKTREEITEM hInsertAfter = 0, void *pData = 0);
	PLKTREEITEM InsertItem(_In_z_ LPCTSTR lpszItem, _In_ int nImage, _In_ PLKTREEITEM hParent = 0, _In_ PLKTREEITEM hInsertAfter = 0, void *pData = 0);
	PLKTREEITEM InsertItem(PLKTREEITEM pInsertItem, PLKTREEITEM hParent = 0, PLKTREEITEM hInsertAfter = 0);
	PLKTREEITEM SetItem(LPCTSTR lpszItem, int nImage, PLKTREEITEM hParent = 0, int nIndex = 0);
    //void SetItemData(void *pData);
	//// Removes the specified item from the control.
	BOOL DeleteItem(_In_ PLKTREEITEM pItem);
	//// Removes all items from the control.
	BOOL DeleteAllItems();
	// 节点展开或是收拢
	void OnExpandOrCollage(PLKTREEITEM pItem);
	// 设置共用前景图片
	void SetMemImage(CLKImage *pImg);
	// 获取根结点
	PLKTREEITEM GetRootItem(){return m_pRootItem;}
    // 获取当前选中项
    PLKTREEITEM GetCurSelected(){return m_pSelectItem;}
    // 设置当前选中项
    void SetCurSelected(PLKTREEITEM pItem);
    // 获取指定项
	PLKTREEITEM GetItemByData(void *pDataA){return m_pRootItem ? m_pRootItem->GetItemByData(pDataA) : 0;}
    // 设置第一项为选中项
    void SetFirstItemToSelected();
    // 获取当前鼠标下的项
    PLKTREEITEM GetCurItem(){return m_pCurItem;}
	// 获取当前鼠标下的项的区域
	bool GetCurItemRect(LPRECT lpRect);
	// 获取当前鼠标下的项的数据
    void *GetCurItemData();
    // 保存数据到内存
    virtual void SaveDataToMem(CtrlDataBase *pData);
    // 从内存中加载数据
    virtual void LoadDataFromMem(CtrlDataBase *pData);
	// 重新刷新滚动条滑块大小
	void ReSetScrollbarSliderPos();
    // 设置右边字符的颜色
    void SetRightStrColour(COLORREF crText){m_crRightStrCor = crText;}
	// 设置根结点
	void SetRootItem(PLKTREEITEM pItem);
	// 设置状态
	bool SetStateByData(void *pData, int nStateIndex, int nImage, int nState = 0, int nStateCount = 3){if(m_pRootItem) {return m_pRootItem->SetStateByData(pData, nStateIndex, nImage, nState, nStateCount);} return false;}
protected:
	DECLARE_MESSAGE_MAP()
    LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化图片列表
	virtual void InitImageList();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
    // 画print背景
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
	// 察除背景消息
	virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
	UINT				m_uDrawItemCaptionType = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	// 右边字符的颜色
    COLORREF            m_crRightStrCor;
    // 触发HINT消息的时间(秒)
    int                 m_nHintTime;
    // 是否保存到内存或从内存加载过
    bool                m_bIsLoadedORSave;
	// 客户区域是否需要透过父的内存前景
	bool				m_bTransparent;
	// 内存缓冲是否是自己创建的
	bool				m_bImgIsOwner;
	// 背景常态图片(内存缓冲)
	CLKImage			*m_pNormalImg;
	// 高亮选中前景图片
	CLKImage			*m_pSelectImg;
	// 第一个根结点(不显示)
	PLKTREEITEM			m_pRootItem;
	// 选中结点
	PLKTREEITEM			m_pSelectItem;
	// 高亮结点(正处于鼠标移动下的)
	PLKTREEITEM			m_pCurItem;
	// 每一级与上一级之间的缩进
	UINT				m_nIndent;
	// 标题
	CString				m_strTitle;
	// 控件第一个显示的元素在顶部的偏移
	int			m_nTopOffset;
	// 可见元素总个数
	int			m_nVisibleItemCount;
	// 可见元素起始索引
	int			m_nVisibleItemBeginIndex;
	// 可见元素起始项指针
	PLKTREEITEM	m_pVisibleItemBeginItem;
	// 展开元素总个数
	int			m_nExpandItemCount;
	// 每一项的高度
	int			m_nItemHeight;
	// 项之间的间距
	int			m_nItemSplit;
	// 项状态之间的间距
	int			m_nItemStateSplit;
	// 项的左边距
	int			m_nItemLeftMargin;
	// 计算可见元素的个素
	void CalcVisibleCount();
	// 计算展开项的总数
	int CalcExpandItemCount(PLKTREEITEM pItem);
	void CalcExpandItemCount();
	// 根据鼠标指针的位置获取项索引　－　相对于起始可见索引
	int GetItemIndexByPoint(CPoint point);
	// 根据鼠标指针的位置获取项指针
	PLKTREEITEM GetItemByPoint(CPoint point);
	// 从索引获取指定的项指针
	PLKTREEITEM GetItemByIndex(int nIndex);
	// 从可见的索引获取指定的项指针
	PLKTREEITEM GetItemByVisibleIndex(int nIndex);
	PLKTREEITEM GetExpandItemByIndex(int nIndex, PLKTREEITEM pBaseItem);
	// 根据可见索引获取指定项的区域
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
	// 根据可见项获取指定项的区域
	bool GetItemRectByItem(PLKTREEITEM pItem, LPRECT lpRect);
	// 获取下一个展开的项指针
	PLKTREEITEM GetNextExpandItem(PLKTREEITEM pItem);
	// 画项
	void DrawItem(CDC *pDC, PLKTREEITEM pItem, CRect rtItem);
	// 通知显示按钮
	/*void NotifyShowButton(CRect &rtItem, PLKTREEITEM pItem);*/
	// 显示按钮
	void ShowButton(CRect &rtItem, PLKTREEITEM pItem);
	// 获取待画项的宽度
	int GetItemWidth();/**/
	// 获取待画项的深度
	int GetItemLevel(PLKTREEITEM pItem);
	// 计算显示起始项
	void CalcBeginVisibleItem();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


