// ..\..\..\common\LKComponent\gdi\LKMonthCalCtrl.cpp : 实现文件
//

#include "LKMonthCalCtrl.h"
#include "LKImageMgr.h"


// CLKMonthCalCtrl

IMPLEMENT_DYNAMIC(CLKMonthCalCtrl, CLKCtrlBase)

CLKMonthCalCtrl::CLKMonthCalCtrl(int nImageIndex)
: CLKCtrlBase(nImageIndex)
{

}

CLKMonthCalCtrl::~CLKMonthCalCtrl()
{
}

BOOL CLKMonthCalCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
    // 初始化图片列表(0 - 按钮背景， 1 - 控件背景， 2 - 箭头) 
	CLKImageMgr::GetPartImageS(m_arrImage, LKIMAGELISTINDEX_MONTHCAL_BEGIN, LKIMAGELISTINDEX_MONTHCAL_END);
	CLKImage *pImage = GetParseImg();
    CRect rtWnd(rect);
	if(pImage)
	{
		int nWidth = pImage->GetWidth();
		int nHeight = pImage->GetHeight() / pImage->GetImageNumber();
        rtWnd.right = rect.left + nWidth;
        rtWnd.bottom = rect.top + nHeight;
	}
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE, rtWnd, pParentWnd, nID); 

    if(pParentWnd)
    {
        CWnd *pWnd = pParentWnd->GetWindow(GW_CHILD);
        if(pWnd)
        {
            CWnd *pWnd2 = pWnd->GetWindow(GW_HWNDLAST);
            if(pWnd2)
            {
                if(pWnd2 == this)
                {
                    pWnd2 = pWnd2->GetWindow(GW_HWNDPREV);
                }
                if(pWnd2)
                {
                    SetWindowPos(pWnd2, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
                }
            }
        }
    }
    //CalcItemSize();
	return bRet;
}


BEGIN_MESSAGE_MAP(CLKMonthCalCtrl, CLKCtrlBase)
END_MESSAGE_MAP()



// CLKMonthCalCtrl 消息处理程序




// 解析出相关图片到内存缓存中
void CLKMonthCalCtrl::ParseImageToMem()
{
    CLKImage *pBGImg = GetParseImg();
	if(pBGImg)
	{
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETBGIMG, (LPARAM)&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);

		CLKImage *pImg = GetMemImg();
		if(pImg) delete pImg;
		pImg = new CLKImage(m_rtClient.Width(), m_rtClient.Height());
	    pBGImg->DrawToImage(pImg, 0, &prm);
		SetMemImg(pImg);
	}
}

// 刷新前景缓存
void CLKMonthCalCtrl::ReSetMem()
{
	ParseImageToMem();
	if(IsWindowVisible())
	{
		Invalidate();
	}
}

// 创建控件后初始化
void CLKMonthCalCtrl::InitCtrlAfterCreate()
{
	ParseImageToMem();
}
// 在缓存上绘图
void CLKMonthCalCtrl::OnPaint(CDC *pDC)
{
	if(GetParseImg())
	{
		CRect rtDest(m_rtClient);
	}
}

// 将前景画在指定的CDC设置上
void CLKMonthCalCtrl::OnDrawFGToDC(BGParam *prm)
{
    CDC *pDC = (CDC *)prm->pBG;
	if(GetParseImg())
	{
		CRect rtDest(m_rtClient);
        rtDest.MoveToXY(prm->rt.left, prm->rt.top);
	}
}

