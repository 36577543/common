// ..\..\common\LKComponent\GDI\LKTab.cpp : 实现文件
//

#include "LKTab.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"


// CLKTab

IMPLEMENT_DYNAMIC(CLKTab, CLKCtrlBase)

CLKTab::CLKTab(int nImageIndex, bool bVertical)
: CLKCtrlBase(nImageIndex)
, m_pBGImage(0)
, m_bVertical(bVertical)
, m_nMargin(2)
, m_nItemSplit(0)
, m_nItemWidth(61)
, m_nItemHeight(21)
, m_nSelectIndex(-1)
, m_nCurIndex(-1)
// 是否自动调节项尺寸
, m_bIsAdjustItmeSize(false)
{

}

CLKTab::~CLKTab()
{
	CLKImage *pImg = GetMemImg();
	if(pImg) delete pImg;
	for(int i = 0; i < m_arrItems.GetCount(); i++)
	{
		delete m_arrItems[i];
	}
	m_arrItems.RemoveAll();
}

BOOL CLKTab::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
    CalcItemSize();
	return bRet;
}

void CLKTab::SetBGImage(int nImageIndex)
{
	m_pBGImage = CLKImageMgr::GetImageS(nImageIndex);
}

// 加入一项
void CLKTab::AddItem(CString strText, int nImgIndex, CWnd *pWnd, void *pData)
{
	LKTabItem *pItem = new LKTabItem;
	pItem->strText = strText;
	if(nImgIndex >= 0)
	{
		pItem->pImg = CLKImageMgr::GetImageS(nImgIndex);
	}
	pItem->pWnd = pWnd;
	pItem->pData = pData;
	m_arrItems.Add(pItem);
    CalcItemSize();
}

void CLKTab::AddItem(CLKImage *pImg, CWnd *pWnd, bool bMustFree)
{
	LKTabItem *pItem = new LKTabItem;
	pItem->strText = _T("");
	pItem->pImg = pImg;
	pItem->pWnd = pWnd;
	pItem->bMustFree = bMustFree;
	m_arrItems.Add(pItem);
}

// 删除项
void CLKTab::DelAllItem()
{
	for (int i = 0; i < m_arrItems.GetCount(); i++)
	{
		delete m_arrItems[i];
	}
	m_arrItems.RemoveAll();
}

BEGIN_MESSAGE_MAP(CLKTab, CLKCtrlBase)
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CLKTab 消息处理程序
void CLKTab::OnMouseLeave()
{
	CRect rtItem(0, 0, 0, 0);
	if(GetItemRectByIndex(m_nCurIndex, &rtItem))
	{
		InvalidateRect(&rtItem);
	}
	m_nCurIndex = -1;
	CLKCtrlBase::OnMouseLeave();
}

void CLKTab::OnMouseMove(UINT nFlags, CPoint point)
{
	int nIndex = GetItemIndexByPoint(point);
	if(nIndex != m_nCurIndex)
	{
		CRect rtItem(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
			InvalidateRect(&rtItem);
		}
		m_nCurIndex = nIndex;
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
			InvalidateRect(&rtItem);
		}
		if(nIndex >= 0)
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


void CLKTab::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_nCurIndex != m_nSelectIndex && m_nCurIndex >= 0)
	{
		CRect rtItem(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nSelectIndex, &rtItem))
		{
			InvalidateRect(&rtItem);
		}
		m_nSelectIndex = m_nCurIndex;
		rtItem = CRect(0, 0, 0, 0);
		if(GetItemRectByIndex(m_nCurIndex, &rtItem))
		{
			InvalidateRect(&rtItem);
		}
		::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKTAP_MSG_WPARAM_ITEMCHANGED, (LPARAM)GetSafeHwnd());
	}
	CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKTab::OnLButtonUp(UINT nFlags, CPoint point)
{
	//InitLClick();
	//::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	CLKCtrlBase::OnLButtonUp(nFlags, point);
}



// 创建控件后初始化
void CLKTab::InitCtrlAfterCreate()
{
	ParseImageToMem();
}

// 刷新前景缓存
void CLKTab::ReSetMem()
{
    CalcItemSize();
	ParseImageToMem();
	if(IsWindowVisible())
	{
        //// 倒圆角
        //CRect rc;
        //GetWindowRect(&rc);
        //GetClientRect(&m_rtClient);
        //CRgn rgn, rgn2;
        //rc.MoveToXY(0, 0);
        //rgn.CreateRoundRectRgn(rc.left - 1, rc.top - 1, rc.right + 2, rc.bottom + 2, 11, 11); //创建一个圆角矩形
        //CRect rc2(rc);
        //rc2.top = rc.bottom - 4;
        //rgn2.CreateRectRgn(rc2.left, rc2.top, rc2.right, rc2.bottom);
        //rgn.CombineRgn(&rgn, &rgn2, RGN_OR);
        //SetWindowRgn(rgn,TRUE); 
		Invalidate();
	}
}
// 解析出相关图片到内存缓存中
void CLKTab::ParseImageToMem()
{
	if(m_pBGImage)
	{
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);

		CLKImage *pImg = GetMemImg();
		if(pImg) delete pImg;
		pImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
        m_pBGImage->DrawToImage(pImg, 0, &prm);
		SetMemImg(pImg);
	}
}

// 初始化图片列表
void CLKTab::InitImageList()
{
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_TAB_BEGIN, LKIMAGELISTINDEX_TAB_END);
	CLKImage *pImage = GetParseImg();
	if(pImage)
	{
		m_nItemWidth = pImage->GetWidth();
		m_nItemHeight = pImage->GetHeight() / pImage->GetImageNumber();
	}
}

// 在缓存上绘图
void CLKTab::OnPaint(CDC *pDC)
{
	if(GetParseImg())
	{
		CRect rtDest(m_rtClient);
		for(int i = 0; i < m_arrItems.GetCount(); i++)
		{
			if(m_bVertical)
			{
				rtDest.top = m_nMargin + i * (m_nItemHeight + m_nItemSplit);
				rtDest.bottom = rtDest.top + m_nItemHeight;
			}else
			{
				rtDest.left = m_nMargin + i * (m_nItemWidth + m_nItemSplit);
				rtDest.right = rtDest.left + m_nItemWidth;
			}
			int nIndex = (m_nSelectIndex == i ? 1 : (m_nCurIndex == i ? 1 : 0));
            if(m_bIsAdjustItmeSize)
            {
	            if(m_bVertical)
	            {
		            // 纵向
			        GetParseImg()->DrawToDC(pDC, rtDest, nIndex, LKSKINBASE_DRAWBGSTYLE_TOPMIDBOTTOM);
	            }else
                {
		            // 横向
			        GetParseImg()->DrawToDC(pDC, rtDest, nIndex, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
                }
            }else
            {
			    GetParseImg()->DrawToDC(pDC, rtDest, nIndex);
            }

			// 画图标(居中)
			CRect rtIcon(0, 0, 0, 0);
			CLKImage *pImg = m_arrItems[i]->pImg;
			if(pImg && pImg->IsValid())
			{
				int nImgHeight = pImg->GetHeight() / pImg->GetImageNumber();
				rtIcon.left = rtDest.left + (rtDest.Width() - pImg->GetWidth()) / 2;
				rtIcon.top = rtDest.top + (rtDest.Height() - nImgHeight) / 2;
				rtIcon.right = rtIcon.left + pImg->GetWidth();
				rtIcon.bottom = rtIcon.top + nImgHeight;
				pImg->DrawToDC(pDC, rtIcon, 0);
			}else if(!m_arrItems[i]->strText.IsEmpty())
			{			
				// 画文本
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(m_crText);
				CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST90());
				pDC->DrawText(m_arrItems[i]->strText, rtDest, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				pDC->SelectObject(pOld);
			}
		}
	}

	
}

// 将前景画在指定的CDC设置上
void CLKTab::OnDrawFGToDC(BGParam *prm)
{
    CDC *pDC = (CDC *)prm->pBG;
	if(GetParseImg())
	{
		CRect rtDest(m_rtClient);
        rtDest.MoveToXY(prm->rt.left, prm->rt.top);
		for(int i = 0; i < m_arrItems.GetCount(); i++)
		{
			if(m_bVertical)
			{
				rtDest.top = m_nMargin + i * (m_nItemHeight + m_nItemSplit);
				rtDest.bottom = rtDest.top + m_nItemHeight;
			}else
			{
				rtDest.left = m_nMargin + i * (m_nItemWidth + m_nItemSplit);
				rtDest.right = rtDest.left + m_nItemWidth;
			}
			int nIndex = (m_nSelectIndex == i ? 2 : (m_nCurIndex == i ? 0 : 1));
            if(m_bIsAdjustItmeSize)
            {
	            if(m_bVertical)
	            {
		            // 纵向
			        GetParseImg()->DrawToDC(pDC, rtDest, nIndex, LKSKINBASE_DRAWBGSTYLE_TOPMIDBOTTOM);
	            }else
                {
		            // 横向
			        GetParseImg()->DrawToDC(pDC, rtDest, nIndex, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
                }
            }else
            {
			    GetParseImg()->DrawToDC(pDC, rtDest, nIndex);
            }

			// 画图标(居中)
			CRect rtIcon(0, 0, 0, 0);
			CLKImage *pImg = m_arrItems[i]->pImg;
			if(pImg && pImg->IsValid())
			{
				int nImgHeight = pImg->GetHeight() / pImg->GetImageNumber();
				rtIcon.left = rtDest.left + (rtDest.Width() - pImg->GetWidth()) / 2;
				rtIcon.top = rtDest.top + (rtDest.Height() - nImgHeight) / 2;
				rtIcon.right = rtIcon.left + pImg->GetWidth();
				rtIcon.bottom = rtIcon.top + nImgHeight;
				pImg->DrawToDC(pDC, rtIcon, 0);
			}else if(!m_arrItems[i]->strText.IsEmpty())
			{			
				// 画文本
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(m_crText);
				CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST90());
				pDC->DrawText(m_arrItems[i]->strText, rtDest, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				pDC->SelectObject(pOld);
			}
		}
	}
}

// 根据鼠标指针的位置获取项索引　－　相对于起始可见索引
int CLKTab::GetItemIndexByPoint(CPoint point)
{
	//int nBase = point.y / LKLISTCTRLROW_DEFAULT_HIGHT;
	if(m_bVertical)
	{
		// 纵向
		int m = (point.y - m_nMargin + m_nItemSplit) % (m_nItemHeight + m_nItemSplit);
		if(m > 0 && m <= m_nItemHeight)
		{
			int n = (point.y - m_nMargin + m_nItemSplit) / (m_nItemHeight + m_nItemSplit);
			if(n < m_arrItems.GetCount())
			{
				return n;
			}
		}
	}else
	{
		// 橫向
		int m = (point.x - m_nMargin + m_nItemSplit) % (m_nItemWidth + m_nItemSplit);
		if(m > 0 && m <= m_nItemWidth)
		{
			int n = (point.x - m_nMargin + m_nItemSplit) / (m_nItemWidth + m_nItemSplit);
			if(n < m_arrItems.GetCount())
			{
				return n;
			}
		}
	}
	return -1;
}

// 根据可见索引获取指定项的区域
bool CLKTab::GetItemRectByIndex(int nIndex, LPRECT lpRect)
{
	if(nIndex >= 0 && nIndex < m_arrItems.GetCount())
	{
		*lpRect = m_rtClient;
		if(m_bVertical)
		{
			// 纵向
			lpRect->top = m_nMargin + (m_nItemHeight + m_nItemSplit) * nIndex;
			lpRect->bottom = lpRect->top + m_nItemHeight;
		}else
		{
			// 橫向
			lpRect->left = m_nMargin + (m_nItemWidth + m_nItemSplit) * nIndex;
			lpRect->right = lpRect->left + m_nItemWidth;
		}
		return true;
	}
	return false;
}

// 计算项大小
void CLKTab::CalcItemSize()
{
    if(m_bIsAdjustItmeSize && GetSafeHwnd())
    {
	    if(m_bVertical)
	    {
		    // 纵向
            m_nItemHeight = (m_rtClient.Height() - 2 * m_nMargin + m_nItemSplit) / m_arrItems.GetCount() - m_nItemSplit;
	    }else
        {
		    // 横向
            m_nItemWidth = (m_rtClient.Width() - 2 * m_nMargin + m_nItemSplit) / m_arrItems.GetCount() - m_nItemSplit;
        }
    }
}