#pragma once
#include "lkpicture.h"

class CLKPictureEx : public CLKPicture
{
public:
    CLKPictureEx(void);
    virtual ~CLKPictureEx(void);
    bool Create(CWnd *pWnd, CRect rt);
	virtual void UnLoad();
    // 播放
    void Play();
    // 显示一帧静态图片
    void Draw(int nFrameIndex = 0);
    // 停止播放
	void Stop(bool bInit = false);   // stops animation
    // 更新动画到指定DC上
    void UpdateDC(CDC *pDC);
    virtual void SetBkColor(COLORREF clr);
	// 获取显示区域
	CRect GetGIFRect(){return m_rtImage;}
protected:
	// loads a picture from a global memory block (allocated by GlobalAlloc)
	// Warning: this function DOES NOT free the global memory, pointed to by hGlobal
	virtual bool InnerLoad(HGLOBAL hGlobal, DWORD dwSize);
    // 计算图片对应的像素宽度
    virtual void CalcImageWidth(int hmWidth, int hmHeight);
private:
    // 父窗口
    CWnd    *m_pParent;
    // 在父窗口中的显示区域
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
	// 画第一帧
	void DrawFrame(int nFrameIndex);
    bool PrepareDC(int nWidth, int nHeight);
	void ThreadAnimation();
	static UINT WINAPI _ThreadAnimation(LPVOID pParam);
////////////////////////////////////////////////////////////////////////////
public:
    // 更新动画到指定DC,指定区域上
    void UpdateDC(CWnd *pWnd, CDC *pDC, const CRect &rt, int nFrameIndex);
	// 画第一帧
	void DrawFrameToDC(CWnd *pWnd, CDC *pDC, const CRect &rt, int nFrameIndex);
	// 是否可以刷新
	bool IsCanRedraw(int nTFrameIndex, DWORD nPreFrameTime);
	// 获取下一帧图片
	int GetNextFrameIndex(int nTFrameIndex);
	//// 将下一帧图片作为当前图片
	//int NextFrameIndex();
private:
	// 最后一次刷新控件的时间(计算机启动时间算起)
	//DWORD m_nTimer;
	// 最后一次刷新控件的帧索引
	//int m_nTFrameIndex;
};

// ID和对象指针结构
struct PictureAndID
{
	// id
	int nId;
	CLKPictureEx *pImage;
};

