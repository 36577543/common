#pragma once
#include "LKCommonCtrlEx.h"


// CLKComboBox

class CLKComboBox : public CLKCommonCtrlEx
{
	DECLARE_DYNAMIC(CLKComboBox)
public:
	// 编辑框消息拦截函数
	static LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
public:
	CLKComboBox(int nImageIndex = 1, int nButtonIndex = 3);
	virtual ~CLKComboBox();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	// 获取项总数
	int GetCount(){return m_strItems.GetCount();}
	// 获取当前选中项
	int GetCurSel();
	CString GetCurText();
	// 设置当前选中项
	void SetCurSel(int nIndex);
	// 设置提示信息
	void SetHintText(CString strHint);
	// 设置文本
	void SetWindowText(LPCTSTR lpszString);
	// 设置按钮索引
	void SetButtonIndex(int nIndex){m_nButtonIndex = nIndex;}
	// 增加项
	int AddItem(CString strText);
    // 删除项
    CString DeleteItem(int nIndex);
	operator CEdit*() { return &m_ctlEdit; }
public:
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	// 文本发生改变
	void OnEnChangeEdit();
public:
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
	// 初始化图片列表
	virtual void InitImageList();
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 鼠标进入
	virtual void OnMouseEnter(CPoint point);
	// 刷新前景缓存
	virtual void ReSetMem();
private:
	// 文本改变前的数据
	CString		m_strOldText;
	// 是否可以自动完成
	bool		m_bCanAutoComplete;
	// 编辑框刷子
	CBrush		*m_pEditBr;
	// 兄窗口
	CWnd		*m_pPreWnd;
	// 下拉按钮索引
	int			m_nButtonIndex;
	// 下拉时记录下拉框位置
	CRect		m_rtOldWnd;
	// 下拉按钮的位置
	CRect		m_rtButton;
	// 是否捕获鼠标
	bool		m_bCapture;
	// 列表框最大高度
	int			m_nMaxHeight;
	// 列表框下拉高度
	int			m_nDownHeight;
	// 下拉框按钮
	CLKImage	*m_pButtonImg;
	// 提示文本
	CString		m_strHint;
	// 提示文本的颜色
	COLORREF	m_colHint;
	// 内嵌编辑控件
	CEdit		m_ctlEdit;
	// 编辑框原始回调函数地址
	WNDPROC		m_pOldEditProc;
	// 获取窗口下拉后的高度
	int GetNewWindowHeight(){return m_nDownHeight;}
	// 获取最后一个ＴＡＢ键顺序的兄弟控件
	CWnd *GetLastBrotherWnd();
	void DropDown();
	// 创建编辑框背景刷子
	void CreateEditBrush();

protected:
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
	// 计算滚动条位置
	virtual void CalcScrollbarRect();

private:
	// 是否动画滑出
	bool			m_bAnimate;
	// 高亮选中前景图片
	CLKImage		*m_pSelectImg;
	// 高亮选中删除图片
	CLKImage		*m_pDeleteImg;
	// 文本的左边距
	int				m_nTextLeftMargin;
	// 项的左边距
	int				m_nItemLeftMargin;
	// 项的顶部距离
	int				m_nTopOffset;
	// 项的右边距
	int				m_nItemRightMargin;
	// 项之间的间距
	int				m_nItemSplit;
	// 可见元素总个数
	int				m_nVisibleItemCount;
	// 可见元素起始索引
	int				m_nVisibleItemBeginIndex;
	// 每一项的高度
	int				m_nItemHeight;
	// 选中结点索引
	int				m_nSelectItem;
	// 高亮结点索引(正处于鼠标移动下的)
	int				m_nCurItem;
	// 删除区域是否处于当前鼠标下
	bool			m_bIsDeleteArea;
	// 项内容
	CStringArray	m_strItems;
	// 画项
	void DrawItem(CDC *pDC, CString &strText, CRect rtItem, bool bHighLight, bool bIsDelete = false);
	// 获取项总高度
	int GetItemsHeight(){return (m_nItemHeight + m_nItemSplit) * m_strItems.GetCount() - m_nItemSplit;}
	// 计算可见项总高度
	void CalcItemsHeight();
	// 计算项总高度
	int CalcVisibleItemCount();
	// 根据鼠标指针的位置获取项索引　－　相对于起始可见索引
	int GetItemIndexByPoint(CPoint point);
	// 根据可见索引获取指定项的区域
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
	// 根据鼠标指针的位置获取指定项的区域
	bool GetItemRectByPoint(CPoint point, LPRECT lpRect);
	// 获取垂直滚动条的宽度
	int GetVScrollbarWidth();
	// 获取数据区域
	void GetDataRect(CRect &rtData);
	// 滑出
	void SlideOut(CWnd *pWnd = 0);
	// 设置提示后续文字
	bool AutoCompleteText();
	// 从列表框中查找编辑框中的文字
	int FindListText();
    // 释放下拉后的内存图片()
    void FreeOperatorImg();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


