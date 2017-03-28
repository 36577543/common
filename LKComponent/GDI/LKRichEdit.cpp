// ..\..\..\common\LKComponent\GDI\LKRichEdit.cpp : 实现文件
//

#include "LKRichEdit.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// 更新定时器ID
#define CIMAGEPLUGCTRL_TIMERID_UPDATEIMAGE		1000


//// CLKRichEdit
//
//IMPLEMENT_DYNAMIC(CLKRichEdit, CRichEditCtrl)
//
//CLKRichEdit::CLKRichEdit()
//{
//}
//
//CLKRichEdit::~CLKRichEdit()
//{
//}
//
//
//BOOL CLKRichEdit::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
//{
//	BOOL bRet = CRichEditCtrl::Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL, rect, pParentWnd, nID); 
//	return bRet;
//}
//
//BEGIN_MESSAGE_MAP(CLKRichEdit, CRichEditCtrl)
//END_MESSAGE_MAP()



// CLKRichEdit 消息处理程序






// CLKRichEdit

IMPLEMENT_DYNAMIC(CLKRichEdit, CRichEditCtrl)

CLKRichEdit::CLKRichEdit()
: m_bShowScrollbar(true)
{
     //EnableActiveAccessibility();
	//SetOLECallback(&m_oleCallback);
}

CLKRichEdit::~CLKRichEdit()
{
}

BOOL CLKRichEdit::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CRichEditCtrl::Create(WS_CHILD/* | WS_CLIPCHILDREN*/ | WS_CLIPSIBLINGS | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL/**/ | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN, rect, pParentWnd, nID); 
	m_uOwnOleClipboardFormat = RegisterClipboardFormat(STR_IMSCHOOL_OLE_CLIPBOARD_FORMAT);  
	SetEventMask(GetEventMask() | ENM_CHANGE | ENM_SCROLL | ENM_SCROLLEVENTS | ENM_DRAGDROPDONE | ENM_OBJECTPOSITIONS);

	CRect rtS(rect);
	rtS.MoveToXY(0, 0);
	rtS.right -= 1;
	rtS.left = rtS.right - m_vScrollbar.GetDefaultWidth();
	m_vScrollbar.Create(_T(""), rtS, this, 100, false);
	return bRet;
}

// 插入一个图片对象的ID
void CLKRichEdit::InsertImage(int nId, CLKPictureEx *pImg)
{
	if(0 == pImg)
	{
		int nWidth = 24;
		int nHeight = 24;
		InsertImageI(nId, nWidth, nHeight);
	}else
	{
		int nWidth = pImg->GetSize().cx;
		int nHeight = pImg->GetSize().cy;
		InsertImageI(nId, nWidth, nHeight);
	}
	// 设置更新定时器
	SetTimer(CIMAGEPLUGCTRL_TIMERID_UPDATEIMAGE, 10, 0);
}

// 回调接口的实现，根据不同的上下文获取不同的右键菜单
HMENU CLKRichEdit::GetContextMenu(WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg)
{
	//ATLTRACE(_T("CLKRichEdit::GetContextMenu()/r/n"));  
	CMenu menu,*pSubMenu = NULL;
	HMENU hMenu = NULL;
	//if( seltype == SEL_OBJECT )
	//{
	//	menu.LoadMenu( IDR_MENU1 );
	//	pSubMenu = menu.GetSubMenu( 0 );
	//	POINT pt;
	//	GetCursorPos( &pt );
	//	DWORD dwCmd = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RETURNCMD,pt.x,pt.y,this);
	//	if( dwCmd == ID_OLE_SAVE2FILE )
	//	{
	//		CComPtr<IDynamicGif> spDynGif;
	//		lpoleobj->QueryInterface( &spDynGif );
	//		if( spDynGif ){
	//			CComBSTR bstrFile;
	//			// 保存文件到另外一个文件，这里控件根据控件中文件类型的不同设置
	//			// 不同的扩展名，如果采用对话框的形式保存文件时注意分析文件的扩展名，来正确的保存文件类型。
	//			spDynGif->GetFilePath( &bstrFile );
	//			CopyFileW( (BSTR)bstrFile,L"d://1111.gif",FALSE );
	//		}
	//	}
	//}
	return hMenu;
}

int CLKRichEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_pRichEditOle = GetIRichEditOle();
	//if ( !m_bCallbackSet )
	//{
	//	//SetOLECallback( m_pIRichEditOleCallback );
	//}

	return 0;
}

void CLKRichEdit::PreSubclassWindow()
{
     //SetEventMask(ENM_SELCHANGE | ENM_CHANGE | ENM_SCROLL);
	 // 在这里设置回调接口
     //VERIFY(SetOLECallback(&m_xRichEditOleCallback));
	 //BOOL b = ::IsWindow(m_hWnd); 
	 //b = (BOOL)::SendMessage(m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM)&m_xRichEditOleCallback);
     //SetOLECallback(&m_xRichEditOleCallback);
     CRichEditCtrl::PreSubclassWindow();
}

void CLKRichEdit::SetOLECallback()
{
	 BOOL b = ::IsWindow(m_hWnd); 
	 b = (BOOL)::SendMessage(m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM)&m_xRichEditOleCallback);
     //SetOLECallback(&m_xRichEditOleCallback);
     //SetOLECallback(&m_xRichEditOleCallback);
}

BOOL CLKRichEdit::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	//IRichEditOle* lpRichEditOle = GetIRichEditOle();
	//if(m_pRichEditOle) m_pRichEditOle->Release();
	BOOL b = ::IsWindow(m_hWnd); 
	b = (BOOL)::SendMessage(m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM)0);
	m_xRichEditOleCallback.Release();
	return TRUE;
	//return CRichEditCtrl::DestroyWindow();
}

BEGIN_MESSAGE_MAP(CLKRichEdit, CRichEditCtrl)
	ON_MESSAGE(LKCOMMON_MSG, OnLKCtrlMsg)
	ON_WM_CREATE()
	//ON_WM_SIZE()
END_MESSAGE_MAP()



// CLKRichEdit 消息处理程序
LRESULT CLKRichEdit::OnLKCtrlMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKCOMMON_MSG_WPARAM_GETMEMIMG == wParam) 
	{
		long style = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
		//if(style & WS_EX_TRANSPARENT)
		//{
			GetParent()->SendMessage(LKCOMMON_MSG, wParam, lParam);
			CRect rt;
			CLKCtrlBase::GetRectInParent(this, &rt);
			PBGParam p = (PBGParam)lParam;
			p->rt.OffsetRect(rt.left, rt.top);
		//}
	}else if(LKCOMMON_MSG_WPARAM_GETBGIMG == wParam) 
	{
		long style = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
		if(style & WS_EX_TRANSPARENT)
		{
			GetParent()->SendMessage(LKCOMMON_MSG, wParam, lParam);
			CRect rt;
			CLKCtrlBase::GetRectInParent(this, &rt);
			PBGParam p = (PBGParam)lParam;
			p->rt.OffsetRect(rt.left, rt.top);
		}
	}
	return 1;
}

void CLKRichEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MessageBeep(100);
	//CRichEditCtrl::OnLButtonDblClk(nFlags, point);
}


// ole控件的回调函数
void CLKRichEdit::OnDrawImage(LONG lWnd, CDC *pDC, int nImageId, CRect &rt, int nFrameIndex)
{
	CWnd *pThis = (CWnd *)lWnd;
	PictureAndID img;
	img.nId = nImageId;
	img.pImage = 0;
	if(pThis && pThis->GetParent())
	{
		pThis->GetParent()->SendMessage(LKCOMMON_MSG, LKRICHEDIT_MSG_WPARAM_GETIMAGEBYID, (LPARAM)&img);
		if(img.pImage)
		{
			img.pImage->UpdateDC(pThis, pDC, rt, nFrameIndex);
		}
	}
}

// 获取指定接口的ole对象对应的索引
int CLKRichEdit::GetImageObjectIndex(LPOLEOBJECT lpoleobj)
{
	IRichEditOle* lpRichEditOle = GetIRichEditOle();
	for(int i = 0; i < lpRichEditOle->GetObjectCount(); i++)
	{
		REOBJECT		reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));             
		reobject.cbStruct	= sizeof(REOBJECT);
		HRESULT hr = lpRichEditOle->GetObject(i, &reobject, REO_GETOBJ_POLEOBJ);
		if(SUCCEEDED(hr))
		{
			if(reobject.poleobj == lpoleobj) 
			{
				reobject.poleobj->Release();
				lpRichEditOle->Release();
				return i;
			}
			reobject.poleobj->Release();
		}
	}
    if(lpRichEditOle)   lpRichEditOle->Release();
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 实现IRichEditOleCallback回调接口
BEGIN_INTERFACE_MAP(CLKRichEdit, CRichEditCtrl)
     // we use IID_IUnknown because richedit doesn't define an IID
     INTERFACE_PART(CLKRichEdit, IID_IUnknown, RichEditOleCallback)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) CLKRichEdit::XRichEditOleCallback::AddRef()
{
     METHOD_PROLOGUE_EX_(CLKRichEdit, RichEditOleCallback)
     return (ULONG)pThis->InternalAddRef();
}

STDMETHODIMP_(ULONG) CLKRichEdit::XRichEditOleCallback::Release()
{
     METHOD_PROLOGUE_EX_(CLKRichEdit, RichEditOleCallback)
     return (ULONG)pThis->InternalRelease();
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
     METHOD_PROLOGUE_EX_(CLKRichEdit, RichEditOleCallback)
     return (HRESULT)pThis->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::GetNewStorage(LPSTORAGE* ppstg)
{
	METHOD_PROLOGUE_EX_(CLKRichEdit, RichEditOleCallback)
	ATLTRACE(_T("CLKRichEdit::XRichEditOleCallback::GetNewStorage()/r/n"));
	COleClientItem item;
	item.GetItemStorageFlat();
	*ppstg = item.m_lpStorage;
	HRESULT hRes = E_OUTOFMEMORY;
	if (item.m_lpStorage != NULL)
	{
		item.m_lpStorage = NULL;
		hRes = S_OK;
	}
	return hRes;

	//if (!ppstg) 
	//	return E_INVALIDARG; 
	//*ppstg = NULL; 

	//// 
	////   We   need   to   create   a   new   storage   for   an   object   to   occupy.     We 're   going 
	////   to   do   this   the   easy   way   and   just   create   a   storage   on   an   HGLOBAL   and   let 
	////   OLE   do   the   management.     When   it   comes   to   saving   things   we 'll   just   let 
	////   the   RichEdit   control   do   the   work.     Keep   in   mind   this   is   not   efficient,   
	////   but   this   program   is   just   for   demonstration. 
	//// 
	//LPLOCKBYTES pLockBytes; 
	//HRESULT hr = CreateILockBytesOnHGlobal(NULL,   TRUE,   &pLockBytes); 
	//if (FAILED(hr)) 
	//	return hr; 
	//hr = StgCreateDocfileOnILockBytes(pLockBytes, STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, ppstg); 
	//pLockBytes-> Release(); 
	//return hr; 
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::GetInPlaceContext(
     LPOLEINPLACEFRAME* lplpFrame, LPOLEINPLACEUIWINDOW* lplpDoc,
     LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
     METHOD_PROLOGUE_EX(CLKRichEdit, RichEditOleCallback)     
     ATLTRACENOTIMPL(_T("CLKRichEdit::XRichEditOleCallback::GetInPlaceContext()/r/n"));
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::ShowContainerUI(BOOL fShow)
{
     METHOD_PROLOGUE_EX(CLKRichEdit, RichEditOleCallback)
     ATLTRACENOTIMPL(_T("CLKRichEdit::XRichEditOleCallback::ShowContainerUI()/r/n"));
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::QueryInsertObject(
     LPCLSID lpclsid/**/, LPSTORAGE pstg/**/, LONG cp/**/)
{
	METHOD_PROLOGUE_EX(CLKRichEdit, RichEditOleCallback)
	ATLTRACE(_T("CLKRichEdit::XRichEditOleCallback::QueryInsertObject() cp :%d/r/n"), cp);
	//if(-1 == cp)
	//{
	//	pThis->InsertImageObject(cp);
	//}
	return S_OK;
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::DeleteObject(LPOLEOBJECT lpoleobj/**/)
{
	METHOD_PROLOGUE_EX(CLKRichEdit, RichEditOleCallback)
	//// 删除指定接口的ole对象
	//pThis->DeleteImageObject(lpoleobj);
	return S_OK;
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::QueryAcceptData(
     LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, DWORD reco,
     BOOL fReally, HGLOBAL hMetaPict)
{
	METHOD_PROLOGUE_EX(CLKRichEdit, RichEditOleCallback)
	ATLTRACENOTIMPL(_T("CLKRichEdit::XRichEditOleCallback::QueryAcceptData()/r/n"));
    //return E_NOTIMPL;  
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::ContextSensitiveHelp(BOOL /*fEnterMode*/)
{
     ATLTRACENOTIMPL(_T("CLKRichEdit::XRichEditOleCallback::ContextSensitiveHelp()/r/n"));    
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::GetClipboardData(
     CHARRANGE* lpchrg, DWORD reco, LPDATAOBJECT* lplpdataobj)
{
	METHOD_PROLOGUE_EX(CLKRichEdit, RichEditOleCallback)
	ATLTRACENOTIMPL(_T("CLKRichEdit::XRichEditOleCallback::GetClipboardData()/r/n"));    
	//return E_NOTIMPL;  
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::GetDragDropEffect(
     BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
     //return S_OK;
     return E_NOTIMPL;
}

STDMETHODIMP CLKRichEdit::XRichEditOleCallback::GetContextMenu(WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg, HMENU* lphmenu)
{
     METHOD_PROLOGUE_EX(CLKRichEdit, RichEditOleCallback)
     ATLTRACE(_T("CLKRichEdit::XRichEditOleCallback::GetContextMenu()/r/n"));
     // 这里把右键菜单的处理委托给控件来处理 
     HMENU hMenu = pThis->GetContextMenu(seltype, lpoleobj, lpchrg);
     if (hMenu == NULL)
         return E_NOTIMPL;
     *lphmenu = hMenu;
     return S_OK;
}

void CLKRichEdit::OnSize(UINT nType, int cx, int cy)
{
	CRichEditCtrl::OnSize(nType, cx, cy);

	//if(m_bShowScrollbar && m_vScrollbar.GetSafeHwnd())
	//{
	//	CRect rt;
	//	GetClientRect(&rt);
	//	rt.right -= 1;
	//	rt.left = rt.right - m_vScrollbar.GetDefaultWidth();
	//	m_vScrollbar.MoveWindow(&rt);
	//}
	// TODO: 在此处添加消息处理程序代码
}
