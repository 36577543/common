#pragma once
#include "LKImage.h"


// LKBalloonWnd

class CLKBalloonWnd : public CWnd
{
	DECLARE_DYNAMIC(CLKBalloonWnd)

public:
	static void Show(CWnd *pParentWnd, CRect rt, CString strText, UINT nSecs = 3, BOOL bBalloonUp = true);
	static void Hide();
	CLKBalloonWnd();
	virtual ~CLKBalloonWnd();
	virtual BOOL Create(LPCTSTR lpszText, const RECT& rect, CWnd* pParentWnd);
	// 设置左边空白宽度
	void SetLeftMargin(int nLeftMargin){m_nLeftMargin = nLeftMargin;}

protected:
	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
private:
	// 窗口左上角位置
	CPoint		m_ptDlgPos;
	// 左边空白宽度
	int			m_nLeftMargin;
	// 定时注销时间
	UINT		m_uDestroyTime;
	// 背景图片
	CLKImage	*m_pImgBG;
	// 提示文本
	CString		m_strText;
	// 文本字体
	CFont		*m_pFtText;
	// 文本颜色
	COLORREF	m_colText;
	// 画窗体背景
	void DrawDlgBG(CDC *pDC, CRect rtClient);
	// 画文本
	void DrawInfoText(CDC *pDC, CRect rtClient);
	// 获取提示字符所占长度
	int GetTextExtent();
	// 获取提示字符所占高度
	int GetTextHeight();
public:
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnNcDestroy();
protected:
	virtual void PostNcDestroy();
};


