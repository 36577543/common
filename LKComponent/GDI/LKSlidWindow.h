#pragma once
#include "GDICommon.h"


// CLKSlidWindow 对话框

class CLKSlidWindow : public CDialog
{
	DECLARE_DYNAMIC(CLKSlidWindow)

public:
	CLKSlidWindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLKSlidWindow();

    bool Create(CRect rtWnd);
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
    bool DoUpdateDummyDialog();
    // 滑上
    static void SlidUp(CWnd *pWnd, CWnd *pCur, CWnd *pPre);
    // 滑下
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
