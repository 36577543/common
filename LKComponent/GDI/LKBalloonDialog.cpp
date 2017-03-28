// ..\common\lkcomponent\LKBalloonDialog.cpp : 实现文件
//

#include "LKBalloonDialog.h"
#include "LKFontMgr.h"
#include "LKImageMgr.h"

// 注销定时器
#define	LKBALLOONDLG_TIMER_DESTROY	1

// CLKBalloonDialog 对话框

IMPLEMENT_DYNAMIC(CLKBalloonDialog, CDialog)

void CLKBalloonDialog::Show(UINT nIDTemplate, CRect rt, CString strText, UINT nSecs, BOOL bBalloonUp)
{
	CLKBalloonDialog dlg(nIDTemplate);
	dlg.m_strText = strText;
	dlg.m_uDestroyTime = nSecs;
	dlg.m_ptDlgPos = rt.TopLeft();
	dlg.DoModal();
}

CLKBalloonDialog::CLKBalloonDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(nIDTemplate, pParent)
, m_pImgBG(0)
, m_uDestroyTime(0)
, m_colText(RGB(43, 85, 128))
, m_nLeftMargin(2)
, m_ptDlgPos(0, 0)
{
	//m_ftText.CreatePointFont(90,_T("Arial"));
	m_pImgBG = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_DIALOGBALLOON);
}

CLKBalloonDialog::~CLKBalloonDialog()
{
}

// 设置背景图片
//void CLKBalloonDialog::SetSkinBG(CString &strImg, DWORD nImageType)
//{
//	if(!m_imgBG.Load(strImg, nImageType))
//	{
//		// error log
//	}
//}
//
//void CLKBalloonDialog::SetSkinBG(HRSRC hImg, DWORD nImageType)
//{
//	if(!m_imgBG.LoadResource(hImg, nImageType, AfxGetApp()->m_hInstance))
//	{
//		// error log
//	}
//}
//
//void CLKBalloonDialog::SetSkinBG(CxImage *pImgBG)
//{
//	if(pImgBG)
//	{
//		if(pImgBG->IsValid())
//		{
//			m_imgBG = *pImgBG;
//		}
//	}
//}

// 获取提示字符所占长度
int CLKBalloonDialog::GetTextExtent()
{
	CPaintDC dc(this);
	dc.SelectObject(CLKFontMgr::GetST90());
	CSize sz = dc.GetOutputTextExtent(GetText());
	return sz.cx;
}

// 获取提示字符所占高度
int CLKBalloonDialog::GetTextHeight()
{
	CPaintDC dc(this);
	dc.SelectObject(CLKFontMgr::GetST90());
	CSize sz = dc.GetOutputTextExtent(GetText());
	return sz.cy;
}

BEGIN_MESSAGE_MAP(CLKBalloonDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()


// CLKBalloonDialog 消息处理程序

void CLKBalloonDialog::OnPaint()
{
	//CRect rtClient;
	//GetClientRect(&rtClient);
	CPaintDC dc(this); // device context for painting
	CRect	rt;
	GetClientRect(&rt);
	//dc.SetBkMode(TRANSPARENT);
    CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap Screen;
	Screen.CreateCompatibleBitmap(&dc, rt.Width(), rt.Height());
	CBitmap *pOld = dcMem.SelectObject(&Screen);
	dcMem.SetBkMode(TRANSPARENT);
	// 画背景框架
	DrawDlgBG(&dcMem, rt);
	// 画文本
	DrawInfoText(&dcMem, rt);
	dc.BitBlt(0, 0, rt.Width(), rt.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOld);
	Screen.DeleteObject();
	dcMem.DeleteDC();
}

HBRUSH CLKBalloonDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if(nCtlColor == CTLCOLOR_DLG)
    {
        return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
	return hbr;
}

void CLKBalloonDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(bShow)
	{
		OnShow();
		if(m_uDestroyTime)
		{
			SetTimer(LKBALLOONDLG_TIMER_DESTROY, m_uDestroyTime * 1000, 0);
		}
	}
	// TODO: 在此处添加消息处理程序代码
}

void CLKBalloonDialog::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch(nIDEvent)
	{
	// 注销
	case LKBALLOONDLG_TIMER_DESTROY:
		ReleaseCapture();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 私有函数
///////////////////////////////////////////////////////////////////////////////////////////////////

// 画窗体背景
void CLKBalloonDialog::DrawDlgBG(CDC *pDC, CRect rtClient)
{
	if(m_pImgBG->IsValid())
	{
		// 屏幕ＤＣ
		CWnd *pWnd = GetDesktopWindow();
		CDC *pWndDC = pWnd->GetDC();
		if(pWndDC && pWndDC->GetSafeHdc())
		{
			pDC->BitBlt(0, 0, rtClient.Width(), rtClient.Height(), pWndDC, m_ptDlgPos.x, m_ptDlgPos.y, SRCCOPY);
			pWnd->ReleaseDC(pWndDC);
		}
		m_pImgBG->DrawToDC(pDC, rtClient, 0, LKSKINBASE_DRAWBGSTYLE_ALL);
	}
}

// 画文本
void CLKBalloonDialog::DrawInfoText(CDC *pDC, CRect rtClient)
{
	if(m_strText.IsEmpty()) return;
	CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST90());
	pDC->SetTextColor(m_colText);
	rtClient.left += m_nLeftMargin;
	if(m_pImgBG->IsValid())
	{
		CRect rtImg = m_pImgBG->GetFrameRect();
		rtClient.top += rtImg.top - rtImg.bottom + 2;
	}
	pDC->DrawText(m_strText, rtClient, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	pDC->SelectObject(pOld);
}


void CLKBalloonDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnLBtnDown(nFlags, point);
	CDialog::OnLButtonDown(nFlags, point);
	ClientToScreen(&point);
	::mouse_event(MOUSEEVENTF_LEFTDOWN, point.x, point.y, 0, 0);
}

void CLKBalloonDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnRBtnDown(nFlags, point);
	CDialog::OnRButtonDown(nFlags, point);
}

void CLKBalloonDialog::OnCaptureChanged(CWnd *pWnd)
{
	CDialog::OnCaptureChanged(pWnd);
	OnOK();
}

BOOL CLKBalloonDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rt(m_ptDlgPos.x, m_ptDlgPos.y, 0, 0);
	int nLen = GetTextExtent();
	int nHeight = GetTextHeight();
	rt.right = rt.left + nLen + 11;
	rt.bottom = rt.top + nHeight + 15 + 3;
	MoveWindow(&rt);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
