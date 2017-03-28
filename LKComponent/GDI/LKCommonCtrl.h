#pragma once
#include "LKCtrlBase.h"


// CLKCommonCtrl

typedef enum mousestate
{
	esb_nohere,
	esb_hover,
	esb_down,
	esb_drag
}EMState;

class CLKCommonCtrl : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKCommonCtrl)

public:
	CLKCommonCtrl(int nImageIndex = 1);
	virtual ~CLKCommonCtrl();
	// ������̬����ǰ��(�Ǵ���ģʽ - ����ģʽ)
	void SetFourStateMemImg(CLKImage *pNormalImg, CLKImage *pHighLightImg, CLKImage *pOperatorImg, CLKImage *pDisableImg);
	// ��ȡ��̬����ǰ��
	void GetFourStateMemImg(CLKImage *&pNormalImg, CLKImage *&pHighLightImg, CLKImage *&pOperatorImg, CLKImage *&pDisableImg){pNormalImg = m_pNormalImg; pHighLightImg = m_pHighLightImg; pOperatorImg = m_pOperatorImg; pDisableImg = m_pDisableImg;}
	// ��ȡ��������ǰ��
	CLKImage *GetNormalMemImg(){return m_pNormalImg;}
	// ��ȡ��������ǰ��
	CLKImage *GetHighLightMemImg(){return m_pHighLightImg;}
	// ��ȡ��������ǰ��
	CLKImage *GetOperatorMemImg(){return m_pOperatorImg;}
	// ��ȡ��Ч����ǰ��
	CLKImage *GetDisableImg(){return m_pDisableImg;}
protected:
	DECLARE_MESSAGE_MAP()
protected:
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ��ʼ�����������ϢLPARAM����
	virtual void InitLClick(){}
	// ������
	virtual void OnMouseEnter(CPoint point){}
	// ����normalͼƬ����
	void CreateNormalImg(int nIndex, BGParam &prm, CLKImage *pImage = 0);
	// ��������ͼƬ����
	void CreateHighLightImg(int nIndex, BGParam &prm, CLKImage *pImage = 0);
	// ��������ͼƬ����
	void CreateOperatorImg(int nIndex, BGParam &prm, CLKImage *pImage = 0);
protected:
	// ���״̬
	EMState		m_emsCommon;
	// ��̬���ڴ滺���Ƿ����Լ�������
	bool		m_bImgIsOwner;
	// ǰ����̬ͼƬ(�ڴ滺��)
	CLKImage	*m_pNormalImg;
	// ǰ������ͼƬ(�ڴ滺��)
	CLKImage	*m_pHighLightImg;
	// ǰ������ͼƬ(�ڴ滺��)
	CLKImage	*m_pOperatorImg;
	// ǰ����ֹͼƬ(�ڴ滺��)
	CLKImage	*m_pDisableImg;
public:
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	// �ͷ���̬����ǰ��
	void FreeFourStateMemImg();
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


