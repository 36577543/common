#pragma once
#include "LKCtrlBase.h"

// 状态
// 常态
#define LKTABITEM_STATE_NORMAL		0
// 选中
#define LKTABITEM_STATE_SELECT		1
// 当前-鼠标在上
#define LKTABITEM_STATE_MOUSEHOVE	2

typedef struct _LKTabItem
{
	// 标题
	CString		strText;
	// 状态(常态，选中，当前-鼠标在上)
	//int			nState;
	// 图标
	CLKImage	*pImg;
	// 关联的窗口指针
	CWnd		*pWnd;
	// 是否需要本控件释放
	bool		bMustFree;
    // 项的宽度
    int         nWidth;
    // 相关数据
    void        *pData;
	_LKTabItem(){pImg = 0; pWnd = 0; bMustFree = false;nWidth = 60; pData = 0;}
	~_LKTabItem(){if(bMustFree && pImg) delete pImg;}
}LKTabItem;
typedef CArray<LKTabItem *, LKTabItem *> LKTabItemArray;
// CLKTab

class CLKTab : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKTab)

public:
	CLKTab(int nImageIndex = 1, bool bVertical = false);
	virtual ~CLKTab();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	// 设置背景
	void SetBGImage(int nImageIndex);
	// 加入一项
	void AddItem(CString strText, int nImgIndex = -1, CWnd *pWnd = 0, void *pData = 0);
	void AddItem(CLKImage *pImg, CWnd *pWnd = 0, bool bMustFree = true);
	// 删除项
	void DelAllItem();
	// 选中一项
	void SelectItem(int nIndex = 0){ m_nSelectIndex = nIndex; ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTAP_MSG_WPARAM_ITEMCHANGED, (LPARAM)GetSafeHwnd()); }
	// 获取选中项索引
	int GetSelectedItem(){ return m_nSelectIndex; }
	// 获取选中项数据
	void *GetSelectedItemData(){ if (m_nSelectIndex >= 0 && m_nSelectIndex < m_arrItems.GetSize()){ return m_arrItems[m_nSelectIndex]->pData; } return 0; }
	// 获取选中项文本
	CString GetSelectedText(){ if (m_nSelectIndex >= 0 && m_nSelectIndex < m_arrItems.GetSize()){ return m_arrItems[m_nSelectIndex]->strText; } return L""; }
	// 设置顶左边距
	void SetMargin(int nMargin){m_nMargin = nMargin;};
    // 设置是否自动调节项尺寸
    void SetIsAdjustItmeSize(bool bIsAdjustItmeSize = true){m_bIsAdjustItmeSize = bIsAdjustItmeSize;CalcItemSize();}
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	DECLARE_MESSAGE_MAP()
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化图片列表
	virtual void InitImageList();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
    // 将前景画在指定的CDC设置上
    virtual void OnDrawFGToDC(BGParam *prm);
	// 察除背景消息
    virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
    // 是否自动调节项尺寸
    bool            m_bIsAdjustItmeSize;
	// 是否是纵向
	bool			m_bVertical;
	// 顶左边距
	int				m_nMargin;
	// 项间隔
	int				m_nItemSplit;
	// 项宽
	int				m_nItemWidth;
	// 项高
	int				m_nItemHeight;
	// 选中项索引
	int				m_nSelectIndex;
	// 当前鼠标下项的索引
	int				m_nCurIndex;
	// 背景图片
	CLKImage		*m_pBGImage;
	// 项列表
	LKTabItemArray	m_arrItems;
	// 根据鼠标指针的位置获取项索引
	int GetItemIndexByPoint(CPoint point);
	// 根据可见索引获取指定项的区域
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
    // 计算项大小
    void CalcItemSize();
};


