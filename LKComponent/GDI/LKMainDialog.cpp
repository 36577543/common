// ..\..\common\LKComponent\GDI\LKMainDialog.cpp : ʵ���ļ�
//

#include "LKMainDialog.h"
#include "LKImageMgr.h"

// CLKMainDialog

IMPLEMENT_DYNAMIC(CLKMainDialog, CLKSlidDialog)

CLKMainDialog::CLKMainDialog(UINT nIDTemplate, CWnd* pParent)
: CLKSlidDialog(nIDTemplate, pParent)
, m_hTrayIcon(0)
, m_hTrayGrayIcon(0)
{
	SetStatusBarHeight(20);
}

CLKMainDialog::~CLKMainDialog()
{
}

// ��������״̬
void CLKMainDialog::UpdatTrayState()
{
	CString strText;
	GetWindowText(strText);
	// ��ͼ�����ϵͳ����
	m_ndTray.uFlags				= NIF_ICON | NIF_TIP;
	memcpy(m_ndTray.szTip, strText.GetBuffer(), (strText.GetLength() + 1) * sizeof(TCHAR));
	m_ndTray.hIcon				= m_hCurTrayIcon;
	Shell_NotifyIcon(NIM_MODIFY, &m_ndTray);
}

BEGIN_MESSAGE_MAP(CLKMainDialog, CLKSlidDialog)
	//ON_WM_DESTROY()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_LKTRAYNOTYFY, OnNotifyTray)
END_MESSAGE_MAP()



// CLKMainDialog ��Ϣ�������
// ��������ͼ����Ϣ�¼�����
LRESULT CLKMainDialog::OnNotifyTray(WPARAM wParam, LPARAM lParam)
{
    if(OnNotifyTrayOtherModule(wParam, lParam))
    {
        return 1;
    }
	if (lParam == WM_RBUTTONDOWN)
	{
        //OnTrayRButtonDown();
	}else if (lParam == WM_LBUTTONDOWN)
	{
		//::PostMessage(GetSafeHwnd(), WM_SYSCOMMAND, SC_RESTORE, 0);
		//SetForegroundWindow();
        OnTrayLButtonDown();
	}else if(lParam == WM_RBUTTONUP)
    {
        ::SetForegroundWindow(m_ndTray.hWnd);
        OnTrayRButtonUp();
    }else if(lParam == WM_LBUTTONDBLCLK)
    {
        OnTrayLDBClick();
    }
	return 1;
}

BOOL CLKMainDialog::OnInitDialog()
{
	CLKSlidDialog::OnInitDialog();
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//InitTray();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLKMainDialog::OnDestroy()
{
	// �ͷ�����ͼ��
	Shell_NotifyIcon(NIM_DELETE, &m_ndTray);
	m_hCurTrayIcon				= 0;
	CLKSlidDialog::OnDestroy();
}
 
// ��ʼ����������
void CLKMainDialog::InitBGBmp()
{
	CLKImage *pImage = m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_BG];
	if(pImage && pImage->IsValid())
	{
		// ������������
		CreateBGImage();
		DrawNCBG();

		// ���ڱ�����Դ�ϴ���ǰ�ͷŴ���Դ
		m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_BG] = 0;
		delete pImage;
		CLKImageMgr *p = CLKImageMgr::GetInstance();
		p->SetImage(LKGDIBASEDIALOG_IMAGEINDEX_BG + LKIMAGELISTINDEX_DIALOG_BEGIN);
	}
}

// ��С����ť������
void CLKMainDialog::OnMinClick()
{
	CRect   rcFrom,   rcTo;
	FindOutPositionOfIconDirectly(rcTo);
	//rcTo.right = rcTo.left;
	//rcTo.bottom = rcTo.top;
	GetWindowRect(&rcFrom);
	DrawAnimatedRects(IDANI_CAPTION, rcFrom, rcTo);
	ShowWindow(SW_HIDE);
}


////////////�������/////////////////////////////////////////////

// �ص�����������֪ͨ�����ҳ�����
BOOL CALLBACK CLKMainDialog::FindToolBarInTrayWnd(HWND hwnd, LPARAM lParam)
{    
	TCHAR szClassName[256];
    GetClassName(hwnd, szClassName, 255);    // Did we find the Main System Tray? If so, then get its size and quit
	if (_tcscmp(szClassName, _T("ToolbarWindow32")) == 0)    
	{        
		HWND* pWnd = (HWND*)lParam;
		*pWnd = hwnd;
        return FALSE;    
	}    
	return TRUE;
}

// �ص����������������ҳ�����֪ͨ����
BOOL CALLBACK CLKMainDialog::FindTrayWnd(HWND hwnd, LPARAM lParam)
{    
	TCHAR szClassName[256];
    GetClassName(hwnd, szClassName, 255);    // Did we find the Main System Tray? If so, then get its size and quit
	if (_tcscmp(szClassName, _T("TrayNotifyWnd")) == 0)    
	{        
		HWND* pWnd = (HWND*)lParam;
		*pWnd = hwnd;
        return FALSE;    
	}    
	return TRUE;
}

// ��ȡ����������
HWND CLKMainDialog::GetTrayNotifyWnd()
{
	HWND hWndTrayNotifyWnd = NULL;
	
	HWND hWndShellTrayWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
    if (hWndShellTrayWnd)    
	{        
		EnumChildWindows(hWndShellTrayWnd, FindTrayWnd, (LPARAM)&hWndTrayNotifyWnd);   
		
		if(hWndTrayNotifyWnd && IsWindow(hWndTrayNotifyWnd))
		{
			HWND hWndToolBarWnd = NULL;
			EnumChildWindows(hWndTrayNotifyWnd, FindToolBarInTrayWnd, (LPARAM)&hWndToolBarWnd);   
			if(hWndToolBarWnd)
			{
				return hWndToolBarWnd;
			}
		}

		return hWndTrayNotifyWnd;
	}  

	return hWndShellTrayWnd;
}

// ��ȡ��������
CRect CLKMainDialog::GetTrayWndRect()
{    
	CRect rect(0,0,0,0);
	
	HWND hWndTrayWnd = GetTrayNotifyWnd();
	if(hWndTrayWnd)
	{
		::GetWindowRect(hWndTrayWnd, &rect);
		return rect;
	}

	int nWidth  = CURSCREEN_WIDTH;
    int nHeight = CURSCREEN_HEIGHT;
    rect.SetRect(nWidth-40, nHeight-20, nWidth, nHeight);

	return rect;
}

// ��ȡӦ�ó���ָ������ͼ������
BOOL CLKMainDialog::FindOutPositionOfIconDirectly(CRect& a_rcIcon)
{
    //ProcessPrivilege(TRUE);
	//first of all let's find a Tool bar control embed in Tray window
	HWND hWndTray = GetTrayNotifyWnd();
    if (hWndTray == NULL)    
	{
		return FALSE;
	}

	//now we have to get an ID of the parent process for system tray
	DWORD dwTrayProcessID = -1;
	GetWindowThreadProcessId(hWndTray, &dwTrayProcessID);
	if(dwTrayProcessID <= 0)
	{
		return FALSE;
	}

	HANDLE hTrayProc = OpenProcess(PROCESS_ALL_ACCESS, 0, dwTrayProcessID);
	if(hTrayProc == NULL)
	{
        int n = ::GetLastError();
		return FALSE;
	}
 
	//now we check how many buttons is there - should be more than 0
	int iButtonsCount = ::SendMessage(hWndTray, TB_BUTTONCOUNT, 0, 0);

	//We want to get data from another process - it's not possible to just send messages like TB_GETBUTTON with a localy
	//allocated buffer for return data. Pointer to localy allocated data has no usefull meaning in a context of another
	//process (since Win95) - so we need to allocate some memory inside Tray process.
	//We allocate sizeof(TBBUTTON) bytes of memory - because TBBUTTON is the biggest structure we will fetch. But this buffer
	//will be also used to get smaller pieces of data like RECT structures.
	LPVOID lpData = VirtualAllocEx(hTrayProc, NULL, sizeof(TBBUTTON), MEM_COMMIT, PAGE_READWRITE);
	if( lpData == NULL || iButtonsCount < 1 )
	{
		CloseHandle(hTrayProc);
		return FALSE;
	}

	BOOL bIconFound = FALSE;

	for(int iButton=0; iButton<iButtonsCount; iButton++)
	{
		//first let's read TBUTTON information about each button in a task bar of tray

		DWORD dwBytesRead = -1;
		TBBUTTON buttonData;
		::SendMessage(hWndTray, TB_GETBUTTON, iButton, (LPARAM)lpData);
		ReadProcessMemory(hTrayProc, lpData, &buttonData, sizeof(TBBUTTON), &dwBytesRead);
		if(dwBytesRead < sizeof(TBBUTTON))
		{
			continue;
		}

		//now let's read extra data associated with each button: there will be a HWND of the window that created an icon and icon ID
		DWORD dwExtraData[2] = { 0,0 };
		ReadProcessMemory(hTrayProc, (LPVOID)buttonData.dwData, dwExtraData, sizeof(dwExtraData), &dwBytesRead);
		if(dwBytesRead < sizeof(dwExtraData))
		{
			continue;
		}

		HWND hWndOfIconOwner = (HWND) dwExtraData[0];
		int  iIconId		 = (int)  dwExtraData[1];
		
        if(hWndOfIconOwner != m_ndTray.hWnd || iIconId != m_ndTray.uID)
		{
			continue;
		}
		
		//we found our icon - in WinXP it could be hidden - let's check it:
		if( buttonData.fsState & TBSTATE_HIDDEN )
		{
			break;
		}

		//now just ask a tool bar of rectangle of our icon
		RECT rcPosition = {0,0};
		::SendMessage(hWndTray, TB_GETITEMRECT, iButton, (LPARAM)lpData);
		ReadProcessMemory(hTrayProc, lpData, &rcPosition, sizeof(RECT), &dwBytesRead);

		if(dwBytesRead < sizeof(RECT))
		{
			continue;
		}

		::MapWindowPoints(hWndTray, NULL, (LPPOINT)&rcPosition, 2);
		a_rcIcon = rcPosition;
		
		bIconFound = TRUE;
		break;
	}

	if(bIconFound == FALSE)
	{
		a_rcIcon = GetTrayWndRect(); //we failed to detect position of icon - let's return fail safe cooridinates of system tray
	}

	VirtualFreeEx(hTrayProc, lpData, NULL, MEM_RELEASE);
	CloseHandle(hTrayProc);

	return bIconFound;	
}
	
// ��ʼ������
void CLKMainDialog::InitTray()
{
	CString strText;
	GetWindowText(strText);
	// ��ͼ�����ϵͳ����
	//HICON h = (HICON) CopyImage(m_hSysIcon, IMAGE_ICON,0, 0, LR_COPYFROMRESOURCE | LR_MONOCHROME);
	m_ndTray.cbSize				= sizeof (NOTIFYICONDATA);
	m_ndTray.uID				= 200;
	m_ndTray.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	memcpy(m_ndTray.szTip, strText.GetBuffer(), (strText.GetLength() + 1) * sizeof(TCHAR));
	m_ndTray.hWnd				= GetSafeHwnd();
	m_ndTray.uCallbackMessage	= WM_LKTRAYNOTYFY;
	m_ndTray.hIcon				= m_hCurTrayIcon;
	Shell_NotifyIcon(NIM_ADD, &m_ndTray);
}

