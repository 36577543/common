// ..\..\..\common\LKComponent\gdi\LKDrawer.cpp : 实现文件
//
#include "LKDrawer.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"


// CLKDrawer

IMPLEMENT_DYNAMIC(CLKDrawer, CLKCtrlBase)

CLKDrawer::CLKDrawer(int nImageIndex)
: CLKCtrlBase(nImageIndex)
, m_nSelectedIndex(-1)
{

}

CLKDrawer::~CLKDrawer()
{
    CLKImage *pFGImg = GetMemImg();
	if(pFGImg) delete pFGImg;
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        CLKDrawerItem *pItem = m_arrItem[i];
        if(pItem)
        {
            delete pItem;
        }
    }
    m_arrItem.RemoveAll();
}

BOOL CLKDrawer::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, pParentWnd, nID); 
    // 创建项
    //CreateItems();
    //AdjustItemPos();
	return bRet;
}
BOOL CLKDrawer::Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(strCaption, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKDrawer::CreateHide(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_CLIPCHILDREN, rect, pParentWnd, nID); 
    // 创建项
    //CreateItems();
    //AdjustItemPos();
	return bRet;
}

// 初始化图片列表
void CLKDrawer::InitImageList()
{
	//CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_DRAWER_BEGIN, LKIMAGELISTINDEX_DRAWER_BEGIN);
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_COMMON_BEGIN, LKIMAGELISTINDEX_COMMON_BEGIN + 1);

    CLKImage *pImg = GetParseImg();
    if(pImg)
    {
        m_rtClientOffset = pImg->GetFrameRect();
    }
}


// 增加一项
void CLKDrawer::Add(CString strText)
{
    CLKDrawerItem *pItem = new CLKDrawerItem;
    pItem->SetCaption(strText);
    // 创建项
    CreateItem(pItem, m_arrItem.GetCount());
    m_arrItem.Add(pItem);
    CalcItemsRect();
    AdjustItemPos();
    //Invalidate();
}

// 选中一项
void CLKDrawer::SelectItem(int nIndex)
{
    if(m_nSelectedIndex != nIndex)
    {
        // 保存老数据
        if(m_nSelectedIndex >= 0 && m_nSelectedIndex < m_arrItem.GetCount())
        {
            CLKDrawerItem *pItem = m_arrItem[m_nSelectedIndex];
            pItem->SaveChildDataToMem();
        }
        int nOldIndex = m_nSelectedIndex;
        m_nSelectedIndex = nIndex;
        CalcItemsCollectRect();
        if(nOldIndex >= 0 && nOldIndex < m_arrItem.GetCount())
        {
            CLKDrawerItem *pItem = m_arrItem[nOldIndex];
            pItem->SetExpandState(false);
        }
        if(nIndex >= 0 && nIndex < m_arrItem.GetCount())
        {
            CLKDrawerItem *pItem = m_arrItem[nIndex];
            pItem->SetExpandState();
        }
        AdjustItemPos(nOldIndex);

        // 加载当前数据
        if(m_nSelectedIndex >= 0 && m_nSelectedIndex < m_arrItem.GetCount())
        {
            CLKDrawerItem *pItem = m_arrItem[m_nSelectedIndex];
            pItem->LoadChildDataToMem();
            pItem->SetFocus();
        }
    }
}

// 增加一个按钮到指定索引项
void CLKDrawer::AddButton(int nIndex, CString strCaption, CLKImage *pImg, int nButtonType)
{
    if(nIndex >= 0 && nIndex < m_arrItem.GetCount())
    {
        m_arrItem[nIndex]->AddButton(strCaption, pImg, nButtonType);
    }
}

// 获取子窗口
CLKCtrlBase *CLKDrawer::GetChildWnd(int nIndex)
{
    if(nIndex >= 0 && nIndex < m_arrItem.GetCount())
    {
        return m_arrItem[nIndex]->GetChildWnd();
    }
    return 0;
}

// 设置子窗口
void CLKDrawer::SetChildWnd(int nIndex, CLKCtrlBase *pChildWnd)
{
    if(nIndex >= 0 && nIndex < m_arrItem.GetCount())
    {
        m_arrItem[nIndex]->SetChildWnd(pChildWnd);
    }
}
void CLKDrawer::SetChildWnd(CLKCtrlBase *pChildWnd)
{
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        m_arrItem[i]->SetChildWnd(pChildWnd);
        if(i == m_nSelectedIndex && m_arrItem[i]->GetSafeHwnd() && pChildWnd->GetSafeHwnd())
        {
            CRect rtItem;
            m_arrItem[i]->GetCurRect(rtItem);
            pChildWnd->SetParent(m_arrItem[i]);
            rtItem.top += m_arrItem[i]->GetHeadHeight();
            pChildWnd->MoveWindow(&rtItem);
        }
    }
}

// 设置关联数据指针
void CLKDrawer::SetDataPtr(PCtrlDataBase pData)
{
    m_pData = pData;
    DrawData *p = (DrawData *)m_pData;
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        if(p->arrItem.GetCount() > i)
        {
            DrawItemData *pItem = p->arrItem[i];
            m_arrItem[i]->SetDataPtr(pItem);
        }
    }
    CLKCtrlBase::SetDataPtr(pData);
}

// 是否获取父背景(或者父前景)
void CLKDrawer::SetChildGetParentBG(bool b)
{
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        m_arrItem[i]->SetGetParentBG(b);
    }
}

BEGIN_MESSAGE_MAP(CLKDrawer, CLKCtrlBase)
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
    ON_WM_SETFOCUS()
//    ON_WM_MOVE()
END_MESSAGE_MAP()




// CLKDrawer 消息处理程序

LRESULT CLKDrawer::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
    if(LKDRAWER_MSG_WPARAM_ITEMCHANGED == wParam)
    {
        for(int i = 0; i < m_arrItem.GetCount(); i++)
        {
            CLKDrawerItem *pItem = m_arrItem[i];
            if(pItem && pItem->GetSafeHwnd() == (HWND)lParam)
            {
                if(pItem->GetExpandState())
                {
                    // 收拢
                    if(i > 0)
                    {
                        SelectItem(i - 1);
                    }
                }else
                {
                    // 展开
                    SelectItem(i);
                }
                break;
            }
        }
        return 1;
    }
    return CLKCtrlBase::OnCommonMsg(wParam, lParam);
}

// 画print背景
int CLKDrawer::OnPrint(WPARAM wParam, LPARAM lParam)
{
    BGParam rm;
    GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&rm);
	CRect rt;
	GetRectInParent(&rt);
	rm.rt.OffsetRect(rt.left, rt.top);
    CDC *pDC = CDC::FromHandle((HDC)wParam);
    CLKImage *pImg = (CLKImage *)rm.pBG;
    pImg->Draw(pDC->GetSafeHdc(), 0, 0, rt.Width(), rt.Height(), rm.rt.left, rm.rt.top, rt.Width(), rt.Height());
    DrawNCArea(pDC);
    return CWnd::WindowProc(WM_PRINT, wParam, lParam);
}

// 刷新前景缓存
void CLKDrawer::ReSetMem()
{
	ParseImageToMem();
	if(GetSafeHwnd())
	{
		// 倒圆角
        CreateDefaultRoundWnd();
        //ClipClientRect();
    }
    CalcItemsRect();
    AdjustItemPos();
    //Invalidate();
    //PostMessage(WM_PAINT);
}

// 解析出相关图片到内存缓存中
void CLKDrawer::ParseImageToMem()
{
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		// 
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParentImage(&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);
		int nNumber = pImg->GetImageNumber();
		CRect rtWnd;
		GetWindowRect(&rtWnd);
        if(nNumber > 0 && !rtWnd.IsRectEmpty())
		{
			rtWnd.MoveToXY(0, 0);
            CLKImage *pFGImg = GetMemImg();
			if(pFGImg) delete pFGImg;
			pFGImg = new CLKImage(rtWnd.Width(), rtWnd.Height()); 
			AngleOffset angleOffset(4, 0, 4);
			pImg->DrawToImage(pFGImg, &prm, LKSKINBASE_DRAWBGSTYLE_FRAME, angleOffset);
			//pImg->DrawToImage(pFGImg, &prm, LKSKINBASE_DRAWBGSTYLE_ALL, angleOffset);

            /*************特殊处理，将客户区域背景使用父窗口中前景*****************/
		    prm.rt = CRect(0, 0, 0, 0);
		    GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
		    prm.rt.OffsetRect(rt.left, rt.top);
			CLKImage *pBG = (CLKImage *)prm.pBG;
            CRect rtFrm = pImg->GetFrameRect();
            CRect rtSrc;
            rtSrc.left = prm.rt.left + rtFrm.left;
            rtSrc.top  = prm.rt.top + rtFrm.top;
            rtSrc.right = rtSrc.left + rtWnd.Width() - rtFrm.left - rtFrm.right;
            rtSrc.bottom = rtSrc.top + rtWnd.Height() - rtFrm.top - rtFrm.bottom;
            CRect rtD(rtWnd);
            rtD.DeflateRect(rtFrm.left, rtFrm.top, rtFrm.right, rtFrm.bottom);
            pBG->DrawToImage(pFGImg, rtD, rtSrc);
            /******************************/

            // 画标题
            CString strText;
            GetWindowText(strText);
            if(!strText.IsEmpty())
            {
                CRect rtText(0, 0, rtWnd.Width(), m_rtClientOffset.top);
                pFGImg->DrawText(strText, rtText, CLKFontMgr::GetSTB90());
            }
			SetMemImg(pFGImg);
		}
	}
}


// 创建项
void CLKDrawer::CreateItem(CLKDrawerItem *pItem, int nId)
{
    if(pItem && !pItem->GetSafeHwnd() && GetSafeHwnd())
    {
        CRect rtItem(m_rtClient);
        //// 计算项的位置
        //CalcItemRect(pItem);
        //// 获取项的位置
        //pItem->GetCurRect(rtItem);
        pItem->Create(rtItem, this, nId);
    }
}

void CLKDrawer::CreateItems()
{
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        CLKDrawerItem *pItem = m_arrItem[i];
        if(pItem)
        {
            CreateItem(pItem, i);
        }
    }
    CalcItemsRect();
}

// 计算所有项的位置
void CLKDrawer::CalcItemsRect()
{
    CRect rtItem(0, 0, 0, 0);
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        CLKDrawerItem *pItem = m_arrItem[i];
        rtItem = m_rtClient;
        int nHeadHeight = pItem->GetHeadHeight();
        rtItem.top += nHeadHeight * i;
        rtItem.bottom -= nHeadHeight * (m_arrItem.GetCount() - 1 - i);
        pItem->SetExpandRect(rtItem);

        // 计算收起时各项位置
        //CalcItemsCollectRect();
        if(m_nSelectedIndex >= i)
        {
            rtItem.bottom = rtItem.top + nHeadHeight;
        }else
        {
            rtItem.bottom = m_rtClient.bottom - nHeadHeight * (m_arrItem.GetCount() - 1 - i);
            rtItem.top = rtItem.bottom - nHeadHeight;
        }
        pItem->SetCollectRect(rtItem);
    }
}

// 计算收起时各项位置
void CLKDrawer::CalcItemsCollectRect()
{
    CRect rtItem(0, 0, 0, 0);
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        CLKDrawerItem *pItem = m_arrItem[i];
        rtItem = m_rtClient;
        int nHeadHeight = pItem->GetHeadHeight();
        rtItem.top += nHeadHeight * i;
        rtItem.bottom -= nHeadHeight * (m_arrItem.GetCount() - 1 - i);

        // 计算收起时各项位置
        if(m_nSelectedIndex >= i)
        {
            rtItem.bottom = rtItem.top + nHeadHeight;
        }else
        {
            rtItem.bottom = m_rtClient.bottom - nHeadHeight * (m_arrItem.GetCount() - 1 - i);
            rtItem.top = rtItem.bottom - nHeadHeight;
        }
        pItem->SetCollectRect(rtItem);
    }
}

// 调整项位置
void CLKDrawer::AdjustItemPos()
{
    if(GetSafeHwnd())
    {
        CRect rtItem(0, 0, 0, 0);
        for(int i = 0; i < m_arrItem.GetCount(); i++)
        {
            CLKDrawerItem *pItem = m_arrItem[i];
            pItem->GetCurRect(rtItem);
            pItem->MoveWindow(&rtItem);
            // 当前项
            if(m_nSelectedIndex == i)
            {
                m_arrItem[m_nSelectedIndex]->AdjustChildWndRect();
            }
        }
    }
}
void CLKDrawer::AdjustItemPos(int nOldIndex)
{
    if(GetSafeHwnd())
    {
        CRect rtItem(0, 0, 0, 0);
        int n1, n2;
        if(nOldIndex < m_nSelectedIndex)
        {
            n1 = nOldIndex;
            n2 = m_nSelectedIndex;
        }else
        {
            n2 = nOldIndex;
            n1 = m_nSelectedIndex;
        }
        for(int i = n1; i <= n2; i++)
        {
            if(i >= 0 && i < m_arrItem.GetCount())
            {
                CLKDrawerItem *pItem = m_arrItem[i];
                pItem->GetCurRect(rtItem);
                pItem->MoveWindow(&rtItem);
            }
        }


        CLKCtrlBase *pOldWnd = 0;
        if(nOldIndex >= 0 && nOldIndex < m_arrItem.GetCount())
        {
            pOldWnd = m_arrItem[nOldIndex]->GetChildWnd();
        }
        CLKCtrlBase *pNewWnd = m_arrItem[m_nSelectedIndex]->GetChildWnd();
        if(pNewWnd != pOldWnd)
        {
            // 不同的窗口
            if(pOldWnd)
            {
                pOldWnd->ShowWindow(SW_HIDE);
            }
            if(pNewWnd)
            {
                pNewWnd->ShowWindow(SW_SHOW);
                //pNewWnd->SetFocus();
            }
        }else if(pOldWnd != 0)
        {
            // 相同的窗口
            //pOldWnd->SetParent(m_arrItem[m_nSelectedIndex]);
            m_arrItem[m_nSelectedIndex]->AdjustChildWndRect();

            if(nOldIndex >= 0 && nOldIndex < m_arrItem.GetCount())
            {
                m_arrItem[nOldIndex]->SaveChildDataToMem();
            }
            m_arrItem[m_nSelectedIndex]->LoadChildDataToMem();
        }
    }
}

// 保存数据到内存
void CLKDrawer::SaveDataToMem(CtrlDataBase *pData)
{
    SetDataPtr(pData);
    DrawData *p = (DrawData *)pData;
    p->nCurIndex = m_nSelectedIndex;
    //for(int i = 0; i < p->arrItem.GetCount(); i++)
    //{
    //    delete p->arrItem[i];
    //}
    //p->arrItem.RemoveAll();
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        //DrawItemData *pItem = new DrawItemData;
        m_arrItem[i]->SaveDataToMem(p->arrItem[i]);
        //p->arrItem.Add(pItem);
    }
    CLKCtrlBase::SaveDataToMem(pData);
}

// 从内存中加载数据
void CLKDrawer::LoadDataFromMem(CtrlDataBase *pData)
{
    SetDataPtr(pData);
    DrawData *p = (DrawData *)pData;
    for(int i = 0; i < m_arrItem.GetCount(); i++)
    {
        m_arrItem[i]->SetChildWnd(0);
        m_arrItem[i]->DestroyWindow();
        delete m_arrItem[i];
    }
    m_arrItem.RemoveAll();
    for(int i = 0; i < p->arrItem.GetCount(); i++)
    {
        Add();
        //m_arrItem[i]->SetChildWnd();
        m_arrItem[i]->LoadDataFromMem(p->arrItem[i]);
    }
    if(m_nSelectedIndex == p->nCurIndex)
    {
        m_nSelectedIndex = -1;
    }
    SelectItem(p->nCurIndex);
    CLKCtrlBase::LoadDataFromMem(pData);
}

void CLKDrawer::OnSetFocus(CWnd* pOldWnd)
{
    CLKCtrlBase::OnSetFocus(pOldWnd);

    if(m_nSelectedIndex >= 0 && m_nSelectedIndex < m_arrItem.GetCount())
    {
        m_arrItem[m_nSelectedIndex]->SetFocus();
    }
}

//void CLKDrawer::OnMove(int x, int y)
//{
//    CLKCtrlBase::OnMove(x, y);
//
//    //Invalidate();
//    // TODO: 在此处添加消息处理程序代码
//}
