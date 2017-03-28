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
    // ������ͼ��
    void SetItemImage(int nIndex, CLKImage *pImg);
protected:
    CMenu           m_menu;
	// �Ƿ񻭹̶���С��ͼ��
	bool			m_bIsDrawFixIcon;
    virtual void OnPrint(CDC *pDC, HWND hWnd = 0);
    virtual void OnMeasureItem(LPMEASUREITEMSTRUCT lpms);
    virtual void OnDrawItem(LPDRAWITEMSTRUCT lpdis);
private:
    static bool m_bIsRegedit;
    static WNDPROC m_pOldMenuWndProc;
	static LRESULT CALLBACK NewMenuWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
private:
    // �Ƿ���Ӧ��wm_print��Ϣ
    bool            m_bProcPrintMsg;
    // �߿���
    CRect           m_rtOffset;
    // �ı�����߾�
    int             m_nCaptionLeftMargin;
    // ����
    CLKImage        *m_pBGImg;
    ULONG_PTR       m_oldMenuData;
    LKMenuItemArray m_ltImage;


    WNDPROC m_pOldWndProc;
    LONG    m_lOldUserData;
	static LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    // �����汳����CLKImage��
    void DrawDeskBGToImg(CLKImage *pImg, CRect rt);
    // ����
    void DrawItem(CDC *pDC, PLKMenuItem pItem, CRect rtDest);
};


