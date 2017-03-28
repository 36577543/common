// DynDialogItemEx.cpp: implementation of the CDynDialogItemEx class.
//
// Written by Marcel Scherpenisse 
//        mailto:Marcel_Scherpenisse@insad.nl
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. If the source code in this file is used in 
// any commercial application then a statement along the lines of 
// "Portions copyright (c) Marcel Scherpenisse, 2002" must be included in 
// the startup banner, "About" box or printed documentation. An email 
// letting me know that you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
//////////////////////////////////////////////////////////////////////


//#include "stdafx.h"
#include "DynDialogEx.h"
#include "LKImageMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynDialogEx dialog


CDynDialogEx::CDynDialogEx(CWnd* pParent /*=NULL*/)
	: CDialog()
{
	//{{AFX_DATA_INIT(CDynDialogEx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//m_DialogTemplate.style = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | DS_SETFONT;
	m_DialogTemplate.style = WS_POPUP | WS_VISIBLE | DS_SETFONT | WS_DISABLED;
	m_DialogTemplate.dwExtendedStyle = WS_EX_DLGMODALFRAME;// | */WS_EX_LAYERED;
	m_DialogTemplate.x					= 0;
	m_DialogTemplate.y					= 0;
	m_DialogTemplate.cx					= 52; // 4 horizontal units are the width of one character
	m_DialogTemplate.cy					= 52; // 8 vertical units are the height of one character
	m_DialogTemplate.cdit				= 0;  // nr of dialog items in the dialog

	m_pParentWnd	= pParent;
	m_strCaption	= _T("");
	m_pFont			= NULL;
	m_wFontSize		= 0;
	m_nCurRow		= FIRSTROW1;
	m_bAddSystemButtons = TRUE;

	m_bIsFontCreated = FALSE;
	m_bModelessDlg = FALSE;
}

CDynDialogEx::~CDynDialogEx()
{
	CDynDialogItemEx *pDynDialogItemEx = NULL;
	for (int i = m_arrDlgItemPtr.GetSize() - 1; i >= 0; i--) {
		pDynDialogItemEx = m_arrDlgItemPtr[i];
		if (pDynDialogItemEx != NULL) {
			delete pDynDialogItemEx;
			pDynDialogItemEx = NULL;
		}
	}

	if (m_bIsFontCreated) {
		delete m_pFont;
	}
}

CWnd *CDynDialogEx::GetParent()
{
	return m_pParentWnd;
}

void CDynDialogEx::AddStyles(DWORD dwStyles)
{
	m_DialogTemplate.style |= dwStyles;
}

void CDynDialogEx::RemoveStyles(DWORD dwStyles)
{
	m_DialogTemplate.style &= (~dwStyles);
}

void CDynDialogEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynDialogEx)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	CDynDialogItemEx *pDynDialogItemEx = NULL;
	for (int i = m_arrDlgItemPtr.GetSize() - 1; i >= 0; i--) {
		pDynDialogItemEx = m_arrDlgItemPtr[i];
		if (pDynDialogItemEx != NULL) {
			pDynDialogItemEx->DoDataExchange(pDX);
		}
	}
}

BEGIN_MESSAGE_MAP(CDynDialogEx, CDialog)
	//{{AFX_MSG_MAP(CDynDialogEx)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_HELP, OnHelpMsg)
	ON_WM_MEASUREITEM()		// TMB! 06-12-2001 Adjusted.. was wrongly an ON_MESSAGE()!!
	ON_WM_DRAWITEM()			// TMB! 06-12-2001 Adjusted.. was wrongly an ON_MESSAGE()!!
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynDialogEx message handlers

int CDynDialogEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->cx = 52;
	lpCreateStruct->cy = 52;
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//if we have no font, create one here
	if (m_pFont == NULL) {
		LOGFONT LogFont;

		// Can do better???
		memset(&LogFont, 0x00, sizeof(LogFont));
		CString s = _T("MS Sans Serif");
		memcpy(LogFont.lfFaceName, s.GetBuffer(), (s.GetLength() + 1) * sizeof(WCHAR));
		LogFont.lfHeight = 8;

		m_pFont = new CFont;
		m_pFont->CreateFontIndirect(&LogFont);
		SetFont(m_pFont);
		m_bIsFontCreated = TRUE;
	}

	//Create all the controls on the dialog
	CDynDialogItemEx *pDynDialogItemEx = NULL;
	for (int i = 0; i < m_arrDlgItemPtr.GetSize(); i++) {
		pDynDialogItemEx = m_arrDlgItemPtr[i];
		if (pDynDialogItemEx != NULL) {
			if (!pDynDialogItemEx->IsDlgItemSubclassed()) {
				if (!pDynDialogItemEx->CreateEx(this)) {
					AfxMessageBox(_T("Failed to create DlgItem."));
				}
				else if (pDynDialogItemEx->GetSafeHwnd() != NULL) {
					pDynDialogItemEx->SetFont(m_pFont, FALSE);
				}
			}
		}
	}
	
	return 0;
}

BOOL CDynDialogEx::OnInitDialog() 
{
	//Reposition all the controls on the dialog...
	CDynDialogItemEx *pDynDialogItemEx = NULL;
	for (int i = 0; i < m_arrDlgItemPtr.GetSize(); i++) {
		pDynDialogItemEx = m_arrDlgItemPtr[i];
		if (pDynDialogItemEx != NULL) {
			if (!pDynDialogItemEx->IsDlgItemSubclassed() && !pDynDialogItemEx->SetWindowPos(this)) {
				CString strMessage;
				strMessage.Format(_T("Failed SetWindowPos for control %s."), pDynDialogItemEx->GetClassName());
				AfxMessageBox(strMessage);
			}
		}
	}

	CDialog::OnInitDialog();

	CenterWindow();
	
	//DoUpdateDummyDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CDynDialogEx::DoModal() 
{
	//Do we need OK and Cancel buttons??
	if (m_bAddSystemButtons) {
		AddSystemButtons();
	}

	//
	// Get font info from mainwindow of the application
	//
	CFont* pParentFont = m_pFont;
	if (pParentFont == NULL && m_pParentWnd != NULL) {
		pParentFont = m_pParentWnd->GetFont();
	}
	if (pParentFont == NULL && AfxGetApp()->m_pActiveWnd != NULL) {
		pParentFont = AfxGetApp()->m_pActiveWnd->GetFont();
	}
	LOGFONT LogFont;
	memset(&LogFont, 0x00, sizeof(LogFont));
	if (pParentFont != NULL) {
		pParentFont->GetLogFont(&LogFont);
	}
	else {
		// Can do better???
		CString s = _T("MS Sans Serif");
		memcpy(LogFont.lfFaceName, s.GetBuffer(), (s.GetLength() + 1) * sizeof(TCHAR));
		LogFont.lfHeight = 8;
	}

	////Prework for setting font in dialog...
	//int cWC = MultiByteToWideChar(CP_ACP, 0, LogFont.lfFaceName, -1, NULL, 0);
	//int nFontNameLen = cWC + 1;
	//WCHAR *szFontName = new WCHAR[nFontNameLen];
	//// Copy the string
	//MultiByteToWideChar(CP_ACP, 0, LogFont.lfFaceName, -1, (LPWSTR) szFontName, cWC);
	//szFontName[cWC] = 0;
	//nFontNameLen = (cWC) * sizeof(WCHAR);

	if (m_wFontSize == 0) {
		m_wFontSize = (unsigned short)LogFont.lfHeight;
	}
	CString strFontName = LogFont.lfFaceName;
	////Prework for setting caption in dialog...
	//cWC = MultiByteToWideChar(CP_ACP, 0, m_strCaption, -1, NULL, 0);
	//int szBoxLen = cWC + 1;
	//WCHAR *szBoxCaption = new WCHAR[szBoxLen];
	//// Copy the string
	//MultiByteToWideChar(CP_ACP, 0, m_strCaption, -1, (LPWSTR) szBoxCaption, cWC);
	//szBoxCaption[cWC] = 0;
	//szBoxLen = (cWC) * sizeof(WCHAR);

	int iRet = -1;
	//Here 's the stuff to build the dialog template in memory
	//without the controls being in the template
	//(Our first try, was this same template with some additional code
	//for each control placed on it, that's why this class is cold Ex :)
	//This gave some problems on WIN9x systems, where EDIT boxes
	//were not shown with 3D-look, but as flat controls)
	int nBufferSize = sizeof(DLGTEMPLATE)+(2 * sizeof(WORD)) /*menu and class*/ + m_strCaption.GetLength() * sizeof(WCHAR)  /*size of caption*/
		+sizeof(WORD) /*fontsize*/ + strFontName.GetLength() * sizeof(WCHAR) /*size of fontname*/;

	//Are there any subclassed controls...
	if (m_DialogTemplate.cdit > 0) {
		nBufferSize = (nBufferSize + 3) & ~3;  // adjust size to make first control DWORD aligned

		CDynDialogItemEx *pDynDialogItemEx = NULL;
		for (int i = 0; i < m_arrDlgItemPtr.GetSize(); i++) {
			pDynDialogItemEx = m_arrDlgItemPtr[i];
			if (pDynDialogItemEx != NULL) {
				if (pDynDialogItemEx->IsDlgItemSubclassed()) {
					int nItemLength = sizeof(DLGITEMTEMPLATE) + 3 * sizeof(WORD);
					nItemLength += (pDynDialogItemEx->GetCaptionLength() + 1) * sizeof(WCHAR);

					if (i != m_DialogTemplate.cdit - 1) {   // the last control does not need extra bytes
						nItemLength = (nItemLength + 3) & ~3;  // take into account gap so next control is DWORD aligned
					}
					nBufferSize += nItemLength;
				}
			}
		}
	}

	HLOCAL hLocal = LocalAlloc(LHND, nBufferSize);
	if (hLocal != NULL) {
		BYTE*	pBuffer = (BYTE*)LocalLock(hLocal);
		if (pBuffer == NULL) {
			LocalFree(hLocal);
			AfxMessageBox(_T("CDynDialogEx::DoModal() : LocalLock Failed"));
		}

		BYTE *pdest = pBuffer;
		// transfer DLGTEMPLATE structure to the buffer
		memcpy(pdest, &m_DialogTemplate, sizeof(DLGTEMPLATE));	// DLGTemplate
		pdest += sizeof(DLGTEMPLATE);
		*(WORD*)pdest = 0;									// no menu						 -- WORD to say it is 0 bytes
		pdest += sizeof(WORD);								// Increment
		*(WORD*)(pdest + 1) = 0;							// use default window class -- WORD to say it is 0 bytes
		pdest += sizeof(WORD);								// Increment
		memcpy(pdest, m_strCaption.GetBuffer(), m_strCaption.GetLength() * sizeof(WCHAR));			// Caption
		pdest += m_strCaption.GetLength() * sizeof(WCHAR);

		*(WORD*)pdest = m_wFontSize;						// font size
		pdest += sizeof(WORD);
		memcpy(pdest, strFontName.GetBuffer(), strFontName.GetLength() * sizeof(WCHAR));		// font name
		pdest += strFontName.GetLength() * sizeof(WCHAR);

		// will now transfer the information for each one of subclassed controls...
		if (m_DialogTemplate.cdit > 0) {
			CDynDialogItemEx *pDynDialogItemEx = NULL;
			for (int i = 0; i < m_arrDlgItemPtr.GetSize(); i++) {
				pDynDialogItemEx = m_arrDlgItemPtr[i];
				if (pDynDialogItemEx != NULL) {
					if (pDynDialogItemEx->IsDlgItemSubclassed()) {
						pdest = pDynDialogItemEx->FillBufferWithItemTemplate(pdest);
					}
				}
			}
		}
		ASSERT(pdest - pBuffer == nBufferSize); // just make sure we did not overrun the heap

		//Next lines to make sure that MFC makes no ASSERTION when PREVIOUS/NEXT is pressed:)
		if (m_lpDialogTemplate != NULL) {
			m_lpDialogTemplate = NULL;
		}

		//These are the MFC functions, which do the job...
		//if (m_bModelessDlg) {
			iRet = CreateIndirect((LPDLGTEMPLATE)pBuffer, m_pParentWnd);		
		//}
		//else {
		//	InitModalIndirect((LPDLGTEMPLATE)pBuffer, m_pParentWnd);
		//	iRet = CDialog::DoModal();
		//}

		LocalUnlock(hLocal);
		LocalFree(hLocal);

		//delete [] szBoxCaption;
		//delete [] szFontName;
		return iRet;
	}
	else {
		AfxMessageBox(_T("CDynDialogEx::DoModal() : LocalAllock Failed"));
		return -1;
	}
}

void CDynDialogEx::SetFont(CFont *pFont)
{
	m_pFont = pFont;
}

CFont *CDynDialogEx::GetFont()
{
	return m_pFont;
}

void CDynDialogEx::SetFontSize(WORD wSize)
{
	m_wFontSize = wSize;
}

WORD CDynDialogEx::GetFontSize()
{
	return m_wFontSize;
}

void CDynDialogEx::SetUseSystemButtons(BOOL bUse /*= TRUE*/)
{
	m_bAddSystemButtons = bUse;
}

void CDynDialogEx::GetDlgRect(LPRECT lpRect)
{
	ASSERT(lpRect);
	lpRect->left   = m_DialogTemplate.x;
	lpRect->top    = m_DialogTemplate.y;
	lpRect->right  = lpRect->left + m_DialogTemplate.cx;
	lpRect->bottom = lpRect->top + m_DialogTemplate.cy;
}

void CDynDialogEx::SetDlgRect(LPRECT lpRect)
{
	ASSERT(lpRect);
//#pragma warning(disable : 4244)
	m_DialogTemplate.cx = (short)(lpRect->right  - lpRect->left);
	m_DialogTemplate.cy = (short)(lpRect->bottom - lpRect->top);
	m_DialogTemplate.x  = (short)(lpRect->left);
	m_DialogTemplate.y  = (short)(lpRect->top);
//#pragma warning(default : 4244)
}

void CDynDialogEx::SetDlgRectangle(LPRECT pRect)
{
	RECT rect;

	GetDlgRect(&rect);
	if (rect.left > pRect->left) {
		rect.left = pRect->left;
	}
	if (rect.right < pRect->right) {
		rect.right = pRect->right + 5;
	}
	if (rect.top > pRect->top) {
		rect.top = pRect->top;
	}
	if (rect.bottom < pRect->bottom) {
		rect.bottom = pRect->bottom + 5;
	}
	SetDlgRect(&rect);
}

UINT CDynDialogEx::AddDlgControl(DLGITEMTEMPLATECONTROLS TypeControl,
											LPCTSTR lpszCaption,
											DWORD dwStyle,
											DWORD dwExtendedStyle,
											LPRECT pRect /*= NULL*/,
											void *pData /*= NULL*/,
											UINT nID /*= 0*/)
{
	UINT nRet = 0;
	//In case no rectangle given create our own...
	CRect Rect(FIXEDCOL1, m_nCurRow, FIXEDCOL2, m_nCurRow + ROWSTEPSIZE);

	//if no rectangle given use our own...
	if (pRect == NULL) {
		pRect = &Rect;
	}
//	else {
//		m_nCurRow = max(m_nCurRow, pRect->bottom) - m_nCurRow;
//	}

	m_nCurRow += (ROWSTEPSIZE);

	//Update dialogtemplate boundaries
	SetDlgRectangle(pRect);

	//Create control and add to array of controls
	CDynDialogItemEx *pDynDialogItemEx = new CDynDialogItemEx;
	if (pDynDialogItemEx != NULL) {
		nRet = pDynDialogItemEx->InitDialogItem(TypeControl, dwStyle, dwExtendedStyle, pRect, lpszCaption, nID, FALSE, pData);
		m_arrDlgItemPtr.Add(pDynDialogItemEx);
	}

	//Return ID of Control we created.
	return nRet;
}

UINT CDynDialogEx::AddDlgControl(LPCSTR lpszClassName,
											LPCTSTR lpszCaption,
											DWORD dwStyle,
											DWORD dwExtendedStyle,
											LPRECT pRect /*= NULL*/,
											void *pData /*= NULL*/,
											UINT nID /*= 0*/)
{
	UINT nRet = 0;
	//In case no rectangle given create our own...
	CRect Rect(FIXEDCOL1, m_nCurRow, FIXEDCOL2, m_nCurRow + ROWSTEPSIZE);

	//if no rectangle given use our own...
	if (pRect == NULL) {
		pRect = &Rect;
		Rect.right += INPUTCOL;
	}
//	else {
//		m_nCurRow = max(m_nCurRow, pRect->bottom) - m_nCurRow;
//	}

	m_nCurRow += (ROWSTEPSIZE);

	//Update dialogtemplate boundaries
	SetDlgRectangle(pRect);

	//Create control and add to array of controls
	CDynDialogItemEx *pDynDialogItemEx = new CDynDialogItemEx;
	if (pDynDialogItemEx != NULL) {
		nRet = pDynDialogItemEx->InitDialogItem(lpszClassName, dwStyle, dwExtendedStyle, pRect, lpszCaption, nID, FALSE, pData);
		m_arrDlgItemPtr.Add(pDynDialogItemEx);
	}

	//Return ID of Control we created.
	return nRet;
}

UINT CDynDialogEx::AddSubclassedDlgControl(LPCSTR lpszClassName,
														LPCTSTR lpszCaption,
														DWORD dwStyle,
														DWORD dwExtendedStyle,
														LPRECT pRect /*= NULL*/,
														UINT nID /*= 0*/)
{
	UINT nRet = 0;
	//In case no rectangle given create our own...
	CRect Rect(FIXEDCOL1, m_nCurRow, FIXEDCOL2, m_nCurRow + ROWSTEPSIZE);

	//if no rectangle given use our own...
	if (pRect == NULL) {
		pRect = &Rect;
		Rect.right += INPUTCOL;
	}
//	else {
//		m_nCurRow = max(m_nCurRow, pRect->bottom) - m_nCurRow;
//	}

	m_nCurRow += (ROWSTEPSIZE);

	//Update dialogtemplate boundaries
	SetDlgRectangle(pRect);

	//Create control and add to array of controls
	CDynDialogItemEx *pDynDialogItemEx = new CDynDialogItemEx;
	if (pDynDialogItemEx != NULL) {
		nRet = pDynDialogItemEx->InitDialogItem(lpszClassName, dwStyle, dwExtendedStyle, pRect, lpszCaption, nID, TRUE);
		m_arrDlgItemPtr.Add(pDynDialogItemEx);
		m_DialogTemplate.cdit++;
	}

	//Return ID of Control we created.
	return nRet;
}

void CDynDialogEx::AddSystemButtons()
{
	m_nCurRow += 6;			// Leave some room!
	CRect rect(FIXEDCOL1, m_nCurRow, FIXEDCOL2 - 60, m_nCurRow + (long)(ROWSTEPSIZE * 1.2));

	AddDlgControl(BUTTON, _T("OK"), STYLE_BUTTON, EXSTYLE_BUTTON, &rect, NULL, IDOK);
		
	// This has to be revised later.
	rect.left  += 55;
	rect.right += 55;
	AddDlgControl(BUTTON, _T("Annuleren"), STYLE_BUTTON, EXSTYLE_BUTTON, &rect, NULL, IDCANCEL);

}

void CDynDialogEx::SetWindowTitle(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;
}

void CDynDialogEx::SetUseModeless(BOOL bModelessDlg /*= TRUE*/)
{
	m_bModelessDlg = bModelessDlg;
}

void CDynDialogEx::OnCancel()
{
	if (m_bModelessDlg) {
		DestroyWindow();
	}
	else {
		CDialog::OnCancel();
	}
}

void CDynDialogEx::OnOK()
{
	if (m_bModelessDlg) {
		DestroyWindow();
	}
	else {
		CDialog::OnOK();
	}
}

BOOL CDynDialogEx::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	//wParam
	//The low-order word of wParam identifies the command ID of the menu item, control, or accelerator.
	//The high-order word of wParam specifies the notification message if the message is from a control.
	//If the message is from an accelerator, the high-order word is 1.
	//If the message is from a menu, the high-order word is 0.

	//lParam
	//Identifies the control that sends the message if the message is from a control. Otherwise, lParam is 0.

	WORD wControlID = LOWORD(wParam);
	WORD wMessageID = HIWORD(wParam);

	if (wControlID != 0) {
		switch (wControlID) {
		case IDOK:
			OnOK();
			return TRUE;
			break;
		case IDCANCEL:
			OnCancel();
			return TRUE;
			break;
		default:
			//if we have a parent send the message to the parent, so we can handle the message over there.
			if (m_pParentWnd != NULL) {
				::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_COMMAND, wParam, lParam);
			}
			break;
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

// Hellup!
afx_msg LRESULT CDynDialogEx::OnHelpMsg(WPARAM wParam, LPARAM lParam)
{
	//lParam		<<-- Contains: (LPHELPINFO)lParam
	// >> typedef  struct  tagHELPINFO { 
	//     UINT     cbSize; 
	//     int      iContextType 
	//     int      iCtrlId; 
	//     HANDLE   hItemHandle; 
	//     DWORD    dwContextId; 
	//     POINT    MousePos; 
	// } HELPINFO, FAR *LPHELPINFO;

	//
	// User pressed F1 in dialog, call the OnHelp() function, this can be overridden...
	//
	OnHelp();
	return TRUE;
}

// Default implementation of the OnHelp()
// This one is to be overridden by a caller ;)
void CDynDialogEx::OnHelp()
{
	CDialog::OnHelp();
}

bool CDynDialogEx::DoUpdateDummyDialog()
{
	BYTE SourceConstantAlpha = 255;
	// make sure the window has the WS_EX_LAYERED style
	ModifyStyleEx(0, WS_EX_LAYERED);
	// get the window rectangule (we are only interested in the top left position)
	CRect rectDlg(0, 0, m_nWidth, m_nHeight);
	//GetWindowRect(rectDlg);
	// Create/setup the DC's
	CDC dcScreen;
	CDC dcMemory;
	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);
	CBitmap bmpx;
	bmpx.CreateCompatibleBitmap(&dcScreen, rectDlg.Width(), rectDlg.Height());
	CBitmap *pOldBitmap = dcMemory.SelectObject(&bmpx);
	CRect rtWnd(rectDlg);
	rtWnd.MoveToXY(0, 0);
	OnDraw(&dcMemory, rtWnd);
	//dcMemory.MoveTo(0, 0);
	//dcMemory.LineTo(rtWnd.right, rtWnd.bottom);
	//pImg->Draw(dcMemory.GetSafeHdc(), 0, 0);

	// calculate the new window position/size based on the bitmap size
	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(rectDlg.Width(), rectDlg.Height());
	// setup the blend function
	BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0, 0); // start point of the copy from dcMemory to dcScreen
	// perform the alpha blend
	BOOL bRet = ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
	if (bRet == 0)
	{
		TRACE(_T("UpdateLayeredWindow %d\r\n"), ::GetLastError());
		return false;
	}
	//ASSERT(bRet); // ops... something bad just occured....
	// clean up
	dcMemory.SelectObject(pOldBitmap);
	//OnDrawItems(&dcScreen, rectDlg);
	return true;
}

void CDynDialogEx::OnDraw(CDC *pDC, CRect &rtWnd)
{
	
	//CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_ICONMESSAGE);
	//CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_SKINFRAME);
	CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_FOCUS);
	if (pImg)
	{
		AngleOffset angleOffset(0);
		//pImg->DrawToDC(pDC, rtWnd, 0);
		pImg->DrawToDC(pDC, rtWnd, 0, LKSKINBASE_DRAWBGSTYLE_ALL, 0, angleOffset);
		//pImg->DrawToDC(pDC, rtWnd, 1, LKSKINBASE_DRAWBGSTYLE_FRAME);
	}
}


void CDynDialogEx::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
	{
		//DoUpdateDummyDialog();
	}
	// TODO:  在此处添加消息处理程序代码
}


void CDynDialogEx::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	DoUpdateDummyDialog();
}


void CDynDialogEx::OnNcPaint()
{
	DoUpdateDummyDialog();
}


void CDynDialogEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonDblClk(nFlags, point);
}


void CDynDialogEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonDown(nFlags, point);
}


void CDynDialogEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonUp(nFlags, point);
}


void CDynDialogEx::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialog::OnNcLButtonDown(nHitTest, point);
}


LRESULT CDynDialogEx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO:  在此添加专用代码和/或调用基类
	//if (message >= WM_NCHITTEST && message < WM_NCMBUTTONDBLCLK)
	//{
	//	MessageBeep(20);
	//}
	if (message == WM_LBUTTONDOWN)
	{
		MessageBeep(20);
	}
	if (message == WM_LBUTTONUP)
	{
		MessageBeep(20);
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}


LRESULT CDynDialogEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCHITTEST)
	{
		MessageBeep(20);
	}
	if (message == WM_NCLBUTTONDOWN)
	{
		MessageBeep(20);
	}
	if (message == WM_LBUTTONUP)
	{
		MessageBeep(20);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


BOOL CDynDialogEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_NCHITTEST)
	{
		MessageBeep(20);
	}
	if (pMsg->message == WM_NCLBUTTONDOWN)
	{
		MessageBeep(20);
	}
	if (pMsg->message == WM_LBUTTONUP)
	{
		MessageBeep(20);
	}

	return CDialog::PreTranslateMessage(pMsg);
}
