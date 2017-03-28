#pragma once
#include "LKCtrlBaseEx.h"


// CLKScrollbar

class CLKScrollbar : public CLKCtrlBaseEx
{
	DECLARE_DYNAMIC(CLKScrollbar)

public:
	CLKScrollbar();
	virtual ~CLKScrollbar();
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID, bool bVisible = true);
	// 获取默认宽度
	int GetDefaultWidth();
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
	// 没有取得背景
	virtual void OnDrawNoParentBG(CDC *pDC);
	// 察除背景消息
    virtual bool OnEraseBG(CDC *pDC){return 1;}
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
private:
	// 拥有者
	CWnd	*m_pOwner;
	WNDPROC	m_funOldProc;
	static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};


