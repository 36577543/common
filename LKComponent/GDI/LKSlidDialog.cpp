// ..\..\common\LKComponent\GDI\LKSlidDialog.cpp : 实现文件
//

#include "LKSlidDialog.h"

// 检测隐藏状态定时器标识
#define TRIPDLG_TIMERID_CHECKHIDESTATE          1000
// 检测隐藏状态定时器时间间隔
#define TRIPDLG_TIMERINTERVAL_CHECKHIDESTATE    500


// CLKSlidDialog 对话框

IMPLEMENT_DYNAMIC(CLKSlidDialog, CLKDialog)

CLKSlidDialog::CLKSlidDialog(UINT nIDTemplate, CWnd* pParent)
: CLKDialog(nIDTemplate, pParent)
// 停靠状态
, m_dsmState(dsm_none)
// 允许隐藏
, m_bCanHide(true)
// 保存要绘制到动画显示的图片
, m_pAnimateImg(0)
// 皮肤改变
, m_bChangeSkin(false)
// 窗体停靠参数
, m_nStockType(WM_MAINDLG_STOCKMODE_NONE)
{

}

CLKSlidDialog::~CLKSlidDialog()
{
}


BEGIN_MESSAGE_MAP(CLKSlidDialog, CLKDialog)
    ON_WM_MOVING()
    ON_WM_TIMER()
    ON_WM_ACTIVATE()
    ON_WM_DESTROY()
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CLKSlidDialog 消息处理程序


void CLKSlidDialog::OpenMainPanel()
{
	if(!IsWindowVisible())
	{
		ShowWindow(SW_SHOW);
	}
	else if (IsIconic())
	{
		ShowWindow(SW_RESTORE);
	}
	else
	{
		if (dsm_none == m_dsmState)
        {
        }else
        {
            SlidOutMainDlg();
            SetFocus();
            KillTimer(TRIPDLG_TIMERID_CHECKHIDESTATE);
        }
    }
	SetForegroundWindow();
}


void CLKSlidDialog::OnMoving(UINT fwSide, LPRECT pRect)
{

    WINDOWPLACEMENT wndpl;
    GetWindowPlacement(&wndpl);
    if(wndpl.showCmd == SW_SHOWMAXIMIZED)
    {
        CLKDialog::OnMoving(fwSide, pRect);
        return;
    }

    ChangeStockType(m_nStockType);
    if(WM_MAINDLG_STOCKMODE_NONE == m_nStockType)
	{
        CLKDialog::OnMoving(fwSide, pRect);
        KillTimer(TRIPDLG_TIMERID_CHECKHIDESTATE);
		return;
	}
	m_dsmState = dsm_none;
	if(pRect->left <= 0)
	{
		((CRect *)pRect)->MoveToX(0);
		if(WM_MAINDLG_STOCKMODE_LTR == m_nStockType)
		{
			m_dsmState = dsm_left;
            // 设备检测定时器
            SetTimer(TRIPDLG_TIMERID_CHECKHIDESTATE, TRIPDLG_TIMERINTERVAL_CHECKHIDESTATE, 0);
		}
	}else if(pRect->top <= 0 )
	{
		((CRect *)pRect)->MoveToY(0);
		m_dsmState = dsm_top;
        // 设备检测定时器
        SetTimer(TRIPDLG_TIMERID_CHECKHIDESTATE, TRIPDLG_TIMERINTERVAL_CHECKHIDESTATE, 0);
	}else
    {
	    int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);

	    if(pRect->right >= nScreenX )
	    {
		    ((CRect *)pRect)->MoveToX(nScreenX - ((CRect *)pRect)->Width());
		    if(WM_MAINDLG_STOCKMODE_LTR == m_nStockType)
		    {
			    m_dsmState = dsm_right;
                // 设备检测定时器
                SetTimer(TRIPDLG_TIMERID_CHECKHIDESTATE, TRIPDLG_TIMERINTERVAL_CHECKHIDESTATE, 0);
		    }
	    }
    }
    CLKDialog::OnMoving(fwSide, pRect);
}

void CLKSlidDialog::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if(TRIPDLG_TIMERID_CHECKHIDESTATE == nIDEvent)
    {
        KillTimer(nIDEvent);
	    WINDOWPLACEMENT wndpl;
	    GetWindowPlacement(&wndpl);
	    if(SW_SHOWMAXIMIZED == wndpl.showCmd || SW_HIDE == wndpl.showCmd)
	    {
            SetTimer(nIDEvent, TRIPDLG_TIMERINTERVAL_CHECKHIDESTATE, 0);
            return;
        }
        CPoint pt;
        ::GetCursorPos(&pt);
        if(IsWindowVisible())
        {
            if(m_rtWnd.PtInRect(pt))
            {
                // 显示
                SlidOutMainDlg();
                //SetTimer(nIDEvent, TRIPDLG_TIMERINTERVAL_CHECKHIDESTATE, 0);
                //return;
            }else
            {
                // 隐藏
                HideMainDlg();
            }
        }
        if(dsm_none != m_dsmState)
        {
            SetTimer(nIDEvent, TRIPDLG_TIMERINTERVAL_CHECKHIDESTATE, 0);
        }
    }

    CLKDialog::OnTimer(nIDEvent);
}

// 隐藏主窗口
void CLKSlidDialog::HideMainDlg()
{
    if(GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) & WS_EX_TOPMOST)
    {
        return;
    }
    if(!m_bCanHide)
    {
        return;
    }
    // 过滤
    if(dsm_left == m_dsmState)
    {
        if(m_rtWnd.left != 0) return;
    }else if(dsm_top == m_dsmState)
    {
        if(CSystemInfo::GetSystemVer() > WINDOWS_XP)
        {
        }else
        {
            if(m_rtWnd.top != 0) return;
        }
    }else if(dsm_right == m_dsmState)
    {
        if(m_rtWnd.right != CURSCREEN_WIDTH) return;
    }
    // 保存DC
    if(m_pAnimateImg 
        && ((m_pAnimateImg->GetWidth() != m_rtWnd.Width() || m_pAnimateImg->GetHeight() != m_rtWnd.Height()) || m_bChangeSkin/**/))
    {
        delete m_pAnimateImg;
        m_pAnimateImg = 0;
    }
    if(!m_pAnimateImg)
    {
        m_bChangeSkin = false;
        m_pAnimateImg = new CLKImage(m_rtWnd.Width(), m_rtWnd.Height());
        SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
        ::UpdateWindow(GetSafeHwnd());
        CDC *pDC = GetWindowDC();
        //CDC *pDC = CDC::FromHandle(::GetDC(0));
        if(pDC)
        {
            CRect rtSrc(m_rtWnd);
            rtSrc.MoveToXY(0, 0);
            m_pAnimateImg->DrawFromDC(pDC, rtSrc);
            ReleaseDC(pDC);
        }
    }

    if(IsWindowVisible())
    {
        if(dsm_left == m_dsmState)
        {
            AnimateWindow(200, AW_HOR_NEGATIVE | AW_SLIDE | AW_HIDE);
            // 显示边界
            SetWindowPos(&wndTopMost, 2 - m_rtWnd.Width(), m_rtWnd.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
        }else if(dsm_top == m_dsmState)
        {
            AnimateWindow(200, AW_VER_NEGATIVE | AW_SLIDE | AW_HIDE);
            // 显示边界
            SetWindowPos(&wndTopMost, m_rtWnd.left, 2 - m_rtWnd.Height(), 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
        }else if(dsm_right == m_dsmState)
        {
            AnimateWindow(200, AW_HOR_POSITIVE | AW_SLIDE | AW_HIDE);
            // 显示边界
            SetWindowPos(&wndTopMost, CURSCREEN_WIDTH - 2, m_rtWnd.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
        }
    }
    // ShowWindow(SW_HIDE);
}

// 显示
void CLKSlidDialog::SlidOutMainDlg()
{
    if(GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) & WS_EX_TOPMOST)
    {
        //ShowWindow(SW_HIDE);
        if(dsm_left == m_dsmState)
        {
            SetWindowPos(&wndNoTopMost, 0, m_rtWnd.top, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOZORDER);
            AnimateWindow(200, AW_HOR_POSITIVE | AW_SLIDE);
            // 显示边界
            SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
        }else if(dsm_top == m_dsmState)
        {
            SetWindowPos(&wndNoTopMost, m_rtWnd.left, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOZORDER);
            AnimateWindow(200, AW_VER_POSITIVE | AW_SLIDE);
            // 显示边界
            SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
        }else if(dsm_right == m_dsmState)
        {
            SetWindowPos(&wndNoTopMost, CURSCREEN_WIDTH - m_rtWnd.Width(), m_rtWnd.top, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOZORDER);
            AnimateWindow(200, AW_HOR_NEGATIVE | AW_SLIDE);
            // 显示边界
            SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
        }
    }
}


void CLKSlidDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CLKDialog::OnActivate(nState, pWndOther, bMinimized);

    if(WA_INACTIVE == nState)
    {
        if(dsm_none != m_dsmState)
        {
            ChangeStockType(m_nStockType);
	        if(WM_MAINDLG_STOCKMODE_NONE != m_nStockType)
	        {
                // 设备检测定时器
                SetTimer(TRIPDLG_TIMERID_CHECKHIDESTATE, TRIPDLG_TIMERINTERVAL_CHECKHIDESTATE, 0);
            }
        }
    }
}

void CLKSlidDialog::OnDestroy()
{
    CLKDialog::OnDestroy();
    if(m_pAnimateImg)
    {
        delete m_pAnimateImg;
        m_pAnimateImg = 0;
    }
}

// 处理窗口wm_print消息
void CLKSlidDialog::OnPrint(CDC *pDC)
{
    if(m_pAnimateImg && m_pAnimateImg->IsValid())
    {
        m_pAnimateImg->Draw(pDC->GetSafeHdc(), 0, 0);
    }
}


void CLKSlidDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CLKDialog::OnShowWindow(bShow, nStatus);

    if(bShow)
    {
        if(dsm_none != m_dsmState)
        {
            // 设备检测定时器
            SetTimer(TRIPDLG_TIMERID_CHECKHIDESTATE, TRIPDLG_TIMERINTERVAL_CHECKHIDESTATE, 0);
        }
    }else
    {
        KillTimer(TRIPDLG_TIMERID_CHECKHIDESTATE);
    }
    // TODO: 在此处添加消息处理程序代码
}
