#pragma once
#include "LKCtrlBase.h"

// �����Ƕ�ؼ�����
// ���������ɱ༭
#define	LKLISTCTRL_INNERCTRL_STYLE_NONE		0
// �༭��
#define	LKLISTCTRL_INNERCTRL_STYLE_EDIT		1
// ��Ͽ�
#define	LKLISTCTRL_INNERCTRL_STYLE_COMBO	2
// ������
#define	LKLISTCTRL_INNERCTRL_STYLE_TIMEDATE	3

class CLKListCtrlHeaderItem
{
public:
	CLKListCtrlHeaderItem()
	{
		m_strCaption.Empty();
		m_nWidth		= 80;
		m_nCtlStyle		= LKLISTCTRL_INNERCTRL_STYLE_NONE;
		m_nFormat		= DT_CENTER | DT_SINGLELINE | DT_VCENTER;
		m_pImg			= 0;
		m_pWnd			= 0;
	}
	CLKListCtrlHeaderItem(CString strCaption, int nWidth, UINT uStyle, int	nFormat, CLKImage *pImg)
	{
		m_strCaption	= strCaption;
		m_nWidth		= nWidth;
		m_nCtlStyle		= uStyle;
		m_nFormat		= nFormat;
		m_pImg			= pImg;
		m_pWnd			= 0;
	}

	~CLKListCtrlHeaderItem(){}
//private:
	// ���ʽ(�༭���������������ڿ�)
	UINT		m_nCtlStyle;
	// ����
	CString		m_strCaption;
	// �����ʽ
	int			m_nFormat;
	// �п�
	int			m_nWidth;
	// ͼ��
	CLKImage	*m_pImg;
	// �ؼ�
	CWnd		*m_pWnd;
};

typedef CArray<CLKListCtrlHeaderItem *, CLKListCtrlHeaderItem *> LKListCtrlHIArry;
// CLKListCtrlHeader

class CLKListCtrlHeader : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKListCtrlHeader)

public:
	CLKListCtrlHeader(int nImageIndex = 1);
	virtual ~CLKListCtrlHeader();
	virtual BOOL Create(CWnd* pParentWnd, UINT nID);
	// ��ȡ������
	int GetCount(){return m_arrItem.GetCount();}
	// ��ȡ���ܿ��
	int GetItemsWidth();
	// ��ȡ�п��
	int GetItemWidth(int nIndex);
	// ��ȡ���������Ҷ�λ��
	void GetLeftAndRight(int *pLeft, int *pRight);
	// ����һ��
	int AddItem(CString strCaption, int nWidth = 80, UINT uStyle = LKLISTCTRL_INNERCTRL_STYLE_NONE, int	nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER, CLKImage *pImg = 0);
public:
	LRESULT CLKListCtrlHeader::OnCommonMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnNcHitTest(CPoint point);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem(){}
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
private:
	// ��ʼ�϶�
	bool				m_bIsDrag;
	// ������б�ͷ���ƶ�ʱ��������������
	int					m_nCurCol;
	// ������б�ͷ�ϰ���ʱ��λ��
	CPoint				m_ptOld;
	// �б�ͷƫ����(������)
	int					m_nOffset;
	// �б�ͷ�߶�
	int					m_nHeight;
	// ���б�
	LKListCtrlHIArry	m_arrItem;
	// ����һ��
	int AddItem(CLKListCtrlHeaderItem *pItem);
	// �ͷ�������
	void FreeItems();
};


