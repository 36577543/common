#pragma once
#include "GDICommon.h"


// CLKSlidWindow �Ի���

class CLKSlidWindow : public CDialog
{
	DECLARE_DYNAMIC(CLKSlidWindow)

public:
	CLKSlidWindow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLKSlidWindow();

    bool Create(CRect rtWnd);
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
    bool DoUpdateDummyDialog();
    // ����
    static void SlidUp(CWnd *pWnd, CWnd *pCur, CWnd *pPre);
    // ����
    static void SlidDown(CWnd *pWnd, CWnd *pCur, CWnd *pNext);
protected:

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
private:
    CWnd *m_pParent;
    void OnDraw(CDC *pDC);
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
