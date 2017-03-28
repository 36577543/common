/******************************************************************************
* 版权所有 (C)2010, Sandongcun开发组
*
* 文件名称：CLKMonthCalCtrl.h
* 内容摘要：日历控件类
* 其它说明：
* 当前版本：v1.0
* 作    者：刘坤
* 开始日期：2012.03.30  
* 完成日期：-  
******************************************************************************/
#pragma once
#include "LKCtrlBase.h"


// CLKMonthCalCtrl

class CLKMonthCalCtrl : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKMonthCalCtrl)

public:
	CLKMonthCalCtrl(int nImageIndex = 1);
	virtual ~CLKMonthCalCtrl();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	DECLARE_MESSAGE_MAP()
protected:
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
    // 将前景画在指定的CDC设置上
    virtual void OnDrawFGToDC(BGParam *prm);
	// 察除背景消息
    virtual bool OnEraseBG(CDC *pDC){return 1;}

};


