// ..\..\..\common\LKComponent\gdi\LKDrawer.cpp : ʵ���ļ�
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
    // ������
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
    // ������
    //CreateItems();
    //AdjustItemPos();
	return bRet;
}

// ��ʼ��ͼƬ�б�
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


// ����һ��
void CLKDrawer::Add(CString strText)
{
    CLKDrawerItem *pItem = new CLKDrawerItem;
    pItem->SetCaption(strText);
    // ������
    CreateItem(pItem, m_arrItem.GetCount());
    m_arrItem.Add(pItem);
    CalcItemsRect();
    AdjustItemPos();
    //Invalidate();
}

// ѡ��һ��
void CLKDrawer::SelectItem(int nIndex)
{
    if(m_nSelectedIndex != nIndex)
    {
        // ����������
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

        // ���ص�ǰ����
        if(m_nSelectedIndex >= 0 && m_nSelectedIndex < m_arrItem.GetCount())
        {
            CLKDrawerItem *pItem = m_arrItem[m_nSelectedIndex];
            pItem->LoadChildDataToMem();
            pItem->SetFocus();
        }
    }
}

// ����һ����ť��ָ��������
void CLKDrawer::AddButton(int nIndex, CString strCaption, CLKImage *pImg, int nButtonType)
{
    if(nIndex >= 0 && nIndex < m_arrItem.GetCount())
    {
        m_arrItem[nIndex]->AddButton(strCaption, pImg, nButtonType);
    }
}

// ��ȡ�Ӵ���
CLKCtrlBase *CLKDrawer::GetChildWnd(int nIndex)
{
    if(nIndex >= 0 && nIndex < m_arrItem.GetCount())
    {
        return m_arrItem[nIndex]->GetChildWnd();
    }
    return 0;
}

// �����Ӵ���
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

// ���ù�������ָ��
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

// �Ƿ��ȡ������(���߸�ǰ��)
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




// CLKDrawer ��Ϣ�������

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
                    // ��£
                    if(i > 0)
                    {
                        SelectItem(i - 1);
                    }
                }else
                {
                    // չ��
                    SelectItem(i);
                }
                break;
            }
        }
        return 1;
    }
    return CLKCtrlBase::OnCommonMsg(wParam, lParam);
}

// ��print����
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

// ˢ��ǰ������
void CLKDrawer::ReSetMem()
{
	ParseImageToMem();
	if(GetSafeHwnd())
	{
		// ��Բ��
        CreateDefaultRoundWnd();
        //ClipClientRect();
    }
    CalcItemsRect();
    AdjustItemPos();
    //Invalidate();
    //PostMessage(WM_PAINT);
}

// ���������ͼƬ���ڴ滺����
void CLKDrawer::ParseImageToMem()
{
	// ��ȡ��Ҫ������ͼƬ
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

            /*************���⴦�����ͻ����򱳾�ʹ�ø�������ǰ��*****************/
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

            // ������
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


// ������
void CLKDrawer::CreateItem(CLKDrawerItem *pItem, int nId)
{
    if(pItem && !pItem->GetSafeHwnd() && GetSafeHwnd())
    {
        CRect rtItem(m_rtClient);
        //// �������λ��
        //CalcItemRect(pItem);
        //// ��ȡ���λ��
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

// �����������λ��
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

        // ��������ʱ����λ��
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

// ��������ʱ����λ��
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

        // ��������ʱ����λ��
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

// ������λ��
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
            // ��ǰ��
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
            // ��ͬ�Ĵ���
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
            // ��ͬ�Ĵ���
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

// �������ݵ��ڴ�
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

// ���ڴ��м�������
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
//    // TODO: �ڴ˴������Ϣ����������
//}
