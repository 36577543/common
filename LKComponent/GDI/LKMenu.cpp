// ..\..\common\LKComponent\GDI\LKMenu.cpp : 实现文件
//

#include "LKMenu.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"


// CLKMenu
bool CLKMenu::m_bIsRegedit = false;
WNDPROC CLKMenu::m_pOldMenuWndProc = 0;
void CLKMenu::RegeditMenu()
{
    if(!m_bIsRegedit)
    {
        WNDCLASS wc;
        if(GetClassInfo(NULL, _T("#32768"), &wc))
        {
            wc.style &= ~CS_DROPSHADOW;
            if(!m_pOldMenuWndProc)
            {
                m_pOldMenuWndProc = wc.lpfnWndProc;
            }

            wc.lpfnWndProc = (WNDPROC)NewMenuWndProc;
            SetLastError(0);
            if(RegisterClass(&wc)==0)
            {
                ::AfxMessageBox(_T(""));
            }else
            {
                m_bIsRegedit = true;
            }
        }
    }
}

void CLKMenu::UnRegeditMenu()
{
    if(m_bIsRegedit)
    {
        WNDCLASS wc;
        if(GetClassInfo(NULL, _T("#32768"), &wc))
        {
            wc.style |= CS_DROPSHADOW;
            if(m_pOldMenuWndProc)
            {
                wc.lpfnWndProc = m_pOldMenuWndProc;
            }

            if(RegisterClass(&wc)==0)
            {
                ::AfxMessageBox(_T(""));
            }else
            {
                m_pOldMenuWndProc = 0;
                m_bIsRegedit = false;
            }
        }
    }
}

LRESULT CALLBACK CLKMenu::NewWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CLKMenu *pThis = (CLKMenu *)GetWindowLong(hWnd, GWL_USERDATA);
    if(WM_MEASUREITEM == msg)
    {
        //TRACE(_T("WM_MEASUREITEM\r\n"));
		LPMEASUREITEMSTRUCT lpms = (LPMEASUREITEMSTRUCT)lParam;
        pThis->OnMeasureItem(lpms);
    }else if(WM_DRAWITEM == msg)
    {
        //TRACE(_T("WM_DRAWITEM\r\n"));
		LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT) lParam;
        if(ODT_MENU == lpdis->CtlType)
        {
            pThis->OnDrawItem(lpdis);
        }
    }
    //if(WM_ENTERIDLE == msg)
    //{
    //    if(MSGF_MENU == wParam)
    //    {
    //        //TRACE(_T("WM_ENTERIDLE\r\n"));
    //    }
    //}
    if(msg == WM_PRINT)
    {
        TRACE(_T("PARENT WM_PRINT: %d\r\n"), (int)lParam);
    }
	return ::CallWindowProc(pThis->m_pOldWndProc, hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CLKMenu::NewMenuWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //首先要去掉菜单窗口的WS_BORDER风格和WS_EX_DLGMODALFRAME   ,   WS_EX_WINDOWEDGE扩展风格
    //if(WM_CREATE == msg)
    //{
    //    //LPCREATESTRUCT lp = (LPCREATESTRUCT)lParam;
    //    //lp->cx = 500;
    //    LRESULT lResult = CallWindowProc(m_pOldMenuWndProc, hWnd, msg, wParam, lParam);
    //    DWORD dwStyle =   ::GetWindowLong(hWnd,   GWL_STYLE);
    //    DWORD dwNewStyle = (dwStyle   &   ~WS_BORDER);
    //    ::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle);
    //    DWORD dwExStyle = ::GetWindowLong(hWnd,   GWL_EXSTYLE);
    //    DWORD dwNewExStyle = (dwExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE));
    //    ::SetWindowLong(hWnd, GWL_EXSTYLE, dwNewExStyle);
    //    return   lResult;
    //}

    //最后,   因为你在MeasureItem里指定的菜单大小，系统会自动替你加上边框，我们必须去掉这部分额外的尺寸
    //if(WM_GETICON == msg)
    //{
    //    ::TRACE(_T("WM_GETICON\r\n"));
    //    //    return   0;
    //}
    if((WM_PAINT == msg) || (msg == WM_NCCALCSIZE) || (msg == WM_PRINT) || (WM_ERASEBKGND == msg) || (WM_NCPAINT == msg)
        || (msg == WM_PRINTCLIENT) || (WM_WINDOWPOSCHANGING == msg))
    {
        HMENU hMenu = (HMENU)SendMessage(hWnd, MN_GETHMENU, 0, 0);
        if(!hMenu)
        {
	        return ::CallWindowProc(m_pOldMenuWndProc, hWnd, msg, wParam, lParam);
        }
        MENUINFO mi;
        mi.cbSize = sizeof(MENUINFO);// 这句不能少！！！
        mi.fMask = MIM_MENUDATA;
        ::GetMenuInfo(hMenu, &mi);
        CLKMenu *pThis = (CLKMenu *)mi.dwMenuData;
        if(!pThis || !pThis->IsKindOf(RUNTIME_CLASS(CLKMenu)))
        {
	        return ::CallWindowProc(m_pOldMenuWndProc, hWnd, msg, wParam, lParam);
        }
        if(msg == WM_NCCALCSIZE)
        {
            NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*)lParam;
            lpncsp->rgrc[0].top    += pThis->m_rtOffset.top;		// 默认标题栏高度为0
            lpncsp->rgrc[0].left   += pThis->m_rtOffset.left;	// 默认左边框宽度为0
            lpncsp->rgrc[0].right  -= pThis->m_rtOffset.right;	// 默认右边框宽度为0 
            lpncsp->rgrc[0].bottom -= pThis->m_rtOffset.bottom;	// 默认底边框高度为0
            return 1;
        }

        if(msg == WM_PRINT)
        {
            TRACE(_T("WM_PRINT: %d\r\n"), (int)lParam);
            if(PRF_OWNED & lParam) return 0;
            pThis->m_bProcPrintMsg = true;
            CRect rtWnd;
            GetWindowRect(hWnd, &rtWnd);
            //if(rtWnd.right > 1280 || rtWnd.bottom > 800) return 1;
            CDC *pDC = CDC::FromHandle((HDC)wParam);
            //::SendMessage(hWnd, WM_NCPAINT, wParam, 0);
            pThis->OnPrint(pDC, hWnd);
            CRect rtClient;
            GetClientRect(hWnd, &rtClient);

            CDC memDC;
            memDC.CreateCompatibleDC(pDC);
            CBitmap bmp;
            bmp.CreateCompatibleBitmap(pDC, rtClient.Width(), rtClient.Height());

            CBitmap *pOld = memDC.SelectObject(&bmp);
            memDC.BitBlt(0, 0, rtClient.Width(), rtClient.Height(), pDC, pThis->m_rtOffset.left, pThis->m_rtOffset.top, SRCCOPY);
            
            LRESULT lResult = CallWindowProc(m_pOldMenuWndProc, hWnd, WM_PRINTCLIENT, (WPARAM)memDC.GetSafeHdc(), lParam);
            pDC->BitBlt(pThis->m_rtOffset.left, pThis->m_rtOffset.top, rtClient.Width(), rtClient.Height(), &memDC, 0, 0, SRCCOPY);
            //
            memDC.SelectObject(pOld);
            bmp.DeleteObject();
            memDC.DeleteDC();
            return lResult;
        }

        if(WM_PAINT == msg && !pThis->m_bProcPrintMsg)
        {
            TRACE(_T("WM_PAINT\r\n"));
		    PAINTSTRUCT paint;
            HDC hDC = ::BeginPaint(hWnd, &paint);
            ::SendMessage(hWnd, WM_PRINTCLIENT, (WPARAM)hDC, (LPARAM)14);
		    EndPaint(hWnd, &paint);
            return 1;//lResult;
        }
        if(WM_ERASEBKGND == msg && !pThis->m_bProcPrintMsg)
        {
            TRACE(_T("WM_ERASEBKGND\r\n"));
            return 1;
        }
        if(WM_NCPAINT == msg && !pThis->m_bProcPrintMsg)
        {
            TRACE(_T("WM_NCPAINT\r\n")); //PRF_ERASEBKGND
            if(wParam > 1)
            {
                pThis->OnPrint(CDC::FromHandle((HDC)wParam), hWnd);
            }else
            {
                HDC hdc = ::GetWindowDC(hWnd);
                // Paint into this DC 

                CDC *pDC = CDC::FromHandle(hdc);
                pThis->OnPrint(pDC, hWnd);
                ReleaseDC(hWnd, hdc);
            }
            return 1;
        }
        //if(msg == WM_PRINTCLIENT)
        //{
        //    TRACE(_T("WM_PRINTCLIENT: %d\r\n"), (int)lParam);
        //}
        //if(WM_MOVE == msg)
        //{
        //    TRACE(_T("WM_MOVE\r\n"));
        //}
        //if(WM_SIZE == msg)
        //{
        //    static int n(0);
        //    TRACE(_T("WM_SIZE: %d\r\n"), n++);
        //}
        if(WM_WINDOWPOSCHANGING == msg)
        {
            //将菜单大小改小
            LPWINDOWPOS   lpPos   =   (LPWINDOWPOS)lParam;
            int nX = GetSystemMetrics(SM_CXBORDER);
            int nY = GetSystemMetrics(SM_CYBORDER);
            lpPos-> cx   +=   4; //2*GetSystemMetrics(SM_CXBORDER) + 5;
            lpPos-> cy   +=   4; //2*GetSystemMetrics(SM_CYBORDER) + 5;
            LRESULT lResult   =   CallWindowProc(m_pOldMenuWndProc,   hWnd,   msg,   wParam,   lParam);
            //::TRACE(_T("WM_WINDOWPOSCHANGING%d-%d\r\n"), lpPos->cx, lpPos->cy);
            return   0;
        }
    }
	return ::CallWindowProc(m_pOldMenuWndProc, hWnd, msg, wParam, lParam);
}


IMPLEMENT_DYNAMIC(CLKMenu, CObject)
CLKMenu::CLKMenu()
: m_pOldWndProc(0)
, m_bProcPrintMsg(false)
, m_lOldUserData(0)
, m_pBGImg(0)
, m_nCaptionLeftMargin(31)
, m_rtOffset(5, 5, 5, 5)
, m_bIsDrawFixIcon(false)
{
    RegeditMenu();
    m_menu.CreatePopupMenu();
    MENUINFO mi;
    mi.cbSize = sizeof(MENUINFO);// 这句不能少！！！
    mi.fMask = MIM_MENUDATA;
    m_menu.GetMenuInfo(&mi);
    m_oldMenuData = mi.dwMenuData;
    mi.dwMenuData = (ULONG_PTR)this;
    if(!m_menu.SetMenuInfo(&mi))
    {
        ::AfxMessageBox(_T("SetMenuInfo fale;"));
    }

    // 背景
    mi.fMask = MIM_BACKGROUND;
    mi.hbrBack = (HBRUSH)GetStockObject(WHITE_BRUSH);
    m_menu.SetMenuInfo(&mi);
}

CLKMenu::~CLKMenu()
{
    // 释放菜单项
    for(UINT i = 0; i < m_menu.GetMenuItemCount(); i++)
    {
        //
        MENUITEMINFO mi;
        mi.cbSize = sizeof(MENUITEMINFO);
        mi.fMask = MIIM_FTYPE;
        if(m_menu.GetMenuItemInfo(i, &mi, true))
        {
            if(MFT_OWNERDRAW & mi.fType)
            {
                mi.fMask = MIIM_DATA;
                if(m_menu.GetMenuItemInfo(i, &mi, true))
                {
                    PLKMenuItem pItem = (PLKMenuItem)mi.dwItemData;
                    if(pItem)
                    {
                        delete pItem;
                    }
                }
            }
        }
    }
    MENUINFO mi;
    mi.cbSize = sizeof(MENUINFO);
    mi.fMask = MIM_MENUDATA;
    mi.dwMenuData = m_oldMenuData;
    m_menu.SetMenuInfo(&mi);
    m_menu.DestroyMenu();

    // 释放内存图片
    if(m_pBGImg)
    {
        if(m_pBGImg->IsValid())
        {
            m_pBGImg->Destroy();
        }
        delete m_pBGImg;
    }
}

bool CLKMenu::AddMenuItem(int nId, CString strCaption, CLKImage *pImg)
{
    int nCount = m_menu.GetMenuItemCount();
    PLKMenuItem pItem = new LKMenuItem;
    pItem->pImg = pImg;
    pItem->strCaption = strCaption;
    bool bRet = !!m_menu.AppendMenu(MF_OWNERDRAW/* | MF_STRING*/, nId, (LPCTSTR)pItem);
    return bRet;
}

// 更改项图标
void CLKMenu::SetItemImage(int nIndex, CLKImage *pImg)
{
    if(nIndex >= 0 && nIndex < m_menu.GetMenuItemCount())
    {
        MENUITEMINFO mi;
        mi.cbSize = sizeof(MENUITEMINFO);
        mi.fMask = MIIM_FTYPE;
        if(m_menu.GetMenuItemInfo(nIndex, &mi, true))
        {
            if(MFT_OWNERDRAW & mi.fType)
            {
                mi.fMask = MIIM_DATA;
                if(m_menu.GetMenuItemInfo(nIndex, &mi, true))
                {
                    PLKMenuItem pItem = (PLKMenuItem)mi.dwItemData;
                    if(pItem)
                    {
                        pItem->pImg = pImg;
                        //m_menu.SetMenuItemInfo(nIndex, &mi, true);
                    }
                }
            }
        }
    }
}

bool CLKMenu::TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect)
{
    m_pOldWndProc = (WNDPROC)::SetWindowLong(pWnd->GetSafeHwnd(), GWL_WNDPROC,(LONG)NewWndProc);
    LONG lOld = ::SetWindowLong(pWnd->GetSafeHwnd(), GWL_USERDATA, (LONG)this);

    bool bRet = !!m_menu.TrackPopupMenu(nFlags, x, y, pWnd, lpRect);

    ::SetWindowLong(pWnd->GetSafeHwnd(), GWL_USERDATA, lOld);
    ::SetWindowLong(pWnd->GetSafeHwnd(), GWL_WNDPROC,(LONG)m_pOldWndProc);

    return bRet;
}


// CLKMenu 消息处理程序

void CLKMenu::OnPrint(CDC *pDC, HWND hWnd)
{
    //::TRACE(_T("OnPrint\r\n"));
    CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_MENU_BG);
    if(pImg)
    {
        //HWND h = WindowFromDC(pDC->GetSafeHdc());
        if (IsWindow(hWnd))
        {
            CRect rtDest;
            GetWindowRect(hWnd, &rtDest);
            
            if(!m_pBGImg && pDC && pDC->GetSafeHdc())
            {
                m_pBGImg = new CLKImage(rtDest.Width(), rtDest.Height());
            }else
            {
                //::TRACE(_T("OnPrint\r\n"));
            }
            // 先画桌面背景到设备上下文中
            DrawDeskBGToImg(m_pBGImg, rtDest);
            rtDest.MoveToXY(0, 0);
            pImg->DrawToImage(m_pBGImg, 0, 0, LKSKINBASE_DRAWBGSTYLE_ALL);
            m_pBGImg->DrawToDC(pDC, rtDest, 0);
        }
    }
}

void CLKMenu::OnMeasureItem(LPMEASUREITEMSTRUCT lpms)
{
    //::TRACE(_T("OnMeasureItem\r\n"));
	lpms->itemWidth = 180;
	lpms->itemHeight = 22;
}

void CLKMenu::OnDrawItem(LPDRAWITEMSTRUCT lpdis)
{
    //::TRACE(_T("OnDrawItem\r\n"));
    CDC *pDC = CDC::FromHandle(lpdis->hDC);
	HWND hMenuWnd = WindowFromDC(lpdis->hDC);
	//if (IsWindow(hMenuWnd)) 
 //   {
	//	DWORD dwStyle =0;
	//	dwStyle = GetWindowLong(hMenuWnd, GWL_EXSTYLE);
	//	SetWindowLong(hMenuWnd, GWL_EXSTYLE, dwStyle|WS_EX_LAYERED);
	//	SetLayeredWindowAttributes(hMenuWnd, 0, 190, LWA_ALPHA);
	//}
    pDC->SetBkMode(TRANSPARENT);
    PLKMenuItem pItem = (PLKMenuItem)lpdis->itemData;

    CRect rtDest(lpdis->rcItem);
    m_pBGImg->BitBlt(lpdis->hDC, rtDest.left, rtDest.top, rtDest.Width(), rtDest.Height(), rtDest.left + m_rtOffset.left, rtDest.top + m_rtOffset.top, SRCCOPY);
    if(ODS_SELECTED & lpdis->itemState)
    {
        //::TRACE(_T("ODS_SELECTED width: %d, height: %d\r\n"), lpdis->rcItem.right - lpdis->rcItem.left, lpdis->rcItem.bottom - lpdis->rcItem.top);
        BGParam bgPrm;
        bgPrm.pBG = m_pBGImg;
        bgPrm.rt = rtDest;
        bgPrm.rt.OffsetRect(m_rtOffset.left, m_rtOffset.top);
        CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_SELECTANDFOCUS);
        pImg->DrawToDC(CDC::FromHandle(lpdis->hDC), rtDest, 0, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
    }else
    {
        //::TRACE(_T("OnDrawItem itemState: %d\r\n"), lpdis->itemState);
    }
    // 画项
    DrawItem(CDC::FromHandle(lpdis->hDC), pItem, rtDest);
}

// 画桌面背景到CLKImage中
void CLKMenu::DrawDeskBGToImg(CLKImage *pImg, CRect rt)
{
    //获取桌面窗口句柄
    HWND hWnd = GetDesktopWindow();
    //获取桌面窗口DC
    HDC hDC = GetWindowDC(hWnd);
    if(hDC)
    {
        if(CSystemInfo::GetSystemVer() > WINDOWS_XP)
        {
            CRect rtDesk;
            SystemParametersInfo(SPI_GETWORKAREA, 0, &rtDesk, 0);
            //int nXFrame = rt.Width() + 4;
            //int nYFrame = rt.Height() + 6;
            if(rt.right > rtDesk.Width() + GetSystemMetrics(SM_CXFRAME))
            {
                int nWidth = rt.Width();
                rt.right = rt.left;
                rt.left = rt.right - nWidth;
                rt.OffsetRect(GetSystemMetrics(SM_CXFRAME), 0);
            }
            if(rt.bottom > rtDesk.Height() + 4/* + GetSystemMetrics(SM_CYFRAME)*/)
            {
                int nHeight = rt.Height();
                rt.bottom = rt.top;
                rt.top = rt.bottom - nHeight;
                rt.OffsetRect(0, GetSystemMetrics(SM_CYFRAME) - 4);
            }
        }else
        {
            if(rt.right > CURSCREEN_WIDTH + GetSystemMetrics(SM_CXFRAME))
            {
                int nWidth = rt.Width();
                rt.right = rt.left;
                rt.left = rt.right - nWidth;
                rt.OffsetRect(GetSystemMetrics(SM_CXFRAME), 0);
            }
            if(rt.bottom > CURSCREEN_HEIGHT + GetSystemMetrics(SM_CYFRAME))
            {
                int nHeight = rt.Height();
                rt.bottom = rt.top;
                rt.top = rt.bottom - nHeight;
                rt.OffsetRect(0, GetSystemMetrics(SM_CYFRAME));
            }
        }
        pImg->DrawFromDC(/*pImg, */CDC::FromHandle(hDC), rt);

        ::ReleaseDC(hWnd, hDC);
    }
}

// 画项
void CLKMenu::DrawItem(CDC *pDC, PLKMenuItem pItem, CRect rtDest)
{
    CRect rtIcon(rtDest);
    rtIcon.right = rtIcon.left + 26;
    if(pItem->pImg)
    {
		if(m_bIsDrawFixIcon)
		{
			rtIcon.left += (rtIcon.Width() - 16) / 2;
			rtIcon.top += (rtIcon.Height() - 16) / 2;
			rtIcon.right = rtIcon.left + 16;
			rtIcon.bottom = rtIcon.top + 16;
		}else
		{
			rtIcon.left += (rtIcon.Width() - pItem->pImg->GetWidth()) / 2;
			rtIcon.top += (rtIcon.Height() - pItem->pImg->GetHeight()) / 2;
			rtIcon.right = rtIcon.left + pItem->pImg->GetWidth();
			rtIcon.bottom = rtIcon.top + pItem->pImg->GetHeight();
		}
        pItem->pImg->DrawToDC(pDC, rtIcon, 0);
    }

    CRect rtText(rtDest);
    rtText.left = m_nCaptionLeftMargin;

    CFont *pOldFont = pDC->SelectObject(CLKFontMgr::GetST90());
    pDC->DrawText(pItem->strCaption, &rtText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    pDC->SelectObject(pOldFont);
}
