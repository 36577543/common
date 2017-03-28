// ..\..\common\LKComponent\GDI\LKCtrlBase.cpp : ʵ���ļ�
//

//#include "stdafx.h"
#include "LKCtrlBase.h"
#include "LKImageMgr.h"

// CLKCtrlBase

IMPLEMENT_DYNAMIC(CLKCtrlBase, CWnd)

CLKCtrlBase::CLKCtrlBase(int nImageIndex)
: m_pBGImg(0)
// �ͻ���ƫ����(�߿�Ĭ����)
, m_rtClientOffset(0, 0, 0, 0)
, m_pMemBmp(0)
, m_rtClient(0, 0, 0, 0)
, m_nImageIndex(nImageIndex)
, m_crText(RGB(43, 85, 128))
// �Ƿ��������
, m_bEraseBG(false)
// �Ƿ��ȡ������(���߸�ǰ��)
, m_bGetParentBG(false)
, m_pData(0)
{
	// ���캯���е����麯���޶�̬����Ч��
	// ��ʼ��ͼƬ�б�
}

CLKCtrlBase::~CLKCtrlBase()
{
	/*if(m_pMemBmp)
	{
		delete m_pMemBmp;
	}*/
}

BOOL CLKCtrlBase::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	//InitImageList();
	//BOOL bRet = CWnd::Create(AfxRegisterWndClass(CS_DBLCLKS), _T("CLKCtrlBase"), dwStyle, rect, pParentWnd, nID);
	//InitCtrlAfterCreate();
	return Create(NULL, _T("CLKCtrlBase"), dwStyle, rect, pParentWnd, nID);
	//return CWnd::Create(AfxRegisterWndClass(CS_DBLCLKS), _T("CLKCtrlBase"), dwStyle, rect, pParentWnd, nID);
}

BOOL CLKCtrlBase::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	//InitImageList();
	//BOOL bRet = CWnd::Create(AfxRegisterWndClass(CS_DBLCLKS), lpszText, dwStyle, rect, pParentWnd, nID);
	//InitCtrlAfterCreate();
	return Create(NULL, lpszText, dwStyle, rect, pParentWnd, nID);
}

BOOL CLKCtrlBase::Create(UINT nClassStyle, LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	InitImageList();
	BOOL bRet = CWnd::Create(AfxRegisterWndClass(nClassStyle), lpszText, dwStyle, rect, pParentWnd, nID);
	InitCtrlAfterCreate();
	return bRet;
}

// ������Ҫ������ͼƬ
void CLKCtrlBase::SetParseImg(CLKImage *pImg)
{
	if(m_arrImage.GetCount() > m_nImageIndex && m_nImageIndex >= 0)
	{
		// Ĭ�ϵڶ���ͼƬ����Ҫ������ǰ��ͼƬ
		m_arrImage[m_nImageIndex] = pImg;
		// ˢ��ǰ������
		ReSetMem();
	}
}

// ��ȡָ���ַ����Ŀ��
int CLKCtrlBase::GetStringWidth(CString &strText, CFont *pFont)
{
    HWND hWnd = ::GetDesktopWindow();
    HDC hDC = ::GetDC(hWnd);
    CDC *pDC = CDC::FromHandle(hDC);
    int nWidth(0);
    if(pDC && pDC->GetSafeHdc())
    {
        CFont *pOld = pDC->SelectObject(pFont);
        CRect rtText(0, 0, 0, 0);
        pDC->DrawText(strText, strText.GetLength(), &rtText, DT_CALCRECT);
        pDC->SelectObject(pOld);
        nWidth = rtText.Width();
        ::ReleaseDC(hWnd, hDC);
    }
    return nWidth;
}

// ��ȡָ���ַ����ĸ߶ȺͿ��
void CLKCtrlBase::GetStringWidthAndHeight(CString &strText, LOGFONT *pLF, int &nWidth, int &nHeight)
{
    CFont ft;
    ft.CreateFontIndirect(pLF);
    GetStringWidthAndHeight(strText, &ft, nWidth, nHeight);
    //ft.
}

// ���Ҵ��ַ�����ʼƫ��λ�õ�����
int CLKCtrlBase::FindStringIndexByPos(CString strText, int nOffsetPos, CFont *pFont, CDC *pDC)
{
    int nIndex = -1;
    if(0 == pDC)
    {
        HWND hWnd = ::GetDesktopWindow();
        HDC hDC = ::GetDC(hWnd);
        pDC = CDC::FromHandle(hDC);
        if(pDC && pDC->GetSafeHdc())
        {
            CFont *pOld = pDC->SelectObject(pFont);
            CRect rtText(0, 0, 0, 0);
            //pDC->DrawText(strText, strText.GetLength(), &rtText, DT_CALCRECT);
            nIndex = FindStringIndexByPos(strText, nOffsetPos, pDC);
            pDC->SelectObject(pOld);
            ::ReleaseDC(hWnd, hDC);
        }
    }else
    {
        if(pDC && pDC->GetSafeHdc())
        {
            CFont *pOld = pDC->SelectObject(pFont);
            CRect rtText(0, 0, 0, 0);
            nIndex = FindStringIndexByPos(strText, nOffsetPos, pDC);
            pDC->SelectObject(pOld);
        }
    }
    return nIndex;
}

int CLKCtrlBase::FindStringIndexByPos(CString strText, int nOffsetPos, CFont *pFont)
{
    CDC *pDC = GetWindowDC();
    int nRet = FindStringIndexByPos(strText, nOffsetPos, pFont, pDC);
    ReleaseDC(pDC);
    return nRet;
}
int CLKCtrlBase::FindStringIndexByPos(CString strText, int nOffsetPos, LOGFONT *pLF)
{
    CDC *pDC = GetWindowDC();
    int nRet = FindStringIndexByPos(strText, nOffsetPos, pLF, pDC);
    ReleaseDC(pDC);
    return nRet;
}

int CLKCtrlBase::FindStringIndexByPos(CString strText, int nOffsetPos, LOGFONT *pLF, CDC *pDC)
{
    CFont ft;
    ft.CreateFontIndirect(pLF);
    int nRet = FindStringIndexByPos(strText, nOffsetPos, &ft, pDC);
    ft.DeleteObject();
    return nRet;
}

void CLKCtrlBase::GetStringWidthAndHeight(CString &strText, CFont *pFont, int &nWidth, int &nHeight)
{
    CDC *pDC = 0;
    HWND hWnd = GetSafeHwnd();
    if(hWnd)
    {
        pDC = GetDC();
        GetStringWidthAndHeight(strText, pFont, nWidth, nHeight, pDC);
        ReleaseDC(pDC);
    }else
    {
        GetStringWidthAndHeight(strText, pFont, nWidth, nHeight, 0);
    }
}

void CLKCtrlBase::GetStringWidthAndHeight(CString &strText, LOGFONT *pLF, int &nWidth, int &nHeight, CDC *pDC)
{
    CFont ft;
    ft.CreateFontIndirect(pLF);
    GetStringWidthAndHeight(strText, &ft, nWidth, nHeight, pDC);
    ft.DeleteObject();
}

void CLKCtrlBase::GetStringWidthAndHeight(CString &strText, CFont *pFont, int &nWidth, int &nHeight, CDC *pDC)
{
    nWidth = 0;
    nHeight = 0;
    if(0 == pDC)
    {
        HWND hWnd = ::GetDesktopWindow();
        HDC hDC = ::GetDC(hWnd);
        pDC = CDC::FromHandle(hDC);
        if(pDC && pDC->GetSafeHdc())
        {
            CFont *pOld = pDC->SelectObject(pFont);
            CRect rtText(0, 0, 0, 0);
            pDC->DrawText(strText, strText.GetLength(), &rtText, DT_CALCRECT);
            pDC->SelectObject(pOld);
            ::ReleaseDC(hWnd, hDC);
            nWidth = rtText.Width();
            nHeight = rtText.Height();
        }
    }else
    {
        if(pDC && pDC->GetSafeHdc())
        {
            CFont *pOld = pDC->SelectObject(pFont);
            CRect rtText(0, 0, 0, 0);
            pDC->DrawText(strText, strText.GetLength(), &rtText, DT_CALCRECT);
            pDC->SelectObject(pOld);
            nWidth = rtText.Width();
            nHeight = rtText.Height();
        }
    }
}

BEGIN_MESSAGE_MAP(CLKCtrlBase, CWnd)
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



//=================================================================================================
// CLKCtrlBase ��Ϣ�������  ====��ʼ====
LRESULT CLKCtrlBase::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKCOMMON_MSG_WPARAM_GETBGIMG == wParam)
	{
		if(m_pBGImg)
		{
			PBGParam p = (PBGParam)lParam;
			p->pBG = m_pBGImg;
		}else
		{
			GetParent()->SendMessage(LKCOMMON_MSG, wParam, lParam);
			CRect rt;
			GetRectInParent(&rt);
			PBGParam p = (PBGParam)lParam;
			p->rt.OffsetRect(rt.left, rt.top);
		}
	}else if(LKCOMMON_MSG_WPARAM_GETMEMIMG == wParam)
	{
		if(m_pMemBmp)
		{
			PBGParam p = (PBGParam)lParam;
			p->pBG = m_pMemBmp;
		}else
		{
			GetParent()->SendMessage(LKCOMMON_MSG, wParam, lParam);
			CRect rt;
			GetRectInParent(&rt);
			PBGParam p = (PBGParam)lParam;
			p->rt.OffsetRect(rt.left, rt.top);
		}
	}else if(LKCOMMON_MSG_WPARAM_RESETMEM == wParam)
	{
		// �ֶ�ˢ�±���
		ReSetMem();
		// �����ӿؼ�
		CWnd *pWnd = GetWindow(GW_CHILD);
		while(pWnd)
		{
			pWnd->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_RESETMEM);
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}
    }else if(LKCOMMON_MSG_WPARAM_DRAWDCNOBG == wParam)
    {
		CRect rt;
		GetRectInParent(&rt);
        BGParam *prm = (BGParam *)lParam;
        prm->rt.OffsetRect(rt.left, rt.top);
        OnDrawFGToDC(prm);
        rt = prm->rt;
        // �����ӿؼ�
        CWnd *pWnd = GetWindow(GW_CHILD);
        while(pWnd)
        {
            prm->rt = rt;
	        pWnd->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_DRAWDCNOBG, lParam);
	        pWnd = pWnd->GetWindow(GW_HWNDNEXT);
        }
	}else
    {
        ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, wParam, lParam);
    }
	return 1;
}

void CLKCtrlBase::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	lpncsp->rgrc[0].top    += m_rtClientOffset.top;		// 
	lpncsp->rgrc[0].left   += m_rtClientOffset.left;	// 
	lpncsp->rgrc[0].right  -= m_rtClientOffset.right;	//
	lpncsp->rgrc[0].bottom -= m_rtClientOffset.bottom;	// 
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CLKCtrlBase::OnSize(UINT nType, int cx, int cy)
{
	CRect rtOld(m_rtClient);
	CWnd::OnSize(nType, cx, cy);
	GetClientRect(&m_rtClient);
	if(m_rtClient.right <= 0 || m_rtClient.bottom <= 0 || m_rtClient.Width() == 0 || m_rtClient.Height() == 0) return;
	if(rtOld.Width() != m_rtClient.Width() || rtOld.Height() != m_rtClient.Height())
	{
		// ˢ��ǰ������
		ReSetMem();
	}
	// TODO: �ڴ˴������Ϣ����������
}


BOOL CLKCtrlBase::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(::LoadCursor(NULL, IDC_ARROW)); 
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

// CLKCtrlBase ��Ϣ�������	====����====
//=================================================================================================

LRESULT CLKCtrlBase::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_PAINT)
	{
		PAINTSTRUCT paint;
		CDC *pDC = BeginPaint(&paint);
		DWORD dw = ::GetTickCount();
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDC, m_rtClient.Width(), m_rtClient.Height());
		CBitmap *pOldBmp = memDC.SelectObject(&bmp);
		memDC.IntersectClipRect(&paint.rcPaint);
		if(m_bEraseBG)
		{
			m_bEraseBG = false;
			BGParam prm;
			prm.rt = CRect(0, 0, 0, 0);

			GetParentImage(&prm);
			CRect rt;
			GetRectInParent(&rt);
			prm.rt.OffsetRect(rt.left, rt.top);
			//memDC.BitBlt(0, 0, m_rtClient.Width(), m_rtClient.Height(), prm.pBG, prm.rt.left, prm.rt.top, SRCCOPY);
			CLKImage *pImg = (CLKImage *)prm.pBG;
			if(pImg)
			{
				pImg->BitBlt(memDC.GetSafeHdc(), 0, 0, m_rtClient.Width(), m_rtClient.Height(), prm.rt.left, prm.rt.top, SRCCOPY);
			}else
			{
				// û��ȡ�ñ���
				OnDrawNoParentBG(&memDC);
			}
		}else
		{
			memDC.BitBlt(0, 0, m_rtClient.Width(), m_rtClient.Height(), pDC, 0, 0, SRCCOPY);
		}
		static int n = 0;
		int nWidth = paint.rcPaint.right - paint.rcPaint.left;
		int nHeight = paint.rcPaint.bottom - paint.rcPaint.top;
		int nLeft = paint.rcPaint.left + m_rtClientOffset.left;
		int nTop = paint.rcPaint.top + m_rtClientOffset.top;
		if(m_pMemBmp)
		{
			//m_pMemBmp->AlphaBlend(memDC.GetSafeHdc(), paint.rcPaint.left, paint.rcPaint.top, nWidth, nHeight,  nLeft, nTop, nWidth, nHeight);
			m_pMemBmp->BitBlt(memDC.GetSafeHdc(), paint.rcPaint.left, paint.rcPaint.top, nWidth, nHeight,  nLeft, nTop, SRCCOPY);
			
		}
		OnPaintEx(pDC, &memDC);
		pDC->BitBlt(0, 0, m_rtClient.Width(), m_rtClient.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBmp);
		bmp.DeleteObject();
		memDC.DeleteDC();
		#ifdef LKBASECTRL_PAINT_LOG
			TRACE("CLKCtrlBase:  %d time: %ds; width: %d, height: %d\r\n", n++, ::GetTickCount() - dw, paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top);
		#endif
		EndPaint(&paint);
		return 1;
	}else if(message == WM_ERASEBKGND)
	{
		//if(CSystemInfo::GetSystemVer() > WINDOWS_XP)
		//{
		//	// �汾���ڣأУ�����win7
		//	PostMessage(WM_NCPAINT, 1);
		//}
		m_bEraseBG = true;
		CDC *pDC = CDC::FromHandle((HDC)wParam);
		return OnEraseBG(pDC);
	}else if(message == WM_NCPAINT)
	{
		if(m_rtClientOffset.left > 0 || m_rtClientOffset.top > 0 || m_rtClientOffset.right > 0 || m_rtClientOffset.bottom > 0)
		{
			CDC *pDC(0);
			pDC = GetWindowDC();
			if(pDC)
			{
				CRect rtWnd;
				GetWindowRect(&rtWnd);
				rtWnd.MoveToXY(0, 0);
				pDC->ExcludeClipRect(m_rtClientOffset.left, m_rtClientOffset.top, rtWnd.Width() - m_rtClientOffset.right, rtWnd.Height() - m_rtClientOffset.bottom);
				DrawNCArea(pDC);
				ReleaseDC(pDC);
			}
		}
		return 1;
	}else if(WM_MOVE == message)
	{
		ReSetMem();
	}else if(WM_CTLCOLOREDIT == message)
	{
		//if(CSystemInfo::GetSystemVer() > WINDOWS_XP)
		//{
		//	// �汾���ڣأУ�����win7
		//	SendMessage(WM_NCPAINT, 1);
		//}
	}else if(WM_PRINT == message)
    {
        return OnPrint(wParam, lParam);
    }
	return CWnd::WindowProc(message, wParam, lParam);
}


//=================================================================================================
// CLKCtrlBase �����麯��	====��ʼ====
// ��print����
int CLKCtrlBase::OnPrint(WPARAM wParam, LPARAM lParam)
{
	if(IsWindowVisible())
	{
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		CDC *pDC = CDC::FromHandle((HDC)wParam);
		OnPaint(pDC);
	}
    return 0;
}

// ������
void CLKCtrlBase::DrawBG(CDC *pDC)
{
	//GxGraphics graphics(*pDC);
	//graphics.
	//graphics.DrawImage(*m_pBGImg, 0, 0);
}

// ��ȡ��Ҫ������ͼƬ
CLKImage *CLKCtrlBase::GetParseImg()
{
	if(m_arrImage.GetCount() > m_nImageIndex && m_nImageIndex >= 0)
	{
		// Ĭ�ϵڶ���ͼƬ����Ҫ������ǰ��ͼƬ
		return m_arrImage[m_nImageIndex];
	}
	return 0;
}

// ���ǿͻ�����
void CLKCtrlBase::DrawNCArea(CDC *pDC)
{
	if(m_pMemBmp && m_pMemBmp->IsValid())
	{
		CRect rtDest;
		GetWindowRect(&rtDest);
		rtDest.MoveToXY(0, 0);
		m_pMemBmp->BitBlt(pDC->GetSafeHdc(), 0, 0, SRCCOPY);
	}else
	{
		//MessageBox(_T("aaaaaaa"));
	}
}

// ���������Ϣ
bool CLKCtrlBase::OnEraseBG(CDC *pDC)
{
	if(!GetMemImg())
	{
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParentImage(&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);
		CLKImage *pBG = (CLKImage *)prm.pBG;
		if(pBG && pBG->IsValid())
		{
			pBG->Draw(pDC->GetSafeHdc(), 0, 0, rt.Width(), rt.Height(), rt.left, rt.top, rt.Width(), rt.Height());
		}
	}/**/
	return 1;
}

// ��ȡ�㴰��ͼƬ
void CLKCtrlBase::GetParentImage(BGParam *prm)
{
	if(m_bGetParentBG)
	{
		GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETBGIMG, (LPARAM)prm);
	}else
	{
		GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)prm);
	}
}

// CLKCtrlBase �����麯��	====����====
//=================================================================================================

// ��ȡ�ؼ��ڸ������е�λ��(�����������λ�ã������ǿͻ���)
void CLKCtrlBase::GetRectInParent(CWnd *pWnd, CRect *rt)
{
	if(!pWnd || !pWnd->GetSafeHwnd() || !pWnd->GetParent()) return;
	pWnd->GetWindowRect(rt);
	CRect rtp;
	pWnd->GetParent()->GetWindowRect(&rtp);
	int nOffsetx = rt->left - rtp.left;
	int nOffsety = rt->top  - rtp.top;
	rt->MoveToXY(nOffsetx, nOffsety);
}

void CLKCtrlBase::GetRectInParent(CRect *rt)
{
	if(!GetParent()) return;
	GetWindowRect(rt);
	CRect rtp;
	GetParent()->GetWindowRect(&rtp);
	int nOffsetx = rt->left - rtp.left;
	int nOffsety = rt->top  - rtp.top;
	rt->MoveToXY(nOffsetx, nOffsety);
}

// ����Ĭ�Ͻ�
void CLKCtrlBase::CreateDefaultRoundWnd()
{
	// ��Բ��
	CRect rc;
	GetWindowRect(&rc);
    GetClientRect(&m_rtClient);
	// ֻ�е����ڷǿͻ����Ž��е���
	if(rc.Height() - m_rtClient.Height() > 2)
	{
		CRgn rgn, rgn2;
		rc.MoveToXY(0, 0);
		rgn.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //����һ��Բ�Ǿ���
		CRect rc2(rc);
		rc2.top = rc.bottom - 4;
		rgn2.CreateRectRgn(rc2.left, rc2.top, rc2.right, rc2.bottom);
		rgn.CombineRgn(&rgn, &rgn2, RGN_OR);
		SetWindowRgn(rgn,TRUE); 
	}
}

// �ü��ͻ���
void CLKCtrlBase::ClipClientRect(CRect rtOffset)
{
	CRect rWnd, rc;
	GetWindowRect(&rWnd);
    GetClientRect(&rc);

	rWnd.MoveToXY(0, 0);
    rc.MoveToXY(rtOffset.left, rtOffset.top);
    
	CRgn rgn, rgn2;
	rgn.CreateRectRgn(0, 0, 0, 0); 
    if(GetWindowRgn(rgn))
    {
        rgn2.CreateRectRgn(rc.left, rc.top, rWnd.right - rtOffset.right, rWnd.bottom - rtOffset.bottom);
        rgn.CombineRgn(&rgn, &rgn2, RGN_DIFF);
	    SetWindowRgn(rgn,TRUE); 
    }
}

// ��ȡ�ؼ��ڸ����ڿͻ����е�λ��
void CLKCtrlBase::GetRectInParentClient(CRect *rt)
{
	if(!GetParent()) return;
	GetWindowRect(rt);
	GetParent()->ScreenToClient(rt);
}


// ���Ҵ��ַ�����ʼƫ��λ�õ�����
int CLKCtrlBase::FindStringIndexByPos(CString strText, int nOffsetPos, CDC *pDC)
{
    // �����ַ�����
    CRect rtText(0, 0, 0, 0);
    int nIndex = 0, nLeft = 0, nRight = strText.GetLength();
    int nMid = (nRight - nLeft) / 2;
    CString strMid;
    while(nRight > nLeft)
    {
        if(nRight - nLeft == 1)
        {
            nIndex = nLeft;
            break;
        }
        strMid = strText.Left(nLeft + nMid);
        pDC->DrawText(strMid, strMid.GetLength(), &rtText, DT_CALCRECT);

        if(rtText.Width() == nOffsetPos)
        {
            nIndex = nLeft + nMid;
            break;
        }else if(rtText.Width() > nOffsetPos)
        {
            nRight -= nMid;
        }else
        {
            nLeft += nMid;
        }
        nMid = (nRight - nLeft) / 2;
    }
    return nIndex;
}
