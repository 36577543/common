#pragma once

#include "LKCommonCtrl.h"
// CLKButton

// 默认按钮宽度
#define DEFAULT_BUTTONWIDTH     64
// 默认按钮高度
#define DEFAULT_BUTTONHEIGHT    22


class CLKButton : public CLKCommonCtrl
{
	DECLARE_DYNAMIC(CLKButton)

public:
	CLKButton(int nImageIndex = 1);
	virtual ~CLKButton();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateHide(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateHide(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// 设置图标宽度，高度
	void SetIconWidthAndHeight(int nW, int nH){ m_nIconWidth = nW; m_nIconHeight = nH; }
	void SetImage(int nImageIndex);
	void SetParseImage(int nImageIndex);
	void SetLeftMargin(int nLeftMargin){m_nLeftMargin = nLeftMargin;}
	// 鼠标按下时图标与文本的偏移量
	void SetHighLightOffset(int nHighLinghtOffset = 1){m_nHighLinghtOffset = nHighLinghtOffset;}
	//// 设置文本颜色
	//void SetTextColor(COLORREF crText);//{m_crText = crText;}
	//// 设备文本字体
	//void SetTextFont(CFont *pFont){ m_pFtText = pFont; }
protected:
	DECLARE_MESSAGE_MAP()
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化图片列表
	virtual void InitImageList();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
	// 察除背景消息
	virtual bool OnEraseBG(CDC *pDC);
	// 获取需要解析的图片
	virtual CLKImage *GetParseImg();
private:
	// 图标宽度
	int m_nIconWidth = 16; 
	// 图标高度
	int m_nIconHeight = 16;
	CLKImage	*m_pImage;
	CLKImage	*m_pBGImage;
	int			m_nLeftMargin;
	int			m_nIconTextSplit;
	// 鼠标按下时图标与文本的偏移量
	int			m_nHighLinghtOffset;
	//COLORREF	m_crText;
	//CFont		*m_pFtText;
};


