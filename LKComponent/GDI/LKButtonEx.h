#pragma once
#include "LKButton.h"


// CLKButtonEx

class CLKButtonEx : public CLKButton
{
	DECLARE_DYNAMIC(CLKButtonEx)

public:
	CLKButtonEx(CLKImage *pImg = 0, CString strImage = _T(""));
	virtual ~CLKButtonEx();
	// ����ѡ��״̬
	void SetSelected(bool bSelected = true);
	// ��ȡ��ͼƬ
	CLKImage *GetButtonImg(){return m_pImg;}
	// ��ȡ��ͼƬ·��
	CString GetButtonImgPath(){return m_strImage;}
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// ��ȡ��Ҫ������ͼƬ
	virtual CLKImage *GetParseImg();
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
private:
    // �Ƿ����϶�
    bool     m_bDrag;
	// ͼƬ·��
	CString	 m_strImage;
	// �Ƿ�ѡ��
	bool	 m_bSelected;
	// ��ťͼƬ
	CLKImage *m_pImg;
	// ��ťɾ��ͼƬ
	CLKImage *m_pDeleteImg;
	// ����Ƿ���ɾ��ͼƬ��
	bool	 m_bDeletedMouseDown;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


