// ..\..\common\LKComponent\GDI\LKButton.cpp : 实现文件
//

//#include "stdafx.h"
#include "LKButton.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// CLKButton

IMPLEMENT_DYNAMIC(CLKButton, CLKCommonCtrl)

CLKButton::CLKButton(int nImageIndex)
: CLKCommonCtrl(nImageIndex)
, m_pImage(0)
, m_nLeftMargin(2)
, m_nIconTextSplit(2)
// 鼠标按下时图标与文本的偏移量
, m_nHighLinghtOffset(1)
, m_pBGImage(0)
{
	m_crText = CLKFontMgr::GetDefaultTextColor();
}

CLKButton::~CLKButton()
{
    if(GetSafeHwnd())
    {
        DestroyWindow();
    }
}

BOOL CLKButton::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCommonCtrl::Create(_T(""), WS_CHILD | WS_VISIBLE | SS_NOTIFY, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKButton::Create(CString strCatpion, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCommonCtrl::Create(strCatpion, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKButton::CreateHide(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCommonCtrl::Create(_T(""), WS_CHILD, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKButton::CreateHide(CString strCatpion, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCommonCtrl::Create(strCatpion, WS_CHILD, rect, pParentWnd, nID); 
	return bRet;
}

void CLKButton::SetImage(int nImageIndex)
{
	m_pImage = CLKImageMgr::GetImageS(nImageIndex);
}

void CLKButton::SetParseImage(int nImageIndex)
{
	m_pBGImage = CLKImageMgr::GetImageS(nImageIndex);
}

//// 设置文本颜色
//void CLKButton::SetTextColor(COLORREF crText)
//{
//	m_crText = crText;
//}

BEGIN_MESSAGE_MAP(CLKButton, CLKCommonCtrl)
END_MESSAGE_MAP()



// CLKButton 消息处理程序

// 解析出相关图片到内存缓存中
void CLKButton::ParseImageToMem()
{
	CLKCommonCtrl::ParseImageToMem();
	if(m_pNormalImg)
	{
		if(m_pImage)
		{
			int nTop = m_rtClient.top + (m_rtClient.Height() - m_pImage->GetHeight()) / 2;
			CRect rtDest(m_nLeftMargin, nTop, m_pImage->GetWidth(), m_pImage->GetHeight());
			m_pImage->DrawToImage(m_pNormalImg, rtDest);
			if(m_pHighLightImg)
			{
				m_pImage->DrawToImage(m_pHighLightImg, rtDest);
			}
			if(m_pOperatorImg)
			{
                rtDest.OffsetRect(1, 1);
				m_pImage->DrawToImage(m_pOperatorImg, rtDest);
			}
		}
	}
	/**/
}

// 刷新前景缓存
void CLKButton::ReSetMem()
{
	CLKCommonCtrl::ReSetMem();
}

// 初始化图片列表
void CLKButton::InitImageList()
{
	// 初始化图片列表
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_BUTTON_BEGIN, LKIMAGELISTINDEX_BUTTON_END);
}

// 获取需要解析的图片
CLKImage *CLKButton::GetParseImg()
{
	if(m_pBGImage)
	{
		return m_pBGImage;
	}else
	{
		return CLKCommonCtrl::GetParseImg();
	}
}

// 察除背景消息
bool CLKButton::OnEraseBG(CDC *pDC)
{
	return 1;
}

// 在缓存上绘图
void CLKButton::OnPaint(CDC *pDC)
{
	CString strText;
	GetWindowText(strText);
	if(!m_pNormalImg)
	{
		int nTextLeft = m_nLeftMargin;
		if(m_pImage)
		{
            // 画图标
            int nImageHeight = m_pImage->GetHeight() / m_pImage->GetImageNumber();
			//int nTop = m_rtClient.top + (m_rtClient.Height() - nImageHeight) / 2;
			int nTop = m_rtClient.top + (m_rtClient.Height() - m_nIconHeight) / 2;
			CRect rtIcon(m_nLeftMargin, nTop, m_nLeftMargin + m_nIconWidth, nTop + m_nIconHeight);
			if (esb_down == m_emsCommon)
			{
				rtIcon.OffsetRect(m_nHighLinghtOffset, m_nHighLinghtOffset);
				//CRect rtIcon(m_nLeftMargin + m_nHighLinghtOffset, nTop + m_nHighLinghtOffset, m_nLeftMargin + m_nHighLinghtOffset + m_nIconWidth, nTop + m_nHighLinghtOffset + m_nIconHeight);
				m_pImage->DrawToDC(pDC, rtIcon, 0);
			}else
			{
				//CRect rtIcon(m_nLeftMargin, nTop, m_nLeftMargin + m_nIconWidth, nTop + m_nIconHeight);
				m_pImage->DrawToDC(pDC, rtIcon, 0);
			}
			//nTextLeft += m_pImage->GetWidth() + m_nIconTextSplit;
			nTextLeft += m_nIconWidth + m_nIconTextSplit;
		}

		if(!strText.IsEmpty() /*&& !m_pOperatorImg && !m_pHighLightImg*/)
		{
			CRect rtClient(m_rtClient);
			rtClient.left =  nTextLeft; 
			pDC->SetBkMode(TRANSPARENT);
			COLORREF crText = m_crText;//CLKFontMgr::GetDefaultTextColor();
            if(!IsWindowEnabled())
            {
                crText = RGB(190, 190, 190);
            }
			if (!m_pFtText)
			{
				m_pFtText = CLKFontMgr::GetST90();
			}
			CFont *pOld = pDC->SelectObject(m_pFtText);
			pDC->SetTextColor(crText);
			if(esb_nohere == m_emsCommon)
			{
				pDC->DrawText(strText, rtClient, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}else if(esb_hover == m_emsCommon)
			{
				pDC->DrawText(strText, rtClient, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}else if(esb_down == m_emsCommon)
			{
				//rtClient.top += m_nHighLinghtOffset;
				//rtClient.left+= m_nHighLinghtOffset;
                rtClient.OffsetRect(m_nHighLinghtOffset, m_nHighLinghtOffset);
				pDC->DrawText(strText, rtClient, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
			pDC->SelectObject(pOld);
        }
	}else if(!strText.IsEmpty())
    {
		CRect rtClient(m_rtClient);
		if(m_pImage)
		{
            rtClient.left += m_pImage->GetWidth();
        }
		pDC->SetBkMode(TRANSPARENT);
		COLORREF crText = m_crText;
        if(!IsWindowEnabled())
        {
            crText = RGB(190, 190, 190);
        }
		CFont *pFont = CLKFontMgr::GetST90();
		CFont *pOld = pDC->SelectObject(pFont);
		pDC->SetTextColor(crText);
		if(esb_nohere == m_emsCommon || esb_hover == m_emsCommon)
		{
			pDC->DrawText(strText, rtClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}else if(esb_down == m_emsCommon)
        {
            rtClient.OffsetRect(1, 1);
			pDC->DrawText(strText, rtClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
		pDC->SelectObject(pOld);
	}
}
