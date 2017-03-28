#pragma once
#include "LKCtrlBase.h"


// CLKUrlWnd

class CLKUrlWnd : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKUrlWnd)

public:
	CLKUrlWnd();
	virtual ~CLKUrlWnd();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, CLKImage *pImg = 0);
	// 设置默认图片
	void SetImage(CLKImage *pImg){m_pImg = pImg;}
	void SetImage2(CLKImage *pImg);
	// 获取图片
	CLKImage *GetImage(){return m_pImg;}
	CLKImage *GetImage2(){return m_pImg2;}
	// 设置链接
	void SetUrlString(CString strUrl){m_strUrl = strUrl;}
protected:
	DECLARE_MESSAGE_MAP()
	//// 解析出相关图片到内存缓存中
	//virtual void ParseImageToMem();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化图片列表
	virtual void InitImageList();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
	// 察除背景消息
	virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
	CLKImage *m_pImg;
	CLKImage *m_pImg2;
	// URL
	CString		m_strUrl;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


