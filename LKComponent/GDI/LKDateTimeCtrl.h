/******************************************************************************
* ��Ȩ���� (C)2010, Sandongcun������
*
* �ļ����ƣ�CLKMonthCalCtrl.h
* ����ժҪ�����������ؼ���
* ����˵����
* ��ǰ�汾��v1.0
* ��    �ߣ�����
* ��ʼ���ڣ�2012.03.30  
* ������ڣ�-  
* ע�⣺
*   �������ӿؼ��༭���뿪�����ؼ���������ʱ�� ��ʱ���ؼ���WM_MOUSEMOVE����
*   �ӿؼ��༭���WM_MOUSELEAVE����.
******************************************************************************/
#pragma once
#include "LKCtrlBase.h"
#include "LKMonthCalCtrl.h"

// CLKDateTimeCtrl

class CLKDateTimeCtrl : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKDateTimeCtrl)

public:
	// �༭����Ϣ���غ���
	static LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	CLKDateTimeCtrl();
	virtual ~CLKDateTimeCtrl();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
public:
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
    // �Ƿ����༭��
    bool    m_bEnterEdit;
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
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
    // ��ǰ������ָ����CDC������
    virtual void OnDrawFGToDC(BGParam *prm);
	// ���������Ϣ
    virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
    // �༭��ؼ�
    CEdit   m_ctlEdit;
    // �����ؼ�
    CLKMonthCalCtrl     *m_pDate;
    // ǰ������
    CLKImage *m_pImgArr[2];
    // ״̬��0 - ��̬�� 1 - ����״̬��
    int     m_nState;
    // ��괦��ʲô֮��(0 - δ֪�� 1 - ��գ� 2 - ��������)
    int     m_nMouseWhere;
    // ��갴���ǵ�λ��(0 - δ֪�� 1 - ��գ� 2 - ��������)
    int     m_nMouseDown;
    // ��հ�ťλ��
    CRect   m_rtClear;
    // ����������ťλ��
    CRect   m_rtPop;
	// �༭��ԭʼ�ص�������ַ
	WNDPROC		m_pOldEditProc;
    // ���������ؼ�
    void PopMonthCalCtrl();
public:
    afx_msg void OnSetFocus(CWnd* pOldWnd);
};


