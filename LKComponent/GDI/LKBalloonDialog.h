#pragma once
#include "GDICommon.h"
#include "LKImage.h"


// CLKBalloonDialog 对话框

class CLKBalloonDialog : public CDialog
{
	DECLARE_DYNAMIC(CLKBalloonDialog)

public:
	static void Show(UINT nIDTemplate, CRect rt, CString strText, UINT nSecs = 3, BOOL bBalloonUp = true);
	CLKBalloonDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLKBalloonDialog();
	// 设置背景图片
    //void SetSkinBG(CString &strImg, DWORD nImageType = CXIMAGE_FORMAT_PNG);
    //void SetSkinBG(HRSRC hImg, DWORD nImageType = CXIMAGE_FORMAT_PNG);
    //void SetSkinBG(CxImage *pImgBG);
	// 设置文本
	void SetText(CString &strText){m_strText = strText;}
	// 设置文本
	CString GetText(){return m_strText;}
	// 设置注销时间
	void SetDestroyTime(UINT uDestroyTime){m_uDestroyTime = uDestroyTime;}
	// 获取提示字符所占长度
	int GetTextExtent();
	// 获取提示字符所占高度
	int GetTextHeight();
	// 设置左边空白宽度
	void SetLeftMargin(int nLeftMargin){m_nLeftMargin = nLeftMargin;}
	// 获取左边空白宽度
	int GetLeftMargin(){return m_nLeftMargin;}
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
protected:
	virtual void OnLBtnDown(UINT nFlags, CPoint point){ReleaseCapture();}
	virtual void OnRBtnDown(UINT nFlags, CPoint point){ReleaseCapture();}
	virtual void OnShow(){SetCapture();}
	virtual void OnHide(){}

	DECLARE_MESSAGE_MAP()
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
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	virtual BOOL OnInitDialog();
};
