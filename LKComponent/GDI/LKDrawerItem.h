#pragma once
#include "LKCtrlBase.h"

typedef struct _DrawerItemButton
{
    // 标识
    int     nId;
    // 图标
    CLKImage *pImg;
    // 标题
    CString strCaption;
    // 按钮类型(0 - 普通按钮; 1 - 复选按钮)
    int     nType;
    // 区域
    CRect   rtWnd;
    // 当前状态（仅对复选按钮有效, 0 - 未选中； 1 - 选中）
    int     nSelect;
    _DrawerItemButton()
    {
        nId         = 0;
        pImg        = 0;
        strCaption  = _T("");
        nType       = 0;
        rtWnd       = CRect(0, 0, 0, 0);
        nSelect     = 0;
    }
    _DrawerItemButton(_DrawerItemButton &obj)
    {
        nId         = obj.nId;
        pImg        = obj.pImg;
        strCaption  = obj.strCaption;
        nType       = obj.nType;
        rtWnd       = obj.rtWnd;
        nSelect     = obj.nSelect;
    }
}DrawerItemButton, *PDrawerItemButton;
typedef CArray<PDrawerItemButton, PDrawerItemButton> PDIBArray;

// 数据
struct DrawItemData : public CtrlDataBase
{
    CString         strCaption;
    bool            bExpand;
    PDIBArray       arrButtons;
    CtrlDataBase    *pData;
    DrawItemData()
    {
        bExpand = false;
        pData   = 0;
    }
    ~DrawItemData()
    {
        for(int i = 0; i < arrButtons.GetCount(); i++)
        {
            delete arrButtons[i];
        }
        if(pData)
        {
            delete pData;
        }
        //arrButtons.RemoveAll();
    }
};

// 三种状态（展开; 收拢; 鼠标处于其上的高亮状态）
// CLKDrawerItem

class CLKDrawerItem : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKDrawerItem)

public:
	CLKDrawerItem(int nImageIndex = 0);
	virtual ~CLKDrawerItem();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
    // 设置标题
    void SetCaption(CString &strCaption){m_strCaption = strCaption;}
    // 获取项的位置
    void GetCurRect(CRect &rtItem){rtItem = m_bExpand ? m_rtExpand : m_rtCollect;}
    // 获取项的头部高度
    int GetHeadHeight(){return m_nHeadHeight;}
    // 设置收拢位置
    void SetCollectRect(CRect &rtItem){m_rtCollect = rtItem;}
    // 设置展开位置
    void SetExpandRect(CRect &rtItem){m_rtExpand = rtItem;}
    // 获取展开状态
    bool GetExpandState(){return m_bExpand;}
    // 设置展开状态
    void SetExpandState(bool bExpand = true);
    // 增加一个按钮
    void AddButton(int nIndex, CString strCaption, CLKImage *pImg = 0, int nButtonType = 0, int nSelect = 0);
    void AddButton(CString strCaption, CLKImage *pImg = 0, int nButtonType = 0);
    // 设置关联数据指针
    virtual void SetDataPtr(PCtrlDataBase pData);
    // 保存数据到内存
    virtual void SaveDataToMem(CtrlDataBase *pData);
    // 从内存中加载数据
    virtual void LoadDataFromMem(CtrlDataBase *pData);
    // 保存子窗口数据到内存
    void SaveChildDataToMem();
    // 从内存中加载数据到子窗口
    void LoadChildDataToMem();
    // 获取子窗口
    CLKCtrlBase *GetChildWnd(){return m_pChildWnd;}
    // 设置子窗口
    void SetChildWnd(CLKCtrlBase *pChildWnd);
    // 调整子控件的区域
    void AdjustChildWndRect();
    // 去除子控件引用
    void RemoveChildWnd(){if(m_pChildWnd){m_pChildWnd->SetParent(0);}}
    LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
	// 刷新前景缓存
	virtual void ReSetMem();
	// 初始化图片列表
	virtual void InitImageList();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
    // 画print背景
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
    // 将前景画在指定的CDC设置上
    virtual void OnDrawFGToDC(BGParam *prm);
	// 察除背景消息
    virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
    // 鼠标指向的当前按钮索引（-2 - 不在当前控件上头部, -1 - 在当前控件头部上但不在按钮上， 0 - 在第一个按钮上。。。。。。）
    int     m_nCurIndex;
    // 鼠标左键按下时的当前按钮索引(与弹起时对应)
    int     m_nCurDownIndex;
    // 鼠标已按下
    bool    m_bMouseDown;
    // 标题
    CString m_strCaption;
    // 状态（true - 展开; false - 收拢） 
    bool    m_bExpand;
    // 收拢时的位置(相对于父窗口的位置)
    CRect   m_rtCollect;
    // 展开时的位置(相对于父窗口的位置)
    CRect   m_rtExpand;
    // 头部高度
    int     m_nHeadHeight;
    // 按钮边空白距离
    int     m_nMargin;
    // 按钮图标与文本的距离
    int     m_nImageTextSplit;
    // 按钮间距
    int     m_nButtonSplit;
    // 字窗口
    CLKCtrlBase *m_pChildWnd;
    // 按钮背景
    CLKImage    *m_pImgButton;
    // 按钮列表
    PDIBArray   m_arrItem;
	// 解析出相关图片到内存缓存中
	void ParseImageToMem();
    // 根据鼠标指针的位置获取项索引
    int GetItemIndexByPoint(CPoint point);
    // 画标签文本
    void DrawCaption(CDC *pDC, CRect &rtText);
    // 画按钮
    void DrawButtons(CDC *pDC);
    void DrawButton(CDC *pDC, PDrawerItemButton pItem, bool bIsCur = false);
    // 计算按钮区域
    void CalcItemRect();
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL DestroyWindow();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
};


