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
//ע���Լ���ole �������ʽ  
#define   STR_IMSCHOOL_OLE_CLIPBOARD_FORMAT   _T("STR_IMSCHOOL_OLE_CLIPBOARD_FORMAT")  

// CLKRichEdit

class CLKRichEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CLKRichEdit)

public:
	CLKRichEdit();
	virtual ~CLKRichEdit();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// �ص��ӿڵ�ʵ�֣����ݲ�ͬ�������Ļ�ȡ��ͬ���Ҽ��˵�
	HMENU GetContextMenu(WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg);
	void SetOLECallback();
	// ����һ��ͼƬ����
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
	// ��ȡָ���ӿڵ�ole�����Ӧ������
	int GetImageObjectIndex(LPOLEOBJECT lpoleobj);
	// ɾ��ָ���ӿڵ�ole����
	virtual bool DeleteImageObject(LPOLEOBJECT lpoleobj){return false;}
	// ��ָ��λ�ò���ole����
	virtual bool InsertImageObject(int nIndex){return false;}
//private:
	// �Ƿ���ʾ������
	bool			m_bShowScrollbar;
	// ��ֱ������
	CLKScrollbar	m_vScrollbar;
public:
	// ʵ��IRichEditOleCallback�ص��ӿ�
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

