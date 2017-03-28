// ..\..\..\common\LKComponent\gdi\LKDrawerItem.cpp : 实现文件
//

#include "LKDrawerItem.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"


// CLKDrawerItem

IMPLEMENT_DYNAMIC(CLKDrawerItem, CLKCtrlBase)

CLKDrawerItem::CLKDrawerItem(int nImageIndex)
: CLKCtrlBase(nImageIndex)
, m_bExpand(false)
, m_rtCollect(0, 0, 0, 0)
, m_rtExpand(0, 0, 0, 0)
, m_nCurIndex(-2)
, m_nCurDownIndex(-2)
, m_bMouseDown(false)
, m_pImgButton(0)
, m_nMargin(3)
, m_nImageTextSplit(1)
, m_nButtonSplit(1)
, m_pChildWnd(0)
{

}

CLKDrawerItem::~CLKDrawerItem()
{
	CLKImage *pImg = GetMemImg();
    if(pImg)
    {
        delete pImg;
    }

    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        delete m_arrItem[i];
    }
    m_arrItem.RemoveAll();
}

BOOL CLKDrawerItem::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, pParentWnd, nID); 
	return bRet;
}

// 设置展开状态
void CLKDrawerItem::SetExpandState(bool bExpand)
{
    if(m_bExpand != bExpand)
    {
        m_bExpand = bExpand;
    }
}

// 增加一个按钮
void CLKDrawerItem::AddButton(CString strCaption, CLKImage *pImg, int nButtonType)
{
    int nIndex = m_arrItem.GetCount();
    AddButton(nIndex, strCaption, pImg, nButtonType);
}

void CLKDrawerItem::AddButton(int nIndex, CString strCaption, CLKImage *pImg, int nButtonType, int nSelect)
{
    PDrawerItemButton pItem = new DrawerItemButton;
    pItem->nId = nIndex;
    pItem->pImg = pImg;
    pItem->nType = nButtonType;
    pItem->strCaption = strCaption;
    pItem->nSelect = nSelect;
    m_arrItem.Add(pItem);

    // 计算按钮区域
    CalcItemRect();
}

// 设置子窗口
void CLKDrawerItem::SetChildWnd(CLKCtrlBase *pChildWnd)
{
    if(m_pChildWnd)
    {
        m_pChildWnd->SetWindowPos(0, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
        m_pChildWnd->SetParent(0);
    }
    m_pChildWnd = pChildWnd;
}

BEGIN_MESSAGE_MAP(CLKDrawerItem, CLKCtrlBase)
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_SIZE()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CLKDrawerItem 消息处理程序
LRESULT CLKDrawerItem::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
    return CLKCtrlBase::OnCommonMsg(wParam, lParam);
}

void CLKDrawerItem::OnSize(UINT nType, int cx, int cy)
{
    CLKCtrlBase::OnSize(nType, cx, cy);

    if(m_pChildWnd && m_pChildWnd->GetSafeHwnd() && m_bExpand)
    {
        CRect rtClient(m_rtClient);
        rtClient.top += m_nHeadHeight;
        m_pChildWnd->MoveWindow(&rtClient);
        //m_pChildWnd->SetWindowPos(0, rtClient.left, rtClient.top, rtClient.Width(), rtClient.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
    }
    // TODO: 在此处添加消息处理程序代码
}

// 刷新前景缓存
void CLKDrawerItem::ReSetMem()
{
	ParseImageToMem();
	if(IsWindowVisible())
	{
		Invalidate();
	}
}

// 初始化图片列表
void CLKDrawerItem::InitImageList()
{
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_DRAWERITEM_BEGIN, LKIMAGELISTINDEX_DRAWERITEM_END);
	CLKImage *pImage = GetParseImg();
	if(pImage)
	{
        m_nHeadHeight = pImage->GetHeight() / pImage->GetImageNumber();
	}

    // 按钮背景图片
    m_pImgButton = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_BUTTON_BEGIN + 1);
}

// 创建控件后初始化
void CLKDrawerItem::InitCtrlAfterCreate()
{
	ParseImageToMem();
}

// 在缓存上绘图
void CLKDrawerItem::OnPaint(CDC *pDC)
{
	if(GetParseImg())
	{
		CRect rtDest(m_rtClient);
        rtDest.bottom = rtDest.top + m_nHeadHeight;
        int nIndex;
        if(m_nCurIndex >= -1)
        {
            if(m_bExpand)
            {
                nIndex = 3;
            }else
            {
                nIndex = 1;
            }
        }else if(m_bExpand)
        {
            nIndex = 2;
        }else
        {
            nIndex = 0;
        }
        CLKImage *pImg = GetParseImg();
        if(pImg)
        {
            pImg->DrawToDC(pDC, rtDest, nIndex, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
        }
	}

    CRect rtText(m_rtClient);
    rtText.left = 20;
    rtText.top += 5;
    // 画标签文本
    DrawCaption(pDC, rtText);

    // 画按钮
    DrawButtons(pDC);
}
//
// 画print背景
int CLKDrawerItem::OnPrint(WPARAM wParam, LPARAM lParam)
{
    BGParam rm;
    GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&rm);
	CRect rt;
	GetRectInParent(&rt);
	rm.rt.OffsetRect(rt.left, rt.top);
    CDC *pDC = CDC::FromHandle((HDC)wParam);
    CLKImage *pImg = (CLKImage *)rm.pBG;
    pImg->Draw(pDC->GetSafeHdc(), 0, 0, m_rtClient.Width(), m_rtClient.Height(), rm.rt.left, rm.rt.top, m_rtClient.Width(), m_rtClient.Height());
    OnPaint(pDC);
    //return CLKCtrlBase::OnPrint(wParam, lParam);
    return CWnd::WindowProc(WM_PRINT, wParam, lParam);
    //return 0;
}


// 将前景画在指定的CDC设置上(响应wm_print)
void CLKDrawerItem::OnDrawFGToDC(BGParam *prm)
{
    CDC *pDC = (CDC *)prm->pBG;
	if(GetParseImg())
	{
		CRect rtDest(m_rtClient);
        rtDest.MoveToXY(prm->rt.left, prm->rt.top);
        rtDest.bottom = rtDest.top + m_nHeadHeight;
        int nIndex;
        if(m_nCurIndex >= -1)
        {
            if(m_bExpand)
            {
                nIndex = 3;
            }else
            {
                nIndex = 1;
            }
        }else if(m_bExpand)
        {
            nIndex = 2;
        }else
        {
            nIndex = 0;
        }
        CLKImage *pImg = GetParseImg();
        if(pImg)
        {
            pImg->DrawToDC(pDC, rtDest, nIndex, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
        }
	}
}


// 解析出相关图片到内存缓存中
void CLKDrawerItem::ParseImageToMem()
{
    // 计算按钮区域
    CalcItemRect();
}


void CLKDrawerItem::OnMouseMove(UINT nFlags, CPoint point)
{
	int nIndex = GetItemIndexByPoint(point);
	if(nIndex != m_nCurIndex)
	{
        m_nCurIndex = nIndex;
        Invalidate();
		if(nIndex >= -1)
		{
			TRACKMOUSEEVENT   tme;   
			tme.cbSize		= sizeof(tme);   
			tme.dwFlags		= TME_LEAVE; 
			tme.hwndTrack	= this->GetSafeHwnd();   
			tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
			::TrackMouseEvent(&tme);
		}
	}
    CLKCtrlBase::OnMouseMove(nFlags, point);
}

void CLKDrawerItem::OnMouseLeave()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    m_nCurIndex = -2;
    if(m_bMouseDown)
    {
        m_bMouseDown = false;
    }
    Invalidate();
    CLKCtrlBase::OnMouseLeave();
}

void CLKDrawerItem::OnLButtonDown(UINT nFlags, CPoint point)
{
    //int nIndex = GetItemIndexByPoint(point);
    //if(nIndex == -1)
    //{
    //    m_bMouseDown = true;
    //}
    if(!m_bMouseDown)
    {
        m_bMouseDown = true;
        Invalidate();
    }
    m_nCurDownIndex = m_nCurIndex;
    CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKDrawerItem::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(m_bMouseDown)
    {
        int nIndex = GetItemIndexByPoint(point);
        if(m_nCurDownIndex == nIndex)
        {
            if(nIndex == -1)
            {
                // 选中该项
		        ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKDRAWER_MSG_WPARAM_ITEMCHANGED, (LPARAM)GetSafeHwnd());
            }else if(nIndex >= 0)
            {
                if(1 == m_arrItem[nIndex]->nType)
                {
                    m_arrItem[nIndex]->nSelect = !m_arrItem[nIndex]->nSelect;
                    // 选中或不选中该项的子项
                    if(m_arrItem[nIndex]->nSelect)
                    {
	                    ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKDRAWER_MSG_WPARAM_ITEMBTNSELECTED, (LPARAM)nIndex);
                    }else
                    {
	                    ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKDRAWER_MSG_WPARAM_ITEMBTNCANCEL, (LPARAM)nIndex);
                    }
                }else
                {
                    // 单击该项的子项
	                ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKDRAWER_MSG_WPARAM_ITEMBTNCLICKED, (LPARAM)nIndex);
                }
            }
        }

        m_bMouseDown = false;
        Invalidate();
    }
    CLKCtrlBase::OnLButtonUp(nFlags, point);
}

// 根据鼠标指针的位置获取项索引
int CLKDrawerItem::GetItemIndexByPoint(CPoint point)
{
	CRect rtDest(m_rtClient);
    rtDest.bottom = rtDest.top + m_nHeadHeight;
    if(rtDest.PtInRect(point))
    {
        for(int i = 0; i < m_arrItem.GetCount(); i++)
        {
            PDrawerItemButton pItem = m_arrItem[i];
            if(pItem->rtWnd.PtInRect(point))
            {
                return i;
            }
        }
        return -1;
    }
	return -2;
}

// 画标签文本
void CLKDrawerItem::DrawCaption(CDC *pDC, CRect &rtText)
{
    if(!m_strCaption.IsEmpty())
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(m_crText);
        CFont *pF = m_bExpand ? CLKFontMgr::GetSTB90() : CLKFontMgr::GetST90();
        CFont *pFont = pDC->SelectObject(pF);
        pDC->DrawText(m_strCaption, m_strCaption.GetLength(), &rtText, DT_LEFT | DT_VCENTER);
        pDC->SelectObject(pFont);
    }
}

// 画按钮
void CLKDrawerItem::DrawButtons(CDC *pDC)
{
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        PDrawerItemButton pItem = m_arrItem[i];
        if(m_nCurIndex == i)
        {
            DrawButton(pDC, pItem, true);
        }else
        {
            DrawButton(pDC, pItem);
        }
    }
}
void CLKDrawerItem::DrawButton(CDC *pDC, PDrawerItemButton pItem, bool bIsCur)
{
    if(pItem && !pItem->rtWnd.IsRectEmpty())
    {
        bool bDown(false);
        if(m_pImgButton && 1 == pItem->nType)
        {
            // 复选按钮
            if(pItem->nSelect)
            {
                // 选中
                // 画通用背景
                m_pImgButton->DrawToDC(pDC, pItem->rtWnd, 1, LKSKINBASE_DRAWBGSTYLE_ALL);
                bDown = true;
            }else if(bIsCur)
            {
                // 画通用背景
                //m_pImgButton->DrawToDC(pDC, pItem->rtWnd, 0, LKSKINBASE_DRAWBGSTYLE_ALL);
                if(m_bMouseDown)
                {
                    m_pImgButton->DrawToDC(pDC, pItem->rtWnd, 1, LKSKINBASE_DRAWBGSTYLE_ALL);
                    bDown = true;
                }else
                {
                    m_pImgButton->DrawToDC(pDC, pItem->rtWnd, 0, LKSKINBASE_DRAWBGSTYLE_ALL);
                }
            }
        }else if(m_pImgButton && bIsCur)
        {
            // 画通用背景
            if(m_bMouseDown)
            {
                m_pImgButton->DrawToDC(pDC, pItem->rtWnd, 1, LKSKINBASE_DRAWBGSTYLE_ALL);
                bDown = true;
            }else
            {
                m_pImgButton->DrawToDC(pDC, pItem->rtWnd, 0, LKSKINBASE_DRAWBGSTYLE_ALL);
            }
        }

        CRect rtText(pItem->rtWnd);
        CRect rtIcon(pItem->rtWnd);
        // 画图标
        if(pItem->pImg)
        {
            if(bDown)
            {
                rtIcon.OffsetRect(1, 1);
            }
            rtIcon.left += m_nMargin;
            rtIcon.right = rtIcon.left + pItem->pImg->GetWidth();
            rtIcon.top += (rtIcon.Height() - pItem->pImg->GetHeight()) / 2;
            rtIcon.bottom = rtIcon.top + pItem->pImg->GetHeight();
            pItem->pImg->DrawToDC(pDC, rtIcon, 0);

            rtText.left = rtIcon.right + m_nImageTextSplit;
        }
        // 画文本
        if(!pItem->strCaption.IsEmpty())
        {
            if(bDown)
            {
                rtText.OffsetRect(1, 1);
                rtText.left--;
            }
            CFont *pFont = pDC->SelectObject(CLKFontMgr::GetST90());
            pDC->DrawText(pItem->strCaption, pItem->strCaption.GetLength(), &rtText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            pDC->SelectObject(pFont);
        }
    }
}

// 计算按钮区域
void CLKDrawerItem::CalcItemRect()
{
    if(GetSafeHwnd())
    {
        CRect rtClient;
        GetClientRect(&rtClient);
        CRect rtBtn(rtClient);
        rtBtn.bottom = rtBtn.top + m_nHeadHeight;
        rtBtn.DeflateRect(1, 1); 
        for(int i = 0; i < m_arrItem.GetCount(); i++)
        {
            PDrawerItemButton pItem = m_arrItem[i];
            int nWidth = GetStringWidth(pItem->strCaption, CLKFontMgr::GetST90());
            if(pItem->pImg)
            {
                nWidth += pItem->pImg->GetWidth();
                if(!pItem->strCaption.IsEmpty())
                {
                   nWidth += m_nImageTextSplit;
                }
            }
            rtBtn.left = rtBtn.right - nWidth - 2 * m_nMargin - m_nButtonSplit;
            pItem->rtWnd = rtBtn;
            rtBtn.right = rtBtn.left;
        }
    }
}

// 调整子控件的区域
void CLKDrawerItem::AdjustChildWndRect()
{
    if(m_pChildWnd && m_bExpand)
    {
        CRect rtClient(m_rtClient);
        //GetClientRect(&rtClient);
        rtClient.top += m_nHeadHeight;
        m_pChildWnd->SetParent(this);
        //m_pChildWnd->MoveWindow(&rtClient);
        m_pChildWnd->SetWindowPos(0, rtClient.left, rtClient.top, rtClient.Width(), rtClient.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
    }else if(m_pChildWnd)
    {
        m_pChildWnd->ShowWindow(SW_HIDE);
    }
}

// 设置关联数据指针
void CLKDrawerItem::SetDataPtr(PCtrlDataBase pData)
{
    m_pData = pData;
    DrawItemData *p = (DrawItemData *)m_pData;
    if(p && p->pData)
    {
        if(m_pChildWnd)
        {
            m_pChildWnd->SetDataPtr(p->pData);
        }
    }
    CLKCtrlBase::SetDataPtr(pData);
}

// 保存数据到内存
void CLKDrawerItem::SaveDataToMem(CtrlDataBase *pData)
{
    if(pData)
    {
        m_pData = pData;
        DrawItemData *p = (DrawItemData *)pData;
        p->strCaption = m_strCaption;
        p->bExpand    = m_bExpand;
        //for(int i = 0; i < p->arrButtons.GetCount(); i++)
        //{
        //    delete p->arrButtons[i];
        //}
        //p->arrButtons.RemoveAll();
        if(p->arrButtons.GetCount() == 0)
        {
            for(int i = 0; i < m_arrItem.GetCount(); i++)
            {
                PDrawerItemButton pItem = new DrawerItemButton(*m_arrItem[i]);
                p->arrButtons.Add(pItem);
            }
        }
        if(p->pData)
        {
            m_pChildWnd = p->pData->pWnd;
        }
        if(m_pChildWnd && p->pData && p->bExpand)
        {
            m_pChildWnd->SaveDataToMem(p->pData);
        }
    }
    CLKCtrlBase::SaveDataToMem(pData);
}

// 从内存中加载数据
void CLKDrawerItem::LoadDataFromMem(CtrlDataBase *pData)
{
    if(pData)
    {
        m_pData = pData;
        DrawItemData *p = (DrawItemData *)pData;
        m_strCaption = p->strCaption;
        m_bExpand = p->bExpand;
        for(int i = 0; i < m_arrItem.GetCount(); i++)
        {
            delete m_arrItem[i];
        }
        m_arrItem.RemoveAll();
        for(int i = 0; i < p->arrButtons.GetCount(); i++)
        {
            PDrawerItemButton pItem = new DrawerItemButton(*(p->arrButtons[i]));
            m_arrItem.Add(pItem);
        }
        // 计算按钮区域
        CalcItemRect();

        if(p->pData && p->pData->pWnd && p->bExpand)
        {
            m_pChildWnd = p->pData->pWnd;
            m_pChildWnd->SetParent(this);
            m_pChildWnd->LoadDataFromMem(p->pData);
        }
        if(IsWindowVisible())
        {
            Invalidate();
        }
    }
    CLKCtrlBase::LoadDataFromMem(pData);
}

// 保存子窗口数据到内存
void CLKDrawerItem::SaveChildDataToMem()
{
    DrawItemData *p = (DrawItemData *)m_pData;
    if(p && m_pChildWnd && p->pData)
    {
        m_pChildWnd->SaveDataToMem(p->pData);
    }
}

// 从内存中加载数据到子窗口
void CLKDrawerItem::LoadChildDataToMem()
{
    DrawItemData *p = (DrawItemData *)m_pData;
    if(p && p->pData && p->pData->pWnd)
    {
        m_pChildWnd = p->pData->pWnd;
        m_pChildWnd->SetParent(this);
        m_pChildWnd->LoadDataFromMem(p->pData);
        if(m_pChildWnd)
        {
            m_pChildWnd->ShowWindow(SW_SHOW);
        }
        if(IsWindowVisible())
        {
            Invalidate();
        }
    }
}

BOOL CLKDrawerItem::DestroyWindow()
{
    // TODO: 在此添加专用代码和/或调用基类
    //if(m_pChildWnd)
    //{
    //    m_pChildWnd->SetParent(GetParent());
    //}

    return CLKCtrlBase::DestroyWindow();
}

void CLKDrawerItem::OnSetFocus(CWnd* pOldWnd)
{
    CLKCtrlBase::OnSetFocus(pOldWnd);

    if(m_pChildWnd)
    {
        m_pChildWnd->SetFocus();
    }
    // TODO: 在此处添加消息处理程序代码
}
