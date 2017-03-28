// ..\..\..\common\LKComponent\GDI\LKImageWnd.cpp : ʵ���ļ�
//

#include "LKImageWnd.h"
#include "LKImageMgr.h"


// CLKImageWnd

IMPLEMENT_DYNAMIC(CLKImageWnd, CLKCtrlBase)

CLKImageWnd::CLKImageWnd(CLKImage *pBGImg, CLKImage *pImg)
: CLKCtrlBase(-1)
, m_pBGImg(pBGImg)
, m_pImg(pImg)
{
}

CLKImageWnd::~CLKImageWnd()
{
    CLKImage *pImg = GetMemImg();
    if(pImg) delete pImg;
}


BOOL CLKImageWnd::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}

BEGIN_MESSAGE_MAP(CLKImageWnd, CLKCtrlBase)
END_MESSAGE_MAP()



// CLKImageWnd ��Ϣ�������


// ˢ��ǰ������
void CLKImageWnd::ReSetMem()
{
	ParseImageToMem();

	if(IsWindowVisible())
	{
		Invalidate();
	}
}
// ���������ͼƬ���ڴ滺����
void CLKImageWnd::ParseImageToMem()
{
	// ��ȡ��Ҫ������ͼƬ
	CLKImage *pImg = m_pBGImg;
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
			CRect rtWnd;
			GetWindowRect(&rtWnd);
			rtWnd.MoveToXY(0, 0);
            CLKImage *pMemImg = GetMemImg();
			if(pMemImg) delete pMemImg;
			pMemImg = new CLKImage(rtWnd.Width(), rtWnd.Height()); 
			AngleOffset angleOffset(4, 0, 4);

			pImg->DrawToImage(pMemImg, &prm, LKSKINBASE_DRAWBGSTYLE_ALL, angleOffset);
			SetMemImg(pMemImg);
		}
	}
}

// ��ʼ��ͼƬ�б�
void CLKImageWnd::InitImageList()
{
	// ��ʼ��ͼƬ�б�
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_EDIT_BEGIN, LKIMAGELISTINDEX_EDIT_END);
}

// �����ؼ����ʼ��
void CLKImageWnd::InitCtrlAfterCreate()
{
	ParseImageToMem();
	CLKCtrlBase::InitCtrlAfterCreate();
}

int CLKImageWnd::OnPrint(WPARAM wParam, LPARAM lParam)
{
    CRect rtWnd;
    GetWindowRect(&rtWnd);
    CDC *pDC = CDC::FromHandle((HDC)wParam);
    CLKImage *pMemImg = GetMemImg();
    if(pMemImg)
    {
        rtWnd.MoveToXY(0, 0);
        pMemImg->DrawToDC(pDC, rtWnd, 0);
    }
    OnPaint(pDC);
    return 0;
}

// �ڻ����ϻ�ͼ
void CLKImageWnd::OnPaint(CDC *pDC)
{
	if(m_pImg && m_pImg->IsValid())
	{
		CRect rtImg(m_rtClient);
		rtImg.DeflateRect(4, 4);
		m_pImg->DrawToDC(pDC, rtImg, 0);
	}
}
