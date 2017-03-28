// ..\..\common\LKComponent\GDI\LKUrlWnd.cpp : 实现文件
//

#include "LKUrlWnd.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"


// CLKUrlWnd

IMPLEMENT_DYNAMIC(CLKUrlWnd, CLKCtrlBase)

CLKUrlWnd::CLKUrlWnd()
: CLKCtrlBase(-1)
, m_pImg(0)
, m_pImg2(0)
{

}

CLKUrlWnd::~CLKUrlWnd()
{
}

BOOL CLKUrlWnd::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, CLKImage *pImg)
{
	m_pImg = pImg;
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID); 
	return bRet;
}

void CLKUrlWnd::SetImage2(CLKImage *pImg)
{
	m_pImg2 = pImg;
	if(GetSafeHwnd())
	{
		ReSetMem();
	}
}

BEGIN_MESSAGE_MAP(CLKUrlWnd, CLKCtrlBase)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CLKUrlWnd 消息处理程序



// 在缓存上绘图
void CLKUrlWnd::OnPaint(CDC *pDC)
{
	if(m_pImg && m_pImg->IsValid())
	{
		m_pImg->DrawToDC(pDC, m_rtClient, 0);
	}
	if(m_pImg2 && m_pImg2->IsValid())
	{
		CRect rt(m_rtClient);
		rt.DeflateRect(4, 4, 4, 4);
		m_pImg2->DrawToDC(pDC, rt, 0);
	}
}

// 创建控件后初始化
void CLKUrlWnd::InitCtrlAfterCreate()
{
	//ParseImageToMem();
	InitImageList();
}

// 刷新前景缓存
void CLKUrlWnd::ReSetMem()
{
	//ParseImageToMem();
	if(IsWindowVisible())
	{
		Invalidate();
	}
}
//// 解析出相关图片到内存缓存中
//void CLKUrlWnd::ParseImageToMem()
//{
//}
//
// 初始化图片列表
void CLKUrlWnd::InitImageList()
{
	if(!m_pImg)
	{
		//m_pImg = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_ICONAD);
	}
}


void CLKUrlWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_strUrl.IsEmpty())
	{
		::ShellExecute(NULL, _T("open"), _T("IEXPLORE.EXE"), m_strUrl, _T(""), SW_SHOWMAXIMIZED);
	}
	CLKCtrlBase::OnLButtonDown(nFlags, point);
}
