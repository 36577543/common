#pragma once
#include "GDICommon.h"
#include "LKImage.h"


// CLKBalloonDialog �Ի���

class CLKBalloonDialog : public CDialog
{
	DECLARE_DYNAMIC(CLKBalloonDialog)

public:
	static void Show(UINT nIDTemplate, CRect rt, CString strText, UINT nSecs = 3, BOOL bBalloonUp = true);
	CLKBalloonDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLKBalloonDialog();
	// ���ñ���ͼƬ
    //void SetSkinBG(CString &strImg, DWORD nImageType = CXIMAGE_FORMAT_PNG);
    //void SetSkinBG(HRSRC hImg, DWORD nImageType = CXIMAGE_FORMAT_PNG);
    //void SetSkinBG(CxImage *pImgBG);
	// �����ı�
	void SetText(CString &strText){m_strText = strText;}
	// �����ı�
	CString GetText(){return m_strText;}
	// ����ע��ʱ��
	void SetDestroyTime(UINT uDestroyTime){m_uDestroyTime = uDestroyTime;}
	// ��ȡ��ʾ�ַ���ռ����
	int GetTextExtent();
	// ��ȡ��ʾ�ַ���ռ�߶�
	int GetTextHeight();
	// ������߿հ׿��
	void SetLeftMargin(int nLeftMargin){m_nLeftMargin = nLeftMargin;}
	// ��ȡ��߿հ׿��
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
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	virtual BOOL OnInitDialog();
};
