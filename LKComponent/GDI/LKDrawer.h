#pragma once
//#include "LKCtrlBase.h"
#include "LKDrawerItem.h"

// 数据
struct DrawData : public CtrlDataBase
{
    // 当前选中项
    int                                     nCurIndex;
    // 项列表
    CArray<DrawItemData *, DrawItemData *>  arrItem;
    ~DrawData()
    {
        for(int i = 0; i < arrItem.GetCount(); i++)
        {
            delete arrItem[i];
        }
    }
};

// CLKDrawer

class CLKDrawer : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKDrawer)

public:
	CLKDrawer(int nImageIndex = 0);
	virtual ~CLKDrawer();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateHide(const RECT& rect, CWnd* pParentWnd, UINT nID);

    // 增加一项
    void Add(CString strText = _T(""));
    // 选中一项
    void SelectItem(int nIndex);
    // 获取选中项的索引
    int GetSelectedIndex(){return m_nSelectedIndex;}
    // 增加一个按钮到指定索引项
    void AddButton(int nIndex, CString strCaption, CLKImage *pImg = 0, int nButtonType = 0);
    // 获取子窗口
    CLKCtrlBase *GetChildWnd(int nIndex);
    // 获取子项
    CWnd *GetItemWnd(int nIndex){if(nIndex >= 0 && nIndex < m_arrItem.GetCount()){return m_arrItem[nIndex];}return 0;}
    // 设置子窗口
    void SetChildWnd(int nIndex, CLKCtrlBase *pChildWnd);
    void SetChildWnd(CLKCtrlBase *pChildWnd);
    // 设置关联数据指针
    void SetDataPtr(PCtrlDataBase pData);
    // 保存数据到内存
    virtual void SaveDataToMem(CtrlDataBase *pData);
    // 从内存中加载数据
    virtual void LoadDataFromMem(CtrlDataBase *pData);
	// 是否获取父背景(或者父前景)
	void SetChildGetParentBG(bool b = true);
	//// 重置背景
 //   void ResetBGDC();
protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
	// 初始化图片列表
	virtual void InitImageList();
	// 刷新前景缓存
	virtual void ReSetMem();
	// 察除背景消息
    virtual bool OnEraseBG(CDC *pDC){return 1;}
    // 画print背景
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
private:
    // 当前选中项的索引
    int     m_nSelectedIndex;
    // 项数组
    CArray<CLKDrawerItem *, CLKDrawerItem *> m_arrItem; 
    // 创建项
    void CreateItem(CLKDrawerItem *pItem, int nId);
    void CreateItems();
    // 计算所有项的位置
    void CalcItemsRect();
    // 计算收起时各项位置
    void CalcItemsCollectRect();
    // 调整项位置
    void AdjustItemPos();
    void AdjustItemPos(int nOldIndex);
public:
    afx_msg void OnSetFocus(CWnd* pOldWnd);
//    afx_msg void OnMove(int x, int y);
};


