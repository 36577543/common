#pragma once
#include "LKCommonCtrl.h"


// CLKCheckBox

class CLKCheckBox : public CLKCommonCtrl
{
	DECLARE_DYNAMIC(CLKCheckBox)

public:
	CLKCheckBox(int nImageIndex = 1);
	virtual ~CLKCheckBox();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
    // ����ѡ��״̬
    void SetSelected(bool bSelected = true, bool bErase = true);
    // ��ȡѡ��״̬
    bool GetSelected(){return m_bSelected;}
	// �����ı���ɫ
	void SetTextColor(COLORREF crText){m_crText = crText;}

protected:
	DECLARE_MESSAGE_MAP()
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
	// ��ʼ�����������ϢLPARAM����
	virtual void InitLClick();
private:
	// ��߾�
	int		m_nLeftMargin;
	// ͼ����ո�֮��ľ���
	int		m_nIconTextSplit;
	// �Ƿ�ѡ��
	bool	m_bSelected;
	COLORREF	m_crText;
};


