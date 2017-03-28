#pragma once

#include "LKCtrlBase.h"

struct LKSCROLLINFO
{
	int nMax;
	int nPos;
	int nSliderSize;
    // ÿ���ж��ٸ�λ��
	int nPosPerPage;
	// �������ߴ�
	int nMax2;
	// ����λ��
	int nPos2;
};

// CLKCtrlBaseEx

class CLKCtrlBaseEx : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKCtrlBaseEx)

public:
	CLKCtrlBaseEx(int nImageIndex = 1);
	virtual ~CLKCtrlBaseEx();

protected:
	DECLARE_MESSAGE_MAP()

	// ���������Ϣ
    virtual bool OnEraseBG(CDC *pDC){return 1;}
	// �ڻ����ϻ�ͼ
	virtual void OnPaintEx(CDC *pDC, CDC *pMemDC);
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	// ���������λ��
	virtual void CalcScrollbarRect();

	// ���ù�����λ��
	void SetScrollbarRect(CRect rtVScrollbar, CRect rtHScrollbar){m_rtVScrollbar = rtVScrollbar;m_rtHScrollbar = rtHScrollbar;}
	void SetVScrollbarRect(CRect rtScrollbar){m_rtVScrollbar = rtScrollbar;}
	void SetHScrollbarRect(CRect rtScrollbar){m_rtHScrollbar = rtScrollbar;}
	// ��ȡ������λ��
	void GetVScrollbarRect(CRect &rtScrollbar){rtScrollbar = m_rtVScrollbar;}
	int GetVScrollbarWidth(){return m_rtVScrollbar.Width();}
	void GetHScrollbarRect(CRect &rtScrollbar){rtScrollbar = m_rtHScrollbar;}
	// �������Ƿ���Ч
	bool IsVScrollbarValid(){return m_bVScrollbarVisible && m_pVScrollbarImg && !m_rtVScrollbar.IsRectEmpty();}
	bool IsHScrollbarValid(){return m_bHScrollbarVisible && m_pHScrollbarImg && !m_rtHScrollbar.IsRectEmpty();}
	// ���ù�������겶���־
	void SetScrollbarCaptureFlag(bool bFlag = true){m_bCapture = bFlag;m_bWndCapture = bFlag;}
	// ��ȡ�����������϶�״̬
	bool GetScrollbarDraging(){return m_bScrollbarDraging;}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////// ��������� �����ڿͻ�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	// ���ù�����ͼƬ
	void SetScrollbarImg(CLKImage *pVScrollbarImg, CLKImage *pHScrollbarImg){m_pVScrollbarImg = pVScrollbarImg; m_pHScrollbarImg = pHScrollbarImg; CalcScrollbarRect();}
	// ���ô�ֱ�������Ƿ�ɼ�
	void SetVScrollbarVisible(bool bScrollbarVisible = true){m_bVScrollbarVisible = bScrollbarVisible;}
	// ����ˮƽ�������Ƿ�ɼ�
	void SetHScrollbarVisible(bool bScrollbarVisible = true){m_bHScrollbarVisible = bScrollbarVisible;}
	// ���ô�ֱ���������鷶Χ
	void SetVScrollbarRang(int nMax, int nMax2, bool bReDraw = false);//{m_siVScrollbar.nMax = nMax;m_siVScrollbar.nMax2 = nMax2;CalcVSliderSize();}
	void SetVScrollbarRang(int nMax);//{m_siVScrollbar.nMax = nMax;m_siVScrollbar.nMax2 = nMax;CalcVSliderSize();}
	// ����ˮƽ���������鷶Χ
	void SetHScrollbarRang(int nMax, int nMax2){m_siHScrollbar.nMax = nMax;m_siHScrollbar.nMax2 = nMax2;CalcHSliderSize();}
	void SetHScrollbarRang(int nMax){m_siHScrollbar.nMax = nMax;m_siHScrollbar.nMax2 = nMax;CalcHSliderSize();}
	// ���ô�ֱ����������λ��
	void SetVScrollbarSliderPos(int nPos = 0);
	// ����ˮƽ����������λ��
	void SetHScrollbarSliderPos(int nPos = 0);
	// ��ȡ��ֱ����������λ��
	int GetVScrollbarSliderPos(){return m_siVScrollbar.nPos;}
	// ��ȡˮƽ����������λ��
	//void GetHScrollbarSliderPos(int nPos = 0);
	// ���ָ���Ƿ��ڹ�������
	bool PtInScrollbar(CPoint &point);
private:
	// �����϶�������
	bool		m_bScrollbarDraging;
	// ����ڹ������ϰ���ʱ��λ��
	CPoint		m_ptOld;
	// �Ƿ񲶻����
	bool		m_bCapture;
	// �������Ѳ�������־
	bool		m_bWndCapture;
	// ��ֱ��������ǰͼƬ������
	int			m_nCurVScrollbarImgIndex;
	// ˮƽ��������ǰͼƬ������
	int			m_nCurHScrollbarImgIndex;
	// ��ֱ��������Ϣ
	LKSCROLLINFO	m_siVScrollbar;
	// ˮƽ��������Ϣ
	LKSCROLLINFO	m_siHScrollbar;
	// ��ֱ������ͼƬ
	CLKImage	*m_pVScrollbarImg;
	// ˮƽ������ͼƬ
	CLKImage	*m_pHScrollbarImg;
	// ��ֱ�������Ƿ�ɼ�
	bool		m_bVScrollbarVisible;
	// ˮƽ�������Ƿ�ɼ�
	bool		m_bHScrollbarVisible;
	// ��ֱ��������������
	CRect		m_rtVScrollbar;
	// ˮƽ��������������
	CRect		m_rtHScrollbar;
	// �����������ͻ���
	void DrawScrollbar(CDC *pDC);
	// ����ֱ������
	void DrawVScrollbar(CDC *pDC);
	// ��ˮƽ������
	void DrawHScrollbar(CDC *pDC);
	// ���»���
	void InvalidateSlider();
	// ���´�ֱ����������
	void InvalidateVSlider(bool bErase = false);
	// ����ˮƽ����������
	void InvalidateHSlider();
	// ����pos���㴹ֱ����������λ��pos2
	void CalcVSliderPos2();
	// ����pos2���㴹ֱ����������λ��pos
	void CalcVSliderPos();
	// ����pos����ˮƽ����������λ��pos2
	void CalcHSliderPos2();
	// ����pos2����ˮƽ����������λ��pos
	void CalcHSliderPos();
	// ���㴹ֱ�����������С
	void CalcVSliderSize();
	// ����ˮƽ�����������С
	void CalcHSliderSize();
	// ��ȡ��ֱ�������������±ߵ�λ��
	int GetVScrollbarMaxPos();
	// ��ȡˮƽ�������������ұߵ�λ��
	int GetHScrollbarMaxPos();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg BOOL OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


