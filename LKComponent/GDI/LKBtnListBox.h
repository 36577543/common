#pragma once
#include "LKCtrlBaseEx.h"
#include "LKButtonEx.h"

struct DeleteItemParam
{
    int     nIndex;
    bool    bDel;
};

// CLKBtnListBox

class CLKBtnListBox : public CLKCtrlBaseEx
{
	DECLARE_DYNAMIC(CLKBtnListBox)

public:
	CLKBtnListBox();
	virtual ~CLKBtnListBox();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// 增加一项
	void AddItem(CLKImage *pImg, CString strImage = _T(""));
	void AddItem(CString strImage);
	// 手动刷新
	void UpdateCtrl();
	// 清空选中状态
	void ClearSelectItem();
	// 获取当前选中项
	int GetSelectItem(){return m_nSelectIndex;}
	// 获取指定项的图片路径
	CString GetButtonImgPath(int nIndex);
	// 设置当前选中项
	void SetSelectItem(int nIndex);
	void SetSelectItem(CString &strFileName);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// 刷新前景缓存
	virtual void ReSetMem();
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);

private:
	// 最大列数
	int		m_nMaxCol;
	// 最大可见项个数
	int		m_nMaxVisibleCount;
	// 项的宽度
	int		m_nItemWidth;
	// 项的高度
	int		m_nItemHeight;
	// 可见项起始索引
	int		m_nVisibleIndex;
	// 当前选中索引
	int		m_nSelectIndex;
	CArray<CLKButtonEx *, CLKButtonEx *> m_ltItems;
	// 调整按钮位置
	void AdjustButton(bool bAdjustVScrollbar = true);
	// 获取数据区域
	void GetDataRect(CRect &rt);
	// 计算最大列数
	void CalcMaxColumn();
	// 计算最大可见项数
	void CalcMaxVisibleCount();
	// 获取指定索引项的区域
	CRect GetRectByIndex(int nIndex);
	// 根据项ＩＤ获取指定项索引
	int GetItemIndexById(int nId);
	// 根据项索引获取指定项ＩＤ
	int GetItemIdByIndex(int nIndex);
	// 获取最大行数
	int GetMaxRow();
	// 获取可见行数
	int GetVisibleRow();
	// 获取所有行高
	int GetRowsHeight();
};


