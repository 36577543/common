#pragma once
#include "LKCtrlBase.h"


// CLKImageWnd

class CLKImageWnd : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKImageWnd)

public:
	CLKImageWnd(CLKImage *pBGImg = 0, CLKImage *pImg = 0);
	virtual ~CLKImageWnd();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

    // 设置要背景图片
    void SetCtrlBGImg(CLKImage *pImg){m_pBGImg = pImg;}
    // 设置要显示的图片
    void SetCtrlImg(CLKImage *pImg){m_pImg = pImg;}
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
    // 画print背景
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
private:
    // 背景图片
    CLKImage *m_pBGImg;
    // 要显示的图片
    CLKImage *m_pImg;
};


