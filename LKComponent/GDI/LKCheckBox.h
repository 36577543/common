#pragma once
#include "LKCommonCtrl.h"


// CLKCheckBox

class CLKCheckBox : public CLKCommonCtrl
{
	DECLARE_DYNAMIC(CLKCheckBox)

public:
	CLKCheckBox(int nImageIndex = 1);
	virtual ~CLKCheckBox();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
    // 设置选中状态
    void SetSelected(bool bSelected = true, bool bErase = true);
    // 获取选中状态
    bool GetSelected(){return m_bSelected;}
	// 设置文本颜色
	void SetTextColor(COLORREF crText){m_crText = crText;}

protected:
	DECLARE_MESSAGE_MAP()
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化图片列表
	virtual void InitImageList();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
	// 初始化左键单击消息LPARAM参数
	virtual void InitLClick();
private:
	// 左边距
	int		m_nLeftMargin;
	// 图标与空格之间的距离
	int		m_nIconTextSplit;
	// 是否选中
	bool	m_bSelected;
	COLORREF	m_crText;
};


