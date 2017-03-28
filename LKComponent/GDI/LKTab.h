#pragma once
#include "LKCtrlBase.h"

// ״̬
// ��̬
#define LKTABITEM_STATE_NORMAL		0
// ѡ��
#define LKTABITEM_STATE_SELECT		1
// ��ǰ-�������
#define LKTABITEM_STATE_MOUSEHOVE	2

typedef struct _LKTabItem
{
	// ����
	CString		strText;
	// ״̬(��̬��ѡ�У���ǰ-�������)
	//int			nState;
	// ͼ��
	CLKImage	*pImg;
	// �����Ĵ���ָ��
	CWnd		*pWnd;
	// �Ƿ���Ҫ���ؼ��ͷ�
	bool		bMustFree;
    // ��Ŀ��
    int         nWidth;
    // �������
    void        *pData;
	_LKTabItem(){pImg = 0; pWnd = 0; bMustFree = false;nWidth = 60; pData = 0;}
	~_LKTabItem(){if(bMustFree && pImg) delete pImg;}
}LKTabItem;
typedef CArray<LKTabItem *, LKTabItem *> LKTabItemArray;
// CLKTab

class CLKTab : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKTab)

public:
	CLKTab(int nImageIndex = 1, bool bVertical = false);
	virtual ~CLKTab();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	// ���ñ���
	void SetBGImage(int nImageIndex);
	// ����һ��
	void AddItem(CString strText, int nImgIndex = -1, CWnd *pWnd = 0, void *pData = 0);
	void AddItem(CLKImage *pImg, CWnd *pWnd = 0, bool bMustFree = true);
	// ɾ����
	void DelAllItem();
	// ѡ��һ��
	void SelectItem(int nIndex = 0){ m_nSelectIndex = nIndex; ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTAP_MSG_WPARAM_ITEMCHANGED, (LPARAM)GetSafeHwnd()); }
	// ��ȡѡ��������
	int GetSelectedItem(){ return m_nSelectIndex; }
	// ��ȡѡ��������
	void *GetSelectedItemData(){ if (m_nSelectIndex >= 0 && m_nSelectIndex < m_arrItems.GetSize()){ return m_arrItems[m_nSelectIndex]->pData; } return 0; }
	// ��ȡѡ�����ı�
	CString GetSelectedText(){ if (m_nSelectIndex >= 0 && m_nSelectIndex < m_arrItems.GetSize()){ return m_arrItems[m_nSelectIndex]->strText; } return L""; }
	// ���ö���߾�
	void SetMargin(int nMargin){m_nMargin = nMargin;};
    // �����Ƿ��Զ�������ߴ�
    void SetIsAdjustItmeSize(bool bIsAdjustItmeSize = true){m_bIsAdjustItmeSize = bIsAdjustItmeSize;CalcItemSize();}
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	DECLARE_MESSAGE_MAP()
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
    // ��ǰ������ָ����CDC������
    virtual void OnDrawFGToDC(BGParam *prm);
	// ���������Ϣ
    virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
    // �Ƿ��Զ�������ߴ�
    bool            m_bIsAdjustItmeSize;
	// �Ƿ�������
	bool			m_bVertical;
	// ����߾�
	int				m_nMargin;
	// ����
	int				m_nItemSplit;
	// ���
	int				m_nItemWidth;
	// ���
	int				m_nItemHeight;
	// ѡ��������
	int				m_nSelectIndex;
	// ��ǰ������������
	int				m_nCurIndex;
	// ����ͼƬ
	CLKImage		*m_pBGImage;
	// ���б�
	LKTabItemArray	m_arrItems;
	// �������ָ���λ�û�ȡ������
	int GetItemIndexByPoint(CPoint point);
	// ���ݿɼ�������ȡָ���������
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
    // �������С
    void CalcItemSize();
};


