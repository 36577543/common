#pragma once
#include "LKCtrlBaseEx.h"
#include "RichOle.h"
#include "LKPictureEx.h"
#include "LKScrollbar.h"


/*
If you use the rich edit control directly from your MFC application, you should call this function to assure that MFC has properly 
initialized the rich edit control runtime. If you use rich edit via CRichEditCtrl, CRichEditView, or CRichEditDoc, you don't need 
to call this function.
*/
//class CLKRichEdit : public CRichEditCtrl
//{
//	DECLARE_DYNAMIC(CLKRichEdit)
//
//public:
//	CLKRichEdit();
//	virtual ~CLKRichEdit();
//	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
//protected:
//	DECLARE_MESSAGE_MAP()
//};
//
//
//
//注册自己的ole 剪贴板格式  
#define   STR_IMSCHOOL_OLE_CLIPBOARD_FORMAT   _T("STR_IMSCHOOL_OLE_CLIPBOARD_FORMAT")  

// CLKRichEdit

class CLKRichEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CLKRichEdit)

public:
	CLKRichEdit();
	virtual ~CLKRichEdit();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// 回调接口的实现，根据不同的上下文获取不同的右键菜单
	HMENU GetContextMenu(WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg);
	void SetOLECallback();
	// 插入一个图片对象
	void InsertImage(int nId, CLKPictureEx *pImg = 0);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	UINT m_uOwnOleClipboardFormat;
protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnLKCtrlMsg(WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL DestroyWindow();
	virtual bool InsertImageI(int nId, int nWidth, int nHeight){return false;}
	static void OnDrawImage(LONG lWnd, CDC *pDC, int nImageId, CRect &rt, int nFrameIndex);
protected:
	// 获取指定接口的ole对象对应的索引
	int GetImageObjectIndex(LPOLEOBJECT lpoleobj);
	// 删除指定接口的ole对象
	virtual bool DeleteImageObject(LPOLEOBJECT lpoleobj){return false;}
	// 在指定位置插入ole对象
	virtual bool InsertImageObject(int nIndex){return false;}
//private:
	// 是否显示滚动条
	bool			m_bShowScrollbar;
	// 垂直滚动条
	CLKScrollbar	m_vScrollbar;
public:
	// 实现IRichEditOleCallback回调接口
	BEGIN_INTERFACE_PART(RichEditOleCallback, IRichEditOleCallback)
		INIT_INTERFACE_PART(CLKRichEdit, RichEditOleCallback)
		STDMETHOD(GetNewStorage)(LPSTORAGE*);
		STDMETHOD(GetInPlaceContext)(LPOLEINPLACEFRAME*, LPOLEINPLACEUIWINDOW*, LPOLEINPLACEFRAMEINFO);
		STDMETHOD(ShowContainerUI)(BOOL);
		STDMETHOD(QueryInsertObject)(LPCLSID, LPSTORAGE, LONG);
		STDMETHOD(DeleteObject)(LPOLEOBJECT);
		STDMETHOD(QueryAcceptData)(LPDATAOBJECT, CLIPFORMAT*, DWORD,BOOL, HGLOBAL);
		STDMETHOD(ContextSensitiveHelp)(BOOL);
		STDMETHOD(GetClipboardData)(CHARRANGE*, DWORD, LPDATAOBJECT*);
		STDMETHOD(GetDragDropEffect)(BOOL, DWORD, LPDWORD);
		STDMETHOD(GetContextMenu)(WORD, LPOLEOBJECT, CHARRANGE*, HMENU*);
	END_INTERFACE_PART(RichEditOleCallback)
	DECLARE_INTERFACE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

