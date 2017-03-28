#pragma once
#include "LKTabEx.h"


// CLKDragTab

class CLKDragTab : public CLKTabEx
{
	DECLARE_DYNAMIC(CLKDragTab)

public:
	CLKDragTab(int nImageIndex = 1);
	virtual ~CLKDragTab();
    // »ñÈ¡±³¾°Í¼Æ¬µÄ±ß¿ò
    CRect GetBGImgFrameRect();
protected:
	DECLARE_MESSAGE_MAP()
};


