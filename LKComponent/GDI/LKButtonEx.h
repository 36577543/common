#pragma once
#include "LKButton.h"


// CLKButtonEx

class CLKButtonEx : public CLKButton
{
	DECLARE_DYNAMIC(CLKButtonEx)

public:
	CLKButtonEx(CLKImage *pImg = 0, CString strImage = _T(""));
	virtual ~CLKButtonEx();
	// 设置选中状态
	void SetSelected(bool bSelected = true);
	// 获取主图片
	CLKImage *GetButtonImg(){return m_pImg;}
	// 获取主图片路径
	CString GetButtonImgPath(){return m_strImage;}
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 获取需要解析的图片
	virtual CLKImage *GetParseImg();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
private:
    // 是否在拖动
    bool     m_bDrag;
	// 图片路径
	CString	 m_strImage;
	// 是否选中
	bool	 m_bSelected;
	// 按钮图片
	CLKImage *m_pImg;
	// 按钮删除图片
	CLKImage *m_pDeleteImg;
	// 鼠标是否在删除图片上
	bool	 m_bDeletedMouseDown;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


