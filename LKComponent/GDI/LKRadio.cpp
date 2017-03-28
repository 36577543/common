// LKRadio.cpp : 实现文件
//

#include "LKRadio.h"
#include "LKFontMgr.h"
#include "LKImageMgr.h"


// CLKRadio

IMPLEMENT_DYNAMIC(CLKRadio, CLKCommonCtrl)

CLKRadio::CLKRadio(int nImageIndex)
: CLKCommonCtrl(nImageIndex)
, m_nLeftMargin(2)
, m_nIconTextSplit(2)
, m_bSelected(false)
{

}

CLKRadio::~CLKRadio()
{
}

BOOL CLKRadio::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, bool bIsGroup)
{
	BOOL bRet(false);
	if(bIsGroup)
	{
		bRet = CLKCommonCtrl::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_GROUP, rect, pParentWnd, nID); 
	}else
	{
		bRet = CLKCommonCtrl::Create(_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	}
	return bRet;
}

BOOL CLKRadio::Create(CString strCatpion, const RECT& rect, CWnd* pParentWnd, UINT nID, bool bIsGroup)
{
	BOOL bRet(false);
	if(bIsGroup)
	{
		bRet = CLKCommonCtrl::Create(strCatpion, WS_CHILD | WS_VISIBLE | WS_GROUP, rect, pParentWnd, nID); 
	}else
	{
		bRet = CLKCommonCtrl::Create(strCatpion, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	}
	return bRet;
}



BEGIN_MESSAGE_MAP(CLKRadio, CLKCommonCtrl)
END_MESSAGE_MAP()



// CLKRadio 消息处理程序

// 解析出相关图片到内存缓存中
void CLKRadio::ParseImageToMem()
{
	//CLKCommonCtrl::ParseImageToMem();
	m_bImgIsOwner = true;
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	int nLeft(m_nLeftMargin);
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

		if(nNumber > 0)
		{
			CRect rtWnd(0, 0, pImg->GetWidth(), pImg->GetHeight() / nNumber);
			if(m_pNormalImg) delete m_pNormalImg;
			m_pNormalImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
			int nTop = (m_rtClient.Height() - pImg->GetHeight() / nNumber) / 2;
			CRect rtDest(nLeft, nTop, m_rtClient.Width(), m_rtClient.Height());
			CRect rtOffset(0, 0, 0, 0);
			pImg->DrawToImage(m_pNormalImg, rtDest, rtOffset, 0, &prm, LKSKINBASE_DRAWBGSTYLE_COPY);

			if(m_pHighLightImg) delete m_pHighLightImg;
			m_pHighLightImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
			pImg->DrawToImage(m_pHighLightImg, rtDest, rtOffset, 1, &prm, LKSKINBASE_DRAWBGSTYLE_COPY);

			if(m_pOperatorImg) delete m_pOperatorImg;
			m_pOperatorImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
			pImg->DrawToImage(m_pOperatorImg, rtDest, rtOffset, 2, &prm, LKSKINBASE_DRAWBGSTYLE_COPY);

			SetMemImg(m_pNormalImg);
			nLeft += pImg->GetWidth() + m_nIconTextSplit;
		}
	}
	
	CString strText;
	GetWindowText(strText);
	if(!strText.IsEmpty())
	{
		CRect rtText(m_rtClient);
		rtText.left = nLeft;
		m_pNormalImg->DrawText(strText, rtText, CLKFontMgr::GetST90(), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		m_pHighLightImg->DrawText(strText, rtText, CLKFontMgr::GetST90(), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		m_pOperatorImg->DrawText(strText, rtText, CLKFontMgr::GetST90(), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
}

// 刷新前景缓存
void CLKRadio::ReSetMem()
{
	CLKCommonCtrl::ReSetMem();
}

// 初始化图片列表
void CLKRadio::InitImageList()
{
	// 初始化图片列表
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_RADIOBOX_BEGIN, LKIMAGELISTINDEX_RADIOBOX_END);
}

// 在缓存上绘图
void CLKRadio::OnPaint(CDC *pDC)
{
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg && m_bSelected)
	{
		int nTop = (m_rtClient.Height() - pImg->GetHeight() / pImg->GetImageNumber()) / 2;
		CRect rtDest(m_nLeftMargin, nTop, m_rtClient.Width(), m_rtClient.Height()), rtOffset(0, 0, 0, 0);
		pImg->DrawToDC(pDC, rtDest, rtOffset, 3, LKSKINBASE_DRAWBGSTYLE_COPY);
	}
}

// 初始化左键单击消息LPARAM参数
void CLKRadio::InitLClick()
{
	if(!m_bSelected)
	{
        SetSelected();
		CWnd *pWnd(this);
		DWORD dwStyle(0);
		dwStyle = GetStyle();
		while(!(dwStyle & WS_GROUP))
		{
			// 往前寻找
			pWnd = pWnd->GetWindow(GW_HWNDPREV);
			if(pWnd && pWnd->GetSafeHwnd() && pWnd->IsKindOf(RUNTIME_CLASS(CLKRadio)))
			{
				((CLKRadio *)pWnd)->SetSelected(false);
				dwStyle = pWnd->GetStyle();
				if(dwStyle & WS_GROUP)
				{
					break;
				}
			}else
			{
				break;
			}
		}
		pWnd = this;
		while(1)
		{
			// 往后寻找
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
			if(pWnd && pWnd->GetSafeHwnd() && pWnd->IsKindOf(RUNTIME_CLASS(CLKRadio)))
			{
				dwStyle = pWnd->GetStyle();
				if(dwStyle & WS_GROUP)
				{
					break;
				}
				((CLKRadio *)pWnd)->SetSelected(false);
			}else
			{
				break;
			}
		}
		
	}
}

// 获当前选取中的索引
int CLKRadio::GetCurIndex()
{
    int nIndex = -1;
	CWnd *pWnd(this);
	DWORD dwStyle(0);
	dwStyle = GetStyle();
	while(!(dwStyle & WS_GROUP))
	{
		// 往前寻找
		pWnd = pWnd->GetWindow(GW_HWNDPREV);
		if(pWnd && pWnd->GetSafeHwnd() && pWnd->IsKindOf(RUNTIME_CLASS(CLKRadio)))
		{
			dwStyle = pWnd->GetStyle();
			if(dwStyle & WS_GROUP)
			{
				break;
			}
		}else
		{
			break;
		}
	}
	//pWnd = this;
	for(int i = 0; 1; i++)
	{
		// 往后寻找
		if(pWnd && pWnd->GetSafeHwnd() && pWnd->IsKindOf(RUNTIME_CLASS(CLKRadio)))
		{
			if(((CLKRadio *)pWnd)->m_bSelected)
			{
                nIndex = i;
                break;
			}
		}else
		{
			break;
		}
		dwStyle = pWnd->GetStyle();
		if((dwStyle & WS_GROUP) && i > 0)
		{
			break;
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
    return nIndex;
}

// 设置当前选取中的索引
void CLKRadio::SetCurIndex(int nIndex)
{
    //int nIndex = -1;
	CWnd *pWnd(this);
	DWORD dwStyle(0);
	dwStyle = GetStyle();
	while(!(dwStyle & WS_GROUP))
	{
		// 往前寻找
		pWnd = pWnd->GetWindow(GW_HWNDPREV);
		if(pWnd && pWnd->GetSafeHwnd() && pWnd->IsKindOf(RUNTIME_CLASS(CLKRadio)))
		{
			if(((CLKRadio *)pWnd)->m_bSelected)
			{
				((CLKRadio *)pWnd)->SetSelected(false);
			}
			dwStyle = pWnd->GetStyle();
			if(dwStyle & WS_GROUP)
			{
				break;
			}
		}else
		{
			break;
		}
	}
	//pWnd = this;
	for(int i = 0; 1; i++)
	{
		// 往后寻找
		if(pWnd && pWnd->GetSafeHwnd() && pWnd->IsKindOf(RUNTIME_CLASS(CLKRadio)))
		{
			if(nIndex == i)
			{
                ((CLKRadio *)pWnd)->SetSelected();
                break;
			}
		}else
		{
			break;
		}
		dwStyle = pWnd->GetStyle();
		if((dwStyle & WS_GROUP) && i > 0)
		{
			break;
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CLKRadio::SetSelected(bool bSelected)
{
    if(bSelected && !m_bSelected)
    {
        GetParent()->PostMessage(LKCOMMON_MSG, LKBUTTONEX_MSG_WPARAM_SELECTEDCHANGEED, (LPARAM)this);
    }
    if(m_bSelected != bSelected)
    {
        m_bSelected = bSelected;
        if(IsWindowVisible())
        {
            Invalidate();
        }
    }
}
