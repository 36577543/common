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

    // 设置两边窗口指针
    void SetTwoItem(CWnd *pFirst, CWnd *pSecond);
protected:
	DECLARE_MESSAGE_MAP()
private:
    // 是否是水平方向
    bool m_bHorizon;
    // 左或上
    CWnd *m_pFirst;
    // 右或下
    CWnd *m_pSecond;
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


