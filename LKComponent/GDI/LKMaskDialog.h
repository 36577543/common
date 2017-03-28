#pragma once
#include "LKDialog.h"


// CLKMaskDialog

class CLKMaskDialog : public CLKDialog
{
	DECLARE_DYNAMIC(CLKMaskDialog)

public:
	CLKMaskDialog(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CLKMaskDialog();

protected:
	DECLARE_MESSAGE_MAP()
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// ��ʼ������ǰ��ͼƬ
	virtual void OnInitMemImg();
    // ���õײ�����ͼƬ
    void SetBottomImg(CLKImage *pImg){m_pBottomImage = pImg;}
	// �޶����������С��
	//virtual void OnSetMinMaxInfo(MINMAXINFO *lpMMI);
private:
	CLKImage	*m_pTopImage;
	CLKImage	*m_pBottomImage;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


