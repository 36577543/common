// ..\..\common\LKComponent\GDI\LKCheckBox.cpp : 实现文件
//

#include "LKCheckBox.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// CLKCheckBox

IMPLEMENT_DYNAMIC(CLKCheckBox, CLKCommonCtrl)

CLKCheckBox::CLKCheckBox(int nImageIndex)
: CLKCommonCtrl(nImageIndex)
, m_nLeftMargin(2)
, m_nIconTextSplit(2)
, m_bSelected(false)
{
	m_crText = CLKFontMgr::GetDefaultTextColor();
}

CLKCheckBox::~CLKCheckBox()
{
}

BOOL CLKCheckBox::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCommonCtrl::Create(_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}

BOOL CLKCheckBox::Create(CString strCatpion, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCommonCtrl::Create(strCatpion, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}


BEGIN_MESSAGE_MAP(CLKCheckBox, CLKCommonCtrl)
END_MESSAGE_MAP()



// CLKCheckBox 消息处理程序

// 解析出相关图片到内存缓存中
void CLKCheckBox::ParseImageToMem()
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
		m_pNormalImg->DrawText(strText, rtText, CLKFontMgr::GetST90(), DT_LEFT | DT_VCENTER | DT_SINGLELINE, m_crText);
		m_pHighLightImg->DrawText(strText, rtText, CLKFontMgr::GetST90(), DT_LEFT | DT_VCENTER | DT_SINGLELINE, m_crText);
		m_pOperatorImg->DrawText(strText, rtText, CLKFontMgr::GetST90(), DT_LEFT | DT_VCENTER | DT_SINGLELINE, m_crText);
	}
}

// 刷新前景缓存
void CLKCheckBox::ReSetMem()
{
	CLKCommonCtrl::ReSetMem();
}

// 初始化图片列表
void CLKCheckBox::InitImageList()
{
	// 初始化图片列表
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_CHECKBOX_BEGIN, LKIMAGELISTINDEX_CHECKBOX_END);
}

// 在缓存上绘图
void CLKCheckBox::OnPaint(CDC *pDC)
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
void CLKCheckBox::InitLClick()
{
	//m_bSelected = !m_bSelected;
    SetSelected(!m_bSelected);
}

// 设置选中状态
void CLKCheckBox::SetSelected(bool bSelected, bool bErase)
{ 
    //m_bSelected = bSelected;
    //Invalidate(bErase);

    if(m_bSelected != bSelected)
    {
        GetParent()->PostMessage(LKCOMMON_MSG, LKBUTTONEX_MSG_WPARAM_SELECTEDCHANGEED2, (LPARAM)this);
        m_bSelected = bSelected;
        if(IsWindowVisible() && bErase)
        {
            Invalidate();
        }
    }
}
