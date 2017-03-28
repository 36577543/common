// ..\..\common\LKComponent\GDI\LKCommonCtrlEx.cpp : ʵ���ļ�
//

#include "LKCommonCtrlEx.h"


// CLKCommonCtrlEx

IMPLEMENT_DYNAMIC(CLKCommonCtrlEx, CLKCtrlBaseEx)

CLKCommonCtrlEx::CLKCommonCtrlEx(int nImageIndex)
: CLKCtrlBaseEx(nImageIndex)
// ǰ����̬ͼƬ(�ڴ滺��)
, m_pNormalImg(0)
// ǰ������ͼƬ(�ڴ滺��)
, m_pHighLightImg(0)
// ǰ������ͼƬ(�ڴ滺��)
, m_pOperatorImg(0)
// ǰ����ֹͼƬ(�ڴ滺��)
, m_pDisableImg(0)
// ��̬���ڴ滺���Ƿ����Լ�������
, m_bImgIsOwner(false)
{
}

CLKCommonCtrlEx::~CLKCommonCtrlEx()
{
	FreeFourStateMemImg();
}

// ������̬����ǰ��(�Ǵ���ģʽ - ����ģʽ)
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



// CLKCommonCtrlEx ��Ϣ�������


// �����ؼ����ʼ��
void CLKCommonCtrlEx::InitCtrlAfterCreate()
{
	ParseImageToMem();
}

// ˢ��ǰ������
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


// ���������ͼƬ���ڴ滺����
void CLKCommonCtrlEx::ParseImageToMem()
{
	m_bImgIsOwner = true;
	//CPaintDC dc(this);
	// ��ȡ��Ҫ������ͼƬ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_pHighLightImg && GetMemImg() == m_pNormalImg)
	{
		SetMemImg(m_pHighLightImg);
		TRACKMOUSEEVENT   tme;   
		tme.cbSize		= sizeof(tme);   
		tme.dwFlags		= TME_LEAVE; 
		tme.hwndTrack	= this->GetSafeHwnd();   
		tme.dwHoverTime = HOVER_DEFAULT; //ֻ��HOVER��Ч
		::TrackMouseEvent(&tme);
		Invalidate();

		// ������
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

// �ͷ���̬����ǰ��
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
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CLKCtrlBaseEx::OnTimer(nIDEvent);
//}
