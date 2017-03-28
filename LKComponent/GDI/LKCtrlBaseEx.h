#pragma once

#include "LKCtrlBase.h"

struct LKSCROLLINFO
{
	int nMax;
	int nPos;
	int nSliderSize;
    // 每面有多少个位置
	int nPosPerPage;
	// 具体最大尺寸
	int nMax2;
	// 具体位置
	int nPos2;
};

// CLKCtrlBaseEx

class CLKCtrlBaseEx : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKCtrlBaseEx)

public:
	CLKCtrlBaseEx(int nImageIndex = 1);
	virtual ~CLKCtrlBaseEx();

protected:
	DECLARE_MESSAGE_MAP()

	// 察除背景消息
    virtual bool OnEraseBG(CDC *pDC){return 1;}
	// 在缓存上绘图
	virtual void OnPaintEx(CDC *pDC, CDC *pMemDC);
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 计算滚动条位置
	virtual void CalcScrollbarRect();

	// 设置滚动条位置
	void SetScrollbarRect(CRect rtVScrollbar, CRect rtHScrollbar){m_rtVScrollbar = rtVScrollbar;m_rtHScrollbar = rtHScrollbar;}
	void SetVScrollbarRect(CRect rtScrollbar){m_rtVScrollbar = rtScrollbar;}
	void SetHScrollbarRect(CRect rtScrollbar){m_rtHScrollbar = rtScrollbar;}
	// 获取滚动条位置
	void GetVScrollbarRect(CRect &rtScrollbar){rtScrollbar = m_rtVScrollbar;}
	int GetVScrollbarWidth(){return m_rtVScrollbar.Width();}
	void GetHScrollbarRect(CRect &rtScrollbar){rtScrollbar = m_rtHScrollbar;}
	// 滚动条是否有效
	bool IsVScrollbarValid(){return m_bVScrollbarVisible && m_pVScrollbarImg && !m_rtVScrollbar.IsRectEmpty();}
	bool IsHScrollbarValid(){return m_bHScrollbarVisible && m_pHScrollbarImg && !m_rtHScrollbar.IsRectEmpty();}
	// 设置滚动条鼠标捕获标志
	void SetScrollbarCaptureFlag(bool bFlag = true){m_bCapture = bFlag;m_bWndCapture = bFlag;}
	// 获取滚动条正在拖动状态
	bool GetScrollbarDraging(){return m_bScrollbarDraging;}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////// 滚动条相关 放置于客户区域
/////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	// 设置滚动条图片
	void SetScrollbarImg(CLKImage *pVScrollbarImg, CLKImage *pHScrollbarImg){m_pVScrollbarImg = pVScrollbarImg; m_pHScrollbarImg = pHScrollbarImg; CalcScrollbarRect();}
	// 设置垂直滚动条是否可见
	void SetVScrollbarVisible(bool bScrollbarVisible = true){m_bVScrollbarVisible = bScrollbarVisible;}
	// 设置水平滚动条是否可见
	void SetHScrollbarVisible(bool bScrollbarVisible = true){m_bHScrollbarVisible = bScrollbarVisible;}
	// 设置垂直滚动条滑块范围
	void SetVScrollbarRang(int nMax, int nMax2, bool bReDraw = false);//{m_siVScrollbar.nMax = nMax;m_siVScrollbar.nMax2 = nMax2;CalcVSliderSize();}
	void SetVScrollbarRang(int nMax);//{m_siVScrollbar.nMax = nMax;m_siVScrollbar.nMax2 = nMax;CalcVSliderSize();}
	// 设置水平滚动条滑块范围
	void SetHScrollbarRang(int nMax, int nMax2){m_siHScrollbar.nMax = nMax;m_siHScrollbar.nMax2 = nMax2;CalcHSliderSize();}
	void SetHScrollbarRang(int nMax){m_siHScrollbar.nMax = nMax;m_siHScrollbar.nMax2 = nMax;CalcHSliderSize();}
	// 设置垂直滚动条滑块位置
	void SetVScrollbarSliderPos(int nPos = 0);
	// 设置水平滚动条滑块位置
	void SetHScrollbarSliderPos(int nPos = 0);
	// 获取垂直滚动条滑块位置
	int GetVScrollbarSliderPos(){return m_siVScrollbar.nPos;}
	// 获取水平滚动条滑块位置
	//void GetHScrollbarSliderPos(int nPos = 0);
	// 鼠标指针是否在滚动条上
	bool PtInScrollbar(CPoint &point);
private:
	// 正在拖动滚动条
	bool		m_bScrollbarDraging;
	// 鼠标在滚动条上按下时的位置
	CPoint		m_ptOld;
	// 是否捕获鼠标
	bool		m_bCapture;
	// 派生类已捕获鼠标标志
	bool		m_bWndCapture;
	// 垂直滚动条当前图片内索引
	int			m_nCurVScrollbarImgIndex;
	// 水平滚动条当前图片内索引
	int			m_nCurHScrollbarImgIndex;
	// 垂直滚动条信息
	LKSCROLLINFO	m_siVScrollbar;
	// 水平滚动条信息
	LKSCROLLINFO	m_siHScrollbar;
	// 垂直滚动条图片
	CLKImage	*m_pVScrollbarImg;
	// 水平滚动条图片
	CLKImage	*m_pHScrollbarImg;
	// 垂直滚动条是否可见
	bool		m_bVScrollbarVisible;
	// 水平滚动条是否可见
	bool		m_bHScrollbarVisible;
	// 垂直滚动条放置区域
	CRect		m_rtVScrollbar;
	// 水平滚动条放置区域
	CRect		m_rtHScrollbar;
	// 画滚动条及客户区
	void DrawScrollbar(CDC *pDC);
	// 画垂直滚动条
	void DrawVScrollbar(CDC *pDC);
	// 画水平滚动条
	void DrawHScrollbar(CDC *pDC);
	// 更新滑块
	void InvalidateSlider();
	// 更新垂直滚动条滑块
	void InvalidateVSlider(bool bErase = false);
	// 更新水平滚动条滑块
	void InvalidateHSlider();
	// 根据pos计算垂直滚动条滑块位置pos2
	void CalcVSliderPos2();
	// 根据pos2计算垂直滚动条滑块位置pos
	void CalcVSliderPos();
	// 根据pos计算水平滚动条滑块位置pos2
	void CalcHSliderPos2();
	// 根据pos2计算水平滚动条滑块位置pos
	void CalcHSliderPos();
	// 计算垂直滚动条滑块大小
	void CalcVSliderSize();
	// 计算水平滚动条滑块大小
	void CalcHSliderSize();
	// 获取垂直滚动条滑块最下边的位置
	int GetVScrollbarMaxPos();
	// 获取水平滚动条滑块最右边的位置
	int GetHScrollbarMaxPos();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg BOOL OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


