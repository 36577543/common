// ..\..\..\common\LKComponent\GDI\LKDragTab.cpp : ʵ���ļ�
//

#include "LKDragTab.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"


// CLKDragTab

IMPLEMENT_DYNAMIC(CLKDragTab, CLKTabEx)

CLKDragTab::CLKDragTab(int nImageIndex)
: CLKTabEx(nImageIndex)
{

}

CLKDragTab::~CLKDragTab()
{
}

// ��ȡ����ͼƬ�ı߿�
CRect CLKDragTab::GetBGImgFrameRect()
{
    return GetCtrlClientRect();
    //return rt;
}

BEGIN_MESSAGE_MAP(CLKDragTab, CLKTabEx)
END_MESSAGE_MAP()



// CLKDragTab ��Ϣ�������


