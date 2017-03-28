// ..\..\common\LKComponent\GDI\LKMaskDialog.cpp : ʵ���ļ�
//

#include "LKMaskDialog.h"
#include "LKImageMgr.h"

// CLKMaskDialog

IMPLEMENT_DYNAMIC(CLKMaskDialog, CLKDialog)

CLKMaskDialog::CLKMaskDialog(UINT nIDTemplate, CWnd* pParent)
: CLKDialog(nIDTemplate, pParent)
, m_pTopImage(0)
, m_pBottomImage(0)
{
	//SetStatusBarHeight(5);
}

CLKMaskDialog::~CLKMaskDialog()
{
}


BEGIN_MESSAGE_MAP(CLKMaskDialog, CLKDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CLKMaskDialog ��Ϣ�������


// ��ʼ��ͼƬ�б�
void CLKMaskDialog::InitImageList()
{
	// ��ʼ��ͼƬ�б�
	CLKDialog::InitImageList();
	//m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MAX] = 0;
	//m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_RESTORE] = 0;
	//m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_SELF] = 0;
	int nIndex = m_arrImage.GetCount();
	// �����ɰ�
	m_pTopImage = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_TOPMASK6);
	// �ײ��ɰ�
	m_pBottomImage = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_BOTTOMMASK27);

	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MAX] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_RESTORE] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_MIN] = 0;
	m_arrImage[LKGDIBASEDIALOG_IMAGEINDEX_SELF] = 0;
}

// ��ʼ������ǰ��ͼƬ
void CLKMaskDialog::OnInitMemImg()
{
	CLKImage *pImg = GetMemImage();
	if(pImg && pImg->IsValid())
	{
		CRect rtClient;
		GetClientRect(&rtClient);
		rtClient.OffsetRect(GetLeftFrameWidth(), GetTitleBarHeight());
		if(m_pTopImage)
		{
			CRect rt(rtClient);
			rt.bottom = rt.top + m_pTopImage->GetHeight();
			//rt.OffsetRect(GetLeftFrameWidth(), GetTitleBarHeight());
			m_pTopImage->DrawToImageLR(pImg, rt); 
			rtClient.top  = rt.bottom;
		}
		if(m_pBottomImage)
		{
			CRect rt(rtClient);
			rt.top = rt.bottom - m_pBottomImage->GetHeight();
			//rt.OffsetRect(GetLeftFrameWidth(), GetTitleBarHeight());
			m_pBottomImage->DrawToImage(pImg, rt); 
			rtClient.bottom = rt.top;
		}

		// ���м��ɰ�
		CLKImage img(1, 1);
		img.SetPixelRGB(0, 0, 255, 255, 255);
		img.DrawToImage(pImg, rtClient, (int)(0.95 * 255));
	}

}


HBRUSH CLKMaskDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(pWnd->GetDlgCtrlID() == IDC_STATIC)
    {
		pDC->SetTextColor(m_crText);
        pDC->SetBkMode( TRANSPARENT );
        return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	HBRUSH hbr = CLKDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
//
//LRESULT CLKMaskDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//	if(WM_GETMINMAXINFO == message)
//	{
//		MINMAXINFO* lpMMI = (MINMAXINFO *)lParam;
//		//int nXFrame = ::GetSystemMetrics(SM_CXSCREEN);
//		//int nYFrame = ::GetSystemMetrics(SM_CYSCREEN);
//		//lpMMI->ptMaxPosition.x = 0;
//		//lpMMI->ptMaxPosition.y = 0;
//		lpMMI->ptMaxSize.x = m_bMinSize.x;
//		lpMMI->ptMaxSize.y = m_bMinSize.y;
//		lpMMI->ptMaxTrackSize.x = m_bMinSize.x;
//		lpMMI->ptMaxTrackSize.y = m_bMinSize.y;
//
//		lpMMI->ptMinTrackSize.x = m_bMinSize.x;
//		lpMMI->ptMinTrackSize.y = m_bMinSize.y;
//		return 1;
//	}
//
//	return CLKDialog::WindowProc(message, wParam, lParam);
//}
//
//BOOL CLKMaskDialog::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//	cs.cx = m_bMinSize.x;
//	cs.cy = m_bMinSize.y;
//	return CLKDialog::PreCreateWindow(cs);
//}

// �޶����������С��
//void CLKMaskDialog::OnSetMinMaxInfo(MINMAXINFO *lpMMI)
//{
//	lpMMI->ptMaxTrackSize.x = m_bMinSize.x;
//	lpMMI->ptMaxTrackSize.y = m_bMinSize.y;
//
//	lpMMI->ptMinTrackSize.x = m_bMinSize.x;
//	lpMMI->ptMinTrackSize.y = m_bMinSize.y;
//}
