#pragma once
#include "LKCommonCtrlEx.h"


// CLKComboBox

class CLKComboBox : public CLKCommonCtrlEx
{
	DECLARE_DYNAMIC(CLKComboBox)
public:
	// �༭����Ϣ���غ���
	static LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
public:
	CLKComboBox(int nImageIndex = 1, int nButtonIndex = 3);
	virtual ~CLKComboBox();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	// ��ȡ������
	int GetCount(){return m_strItems.GetCount();}
	// ��ȡ��ǰѡ����
	int GetCurSel();
	CString GetCurText();
	// ���õ�ǰѡ����
	void SetCurSel(int nIndex);
	// ������ʾ��Ϣ
	void SetHintText(CString strHint);
	// �����ı�
	void SetWindowText(LPCTSTR lpszString);
	// ���ð�ť����
	void SetButtonIndex(int nIndex){m_nButtonIndex = nIndex;}
	// ������
	int AddItem(CString strText);
    // ɾ����
    CString DeleteItem(int nIndex);
	operator CEdit*() { return &m_ctlEdit; }
public:
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	// �ı������ı�
	void OnEnChangeEdit();
public:
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	// ������
	virtual void OnMouseEnter(CPoint point);
	// ˢ��ǰ������
	virtual void ReSetMem();
private:
	// �ı��ı�ǰ������
	CString		m_strOldText;
	// �Ƿ�����Զ����
	bool		m_bCanAutoComplete;
	// �༭��ˢ��
	CBrush		*m_pEditBr;
	// �ִ���
	CWnd		*m_pPreWnd;
	// ������ť����
	int			m_nButtonIndex;
	// ����ʱ��¼������λ��
	CRect		m_rtOldWnd;
	// ������ť��λ��
	CRect		m_rtButton;
	// �Ƿ񲶻����
	bool		m_bCapture;
	// �б�����߶�
	int			m_nMaxHeight;
	// �б�������߶�
	int			m_nDownHeight;
	// ������ť
	CLKImage	*m_pButtonImg;
	// ��ʾ�ı�
	CString		m_strHint;
	// ��ʾ�ı�����ɫ
	COLORREF	m_colHint;
	// ��Ƕ�༭�ؼ�
	CEdit		m_ctlEdit;
	// �༭��ԭʼ�ص�������ַ
	WNDPROC		m_pOldEditProc;
	// ��ȡ����������ĸ߶�
	int GetNewWindowHeight(){return m_nDownHeight;}
	// ��ȡ���һ���ԣ��¼�˳����ֵܿؼ�
	CWnd *GetLastBrotherWnd();
	void DropDown();
	// �����༭�򱳾�ˢ��
	void CreateEditBrush();

protected:
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
	// ���������λ��
	virtual void CalcScrollbarRect();

private:
	// �Ƿ񶯻�����
	bool			m_bAnimate;
	// ����ѡ��ǰ��ͼƬ
	CLKImage		*m_pSelectImg;
	// ����ѡ��ɾ��ͼƬ
	CLKImage		*m_pDeleteImg;
	// �ı�����߾�
	int				m_nTextLeftMargin;
	// �����߾�
	int				m_nItemLeftMargin;
	// ��Ķ�������
	int				m_nTopOffset;
	// ����ұ߾�
	int				m_nItemRightMargin;
	// ��֮��ļ��
	int				m_nItemSplit;
	// �ɼ�Ԫ���ܸ���
	int				m_nVisibleItemCount;
	// �ɼ�Ԫ����ʼ����
	int				m_nVisibleItemBeginIndex;
	// ÿһ��ĸ߶�
	int				m_nItemHeight;
	// ѡ�н������
	int				m_nSelectItem;
	// �����������(����������ƶ��µ�)
	int				m_nCurItem;
	// ɾ�������Ƿ��ڵ�ǰ�����
	bool			m_bIsDeleteArea;
	// ������
	CStringArray	m_strItems;
	// ����
	void DrawItem(CDC *pDC, CString &strText, CRect rtItem, bool bHighLight, bool bIsDelete = false);
	// ��ȡ���ܸ߶�
	int GetItemsHeight(){return (m_nItemHeight + m_nItemSplit) * m_strItems.GetCount() - m_nItemSplit;}
	// ����ɼ����ܸ߶�
	void CalcItemsHeight();
	// �������ܸ߶�
	int CalcVisibleItemCount();
	// �������ָ���λ�û�ȡ�������������������ʼ�ɼ�����
	int GetItemIndexByPoint(CPoint point);
	// ���ݿɼ�������ȡָ���������
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
	// �������ָ���λ�û�ȡָ���������
	bool GetItemRectByPoint(CPoint point, LPRECT lpRect);
	// ��ȡ��ֱ�������Ŀ��
	int GetVScrollbarWidth();
	// ��ȡ��������
	void GetDataRect(CRect &rtData);
	// ����
	void SlideOut(CWnd *pWnd = 0);
	// ������ʾ��������
	bool AutoCompleteText();
	// ���б���в��ұ༭���е�����
	int FindListText();
    // �ͷ���������ڴ�ͼƬ()
    void FreeOperatorImg();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


