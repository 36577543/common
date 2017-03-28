#pragma once
#include "GDICommon.h"
#include "LKImage.h"

// 状态栏默认高度
#define DEFAULT_STATUSBAR_HEIGHT    5


// 图片索引
// 关闭按钮
#define LKGDIBASEDIALOG_IMAGEINDEX_CLOSE	0
// 最小化按钮
#define LKGDIBASEDIALOG_IMAGEINDEX_MIN		1
// 最大化按钮
#define LKGDIBASEDIALOG_IMAGEINDEX_MAX		2
// 恢复按钮
#define LKGDIBASEDIALOG_IMAGEINDEX_RESTORE	3
// 系统按钮
#define LKGDIBASEDIALOG_IMAGEINDEX_SYS		4
// 自定义按钮
#define LKGDIBASEDIALOG_IMAGEINDEX_SELF		5
// 背景
#define LKGDIBASEDIALOG_IMAGEINDEX_BG		6
// 背景框架
#define LKGDIBASEDIALOG_IMAGEINDEX_BGFRAME	7
// 系统按钮ICO
//#define LKGDIBASEDIALOG_IMAGEINDEX_SYSICON	8

// 图片个数
#define LKGDIBASEDIALOG_IMAGEINDEX_COUNT	8

// 对话框快捷键注册ID
// ALT + S
#define LKDIALOG_HOTKEY_ALTS	1001
// ALT + C
#define LKDIALOG_HOTKEY_ALTC	1002

// 鼠标在非客户区位置标识
enum MOUSENCPOS
{
    // 在最小按钮上
    mnp_sys = 0,
    // 在最小按钮上
    mnp_min,
    // 在最大按钮上
	mnp_max,
    // 在关闭按钮上
    mnp_close,
	// 在自定义按钮上
    mnp_menu,
	// 上边界
	mnp_top,
	// 其它自定义区域
	mnp_other,
    // 未知位置
    mnp_unknow
};

// 图片类型
enum BITMAPSTYLE
{
    // 正常
    bs_normal = 0,
    // 鼠标移动
    bs_mousemove,
    // 鼠标按下
    bs_mousepress
};

struct StatusbarItem
{
    // 作标识符(通常情况下为索引)
    int		nId;
    CString	strCaption;
    CRect	rtCaption;
    //CxImage	imgIcon;
    CRect	rtIcon;
    UINT	uFormat;      // text-drawing options
    CFont   ftCaption;
};

// CLKDialog 对话框

class CLKDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLKDialog)

public:
	CLKDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLKDialog();
    virtual BOOL OnInitDialog();

public:
	// 设置顶部偏移
	void SetTopOffset(int nTopOffset){ m_nTopOffset = nTopOffset; }
	// 设置右边偏移
	void SetRightOffset(int nRightOffset){ m_nRightOffset = nRightOffset; }
	// 设置系统图标
	void SetSysIcon(HICON hIcon){ m_hSysIcon = hIcon; }
	// 设置背景缓存
	void SetBGBmp(CLKImage *pBGBmp);
	// 改变背景
	void ChangeBGImg(CLKImage *pImg = 0);
	// 获取标题栏高度
	int GetTitleBarHeight(){return m_rtClientOffset.top;}
	// 获取左边框宽度
	int GetLeftFrameWidth(){return m_rtClientOffset.left;}
	// 获取右边框宽度
	int GetRightFrameWidth(){return m_rtClientOffset.right;}
	// 获取状态栏高度
	int GetStatusBarHeight(){return m_rtClientOffset.bottom;}
	// 设置非客户区域
	void SetNCClientRect(const CRect &rtNc){m_rtClientOffset = rtNc;}
	// 设置状态栏高度
	void SetStatusBarHeight(int nHeight){m_rtClientOffset.bottom = nHeight;}
    // 获取自定义菜单按钮的区域(相对于屏幕)
    void GetSelfRectByScreen(LPRECT lpRect){*lpRect = m_rtMenuWnd; ((CRect *)lpRect)->OffsetRect(m_rtWnd.left, m_rtWnd.top);}
	// 设置字体颜色
	void SetTextCol(COLORREF colText){m_crText = colText;}
	// 获取字体颜色
	COLORREF GetTextCol(){return m_crText;}
    // 设置提示窗口
    void SetToolTip(CToolTipCtrl *pToolTip){if(m_pToolTip){delete m_pToolTip;} m_pToolTip = pToolTip;m_pToolTip->Activate(TRUE);m_pToolTip->AddTool(this, _T(""));}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	// 响应获取背景图片的消息
	void OnGetBGImgMsg(PBGParam p);
	// 响应获取前景内存图片的消息
	void OnGetMemImgMsg(PBGParam p);
protected:
    // 通用大图标
	static HICON m_hCommonIcon;
    // 通用小图标
	static HICON m_hCommonSmallIcon;
    // 提示窗口
    CToolTipCtrl    *m_pToolTip;
    // 控件提示窗口
    CToolTipCtrl    *m_pChildToolTip;
	// 最小化尺寸
	CPoint			m_bMinSize;
	// 图片列表
	ImagePointArray	m_arrImage;
	// 系统图标(ICO格式)
	HICON			m_hSysIcon;
	// 窗体的最小尺寸
	CPoint			m_ptMinWidthHeight;
	// 文本字体
    CFont			m_ftText;
	// 文本颜色
    COLORREF		m_crText;
    // 存储实时窗体大小
    CRect			m_rtWnd;
    // 存储实时关闭按钮的矩形位置
    CRect			m_rtCloseWnd;
    // 存储实时最大化按钮(恢复按钮)的矩形位置
    CRect			m_rtMaxWnd;
    // 存储实时最小化按钮的矩形位置
    CRect			m_rtMinWnd;
    // 存储自定义菜单按钮的矩形位置
    CRect			m_rtMenuWnd;
	// 获取缓存前景
	CLKImage *GetMemImage(){return m_pMemBmp;}
	// 获取缓存背景
	CLKImage *GetBGImage(){return m_pBGBmp;}
	// 设置缓存背景
	void SetBGImage(CLKImage *pImg, bool bIsOwnerBGBmp = false){m_pBGBmp = pImg; m_bIsOwnerBGBmp = bIsOwnerBGBmp;}
	// 计算状态栏位置
	void CalcStatusbarRect();
    // 计算关闭按钮位置
    void CalcCloseRect();
    // 计算系统按钮位置
    void CalcSysRect();
	// 释放背景对象
	void FreeBGImage(){if(m_bIsOwnerBGBmp && m_pBGBmp){delete m_pBGBmp;}m_pBGBmp = 0;m_bIsOwnerBGBmp = false;}
	// 创建背景对象
	void CreateBGImage(){FreeBGImage();m_pBGBmp = new CLKImage(SCREEN_WIDTH, SCREEN_HEIGHT);m_bIsOwnerBGBmp = true;}
	// 画背景
	void DrawNCBG();
	// 初始化背景缓存
	virtual void InitBGBmp();
    // 获取提供背景DC的窗口句柄
    virtual HWND GetProvideBGHwnd(){return GetParent()->GetSafeHwnd();}
	// 初始化窗口前景图片
	virtual void OnInitMemImg(){}
	// 限定窗口最大化最小化
	virtual void OnSetMinMaxInfo(MINMAXINFO *lpMMI);
    // 事件
    virtual BOOL OnEnterPress(HWND hCrtl, LPARAM lparam);
    // 事件
	virtual BOOL OnEscPress(HWND hCrtl, LPARAM lparam){return true;}
    // 事件
    virtual void OnShowHint(MSG *pMsg);
    virtual void OnChildShowHint(MSG *pMsg);
	// 初始化图片列表
	virtual void InitImageList();
    // 计算最小化按钮位置
    virtual void CalcMinRect();
    // 计算最大化按钮位置
	virtual void CalcMaxRect();
    // 计算自定义菜单按钮位置
    virtual void CalcSelfMenuRect();
    // 判断鼠标是否在最小按钮范围内 
    virtual bool IsInMinbox(CPoint point);
    // 判断鼠标是否在最大按钮范围内
    virtual bool IsInMaxbox(CPoint point);
    // 判断鼠标是否在自定义菜单按钮范围内
    virtual bool IsInMenubox(CPoint point);
    // 判断鼠标是否在其它自定义区域范围内
	virtual bool IsInOther(CPoint point){return false;}
    // 判断鼠标是否在老的按钮范围内
    virtual bool IsInOldbox(CPoint point);
    // 系统按钮被按下
	virtual void OnSysClick(){}
    // 关闭按钮被按下
    virtual void OnCloseClick();
    // 最小化按钮被按下
    virtual void OnMinClick();
    // 最大化按钮被按下 
    virtual void OnMaxClick();
    // 自定义菜单按钮被按下
	virtual void OnMenuClick(){}
    // 自定义区域被按下
	virtual void OnOtherClick(){}
	// 菜单选中消息
	virtual void OnMenuItemSelected(int nId){}
	// 重画非客户区自定义其它区域
	virtual void OnDrawOther(CDC *pDC, BITMAPSTYLE bs, bool bErase = true){}
    // 处理非客户区鼠标左键移动事件 
    virtual bool OnNCMouseMoveDef(UINT nHitTest, CPoint point); 
    // 处理非客户区鼠标左键按下事件 
    virtual bool OnNCLMouseDownDef(UINT nHitTest, CPoint point); 
    // 处理非客户区鼠标左键释放事件 
    virtual bool OnNCLMouseUpDef(UINT nHitTest, CPoint point);
    // 处理非客户区鼠标右键按下事件 
	virtual bool OnNCRMouseDownDef(UINT nHitTest, CPoint point){return true;}
	// 处理非客户区鼠标事件 
	virtual bool OnHitTest(CPoint pt, LRESULT &lr){return false;}
    // 处理窗口wm_print消息
    virtual void OnPrint(CDC *pDC){}
    // 处理窗口wm_paint消息
    virtual void OnPaint(CDC *pDC){}
	// 画窗体客户区
	virtual void DrawClientArea();
    // 处理非客户区尺寸事件
    virtual void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    // 窗口位置状态改变
    virtual void OnWindowPosChange(PWINDOWPOS p){}
	// 打开窗体
	//virtual void OpenMainDlg();
	// 修改窗口区域
	virtual void ModiDialogRectRgn(CRgn &rgn){}
	//// 获取应用程序指定托盘图标区域
	//BOOL FindOutPositionOfIconDirectly(const HWND a_hWndOwner, const int a_iButtonID, CRect& a_rcIcon);
private:
	// 顶部偏移位置(关闭按钮顶部偏移)
	int				m_nTopOffset = 0;
	// 右边偏移位置(关闭按钮右边偏移)
	int				m_nRightOffset = 0;
	// 最大化
	bool			m_bIsMax;
	// 内存背景缓冲
	CLKImage		*m_pBGBmp;
	// 是否是独立背景缓存
	bool			m_bIsOwnerBGBmp;
	// 内存缓冲
	CLKImage		*m_pMemBmp;
	// 客户区偏移量
	CRect			m_rtClientOffset;
	// 状态栏项列表
    CPtrList		m_ltStatusItem;
	// 状态栏中已使用的宽度
	int				m_nUsedWidth;
    // 边框的宽度
    int				m_nBordWidth;
	// 关闭按钮的右边距
	int				m_nCloseBtnRight;
    // 存储实时系统按钮的矩形位置
    CRect			m_rtSysWnd;
    // 存储实时系统按钮的矩形位置
    CPoint			m_ptTitle;
    // 鼠标位置标记
    MOUSENCPOS		m_eMouseNCPos;
    // 鼠标最后一次按下时位置标记
    MOUSENCPOS		m_eMouseNCPressPos;
    // 鼠标移动位置的所在区域是否发生变化（即是否从一个区域移动到另一个区域）
    bool			m_bIsMouseChanged;
	// 上一次拖动顶部时间(计算机启动以来的毫秒数)
	DWORD			m_dwMouseDrag;
    void DrawNCIcon(CDC *pDC, MOUSENCPOS mnp, BITMAPSTYLE bs= bs_normal, bool bErase = true);
    // 画非客户区
    void DrawNClientArea(CDC * pDC, bool bFocus = false);
    // 画边框
    void DrawNClientFrame();
    // 画标题栏文本
    void DrawTitleCaption(CDC *pDC);
    // 重画非客户区系统图标(多图片模式)
    void DrawSys(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
    // 重画非客户区关闭按钮图标(多图片模式)
    void DrawClose(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
    // 重画非客户区最小化按钮图标(多图片模式)
    void DrawMin(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
	// 重画非客户区最大化或恢复按钮图标(多图片模式)
	void DrawMax(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
	// 重画非客户区自定义菜单按钮图标(多图片模式)
	void DrawMenu(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
    // 画失去焦点蒙板
    void DrawKillFocusFrame(CDC *pDC);
    // 画标题栏图标
    void DrawTitleBarIcon(CDC *pDC, bool IsNcPaint = true);
	// 重画非客户区通用按钮(三态图片模式)
	void DrawButton(CDC *pDC, CLKImage *pImage, CRect &rtImg, BITMAPSTYLE bs, bool bErase);
	// 画标题栏
	void DrawTitlebar();
	// 画其它边框
	void DrawFrameEdge();
	// 画标题栏背景
	/*void DrawTitleBarBkEx(CDC * pDC);*/

};
