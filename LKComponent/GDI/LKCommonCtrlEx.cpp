// ..\..\common\LKComponent\GDI\LKCommonCtrlEx.cpp : 实现文件
//

#include "LKCommonCtrlEx.h"


// CLKCommonCtrlEx

IMPLEMENT_DYNAMIC(CLKCommonCtrlEx, CLKCtrlBaseEx)

CLKCommonCtrlEx::CLKCommonCtrlEx(int nImageIndex)
: CLKCtrlBaseEx(nImageIndex)
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
{
}

CLKCommonCtrlEx::~CLKCommonCtrlEx()
{
	FreeFourStateMemImg();
}

// 设置四态缓存前景(非创建模式 - 共用模式)
void CLKCommonCtrlEx::SetFourStateMemImg(CLKImage *pNormalImg, CLKImage *pHighLightImg, CLKImage *pOperatorImg, CLKImage *pDisableImg)
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

BEGIN_MESSAGE_MAP(CLKCommonCtrlEx, CLKCtrlBaseEx)
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
//	ON_WM_TIMER()
END_MESSAGE_MAP()



// CLKCommonCtrlEx 消息处理程序


// 创建控件后初始化
void CLKCommonCtrlEx::InitCtrlAfterCreate()
{
	ParseImageToMem();
}

// 刷新前景缓存
void CLKCommonCtrlEx::ReSetMem()
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
void CLKCommonCtrlEx::ParseImageToMem()
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

void CLKCommonCtrlEx::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//static int n = 0;
	//TRACE(_T("mouse leave num: %d\r\n"), n++);
	if(GetMemImg() != m_pNormalImg)
	{
		SetMemImg(m_pNormalImg);
		if(0 == m_pNormalImg)
		{
			CRect rt;
			GetWindowRect(&rt);
			GetParent()->ScreenToClient(&rt);
			GetParent()->InvalidateRect(&rt, false);
		}else
		{
			Invalidate();
		}
	}
	CLKCtrlBaseEx::OnMouseLeave();
}

void CLKCommonCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pHighLightImg && GetMemImg() == m_pNormalImg)
	{
		SetMemImg(m_pHighLightImg);
		TRACKMOUSEEVENT   tme;   
		tme.cbSize		= sizeof(tme);   
		tme.dwFlags		= TME_LEAVE; 
		tme.hwndTrack	= this->GetSafeHwnd();   
		tme.dwHoverTime = HOVER_DEFAULT; //只对HOVER有效
		::TrackMouseEvent(&tme);
		Invalidate();

		// 鼠标进入
		OnMouseEnter(point);
	}
	CLKCtrlBaseEx::OnMouseMove(nFlags, point);
}


void CLKCommonCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_pOperatorImg && GetMemImg() != m_pOperatorImg)
	{
		SetMemImg(m_pOperatorImg);
		Invalidate();
	}
	CLKCtrlBaseEx::OnLButtonDown(nFlags, point);
}

void CLKCommonCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(GetMemImg() == m_pOperatorImg)
	{
		InitLClick();
		::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	}
	if(m_pHighLightImg && GetMemImg() != m_pHighLightImg)
	{
		SetMemImg(m_pHighLightImg);
		Invalidate();
	}
	CLKCtrlBaseEx::OnLButtonUp(nFlags, point);
}

// 释放四态缓存前景
void CLKCommonCtrlEx::FreeFourStateMemImg()
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

//void CLKCommonCtrlEx::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CLKCtrlBaseEx::OnTimer(nIDEvent);
//}
