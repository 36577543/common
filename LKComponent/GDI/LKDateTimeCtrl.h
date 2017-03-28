/******************************************************************************
* 版权所有 (C)2010, Sandongcun开发组
*
* 文件名称：CLKMonthCalCtrl.h
* 内容摘要：下拉日历控件类
* 其它说明：
* 当前版本：v1.0
* 作    者：刘坤
* 开始日期：2012.03.30  
* 完成日期：-  
* 注意：
*   当鼠标从子控件编辑框离开到本控件其它区域时， 这时本控件的WM_MOUSEMOVE先于
*   子控件编辑框的WM_MOUSELEAVE触发.
******************************************************************************/
#pragma once
#include "LKCtrlBase.h"
#include "LKMonthCalCtrl.h"

// CLKDateTimeCtrl

class CLKDateTimeCtrl : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKDateTimeCtrl)

public:
	// 编辑框消息拦截函数
	static LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	CLKDateTimeCtrl();
	virtual ~CLKDateTimeCtrl();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
public:
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
    // 是否进入编辑框
    bool    m_bEnterEdit;
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
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
    // 将前景画在指定的CDC设置上
    virtual void OnDrawFGToDC(BGParam *prm);
	// 察除背景消息
    virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
    // 编辑框控件
    CEdit   m_ctlEdit;
    // 日历控件
    CLKMonthCalCtrl     *m_pDate;
    // 前景缓存
    CLKImage *m_pImgArr[2];
    // 状态（0 - 常态； 1 - 激活状态）
    int     m_nState;
    // 鼠标处于什么之上(0 - 未知； 1 - 清空； 2 - 弹出日历)
    int     m_nMouseWhere;
    // 鼠标按下是的位置(0 - 未知； 1 - 清空； 2 - 弹出日历)
    int     m_nMouseDown;
    // 清空按钮位置
    CRect   m_rtClear;
    // 弹出日历按钮位置
    CRect   m_rtPop;
	// 编辑框原始回调函数地址
	WNDPROC		m_pOldEditProc;
    // 弹出日历控件
    void PopMonthCalCtrl();
public:
    afx_msg void OnSetFocus(CWnd* pOldWnd);
};


