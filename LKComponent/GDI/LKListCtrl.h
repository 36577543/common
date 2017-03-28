#pragma once
#include "LKCtrlBaseEx.h"
#include "LKListCtrlHeader.h"
#include "LKListCtrlLeft.h"

// Ĭ���и� 22
#define LKLISTCTRLROW_DEFAULT_HIGHT		21
// ��
class CLKListCtrlRow
{
public:
	CLKListCtrlRow(int nSize){m_arrRow.SetSize(nSize);m_nHeight = LKLISTCTRLROW_DEFAULT_HIGHT;m_nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER;}
	~CLKListCtrlRow(){}
	CLKListCtrlRow &operator=(const CLKListCtrlRow &row)
	{
		m_nId = row.m_nId;
		m_arrRow.Copy(row.m_arrRow);
		return *this;
	}
	// ����������
	int InsertItem(int nRow, LPCTSTR lpszColumnHeading)
	{
		m_arrRow.InsertAt(nRow, lpszColumnHeading);
		return m_arrRow.GetCount();
	}
	void ClearItemText()
	{
		for(int i = 0; i < m_arrRow.GetCount(); i++)
		{
			m_arrRow[i].Empty();
		}
	}
public:
	// �б�ʶ��������
	int				m_nId;
	// �и�
	int				m_nHeight;
	// �ı�����
	UINT			m_nFormat;
	// 
	CStringArray	m_arrRow;
};
typedef CArray<CLKListCtrlRow *, CLKListCtrlRow *> CLKListCtrlRowArray;


// CLKListCtrl


class CLKListCtrl : public CLKCtrlBaseEx
{
	DECLARE_DYNAMIC(CLKListCtrl)

public:
	CLKListCtrl(int nImageIndex = 2);
	virtual ~CLKListCtrl();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// ����һ��
	int AddColumn(CString strCaption, int nWidth = 80, UINT uStyle = LKLISTCTRL_INNERCTRL_STYLE_NONE, int	nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER, CLKImage *pImg = 0);
	// ����һ��
	int AddItem(CString strCaption);
	// ���������
	void SetItemText(int nIndex, int nChildIndex, CString strCaption);
	// ��ȡѡ�������б��е�����
	int GetSelectedItem(){return m_nSelectIndex;}
	// ��ȡָ������ı�
	CString GetItemText(int nIndex, int nChildIndex);
	// ��ȡ���ܸ���
	int GetItemCount(){return m_arrData.GetCount();}
	// ����б�
	void Clear(){FreeData();}
public:
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
protected:
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ��ʼ�����������ϢLPARAM����
	virtual void InitLClick(){}
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
	// ���������Ϣ
	virtual bool OnEraseBG(CDC *pDC);
private:
	// ��ͷ
	CLKListCtrlHeader	*m_pHeader;
	// ������
	CLKListCtrlLeft		*m_pLeft;
	// ��������
	CRect				m_rtData;
	// ����
	CLKListCtrlRowArray	m_arrData;
	// �Ƿ���Ҫ��ʾ������
	bool				m_bShowHeader;
	// �Ƿ���Ҫ��ʾ�������
	bool				m_bShowLeft;
	// �ڴ滺���Ƿ����Լ�������
	bool				m_bImgIsOwner;
	// ������̬ͼƬ(�ڴ滺��)
	CLKImage			*m_pNormalImg;
	// �ɼ���ʼ��
	int					m_nBeginIndex;
	// �ɼ�������
	int					m_nEndIndex;
	// ��ǰ������(����ƶ�)����ڿɼ���ʼ��ƫ��
	int					m_nCurIndex;
	// ѡ�������� �������ʼ��ƫ��
	int					m_nSelectIndex;
	// ����ѡ��ǰ��ͼƬ
	CLKImage			*m_pSelectImg;
private:
	// �ͷŻ���ǰ��
	void FreeMemImg();
	// �ͷ�����
	void FreeData();
	// �������������
	void CalcShowEndIndex();
	// ��ѡ����
	void DrawSelectedItem(CDC *pDC);
	// ����ǰ��
	void DrawCurItem(CDC *pDC);
	// ���ߺ��ı�
	void DrawLineText(CDC *pDC);
	// �������ָ���λ�û�ȡ�������������������ʼ�ɼ�����
	int GetItemIndexByPoint(CPoint point);
	// ���ݿɼ�������ȡָ���������
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
	// ��������������
	void CalcDataRect();
};


