/******************************************************************************
* 版权所有 (C)2010, Sandongcun开发组
*
* 文件名称：CLKCtrlBase.h
* 内容摘要：组件基类
* 其它说明：
* 当前版本：v1.0
* 作    者：刘坤
* 完成日期：2011.09.25  
******************************************************************************/
#pragma once
#include "LKImage.h"

// 编辑框控件标识
#define LKEDIT_CHILDID_EDIT		1

class CLKCtrlBase;
typedef struct _CtrlDataBase
{
    CLKCtrlBase     *pWnd;
    _CtrlDataBase(){pWnd = 0;}
    //
    virtual ~_CtrlDataBase(){}
}CtrlDataBase, *PCtrlDataBase;

// CLKCtrlBase

class CLKCtrlBase : public CWnd
{
	DECLARE_DYNAMIC(CLKCtrlBase)

public:
	CLKCtrlBase(int nImageIndex = 1);
	virtual ~CLKCtrlBase();
	virtual BOOL Create(UINT nClassStyle, LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// 获取索引图片
	CLKImage *GetImageByArrIndex(int nIndex){if(nIndex >= 0 && nIndex < m_arrImage.GetCount()) return m_arrImage[nIndex]; return 0;}
	BOOL StretchBltPlus(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
	  HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop);
	// 获取控件在父窗口中的位置
	void GetRectInParent(CRect *rt);
	static void GetRectInParent(CWnd *pWnd, CRect *rt);
    // 用两分法查找或是通过计算求出
    static int FindStringIndexByPos(CString strText, int nOffsetPos, CFont *pFont, CDC *pDC);
    static int FindStringIndexByPos(CString strText, int nOffsetPos, LOGFONT *pLF, CDC *pDC);
    int FindStringIndexByPos(CString strText, int nOffsetPos, CFont *pFont);
    int FindStringIndexByPos(CString strText, int nOffsetPos, LOGFONT *pLF);
    // 倒角默认角
    void CreateDefaultRoundWnd();
    // 裁剪客户区
    void ClipClientRect(CRect rtOffset);
	// 获取控件在父窗口客户区中的位置
	void GetRectInParentClient(CRect *rt);
	// 获取窗口高度
	int GetHeight(){return m_rtClient.Height() + m_rtClientOffset.top + m_rtClientOffset.bottom;}
	// 获取窗口宽度
	int GetWidth(){return m_rtClient.Width() + m_rtClientOffset.left + m_rtClientOffset.right;}
	// 设置边框
	void SetClientOffset(CRect rtOffset){m_rtClientOffset = rtOffset;}
	// 获取边框
	CRect GetClientOffset(){return m_rtClientOffset;}
	// 是否获取父背景(或者父前景)
	void SetGetParentBG(bool b = true){m_bGetParentBG = b;}
	// 设置需要解析的图片
	void SetParseImg(CLKImage *pImg);
    // 获取指定字符串的宽度
    static int GetStringWidth(CString &strText, CFont *pFont);
    // 获取指定字符串的高度和宽度
    static void GetStringWidthAndHeight(CString &strText, CFont *pFont, int &nWidth, int &nHeight, CDC *pDC);
    static void GetStringWidthAndHeight(CString &strText, LOGFONT *pLF, int &nWidth, int &nHeight, CDC *pDC);
    void GetStringWidthAndHeight(CString &strText, CFont *pFont, int &nWidth, int &nHeight);
    void GetStringWidthAndHeight(CString &strText, LOGFONT *pLF, int &nWidth, int &nHeight);
    // 设置关联数据指针
    virtual void SetDataPtr(PCtrlDataBase pData){m_pData = pData;}
    // 获取关联数据指针
    PCtrlDataBase GetDataPtr(){return m_pData;}
    // 保存数据到内存
    virtual void SaveDataToMem(CtrlDataBase *pData)
    {
        if(pData)
        {
            pData->pWnd = this;
        }
    }
    virtual void SaveDataToMem(){SaveDataToMem(m_pData);}
    // 从内存中加载数据
    virtual void LoadDataFromMem(CtrlDataBase *pData)
    {
        if(IsWindowVisible())
        {
            Invalidate();
        }
    }
    virtual void LoadDataFromMem(){LoadDataFromMem(m_pData);}
	// 更新背景
	void UpdateMemImage(){PostMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_RESETMEM);}
	// 设置文本颜色
	void SetTextColor(COLORREF crText){m_crText = crText;}
	// 设备文本字体
	void SetTextFont(CFont *pFont){ m_pFtText = pFont; }

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void SetEraseBG(bool bEraseBG){m_bEraseBG = bEraseBG;}
	// 获取是否获取父背景(或者父前景)
	bool GetGetParentBG(){return m_bGetParentBG;}
	// 得到解析图片索引
	int GetImageIndex(){return m_nImageIndex;}
protected:
    // 关联的数据指针
    PCtrlDataBase    m_pData;
	// 是否获取父背景(或者父前景)
	bool		m_bGetParentBG;
	// 是否擦除背景
	bool		m_bEraseBG;
	// 文体颜色
	COLORREF	m_crText;
	// 文本字体
	CFont		*m_pFtText = 0;
	// 非客户边框
	CRect		m_rtClientOffset;
	// 客户区域
	CRect		m_rtClient;
	// 相关图片资源
	ImagePointArray	m_arrImage;
	// 获取前景图片(内存缓冲)
	CLKImage *GetMemImg(){return m_pMemBmp;}
	// 设置前景图片(内存缓冲)
	void SetMemImg(CLKImage *pImg){m_pMemBmp = pImg;}
    // 画print背景
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
    // 将前景画在指定的CDC设置上
    virtual void OnDrawFGToDC(BGParam *prm){}
	// 画背景
	virtual void DrawBG(CDC *pDC);
	// 画非客户区域
	virtual void DrawNCArea(CDC *pDC);
	// 刷新前景缓存
	virtual void ReSetMem(){}
	// 初始化图片列表
	virtual void InitImageList(){}
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate(){}
	// 在缓存上绘图
	virtual void OnPaintEx(CDC *pDC, CDC *pMemDC){OnPaint(pMemDC);}
	virtual void OnPaint(CDC *pDC){}
	// 获取需要解析的图片
	virtual CLKImage *GetParseImg();
	// 察除背景消息
	virtual bool OnEraseBG(CDC *pDC);
	// 没有取得背景
	virtual void OnDrawNoParentBG(CDC *pDC){}
	// 获取父窗口图片
	virtual void GetParentImage(BGParam *prm);
    // 查找从字符串开始偏移位置的索引
    static int FindStringIndexByPos(CString strText, int nOffsetPos, CDC *pDC);
private:
	// 要解析的图片索引(相对于自身列表的起始位置)
	int			m_nImageIndex;
	// 背景图片(如果为空则透明)
	CLKImage	*m_pBGImg;
	// 前景图片(内存缓冲)
	CLKImage	*m_pMemBmp;
};


