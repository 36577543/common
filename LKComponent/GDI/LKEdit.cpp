// ..\..\common\LKComponent\GDI\LKEdit.cpp : 实现文件
//
#include "LKEdit.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"
#include "LKBalloonWnd.h"

// CLKEdit

IMPLEMENT_DYNAMIC(CLKEdit, CLKCommonCtrl)

LRESULT CALLBACK CLKEdit::EditWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	CLKEdit *pThis = (CLKEdit *)GetWindowLong(hWnd, GWL_USERDATA);
	if(msg == WM_MOUSELEAVE)
	{
		pThis->OnMouseLeave();
		//if(pThis->GetMemImg() != pThis->m_pNormalImg)
		//{
		//	CPoint point;
		//	GetCursorPos(&point);
		//	//pThis->ScreenToClient(&point);
		//	CRect rt;
		//	pThis->GetWindowRect(&rt);
		//	if (!rt.PtInRect(point))
		//	{
		//		pThis->SetMemImg(pThis->m_pNormalImg);
		//		//return;
		//	}
		//	if(0 == pThis->m_pNormalImg)
		//	{
		//		pThis->GetParent()->ScreenToClient(&rt);
		//		pThis->GetParent()->InvalidateRect(&rt, false);
		//	}else
		//	{
		//		pThis->SendMessage(WM_NCPAINT, 1);
		//	}
		//}
	}else if(msg == WM_MOUSEMOVE)
	{
		if(pThis->m_pHighLightImg && pThis->GetMemImg() == pThis->m_pNormalImg)
		{
			pThis->SetMemImg(pThis->m_pHighLightImg);
			TRACKMOUSEEVENT etMouse;
			etMouse.cbSize = sizeof(TRACKMOUSEEVENT);   
			etMouse.dwFlags = TME_LEAVE | TME_HOVER;   
			etMouse.dwHoverTime = NULL;   
			etMouse.hwndTrack = hWnd;  
			_TrackMouseEvent(&etMouse);   
			pThis->SendMessage(WM_NCPAINT, 1);
		}
	}else if(WM_SETFOCUS == msg)
	{
		if(!pThis->m_strHint.IsEmpty())
		{
			CString strText;
            pThis->m_bGetPass = true;
			pThis->m_ctlEdit.GetWindowText(strText);
            pThis->m_bGetPass = false;
			if(strText.IsEmpty())
			{
				pThis->m_ctlEdit.SetWindowText(strText);
			}
		}
	}else if(WM_KEYDOWN == msg)
	{
		if(wparam == VK_CAPITAL)
		{
			if(GetKeyState(VK_CAPITAL) & 0x1)
			{
				// 大写键锁定
			}else
			{
				// 释放
				CLKBalloonWnd::Hide();
				//::PostMessage(pThis->GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKEDIT_MSG_WPARAM_CAPSUNLK, (LPARAM)pThis);
			}
		}
		::UpdateWindow(hWnd);
	}else if(WM_KILLFOCUS == msg)
	{
		if(!pThis->m_strHint.IsEmpty())
		{
			CString strText;
            pThis->m_bGetPass = true;
			pThis->m_ctlEdit.GetWindowText(strText);
            pThis->m_bGetPass = false;
			if(strText.IsEmpty())
			{
				pThis->m_ctlEdit.SetWindowText(pThis->m_strHint);
			}
		}
	}else if(WM_GETTEXT == msg)
	{
        {
			CEdit *pEdit = (CEdit *)CEdit::FromHandle(hWnd);
			if((ES_PASSWORD & pThis->m_uEditStyle)
                && !pThis->m_bGetPass)
			{
			    //LRESULT lRet = CallWindowProc(pThis->m_pOldEditProc, hWnd, msg, wparam, lparam);
			    return 0;
            }
        }
		if(!pThis->m_strHint.IsEmpty())
		{
			LRESULT lRet = CallWindowProc(pThis->m_pOldEditProc, hWnd, msg, wparam, lparam);
			TCHAR *p = (TCHAR *)lparam;
			CString s = p;
			if(s.Compare(pThis->m_strHint) == 0)
			{
				memset((char *)lparam, 0, s.GetLength() * sizeof(TCHAR));
			}
			//static int n(0);
			//TRACE(_T("WM_GETTEXT: %d\r\n"), n++);
			return lRet;
		}
	}else if(WM_GETTEXTLENGTH == msg)
	{
        {
			CEdit *pEdit = (CEdit *)CEdit::FromHandle(hWnd);
			if((ES_PASSWORD & pThis->m_uEditStyle)
                && !pThis->m_bGetPass)
			{
			    //LRESULT lRet = CallWindowProc(pThis->m_pOldEditProc, hWnd, msg, wparam, lparam);
			    return 0;
            }
        }
		if(!pThis->m_strHint.IsEmpty())
		{
			LRESULT lRet = CallWindowProc(pThis->m_pOldEditProc, hWnd, msg, wparam, lparam);
			TCHAR *p = (TCHAR *)lparam;
			CString s = p;
			if(s.Compare(pThis->m_strHint) == 0)
			{
				lRet = 0;
			}
			return lRet;
		}
	}else if(WM_SETTEXT == msg)
	{
		if(!pThis->m_strHint.IsEmpty())
		{
			CEdit *pEdit = (CEdit *)CEdit::FromHandle(hWnd);
			if(ES_PASSWORD & pThis->m_uEditStyle)
			{
				TCHAR *p = (TCHAR *)lparam;
				CString s(p);
				if(s.Compare(pThis->m_strHint) == 0)
				{
					pEdit->SetPasswordChar(0);
					//pEdit->SetFont(CLKFontMgr::GetST90());
				}else
				{
					pEdit->SetPasswordChar(pThis->m_cPassChar);
					//pEdit->SetFont(CLKFontMgr::GetMSS80());
				}
			}
		}
	}else if(EM_SHOWBALLOONTIP == msg)
	{
		::PostMessage(pThis->GetSafeHwnd(), LKCOMMON_MSG, LKEDIT_MSG_WPARAM_CAPSLK, (LPARAM)pThis);
		//static int n = 0;
		//TRACE(_T("balloon: %d\r\n"), n++);
		// 去掉默认的提示信息。
		return 1;
	}
	return ::CallWindowProc(pThis->m_pOldEditProc, hWnd, msg, wparam, lparam);
}


CLKEdit::CLKEdit(int nImageIndex)
: CLKCommonCtrl(nImageIndex)
, m_colHint(RGB(160, 160, 160))
, m_uEditStyle(0)
, m_cPassChar(_T('*'))
, m_bGetPass(false)
{
	m_rtBtn = CRect(0, 0, 0, 0);
	m_rtClientOffset = CRect(6, 7, 6, 5);
	m_rtClientEdge = CRect(6, 7, 6, 5);
	m_colBack = CLKFontMgr::GetWhiteTextColor();
}

CLKEdit::~CLKEdit()
{
}

BOOL CLKEdit::CreatePass(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_uEditStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD | ES_WANTRETURN; 
	BOOL bRet = CLKCommonCtrl::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKEdit::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCommonCtrl::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKEdit::Create(UINT uStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_uEditStyle = uStyle;
	BOOL bRet = CLKCommonCtrl::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKEdit::CreateHide(UINT uStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_uEditStyle = uStyle;
	BOOL bRet = CLKCommonCtrl::Create(_T(""), WS_CHILD | WS_TABSTOP, rect, pParentWnd, nID); 
	return bRet;
}
// 获取密码
CString CLKEdit::GetPassword()
{
    CString strPass;
    m_bGetPass = true;
    m_ctlEdit.GetWindowText(strPass);
    m_bGetPass = false;
    return strPass;
}

// 设置提示信息
void CLKEdit::SetHintText(CString strHint)
{
	m_strHint = strHint;
	if(GetSafeHwnd())
	{
		CString strText;
        m_bGetPass = true;
		GetWindowText(strText);
        m_bGetPass = false;
		if(strText.IsEmpty() && !m_strHint.IsEmpty())
		{
			m_ctlEdit.SetWindowText(m_strHint);
		}
	}
}
// 设置文本
void CLKEdit::SetWindowText(LPCTSTR lpszString)
{
	m_ctlEdit.SetWindowText(lpszString);
}

BEGIN_MESSAGE_MAP(CLKEdit, CLKCommonCtrl)
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(LKEDIT_CHILDID_EDIT, OnEnChangeEdit)
	ON_MESSAGE(LKCOMMON_MSG, &CLKEdit::OnCommonMsg)
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CLKEdit 消息处理程序
LRESULT CLKEdit::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKEDIT_MSG_WPARAM_CAPSLK == wParam)
	{
		CRect rt;
		GetWindowRect(&rt);
		rt.OffsetRect(4, rt.Height() - 6);
		//pThis->ClientToScreen(&rt);
		CLKBalloonWnd::Show(GetParent(), rt, _T("大写锁定已打开"));
	}else
	{
		return CLKCommonCtrl::OnCommonMsg(wParam, lParam);
	}
	return 1;
}

// 文本发生改变
void CLKEdit::OnEnChangeEdit()
{
	::SendMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), EN_CHANGE), (LPARAM)GetSafeHwnd());
	m_ctlEdit.Invalidate();
}

// 初始化图片列表
void CLKEdit::InitImageList()
{
	// 初始化图片列表
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_EDIT_BEGIN, LKIMAGELISTINDEX_EDIT_END);
}

// 解析出相关图片到内存缓存中
void CLKEdit::ParseImageToMem()
{
	m_bImgIsOwner = true;
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		// 
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);
		int nNumber = pImg->GetImageNumber();

		CRect rtWnd;
		GetWindowRect(&rtWnd);
		rtWnd.MoveToXY(0, 0);
		if(m_pNormalImg) delete m_pNormalImg;
		m_pNormalImg = new CLKImage(rtWnd.Width(), rtWnd.Height());
		pImg->DrawToImage(m_pNormalImg, 0, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
		if (m_pBtnImg)
		{
			CRect rtSrc(0, 2, m_pBtnImg->GetWidth(), m_pBtnImg->GetHeight() / m_pBtnImg->GetImageNumber() - 2);
			m_rtBtn = CRect(rtWnd.right - m_pBtnImg->GetWidth() - 1, 2, rtWnd.right - 1, rtWnd.bottom - 2/* - m_rtClient.bottom*/);
			m_pBtnImg->DrawToImage(m_pNormalImg, m_rtBtn, rtSrc);
		}
		if(m_pHighLightImg) delete m_pHighLightImg;
		m_pHighLightImg = new CLKImage(rtWnd.Width(), rtWnd.Height());
		pImg->DrawToImage(m_pHighLightImg, 1, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
		if (m_pBtnImg)
		{
			CRect rtSrc(0, 2, m_pBtnImg->GetWidth(), m_pBtnImg->GetHeight() / m_pBtnImg->GetImageNumber() - 2);
			m_rtBtn = CRect(rtWnd.right - m_pBtnImg->GetWidth() - 1, 3, rtWnd.right - 1, rtWnd.bottom - 3/* - m_rtClient.bottom*/);
			m_pBtnImg->DrawToImage(m_pHighLightImg, m_rtBtn, rtSrc);
		}

		SetMemImg(m_pNormalImg);
	}
}

// 创建控件后初始化
void CLKEdit::InitCtrlAfterCreate()
{
	CLKCommonCtrl::InitCtrlAfterCreate();
	if(!m_uEditStyle)
	{
		m_uEditStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL;
	}
	CRect rtEdit = m_rtClient;
	rtEdit.DeflateRect(&m_rtClientEdge);
	m_ctlEdit.Create(m_uEditStyle, rtEdit, this, LKEDIT_CHILDID_EDIT);
	//GetWindowRect(&m_rtClient);
	//m_rtClient.MoveToXY(0, 0);
	SetWindowLong(m_ctlEdit.GetSafeHwnd(), GWL_USERDATA,(LONG)this);
	m_pOldEditProc = (WNDPROC)SetWindowLong(m_ctlEdit.GetSafeHwnd(), GWL_WNDPROC,(LONG)EditWndProc);
	if (!m_pFtText)
	{
		m_pFtText = CLKFontMgr::GetST90();
	}
	m_ctlEdit.SetFont(m_pFtText, true);
	if(!m_strHint.IsEmpty())
	{
		SetHintText(m_strHint);
	}
}

void CLKEdit::OnPaint(CDC *pDC)
{
	if (m_pBtnImg)
	{
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		rtWnd.MoveToXY(0, 0);
		CRect rtSrc(0, 2, m_pBtnImg->GetWidth(), m_pBtnImg->GetHeight() / m_pBtnImg->GetImageNumber() - 2);
		if (m_bInBtn)
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0xFF00)
			{
				rtSrc.OffsetRect(0, 2 * m_pBtnImg->GetHeight() / m_pBtnImg->GetImageNumber());
			}
			else
			{
				rtSrc.OffsetRect(0, m_pBtnImg->GetHeight() / m_pBtnImg->GetImageNumber());
			}
		}
		m_rtBtn = CRect(rtWnd.right - m_pBtnImg->GetWidth() - 1, 3, rtWnd.right - 1, rtWnd.bottom - 3/* - m_rtClient.bottom*/);
		m_pBtnImg->Draw(pDC->GetSafeHdc(), m_rtBtn, rtSrc);
	}

}

//
void CLKEdit::OnSetFocus(CWnd* pOldWnd)
{
	CLKCommonCtrl::OnSetFocus(pOldWnd);
	m_ctlEdit.SetFocus();
	// TODO: 在此处添加消息处理程序代码
}

HBRUSH CLKEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// 当编辑框为read_only   或disable时，在WM_CTLCOLOR中相当于CTLCOLOR_STATIC   ==   nCtlColor
	if(CTLCOLOR_EDIT == nCtlColor || pWnd == &m_ctlEdit)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_crText);
		if(!m_strHint.IsEmpty())
		{
			CString strText;
            m_bGetPass = true;
			pWnd->GetWindowText(strText);
            m_bGetPass = false;
			if(strText.IsEmpty())
			{
				pDC->SetTextColor(m_colHint);
			}
		}
		//return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	if (!m_brBack.GetSafeHandle())
	{
		m_brBack.CreateSolidBrush(m_colBack);
	}
	return (HBRUSH)m_brBack;
	//HBRUSH hbr = CLKCommonCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	//return hbr;
}

void CLKEdit::PreSubclassWindow()
{
	//InitImageList();
	CLKCommonCtrl::PreSubclassWindow();
	//InitCtrlAfterCreate();
}

LRESULT CLKEdit::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//LRESULT lr = CLKCommonCtrl::OnNcHitTest(point);
	return HTCLIENT;
	//return HTCAPTION;
}

void CLKEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	//ScreenToClient(&point);
	if (!m_rtBtn.IsRectNull())
	{
		if (m_rtBtn.PtInRect(point))
		{
			if (!m_bInBtn)
			{
				m_bInBtn = true;
				Invalidate();
			}
		}
		else
		{
			if (m_bInBtn)
			{
				m_bInBtn = false;
				Invalidate();
			}
		}
		//return;
	}

	CLKCommonCtrl::OnMouseMove(nFlags, point);
}

void CLKEdit::OnMouseLeave()
{
	//MessageBeep(5);
	//return CLKCtrlBase::OnMouseLeave();
	if (m_bInBtn)
	{
		m_bInBtn = false;
		Invalidate();
	}

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	CRect rtEdit = m_rtClient;
	rtEdit.DeflateRect(&m_rtClientEdge);
	if (rtEdit.PtInRect(point))
	{
		return;
	}
	CLKCommonCtrl::OnMouseLeave();
}


void CLKEdit::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CLKCommonCtrl::OnMouseHover(nFlags, point);
}


void CLKEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//Invalidate();

	CLKCommonCtrl::OnLButtonDown(nFlags, point);
}


void CLKEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());

	CLKCommonCtrl::OnLButtonUp(nFlags, point);
}
