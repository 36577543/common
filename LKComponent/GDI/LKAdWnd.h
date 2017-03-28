#pragma once
#include "LKCtrlBase.h"
#include <afxdhtml.h>


// CLKAdWnd
class CLKAdDialog : public CDHtmlDialog
{
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(message == WM_ERASEBKGND)
		{
			return 1;
		}else if(WM_PRINT == message)
        {
            //static int n(0);
            //TRACE(L"WM_PRINT:%d\r\n", n++);
            return 0;
        }
		return CWnd::WindowProc(message, wParam, lParam);
	}
	// 屏蔽弹出菜单
	HRESULT STDMETHODCALLTYPE ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
	{
		return S_OK;
	}
	//virtual void OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl)
	//{
	//	DOCHOSTUIINFO info;
	//	GetHostInfo(&info);
	//	CString strUrl;
	//	GetCurrentUrl(strUrl);
	//	if(strUrl.IsEmpty()) return;
	//	CDHtmlDialog::OnBeforeNavigate(pDisp, szUrl);
	//}
	//virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
	//{
	//}
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
	{
		CDHtmlDialog::OnDocumentComplete(pDisp, szUrl); 
		CString strUrl(szUrl);
		if(!strUrl.IsEmpty() && m_spHtmlDoc)
		{
			CComBSTR bstrURL;
			m_spHtmlDoc->get_URL(&bstrURL);
			if(bstrURL == szUrl)
			{
				::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKADDLG_MSG_WPARAM_SUCCESS, 0);
			}
		}
	}
	//BOOL OnInitDialog()
	//{
	//	return CDHtmlDialog::OnInitDialog();
	//}
	void SetUrlString(CString strUrl){m_strCurrentUrl = strUrl;}
};

class CLKAdWnd : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKAdWnd)

public:
	CLKAdWnd();
	virtual ~CLKAdWnd();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, CLKImage *pImg = 0);
	// 设置默认图片
	void SetImage(CLKImage *pImg){m_pImg = pImg;}
	// 设置链接
	void SetUrlString(UINT nIDTemplate, CString strUrl){m_strUrl = strUrl; m_nIDTemplate = nIDTemplate; InitHtmlDlg();}
	// 刷新
	void UpdateDlg(){ReSetMem();}
	// 在线广告窗口
	CLKAdDialog	*m_pDlgAd;
protected:
	DECLARE_MESSAGE_MAP()
	// 刷新前景缓存
	virtual void ReSetMem();
	// 创建控件后初始化
	virtual void InitCtrlAfterCreate();
	// 在缓存上绘图
	virtual void OnPaint(CDC *pDC);
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
private:
	CLKImage	*m_pImg;
	// 网页是否有效
	bool		m_bUrlIsValid;
	// URL
	CString		m_strUrl;
	//
	UINT		m_nIDTemplate;
	//
	void InitHtmlDlg();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


