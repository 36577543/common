#pragma once
#include "LKDialog.h"

// 主窗体停靠方式
// 主窗体停靠上左右
#define WM_MAINDLG_STOCKMODE_LTR	0
// 主窗体只停靠上
#define WM_MAINDLG_STOCKMODE_TOP	1
// 主窗体无停靠
#define WM_MAINDLG_STOCKMODE_NONE	2


// CLKSlidDialog 对话框

class CLKSlidDialog : public CLKDialog
{
	DECLARE_DYNAMIC(CLKSlidDialog)

public:
	enum DialogStockModel{dsm_top, dsm_left, dsm_right, dsm_none};
	CLKSlidDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLKSlidDialog();

    // 获取停靠状态
    DialogStockModel GetDSMState(){return m_dsmState;}
protected:
	DECLARE_MESSAGE_MAP()
    void OpenMainPanel();
protected:
    // 允许隐藏
    bool                m_bCanHide;
    // 皮肤改变
    bool                m_bChangeSkin;
    // 处理窗口wm_print消息
    virtual void OnPrint(CDC *pDC);
    // 隐藏主窗口
    virtual void HideMainDlg();
    // 显示
    virtual void SlidOutMainDlg();
    // 实时改变停靠参数
    virtual void ChangeStockType(int &nStockType){}
private:
    // 停靠参数
    int                 m_nStockType;
	// 停靠状态
	DialogStockModel	m_dsmState;
    // 保存要绘制到动画显示的图片
	CLKImage		    *m_pAnimateImg;
public:
    afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnDestroy();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
