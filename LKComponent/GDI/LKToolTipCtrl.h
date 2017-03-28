#pragma once
#include "GDICommon.h"
#include "LKImage.h"


// CLKToolTipCtrl

class CLKToolTipCtrl : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CLKToolTipCtrl)

public:
	CLKToolTipCtrl();
	virtual ~CLKToolTipCtrl();
public:
    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = 0);

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
//    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
    CLKImage *m_pImgBG;
    CLKImage *m_pImgOldBG;
    CRect    m_rtWnd;
    CRect    m_rtOldWnd;
    // »­×ÀÃæ±³¾°µ½CLKImageÖÐ
    void DrawDeskBGToImg(CLKImage *pImg, CRect rt);
    void RegeditToolTip();
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
//    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};


