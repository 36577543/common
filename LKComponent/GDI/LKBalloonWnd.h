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
	// ������߿հ׿��
	void SetLeftMargin(int nLeftMargin){m_nLeftMargin = nLeftMargin;}

protected:
	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
private:
	// �������Ͻ�λ��
	CPoint		m_ptDlgPos;
	// ��߿հ׿��
	int			m_nLeftMargin;
	// ��ʱע��ʱ��
	UINT		m_uDestroyTime;
	// ����ͼƬ
	CLKImage	*m_pImgBG;
	// ��ʾ�ı�
	CString		m_strText;
	// �ı�����
	CFont		*m_pFtText;
	// �ı���ɫ
	COLORREF	m_colText;
	// �����屳��
	void DrawDlgBG(CDC *pDC, CRect rtClient);
	// ���ı�
	void DrawInfoText(CDC *pDC, CRect rtClient);
	// ��ȡ��ʾ�ַ���ռ����
	int GetTextExtent();
	// ��ȡ��ʾ�ַ���ռ�߶�
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


