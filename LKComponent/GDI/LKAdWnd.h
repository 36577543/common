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
	// ���ε����˵�
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
	// ����Ĭ��ͼƬ
	void SetImage(CLKImage *pImg){m_pImg = pImg;}
	// ��������
	void SetUrlString(UINT nIDTemplate, CString strUrl){m_strUrl = strUrl; m_nIDTemplate = nIDTemplate; InitHtmlDlg();}
	// ˢ��
	void UpdateDlg(){ReSetMem();}
	// ���߹�洰��
	CLKAdDialog	*m_pDlgAd;
protected:
	DECLARE_MESSAGE_MAP()
	// ˢ��ǰ������
	virtual void ReSetMem();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
private:
	CLKImage	*m_pImg;
	// ��ҳ�Ƿ���Ч
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


