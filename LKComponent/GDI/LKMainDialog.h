#pragma once
#include "LKSlidDialog.h"


// CLKMainDialog

class CLKMainDialog : public CLKSlidDialog
{
	DECLARE_DYNAMIC(CLKMainDialog)

public:
	CLKMainDialog(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CLKMainDialog();

	// 获取应用程序指定托盘图标区域
	BOOL FindOutPositionOfIconDirectly(CRect& a_rcIcon);
    // 获取托盘参数指针
    NOTIFYICONDATA *GetTrayParam(){return &m_ndTray;}
    // 关闭程序
    virtual void Close(){}
    // 更新托盘状态
    void UpdatTrayState();
protected:
	DECLARE_MESSAGE_MAP()

	// 初始化背景缓存
	virtual void InitBGBmp();
    virtual void OnTrayLButtonDown(){}
    virtual void OnTrayRButtonDown(){}
    virtual void OnTrayRButtonUp(){}
    virtual void OnTrayLDBClick(){}
    // 最小化按钮被按下
    virtual void OnMinClick();

////////////托盘相关/////////////////////////////////////////////
protected:
    // 托盘图标
    HICON               m_hTrayIcon;
    HICON               m_hTrayGrayIcon;
	// 当前托盘图标句柄
	HICON				m_hCurTrayIcon;
    // 托盘消息是否需要分发其它模块处理（返回true则主窗口不再响应此消息）
    virtual bool OnNotifyTrayOtherModule(WPARAM wParam, LPARAM lParam){return false;}
	// 初始化托盘
	void InitTray();
private:
	//托盘
	NOTIFYICONDATA		m_ndTray;
	// 获取托盘区域
	static CRect GetTrayWndRect();
	// 获取托盘区域句柄
	static HWND GetTrayNotifyWnd();
	// 回调函数 从托盘通知区域找出托盘
	static BOOL CALLBACK FindToolBarInTrayWnd(HWND hwnd, LPARAM lParam);
	// 回调函数从任务栏上找出托盘通知区域
	static BOOL CALLBACK FindTrayWnd(HWND hwnd, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	// 重置托盘图标消息事件处理
	LRESULT OnNotifyTray(WPARAM wParam, LPARAM lParam);
};


