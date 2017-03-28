#pragma once
#include "LKCtrlBase.h"


// CLKImageWnd

class CLKImageWnd : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKImageWnd)

public:
	CLKImageWnd(CLKImage *pBGImg = 0, CLKImage *pImg = 0);
	virtual ~CLKImageWnd();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

    // ����Ҫ����ͼƬ
    void SetCtrlBGImg(CLKImage *pImg){m_pBGImg = pImg;}
    // ����Ҫ��ʾ��ͼƬ
    void SetCtrlImg(CLKImage *pImg){m_pImg = pImg;}
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
    // ��print����
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
private:
    // ����ͼƬ
    CLKImage *m_pBGImg;
    // Ҫ��ʾ��ͼƬ
    CLKImage *m_pImg;
};


