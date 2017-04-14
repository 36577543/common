/******************************************************************************
* 版权所有 (C)2010, Sandongcun开发组
*
* 文件名称：LKFontMgr.h
* 内容摘要：字体管理类
* 其它说明：在动态库中创建新的字体类的话,当动态库模块释放的时候相应的内存被释放，
            这时如果在主模块中再次释放出现异常
* 当前版本：v1.0
* 作    者：刘坤
* 完成日期：2012.04.12  
******************************************************************************/
#pragma once
#include "..\..\Single.h"
#include "GDICommon.h"

class CLKFontMgr :public CSingle<CLKFontMgr>
{
public:
	CLKFontMgr(void);
	~CLKFontMgr(void);
	static CFont *GetMSYH13px();
	// 微软雅黑12磅
	static CFont *GetMSYHPoint12();
	// 微软雅黑13磅
	static CFont *GetMSYHPoint13();
	// 微软雅黑14磅
	static CFont *GetMSYHPoint14();
	// 微软雅黑72磅
	static CFont *GetMSYHPoint72();
	static CFont *GetMSS80();
	static CFont *GetST90();
	static CFont *GetSTB90();
	static CFont *GetST100();
	// 宋体28磅
	static CFont *GetSTPoint28();
	// 宋体24磅
	static CFont *GetSTPoint22();
	// 宋体16磅
	static CFont *GetSTPoint16();
	static CFont *GetSTB100();
	static CFont *GetMSB15();
	static COLORREF GetDefaultTextColor(){return RGB(43, 85, 128);}
	static COLORREF GetYellowTextColor(){return RGB(255, 51, 0);}
	static COLORREF GetGrayTextColor(){ return RGB(160, 160, 160); }
	static COLORREF GetWhiteTextColor(){ return RGB(255, 255, 255); }
private:
	// 微软雅黑13px
	CFont		*m_pFontMSYH13px;
	// 微软雅黑12磅
	CFont		*m_pFontMSYHPoint12;
	// 微软雅黑13磅
	CFont		*m_pFontMSYHPoint13;
	// 微软雅黑14磅
	CFont		*m_pFontMSYHPoint14;
	// 微软雅黑72磅
	CFont		*m_pFontMSYHPoint72;
	// 宋体60
	CFont		*m_pFontMSS80;
	// 宋体90
	CFont		*m_pFontST90;
	// 宋体90粗体
	CFont		*m_pFontSTB90;
	// 宋体28磅
	CFont       *m_pFontSTPoint28;
	// 宋体24磅
	CFont       *m_pFontSTPoint22;
	// 宋体16磅
	CFont       *m_pFontSTPoint16;
	// 宋体100
	CFont		*m_pFontST100;
	// 宋体100粗体
	CFont		*m_pFontSTB100;
	// 微软高15粗体
	CFont		*m_pFontMS15;
	friend class CSingle<CLKFontMgr>;
};
