#pragma once
#include "LKCtrlBaseEx.h"
#include "LKButtonEx.h"

struct DeleteItemParam
{
    int     nIndex;
    bool    bDel;
};

// CLKBtnListBox

class CLKBtnListBox : public CLKCtrlBaseEx
{
	DECLARE_DYNAMIC(CLKBtnListBox)

public:
	CLKBtnListBox();
	virtual ~CLKBtnListBox();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// ����һ��
	void AddItem(CLKImage *pImg, CString strImage = _T(""));
	void AddItem(CString strImage);
	// �ֶ�ˢ��
	void UpdateCtrl();
	// ���ѡ��״̬
	void ClearSelectItem();
	// ��ȡ��ǰѡ����
	int GetSelectItem(){return m_nSelectIndex;}
	// ��ȡָ�����ͼƬ·��
	CString GetButtonImgPath(int nIndex);
	// ���õ�ǰѡ����
	void SetSelectItem(int nIndex);
	void SetSelectItem(CString &strFileName);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// ˢ��ǰ������
	virtual void ReSetMem();
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);

private:
	// �������
	int		m_nMaxCol;
	// ���ɼ������
	int		m_nMaxVisibleCount;
	// ��Ŀ��
	int		m_nItemWidth;
	// ��ĸ߶�
	int		m_nItemHeight;
	// �ɼ�����ʼ����
	int		m_nVisibleIndex;
	// ��ǰѡ������
	int		m_nSelectIndex;
	CArray<CLKButtonEx *, CLKButtonEx *> m_ltItems;
	// ������ťλ��
	void AdjustButton(bool bAdjustVScrollbar = true);
	// ��ȡ��������
	void GetDataRect(CRect &rt);
	// �����������
	void CalcMaxColumn();
	// �������ɼ�����
	void CalcMaxVisibleCount();
	// ��ȡָ�������������
	CRect GetRectByIndex(int nIndex);
	// ������ɣĻ�ȡָ��������
	int GetItemIndexById(int nId);
	// ������������ȡָ����ɣ�
	int GetItemIdByIndex(int nIndex);
	// ��ȡ�������
	int GetMaxRow();
	// ��ȡ�ɼ�����
	int GetVisibleRow();
	// ��ȡ�����и�
	int GetRowsHeight();
};


