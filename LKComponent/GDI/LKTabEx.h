#pragma once
#include "LKCtrlBase.h"
#include "LKTab.h"

// CLKTabEx

class CLKTabEx : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKTabEx)

public:
	CLKTabEx(int nImageIndex = 1);
	virtual ~CLKTabEx();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// 加入一项
	void AddItem(CString strText, int nImgIndex = -1, CWnd *pWnd = 0, void *pData = 0);
	void AddItem(CString strText, CLKImage *pImg, CWnd *pWnd, bool bMustFree, void *pData);
	void AddItem(CLKImage *pImg, CWnd *pWnd = 0, void *pData = 0);
	void SetItem(int nIndex, CString strText, CLKImage *pImg = 0, CWnd *pWnd = 0, void *pData = 0);
	void SetItem(int nIndex, CLKImage *pImg, CWnd *pWnd = 0, void *pData = 0);
	// 设置顶左边距
	void SetMargin(int nMargin){m_nMargin = nMargin;};
    // 设置是否自动调节项尺寸
    void SetIsAdjustItmeSize(bool bIsAdjustItmeSize = true){m_bIsAdjustItmeSize = bIsAdjustItmeSize;ReCalcItemSize();}
	// 选中一项
	void SelectItem(int nIndex = 0){m_nSelectIndex = nIndex;}
	// 获取选中项索引
	int GetSelectedItem(){return m_nSelectIndex;}
    // 设置是否需要关闭按钮
    void SetMustCloseButton(bool bMustCloseButton = true){ m_bMustCloseButton = bMustCloseButton;}
    // 删除一项(从起始索引开始)
    void DeleteItem(int nIndex);
    // 获取指定项数据
    void *GetItemData(int nIndex);
    // 获取项总数
    int GetCount(){return m_arrItems.GetCount();}
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    // 获取控件可放置区域
    CRect GetCtrlClientRect();
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
    // 是否需要关闭按钮
    bool            m_bMustCloseButton;
	// 顶左边距
	int				m_nMargin;
	// 项间隔
	int				m_nItemSplit;
	// 项宽
	int				m_nItemWidth;
    // 项高
    int             m_nItemHeight;
	// 选中项索引
	int				m_nSelectIndex;
	// 当前鼠标下项的索引
	int				m_nCurIndex;
	// 当前鼠标下箭头按钮的索引(-1 - 不在箭头区域， 0 - 左箭头， 1 - 右箭头)
	int				m_nCurArrIndex;
    // 鼠标状态
    bool            m_bMouseDown;
    // 鼠标是在删除按钮上
    bool            m_bInCloseBtn;
    // 显示可见项的索引
    int             m_nVisibleBeginIndex;
    // 左箭头可见性
    bool            m_bVisibleLArr;
    // 右箭头可见性
    bool            m_bVisibleRArr;
    // 左右箭头区域
    CRect           m_rtArr;
	// 背景图片
	CLKImage		*m_pBGImage;
    // 关闭按钮图片
    CLKImage        *m_pCloseImg;
    // 左箭头按钮
    CLKImage        *m_pLeftArrImg;
    // 右箭头按钮
    CLKImage        *m_pRightArrImg;
	// 项列表
	LKTabItemArray	m_arrItems;
	// 根据鼠标指针的位置获取项索引
	int GetItemIndexByPoint(CPoint point);
	// 根据可见索引获取指定项的区域
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
    // 重新计算项大小
    void ReCalcItemSize();
    // 计算指定项大小
    void CalcItemSize(LKTabItem *pItem);
    // 计算箭头的可见性
    void CalcArrVisible();
protected:
//    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


