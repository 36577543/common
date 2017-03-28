// ..\..\..\common\LKComponent\GDI\LKProgressbar.cpp : 实现文件
//

#include "LKImageMgr.h"
#include "LKFontMgr.h"
#include "LKProgressbar.h"


// CLKProgressbar

IMPLEMENT_DYNAMIC(CLKProgressbar, CLKCtrlBase)

CLKProgressbar::CLKProgressbar()
: CLKCtrlBase(0)
, m_nMax(0)
, m_nCur(0)
, m_strHint(_T(""))
{
	
}

CLKProgressbar::~CLKProgressbar()
{
    CLKImage *pImg = GetMemImg();
    if(pImg) delete pImg;
}

BOOL CLKProgressbar::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, pParentWnd, nID); 
	return bRet;
}

BEGIN_MESSAGE_MAP(CLKProgressbar, CLKCtrlBase)
//	ON_WM_MOVE()
END_MESSAGE_MAP()



// CLKProgressbar 消息处理程序

// 刷新前景缓存
void CLKProgressbar::ReSetMem()
{
	ParseImageToMem();

	if(IsWindowVisible())
	{
		Invalidate();
	}
}
// 解析出相关图片到内存缓存中
void CLKProgressbar::ParseImageToMem()
{
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParentImage(&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);
		int nNumber = pImg->GetImageNumber();
		if(nNumber > 0)
		{
			CRect rtWnd(0, 0, m_rtClient.Width(), pImg->GetHeight());
			rtWnd.OffsetRect(0, 20);
            CLKImage *pMemImg = GetMemImg();
			if(pMemImg) delete pMemImg;
			pMemImg = new CLKImage(rtWnd.Width(), rtWnd.Height() + 20); 
			//AngleOffset angleOffset(4, 0, 4);
			CRect rtOffset(3, 2, 3, 2);
			pImg->DrawToImage(pMemImg, rtWnd, rtOffset, 0, &prm, LKSKINBASE_DRAWBGSTYLE_ALL/*, angleOffset*/);
			SetMemImg(pMemImg);
		}
	}
}

// 初始化图片列表
void CLKProgressbar::InitImageList()
{
	// 初始化图片列表
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_PROGRESSBAR_BEGIN, LKIMAGELISTINDEX_PROGRESSBAR_END);
}

// 创建控件后初始化
void CLKProgressbar::InitCtrlAfterCreate()
{
	ParseImageToMem();
	CLKCtrlBase::InitCtrlAfterCreate();
}

// 在缓存上绘图
void CLKProgressbar::OnPaint(CDC *pDC)
{
	CRect rt(m_rtClient);
	rt.bottom = rt.top + 20;
	if(!m_strHint.IsEmpty())
	{
		CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST90());
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(CLKFontMgr::GetDefaultTextColor());
		pDC->DrawText(m_strHint, &rt, DT_LEFT | DT_VCENTER);
		pDC->SelectObject(pOld);
	}

	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		int nNumber = pImg->GetImageNumber();
		if(nNumber > 0)
		{
			int nLeft = 4;
			int nRight = nLeft;
			if(m_nMax > 0 && m_nCur > 0)
			{
				nRight += (m_rtClient.Width() - 3 - nLeft) * m_nCur / m_nMax;
			}
			CRect rtWnd(nLeft, 2, nRight, pImg->GetHeight() - 2);
			rtWnd.OffsetRect(0, 20);
			CRect rtSrc(3, 3, 4, 4);
			if(!rtWnd.IsRectEmpty())
			{
				pImg->Draw(pDC->GetSafeHdc(), rtWnd, rtSrc);
			}
			//SetMemImg(pMemImg);
		}
	}
}
