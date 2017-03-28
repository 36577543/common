#pragma once
#include "LKCtrlBase.h"


// CLKUrlWnd

class CLKUrlWnd : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKUrlWnd)

public:
	CLKUrlWnd();
	virtual ~CLKUrlWnd();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, CLKImage *pImg = 0);
	// ����Ĭ��ͼƬ
	void SetImage(CLKImage *pImg){m_pImg = pImg;}
	void SetImage2(CLKImage *pImg);
	// ��ȡͼƬ
	CLKImage *GetImage(){return m_pImg;}
	CLKImage *GetImage2(){return m_pImg2;}
	// ��������
	void SetUrlString(CString strUrl){m_strUrl = strUrl;}
protected:
	DECLARE_MESSAGE_MAP()
	//// ���������ͼƬ���ڴ滺����
	//virtual void ParseImageToMem();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
	// ���������Ϣ
	virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
	CLKImage *m_pImg;
	CLKImage *m_pImg2;
	// URL
	CString		m_strUrl;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


