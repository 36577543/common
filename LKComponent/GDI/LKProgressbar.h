#pragma once
#include "LKCtrlBase.h"


// CLKProgressbar

class CLKProgressbar : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKProgressbar)

public:
	CLKProgressbar();
	virtual ~CLKProgressbar();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// �������ֵ
	void SetMax(int nMax){m_nMax = nMax;}
	// ���õ�ǰֵ
	void SetCur(int nCur){m_nCur = nCur; if(GetSafeHwnd()) Invalidate();}
	// ������ʾ����
	void SetHintCaption(const CString strCaption){m_strHint = strCaption;}

protected:
	DECLARE_MESSAGE_MAP()
protected:
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
    //// ��print����
    //virtual int OnPrint(WPARAM wParam, LPARAM lParam);
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
	// ���������Ϣ
    virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
	// ���������ֵ
	int	m_nMax;
	// ��������ǰֵ
	int m_nCur;
	// ��ʾ��Ϣ
	CString m_strHint;
};


