#pragma once
#include "LKImage.h"
#include "LKFontMgr.h"


// �����еĴ�����
struct HintDlgDataItem
{
    // ����
    int nIndex;
    // ��������
    CRect rtItem;
    // ��������
    CString strText;
    // ����ͼ��
    CLKImage *pImg;
    // ������(0 - �ַ���, 1 - ͼ��, 2 - ��ͨ��ť, 3 - �ޱ�����ť)
    int      nType;
    // �ַ���ɫ
    COLORREF crText;
};

typedef CArray<HintDlgDataItem, HintDlgDataItem &>  HintDlgDataItemArr;

// CLKHintDialogBase �Ի���

class CLKHintDialogBase : public CDialog
{
	DECLARE_DYNAMIC(CLKHintDialogBase)

public:
	CLKHintDialogBase(UINT nIDTemplate, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLKHintDialogBase();
	virtual BOOL Create(CWnd* pParentWnd = NULL);
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
    // �������ñ���ͼƬ
    void SetBGImage(CLKImage *pBGImg);
    // ��ʱ������ʾ����()
    void DelayHideHintDlg();
    void DelayHideHintDlg(int nElapse);
    // ��������ƿ��Ƿ��Զ����ش���
    void SetAutoHideByMouseLeave(bool bAutoHideByMouseLeave = false){m_bAutoHideByMouseLeave = bAutoHideByMouseLeave;}
    // ��ȡ���ڴ�С
    virtual CRect GetHintRect(){CRect rt;GetWindowRect(&rt);rt.MoveToXY(0, 0);return rt;}
    virtual void GetHintRect(CRect *pRt){}
    // ��մ�����
    void ClearItems(){m_arrItem.RemoveAll();}
    // ����һ���ַ�
    void AddStringItem(int nId, CRect rtItem, CString strItem, COLORREF crText);
    void AddStringItem(CRect rtItem, CString strItem, COLORREF crText = CLKFontMgr::GetDefaultTextColor());
    // ����һ�ť
    void AddButtonItem(int nId, CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText);
    void AddButtonItem(CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText = CLKFontMgr::GetDefaultTextColor());
    // ����һ���ƽ��ť
    void AddFlatButtonItem(int nId, CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText);
    void AddFlatButtonItem(CRect rtItem, CString strItem, CLKImage *pImg, COLORREF crText = CLKFontMgr::GetDefaultTextColor());
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	DECLARE_MESSAGE_MAP()
protected:
    virtual void OnDraw(CDC *pDC, CRect rt);
    virtual void OnDrawBG(CDC *pDC, CRect rt);
    virtual void OnDrawItems(CDC *pDC, CRect rt);
    virtual bool IsHideDialog(){return false;}
    bool DoUpdateDummyDialog();
private:
    // ����
    HintDlgDataItemArr  m_arrItem;
    UINT m_nIDTemplate;
    // ����ͼƬ
    CLKImage *m_pBGImg;
    // ����Ƿ����뿪������
    bool     m_bIsLeave;
    // ����Ƿ���
    bool     m_bMouseDown;
    // ��굱ǰ����������
    int      m_nCurItemIndex;
    // ����ƿ��Ƿ��Զ����ش���
    bool    m_bAutoHideByMouseLeave;
    // ����ͨ��ť
    void DrawButton(HintDlgDataItem *pItem, CDC *pDC, CRect rtItem);
    // ����ƽ��ť
    void DrawFlatButton(HintDlgDataItem *pItem, CDC *pDC, CRect rtItem);
public:
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//    afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
