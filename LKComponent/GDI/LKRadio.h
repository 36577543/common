#pragma once
#include "LKCommonCtrl.h"


// CLKRadio

class CLKRadio : public CLKCommonCtrl
{
	DECLARE_DYNAMIC(CLKRadio)

public:
	CLKRadio(int nImageIndex = 1);
	virtual ~CLKRadio();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, bool bIsGroup = false);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID, bool bIsGroup = false);
    bool GetSelected(){return m_bSelected;}
    void SetSelected(bool bSelected = true);
    // 获当前选取中的索引
    int GetCurIndex();
    // 设置当前选取中的索引
    void SetCurIndex(int nIndex);
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
};


