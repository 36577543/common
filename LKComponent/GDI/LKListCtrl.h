#pragma once
#include "LKCtrlBaseEx.h"
#include "LKListCtrlHeader.h"
#include "LKListCtrlLeft.h"

// 默认行高 22
#define LKLISTCTRLROW_DEFAULT_HIGHT		21
// 行
class CLKListCtrlRow
{
public:
	CLKListCtrlRow(int nSize){m_arrRow.SetSize(nSize);m_nHeight = LKLISTCTRLROW_DEFAULT_HIGHT;m_nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER;}
	~CLKListCtrlRow(){}
	CLKListCtrlRow &operator=(const CLKListCtrlRow &row)
	{
		m_nId = row.m_nId;
		m_arrRow.Copy(row.m_arrRow);
		return *this;
	}
	// 插入数据项
	int InsertItem(int nRow, LPCTSTR lpszColumnHeading)
	{
		m_arrRow.InsertAt(nRow, lpszColumnHeading);
		return m_arrRow.GetCount();
	}
	void ClearItemText()
	{
		for(int i = 0; i < m_arrRow.GetCount(); i++)
		{
			m_arrRow[i].Empty();
		}
	}
public:
	// 行标识（索引）
	int				m_nId;
	// 行高
	int				m_nHeight;
	// 文本类型
	UINT			m_nFormat;
	// 
	CStringArray	m_arrRow;
};
typedef CArray<CLKListCtrlRow *, CLKListCtrlRow *> CLKListCtrlRowArray;


// CLKListCtrl


class CLKListCtrl : public CLKCtrlBaseEx
{
	DECLARE_DYNAMIC(CLKListCtrl)

public:
	CLKListCtrl(int nImageIndex = 2);
	virtual ~CLKListCtrl();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// 增加一列
	int AddColumn(CString strCaption, int nWidth = 80, UINT uStyle = LKLISTCTRL_INNERCTRL_STYLE_NONE, int	nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER, CLKImage *pImg = 0);
	// 增加一项
	int AddItem(CString strCaption);
	// 设置项标题
	void SetItemText(int nIndex, int nChildIndex, CString strCaption);
	// 获取选中项在列表中的索引
	int GetSelectedItem(){return m_nSelectIndex;}
	// 获取指定项的文本
	CString GetItemText(int nIndex, int nChildIndex);
	// 获取项总个数
	int GetItemCount(){return m_arrData.GetCount();}
	// 清空列表
	void Clear(){FreeData();}
public:
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
protected:
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化左键单击消息LPARAM参数
	virtual void InitLClick(){}
	// 初始化图片列表
	virtual void InitImageList();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
	// 察除背景消息
	virtual bool OnEraseBG(CDC *pDC);
private:
	// 列头
	CLKListCtrlHeader	*m_pHeader;
	// 左序列
	CLKListCtrlLeft		*m_pLeft;
	// 数据区域
	CRect				m_rtData;
	// 数据
	CLKListCtrlRowArray	m_arrData;
	// 是否需要显示顶部项
	bool				m_bShowHeader;
	// 是否需要显示左边序列
	bool				m_bShowLeft;
	// 内存缓冲是否是自己创建的
	bool				m_bImgIsOwner;
	// 背景常态图片(内存缓冲)
	CLKImage			*m_pNormalImg;
	// 可见起始项
	int					m_nBeginIndex;
	// 可见结束项
	int					m_nEndIndex;
	// 当前项索引(鼠标移动)相对于可见起始项偏移
	int					m_nCurIndex;
	// 选中项索引 相对于起始项偏移
	int					m_nSelectIndex;
	// 高亮选中前景图片
	CLKImage			*m_pSelectImg;
private:
	// 释放缓存前景
	void FreeMemImg();
	// 释放数据
	void FreeData();
	// 计算结束项索引
	void CalcShowEndIndex();
	// 画选中项
	void DrawSelectedItem(CDC *pDC);
	// 画当前项
	void DrawCurItem(CDC *pDC);
	// 画线和文本
	void DrawLineText(CDC *pDC);
	// 根据鼠标指针的位置获取项索引　－　相对于起始可见索引
	int GetItemIndexByPoint(CPoint point);
	// 根据可见索引获取指定项的区域
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
	// 计算数据区域宽度
	void CalcDataRect();
};


