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
		//p->m_pFontMSYH13px->CreatePointFont(13, L"΢���ź�", NULL);
		p->m_pFontMSYH13px->CreateFont(20, //����ĸ߶ȡ�
			0, //����Ŀ�ȡ�
			0, //�ı��е���б�Ƕȡ�
			0, //�ַ����ߵ���б�Ƕȡ�
			FW_NORMAL, //�ַ��Ĵ�ϸ��
			FALSE, //�����Ƿ�Ϊб�塣
			FALSE, //�����Ƿ���»��ߡ�
			FALSE, //�����Ƿ��ɾ���ߡ�
			DEFAULT_CHARSET, //������ַ�����
			OUT_DEFAULT_PRECIS, //�ַ���������ȡ�
			CLIP_DEFAULT_PRECIS, //�ַ��ü����ȡ�
			PROOF_QUALITY, //��������������
			DEFAULT_PITCH | FF_SWISS, //�ַ���ࣨ����λ���������壨����λ����
			_T("΢���ź�")); //�������ơ� 	

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
		//p->m_pFontMSYH13px->CreatePointFont(13, L"΢���ź�", NULL);
		p->m_pFontMSYH14px->CreateFont(22, //����ĸ߶ȡ�
			0, //����Ŀ�ȡ�
			0, //�ı��е���б�Ƕȡ�
			0, //�ַ����ߵ���б�Ƕȡ�
			FW_NORMAL, //�ַ��Ĵ�ϸ��
			FALSE, //�����Ƿ�Ϊб�塣
			FALSE, //�����Ƿ���»��ߡ�
			FALSE, //�����Ƿ��ɾ���ߡ�
			DEFAULT_CHARSET, //������ַ�����
			OUT_DEFAULT_PRECIS, //�ַ���������ȡ�
			CLIP_DEFAULT_PRECIS, //�ַ��ü����ȡ�
			PROOF_QUALITY, //��������������
			DEFAULT_PITCH | FF_SWISS, //�ַ���ࣨ����λ���������壨����λ����
			_T("΢���ź�")); //�������ơ� 	

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

        p->m_pFontMSS80->CreateFont( 14, //����ĸ߶ȡ�
                                    0, //����Ŀ�ȡ�
                                    0, //�ı��е���б�Ƕȡ�
                                    0, //�ַ����ߵ���б�Ƕȡ�
                                    FW_NORMAL, //�ַ��Ĵ�ϸ��
                                    FALSE, //�����Ƿ�Ϊб�塣
                                    FALSE, //�����Ƿ���»��ߡ�
                                    FALSE, //�����Ƿ��ɾ���ߡ�
                                    DEFAULT_CHARSET, //������ַ�����
                                    OUT_DEFAULT_PRECIS, //�ַ���������ȡ�
                                    CLIP_DEFAULT_PRECIS, //�ַ��ü����ȡ�
                                    PROOF_QUALITY, //��������������
                                    DEFAULT_PITCH | FF_SWISS, //�ַ���ࣨ����λ���������壨����λ����
                                    _T("Microsoft Sans Serif")); //�������ơ� 	

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
		//p->m_pFontST90->CreatePointFont(90,_T("����"));

        p->m_pFontST90->CreateFont( 12, //����ĸ߶ȡ�
                                    0, //����Ŀ�ȡ�
                                    0, //�ı��е���б�Ƕȡ�
                                    0, //�ַ����ߵ���б�Ƕȡ�
                                    FW_NORMAL, //�ַ��Ĵ�ϸ��
                                    FALSE, //�����Ƿ�Ϊб�塣
                                    FALSE, //�����Ƿ���»��ߡ�
                                    FALSE, //�����Ƿ��ɾ���ߡ�
                                    GB2312_CHARSET, //������ַ�����
                                    OUT_DEFAULT_PRECIS, //�ַ���������ȡ�
                                    CLIP_DEFAULT_PRECIS, //�ַ��ü����ȡ�
                                    PROOF_QUALITY, //��������������
                                    DEFAULT_PITCH | FF_SWISS, //�ַ���ࣨ����λ���������壨����λ����
                                    _T("����")); //�������ơ� 	

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

        p->m_pFontSTB90->CreateFont( 12, //����ĸ߶ȡ�
                                    0, //����Ŀ�ȡ�
                                    0, //�ı��е���б�Ƕȡ�
                                    0, //�ַ����ߵ���б�Ƕȡ�
                                    FW_BOLD, //�ַ��Ĵ�ϸ��
                                    FALSE, //�����Ƿ�Ϊб�塣
                                    FALSE, //�����Ƿ���»��ߡ�
                                    FALSE, //�����Ƿ��ɾ���ߡ�
                                    GB2312_CHARSET, //������ַ�����
                                    OUT_DEFAULT_PRECIS, //�ַ���������ȡ�
                                    CLIP_DEFAULT_PRECIS, //�ַ��ü����ȡ�
                                    PROOF_QUALITY, //��������������
                                    DEFAULT_PITCH | FF_SWISS, //�ַ���ࣨ����λ���������壨����λ����
                                    _T("����")); //�������ơ� 	

    }
    if(p && p->m_pFontSTB90->GetSafeHandle())
    {
	    return p->m_pFontSTB90;
    }
    return 0;
}

// ����28��
CFont *CLKFontMgr::GetSTPoint28()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if (!p->m_pFontSTPoint28)
	{
		p->m_pFontSTPoint28 = new CFont;
		p->m_pFontSTPoint28->CreatePointFont(280, _T("����"));
	}
	if (p && p->m_pFontSTPoint28->GetSafeHandle())
	{
		return p->m_pFontSTPoint28;
	}
	return 0;
}

// ����24��
CFont *CLKFontMgr::GetSTPoint22()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if (!p->m_pFontSTPoint22)
	{
		p->m_pFontSTPoint22 = new CFont;
		p->m_pFontSTPoint22->CreatePointFont(220, _T("����"));
	}
	if (p && p->m_pFontSTPoint22->GetSafeHandle())
	{
		return p->m_pFontSTPoint22;
	}
	return 0;
}

// ����16��
CFont *CLKFontMgr::GetSTPoint16()
{
	CLKFontMgr *p = CLKFontMgr::GetInstance();
	if (!p->m_pFontSTPoint16)
	{
		p->m_pFontSTPoint16 = new CFont;
		p->m_pFontSTPoint16->CreatePointFont(160, _T("����"));
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
		p->m_pFontST100->CreatePointFont(100,_T("����"));
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

        p->m_pFontSTB100->CreateFont( 13, //����ĸ߶ȡ�
                                    0, //����Ŀ�ȡ�
                                    0, //�ı��е���б�Ƕȡ�
                                    0, //�ַ����ߵ���б�Ƕȡ�
                                    FW_BOLD, //�ַ��Ĵ�ϸ��
                                    FALSE, //�����Ƿ�Ϊб�塣
                                    FALSE, //�����Ƿ���»��ߡ�
                                    FALSE, //�����Ƿ��ɾ���ߡ�
                                    GB2312_CHARSET, //������ַ�����
                                    OUT_DEFAULT_PRECIS, //�ַ���������ȡ�
                                    CLIP_DEFAULT_PRECIS, //�ַ��ü����ȡ�
                                    PROOF_QUALITY, //��������������
                                    DEFAULT_PITCH | FF_SWISS, //�ַ���ࣨ����λ���������壨����λ����
                                    _T("����")); //�������ơ� 	

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

        p->m_pFontMS15->CreateFont( 15, //����ĸ߶ȡ�
                                    0, //����Ŀ�ȡ�
                                    0, //�ı��е���б�Ƕȡ�
                                    0, //�ַ����ߵ���б�Ƕȡ�
                                    FW_BOLD, //�ַ��Ĵ�ϸ��
                                    FALSE, //�����Ƿ�Ϊб�塣
                                    FALSE, //�����Ƿ���»��ߡ�
                                    FALSE, //�����Ƿ��ɾ���ߡ�
                                    GB2312_CHARSET, //������ַ�����
                                    OUT_DEFAULT_PRECIS, //�ַ���������ȡ�
                                    CLIP_DEFAULT_PRECIS, //�ַ��ü����ȡ�
                                    PROOF_QUALITY, //��������������
                                    DEFAULT_PITCH | FF_SWISS, //�ַ���ࣨ����λ���������壨����λ����
                                    _T("Microsoft Sans Serif")); //�������ơ� 	

    }
    if(p && p->m_pFontMS15->GetSafeHandle())
    {
	    return p->m_pFontMS15;
    }
    return 0;
}
