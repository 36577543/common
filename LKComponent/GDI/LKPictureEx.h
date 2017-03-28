#pragma once
#include "lkpicture.h"

class CLKPictureEx : public CLKPicture
{
public:
    CLKPictureEx(void);
    virtual ~CLKPictureEx(void);
    bool Create(CWnd *pWnd, CRect rt);
	virtual void UnLoad();
    // ����
    void Play();
    // ��ʾһ֡��̬ͼƬ
    void Draw(int nFrameIndex = 0);
    // ֹͣ����
	void Stop(bool bInit = false);   // stops animation
    // ���¶�����ָ��DC��
    void UpdateDC(CDC *pDC);
    virtual void SetBkColor(COLORREF clr);
	// ��ȡ��ʾ����
	CRect GetGIFRect(){return m_rtImage;}
protected:
	// loads a picture from a global memory block (allocated by GlobalAlloc)
	// Warning: this function DOES NOT free the global memory, pointed to by hGlobal
	virtual bool InnerLoad(HGLOBAL hGlobal, DWORD dwSize);
    // ����ͼƬ��Ӧ�����ؿ��
    virtual void CalcImageWidth(int hmWidth, int hmHeight);
private:
    // ������
    CWnd    *m_pParent;
    // �ڸ������е���ʾ����
    CRect   m_rtImage;

	volatile BOOL m_bExitThread;
	HANDLE m_hThread;
	HANDLE m_hExitEvent;

	HDC m_hMemDC;
	HDC m_hDispMemDC;
	HBITMAP m_hDispMemBM;
	HBITMAP m_hDispOldBM;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	// ����һ֡
	void DrawFrame(int nFrameIndex);
    bool PrepareDC(int nWidth, int nHeight);
	void ThreadAnimation();
	static UINT WINAPI _ThreadAnimation(LPVOID pParam);
////////////////////////////////////////////////////////////////////////////
public:
    // ���¶�����ָ��DC,ָ��������
    void UpdateDC(CWnd *pWnd, CDC *pDC, const CRect &rt, int nFrameIndex);
	// ����һ֡
	void DrawFrameToDC(CWnd *pWnd, CDC *pDC, const CRect &rt, int nFrameIndex);
	// �Ƿ����ˢ��
	bool IsCanRedraw(int nTFrameIndex, DWORD nPreFrameTime);
	// ��ȡ��һ֡ͼƬ
	int GetNextFrameIndex(int nTFrameIndex);
	//// ����һ֡ͼƬ��Ϊ��ǰͼƬ
	//int NextFrameIndex();
private:
	// ���һ��ˢ�¿ؼ���ʱ��(���������ʱ������)
	//DWORD m_nTimer;
	// ���һ��ˢ�¿ؼ���֡����
	//int m_nTFrameIndex;
};

// ID�Ͷ���ָ��ṹ
struct PictureAndID
{
	// id
	int nId;
	CLKPictureEx *pImage;
};

