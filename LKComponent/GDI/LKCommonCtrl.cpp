// ..\..\common\LKComponent\GDI\LKCommonCtrl.cpp : 实现文件
//

#include "LKCommonCtrl.h"


// CLKCommonCtrl

IMPLEMENT_DYNAMIC(CLKCommonCtrl, CLKCtrlBase)

CLKCommonCtrl::CLKCommonCtrl(int nImageIndex)
: CLKCtrlBase(nImageIndex)
// 前景常态图片(内存缓冲)
, m_pNormalImg(0)
// 前景高亮图片(内存缓冲)
, m_pHighLightImg(0)
// 前景操作图片(内存缓冲)
, m_pOperatorImg(0)
// 前景禁止图片(内存缓冲)
, m_pDisableImg(0)
// 四态的内存缓冲是否是自己创建的
, m_bImgIsOwner(false)
, m_emsCommon(esb_nohere)
{
}

CLKCommonCtrl::~CLKCommonCtrl()
{
	FreeFourStateMemImg();
}

// 设置四态缓存前景(非创建模式 - 共用模式)
void CLKCommonCtrl::SetFourStateMemImg(CLKImage *pNormalImg, CLKImage *pHighLightImg, CLKImage *pOperatorImg, CLKImage *pDisableImg)
{
	FreeFourStateMemImg();
	m_bImgIsOwner = false;
	m_pNormalImg = pNormalImg;
	m_pHighLightImg = pHighLightImg;
	m_pOperatorImg = pOperatorImg;
	m_pDisableImg = pDisableImg;
	m_pNormalImg = m_pNormalImg;
	SetMemImg(m_pNormalImg);
}

BEGIN_MESSAGE_MAP(CLKCommonCtrl, CLKCtrlBase)
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
//	ON_WM_TIMER()
END_MESSAGE_MAP()



// CLKCommonCtrl 消息处理程序


// 创建控件后初始化
void CLKCommonCtrl::InitCtrlAfterCreate()
{
	ParseImageToMem();
}

// 刷新前景缓存
void CLKCommonCtrl::ReSetMem()
{
	if(m_bImgIsOwner)
	{
		ParseImageToMem();
		if(IsWindowVisible())
		{
			Invalidate();
		}
	}
}


// 解析出相关图片到内存缓存中
void CLKCommonCtrl::ParseImageToMem()
{
	m_bImgIsOwner = true;
	//CPaintDC dc(this);
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		// 
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);
		int nNumber = pImg->GetImageNumber();
		if(nNumber > 2)
		{
			if(m_pNormalImg) delete m_pNormalImg;
			m_pNormalImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
			pImg->DrawToImage(m_pNormalImg, 0, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);

			if(m_pHighLightImg) delete m_pHighLightImg;
			m_pHighLightImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
			pImg->DrawToImage(m_pHighLightImg, 1, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);

			if(m_pOperatorImg) delete m_pOperatorImg;
			m_pOperatorImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
			pImg->DrawToImage(m_pOperatorImg, 2, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);

			if(nNumber > 3)
			{
				if(m_pDisableImg) delete m_pDisableImg;
				m_pDisableImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
				pImg->DrawToImage(m_pDisableImg, 3, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
			}
			SetMemImg(m_pNormalImg);
		}else if(nNumber > 1)
		{
			if(m_pHighLightImg) delete m_pHighLightImg;
			m_pHighLightImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
			pImg->DrawToImage(m_pHighLightImg, 0, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);

			if(m_pOperatorImg) delete m_pOperatorImg;
			m_pOperatorImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
			pImg->DrawToImage(m_pOperatorImg, 1, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
		}
	}
}

void CLKCommonCtrl::OnMouseLeave()
{
	if(m_emsCommon != esb_nohere)
	{
		m_emsCommon = esb_nohere;
		SetMemImg(m_pNormalImg);
		Invalidate();
		//SendMessage(WM_NCPAINT, 1);
	}
	//if(GetMemImg() != m_pNormalImg)
	//{
	//	SetMemImg(m_pNormalImg);
	//	if(0 == m_pNormalImg)
	//	{
	//		CRect rt;
	//		GetWindowRect(&rt);
	//		GetParent()->ScreenToClient(&rt);
	//		GetParent()->InvalidateRect(&rt, false);
	//	}else
	//	{
	//		Invalidate();
	//	}
	//}
	CLKCtrlBase::OnMouseLeave();
}

void CLKCommonCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_emsCommon == esb_nohere)
	{
		m_emsCommon = esb_hover;
		TRACKMOUSEEVENT   tme;   
		tme.cbSize		= sizeof(tme);   
		tme.dwFlags		= TME_LEAVE; 
		tme.hwndTrack	= this->GetSafeHwnd();   
		tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
		::TrackMouseEvent(&tme);

		if(m_pHighLightImg && GetMemImg() == m_pNormalImg)
		{
			SetMemImg(m_pHighLightImg);
		}
		Invalidate();
		// 鼠标进入
		OnMouseEnter(point);
	}
	CLKCtrlBase::OnMouseMove(nFlags, point);
}


void CLKCommonCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_emsCommon != esb_down)
	{
		m_emsCommon = esb_down;
		if(m_pOperatorImg && GetMemImg() != m_pOperatorImg)
		{
			SetMemImg(m_pOperatorImg);
		}
		Invalidate();
	}
	//if(m_pOperatorImg && GetMemImg() != m_pOperatorImg)
	//{
	//	SetMemImg(m_pOperatorImg);
	//	Invalidate();
	//}
	CLKCtrlBase::OnLButtonDown(nFlags, point);
}

void CLKCommonCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_emsCommon == esb_down)
	{
		InitLClick();
		::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	}
	if(m_emsCommon != esb_hover)
	{
		m_emsCommon = esb_hover;
		if(GetMemImg() != m_pHighLightImg)
		{
			SetMemImg(m_pHighLightImg);
		}

        // 防止双击后离开消息得不到触发
		TRACKMOUSEEVENT   tme;   
		tme.cbSize		= sizeof(tme);   
		tme.dwFlags		= TME_LEAVE; 
		tme.hwndTrack	= this->GetSafeHwnd();   
		tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
		::TrackMouseEvent(&tme);
		Invalidate();
	}
	CLKCtrlBase::OnLButtonUp(nFlags, point);
}

// 释放四态缓存前景
void CLKCommonCtrl::FreeFourStateMemImg()
{
	if(m_bImgIsOwner)
	{
		if(m_pNormalImg) 
		{
			delete m_pNormalImg;
			m_pNormalImg = 0;
		}
		if(m_pHighLightImg)
		{
			delete m_pHighLightImg;
			m_pHighLightImg = 0;
		}
		if(m_pOperatorImg)
		{
			delete m_pOperatorImg;
			m_pOperatorImg = 0;
		}
		if(m_pDisableImg)
		{
			delete m_pDisableImg;
			m_pDisableImg = 0;
		}
	}
}

// 创建normal图片背景
void CLKCommonCtrl::CreateNormalImg(int nIndex, BGParam &prm, CLKImage *pImage)
{
	m_bImgIsOwner = true;
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		if(m_pNormalImg) delete m_pNormalImg;
		m_pNormalImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
		//if(pImage)
		//{
		//	CRect rtFrame = pImg->GetFrameRect();
		//	CRect rt(rtFrame.left, rtFrame.top, pImg->GetWidth() - rtFrame.right, pImg->GetHeight() / pImg->GetImageNumber() - rtFrame.bottom);
		//	pImage->DrawToImage(m_pNormalImg, rt);
		//}
		pImg->DrawToImage(m_pNormalImg, pImage, nIndex, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
	}
}

// 创建高亮图片背景
void CLKCommonCtrl::CreateHighLightImg(int nIndex, BGParam &prm, CLKImage *pImage)
{
	m_bImgIsOwner = true;
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		if(m_pHighLightImg) delete m_pHighLightImg;
		m_pHighLightImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
		//if(pImage)
		//{
		//	CRect rtFrame = pImg->GetFrameRect();
		//	CRect rt(rtFrame.left, rtFrame.top, pImg->GetWidth() - rtFrame.right, pImg->GetHeight() / pImg->GetImageNumber() - rtFrame.bottom);
		//	pImage->DrawToImage(m_pHighLightImg, rt);
		//}
		pImg->DrawToImage(m_pHighLightImg, pImage, nIndex, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
	}
}

// 创建操作图片背景
void CLKCommonCtrl::CreateOperatorImg(int nIndex, BGParam &prm, CLKImage *pImage)
{
	m_bImgIsOwner = true;
	//CPaintDC dc(this);
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		if(m_pOperatorImg) delete m_pOperatorImg;
		m_pOperatorImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
		//if(pImage)
		//{
		//	CRect rtFrame = pImg->GetFrameRect();
		//	CRect rt(rtFrame.left, rtFrame.top, pImg->GetWidth() - rtFrame.right, pImg->GetHeight() / pImg->GetImageNumber() - rtFrame.bottom);
		//	//pImage->DrawToImage(m_pOperatorImg, rt);
		//}
		pImg->DrawToImage(m_pOperatorImg, pImage, nIndex, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
	}
}

