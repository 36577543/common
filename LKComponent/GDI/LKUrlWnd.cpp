// ..\..\common\LKComponent\GDI\LKUrlWnd.cpp : ʵ���ļ�
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



// CLKUrlWnd ��Ϣ�������



// �ڻ����ϻ�ͼ
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

// �����ؼ����ʼ��
void CLKUrlWnd::InitCtrlAfterCreate()
{
	//ParseImageToMem();
	InitImageList();
}

// ˢ��ǰ������
void CLKUrlWnd::ReSetMem()
{
	//ParseImageToMem();
	if(IsWindowVisible())
	{
		Invalidate();
	}
}
//// ���������ͼƬ���ڴ滺����
//void CLKUrlWnd::ParseImageToMem()
//{
//}
//
// ��ʼ��ͼƬ�б�
void CLKUrlWnd::InitImageList()
{
	if(!m_pImg)
	{
		//m_pImg = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_ICONAD);
	}
}


void CLKUrlWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!m_strUrl.IsEmpty())
	{
		::ShellExecute(NULL, _T("open"), _T("IEXPLORE.EXE"), m_strUrl, _T(""), SW_SHOWMAXIMIZED);
	}
	CLKCtrlBase::OnLButtonDown(nFlags, point);
}
