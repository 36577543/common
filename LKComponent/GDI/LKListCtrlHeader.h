#pragma once
#include "LKCtrlBase.h"

// 表格内嵌控件类型
// 常量，不可编辑
#define	LKLISTCTRL_INNERCTRL_STYLE_NONE		0
// 编辑框
#define	LKLISTCTRL_INNERCTRL_STYLE_EDIT		1
// 组合框
#define	LKLISTCTRL_INNERCTRL_STYLE_COMBO	2
// 日历框
#define	LKLISTCTRL_INNERCTRL_STYLE_TIMEDATE	3

class CLKListCtrlHeaderItem
{
public:
	CLKListCtrlHeaderItem()
	{
		m_strCaption.Empty();
		m_nWidth		= 80;
		m_nCtlStyle		= LKLISTCTRL_INNERCTRL_STYLE_NONE;
		m_nFormat		= DT_CENTER | DT_SINGLELINE | DT_VCENTER;
		m_pImg			= 0;
		m_pWnd			= 0;
	}
	CLKListCtrlHeaderItem(CString strCaption, int nWidth, UINT uStyle, int	nFormat, CLKImage *pImg)
	{
		m_strCaption	= strCaption;
		m_nWidth		= nWidth;
		m_nCtlStyle		= uStyle;
		m_nFormat		= nFormat;
		m_pImg			= pImg;
		m_pWnd			= 0;
	}

	~CLKListCtrlHeaderItem(){}
//private:
	// 项格式(编辑框，下拉框，下拉日期框)
	UINT		m_nCtlStyle;
	// 标题
	CString		m_strCaption;
	// 标题格式
	int			m_nFormat;
	// 列宽
	int			m_nWidth;
	// 图标
	CLKImage	*m_pImg;
	// 控件
	CWnd		*m_pWnd;
};

typedef CArray<CLKListCtrlHeaderItem *, CLKListCtrlHeaderItem *> LKListCtrlHIArry;
// CLKListCtrlHeader

class CLKListCtrlHeader : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKListCtrlHeader)

public:
	CLKListCtrlHeader(int nImageIndex = 1);
	virtual ~CLKListCtrlHeader();
	virtual BOOL Create(CWnd* pParentWnd, UINT nID);
	// 获取列总数
	int GetCount(){return m_arrItem.GetCount();}
	// 获取列总宽度
	int GetItemsWidth();
	// 获取列宽度
	int GetItemWidth(int nIndex);
	// 获取所有列左右端位置
	void GetLeftAndRight(int *pLeft, int *pRight);
	// 增加一项
	int AddItem(CString strCaption, int nWidth = 80, UINT uStyle = LKLISTCTRL_INNERCTRL_STYLE_NONE, int	nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER, CLKImage *pImg = 0);
public:
	LRESULT CLKListCtrlHeader::OnCommonMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnNcHitTest(CPoint point);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// 初始化图片列表
	virtual void InitImageList();
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem(){}
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
private:
	// 开始拖动
	bool				m_bIsDrag;
	// 鼠标在列表头上移动时的所处的列索引
	int					m_nCurCol;
	// 鼠标在列表头上按下时的位置
	CPoint				m_ptOld;
	// 列表头偏移量(滚动量)
	int					m_nOffset;
	// 列表头高度
	int					m_nHeight;
	// 项列表
	LKListCtrlHIArry	m_arrItem;
	// 增加一项
	int AddItem(CLKListCtrlHeaderItem *pItem);
	// 释放所有项
	void FreeItems();
};


