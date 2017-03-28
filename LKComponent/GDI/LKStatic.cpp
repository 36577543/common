// ..\..\..\common\LKComponent\GDI\LKStatic.cpp : 实现文件
//

#include "LKStatic.h"
#include "LKFontMgr.h"


// CLKStatic

IMPLEMENT_DYNAMIC(CLKStatic, CLKCtrlBase)

CLKStatic::CLKStatic()
: CLKCtrlBase(-1)
// 是否显示阴影
, m_bShadow(false)
{

}

CLKStatic::~CLKStatic()
{
}

BOOL CLKStatic::Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(strCaption, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, pParentWnd, nID); 
	return bRet;
}

// 设置显示内容
void CLKStatic::SetCaption(const CString &strCaption)
{
	if(strCaption.IsEmpty()) return;
	if(GetSafeHwnd())
	{
		SetWindowText(strCaption);
		if(IsWindowVisible())
		{
			Invalidate();
		}
	}
}

BEGIN_MESSAGE_MAP(CLKStatic, CLKCtrlBase)
//	ON_WM_MOVE()
END_MESSAGE_MAP()



// CLKStatic 消息处理程序

// 刷新前景缓存
void CLKStatic::ReSetMem()
{
	ParseImageToMem();

	if(IsWindowVisible())
	{
		Invalidate();
	}
}
// 解析出相关图片到内存缓存中
void CLKStatic::ParseImageToMem()
{
}

// 初始化图片列表
void CLKStatic::InitImageList()
{
}

// 创建控件后初始化
void CLKStatic::InitCtrlAfterCreate()
{
	ParseImageToMem();
	CLKCtrlBase::InitCtrlAfterCreate();
}

// 在缓存上绘图
void CLKStatic::OnPaint(CDC *pDC)
{
	CString strText;
	GetWindowText(strText);
	if(strText.IsEmpty()) return;
	pDC->SetBkMode(TRANSPARENT);

	if(m_bShadow)
	{
		CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST100());
		pDC->SetTextColor(RGB(210, 210, 210));
		DrawShadowText(pDC->GetSafeHdc(), strText, strText.GetLength(), &m_rtClient, DT_LEFT | DT_SINGLELINE | DT_VCENTER, CLKFontMgr::GetDefaultTextColor(), RGB(235, 235, 235), 0, 0);
		pDC->SelectObject(pOld);
	}else
	{
		CFont *pOld = pDC->SelectObject(CLKFontMgr::GetST90());
		pDC->SetTextColor(CLKFontMgr::GetDefaultTextColor());
		pDC->DrawText(strText, &m_rtClient, DT_LEFT | DT_VCENTER);
		pDC->SelectObject(pOld);
	}
}
