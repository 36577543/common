#include "LKFontMgr.h"

CLKFontMgr *CSingle<CLKFontMgr>::m_pInstance = 0;

CLKFontMgr::CLKFontMgr(void)
: m_pFontMSYH14px(0)
, m_pFontMSYH13px(0)
, m_pFontST90(0)
, m_pFontSTB90(0)
, m_pFontMSS80(0)
, m_pFontST100(0)
, m_pFontSTB100(0)
, m_pFontMS15(0)
, m_pFontSTPoint28(0)
, m_pFontSTPoint22(0)
, m_pFontSTPoint16(0)
{
}

CLKFontMgr::~CLKFontMgr(void)
{
	if (m_pFontMSYH14px)
	{
		m_pFontMSYH14px->DeleteObject();
		delete m_pFontMSYH14px;
		m_pFontMSYH14px = 0;
	}
	if (m_pFontMSYH13px)
	{
		m_pFontMSYH13px->DeleteObject();
		delete m_pFontMSYH13px;
		m_pFontMSYH13px = 0;
	}
	if (m_pFontMSS80)
	{
		m_pFontMSS80->DeleteObject();
		delete m_pFontMSS80;
        m_pFontMSS80 = 0;
	}
	if (m_pFontST90)
	{
		m_pFontST90->DeleteObject();
		delete m_pFontST90;
        m_pFontST90 = 0;
	}
	if (m_pFontSTB90)
	{
		m_pFontSTB90->DeleteObject();
		delete m_pFontSTB90;
        m_pFontSTB90 = 0;
	}
	if (m_pFontST100)
	{
		m_pFontST100->DeleteObject();
		delete m_pFontST100;
        m_pFontST100 = 0;
	}
	if (m_pFontSTB100)
	{
		m_pFontSTB100->DeleteObject();
		delete m_pFontSTB100;
        m_pFontSTB100 = 0;
	}
	if (m_pFontMS15)
	{
		m_pFontMS15->DeleteObject();
		delete m_pFontMS15;
        m_pFontMS15 = 0;
	}

	if (m_pFontSTPoint28)
	{
		m_pFontSTPoint28->DeleteObject();
		delete m_pFontSTPoint28;
		m_pFontSTPoint28 = 0;
	}
	if (m_pFontSTPoint22)
	{
		m_pFontSTPoint22->DeleteObject();
		delete m_pFontSTPoint22;
		m_pFontSTPoint22 = 0;
	}
	if (m_pFontSTPoint16)
	{
		m_pFontSTPoint16->DeleteObject();
		delete m_pFontSTPoint16;
		m_pFontSTPoint16 = 0;
	}
}

CFont *CLKFontMgr::GetMSYH13px()
{

	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if (!p->m_pFontMSYH13px)
	{
		p->m_pFontMSYH13px = new CFont;
		//p->m_pFontMSYH13px->CreatePointFont(13, L"微软雅黑", NULL);
		p->m_pFontMSYH13px->CreateFont(20, //字体的高度。
			0, //字体的宽度。
			0, //文本行的倾斜角度。
			0, //字符基线的倾斜角度。
			FW_NORMAL, //字符的粗细。
			FALSE, //字体是否为斜体。
			FALSE, //字体是否带下划线。
			FALSE, //字体是否带删除线。
			DEFAULT_CHARSET, //字体的字符集。
			OUT_DEFAULT_PRECIS, //字符的输出精度。
			CLIP_DEFAULT_PRECIS, //字符裁剪精度。
			PROOF_QUALITY, //字体的输出质量。
			DEFAULT_PITCH | FF_SWISS, //字符间距（低两位），字体族（高四位）。
			_T("微软雅黑")); //字体名称。 	

	}
	if (p && p->m_pFontMSYH13px->GetSafeHandle())
	{
		return p->m_pFontMSYH13px;
	}
	return 0;
}

CFont *CLKFontMgr::GetMSYH14px()
{

	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if (!p->m_pFontMSYH14px)
	{
		p->m_pFontMSYH14px = new CFont;
		//p->m_pFontMSYH13px->CreatePointFont(13, L"微软雅黑", NULL);
		p->m_pFontMSYH14px->CreateFont(22, //字体的高度。
			0, //字体的宽度。
			0, //文本行的倾斜角度。
			0, //字符基线的倾斜角度。
			FW_NORMAL, //字符的粗细。
			FALSE, //字体是否为斜体。
			FALSE, //字体是否带下划线。
			FALSE, //字体是否带删除线。
			DEFAULT_CHARSET, //字体的字符集。
			OUT_DEFAULT_PRECIS, //字符的输出精度。
			CLIP_DEFAULT_PRECIS, //字符裁剪精度。
			PROOF_QUALITY, //字体的输出质量。
			DEFAULT_PITCH | FF_SWISS, //字符间距（低两位），字体族（高四位）。
			_T("微软雅黑")); //字体名称。 	

	}
	if (p && p->m_pFontMSYH14px->GetSafeHandle())
	{
		return p->m_pFontMSYH14px;
	}
	return 0;
}

CFont *CLKFontMgr::GetMSS80()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if(!p->m_pFontMSS80)
	{
		p->m_pFontMSS80 = new CFont;

        p->m_pFontMSS80->CreateFont( 14, //字体的高度。
                                    0, //字体的宽度。
                                    0, //文本行的倾斜角度。
                                    0, //字符基线的倾斜角度。
                                    FW_NORMAL, //字符的粗细。
                                    FALSE, //字体是否为斜体。
                                    FALSE, //字体是否带下划线。
                                    FALSE, //字体是否带删除线。
                                    DEFAULT_CHARSET, //字体的字符集。
                                    OUT_DEFAULT_PRECIS, //字符的输出精度。
                                    CLIP_DEFAULT_PRECIS, //字符裁剪精度。
                                    PROOF_QUALITY, //字体的输出质量。
                                    DEFAULT_PITCH | FF_SWISS, //字符间距（低两位），字体族（高四位）。
                                    _T("Microsoft Sans Serif")); //字体名称。 	

    }
    if(p && p->m_pFontMSS80->GetSafeHandle())
    {
	    return p->m_pFontMSS80;
    }
    return 0;
}

CFont *CLKFontMgr::GetST90()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if(!p->m_pFontST90)
	{
		p->m_pFontST90 = new CFont;
		//p->m_pFontST90->CreatePointFont(90,_T("宋体"));

        p->m_pFontST90->CreateFont( 12, //字体的高度。
                                    0, //字体的宽度。
                                    0, //文本行的倾斜角度。
                                    0, //字符基线的倾斜角度。
                                    FW_NORMAL, //字符的粗细。
                                    FALSE, //字体是否为斜体。
                                    FALSE, //字体是否带下划线。
                                    FALSE, //字体是否带删除线。
                                    GB2312_CHARSET, //字体的字符集。
                                    OUT_DEFAULT_PRECIS, //字符的输出精度。
                                    CLIP_DEFAULT_PRECIS, //字符裁剪精度。
                                    PROOF_QUALITY, //字体的输出质量。
                                    DEFAULT_PITCH | FF_SWISS, //字符间距（低两位），字体族（高四位）。
                                    _T("宋体")); //字体名称。 	

    }
    if(p && p->m_pFontST90->GetSafeHandle())
    {
	    return p->m_pFontST90;
    }
    return 0;
}

CFont *CLKFontMgr::GetSTB90()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if(!p->m_pFontSTB90)
	{
		p->m_pFontSTB90 = new CFont;

        p->m_pFontSTB90->CreateFont( 12, //字体的高度。
                                    0, //字体的宽度。
                                    0, //文本行的倾斜角度。
                                    0, //字符基线的倾斜角度。
                                    FW_BOLD, //字符的粗细。
                                    FALSE, //字体是否为斜体。
                                    FALSE, //字体是否带下划线。
                                    FALSE, //字体是否带删除线。
                                    GB2312_CHARSET, //字体的字符集。
                                    OUT_DEFAULT_PRECIS, //字符的输出精度。
                                    CLIP_DEFAULT_PRECIS, //字符裁剪精度。
                                    PROOF_QUALITY, //字体的输出质量。
                                    DEFAULT_PITCH | FF_SWISS, //字符间距（低两位），字体族（高四位）。
                                    _T("宋体")); //字体名称。 	

    }
    if(p && p->m_pFontSTB90->GetSafeHandle())
    {
	    return p->m_pFontSTB90;
    }
    return 0;
}

// 宋体28磅
CFont *CLKFontMgr::GetSTPoint28()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if (!p->m_pFontSTPoint28)
	{
		p->m_pFontSTPoint28 = new CFont;
		p->m_pFontSTPoint28->CreatePointFont(280, _T("宋体"));
	}
	if (p && p->m_pFontSTPoint28->GetSafeHandle())
	{
		return p->m_pFontSTPoint28;
	}
	return 0;
}

// 宋体24磅
CFont *CLKFontMgr::GetSTPoint22()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if (!p->m_pFontSTPoint22)
	{
		p->m_pFontSTPoint22 = new CFont;
		p->m_pFontSTPoint22->CreatePointFont(220, _T("宋体"));
	}
	if (p && p->m_pFontSTPoint22->GetSafeHandle())
	{
		return p->m_pFontSTPoint22;
	}
	return 0;
}

// 宋体16磅
CFont *CLKFontMgr::GetSTPoint16()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if (!p->m_pFontSTPoint16)
	{
		p->m_pFontSTPoint16 = new CFont;
		p->m_pFontSTPoint16->CreatePointFont(160, _T("宋体"));
	}
	if (p && p->m_pFontSTPoint16->GetSafeHandle())
	{
		return p->m_pFontSTPoint16;
	}
	return 0;
}

CFont *CLKFontMgr::GetST100()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if(!p->m_pFontST100)
	{
		p->m_pFontST100 = new CFont;
		p->m_pFontST100->CreatePointFont(100,_T("宋体"));
	}
    if(p && p->m_pFontST100->GetSafeHandle())
    {
	    return p->m_pFontST100;
    }
    return 0;
}

CFont *CLKFontMgr::GetSTB100()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if(!p->m_pFontSTB100)
	{
		p->m_pFontSTB100 = new CFont;

        p->m_pFontSTB100->CreateFont( 13, //字体的高度。
                                    0, //字体的宽度。
                                    0, //文本行的倾斜角度。
                                    0, //字符基线的倾斜角度。
                                    FW_BOLD, //字符的粗细。
                                    FALSE, //字体是否为斜体。
                                    FALSE, //字体是否带下划线。
                                    FALSE, //字体是否带删除线。
                                    GB2312_CHARSET, //字体的字符集。
                                    OUT_DEFAULT_PRECIS, //字符的输出精度。
                                    CLIP_DEFAULT_PRECIS, //字符裁剪精度。
                                    PROOF_QUALITY, //字体的输出质量。
                                    DEFAULT_PITCH | FF_SWISS, //字符间距（低两位），字体族（高四位）。
                                    _T("宋体")); //字体名称。 	

    }
    if(p && p->m_pFontSTB100->GetSafeHandle())
    {
	    return p->m_pFontSTB100;
    }
    return 0;
}

CFont *CLKFontMgr::GetMSB15()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if(!p->m_pFontMS15)
	{
		p->m_pFontMS15 = new CFont;

        p->m_pFontMS15->CreateFont( 15, //字体的高度。
                                    0, //字体的宽度。
                                    0, //文本行的倾斜角度。
                                    0, //字符基线的倾斜角度。
                                    FW_BOLD, //字符的粗细。
                                    FALSE, //字体是否为斜体。
                                    FALSE, //字体是否带下划线。
                                    FALSE, //字体是否带删除线。
                                    GB2312_CHARSET, //字体的字符集。
                                    OUT_DEFAULT_PRECIS, //字符的输出精度。
                                    CLIP_DEFAULT_PRECIS, //字符裁剪精度。
                                    PROOF_QUALITY, //字体的输出质量。
                                    DEFAULT_PITCH | FF_SWISS, //字符间距（低两位），字体族（高四位）。
                                    _T("Microsoft Sans Serif")); //字体名称。 	

    }
    if(p && p->m_pFontMS15->GetSafeHandle())
    {
	    return p->m_pFontMS15;
    }
    return 0;
}
