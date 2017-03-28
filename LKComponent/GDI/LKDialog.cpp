// ..\..\common\LKComponent\GDI\LKDialog.cpp : ʵ���ļ�
//

//#include "stdafx.h"
#include "LKDialog.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"
#include "math.h"


// CLKDialog �Ի���
// ͨ�ô�ͼ��
HICON CLKDialog::m_hCommonIcon = 0;
// ͨ��Сͼ��
HICON CLKDialog::m_hCommonSmallIcon = 0;

IMPLEMENT_DYNAMIC(CLKDialog, CDialogEx)

CLKDialog::CLKDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CDialogEx(nIDTemplate, pParent)
, m_eMouseNCPos(mnp_unknow)
, m_eMouseNCPressPos(mnp_unknow)
, m_ptTitle(25, 4)
, m_nBordWidth(1)
, m_nCloseBtnRight(0)
, m_ptMinWidthHeight(0, 0)
// �ı���ɫ
, m_crText(RGB(43, 85, 128))
// �ͻ���ƫ����
, m_rtClientOffset(2, 29, 2, DEFAULT_STATUSBAR_HEIGHT)
, m_nUsedWidth(2)
, m_pMemBmp(0)
, m_pBGBmp(0)
, m_bIsOwnerBGBmp(false)
// ���
, m_bIsMax(false)
, m_bMinSize(250, 600)
, m_pToolTip(0)
, m_pChildToolTip(0)
{
	m_ftText.CreatePointFont(90,_T("����"));
    CalcSysRect();
}

CLKDialog::~CLKDialog()
{
	if(m_bIsOwnerBGBmp)
	{
		delete m_pBGBmp;
	}

	if(m_pMemBmp)
	{
		delete m_pMemBmp;
	}
}

BOOL CLKDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    if(m_hCommonIcon)
    {
	    SetIcon(m_hCommonIcon, TRUE);			// ���ô�ͼ��
    }
    if(m_hCommonSmallIcon)
    {
	    SetIcon(m_hCommonSmallIcon, FALSE);		// ����Сͼ��
    }
    return TRUE; 
}

// ���к���

// ���ñ�������
void CLKDialog::SetBGBmp(CLKImage *pBGBmp)
{
	FreeBGImage();
	m_pBGBmp = pBGBmp;
	if(IsWindowVisible())
	{
		Invalidate(true);
	}
}

// �ı䱳��
void CLKDialog::ChangeBGImg(CLKImage *pImg)
{
	if(pImg && pImg->IsValid() && m_pBGBmp && m_pBGBmp->IsValid())
	{
		if(pImg->GetWidth() < SCREEN_WIDTH || pImg->GetHeight() < SCREEN_HEIGHT)
		{
            double fRateX = (double)SCREEN_WIDTH / pImg->GetWidth();
            double fRateY = (double)SCREEN_HEIGHT / pImg->GetHeight();
            //float fRate = fRateX > fRateY ? fRateX : fRateY;
			CRect rtDest(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            if(fRateX > fRateY)
            {
                rtDest.right = SCREEN_WIDTH;
                rtDest.bottom = (LONG)(pImg->GetHeight() * fRateX);
            }else
            {
                rtDest.bottom = SCREEN_HEIGHT;
                rtDest.right = (LONG)(pImg->GetWidth() * fRateY);
            }
			//pImg->DrawToImageByBICUBIC2(m_pBGBmp, rtDest);
			//pImg->DrawToImage(m_pBGBmp, rtDest);
            pImg->StretchBilinear(m_pBGBmp, rtDest.Width(), rtDest.Height(), pImg->GetWidth(), pImg->GetHeight());
            // ��������
            //pImg->StretchToImage(m_pBGBmp);
		}else
		{
			pImg->DrawToImage(m_pBGBmp);
		}
	}
	// �ػ�
	DrawNClientFrame();
}

// CLKDialog ��Ϣ�������

LRESULT CLKDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCPAINT || message == WM_NCACTIVATE)
	{
		CDC* pWinDC = GetWindowDC();
		if (pWinDC)
		{
			DWORD dw = ::GetTickCount();
			pWinDC->ExcludeClipRect(m_rtClientOffset.left, m_rtClientOffset.top, m_rtWnd.Width() - m_rtClientOffset.right, m_rtWnd.Height() - m_rtClientOffset.bottom);
			int bFocuse(true);
            if(message == WM_NCACTIVATE && !wParam)
            {
                bFocuse = false;
            }else if (message == WM_NCPAINT && GetForegroundWindow() != this)
            {
                bFocuse = false;
            }
            DrawNClientArea(pWinDC, 0 == bFocuse ? false : true);

			#ifdef LKDIALOG_NCPAINT_LOG
				static int i = 0;
				TRACE("dialog ncpaint:  %d time: %ds\r\n", i++, ::GetTickCount() - dw);
			#endif
		}
		ReleaseDC(pWinDC);
		return 1;
	}else if(message == WM_NCCALCSIZE)
    {
        if(wParam) OnNcCalcSize((BOOL)wParam, (NCCALCSIZE_PARAMS*)lParam);
		return 1;
    }else if(message == WM_MOVE)
    {
		// ���¼����ͼ��(��Ӧ�����Ϣ��)λ��
	    GetWindowRect(&m_rtWnd);
    }else if(WM_MOVING == message)
    {
        if(IsZoomed())
        {
            CRect *pRect = (CRect *)lParam;
            //RECT   m_rect;
            pRect-> bottom=m_rtWnd.bottom;
            pRect-> left=m_rtWnd.left;
            pRect-> right=m_rtWnd.right;
            pRect-> top=m_rtWnd.top;
        }
    }else if(message == 0x00AE ||  message == 0x00AF)
	{
		//return WM_NCPAINT;
		return 0;
	}else if(message == WM_PAINT || message == WM_ERASEBKGND/**/)
	{
		if(message == WM_PAINT)
		{
			// ע�⣺��Ӧ����Ϣ���Ƿ��أ�����ָ�����ز�Ҫ���˻����Ĭ�ϴ���������������ע��������Ĵ���Ϣ��Ӧ������ˢ�½����Ӱ�죮
			DWORD dw = ::GetTickCount();
			PAINTSTRUCT paint;
			CDC *pDC = BeginPaint(&paint);
			pDC->IntersectClipRect(paint.rcPaint.left, paint.rcPaint.top, paint.rcPaint.right, paint.rcPaint.bottom);
			int nWidth = paint.rcPaint.right - paint.rcPaint.left;
			int nHeight = paint.rcPaint.bottom - paint.rcPaint.top;
			int nLeft = paint.rcPaint.left + m_rtClientOffset.left;
			int nTop = paint.rcPaint.top + m_rtClientOffset.top;
			m_pMemBmp->BitBlt(pDC->GetSafeHdc(), paint.rcPaint.left, paint.rcPaint.top, nWidth, nHeight,  nLeft, nTop);
			if (m_rtClientOffset.top < 20)
			{
				DrawTitleBarIcon(pDC, false);
			}

			OnPaint(pDC);
			EndPaint(&paint);
			ATLTRACE("dialog nWidth: %d, nHeight: %d\r\n", nWidth, nHeight);
			#ifdef LKDIALOG_PAINT_LOG
				static int n = 0;
				TRACE("dialog:  %d time: %ds\r\n", n++, ::GetTickCount() - dw);
			#endif
			if(CSystemInfo::GetSystemVer() > WINDOWS_XP)
			{
				CWnd *pWnd = GetWindow(GW_CHILD);
				while(pWnd)
				{
					pWnd->SendMessage(WM_NCPAINT, 1);
					pWnd = pWnd->GetWindow(GW_HWNDNEXT);
				}
			}
		}
		return 1;
	//}else if(message == WM_MOUSEMOVE)
	//{
	//	if (m_eMouseNCPressPos == mnp_top)
	//	{
	//		if(GetTickCount() - m_dwMouseDrag > 10)
	//		{
	//			CRect rtW;
	//			CPoint point((DWORD)lParam);
	//			GetWindowRect(&rtW);
	//			ClientToScreen(&point);
 //               if(rtW.top != point.y && (m_rtWnd.Height() > m_bMinSize.y || rtW.top > point.y))
	//			{
	//				rtW.top = point.y;
	//				if(m_ptMinWidthHeight.y > 0 && rtW.Height() < m_ptMinWidthHeight.y)
	//				{
	//					rtW.top = rtW.bottom - m_ptMinWidthHeight.y;
	//				}
	//				::LockWindowUpdate(::GetDesktopWindow()); 
	//				MoveWindow(&rtW);
	//				UnlockWindowUpdate();
	//				m_dwMouseDrag = GetTickCount();
	//			}
	//		}
	//		return 1;
 //       }
	//}else if(LKSKINMENUDLG_MSG_BASE <= message && LKSKINMENUDLG_MSG_END >= message)
	//{
	//	// �˵���Ϣ
	//	OnMenuItemSelected(wParam);
	//	return 1;
	}else if(WM_WINDOWPOSCHANGING == message)
	{
        PWINDOWPOS p = (PWINDOWPOS)lParam;
        //if(SWP_FRAMECHANGED & p->flags)
        {
            // λ�øı�
            //CRect rtWnd(p->x, p->y, p->x + p->cx, p->y + p->cy);
            OnWindowPosChange(p);
        }
		if(/*SWP_STATECHANGED*/0x8000 & p->flags)
		{
        }
	}else if(message == WM_SIZE)
    {
		// ���¼����ͼ��(��Ӧ�����Ϣ��)λ��
		if(GetSafeHwnd())
		{
			GetWindowRect(&m_rtWnd);
			if(wParam == SIZE_MINIMIZED)
			{
				//return 1;
				//ShowWindow(SW_HIDE);
			}else if(m_rtWnd.right > 0 && m_rtWnd.bottom > 0 && GetSafeHwnd())
			{
				if(wParam == SIZE_MAXIMIZED)
				{
					m_bIsMax = true;
				}else if(m_bIsMax)
				{
					m_bIsMax = false;
				}
				// ����رհ�ťλ��
				CalcCloseRect();
				// ������󻯰�ťλ��
				CalcMaxRect();
				// ������С����ťλ��
				CalcMinRect();
				// �����Զ��尴ťλ��
				CalcSelfMenuRect();
				// ����״̬��λ��
				CalcStatusbarRect();
				// �����С�ı�
				DrawNClientFrame();
				// ��Բ��
				CRgn rgn;
				CRect rc(m_rtWnd);
				rc -= rc.TopLeft();
				rgn.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //����һ��Բ�Ǿ���
				ModiDialogRectRgn(rgn);
				SetWindowRgn(rgn,TRUE); 
			}
		}
	}else if(LKCOMMON_MSG == message)
	{
		return OnCommonMsg(wParam, lParam);
	}else if(WM_CAPTURECHANGED == message)
	{
		m_eMouseNCPressPos = mnp_unknow;
		m_eMouseNCPos = mnp_unknow;
		//PostMessage(WM_NCMOUSELEAVE, 0, 0);
	}else if(WM_PRINT == message)
    {
        //static int n(0);
        //TRACE(_T("WM_PRINT: %d\r\n"), n++);
        CDC *pDC = CDC::FromHandle((HDC)wParam);
        OnPrint(pDC);
    }
	return CDialogEx::WindowProc(message, wParam, lParam);
}

// ������Ϣ
LRESULT CLKDialog::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message >= WM_NCHITTEST && message < WM_NCMBUTTONDBLCLK)
	{
		// ����ǿͻ�������������
		if (message == WM_NCLBUTTONDOWN)
		{
			CPoint point((DWORD)lParam);
			if(OnNCLMouseDownDef(wParam, point)) 
			{
				//SendMessage(WM_NCPAINT, 0, 0);
				return 0;
			}
		}
		// ����ǿͻ�������Ҽ�����
		if (message == WM_NCRBUTTONDOWN)
		{
			CPoint point((DWORD)lParam);
			if(OnNCRMouseDownDef(wParam, point)) 
			{
				return 0;
			}
		}
		// ����ǿͻ�������ƶ�
		if (message == WM_NCMOUSEMOVE)
		{
			CPoint point((DWORD)lParam);
			if(OnNCMouseMoveDef(wParam, point))
			{
				return 0;
			}
		}
		if(message == WM_NCHITTEST)
		{
			LRESULT lr;// = CDialogEx::DefWindowProc(message, wParam, lParam);
			CPoint pt;
			pt.x = GET_X_LPARAM(lParam); 
			pt.y = GET_Y_LPARAM(lParam);
			CRect rtWindow(m_rtWnd);
			pt.Offset(-rtWindow.left, -rtWindow.top);
			
			if(OnHitTest(pt, lr))
			{
				//
			//}else if(m_rtCloseWnd.PtInRect(pt))
			//{
			//	lr = HTCLOSE;
			}else if(m_rtMaxWnd.PtInRect(pt))
			{
				lr = HTMAXBUTTON;
			}else if(m_rtMinWnd.PtInRect(pt))
			{
				lr = HTMINBUTTON;
			}else if(m_rtMenuWnd.PtInRect(pt))
			{
				lr = HTSYSMENU;
			}else if(IsInOther(pt))
			{
				lr = HTOBJECT;
			}else
			{
				rtWindow.MoveToXY(0, 0);
				rtWindow.DeflateRect(2, 2, 2, 4);
				if(rtWindow.PtInRect(pt))
				{
					lr = HTCAPTION;
				}else
				{
					lr = CDialogEx::DefWindowProc(message, wParam, lParam);
				}
			}
			return lr;
		}else if(WM_NCLBUTTONDBLCLK == message)
		{
			if(m_rtMaxWnd.Width() > 0 && m_rtMaxWnd.left < m_rtCloseWnd.left)
			{
				CPoint pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				CRect rtWindow(m_rtWnd);
				pt.Offset(-rtWindow.left, -rtWindow.top);
				if (!IsInMenubox(pt))
				{
					OnMaxClick();
				}
			}
			return 1;
		}
	}else if (message == WM_LBUTTONUP)
	{
		// �����������ͷ� 
		CPoint point((DWORD)lParam);
		if(OnNCLMouseUpDef(wParam, point)) 
		{
			return 1;
			//SendMessage(WM_NCPAINT, 0, 0);
		}
	}else if (message == WM_NCMOUSELEAVE)
	{
		// ����ǿͻ�������뿪
		if(mnp_unknow == m_eMouseNCPressPos)
		{
            DrawNCIcon(0, m_eMouseNCPos, bs_normal, true);
			m_eMouseNCPos = mnp_unknow;
		}
		return 1;
	}else if(WM_GETMINMAXINFO == message)
	{
		MINMAXINFO* lpMMI = (MINMAXINFO *)lParam;
		OnSetMinMaxInfo(lpMMI);
		return 1;
	}
	return CDialogEx::DefWindowProc(message, wParam, lParam);
}

LRESULT CLKDialog::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKCOMMON_MSG_WPARAM_GETBGIMG == wParam)
	{
		PBGParam p = (PBGParam)lParam;
		OnGetBGImgMsg(p);
	}else if(LKCOMMON_MSG_WPARAM_GETMEMIMG == wParam)
	{
		PBGParam p = (PBGParam)lParam;
		OnGetMemImgMsg(p);
	}else if(LKCOMMON_MSG_WPARAM_RESETMEM == wParam)
	{
		ChangeBGImg();
		CWnd *pWnd = GetWindow(GW_CHILD);
		while(pWnd)
		{
			//pWnd->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_RESETMEM);
			pWnd->PostMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_RESETMEM);
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}

		RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
	}
	return 1;
}
void CLKDialog::OnSetMinMaxInfo(MINMAXINFO *lpMMI)
{
    CRect rt;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
    int nXFrame = rt.Width() + 4;
    int nYFrame = rt.Height() + 6;
    lpMMI->ptMaxPosition.x = rt.left - 2;
    lpMMI->ptMaxPosition.y = rt.top - 2;
	lpMMI->ptMaxSize.x = nXFrame;
	lpMMI->ptMaxSize.y = nYFrame;
	lpMMI->ptMaxTrackSize.x = nXFrame;
	lpMMI->ptMaxTrackSize.y = nYFrame;

	lpMMI->ptMinTrackSize.x = m_bMinSize.x;
	lpMMI->ptMinTrackSize.y = m_bMinSize.y;
}

// ��Ӧ��ȡǰ���ڴ�ͼƬ����Ϣ
void CLKDialog::OnGetMemImgMsg(PBGParam p)
{
	p->pBG = m_pMemBmp;
	if(m_bIsOwnerBGBmp)
	{
		//
		p->rt.left = 0;
		p->rt.top = 0;
	}else
	{
		//
	}
}

// ��Ӧ��ȡ����ͼƬ����Ϣ
void CLKDialog::OnGetBGImgMsg(PBGParam p)
{
	p->pBG = m_pBGBmp;
	if(m_bIsOwnerBGBmp)
	{
		//
		p->rt.left = 0;
		p->rt.top = 0;
	}else
	{
		//
	}
}

BOOL CLKDialog::PreTranslateMessage(MSG* pMsg)
{
    // ���˵��س���
    if((pMsg->message == WM_KEYDOWN))
    {
		if(pMsg->wParam == 13)
		{
			if(OnEnterPress(pMsg->hwnd, pMsg->lParam)) return TRUE;
		}else if(pMsg->wParam == VK_ESCAPE)
		{
			if(OnEscPress(pMsg->hwnd, pMsg->lParam)) return TRUE;
		}
    }else if(WM_NCMOUSEMOVE == pMsg->message || WM_NCMOUSELEAVE == pMsg->message
        || WM_LBUTTONDOWN == pMsg->message || WM_NCLBUTTONDOWN == pMsg->message
        || WM_LBUTTONUP == pMsg->message || WM_NCLBUTTONUP == pMsg->message) 
    {
        OnShowHint(pMsg);
    }else if(WM_MOUSEMOVE == pMsg->message || WM_MOUSELEAVE == pMsg->message)
    {
        OnChildShowHint(pMsg);
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

// ��ʼ��ͼƬ�б�
void CLKDialog::InitImageList()
{
	// ��ʼ��ͼƬ�б�
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_DIALOG_BEGIN, LKIMAGELISTINDEX_DIALOG_END);
}

// ����ǿͻ����ߴ��¼�
void CLKDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    bCalcValidRects = TRUE;//::CreateWindow
    lpncsp->rgrc[0].top    += m_rtClientOffset.top;		// Ĭ�ϱ������߶�Ϊ29
    lpncsp->rgrc[0].left   += m_rtClientOffset.left;	// Ĭ����߿���Ϊ2
    lpncsp->rgrc[0].right  -= m_rtClientOffset.right;	// Ĭ���ұ߿���Ϊ2 
    lpncsp->rgrc[0].bottom -= m_rtClientOffset.bottom;	// Ĭ�ϵױ߿�߶�Ϊ2
}


// ˽�г�Ա

// ��ʼ����������
void CLKDialog::InitBGBmp()
{
	BGParam prm;
	::SendMessage(GetProvideBGHwnd()/*GetParent()->GetSafeHwnd()*/, LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETBGIMG, (LPARAM)&prm);
	if(prm.pBG)
	{
		m_pBGBmp = (CLKImage *)prm.pBG;
		m_bIsOwnerBGBmp = false;
	}
}


// ����رհ�ťλ��
void CLKDialog::CalcCloseRect()
{
	m_rtCloseWnd = CRect(0, 0, 0, 0);
	if(m_arrImage.GetCount() >= LKGDIBASEDIALOG_IMAGEINDEX_COUNT)
	{
		CLKImage *pImg = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_CLOSE];
		if(pImg && pImg->IsValid())
		{
			m_rtCloseWnd.left = m_rtWnd.Width() - m_nCloseBtnRight - pImg->GetWidth() - m_nRightOffset;
			m_rtCloseWnd.top    = m_nTopOffset;
			m_rtCloseWnd.right  = m_rtCloseWnd.left + pImg->GetWidth();
			m_rtCloseWnd.bottom = m_rtCloseWnd.top  + pImg->GetHeight() / 3;
		}
	}
}

// ������󻯰�ťλ��
void CLKDialog::CalcMaxRect()
{
	m_rtMaxWnd = m_rtCloseWnd;
	if(m_arrImage.GetCount() >= LKGDIBASEDIALOG_IMAGEINDEX_COUNT)
	{
		CLKImage *pImg = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MAX];
		if(pImg && pImg->IsValid())
		{
			int nEdgeWidth    = GetSystemMetrics(SM_CXFRAME);
			m_rtMaxWnd.left   = m_rtCloseWnd.left - pImg ->GetWidth();
			m_rtMaxWnd.top	  = m_rtCloseWnd.top;
			m_rtMaxWnd.right  = m_rtMaxWnd.left + pImg->GetWidth();
			m_rtMaxWnd.bottom = m_rtMaxWnd.top  + pImg->GetHeight() / 3;
		}
	}
}

// ������С����ťλ��
void CLKDialog::CalcMinRect()
{
	m_rtMinWnd = m_rtMaxWnd;
	if(m_arrImage.GetCount() >= LKGDIBASEDIALOG_IMAGEINDEX_COUNT)
	{
		CLKImage *pImg = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MIN];
		if(pImg && pImg->IsValid())
		{
			int nEdgeWidth    = GetSystemMetrics(SM_CXFRAME);
			m_rtMinWnd.left   = m_rtMaxWnd.left - pImg->GetWidth();
			m_rtMinWnd.top    = m_rtCloseWnd.top;
			m_rtMinWnd.right  = m_rtMinWnd.left + pImg->GetWidth();
			m_rtMinWnd.bottom = m_rtMinWnd.top  + pImg->GetHeight() / 3;
		}
	}
}

// �����Զ���˵���ťλ��
void CLKDialog::CalcSelfMenuRect()
{
	m_rtMenuWnd = m_rtMinWnd;
	if(m_arrImage.GetCount() >= LKGDIBASEDIALOG_IMAGEINDEX_COUNT)
	{
		CLKImage *pImg = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_SELF];
		if(pImg && pImg->IsValid())
		{
			int nEdgeWidth    = GetSystemMetrics(SM_CXFRAME);
			m_rtMenuWnd.left   = m_rtMinWnd.left - pImg->GetWidth();
			m_rtMenuWnd.top    = m_rtCloseWnd.top;
			m_rtMenuWnd.right  = m_rtMenuWnd.left + pImg->GetWidth();
			m_rtMenuWnd.bottom = m_rtMenuWnd.top  + pImg->GetHeight() / 3;
		}
	}
}

// ����ϵͳ��ťλ��
void CLKDialog::CalcSysRect()
{
    m_rtSysWnd.left   = 4;
    m_rtSysWnd.top    = 3;
    m_rtSysWnd.right  = 29;
    m_rtSysWnd.bottom = 29;
}

// ����״̬��λ��
void CLKDialog::CalcStatusbarRect()
{
    CRect rt(m_rtWnd), rtx;
	rt.MoveToXY(0, 0);
	rt.top = rt.bottom - m_rtClientOffset.bottom;
	rt.bottom = rt.top + m_rtClientOffset.bottom;
    //
    for(POSITION pos = m_ltStatusItem.GetHeadPosition(); pos != 0; m_ltStatusItem.GetNext(pos))
    {
        // ����ÿһ������
        StatusbarItem *p = (StatusbarItem *)m_ltStatusItem.GetAt(pos);
        if(p)
        {
			p->rtIcon.top       = rt.top;
			p->rtIcon.bottom    = rt.bottom - 4;
			p->rtCaption.top    = rt.top;
			p->rtCaption.bottom = rt.bottom - 4;
        }
    }
}

// ������ͻ���
void CLKDialog::DrawClientArea()
{
	CPaintDC dc(this);
	CRect rtClient;
	GetClientRect(&rtClient);
	int nTop  = GetTitleBarHeight();
	int nLeft = GetLeftFrameWidth();
	rtClient.MoveToXY(nLeft, nTop);

	//
}

// ����ǿͻ����������ͷ��¼� 
bool CLKDialog::OnNCLMouseUpDef(UINT nHitTest, CPoint point) 
{
    //TRACE(_T("OnNCLMouseUpDef: x-%d, y-%d\r\n"), point.x, point.y);
    bool bRet = false;
	MOUSENCPOS nOld = m_eMouseNCPressPos;
    ReleaseCapture();
	MOUSENCPOS nNew = m_eMouseNCPressPos;
	m_eMouseNCPressPos = nOld;
	//mouse����ת��Ϊ����������
    point.x += m_rtClientOffset.left;//GetSystemMetrics(SM_CYFRAME);
    point.y += m_rtClientOffset.top;//GetTitleBarHeight();
	//�ж�mouse�Ƿ��Ƶ�ϵͳ��ť�� 
	if (m_rtCloseWnd.PtInRect(point) && m_eMouseNCPressPos == mnp_close/**/)
	{
        DrawNCIcon(0, m_eMouseNCPressPos, bs_mousemove, true);
		m_eMouseNCPressPos = mnp_unknow;
        OnCloseClick();
        bRet = true;
	}else if (IsInMinbox(point) && m_eMouseNCPressPos == mnp_min/**/)
	{
        DrawNCIcon(0, m_eMouseNCPressPos, bs_mousemove, true);
		m_eMouseNCPressPos = mnp_unknow;
        OnMinClick();
        bRet = true;
	}else if (IsInMaxbox(point) && m_eMouseNCPressPos == mnp_max/**/)
	{
        DrawNCIcon(0, m_eMouseNCPressPos, bs_mousemove, true);
		m_eMouseNCPressPos = mnp_unknow;
        OnMaxClick();
        bRet = true;
	}else if (IsInMenubox(point) && m_eMouseNCPressPos == mnp_menu/**/)
	{
        TRACE(_T("InMenubox\r\n"));
        DrawNCIcon(0, m_eMouseNCPressPos, bs_mousemove, true);
		m_eMouseNCPressPos = mnp_unknow;
        OnMenuClick();
        bRet = true;
	}else if (IsInOther(point) && m_eMouseNCPressPos == mnp_other/**/)
	{
        DrawNCIcon(0, m_eMouseNCPressPos, bs_mousemove, true);
		m_eMouseNCPressPos = mnp_unknow;
        OnOtherClick();
        bRet = true;
    }else
    {
		if(m_eMouseNCPressPos == mnp_top) m_eMouseNCPressPos = mnp_unknow;
        DrawNCIcon(0, m_eMouseNCPressPos, bs_normal, true);
    }
	m_eMouseNCPressPos = nNew;
	SendMessage(WM_NCPAINT, 0, 0);
    return bRet;
}

// ����ǿͻ��������������¼� 
bool CLKDialog::OnNCLMouseDownDef(UINT nHitTest, CPoint point) 
{
    bool bRet = false;
	//mouse����ת��Ϊ����������
    point -= m_rtWnd.TopLeft();
	//�ж�mouse�Ƿ��Ƶ�ϵͳ��ť��
	if (m_rtSysWnd.PtInRect(point))
	{
		OnSysClick();
        bRet = true;
	}else if (m_rtCloseWnd.PtInRect(point))
	{
        DrawNCIcon(0, mnp_close, bs_mousepress, true);
        m_eMouseNCPressPos = mnp_close;
        SetCapture();
        bRet = true;
	}else if (IsInMinbox(point))
	{
        DrawNCIcon(0, mnp_min, bs_mousepress, true);
        m_eMouseNCPressPos = mnp_min;
        SetCapture();
        bRet = true;
	}else if (IsInMaxbox(point))
	{
        DrawNCIcon(0, mnp_max, bs_mousepress, true);
        m_eMouseNCPressPos = mnp_max;
        SetCapture();
        bRet = true;
	}else if (IsInMenubox(point))
	{
        DrawNCIcon(0, mnp_menu, bs_mousepress, true);
        m_eMouseNCPressPos = mnp_menu;
        SetCapture();
        bRet = true;
	}else if (IsInOther(point))
	{
        DrawNCIcon(0, mnp_other, bs_mousepress, true);
        m_eMouseNCPressPos = mnp_other;
        SetCapture();
        bRet = true;
  //  }else if (point.y <= 2)
  //  {
		//// ��ȡ�����״
		//HCURSOR hCur = ::GetCursor();
		//HCURSOR hCur1 = ::LoadCursor(NULL, IDC_ARROW);
		//if(hCur != hCur1)
		//{
		//	m_dwMouseDrag = GetTickCount();
		//	m_eMouseNCPressPos = mnp_top;
		//	SetCapture();
		//	bRet = true;
		//}
    }else
    {
        m_eMouseNCPressPos = mnp_unknow;
        DrawNCIcon(0, mnp_unknow, bs_mousepress, true);
    }
    return bRet;
}

// �������ǿͻ����ƶ��¼� 
bool CLKDialog::OnNCMouseMoveDef(UINT nHitTest, CPoint point) 
{
	//mouse����ת��Ϊ���������� ��Ҫ
    point -= m_rtWnd.TopLeft();
	//�ж�mouse�Ƿ��Ƶ�ϵͳ��ť��
	if (m_rtCloseWnd.PtInRect(point))
	{
        if(m_eMouseNCPos != mnp_close)
        {
            DrawNCIcon(0, m_eMouseNCPos, bs_normal, true);
            DrawNCIcon(0, mnp_close, bs_mousemove, true);
            m_eMouseNCPos = mnp_close;
			//SendMessage(WM_NCPAINT, 0, 0);
        }
	}else if (IsInMinbox(point))
	{
        if(m_eMouseNCPos != mnp_min)
        {
            DrawNCIcon(0, m_eMouseNCPos, bs_normal, true);
            DrawNCIcon(0, mnp_min, bs_mousemove, true);
            m_eMouseNCPos = mnp_min;
			//SendMessage(WM_NCPAINT, 0, 0);
        }
	}else if (IsInMaxbox(point))
	{
        if(m_eMouseNCPos != mnp_max)
        {
            DrawNCIcon(0, m_eMouseNCPos, bs_normal, true);
            DrawNCIcon(0, mnp_max, bs_mousemove, true);
            m_eMouseNCPos = mnp_max;
			//SendMessage(WM_NCPAINT, 0, 0);
        }
	}else if (IsInMenubox(point))
	{
        if(m_eMouseNCPos != mnp_menu)
        {
            DrawNCIcon(0, m_eMouseNCPos, bs_normal, true);
            DrawNCIcon(0, mnp_menu, bs_mousemove, true);
            m_eMouseNCPos = mnp_menu;
			//SendMessage(WM_NCPAINT, 0, 0);
        }
	}else if (IsInOther(point))
	{
        if(m_eMouseNCPos != mnp_other)
        {
            DrawNCIcon(0, m_eMouseNCPos, bs_normal, true);
            DrawNCIcon(0, mnp_other, bs_mousemove, true);
            m_eMouseNCPos = mnp_other;
			//SendMessage(WM_NCPAINT, 0, 0);
       }
    }else 
    {
        if(m_eMouseNCPos != mnp_unknow)
        {
            DrawNCIcon(0, m_eMouseNCPos, bs_normal, true);
            DrawNCIcon(0, mnp_unknow, bs_mousemove, true);
            m_eMouseNCPos = mnp_unknow;
			//SendMessage(WM_NCPAINT, 0, 0);
        }
    }
	if(GetCapture() != this)
	{
		TRACKMOUSEEVENT   tme;   
		tme.cbSize		= sizeof(tme);   
		tme.dwFlags		= TME_LEAVE | TME_NONCLIENT; //ע��ǿͻ����뿪  
		tme.hwndTrack	= this->GetSafeHwnd();   
		tme.dwHoverTime = HOVER_DEFAULT; //ֻ��HOVER��Ч
		::TrackMouseEvent(&tme);
		//xxx = true;
	}
	//return true;
	return false;
}

void CLKDialog::DrawNClientArea(CDC * pDC, bool bFocus)
{
	if (m_hWnd)
	{
		if(m_pMemBmp && m_pMemBmp->IsValid())
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, m_rtWnd.Width(), m_rtWnd.Height());
			CBitmap *pOld = memDC.SelectObject(&bmp);
			memDC.ExcludeClipRect(m_rtClientOffset.left, m_rtClientOffset.top, m_rtWnd.Width() - m_rtClientOffset.right, m_rtWnd.Height() - m_rtClientOffset.bottom);
			m_pMemBmp->BitBlt(memDC.GetSafeHdc(), 0, 0);
			if (m_rtClientOffset.top > 20)
			{
				DrawTitleBarIcon(&memDC);
				DrawTitleCaption(&memDC);
			}
			//
            if(!bFocus)
            {
                // ��ʧȥ�����ɰ�
                DrawKillFocusFrame(&memDC);
            }
			pDC->BitBlt(0, 0, m_rtWnd.Width(), m_rtWnd.Height(), &memDC, 0, 0, SRCCOPY);
			memDC.SelectObject(pOld);
			bmp.DeleteObject();
			memDC.DeleteDC();
		}
    }
}

// ������ǿͻ���
void CLKDialog::DrawNClientFrame()
{
	if(m_hWnd)
	{
		if(!m_pBGBmp)
		{
			InitBGBmp();
		}
		if(m_pMemBmp)
		{
			delete m_pMemBmp;
		}
		m_pMemBmp = new CLKImage(m_rtWnd.Width(), m_rtWnd.Height());
		if(m_pBGBmp)
		{
			//if (!m_rtMaxWnd.IsRectEmpty() && m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MAX] != 0)
			//{
			//	// ����ɱ��С
			//	m_pBGBmp->StretchBilinear(m_pMemBmp, m_pMemBmp->GetWidth(), m_pMemBmp->GetHeight(), m_pBGBmp->GetWidth(), m_pBGBmp->GetHeight());
			//}
			//else
			//{
				m_pBGBmp->DrawToImage(m_pMemBmp);
			//}
		}
		// ���߿�
		DrawFrameEdge();
		// ��ʼ������ǰ��ͼƬ
		OnInitMemImg();
	}/**/
}

BOOL CLKDialog::OnEnterPress(HWND hCrtl, LPARAM lparam)
{
	HWND h = GetFocus()->GetSafeHwnd();
	if(h == hCrtl) ::SendMessage(h, WM_KEYDOWN, VK_RETURN, lparam);
    return TRUE;
}

// �ж�����Ƿ����ϵİ�ť��Χ��
bool CLKDialog::IsInOldbox(CPoint point)
{
	if( point.x > m_rtCloseWnd.left && !m_rtCloseWnd.PtInRect(point))
	{
		return true;
	}
    return false;
}

// �ж�����Ƿ�����С��ť��Χ��
bool CLKDialog::IsInMinbox(CPoint point)
{
    //CRect rt(m_rtMinWnd);
    //rt.InflateRect(1, 1, 1, 1);
	if(m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MIN] && m_rtMinWnd.PtInRect(point))
	{
		return true;
	}
    return false;
}

// �ж�����Ƿ������ť��Χ�� 
bool CLKDialog::IsInMaxbox(CPoint point)
{
	if(m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MAX] && m_rtMaxWnd.PtInRect(point))
	{
		return true;
	}
    return false;
}

// �ж�����Ƿ����Զ���˵���ť��Χ��
bool CLKDialog::IsInMenubox(CPoint point)
{
	if(m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_SELF] && m_rtMenuWnd.PtInRect(point))
	{
		return true;
	}
    return false;
}

// �¼�
void CLKDialog::OnShowHint(MSG *pMsg)
{
    if(m_pToolTip && m_pToolTip->GetSafeHwnd())
    {
        if(WM_NCMOUSEMOVE != pMsg->message && WM_NCMOUSELEAVE != pMsg->message)
        {
            m_pToolTip->RelayEvent(pMsg);
            return;
        }
        DWORD dwPos = GetMessagePos();
        CPoint point(LOWORD(dwPos) - m_rtWnd.left, HIWORD(dwPos) - m_rtWnd.top);
        if(m_rtCloseWnd.PtInRect(point))
        {
            if(mnp_close == m_eMouseNCPos) return;
            m_pToolTip->Activate(FALSE);
            m_pToolTip->Pop();
            CString strText;
        	CToolInfo ti;
            m_pToolTip->GetToolInfo(ti, this);
            memcpy(ti.szText, _T("�ر�"), 6);
            m_pToolTip->SetToolInfo(&ti);
            m_pToolTip->Activate(TRUE);
            m_pToolTip->RelayEvent(pMsg);
        }else if(m_rtMaxWnd.PtInRect(point))
        {
            if(mnp_max == m_eMouseNCPos) return;
            m_pToolTip->Activate(FALSE);
            m_pToolTip->Pop();
            CString strText;
        	CToolInfo ti;
            m_pToolTip->GetToolInfo(ti, this);
	        WINDOWPLACEMENT wndpl;
	        GetWindowPlacement(&wndpl);
            if(wndpl.showCmd == SW_SHOWMAXIMIZED)
            {
                memcpy(ti.szText, _T("��ԭ"), 6);
            }else
            {
                memcpy(ti.szText, _T("���"), 8);
            }
            m_pToolTip->SetToolInfo(&ti);
            m_pToolTip->Activate(TRUE);
            m_pToolTip->RelayEvent(pMsg);
        }else if(m_rtMinWnd.PtInRect(point))
        {
            if(mnp_min == m_eMouseNCPos) return;
            m_pToolTip->Activate(FALSE);
            m_pToolTip->Pop();
            CString strText;
        	CToolInfo ti;
            m_pToolTip->GetToolInfo(ti, this);
            memcpy(ti.szText, _T("��С��"), 8);
            m_pToolTip->SetToolInfo(&ti);
            m_pToolTip->Activate(TRUE);
            m_pToolTip->RelayEvent(pMsg);
        }else if(m_rtMenuWnd.PtInRect(point))
        {
            if(mnp_menu == m_eMouseNCPos) return;
            m_pToolTip->Activate(FALSE);
            m_pToolTip->Pop();
            CString strText;
        	CToolInfo ti;
            m_pToolTip->GetToolInfo(ti, this);
            memcpy(ti.szText, _T("�˵�"), 6);
            m_pToolTip->SetToolInfo(&ti);
            m_pToolTip->Activate(TRUE);
            m_pToolTip->RelayEvent(pMsg);
        }else
        {
            //m_pToolTip->RelayEvent(pMsg);
            m_pToolTip->Pop();
        }
    }
}
void CLKDialog::OnChildShowHint(MSG *pMsg)
{
    if(m_pChildToolTip && m_pChildToolTip->GetSafeHwnd())
    {
        //if(WM_MOUSEMOVE == pMsg->message || WM_LBUTTONUP == pMsg->message || WM_LBUTTONDOWN == pMsg->message)
        //{
            m_pChildToolTip->RelayEvent(pMsg);
        //}
    }
}
void CLKDialog::OnCloseClick()
{
    PostMessage(WM_CLOSE);
    //EndDialog(IDCANCEL);
}
 
void CLKDialog::OnMinClick()
{
    PostMessage(WM_SYSCOMMAND, SC_MINIMIZE); 
    //ShowWindow(SW_MINIMIZE);
}

void CLKDialog::OnMaxClick()
{
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	if(wndpl.showCmd == SW_SHOWMAXIMIZED)
	{
		m_bIsMax = false;
		ShowWindow(SW_RESTORE);
		Invalidate();
	}else
	{
  //      CRect rt;
  //      SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
  //      int nXFrame = rt.Width() + 4;
  //      int nYFrame = rt.Height() + 4;
		//m_bIsMax = true;
        //::SetWindowPos(this->m_hWnd, 0, -2, -2, nXFrame, nYFrame, SWP_SHOWWINDOW | SWP_NOZORDER);
		ShowWindow(SW_SHOWMAXIMIZED);
        //wndpl.showCmd = SW_SHOWMAXIMIZED;
        //wndpl.ptMaxPosition.x = 700;
        //wndpl.ptMaxPosition.y = 700;
        //SetWindowPlacement(&wndpl);
     //   int n = wndpl.flags;
	}
}

// �ػ��ǿͻ���ָ��ͼ��
void CLKDialog::DrawNCIcon(CDC *pDC, MOUSENCPOS mnp, BITMAPSTYLE bs, bool bErase)
{
    switch(mnp)
    {
    case mnp_sys:
        {
            DrawSys(pDC, bs, bErase);
            break;
        }
    case mnp_close:
        {
            DrawClose(pDC, bs, bErase);
            break;
        }
    case mnp_max:
        {
            DrawMax(pDC, bs, bErase);
            break;
        }
    case mnp_min:
        {
            // ����С��ͼ��
            DrawMin(pDC, bs, bErase);
            break;
        }
    case mnp_menu:
        {
            // ����С��ͼ��
            DrawMenu(pDC, bs, bErase);
            break;
        }
    case mnp_other:
        {
            // ����������ͼƬ
            OnDrawOther(pDC, bs, bErase);
            break;
        }
    }
}

// �ػ��ǿͻ���ͨ�ð�ť(��̬ͼƬģʽ)
void CLKDialog::DrawButton(CDC *pDC, CLKImage *pImage, CRect &rtImg, BITMAPSTYLE bs, bool bErase)
{
	if(!pImage) return;
	CLKImage *pImg = pImage;
	if(pImg)
	{
		int nHeight = pImg->GetHeight() / 3;
		int nOffset = bs == bs_normal ? 0 :(bs == bs_mousemove ? nHeight : 2 * nHeight);
		if(pDC)
		{
			//if(bErase && m_pBGBmp)
			//{
			//	m_pBGBmp->BitBlt(pDC->GetSafeHdc(), rtImg.left, rtImg.top, rtImg.Width(), rtImg.Height(), rtImg.left, rtImg.top, SRCCOPY);
			//}
			if(bErase && m_pMemBmp)
			{
				m_pMemBmp->BitBlt(pDC->GetSafeHdc(), rtImg.left, rtImg.top, rtImg.Width(), rtImg.Height(), rtImg.left, rtImg.top, SRCCOPY);
			}
			pImage->Draw(pDC->GetSafeHdc(), rtImg.left, rtImg.top, rtImg.Width(), rtImg.Height(), 0, nOffset, rtImg.Width(), rtImg.Height());
		}else
		{
			pDC = GetWindowDC();
			if(pDC)
			{
				if(bErase && m_pMemBmp)
				{
					m_pMemBmp->BitBlt(pDC->GetSafeHdc(), rtImg.left, rtImg.top, rtImg.Width(), rtImg.Height(), rtImg.left, rtImg.top, SRCCOPY);
				}
				pImage->Draw(pDC->GetSafeHdc(), rtImg.left, rtImg.top, rtImg.Width(), rtImg.Height(), 0, nOffset, rtImg.Width(), rtImg.Height());
                
                
                if(GetForegroundWindow() != this/* && bs == bs_normal*/)
                {
                    CLKImage img(1, rtImg.Height(), 125, 125 - rtImg.Height() * 5);
                    if(img.IsValid())
                    {
                        img.DrawToDC(pDC, rtImg, 0);
                    }
                }
				ReleaseDC(pDC);
			}
		}
	}
}

// �ػ��ǿͻ���ϵͳͼ��(��ͼƬģʽ)
void CLKDialog::DrawSys(CDC *pDC, BITMAPSTYLE bs, bool bErase)
{
	CLKImage *pImg = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_SYS];
	if(pImg)
	{
		DrawButton(pDC, pImg, m_rtSysWnd, bs, bErase);
	}else if(m_hSysIcon)
	{
		//HICON  hIcon = (HICON)::LoadImage(AfxGetApp()->m_hInstance, strIcon, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		//HICON  hIcon = (HICON)::LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		//pDC->DrawIcon(m_rtSysWnd.left, m_rtSysWnd.top, m_hSysIcon);
		//::DrawIconEx(pDC->GetSafeHdc(), m_rtSysWnd.left, m_rtSysWnd.top, m_hSysIcon, 24, 24, 0, NULL, DI_NORMAL); 
	}
}

// �ػ��ǿͻ����رհ�ťͼ��(��ͼƬģʽ)
void CLKDialog::DrawClose(CDC *pDC, BITMAPSTYLE bs, bool bErase)
{
	DrawButton(pDC, m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_CLOSE], m_rtCloseWnd, bs, bErase);
}

// �ػ��ǿͻ�����󻯻�ָ���ťͼ��(��ͼƬģʽ)
void CLKDialog::DrawMax(CDC *pDC, BITMAPSTYLE bs, bool bErase)
{
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	CLKImage *pImg;
	if(wndpl.showCmd == SW_SHOWMAXIMIZED)
	{
		pImg = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_RESTORE];
	}else
	{
		pImg = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MAX];
	}
	DrawButton(pDC, pImg, m_rtMaxWnd, bs, bErase);
}

// �ػ��ǿͻ�����С����ťͼ��(��ͼƬģʽ)
void CLKDialog::DrawMin(CDC *pDC, BITMAPSTYLE bs, bool bErase)
{
	DrawButton(pDC, m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MIN], m_rtMinWnd, bs, bErase);
}

// �ػ��ǿͻ����Զ���˵���ťͼ��(��ͼƬģʽ)
void CLKDialog::DrawMenu(CDC *pDC, BITMAPSTYLE bs, bool bErase)
{
	DrawButton(pDC, m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_SELF], m_rtMenuWnd, bs, bErase);
}

//using namespace Gdiplus;
// ������
void CLKDialog::DrawNCBG()
{
	CLKImage *pImg = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_BG];
	if (pImg && pImg->IsValid() && m_pBGBmp && m_pBGBmp->IsValid())
	{
		if (pImg->GetWidth() < SCREEN_WIDTH || pImg->GetHeight() < SCREEN_HEIGHT)
		{
			double fRateX = (double)SCREEN_WIDTH / pImg->GetWidth();
			double fRateY = (double)SCREEN_HEIGHT / pImg->GetHeight();
			//float fRate = fRateX > fRateY ? fRateX : fRateY;
			CRect rtDest(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (fRateX > fRateY)
			{
				rtDest.right = SCREEN_WIDTH;
				rtDest.bottom = (LONG)(pImg->GetHeight() * fRateX);
			}
			else
			{
				rtDest.bottom = SCREEN_HEIGHT;
				rtDest.right = (LONG)(pImg->GetWidth() * fRateY);
			}
			//pImg->DrawToImageByBICUBIC2(m_pBGBmp, rtDest);
			//pImg->DrawToImage(m_pBGBmp, rtDest);
			pImg->StretchBilinear(m_pBGBmp, rtDest.Width(), rtDest.Height(), pImg->GetWidth(), pImg->GetHeight());
			// ��������
			//pImg->StretchToImage(m_pBGBmp);
		}
		else
		{
			pImg->DrawToImage(m_pBGBmp);
		}
		//pImage->DrawToImage(m_pBGBmp);
	}
}

// ��������
void CLKDialog::DrawTitlebar()
{
}

// �������߿�
void CLKDialog::DrawFrameEdge()
{
	CLKImage *pImage = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_BGFRAME];
	AngleOffset angleOffset(30, 0, 3, 0, 3, 0, 3);
	pImage->DrawToImageFrame(m_pMemBmp, m_rtClientOffset, angleOffset);
	//pImage->DrawToImage(m_pMemBmp);

}

// ���������ı�
void CLKDialog::DrawTitleCaption(CDC *pDC)
{
	CString strText;
	GetWindowText(strText);
	pDC->SetBkMode(TRANSPARENT);
	CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST100());
	CRect rtText(m_rtSysWnd.right, m_rtSysWnd.top, m_rtMenuWnd.left, m_rtMenuWnd.bottom);
	pDC->SetTextColor(RGB(210, 210, 210));
	rtText.OffsetRect(2, 3);
	//rtText.OffsetRect(-1, -1);
	//pDC->DrawText(strText, &rtText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	DrawShadowText(pDC->GetSafeHdc(), strText, strText.GetLength(), &rtText, DT_LEFT | DT_SINGLELINE | DT_VCENTER, CLKFontMgr::GetDefaultTextColor(), RGB(235, 235, 235), 0, 0);
	//rtText.OffsetRect(2, 2);
	//pDC->DrawText(strText, &rtText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	//pDC->SetTextColor(m_crText);
	//rtText.OffsetRect(-1, -1);
	//pDC->SelectObject(CLKFontMgr::GetST100());
	//pDC->DrawText(strText, &rtText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	pDC->SelectObject(pOld);
	
}

// ��������ͼ��
void CLKDialog::DrawTitleBarIcon(CDC *pDC, bool IsNcPaint)
{
	CRect rtOffset(0, 0, 0, 0);
	if (!IsNcPaint)
	{
		rtOffset = m_rtClientOffset;
	}
	m_rtCloseWnd.OffsetRect(-rtOffset.left, -rtOffset.top);
	m_rtMaxWnd.OffsetRect(-rtOffset.left, -rtOffset.top);
	m_rtMinWnd.OffsetRect(-rtOffset.left, -rtOffset.top);
	m_rtMenuWnd.OffsetRect(-rtOffset.left, -rtOffset.top);
	// ��ϵͳͼ��;
    DrawNCIcon(pDC, mnp_sys);
    // ���ر�ͼ��
    DrawNCIcon(pDC, mnp_close);
    // �����ͼ�� 
    DrawNCIcon(pDC, mnp_max);
    // ����С��ͼ��
    DrawNCIcon(pDC, mnp_min);
    // ���Զ���˵���ťͼ��
    DrawNCIcon(pDC, mnp_menu);
    // ���Զ�����������ͼƬ
    DrawNCIcon(pDC, mnp_other);/**/
	m_rtCloseWnd.OffsetRect(rtOffset.left, rtOffset.top);
	m_rtMaxWnd.OffsetRect(rtOffset.left, rtOffset.top);
	m_rtMinWnd.OffsetRect(rtOffset.left, rtOffset.top);
	m_rtMenuWnd.OffsetRect(rtOffset.left, rtOffset.top);
}

// ��ʧȥ�����ɰ�
void CLKDialog::DrawKillFocusFrame(CDC *pDC)
{
	// ���ɰ�
	CLKImage img(1, 26, 125, 0);
    if(img.IsValid())
    {
        CRect rtDest(0, 0, m_rtWnd.Width(), 26);
        img.DrawToDC(pDC, rtDest, 0);

        //img.AlphaBlend(pDC->GetSafeHdc(), 0, 0, m_rtWnd.Width(), 26, 0, 0, 1, 26, 50);
        img.AlphaBlend(pDC->GetSafeHdc(), 0, 2, 2, m_rtWnd.Height() - 2, 0, 0, 1, 1, 100);
        img.AlphaBlend(pDC->GetSafeHdc(), m_rtWnd.Width() - 2, 2, 2, m_rtWnd.Height() - 2, 0, 0, 1, 1, 100);
        img.AlphaBlend(pDC->GetSafeHdc(), 2, m_rtWnd.Height() - 2, m_rtWnd.Width() - 4, 2, 0, 0, 1, 1, 100);
    }
}

//float spline(float x)
//{
//    float result;
//    if(fabs(x)>=0&&fabs(x)<1)
//    {
//        result=1-2*fabs(x)*fabs(x)+fabs(x)*fabs(x)*fabs(x);
//    }else if (fabs(x)>=1&&fabs(x)<2)
//    {
//        result=4-8*fabs(x)+5*fabs(x)*fabs(x)-fabs(x)*fabs(x)*fabs(x);
//    }else if(fabs(x)>=2)
//    {
//        result=0;
//    }
//    return result;
//}
//
//void DrawToImageByBICUBIC2(int nWidth, int nHeight)
//{
//    int newx,newy;//Ŀ�����ص�x��y����
//    int oldx,oldy;//ԭͼ���x��y����
//    int old16R[4][4],old16G[4][4],old16B[4][4];//ԭͼ��16������RGB������ֵ
//    float newR=0.0f,newG=0.0f,newB=0.0f;//Ŀ�����ص��RGB����
//    float f_newx,f_newy;//����任�ĸ���������ֵ
//    int i_x,i_y;//�������������������
//    float u,v;//�����������С������
//    int newHeight = nHeight;
//    int newWidth = nWidth;//Ŀ��ͼ��ĳ��Ϳ�
//
//    int tempR[4];
//    int tempG[4];
//    int tempB[4];
//    for(int k=0;k<4;k++)
//    {
//        tempR[k]=0;
//        tempG[k]=0;
//        tempB[k]=0;
//    }
//    float A[4],C[4];//����A��C
//    // double r1,r2;
//
//
//    for(newy=0; newy < newHeight; newy++)
//    {
//        f_newy=(float)newy/2;
//        i_y=(int)floor(f_newy);
//        v=f_newy-(float)floor(f_newy);//��y��Ŀ�����굽ԭ����ı任ϵ��v
//
//        for (newx=0;newx<newWidth;newx++)
//        {
//            f_newx=(float)newx/2;
//            i_x=(int)floor(f_newx);
//            u=f_newx-(float)floor(f_newx);//��x��Ŀ�����굽ԭ����ı任ϵ��u
//            A[0]=spline(u+1.0f);
//            A[1]=spline(u);
//            A[2]=spline(1.0f-u);
//            A[3]=spline(2.0f-u);
//            C[0]=spline(v+1.0f);
//            C[1]=spline(v);
//            C[2]=spline(1.0f-v);
//            C[3]=spline(2.0f-v);
//            newR=0.0f,newG=0.0f,newB=0.0f;
//            for(int i=0;i<4;i++)
//            {
//                for(int j=0;j<4;j++)
//                {
//                    oldy=i_y+j-1;
//                    oldx=i_x+i-1;
//                    if (oldx<0) oldx=0;  
//                    if (oldy<0) oldy=0;  
//                    if (oldx>=xmax) oldx=xmax-1;  
//                    if (oldy>=ymax) oldy=ymax-1; 
//
//                    old16R[i][j]=Original[oldy*xmax+oldx].rgbRed;
//                    old16G[i][j]=Original[oldy*xmax+oldx].rgbGreen;
//                    old16B[i][j]=Original[oldy*xmax+oldx].rgbBlue;
//                }
//            }
//            for(int n=0;n<4;n++)
//            {
//
//                for(int m=0;m<4;m++)
//                {
//                    newR+=A[m]*old16R[n][m]*C[n];
//                    newG+=A[m]*old16G[n][m]*C[n];
//                    newB+=A[m]*old16B[n][m]*C[n];
//                }
//
//            }
//
//            Result[newy*xmax*2+newx].rgbRed=newR;
//            Result[newy*xmax*2+newx].rgbGreen=newG;
//            Result[newy*xmax*2+newx].rgbBlue=newB;
//        }
//    }
//}
//

////���Ÿ���
//void StretchBltFast1(CLKImage* pDest, int cxDest, int cyDest, CLKImage* pSrc, int cxSrc, int cySrc)
//{
//    int i,j,k;
//    LPBYTE pBitsSrc = (LPBYTE)(pSrc->GetBits()); //���ݿ���ʼλ��
//    LPBYTE pBitsDest = (LPBYTE)(pDest->GetBits());//���ݿ���ʼλ��
//    LPBYTE pixAddrSrc = pBitsSrc;
//    LPBYTE oldpixAddrSrcX = 0;
//    LPBYTE oldpixAddrSrcY = 0;
//    LPBYTE pixAddrDest = pBitsDest;
//
//    LPBYTE pixAddrTemp[16] = {0};
//
//    int strideSrc = pSrc->GetPitch(); //pitch��ʱΪ��
//    int strideDest = pDest->GetPitch();
//    int bytesPerPixelSrc = pSrc->GetBPP()/8;
//    int bytesPerPixelDest = pDest->GetBPP()/8;
//
//    double fRate = (double)cxSrc / cxDest;
//    
//    int nInsertNumberX = cxDest - cxSrc;
//    int nInsertNumberY = cyDest - cySrc;
//    int nInsertSplitX  = cxDest / nInsertNumberX;
//    int nInsertSplitY  = cyDest / nInsertNumberY;
//
//    int x(0), y(0);
//    for (j = 0; j < cyDest && j < pDest->GetHeight(); j++)
//    {
//        pixAddrDest = pBitsDest + strideDest * j;
//        pixAddrSrc = pBitsSrc + strideSrc * y;
//        for (i = 0; i < cxDest && i < pDest->GetWidth(); i++)
//        {
//            if((i % nInsertSplitX == 0 || j % nInsertSplitY == 0)&& /**/i != cxDest - 1 && i != pDest->GetWidth() - 1 && j != cyDest - 1 && j != pDest->GetHeight() - 1 && j != 0 && i != 0)
//            {
//                pixAddrDest += bytesPerPixelDest;
//                continue;
//            }
//            *pixAddrDest = *pixAddrSrc;
//            *(pixAddrDest + 1) = *(pixAddrSrc + 1);
//            *(pixAddrDest + 2) = *(pixAddrSrc + 2);
//            if(i % nInsertSplitX && i != cxDest - 1 && i != pDest->GetWidth() - 1)
//            {
//                pixAddrSrc += bytesPerPixelSrc;
//            }
//            pixAddrDest += bytesPerPixelDest;
//        }
//        if(j % nInsertSplitY && j != cyDest - 1 && j != pDest->GetHeight() - 1)
//        {
//            y++;
//        }
//    }
//
//    y = 0;
//    for (j = 0; j < cyDest && j < pDest->GetHeight(); j++)
//    {
//        pixAddrDest = pBitsDest + strideDest * j;
//        pixAddrSrc = pBitsSrc + strideSrc * y;
//        int nMax = cxDest / nInsertSplitX;
//        nMax = (nMax < pDest->GetWidth() / nInsertSplitX ? nMax : pDest->GetWidth() / nInsertSplitX);
//        for(int x = 1; x < nMax; x++)
//        {
//            int i = x * nInsertSplitX;
//            pixAddrDest += bytesPerPixelDest * nInsertSplitX;
//            LPBYTE pBitsDest1 = pixAddrDest - bytesPerPixelDest;
//            LPBYTE pBitsDest2 = pixAddrDest + bytesPerPixelDest;
//            *pixAddrDest = (*pBitsDest1 + *pBitsDest2) / 2;
//            *(pixAddrDest + 1) = (*(pBitsDest1 + 1) + *(pBitsDest2 + 1)) / 2;
//            *(pixAddrDest + 2) = (*(pBitsDest1 + 2) + *(pBitsDest2 + 2)) / 2;
//        }
//    }
//
//    y = 0;
//    int nMax = cyDest / nInsertSplitY;
//    nMax = (nMax < pDest->GetHeight() / nInsertSplitY ? nMax : pDest->GetHeight() / nInsertSplitY);
//    for (y = 1; y < nMax; y++)
//    {
//        j = y * nInsertSplitY;
//        pixAddrDest = pBitsDest + strideDest * j;
//        for (i = 0; i < cxDest && i < pDest->GetWidth(); i++)
//        {
//            pixAddrDest += bytesPerPixelDest;
//            LPBYTE pBitsDest1 = pixAddrDest - bytesPerPixelDest * pDest->GetWidth();
//            LPBYTE pBitsDest2 = pixAddrDest + bytesPerPixelDest * pDest->GetWidth();
//            *pixAddrDest = (*pBitsDest1 + *pBitsDest2) / 2;
//            *(pixAddrDest + 1) = (*(pBitsDest1 + 1) + *(pBitsDest2 + 1)) / 2;
//            *(pixAddrDest + 2) = (*(pBitsDest1 + 2) + *(pBitsDest2 + 2)) / 2;
//        }
//    }
//}
//
//
//
