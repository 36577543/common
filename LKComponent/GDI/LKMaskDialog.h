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
	// 初始化图片列表
	virtual void InitImageList();
	// 初始化窗口前景图片
	virtual void OnInitMemImg();
    // 设置底部过渡图片
    void SetBottomImg(CLKImage *pImg){m_pBottomImage = pImg;}
	// 限定窗口最大化最小化
	//virtual void OnSetMinMaxInfo(MINMAXINFO *lpMMI);
private:
	CLKImage	*m_pTopImage;
	CLKImage	*m_pBottomImage;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


