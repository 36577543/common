#pragma once

#include "LKCommonCtrl.h"
// CLKButton

// Ĭ�ϰ�ť���
#define DEFAULT_BUTTONWIDTH     64
// Ĭ�ϰ�ť�߶�
#define DEFAULT_BUTTONHEIGHT    22


class CLKButton : public CLKCommonCtrl
{
	DECLARE_DYNAMIC(CLKButton)

public:
	CLKButton(int nImageIndex = 1);
	virtual ~CLKButton();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateHide(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateHide(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// ����ͼ���ȣ��߶�
	void SetIconWidthAndHeight(int nW, int nH){ m_nIconWidth = nW; m_nIconHeight = nH; }
	void SetImage(int nImageIndex);
	void SetParseImage(int nImageIndex);
	void SetLeftMargin(int nLeftMargin){m_nLeftMargin = nLeftMargin;}
	// ��갴��ʱͼ�����ı���ƫ����
	void SetHighLightOffset(int nHighLinghtOffset = 1){m_nHighLinghtOffset = nHighLinghtOffset;}
	//// �����ı���ɫ
	//void SetTextColor(COLORREF crText);//{m_crText = crText;}
	//// �豸�ı�����
	//void SetTextFont(CFont *pFont){ m_pFtText = pFont; }
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
	// ���������Ϣ
	virtual bool OnEraseBG(CDC *pDC);
	// ��ȡ��Ҫ������ͼƬ
	virtual CLKImage *GetParseImg();
private:
	// ͼ����
	int m_nIconWidth = 16; 
	// ͼ��߶�
	int m_nIconHeight = 16;
	CLKImage	*m_pImage;
	CLKImage	*m_pBGImage;
	int			m_nLeftMargin;
	int			m_nIconTextSplit;
	// ��갴��ʱͼ�����ı���ƫ����
	int			m_nHighLinghtOffset;
	//COLORREF	m_crText;
	//CFont		*m_pFtText;
};


