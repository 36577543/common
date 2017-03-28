#pragma once
#include "LKCtrlBase.h"


// CLKStatic

class CLKStatic : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKStatic)

public:
	CLKStatic();
	virtual ~CLKStatic();
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// 是否显示阴影
	void SetShadow(bool bShadow = true){m_bShadow = bShadow;}
	// 设置显示内容
	void SetCaption(const CString &strCaption);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化图片列表
	virtual void InitImageList();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
    //// 画print背景
    //virtual int OnPrint(WPARAM wParam, LPARAM lParam);
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
	// 察除背景消息
    virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
	// 是否显示阴影
	bool m_bShadow;
public:
//	afx_msg void OnMove(int x, int y);
};


