#pragma once
#include "LKSlidDialog.h"


// CLKMainDialog

class CLKMainDialog : public CLKSlidDialog
{
	DECLARE_DYNAMIC(CLKMainDialog)

public:
	CLKMainDialog(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CLKMainDialog();

	// ��ȡӦ�ó���ָ������ͼ������
	BOOL FindOutPositionOfIconDirectly(CRect& a_rcIcon);
    // ��ȡ���̲���ָ��
    NOTIFYICONDATA *GetTrayParam(){return &m_ndTray;}
    // �رճ���
    virtual void Close(){}
    // ��������״̬
    void UpdatTrayState();
protected:
	DECLARE_MESSAGE_MAP()

	// ��ʼ����������
	virtual void InitBGBmp();
    virtual void OnTrayLButtonDown(){}
    virtual void OnTrayRButtonDown(){}
    virtual void OnTrayRButtonUp(){}
    virtual void OnTrayLDBClick(){}
    // ��С����ť������
    virtual void OnMinClick();

////////////�������/////////////////////////////////////////////
protected:
    // ����ͼ��
    HICON               m_hTrayIcon;
    HICON               m_hTrayGrayIcon;
	// ��ǰ����ͼ����
	HICON				m_hCurTrayIcon;
    // ������Ϣ�Ƿ���Ҫ�ַ�����ģ�鴦������true�������ڲ�����Ӧ����Ϣ��
    virtual bool OnNotifyTrayOtherModule(WPARAM wParam, LPARAM lParam){return false;}
	// ��ʼ������
	void InitTray();
private:
	//����
	NOTIFYICONDATA		m_ndTray;
	// ��ȡ��������
	static CRect GetTrayWndRect();
	// ��ȡ����������
	static HWND GetTrayNotifyWnd();
	// �ص����� ������֪ͨ�����ҳ�����
	static BOOL CALLBACK FindToolBarInTrayWnd(HWND hwnd, LPARAM lParam);
	// �ص����������������ҳ�����֪ͨ����
	static BOOL CALLBACK FindTrayWnd(HWND hwnd, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	// ��������ͼ����Ϣ�¼�����
	LRESULT OnNotifyTray(WPARAM wParam, LPARAM lParam);
};


