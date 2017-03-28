#pragma once
#include "LKCtrlBase.h"


// CLKSplit

class CLKSplit : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKSplit)

public:
	CLKSplit(bool bHorizon = true);
	virtual ~CLKSplit();
    virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

    // �������ߴ���ָ��
    void SetTwoItem(CWnd *pFirst, CWnd *pSecond);
protected:
	DECLARE_MESSAGE_MAP()
private:
    // �Ƿ���ˮƽ����
    bool m_bHorizon;
    // �����
    CWnd *m_pFirst;
    // �һ���
    CWnd *m_pSecond;
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


