#pragma once
#include "LKButton.h"
#include "LKCtrlBaseEx.h"
#include "LKFontMgr.h"

typedef struct _TreeItemButton
{
    // ��ʶ
    int     nId;
    // ͼ��
    CLKImage *pImg;
    // ����
    CString strCaption;
    // ���
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


// ���״̬
typedef struct _LKSkinTreeCtrlItemState
{
	// ״̬(��0��ʼ����)
	int nState;
	// ״̬�ܸ���(��ʾ����״̬����ж����֣���Ӧ��ͼ��ͻ��ж��ٸ�)
	int	nStateCount;
	// ״̬��Ӧ��ͼ��������ĳһ��������Ӧ��ͼ����ܻ��ж����
	int	nIconIndex;
	_LKSkinTreeCtrlItemState()
	{
		nState = 0;
		nStateCount = 1;
		nIconIndex  = 0;
	}
}LKSkinTreeCtrlItemState;
typedef CArray<LKSkinTreeCtrlItemState, LKSkinTreeCtrlItemState &> LKSkinTreeCtrlItemStateArray;

// ��Ľṹ
typedef struct _LKSkinTreeCtrlItem
{
    // ���ָ��ĵ�ǰ��ť������-1 - ���ڵ�ǰ�ť�ϣ� 0 - �ڵ�һ����ť�ϡ�������������
    int         m_nCurIndex;
    // ����Ѱ���
    bool        m_bMouseDown;
	// ����
	CString		strCaption;
	// ����2(������ʾ)
	CString		strCaption2;
	// չ������ͼ������
	int			nExpandIndex;
	// չ������״̬
	bool		bExpand;
	// �ұ߰�ť�б�
	PTIBArray   arrBtn;
	// ���״̬�б�
	LKSkinTreeCtrlItemStateArray		arrState;
	// �����
	_LKSkinTreeCtrlItem *pParentItem;
	// �ֽ��
	// �ܽ��
	// �ӽ��
	CArray<_LKSkinTreeCtrlItem *, _LKSkinTreeCtrlItem *&> arrItem;
	// ��һ�����(�������ӽ��)
	_LKSkinTreeCtrlItem *pNextItem;
	// ����
	void		*pData;
	_LKSkinTreeCtrlItem();
	~_LKSkinTreeCtrlItem();
	// ��ȡ״̬ͼƬ����
	int GetStateImageIndex(int nIndex);
    // ��ȡ��Ĳ��
    int GetLevel();
    // ��ȡָ����
    _LKSkinTreeCtrlItem *GetItemByData(void *pDataA);
	// ���½ڵ����һ���ڵ�ֵ
	void SetNextItem(_LKSkinTreeCtrlItem *pNextItemA);
	// ����״̬
	bool SetStateByData(void *pDataA, int nStateIndex, int nImage, int nState = 0, int nStateCount = 3);
	// �Ƿ�ΪĿ¼
	bool IsDir(){ return (arrItem.GetCount() > 0 || nExpandIndex >= 0); }
	// ����������
	void OnExpandIconLButtonDown(){ bExpand = !bExpand; }
	// ����״̬
    int AddState(int nImage, int nState = 0, int nStateCount = 3);
	// ����״̬
    int SetState(bool &bChange, int nStateIndex, int nImage, int nState = 0, int nStateCount = 3);
	// �ı�ĳһ��״̬
	bool ModifyState(int nStateIndex, int nState = 0);
	// ��ȡ��������Ӧ�İ�ť���λ���������ұߵľ���
	int GetButtonLeftFromItemRight(int nIndex);
    // �����ұ��ַ���
    void SetRightCaption(CString strCaption){strCaption2 = strCaption;}
	// �����ұ߰�ť
    void AddButton(CLKImage *pImg = 0, CString strCaption = _T("")){ int nIndex = arrBtn.GetCount();AddButton(nIndex, pImg, strCaption); }
    void AddButton(int nIndex, CLKImage *pImg = 0, CString strCaption = _T(""));
    void DrawButtons(CDC *pDC, CRect rtItem);
    bool IsButtonRect(CPoint point, CRect rtItem);
	void OnLButtonDown(UINT nFlags, CPoint point);
	int OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseLeave();
	bool OnMouseMove(UINT nFlags, CPoint point, CRect rtItem);
}LKTREEITEM, *PLKTREEITEM;

// ����
struct TreeCtrlData : public CtrlDataBase
{
	// ��һ�������(����ʾ)
	PLKTREEITEM			pRootItem;
	// ѡ�н��
	PLKTREEITEM			pSelectItem;
	// �������(����������ƶ��µ�)
	//PLKTREEITEM			pCurItem;
	// ÿһ������һ��֮�������
	UINT				nIndent;
	// ����
	CString				strTitle;
	//// �ұ߰�ť�б�
	//PTIBArray           arrBtn;
	// �ؼ���һ����ʾ��Ԫ���ڶ�����ƫ��
	int			        nTopOffset;
	// �ɼ�Ԫ���ܸ���
	//int			    nVisibleItemCount;
	// �ɼ�Ԫ����ʼ����
	int			        nVisibleItemBeginIndex;
	// �ɼ�Ԫ����ʼ��ָ��
	PLKTREEITEM	        pVisibleItemBeginItem;
	// չ��Ԫ���ܸ���
	int			        nExpandItemCount;
	// ÿһ��ĸ߶�
	int			        nItemHeight;
	// ��֮��ļ��
	int			        nItemSplit;
	// ��״̬֮��ļ��
	int			        nItemStateSplit;
	// �����߾�
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
			// ���½ڵ����һ���ڵ�ֵ
		    //hParent->arrItem[nIndex - 1]->pNextItem = pItem;
			hParent->arrItem[nIndex - 1]->SetNextItem(pItem);
	    }else
	    {
		    pItem->pNextItem = hParent->pNextItem;
		    //hParent->pNextItem = pItem;
	    }
	    hParent->arrItem.InsertAt(nIndex, pItem);
	    pItem->pParentItem = hParent;
	    // չ����£ͼƬ����
	    pItem->nExpandIndex = nImage;
	    pItem->strCaption = lpszItem;
        pItem->pData = pData;
	    // ����չ������
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
    // ��ȡָ����
	PLKTREEITEM GetItemByData(void *pDataA){return pRootItem ? pRootItem->GetItemByData(pDataA) : 0;}
	// ����״̬
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

	// �����������
	int GetItemCount(PLKTREEITEM pItem);
	int GetItemCount();
	// ���ö�������
	void SetTopOffset(int nOffset){ m_nTopOffset = nOffset;}
    // ����״̬���
    void SetItemStateSplit(int nItemStateSplit){m_nItemStateSplit = nItemStateSplit;}
	// ����״̬ͼ��
	bool AddStateImg(HRSRC hImg, CLKImage *pImage);
	// ����״̬ͼ��
	bool InsertStateImg(int nIndex, CLKImage *pImage);
	// ���һ�����״̬
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
	// �ڵ�չ��������£
	void OnExpandOrCollage(PLKTREEITEM pItem);
	// ���ù���ǰ��ͼƬ
	void SetMemImage(CLKImage *pImg);
	// ��ȡ�����
	PLKTREEITEM GetRootItem(){return m_pRootItem;}
    // ��ȡ��ǰѡ����
    PLKTREEITEM GetCurSelected(){return m_pSelectItem;}
    // ���õ�ǰѡ����
    void SetCurSelected(PLKTREEITEM pItem);
    // ��ȡָ����
	PLKTREEITEM GetItemByData(void *pDataA){return m_pRootItem ? m_pRootItem->GetItemByData(pDataA) : 0;}
    // ���õ�һ��Ϊѡ����
    void SetFirstItemToSelected();
    // ��ȡ��ǰ����µ���
    PLKTREEITEM GetCurItem(){return m_pCurItem;}
	// ��ȡ��ǰ����µ��������
	bool GetCurItemRect(LPRECT lpRect);
	// ��ȡ��ǰ����µ��������
    void *GetCurItemData();
    // �������ݵ��ڴ�
    virtual void SaveDataToMem(CtrlDataBase *pData);
    // ���ڴ��м�������
    virtual void LoadDataFromMem(CtrlDataBase *pData);
	// ����ˢ�¹����������С
	void ReSetScrollbarSliderPos();
    // �����ұ��ַ�����ɫ
    void SetRightStrColour(COLORREF crText){m_crRightStrCor = crText;}
	// ���ø����
	void SetRootItem(PLKTREEITEM pItem);
	// ����״̬
	bool SetStateByData(void *pData, int nStateIndex, int nImage, int nState = 0, int nStateCount = 3){if(m_pRootItem) {return m_pRootItem->SetStateByData(pData, nStateIndex, nImage, nState, nStateCount);} return false;}
protected:
	DECLARE_MESSAGE_MAP()
    LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
    // ��print����
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
	// ���������Ϣ
	virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
	UINT				m_uDrawItemCaptionType = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	// �ұ��ַ�����ɫ
    COLORREF            m_crRightStrCor;
    // ����HINT��Ϣ��ʱ��(��)
    int                 m_nHintTime;
    // �Ƿ񱣴浽�ڴ����ڴ���ع�
    bool                m_bIsLoadedORSave;
	// �ͻ������Ƿ���Ҫ͸�������ڴ�ǰ��
	bool				m_bTransparent;
	// �ڴ滺���Ƿ����Լ�������
	bool				m_bImgIsOwner;
	// ������̬ͼƬ(�ڴ滺��)
	CLKImage			*m_pNormalImg;
	// ����ѡ��ǰ��ͼƬ
	CLKImage			*m_pSelectImg;
	// ��һ�������(����ʾ)
	PLKTREEITEM			m_pRootItem;
	// ѡ�н��
	PLKTREEITEM			m_pSelectItem;
	// �������(����������ƶ��µ�)
	PLKTREEITEM			m_pCurItem;
	// ÿһ������һ��֮�������
	UINT				m_nIndent;
	// ����
	CString				m_strTitle;
	// �ؼ���һ����ʾ��Ԫ���ڶ�����ƫ��
	int			m_nTopOffset;
	// �ɼ�Ԫ���ܸ���
	int			m_nVisibleItemCount;
	// �ɼ�Ԫ����ʼ����
	int			m_nVisibleItemBeginIndex;
	// �ɼ�Ԫ����ʼ��ָ��
	PLKTREEITEM	m_pVisibleItemBeginItem;
	// չ��Ԫ���ܸ���
	int			m_nExpandItemCount;
	// ÿһ��ĸ߶�
	int			m_nItemHeight;
	// ��֮��ļ��
	int			m_nItemSplit;
	// ��״̬֮��ļ��
	int			m_nItemStateSplit;
	// �����߾�
	int			m_nItemLeftMargin;
	// ����ɼ�Ԫ�صĸ���
	void CalcVisibleCount();
	// ����չ���������
	int CalcExpandItemCount(PLKTREEITEM pItem);
	void CalcExpandItemCount();
	// �������ָ���λ�û�ȡ�������������������ʼ�ɼ�����
	int GetItemIndexByPoint(CPoint point);
	// �������ָ���λ�û�ȡ��ָ��
	PLKTREEITEM GetItemByPoint(CPoint point);
	// ��������ȡָ������ָ��
	PLKTREEITEM GetItemByIndex(int nIndex);
	// �ӿɼ���������ȡָ������ָ��
	PLKTREEITEM GetItemByVisibleIndex(int nIndex);
	PLKTREEITEM GetExpandItemByIndex(int nIndex, PLKTREEITEM pBaseItem);
	// ���ݿɼ�������ȡָ���������
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
	// ���ݿɼ����ȡָ���������
	bool GetItemRectByItem(PLKTREEITEM pItem, LPRECT lpRect);
	// ��ȡ��һ��չ������ָ��
	PLKTREEITEM GetNextExpandItem(PLKTREEITEM pItem);
	// ����
	void DrawItem(CDC *pDC, PLKTREEITEM pItem, CRect rtItem);
	// ֪ͨ��ʾ��ť
	/*void NotifyShowButton(CRect &rtItem, PLKTREEITEM pItem);*/
	// ��ʾ��ť
	void ShowButton(CRect &rtItem, PLKTREEITEM pItem);
	// ��ȡ������Ŀ��
	int GetItemWidth();/**/
	// ��ȡ����������
	int GetItemLevel(PLKTREEITEM pItem);
	// ������ʾ��ʼ��
	void CalcBeginVisibleItem();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


