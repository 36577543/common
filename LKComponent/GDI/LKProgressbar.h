#pragma once
#include "LKCtrlBase.h"


// CLKProgressbar

class CLKProgressbar : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKProgressbar)

public:
	CLKProgressbar();
	virtual ~CLKProgressbar();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// 设置最大值
	void SetMax(int nMax){m_nMax = nMax;}
	// 设置当前值
	void SetCur(int nCur){m_nCur = nCur; if(GetSafeHwnd()) Invalidate();}
	// 设置提示文字
	void SetHintCaption(const CString strCaption){m_strHint = strCaption;}

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
	// 进度条最大值
	int	m_nMax;
	// 进度条当前值
	int m_nCur;
	// 提示信息
	CString m_strHint;
};


