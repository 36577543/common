#pragma once
#include "LKCtrlBase.h"


// CLKStatic

class CLKStatic : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKStatic)

public:
	CLKStatic();
	virtual ~CLKStatic();
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// �Ƿ���ʾ��Ӱ
	void SetShadow(bool bShadow = true){m_bShadow = bShadow;}
	// ������ʾ����
	void SetCaption(const CString &strCaption);
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
	// �Ƿ���ʾ��Ӱ
	bool m_bShadow;
public:
//	afx_msg void OnMove(int x, int y);
};


