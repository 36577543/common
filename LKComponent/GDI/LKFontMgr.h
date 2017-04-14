/******************************************************************************
* ��Ȩ���� (C)2010, Sandongcun������
*
* �ļ����ƣ�LKFontMgr.h
* ����ժҪ�����������
* ����˵�����ڶ�̬���д����µ�������Ļ�,����̬��ģ���ͷŵ�ʱ����Ӧ���ڴ汻�ͷţ�
            ��ʱ�������ģ�����ٴ��ͷų����쳣
* ��ǰ�汾��v1.0
* ��    �ߣ�����
* ������ڣ�2012.04.12  
******************************************************************************/
#pragma once
#include "..\..\Single.h"
#include "GDICommon.h"

class CLKFontMgr :public CSingle<CLKFontMgr>
{
public:
	CLKFontMgr(void);
	~CLKFontMgr(void);
	static CFont *GetMSYH13px();
	// ΢���ź�12��
	static CFont *GetMSYHPoint12();
	// ΢���ź�13��
	static CFont *GetMSYHPoint13();
	// ΢���ź�14��
	static CFont *GetMSYHPoint14();
	// ΢���ź�72��
	static CFont *GetMSYHPoint72();
	static CFont *GetMSS80();
	static CFont *GetST90();
	static CFont *GetSTB90();
	static CFont *GetST100();
	// ����28��
	static CFont *GetSTPoint28();
	// ����24��
	static CFont *GetSTPoint22();
	// ����16��
	static CFont *GetSTPoint16();
	static CFont *GetSTB100();
	static CFont *GetMSB15();
	static COLORREF GetDefaultTextColor(){return RGB(43, 85, 128);}
	static COLORREF GetYellowTextColor(){return RGB(255, 51, 0);}
	static COLORREF GetGrayTextColor(){ return RGB(160, 160, 160); }
	static COLORREF GetWhiteTextColor(){ return RGB(255, 255, 255); }
private:
	// ΢���ź�13px
	CFont		*m_pFontMSYH13px;
	// ΢���ź�12��
	CFont		*m_pFontMSYHPoint12;
	// ΢���ź�13��
	CFont		*m_pFontMSYHPoint13;
	// ΢���ź�14��
	CFont		*m_pFontMSYHPoint14;
	// ΢���ź�72��
	CFont		*m_pFontMSYHPoint72;
	// ����60
	CFont		*m_pFontMSS80;
	// ����90
	CFont		*m_pFontST90;
	// ����90����
	CFont		*m_pFontSTB90;
	// ����28��
	CFont       *m_pFontSTPoint28;
	// ����24��
	CFont       *m_pFontSTPoint22;
	// ����16��
	CFont       *m_pFontSTPoint16;
	// ����100
	CFont		*m_pFontST100;
	// ����100����
	CFont		*m_pFontSTB100;
	// ΢���15����
	CFont		*m_pFontMS15;
	friend class CSingle<CLKFontMgr>;
};
