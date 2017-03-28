// ..\..\..\common\LKComponent\GDI\LKDragTab.cpp : 实现文件
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

// 获取背景图片的边框
CRect CLKDragTab::GetBGImgFrameRect()
{
    return GetCtrlClientRect();
    //return rt;
}

BEGIN_MESSAGE_MAP(CLKDragTab, CLKTabEx)
END_MESSAGE_MAP()



// CLKDragTab 消息处理程序


