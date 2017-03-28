#pragma once
#include "LKCtrlBaseEx.h"


// CLKScrollbar

class CLKScrollbar : public CLKCtrlBaseEx
{
	DECLARE_DYNAMIC(CLKScrollbar)

public:
	CLKScrollbar();
	virtual ~CLKScrollbar();
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID, bool bVisible = true);
	// ��ȡĬ�Ͽ��
	int GetDefaultWidth();
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
    //// ��print����
    //virtual int OnPrint(WPARAM wParam, LPARAM lParam);
	// û��ȡ�ñ���
	virtual void OnDrawNoParentBG(CDC *pDC);
	// ���������Ϣ
    virtual bool OnEraseBG(CDC *pDC){return 1;}
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
private:
	// ӵ����
	CWnd	*m_pOwner;
	WNDPROC	m_funOldProc;
	static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};


