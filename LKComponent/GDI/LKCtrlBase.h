/******************************************************************************
* ��Ȩ���� (C)2010, Sandongcun������
*
* �ļ����ƣ�CLKCtrlBase.h
* ����ժҪ���������
* ����˵����
* ��ǰ�汾��v1.0
* ��    �ߣ�����
* ������ڣ�2011.09.25  
******************************************************************************/
#pragma once
#include "LKImage.h"

// �༭��ؼ���ʶ
#define LKEDIT_CHILDID_EDIT		1

class CLKCtrlBase;
typedef struct _CtrlDataBase
{
    CLKCtrlBase     *pWnd;
    _CtrlDataBase(){pWnd = 0;}
    //
    virtual ~_CtrlDataBase(){}
}CtrlDataBase, *PCtrlDataBase;

// CLKCtrlBase

class CLKCtrlBase : public CWnd
{
	DECLARE_DYNAMIC(CLKCtrlBase)

public:
	CLKCtrlBase(int nImageIndex = 1);
	virtual ~CLKCtrlBase();
	virtual BOOL Create(UINT nClassStyle, LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// ��ȡ����ͼƬ
	CLKImage *GetImageByArrIndex(int nIndex){if(nIndex >= 0 && nIndex < m_arrImage.GetCount()) return m_arrImage[nIndex]; return 0;}
	BOOL StretchBltPlus(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
	  HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop);
	// ��ȡ�ؼ��ڸ������е�λ��
	void GetRectInParent(CRect *rt);
	static void GetRectInParent(CWnd *pWnd, CRect *rt);
    // �����ַ����һ���ͨ���������
    static int FindStringIndexByPos(CString strText, int nOffsetPos, CFont *pFont, CDC *pDC);
    static int FindStringIndexByPos(CString strText, int nOffsetPos, LOGFONT *pLF, CDC *pDC);
    int FindStringIndexByPos(CString strText, int nOffsetPos, CFont *pFont);
    int FindStringIndexByPos(CString strText, int nOffsetPos, LOGFONT *pLF);
    // ����Ĭ�Ͻ�
    void CreateDefaultRoundWnd();
    // �ü��ͻ���
    void ClipClientRect(CRect rtOffset);
	// ��ȡ�ؼ��ڸ����ڿͻ����е�λ��
	void GetRectInParentClient(CRect *rt);
	// ��ȡ���ڸ߶�
	int GetHeight(){return m_rtClient.Height() + m_rtClientOffset.top + m_rtClientOffset.bottom;}
	// ��ȡ���ڿ��
	int GetWidth(){return m_rtClient.Width() + m_rtClientOffset.left + m_rtClientOffset.right;}
	// ���ñ߿�
	void SetClientOffset(CRect rtOffset){m_rtClientOffset = rtOffset;}
	// ��ȡ�߿�
	CRect GetClientOffset(){return m_rtClientOffset;}
	// �Ƿ��ȡ������(���߸�ǰ��)
	void SetGetParentBG(bool b = true){m_bGetParentBG = b;}
	// ������Ҫ������ͼƬ
	void SetParseImg(CLKImage *pImg);
    // ��ȡָ���ַ����Ŀ��
    static int GetStringWidth(CString &strText, CFont *pFont);
    // ��ȡָ���ַ����ĸ߶ȺͿ��
    static void GetStringWidthAndHeight(CString &strText, CFont *pFont, int &nWidth, int &nHeight, CDC *pDC);
    static void GetStringWidthAndHeight(CString &strText, LOGFONT *pLF, int &nWidth, int &nHeight, CDC *pDC);
    void GetStringWidthAndHeight(CString &strText, CFont *pFont, int &nWidth, int &nHeight);
    void GetStringWidthAndHeight(CString &strText, LOGFONT *pLF, int &nWidth, int &nHeight);
    // ���ù�������ָ��
    virtual void SetDataPtr(PCtrlDataBase pData){m_pData = pData;}
    // ��ȡ��������ָ��
    PCtrlDataBase GetDataPtr(){return m_pData;}
    // �������ݵ��ڴ�
    virtual void SaveDataToMem(CtrlDataBase *pData)
    {
        if(pData)
        {
            pData->pWnd = this;
        }
    }
    virtual void SaveDataToMem(){SaveDataToMem(m_pData);}
    // ���ڴ��м�������
    virtual void LoadDataFromMem(CtrlDataBase *pData)
    {
        if(IsWindowVisible())
        {
            Invalidate();
        }
    }
    virtual void LoadDataFromMem(){LoadDataFromMem(m_pData);}
	// ���±���
	void UpdateMemImage(){PostMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_RESETMEM);}
	// �����ı���ɫ
	void SetTextColor(COLORREF crText){m_crText = crText;}
	// �豸�ı�����
	void SetTextFont(CFont *pFont){ m_pFtText = pFont; }

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void SetEraseBG(bool bEraseBG){m_bEraseBG = bEraseBG;}
	// ��ȡ�Ƿ��ȡ������(���߸�ǰ��)
	bool GetGetParentBG(){return m_bGetParentBG;}
	// �õ�����ͼƬ����
	int GetImageIndex(){return m_nImageIndex;}
protected:
    // ����������ָ��
    PCtrlDataBase    m_pData;
	// �Ƿ��ȡ������(���߸�ǰ��)
	bool		m_bGetParentBG;
	// �Ƿ��������
	bool		m_bEraseBG;
	// ������ɫ
	COLORREF	m_crText;
	// �ı�����
	CFont		*m_pFtText = 0;
	// �ǿͻ��߿�
	CRect		m_rtClientOffset;
	// �ͻ�����
	CRect		m_rtClient;
	// ���ͼƬ��Դ
	ImagePointArray	m_arrImage;
	// ��ȡǰ��ͼƬ(�ڴ滺��)
	CLKImage *GetMemImg(){return m_pMemBmp;}
	// ����ǰ��ͼƬ(�ڴ滺��)
	void SetMemImg(CLKImage *pImg){m_pMemBmp = pImg;}
    // ��print����
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
    // ��ǰ������ָ����CDC������
    virtual void OnDrawFGToDC(BGParam *prm){}
	// ������
	virtual void DrawBG(CDC *pDC);
	// ���ǿͻ�����
	virtual void DrawNCArea(CDC *pDC);
	// ˢ��ǰ������
	virtual void ReSetMem(){}
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList(){}
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate(){}
	// �ڻ����ϻ�ͼ
	virtual void OnPaintEx(CDC *pDC, CDC *pMemDC){OnPaint(pMemDC);}
	virtual void OnPaint(CDC *pDC){}
	// ��ȡ��Ҫ������ͼƬ
	virtual CLKImage *GetParseImg();
	// ���������Ϣ
	virtual bool OnEraseBG(CDC *pDC);
	// û��ȡ�ñ���
	virtual void OnDrawNoParentBG(CDC *pDC){}
	// ��ȡ������ͼƬ
	virtual void GetParentImage(BGParam *prm);
    // ���Ҵ��ַ�����ʼƫ��λ�õ�����
    static int FindStringIndexByPos(CString strText, int nOffsetPos, CDC *pDC);
private:
	// Ҫ������ͼƬ����(����������б����ʼλ��)
	int			m_nImageIndex;
	// ����ͼƬ(���Ϊ����͸��)
	CLKImage	*m_pBGImg;
	// ǰ��ͼƬ(�ڴ滺��)
	CLKImage	*m_pMemBmp;
};


