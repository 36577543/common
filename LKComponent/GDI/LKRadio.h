#pragma once
#include "LKCommonCtrl.h"


// CLKRadio

class CLKRadio : public CLKCommonCtrl
{
	DECLARE_DYNAMIC(CLKRadio)

public:
	CLKRadio(int nImageIndex = 1);
	virtual ~CLKRadio();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, bool bIsGroup = false);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID, bool bIsGroup = false);
    bool GetSelected(){return m_bSelected;}
    void SetSelected(bool bSelected = true);
    // ��ǰѡȡ�е�����
    int GetCurIndex();
    // ���õ�ǰѡȡ�е�����
    void SetCurIndex(int nIndex);
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
};


