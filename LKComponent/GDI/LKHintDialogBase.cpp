// ..\..\..\common\LKComponent\gdi\LKHintDialogBase.cpp : ʵ���ļ�
//

#include "LKHintDialogBase.h"
#include "LKImageMgr.h"

// �رռ�ⶨʱ����ʶ
#define LKHINTDIALOGBASE_TIMERID_CLOSE          1000
// �رռ�ⶨʱ��ɨ����(����)
#define LKHINTDIALOGBASE_TIMERINTERVAL_CLOSE    500

// CLKHintDialogBase �Ի���

IMPLEMENT_DYNAMIC(CLKHintDialogBase, CDialog)

CLKHintDialogBase::CLKHintDialogBase(UINT nIDTemplate, CWnd* pParent)
: CDialog(nIDTemplate, pParent)
, m_nIDTemplate(nIDTemplate)
, m_bIsLeave(true)
, m_nCurItemIndex(-1)
, m_bMouseDown(false)
// ����ƿ��Ƿ��Զ����ش���
, m_bAutoHideByMouseLeave(true)
{
    // ����ͼƬ
    m_pBGImg = CLKImageMgr::GetImageS(LKIMAGEHINTDIALOG_COMMON_BG);
}

CLKHintDialogBase::~CLKHintDialogBase()
{
}

BOOL CLKHintDialogBase::Create(CWnd* pParentWnd)
{
    return Create(m_nIDTemplate, pParentWnd);
}
BOOL CLKHintDialogBase::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
    m_nIDTemplate = nIDTemplate;
    BOOL bRet = CDialog::Create(nIDTemplate, pParentWnd);
    if(bRet)
    {
        ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
        //DoUpdateDummyDialog();
    }
    return bRet;
}
// �������ñ���ͼƬ
void CLKHintDialogBase::SetBGImage(CLKImage *pBGImg)
{
    m_pBGImg = pBGImg;
    if(GetSafeHwnd())
    {
        DoUpdateDummyDialog();
    }
}

// ��ʱ������ʾ����()
void CLKHintDialogBase::DelayHideHintDlg()
{
    SetTimer(LKHINTDIALOGBASE_TIMERID_CLOSE, LKHINTDIALOGBASE_TIMERINTERVAL_CLOSE, 0);
}
void CLKHintDialogBase::DelayHideHintDlg(int nElapse)
{
    SetTimer(LKHINTDIALOGBASE_TIMERID_CLOSE, nElapse, 0);
}

// ����һ���ַ�
void CLKHintDialogBase::AddStringItem(CRect rtItem, CString strItem, COLORREF crText)
{
    AddStringItem(m_arrItem.GetCount(), rtItem, strItem, crText);
}
void CLKHintDialogBase::AddStringItem(int nId, CRect rtItem, CString strItem, COLORREF crText)
{
    HintDlgDataItem item;
    item.nIndex = nId;
    item.nType = 0;
    item.pImg = 0;
    item.rtItem = rtItem;
    item.strText = strItem;
    item.crText = crText;
    m_arrItem.Add(item);
}
// ����һ�ť
void CLKHintDialogBase::AddButtonItem(CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText)
{
    AddButtonItem(m_arrItem.GetCount(), rtItem, strItem, pImg, crText);
}

void CLKHintDialogBase::AddButtonItem(int nId, CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText)
{
    HintDlgDataItem item;
    item.nIndex = nId;
    item.nType = 1;
    item.pImg = pImg;
    item.rtItem = rtItem;
    item.strText = strItem;
    item.crText = crText;
    m_arrItem.Add(item);
}

// ����һ���ƽ��ť
void CLKHintDialogBase::AddFlatButtonItem(CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText)
{
    AddFlatButtonItem(m_arrItem.GetCount(), rtItem, strItem, pImg, crText);
}
void CLKHintDialogBase::AddFlatButtonItem(int nId, CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText)
{
    HintDlgDataItem item;
    item.nIndex = nId;
    item.nType = 2;
    item.pImg = pImg;
    item.rtItem = rtItem;
    item.strText = strItem;
    item.crText = crText;
    m_arrItem.Add(item);
}

BEGIN_MESSAGE_MAP(CLKHintDialogBase, CDialog)
    ON_WM_SIZE()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
//    ON_WM_NCMOUSEMOVE()
ON_WM_SHOWWINDOW()
ON_WM_TIMER()
ON_WM_MOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CLKHintDialogBase::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    //if(GetSafeHwnd())
    //{
    //    DoUpdateDummyDialog();
    //}
    // TODO: �ڴ˴������Ϣ����������
}

void CLKHintDialogBase::OnDraw(CDC *pDC, CRect rt)
{
    OnDrawBG(pDC, rt);
    OnDrawItems(pDC, rt);
}

// ��Ĭ�ϱ���
void CLKHintDialogBase::OnDrawBG(CDC *pDC, CRect rt)
{
    if(m_pBGImg && !rt.IsRectEmpty())
    {
        //m_pBGImg->DrawText(_T("aaaaaaaaaaa"), CRect(0, 10, 100, 30), CLKFontMgr::GetST90());
        m_pBGImg->DrawToDC(pDC, rt, 0, LKSKINBASE_DRAWBGSTYLE_ALL); 
    }
}

// CLKHintDialogBase ��Ϣ�������
bool CLKHintDialogBase::DoUpdateDummyDialog()
{
    BYTE SourceConstantAlpha = 255;
	// make sure the window has the WS_EX_LAYERED style
	ModifyStyleEx(0, WS_EX_LAYERED);
	// get the window rectangule (we are only interested in the top left position)
	CRect rectDlg;
	GetWindowRect(rectDlg);
	// Create/setup the DC's
	CDC dcScreen;
	CDC dcMemory;
	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);
    CBitmap bmpx;
    bmpx.CreateCompatibleBitmap(&dcScreen, rectDlg.Width(), rectDlg.Height()); 
	CBitmap *pOldBitmap= dcMemory.SelectObject(&bmpx);
    CRect rtWnd(rectDlg);
    rtWnd.MoveToXY(0, 0);
    OnDraw(&dcMemory, rtWnd);
    //pImg->Draw(dcMemory.GetSafeHdc(), 0, 0);

	// calculate the new window position/size based on the bitmap size
	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(rectDlg.Width(), rectDlg.Height());
	// setup the blend function
	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0,0); // start point of the copy from dcMemory to dcScreen
	// perform the alpha blend
    BOOL bRet= ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
    if(bRet == 0)
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


void CLKHintDialogBase::OnMouseLeave()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    m_bIsLeave = true;
    m_bMouseDown = false;
    if(-1 != m_nCurItemIndex)
    {
        m_nCurItemIndex = -1;
        DoUpdateDummyDialog();
    }
    if(m_bAutoHideByMouseLeave)
    {
        DelayHideHintDlg();
    }
    CDialog::OnMouseLeave();
}

void CLKHintDialogBase::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if(m_bIsLeave)
    {
        TRACKMOUSEEVENT   tme;   
        tme.cbSize		= sizeof(tme);   
        tme.dwFlags		= TME_LEAVE; 
        tme.hwndTrack	= this->GetSafeHwnd();   
        tme.dwHoverTime = HOVER_DEFAULT; //ֻ��HOVER��Ч
        ::TrackMouseEvent(&tme);
        m_bIsLeave = false;
        KillTimer(LKHINTDIALOGBASE_TIMERID_CLOSE);
    }

    // ����������
    int nIndex = -1;
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        HintDlgDataItem &item = m_arrItem[i];
        if(item.nType != 0 && item.rtItem.PtInRect(point))
        {
            nIndex = item.nIndex;
            break;
        }
    }
    if(nIndex != m_nCurItemIndex)
    {
        m_bMouseDown = false;
        m_nCurItemIndex = nIndex;
        DoUpdateDummyDialog();
    }
    CDialog::OnMouseMove(nFlags, point);
}

void CLKHintDialogBase::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
}

void CLKHintDialogBase::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    switch(nIDEvent)
    {
    case LKHINTDIALOGBASE_TIMERID_CLOSE:
        {
            KillTimer(LKHINTDIALOGBASE_TIMERID_CLOSE);
            ShowWindow(SW_HIDE);
        }
        break;
    }
    CDialog::OnTimer(nIDEvent);
}

void CLKHintDialogBase::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    KillTimer(LKHINTDIALOGBASE_TIMERID_CLOSE);
}

void CLKHintDialogBase::OnDrawItems(CDC *pDC, CRect rt)
{
    pDC->SetBkMode(TRANSPARENT);

	COLORREF crText = CLKFontMgr::GetDefaultTextColor();
    pDC->SetTextColor(crText);
    CFont *pFont = pDC->SelectObject(CLKFontMgr::GetST90());
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        HintDlgDataItem &item = m_arrItem[i];
        CRect rtItem(item.rtItem);
        rtItem.OffsetRect(rt.left, rt.top);
        if(0 == item.nType)
        {
            // ���ַ���
            COLORREF cr = pDC->SetTextColor(item.crText);
            pDC->DrawText(item.strText, &rtItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            pDC->SetTextColor(cr);
        }else if(1 == item.nType)
        {
            // ����ͨ��ť
            DrawButton(&item, pDC, rtItem);
        }else if(2 == item.nType)
        {
            // ����ƽ��ť
            DrawFlatButton(&item, pDC, rtItem);
        }
    }
    pDC->SelectObject(pFont);
    CBitmap *pBmp = pDC->GetCurrentBitmap();

    // ȥ���ַ���͸��������
    BITMAP bmp;
    pBmp->GetBitmap(&bmp);
    int nPP = bmp.bmBitsPixel / 8;
    int nCount = bmp.bmWidth * bmp.bmHeight * nPP;
    BYTE *pBuf = new BYTE[nCount];
    pBmp->GetBitmapBits(nCount, pBuf);
    BYTE bAlpha = 0;
    COLORREF cor;
    for(int i = 0; i < nCount; i += 4)
    {
        bAlpha = pBuf[i + 3];
        cor = RGB(pBuf[i], pBuf[i + 1], pBuf[i + 2]);
        if(bAlpha == 0 && 0 != cor)
        {
            pBuf[i + 3] = 255;
        }
    }
    pBmp->SetBitmapBits(nCount, pBuf);
    delete []pBuf;
}

// ����ͨ��ť
void CLKHintDialogBase::DrawButton(HintDlgDataItem *pItem, CDC *pDC, CRect rtItem)
{
    CRect rtText(rtItem);
    CRect rtIcon(rtItem);
    rtText.DeflateRect(2, 2, 2, 2);
    // ������(3̬С��ť����)
    if(pItem->nIndex == m_nCurItemIndex)
    {
        // ��������
        //SHORT nKey = GetAsyncKeyState(VK_LBUTTON);
        int nImgIndex = 1;
        if(m_bMouseDown)
        {
            nImgIndex = 2;
            rtText.OffsetRect(1, 1);
            rtIcon.OffsetRect(1, 1);
        }
        CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_BUTTON_BEGIN + 2);
        pImg->DrawToDC(pDC, rtItem, nImgIndex, LKSKINBASE_DRAWBGSTYLE_ALL);
    }else
    {
        // ��̬����
        CLKImage *pImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_BUTTON_BEGIN + 2);
        pImg->DrawToDC(pDC, rtItem, 0, LKSKINBASE_DRAWBGSTYLE_ALL);
    }
   
    if(pItem->pImg)
    {
        // ��ͼ��
        pItem->pImg->Draw(pDC->GetSafeHdc(), rtIcon.left + 2, rtIcon.top + 2);
        rtText.left += pItem->pImg->GetWidth() + 2;
    }
    // ���ı�
    if(!pItem->strText.IsEmpty())
    {
        pDC->DrawText(pItem->strText, &rtText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

// ����ƽ��ť
void CLKHintDialogBase::DrawFlatButton(HintDlgDataItem *pItem, CDC *pDC, CRect rtItem)
{
    // ������
    if(pItem->nIndex == m_nCurItemIndex)
    {
        // 
        //SHORT nKey = GetAsyncKeyState(VK_LBUTTON);
        if(m_bMouseDown)
        {
            rtItem.OffsetRect(1, 1);
        }
    }
   
    CRect rtText(rtItem);
    rtText.DeflateRect(2, 2, 2, 2);
    if(pItem->pImg)
    {
        // ��ͼ��
        pItem->pImg->Draw(pDC->GetSafeHdc(), rtItem.left + 2, rtItem.top + 2);
        rtText.left += pItem->pImg->GetWidth() + 2;
    }
    // ���ı�
    if(!pItem->strText.IsEmpty())
    {
        pDC->DrawText(pItem->strText, &rtText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void CLKHintDialogBase::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if(-1 != m_nCurItemIndex)
    {
        m_bMouseDown = true;
        DoUpdateDummyDialog();
    }
    CDialog::OnLButtonDown(nFlags, point);
}

void CLKHintDialogBase::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if(-1 != m_nCurItemIndex && m_bMouseDown)
    {
        m_bMouseDown = false;
        DoUpdateDummyDialog();
    }
    CDialog::OnLButtonUp(nFlags, point);
}
