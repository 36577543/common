#pragma once
#include "LKCtrlBase.h"

typedef struct _LKMenuItem
{
    CString     strCaption;
    CLKImage    *pImg;
}LKMenuItem, *PLKMenuItem;
typedef CArray<PLKMenuItem *, PLKMenuItem *> LKMenuItemArray;
// CLKMenu

class CLKMenu : public CObject//: public CMenu
{
	DECLARE_DYNAMIC(CLKMenu)

public:
	CLKMenu();
	virtual ~CLKMenu();
    static void RegeditMenu();
    static void UnRegeditMenu();
    bool AddMenuItem(int nId, CString strCaption = _T(""), CLKImage *pImg = 0);
	bool TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect = 0);
    // 更改项图标
    void SetItemImage(int nIndex, CLKImage *pImg);
protected:
    CMenu           m_menu;
	// 是否画固定大小的图标
	bool			m_bIsDrawFixIcon;
    virtual void OnPrint(CDC *pDC, HWND hWnd = 0);
    virtual void OnMeasureItem(LPMEASUREITEMSTRUCT lpms);
    virtual void OnDrawItem(LPDRAWITEMSTRUCT lpdis);
private:
    static bool m_bIsRegedit;
    static WNDPROC m_pOldMenuWndProc;
	static LRESULT CALLBACK NewMenuWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
private:
    // 是否响应过wm_print消息
    bool            m_bProcPrintMsg;
    // 边框宽度
    CRect           m_rtOffset;
    // 文本的左边距
    int             m_nCaptionLeftMargin;
    // 背景
    CLKImage        *m_pBGImg;
    ULONG_PTR       m_oldMenuData;
    LKMenuItemArray m_ltImage;


    WNDPROC m_pOldWndProc;
    LONG    m_lOldUserData;
	static LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    // 画桌面背景到CLKImage中
    void DrawDeskBGToImg(CLKImage *pImg, CRect rt);
    // 画项
    void DrawItem(CDC *pDC, PLKMenuItem pItem, CRect rtDest);
};


