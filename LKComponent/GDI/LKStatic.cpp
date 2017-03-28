// ..\..\..\common\LKComponent\GDI\LKStatic.cpp : ʵ���ļ�
//

#include "LKStatic.h"
#include "LKFontMgr.h"


// CLKStatic

IMPLEMENT_DYNAMIC(CLKStatic, CLKCtrlBase)

CLKStatic::CLKStatic()
: CLKCtrlBase(-1)
// �Ƿ���ʾ��Ӱ
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

// ������ʾ����
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



// CLKStatic ��Ϣ�������

// ˢ��ǰ������
void CLKStatic::ReSetMem()
{
	ParseImageToMem();

	if(IsWindowVisible())
	{
		Invalidate();
	}
}
// ���������ͼƬ���ڴ滺����
void CLKStatic::ParseImageToMem()
{
}

// ��ʼ��ͼƬ�б�
void CLKStatic::InitImageList()
{
}

// �����ؼ����ʼ��
void CLKStatic::InitCtrlAfterCreate()
{
	ParseImageToMem();
	CLKCtrlBase::InitCtrlAfterCreate();
}

// �ڻ����ϻ�ͼ
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
