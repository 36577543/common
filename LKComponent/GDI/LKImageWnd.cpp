// ..\..\..\common\LKComponent\GDI\LKImageWnd.cpp : 实现文件
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



// CLKImageWnd 消息处理程序


// 刷新前景缓存
void CLKImageWnd::ReSetMem()
{
	ParseImageToMem();

	if(IsWindowVisible())
	{
		Invalidate();
	}
}
// 解析出相关图片到内存缓存中
void CLKImageWnd::ParseImageToMem()
{
	// 获取需要解析的图片
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

// 初始化图片列表
void CLKImageWnd::InitImageList()
{
	// 初始化图片列表
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_EDIT_BEGIN, LKIMAGELISTINDEX_EDIT_END);
}

// 创建控件后初始化
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

// 在缓存上绘图
void CLKImageWnd::OnPaint(CDC *pDC)
{
	if(m_pImg && m_pImg->IsValid())
	{
		CRect rtImg(m_rtClient);
		rtImg.DeflateRect(4, 4);
		m_pImg->DrawToDC(pDC, rtImg, 0);
	}
}
