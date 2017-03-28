#pragma once
#include "LKDialog.h"

// ������ͣ����ʽ
// ������ͣ��������
#define WM_MAINDLG_STOCKMODE_LTR	0
// ������ֻͣ����
#define WM_MAINDLG_STOCKMODE_TOP	1
// ��������ͣ��
#define WM_MAINDLG_STOCKMODE_NONE	2


// CLKSlidDialog �Ի���

class CLKSlidDialog : public CLKDialog
{
	DECLARE_DYNAMIC(CLKSlidDialog)

public:
	enum DialogStockModel{dsm_top, dsm_left, dsm_right, dsm_none};
	CLKSlidDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLKSlidDialog();

    // ��ȡͣ��״̬
    DialogStockModel GetDSMState(){return m_dsmState;}
protected:
	DECLARE_MESSAGE_MAP()
    void OpenMainPanel();
protected:
    // ��������
    bool                m_bCanHide;
    // Ƥ���ı�
    bool                m_bChangeSkin;
    // ������wm_print��Ϣ
    virtual void OnPrint(CDC *pDC);
    // ����������
    virtual void HideMainDlg();
    // ��ʾ
    virtual void SlidOutMainDlg();
    // ʵʱ�ı�ͣ������
    virtual void ChangeStockType(int &nStockType){}
private:
    // ͣ������
    int                 m_nStockType;
	// ͣ��״̬
	DialogStockModel	m_dsmState;
    // ����Ҫ���Ƶ�������ʾ��ͼƬ
	CLKImage		    *m_pAnimateImg;
public:
    afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnDestroy();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
