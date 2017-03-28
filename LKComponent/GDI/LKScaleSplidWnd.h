#pragma once
#include "LKCommonCtrl.h"

// CLKScaleSplidWnd

class CLKScaleSplidWnd : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKScaleSplidWnd)

public:
	CLKScaleSplidWnd(int nImageIndex = 0, bool bLeft = false);
	virtual ~CLKScaleSplidWnd();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

    // �������ҵĴ���
    void SetLRWnd(CWnd *pLeft, CWnd *pRight){m_pLeftWnd = pLeft; m_pRightWnd = pRight;}
    // ��ȡ�󴰿�ָ��
    CWnd *GetLWnd(){return m_pLeftWnd;}
    // ��ȡ�Ҵ���ָ��
    CWnd *GetRWnd(){return m_pRightWnd;}
    // �Ƿ�չ��
    bool GetIsOpen();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
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
	// ���״̬
	EMState	m_emsCommon;
    // ��ǰ״̬(�Ƿ��������)
    bool    m_bLeft;
    // ��ߵĴ���
    CWnd    *m_pLeftWnd;
    // �ұߵĴ���
    CWnd    *m_pRightWnd;
    // ����ͼƬ�ĸ߶�
    int     m_nItemHeight;
    // ����ͼƬ�Ŀ��
    int     m_nItemWidth;
    // �򿪻�رմ���
    void OpenOrClose();
};


