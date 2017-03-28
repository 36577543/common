#pragma once
#include "LKCtrlBase.h"


// CLKCommonCtrl

typedef enum mousestate
{
	esb_nohere,
	esb_hover,
	esb_down,
	esb_drag
}EMState;

class CLKCommonCtrl : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKCommonCtrl)

public:
	CLKCommonCtrl(int nImageIndex = 1);
	virtual ~CLKCommonCtrl();
	// 设置四态缓存前景(非创建模式 - 共用模式)
	void SetFourStateMemImg(CLKImage *pNormalImg, CLKImage *pHighLightImg, CLKImage *pOperatorImg, CLKImage *pDisableImg);
	// 获取四态缓存前景
	void GetFourStateMemImg(CLKImage *&pNormalImg, CLKImage *&pHighLightImg, CLKImage *&pOperatorImg, CLKImage *&pDisableImg){pNormalImg = m_pNormalImg; pHighLightImg = m_pHighLightImg; pOperatorImg = m_pOperatorImg; pDisableImg = m_pDisableImg;}
	// 获取正常缓存前景
	CLKImage *GetNormalMemImg(){return m_pNormalImg;}
	// 获取高亮缓存前景
	CLKImage *GetHighLightMemImg(){return m_pHighLightImg;}
	// 获取操作缓存前景
	CLKImage *GetOperatorMemImg(){return m_pOperatorImg;}
	// 获取无效缓存前景
	CLKImage *GetDisableImg(){return m_pDisableImg;}
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化左键单击消息LPARAM参数
	virtual void InitLClick(){}
	// 鼠标进入
	virtual void OnMouseEnter(CPoint point){}
	// 创建normal图片背景
	void CreateNormalImg(int nIndex, BGParam &prm, CLKImage *pImage = 0);
	// 创建高亮图片背景
	void CreateHighLightImg(int nIndex, BGParam &prm, CLKImage *pImage = 0);
	// 创建操作图片背景
	void CreateOperatorImg(int nIndex, BGParam &prm, CLKImage *pImage = 0);
protected:
	// 鼠标状态
	EMState		m_emsCommon;
	// 四态的内存缓冲是否是自己创建的
	bool		m_bImgIsOwner;
	// 前景常态图片(内存缓冲)
	CLKImage	*m_pNormalImg;
	// 前景高亮图片(内存缓冲)
	CLKImage	*m_pHighLightImg;
	// 前景操作图片(内存缓冲)
	CLKImage	*m_pOperatorImg;
	// 前景禁止图片(内存缓冲)
	CLKImage	*m_pDisableImg;
public:
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	// 释放四态缓存前景
	void FreeFourStateMemImg();
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


