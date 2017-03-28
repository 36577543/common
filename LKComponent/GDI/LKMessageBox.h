#pragma once
#include "LKMaskDialog.h"
#include "LKButton.h"


// CLKMessageBox 对话框
#define CLKMessageBoxShow(lpszText, lpszCaption, pIcon, nType) CLKMessageBox::Show(IDD_LKMESSAGEBOX, lpszText, lpszCaption, pIcon, nType)
#define CLKMessageBoxShow1(lpszText, lpszCaption, pIcon) CLKMessageBox::Show(IDD_LKMESSAGEBOX, lpszText, lpszCaption, pIcon, MB_OKCANCEL)
#define CLKMessageBoxShow2(lpszText, lpszCaption) CLKMessageBox::Show(IDD_LKMESSAGEBOX, lpszText, lpszCaption, 0, MB_OKCANCEL)
#define CLKMessageBoxShow3(lpszText, lpszCaption, pParentDlg) CLKMessageBox::Show(IDD_LKMESSAGEBOX, lpszText, lpszCaption, 0, MB_OKCANCEL, pParentDlg)
#define CLKMessageBoxShow4(lpszText) CLKMessageBox::Show(IDD_LKMESSAGEBOX, lpszText, _T("错误"), 0, MB_OKCANCEL, this)
#define CLKMessageBoxShow5(lpszText, ptSize) CLKMessageBox::m_ptSize=ptSize;CLKMessageBox::Show(IDD_LKMESSAGEBOX, lpszText, _T("错误"), 0, MB_OK, this);

class CLKMessageBox : public CLKMaskDialog
{
	DECLARE_DYNAMIC(CLKMessageBox)

public:
    static int Show(UINT nIDTemplate, CString lpszText, CString lpszCaption = _T(""), CLKImage *pIcon = 0, UINT nType = MB_OK, CWnd *pDlg = 0);
	CLKMessageBox(UINT nIDTemplate, CWnd* pParent, CString strText = _T(""), CString strCaption = _T(""), CLKImage *pIcon = 0, UINT nType = MB_OKCANCEL);   // 标准构造函数
	virtual ~CLKMessageBox();
	static POINT m_ptSize;
protected:
	DECLARE_MESSAGE_MAP()
private:
    CString     m_strText;
    CString     m_strCaption;
    CLKImage    *m_pIcon;
    UINT        m_nType;
    CLKButton   m_btnOK;
    CLKButton   m_btnCancel;
public:
    virtual BOOL OnInitDialog();
	// 初始化窗口前景图片
	virtual void OnInitMemImg();
	// modal processing
	//virtual INT_PTR DoModal();

};
