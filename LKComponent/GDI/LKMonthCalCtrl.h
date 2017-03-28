/******************************************************************************
* ��Ȩ���� (C)2010, Sandongcun������
*
* �ļ����ƣ�CLKMonthCalCtrl.h
* ����ժҪ�������ؼ���
* ����˵����
* ��ǰ�汾��v1.0
* ��    �ߣ�����
* ��ʼ���ڣ�2012.03.30  
* ������ڣ�-  
******************************************************************************/
#pragma once
#include "LKCtrlBase.h"


// CLKMonthCalCtrl

class CLKMonthCalCtrl : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKMonthCalCtrl)

public:
	CLKMonthCalCtrl(int nImageIndex = 1);
	virtual ~CLKMonthCalCtrl();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	DECLARE_MESSAGE_MAP()
protected:
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
    // ��ǰ������ָ����CDC������
    virtual void OnDrawFGToDC(BGParam *prm);
	// ���������Ϣ
    virtual bool OnEraseBG(CDC *pDC){return 1;}

};


