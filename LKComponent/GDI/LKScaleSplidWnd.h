#pragma once
#include "LKCommonCtrl.h"

// CLKScaleSplidWnd

class CLKScaleSplidWnd : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKScaleSplidWnd)

public:
	CLKScaleSplidWnd(int nImageIndex = 0, bool bLeft = false);
	virtual ~CLKScaleSplidWnd();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

    // 设置左右的窗口
    void SetLRWnd(CWnd *pLeft, CWnd *pRight){m_pLeftWnd = pLeft; m_pRightWnd = pRight;}
    // 获取左窗口指针
    CWnd *GetLWnd(){return m_pLeftWnd;}
    // 获取右窗口指针
    CWnd *GetRWnd(){return m_pRightWnd;}
    // 是否展开
    bool GetIsOpen();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化图片列表
	virtual void InitImageList();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
    // 将前景画在指定的CDC设置上
    virtual void OnDrawFGToDC(BGParam *prm);
	// 察除背景消息
    virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
	// 鼠标状态
	EMState	m_emsCommon;
    // 当前状态(是否正在左边)
    bool    m_bLeft;
    // 左边的窗口
    CWnd    *m_pLeftWnd;
    // 右边的窗口
    CWnd    *m_pRightWnd;
    // 单个图片的高度
    int     m_nItemHeight;
    // 单个图片的宽度
    int     m_nItemWidth;
    // 打开或关闭窗口
    void OpenOrClose();
};


