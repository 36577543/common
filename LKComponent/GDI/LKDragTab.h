#pragma once
#include "LKTabEx.h"


// CLKDragTab

class CLKDragTab : public CLKTabEx
{
	DECLARE_DYNAMIC(CLKDragTab)

public:
	CLKDragTab(int nImageIndex = 1);
	virtual ~CLKDragTab();
    // ��ȡ����ͼƬ�ı߿�
    CRect GetBGImgFrameRect();
protected:
	DECLARE_MESSAGE_MAP()
};


