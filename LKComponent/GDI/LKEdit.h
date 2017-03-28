#pragma once
#include "LKCommonCtrl.h"


// CLKEdit

class CLKEdit : public CLKCommonCtrl
{
	DECLARE_DYNAMIC(CLKEdit)

public:
	// ���ñ߿�
	void SetClientEdge(CRect rtEdge){ m_rtClientEdge = rtEdge; }
	// �༭����Ϣ���غ���
	static LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	// nImageIndex = 1(��24) 2(��26)
	CLKEdit(int nImageIndex = 1);
	virtual ~CLKEdit();
	BOOL CreatePass(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(UINT uStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateHide(UINT uStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// ���ð�ťͼƬ
	void SetBtnImage(CLKImage *pImg){ m_pBtnImg = pImg; }
	// ������ʾ��Ϣ
	void SetHintText(CString strHint);
	// �����ı�
	void SetWindowText(LPCTSTR lpszString);
    // ��ȡ����
    CString GetPassword();
	// �豸������ʾ�ַ�
	void SetPasswordChar(TCHAR ch){ m_cPassChar = ch; }
	// �豸�ı�������ɫ
	void SetTextBGCol(COLORREF rgb){m_colBack = rgb; }
	// ��ȡ�ı�
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
	// �ı������ı�
	void OnEnChangeEdit();

protected:
	DECLARE_MESSAGE_MAP()
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	virtual void OnPaint(CDC *pDC);
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
private:
	// �߿��
	CRect   m_rtClientEdge;
	// ��ť�Ƿ��ڰ�ť��
	bool     m_bInBtn = false;
	// ��ť����
	CRect    m_rtBtn;
	// ��ťͼƬ
	CLKImage *m_pBtnImg = 0; 
	// �Ƿ��Ǳ������ȡ����
    bool        m_bGetPass;
	// ������ʾ�ַ�
	TCHAR		m_cPassChar;
	// �༭������
	UINT		m_uEditStyle;
	// ��ʾ�ı�
	CString		m_strHint;
	// ��ʾ�ı�����ɫ
	COLORREF	m_colHint;
	// ��Ƕ�༭�ؼ�
	CEdit		m_ctlEdit;
	// ����ˢ��
	CBrush		m_brBack;
	// ������ɫ
	COLORREF	m_colBack;
	// �༭��ԭʼ�ص�������ַ
	WNDPROC		m_pOldEditProc;
protected:
	virtual void PreSubclassWindow();
public:
};


