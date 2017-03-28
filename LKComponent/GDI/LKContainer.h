#pragma once
#include "LKCtrlBaseEx.h"
#include <list>
#include "DynDialogEx.h"
#include "LKCommonCtrl.h"

using namespace std;

// CLKContainerItem
typedef struct CLKContainerItem
{
	int nId;
	// 前景图片
	CLKImage *pImg = 0;
	// 文字
	CString strText;
	// 标志
	CString strFlag;
	// 数据
	void     *pData = 0;
}*PLKContainerItem;

// CLKContainer
class CLKContainer : public CLKCtrlBaseEx
{
	DECLARE_DYNAMIC(CLKContainer)

public:
	CLKContainer(int nImageIndex = 0);
	virtual ~CLKContainer();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	// 隐藏焦点框
	void HideDlg();
	// 插入一个控件子项
	bool InsertItem(int nId, CString strItem, CString strFlag, CLKImage *pImg, void *pData = 0, bool bCalcVisible = false);
	// 删除所有控件子项
	void DeleteAllItem();
	// 控件子项区域左，上，右，下偏移值
	CRect GetSubItemsOffest(){ return m_rtSubItemsOffest; }
	// 控件子项的间隔
	int GetItemSplit(){ return m_nItemSplit; }
	// 控件子项的宽
	int GetItemWidth(){ return m_nItemWidth; }
	// 控件子项的高
	int GetItemHeight(){ return m_nItemWidth; }

protected:
	DECLARE_MESSAGE_MAP()
protected:
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	// 解析出相关图片到内存缓存中
	virtual void ParseImageToMem();
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
	// 察除背景消息
	virtual bool OnEraseBG(CDC *pDC){ return 1; }
private:
	// 总的页数
	int m_nPageCount = 0;
	// 当前页面索引
	int m_nCurPageIndex = 0;
	// 当前高亮的项索引
	int m_nCurItemIndex = -1;
	// 当前选中的项
	CLKContainerItem  *m_pCurItem = 0;
	// 鼠标状态
	EMState		m_emsCommon;
	// 
	CDynDialogEx *m_pDialog = 0;
	// 子项状态变更图片
	CLKImage *m_pItemStateImag = 0; 
	// 左箭头状态图片
	CLKImage *m_pItemLeftArrStateImag = 0;
	// 左箭头状态
	int m_nLeftArrState = 0;
	// 右箭头状态图片
	CLKImage *m_pItemRightArrStateImag = 0;
	// 右箭头状态
	int m_nRightArrState = 0;
	// 背景常态图片(内存缓冲)
	CLKImage			*m_pNormalImg;
	// 控件列表
	list<CLKContainerItem *>   m_ltCtrl;
	// 可见控件列表
	list<CLKContainerItem *>   m_ltCurCtrl;
	// 控件子项的背景图片
	CImage *m_pItemBGImg = 0;
	// 控件子项区域左，上，右，下偏移值
	CRect m_rtSubItemsOffest = CRect(10, 10, 10, 24);
	// 控件子项的间隔
	int m_nItemSplit = 10;
	// 控件子项的宽
	int m_nItemWidth = 468; //138;
	// 控件子项的高
	int m_nItemHeight = 243;// 203;
	// 第一个科技的项
	CLKContainerItem  *m_pVisibleItemBeginItem = 0;
	// 绘制子项
	void DrawItem(CDC *pDC, PLKContainerItem pItem, CRect rtCtrl);
	// 画分页按钮
	void DrawButton(CDC *pDC);
	// 计算鼠标在那个区域上
	bool GetRectByPoint(CRect &r, CPoint &point, int &nIndex);
	// 分页按钮状态是否改变
	bool IsPageBtnStateChange(CPoint &point);
	// 鼠标在左分页按钮上
	bool PointInLeftPageBtn(CPoint &point);
	// 鼠标在右分页按钮上
	bool PointInRightPageBtn(CPoint &point);
	// 计算可见控件列表
	void CalcVisibleCtrl();
	void ProcPageBtnState(CPoint point);
	// 向前翻页
	void PagePrev();
	// 向后翻页
	void PageNext();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


