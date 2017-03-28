// ..\..\..\common\LKComponent\GDI\LKProgressbar.cpp : ʵ���ļ�
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



// CLKProgressbar ��Ϣ�������

// ˢ��ǰ������
void CLKProgressbar::ReSetMem()
{
	ParseImageToMem();

	if(IsWindowVisible())
	{
		Invalidate();
	}
}
// ���������ͼƬ���ڴ滺����
void CLKProgressbar::ParseImageToMem()
{
	// ��ȡ��Ҫ������ͼƬ
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

// ��ʼ��ͼƬ�б�
void CLKProgressbar::InitImageList()
{
	// ��ʼ��ͼƬ�б�
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_PROGRESSBAR_BEGIN, LKIMAGELISTINDEX_PROGRESSBAR_END);
}

// �����ؼ����ʼ��
void CLKProgressbar::InitCtrlAfterCreate()
{
	ParseImageToMem();
	CLKCtrlBase::InitCtrlAfterCreate();
}

// �ڻ����ϻ�ͼ
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

	// ��ȡ��Ҫ������ͼƬ
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
