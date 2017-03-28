// ..\..\..\common\LKComponent\GDI\LKSplit.cpp : 实现文件
//

#include "LKSplit.h"


// CLKSplit

IMPLEMENT_DYNAMIC(CLKSplit, CLKCtrlBase)

CLKSplit::CLKSplit(bool bHorizon)
: CLKCtrlBase(-1)
, m_bHorizon(bHorizon)
, m_pFirst(0)
, m_pSecond(0)
{

}

CLKSplit::~CLKSplit()
{
}

BOOL CLKSplit::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rect, pParentWnd, nID); 
	return bRet;
}

// 设置两边窗口指针
void CLKSplit::SetTwoItem(CWnd *pFirst, CWnd *pSecond)
{
    m_pFirst = pFirst;
    m_pSecond = pSecond;
}

BEGIN_MESSAGE_MAP(CLKSplit, CLKCtrlBase)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_SETCURSOR()
    ON_WM_CAPTURECHANGED()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CLKSplit 消息处理程序



void CLKSplit::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    SetCapture();
    CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKSplit::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    ReleaseCapture();
    CLKCtrlBase::OnLButtonUp(nFlags, point);
}

BOOL CLKSplit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if(m_bHorizon)
    {
	    ::SetCursor(::LoadCursor(NULL, IDC_SIZENS)); 
    }else
    {
	    ::SetCursor(::LoadCursor(NULL, IDC_SIZEWE)); 
    }
	return   TRUE; 
	//return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CLKSplit::OnCaptureChanged(CWnd *pWnd)
{
    // TODO: 在此处添加消息处理程序代码

    CLKCtrlBase::OnCaptureChanged(pWnd);
}

void CLKSplit::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if(::GetCapture() == GetSafeHwnd())
    {
        ::SendMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKSPLIT_MSG_DRAG, (LPARAM)this);
    }
    CLKCtrlBase::OnMouseMove(nFlags, point);
}
