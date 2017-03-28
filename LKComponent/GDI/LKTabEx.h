#pragma once
#include "LKCtrlBase.h"
#include "LKTab.h"

// CLKTabEx

class CLKTabEx : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKTabEx)

public:
	CLKTabEx(int nImageIndex = 1);
	virtual ~CLKTabEx();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// ����һ��
	void AddItem(CString strText, int nImgIndex = -1, CWnd *pWnd = 0, void *pData = 0);
	void AddItem(CString strText, CLKImage *pImg, CWnd *pWnd, bool bMustFree, void *pData);
	void AddItem(CLKImage *pImg, CWnd *pWnd = 0, void *pData = 0);
	void SetItem(int nIndex, CString strText, CLKImage *pImg = 0, CWnd *pWnd = 0, void *pData = 0);
	void SetItem(int nIndex, CLKImage *pImg, CWnd *pWnd = 0, void *pData = 0);
	// ���ö���߾�
	void SetMargin(int nMargin){m_nMargin = nMargin;};
    // �����Ƿ��Զ�������ߴ�
    void SetIsAdjustItmeSize(bool bIsAdjustItmeSize = true){m_bIsAdjustItmeSize = bIsAdjustItmeSize;ReCalcItemSize();}
	// ѡ��һ��
	void SelectItem(int nIndex = 0){m_nSelectIndex = nIndex;}
	// ��ȡѡ��������
	int GetSelectedItem(){return m_nSelectIndex;}
    // �����Ƿ���Ҫ�رհ�ť
    void SetMustCloseButton(bool bMustCloseButton = true){ m_bMustCloseButton = bMustCloseButton;}
    // ɾ��һ��(����ʼ������ʼ)
    void DeleteItem(int nIndex);
    // ��ȡָ��������
    void *GetItemData(int nIndex);
    // ��ȡ������
    int GetCount(){return m_arrItems.GetCount();}
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    // ��ȡ�ؼ��ɷ�������
    CRect GetCtrlClientRect();
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
    // �Ƿ���Ҫ�رհ�ť
    bool            m_bMustCloseButton;
	// ����߾�
	int				m_nMargin;
	// ����
	int				m_nItemSplit;
	// ���
	int				m_nItemWidth;
    // ���
    int             m_nItemHeight;
	// ѡ��������
	int				m_nSelectIndex;
	// ��ǰ������������
	int				m_nCurIndex;
	// ��ǰ����¼�ͷ��ť������(-1 - ���ڼ�ͷ���� 0 - ���ͷ�� 1 - �Ҽ�ͷ)
	int				m_nCurArrIndex;
    // ���״̬
    bool            m_bMouseDown;
    // �������ɾ����ť��
    bool            m_bInCloseBtn;
    // ��ʾ�ɼ��������
    int             m_nVisibleBeginIndex;
    // ���ͷ�ɼ���
    bool            m_bVisibleLArr;
    // �Ҽ�ͷ�ɼ���
    bool            m_bVisibleRArr;
    // ���Ҽ�ͷ����
    CRect           m_rtArr;
	// ����ͼƬ
	CLKImage		*m_pBGImage;
    // �رհ�ťͼƬ
    CLKImage        *m_pCloseImg;
    // ���ͷ��ť
    CLKImage        *m_pLeftArrImg;
    // �Ҽ�ͷ��ť
    CLKImage        *m_pRightArrImg;
	// ���б�
	LKTabItemArray	m_arrItems;
	// �������ָ���λ�û�ȡ������
	int GetItemIndexByPoint(CPoint point);
	// ���ݿɼ�������ȡָ���������
	bool GetItemRectByIndex(int nIndex, LPRECT lpRect);
    // ���¼������С
    void ReCalcItemSize();
    // ����ָ�����С
    void CalcItemSize(LKTabItem *pItem);
    // �����ͷ�Ŀɼ���
    void CalcArrVisible();
protected:
//    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


