#pragma once
#include "LKCommonCtrl.h"


// CLKEdit

class CLKEdit : public CLKCommonCtrl
{
	DECLARE_DYNAMIC(CLKEdit)

public:
	// 设置边框
	void SetClientEdge(CRect rtEdge){ m_rtClientEdge = rtEdge; }
	// 编辑框消息拦截函数
	static LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	// nImageIndex = 1(高24) 2(高26)
	CLKEdit(int nImageIndex = 1);
	virtual ~CLKEdit();
	BOOL CreatePass(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(UINT uStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateHide(UINT uStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// 设置按钮图片
	void SetBtnImage(CLKImage *pImg){ m_pBtnImg = pImg; }
	// 设置提示信息
	void SetHintText(CString strHint);
	// 设置文本
	void SetWindowText(LPCTSTR lpszString);
    // 获取密码
    CString GetPassword();
	// 设备密码显示字符
	void SetPasswordChar(TCHAR ch){ m_cPassChar = ch; }
	// 设备文本背景颜色
	void SetTextBGCol(COLORREF rgb){m_colBack = rgb; }
	// 获取文本
	CString GetText(){CString strText; m_ctlEdit.GetWindowText(strText); return strText;}
	operator CEdit*() { return &m_ctlEdit; }
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// 文本发生改变
	void OnEnChangeEdit();

protected:
	DECLARE_MESSAGE_MAP()
	// 初始化图片列表
	virtual void InitImageList();
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	virtual void OnPaint(CDC *pDC);
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
private:
	// 边框距
	CRect   m_rtClientEdge;
	// 按钮是否在按钮上
	bool     m_bInBtn = false;
	// 按钮区域
	CRect    m_rtBtn;
	// 按钮图片
	CLKImage *m_pBtnImg = 0; 
	// 是否是本程序获取密码
    bool        m_bGetPass;
	// 密码显示字符
	TCHAR		m_cPassChar;
	// 编辑框类型
	UINT		m_uEditStyle;
	// 提示文本
	CString		m_strHint;
	// 提示文本的颜色
	COLORREF	m_colHint;
	// 内嵌编辑控件
	CEdit		m_ctlEdit;
	// 背景刷子
	CBrush		m_brBack;
	// 背景颜色
	COLORREF	m_colBack;
	// 编辑框原始回调函数地址
	WNDPROC		m_pOldEditProc;
protected:
	virtual void PreSubclassWindow();
public:
};


