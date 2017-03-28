#pragma once
#include "LKImage.h"
#include "LKFontMgr.h"


// 窗口中的待画项
struct HintDlgDataItem
{
    // 索引
    int nIndex;
    // 待画区域
    CRect rtItem;
    // 待画内容
    CString strText;
    // 待画图标
    CLKImage *pImg;
    // 项类型(0 - 字符串, 1 - 图标, 2 - 普通按钮, 3 - 无背景按钮)
    int      nType;
    // 字符颜色
    COLORREF crText;
};

typedef CArray<HintDlgDataItem, HintDlgDataItem &>  HintDlgDataItemArr;

// CLKHintDialogBase 对话框

class CLKHintDialogBase : public CDialog
{
	DECLARE_DYNAMIC(CLKHintDialogBase)

public:
	CLKHintDialogBase(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLKHintDialogBase();
	virtual BOOL Create(CWnd* pParentWnd = NULL);
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
    // 重新设置背景图片
    void SetBGImage(CLKImage *pBGImg);
    // 延时隐藏提示窗口()
    void DelayHideHintDlg();
    void DelayHideHintDlg(int nElapse);
    // 设置鼠标移开是否自动隐藏窗口
    void SetAutoHideByMouseLeave(bool bAutoHideByMouseLeave = false){m_bAutoHideByMouseLeave = bAutoHideByMouseLeave;}
    // 获取窗口大小
    virtual CRect GetHintRect(){CRect rt;GetWindowRect(&rt);rt.MoveToXY(0, 0);return rt;}
    virtual void GetHintRect(CRect *pRt){}
    // 清空待画项
    void ClearItems(){m_arrItem.RemoveAll();}
    // 增加一项字符
    void AddStringItem(int nId, CRect rtItem, CString strItem, COLORREF crText);
    void AddStringItem(CRect rtItem, CString strItem, COLORREF crText = CLKFontMgr::GetDefaultTextColor());
    // 增加一项按钮
    void AddButtonItem(int nId, CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText);
    void AddButtonItem(CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText = CLKFontMgr::GetDefaultTextColor());
    // 增加一项扁平按钮
    void AddFlatButtonItem(int nId, CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText);
    void AddFlatButtonItem(CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText = CLKFontMgr::GetDefaultTextColor());
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	DECLARE_MESSAGE_MAP()
protected:
    virtual void OnDraw(CDC *pDC, CRect rt);
    virtual void OnDrawBG(CDC *pDC, CRect rt);
    virtual void OnDrawItems(CDC *pDC, CRect rt);
    virtual bool IsHideDialog(){return false;}
    bool DoUpdateDummyDialog();
private:
    // 数据
    HintDlgDataItemArr  m_arrItem;
    UINT m_nIDTemplate;
    // 背景图片
    CLKImage *m_pBGImg;
    // 鼠标是否已离开本窗口
    bool     m_bIsLeave;
    // 鼠标是否按下
    bool     m_bMouseDown;
    // 鼠标当前所处项索引
    int      m_nCurItemIndex;
    // 鼠标移开是否自动隐藏窗口
    bool    m_bAutoHideByMouseLeave;
    // 画普通按钮
    void DrawButton(HintDlgDataItem *pItem, CDC *pDC, CRect rtItem);
    // 画扁平按钮
    void DrawFlatButton(HintDlgDataItem *pItem, CDC *pDC, CRect rtItem);
public:
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//    afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
