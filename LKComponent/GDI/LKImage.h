#pragma once
#ifndef _LKIMAGE_H
#define _LKIMAGE_H

#include "GDICommon.h"
#include "atlimage.h"

// ��Ļ���
#define SCREEN_WIDTH (GetSystemMetrics(SM_CXSCREEN) + 4)
#define SCREEN_HEIGHT (GetSystemMetrics(SM_CYSCREEN) + 4)
#define CURSCREEN_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define CURSCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)


// ��ͼ��ʽ
// �����Ͻ�
#define LKSKINBASE_DRAWBGSTYLE_LT			0x1
// ����߿�
#define LKSKINBASE_DRAWBGSTYLE_LM			0x2
// ���ϱ߿�
#define LKSKINBASE_DRAWBGSTYLE_TM			0x4
// �����Ͻ�
#define LKSKINBASE_DRAWBGSTYLE_RT			0x8
// ���ұ߿�
#define LKSKINBASE_DRAWBGSTYLE_RM			0x10
// �����½�
#define LKSKINBASE_DRAWBGSTYLE_LB			0x20
// ���±߿�
#define LKSKINBASE_DRAWBGSTYLE_BM			0x40
// �����½�
#define LKSKINBASE_DRAWBGSTYLE_RB			0x80
// ���м�
#define LKSKINBASE_DRAWBGSTYLE_MM			0x100
// ����(������)
#define LKSKINBASE_DRAWBGSTYLE_COPY			0x200


// ������
#define LKSKINBASE_DRAWBGSTYLE_ALL		LKSKINBASE_DRAWBGSTYLE_LT | LKSKINBASE_DRAWBGSTYLE_LM | \
LKSKINBASE_DRAWBGSTYLE_TM | LKSKINBASE_DRAWBGSTYLE_RT | LKSKINBASE_DRAWBGSTYLE_RM | \
LKSKINBASE_DRAWBGSTYLE_LB | LKSKINBASE_DRAWBGSTYLE_BM | LKSKINBASE_DRAWBGSTYLE_RB | LKSKINBASE_DRAWBGSTYLE_MM
// ���߿�
#define LKSKINBASE_DRAWBGSTYLE_FRAME		LKSKINBASE_DRAWBGSTYLE_LT | LKSKINBASE_DRAWBGSTYLE_LM | \
LKSKINBASE_DRAWBGSTYLE_TM | LKSKINBASE_DRAWBGSTYLE_RT | LKSKINBASE_DRAWBGSTYLE_RM | \
LKSKINBASE_DRAWBGSTYLE_LB | LKSKINBASE_DRAWBGSTYLE_BM | LKSKINBASE_DRAWBGSTYLE_RB
// ������
#define LKSKINBASE_DRAWBGSTYLE_LEFTTOP		LKSKINBASE_DRAWBGSTYLE_LT | LKSKINBASE_DRAWBGSTYLE_LM | LKSKINBASE_DRAWBGSTYLE_TM
// ������
#define LKSKINBASE_DRAWBGSTYLE_RIGHTBOTTOM	LKSKINBASE_DRAWBGSTYLE_RT | LKSKINBASE_DRAWBGSTYLE_RM | \
LKSKINBASE_DRAWBGSTYLE_LB | LKSKINBASE_DRAWBGSTYLE_BM | LKSKINBASE_DRAWBGSTYLE_RB
// ������
#define LKSKINBASE_DRAWBGSTYLE_TOPMIDBOTTOM	LKSKINBASE_DRAWBGSTYLE_TM | LKSKINBASE_DRAWBGSTYLE_MM | LKSKINBASE_DRAWBGSTYLE_BM
// ������
#define LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT	LKSKINBASE_DRAWBGSTYLE_LM | LKSKINBASE_DRAWBGSTYLE_MM | LKSKINBASE_DRAWBGSTYLE_RM

#pragma pack(1)
typedef struct
{
	BYTE bWidth;		// Width, in pixels, of the image 
	BYTE bHeight;		// Height, in pixels, of the image 
	BYTE bColorCount;	// Number of colors in image (0 if >=8bpp) 
	BYTE bReserved;		// Reserved 
	WORD wPlanes;		// Color Planes 
	WORD wBitCount;		// Bits per pixel 
	DWORD dwBytesInRes; // how many bytes in this resource? 
	WORD nID;			// the ID 
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;

typedef struct
{ 
	WORD idReserved;				// Reserved (must be 0) 
	WORD idType;					// Resource type (1 for icons) 
	WORD idCount;					// How many images? 
	GRPICONDIRENTRY idEntries[1];	// The entries for each image 
} GRPICONDIR, *LPGRPICONDIR;
#pragma pack()
// CreateIconFromResourceEx


// 4ͨ����ɫ����
typedef struct _LKCOLOR
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	//_LKCOLOR(){a = 0; r = 0; g = 0; b = 0;}
}LKCOLOR;

typedef union _LKCOLOREX
{
	COLORREF	cor;
	LKCOLOR		cr;
}LKCOLOREX;

// ��ƫ����
typedef struct _AngleOffset
{
	// ���Ͻǿ�
	unsigned char nLTWidth;
	// ���ϽǸ�
	unsigned char nLTHeight;
	// ���Ͻǿ�
	unsigned char nRTWidth;
	// ���ϽǸ�
	unsigned char nRTHeight;
	// ���½ǿ�
	unsigned char nLBWidth;
	// ���½Ǹ�
	unsigned char nLBHeight;
	// ���½ǿ�
	unsigned char nRBWidth;
	// ���½Ǹ�
	unsigned char nRBHeight;
	_AngleOffset(int nLTx = 0, int nLTy = 0, int nRTx = 0, int nRTy = 0, int nLBx = 0, int nLBy = 0, int nRBx = 0, int nRBy = 0)
	{
		// ���Ͻǿ�
		nLTWidth	= nLTx;
		// ���ϽǸ�
		nLTHeight	= nLTy;
		// ���Ͻǿ�
		nRTWidth	= nRTx;
		// ���ϽǸ�
		nRTHeight	= nRTy;
		// ���½ǿ�
		nLBWidth	= nLBx;
		// ���½Ǹ�
		nLBHeight	= nLBy;
		// ���½ǿ�
		nRBWidth	= nRBx;
		// ���½Ǹ�
		nRBHeight	= nRBy;
	}
}AngleOffset;

class CLKImage : public CImage
{
public:
	CLKImage(void){m_nImgNumber = 1;m_rtOffset = CRect(0, 0, 0, 0);}
	CLKImage(int nWidth, int nHeight, int nBPP = 24)
	{
		m_nImgNumber = 1;
		m_rtOffset = CRect(0, 0, 0, 0);
		if(!Create(nWidth, nHeight, nBPP))
		{
			CString strImage;
			strImage.Format(_T("�޷�����ͼƬ���� ����%d  �ߣ� %d"), nWidth , nHeight);
			::AfxMessageBox(strImage);
		}
	}
	CLKImage(int nWidth, int nHeight, int nStartAlpha, int nFinishAlpha)
    {
		m_nImgNumber = 1;
		m_rtOffset = CRect(0, 0, 0, 0);
		if(!Create(nWidth, nHeight, 32))
		{
			CString strImage;
			strImage.Format(_T("�޷�����ͼƬ���� ����%d  �ߣ� %d"), nWidth , nHeight);
			::AfxMessageBox(strImage);
		}

        LKCOLOREX cor;
	    cor.cor = (RGB(255, 255, 255));
        double n = (double)(nFinishAlpha - nStartAlpha) / (nHeight > 1 ? nHeight - 1 : 1);
	    for(int i = 0; i < nHeight; i++)
	    {
		    unsigned char* pucColor = reinterpret_cast<unsigned char *>(GetPixelAddress(0, i));
		    pucColor[0] = 255;
		    pucColor[1] = 255;
		    pucColor[2] = 255;
		    pucColor[3] = nStartAlpha + (int)(n * i);
	    }
	    // ����͸��ͨ��
        SetAlphaChannel();
    }
	~CLKImage(void){}
	bool CreateImage(HICON hIcon)
	{
		if (!hIcon){ return false; }
		m_nImgNumber = 1;
		m_rtOffset = CRect(0, 0, 0, 0);
		ICONINFO info;
		::GetIconInfo(hIcon, &info);
		BITMAP bmp;
		::GetObject(info.hbmColor, sizeof(BITMAP), &bmp);
		if (bmp.bmType == 0)
		{
			return false;
		}
		if (!Create(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel))
		{
			CString strImage;
			strImage.Format(_T("�޷�����ͼƬ���� ����%d  �ߣ� %d"), bmp.bmWidth, bmp.bmHeight);
			::AfxMessageBox(strImage);
			return false;
		}
		int nSize = bmp.bmBitsPixel * bmp.bmWidth * bmp.bmHeight / 8;
		BYTE *pBmpBits = new BYTE[nSize];
		BYTE *p = pBmpBits;
		::GetBitmapBits(info.hbmColor, nSize, p);
		for (int y = 0; y < bmp.bmHeight; y++)
		{
			for (int x = 0; x < bmp.bmWidth; x++)
			{
				unsigned char* pucColorx = (BYTE *)GetPixelAddress(x, y);
				memcpy(pucColorx, p, bmp.bmBitsPixel / 8);
				p += bmp.bmBitsPixel / 8;
			}
		}
		// ����͸��ͨ��
		SetAlphaChannel();
		delete []pBmpBits;
		return true;
	}
	// ����͸��ͨ��
    void SetAlphaChannel();
public:
	// ��ȡƫ����
	void GetOffsetRect(CRect &rtOffset){ rtOffset = m_rtOffset; }
	// ת��ͼ�굽λͼ
	static HBITMAP ConvertIconToBitmap(HICON  hIcon);
    // ������ɫͼ��
    static HICON CreateGrayIcon(HICON hSrc);
    // ����ͼ��
    HICON CreateIconByImg();
    // ��ͼƬ�ϻ��ı�
	static void DrawText(CDC &memDC, CString strText, CRect rtText, UINT uFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE, COLORREF crText = RGB(43, 85, 128));
	// ��ͼƬ�ϻ��ı�
	bool DrawText(CString strText, CRect rtText, CFont *pFont, UINT uFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE, COLORREF crText = RGB(43, 85, 128));
	// ��ͼƬ�Ͻ���ARGB����
	static void CalcPARGB(CLKImage *pImg){pImg->CalcPARGB();}
    // ���豸�������е�ͼƬ���Ƶ�������
    bool DrawFromDC(CDC *pDC, CRect rtSrc, CPoint ptDest = CPoint(0, 0));
	int GetImageNumber(){return m_nImgNumber;}
	void SetImageNumber(int nImgNumber){m_nImgNumber = nImgNumber;}
	void SetFrameRect(CRect &rtFrame){m_rtOffset = rtFrame;}
	CRect GetFrameRect(){return m_rtOffset;}
	// ��ͼƬ�Դ��Ŀ�ܳߴ续��ָ��ͼƬ��
	bool DrawToImageFrame(CLKImage *pDestImg);
	bool DrawToImageFrame(CLKImage *pDestImg, CRect rtDest);
	bool DrawToImageLR(CLKImage *pDestImg, CRect rtDest);
	// ��ָ����ܳߴ续��ָ��ͼƬ��
	bool DrawToImageFrame(CLKImage *pDestImg, CRect rtOffset, AngleOffset angleOffset);
	bool DrawToImage(CLKImage *pImg);
	void DrawToImage(CLKImage *pDestImg, int nIndex);
	// ��������ģʽ
	void StretchToImage(CLKImage *pImg);
	// ��ͨ����ģʽ(��������)
	void DrawToImage(CLKImage *pDestImg, CRect rtDest);
	// ��ͨ����ģʽ(��������)
	void DrawToImage(CLKImage *pDestImg, int nIndex, CRect rtDest);
	// ��ͨ����ģʽ(��������)
	void DrawToImage(CLKImage *pDestImg, CRect rtDest, CRect rtSrc);
	// ��ͨ����ģʽ(��͸����)
	void DrawToImage(CLKImage *pDestImg, CRect rtDest, int nAlpha);
	// ��ͨ���췽ʽ(������)
	void DrawToImage(CLKImage *pDestImg, int nIndex, PBGParam pParam);
	// ��ͨ���췽ʽ(������)
	void DrawToImage(CLKImage *pDestImg, CRect rtDest, PBGParam pParam);
	bool DrawToImage(CLKImage *pDestImg, int nIndex, PBGParam pParam, UINT uDrawStyle);
	bool DrawToImage(CLKImage *pDestImg, PBGParam pParam, UINT uDrawStyle, AngleOffset angleOffset);
	bool DrawToImage(CLKImage *pDestImg, CRect rtOffset, int nIndex, PBGParam pParam, UINT uDrawStyle);
	bool DrawToImage(CLKImage *pDestImg, CRect rtDest, CRect rtOffset, int nIndex, PBGParam pParam, UINT uDrawStyle, AngleOffset angleOffset = AngleOffset(0));
    // ˫���Բ�ֵ����
    void StretchBilinear(CLKImage* pDest, int cxDest, int cyDest, int cxSrc, int cySrc);
	// pImage ���ڷǼ�������
	bool DrawToImage(CLKImage *pDestImg, CLKImage *pImage, int nIndex, PBGParam pParam, UINT uDrawStyle);
	bool DrawToImage(CLKImage *pDestImg, CLKImage *pImage, CRect rtDest, CRect rtOffset, int nIndex, PBGParam pParam, UINT uDrawStyle, AngleOffset angleOffset = AngleOffset(0));

	bool IsValid(){return !IsNull();}
	bool LoadEx(CString strFileName, bool bCalcARGB = true);
	bool LoadEx(HRSRC hResource, HMODULE hInst = NULL, bool bCalcARGB = true);
	bool LoadEx(UINT nResourceId, bool bCalcARGB, CRect &rtFrame, CString strType, HMODULE hInst = NULL){ return LoadEx(MAKEINTRESOURCE(nResourceId), bCalcARGB, rtFrame, strType, hInst); }
	bool LoadEx(UINT nResourceId, bool bCalcARGB, CRect &rtFrame, UINT nType, HMODULE hInst = NULL){ return LoadEx(MAKEINTRESOURCE(nResourceId), bCalcARGB, rtFrame, MAKEINTRESOURCE(nType), hInst); }
	bool LoadMemImage(void *pMemData, long len);
	//bool LoadPNG(UINT nResourceId, bool bCalcARGB, CRect &rtFrame, HMODULE hInst = NULL){ return LoadEx(MAKEINTRESOURCE(nResourceId), bCalcARGB, rtFrame, MAKEINTRESOURCE(_T("PNG")), hInst); }
	//bool LoadJPG(UINT nResourceId, bool bCalcARGB, CRect &rtFrame, HMODULE hInst = NULL){ return LoadEx(MAKEINTRESOURCE(nResourceId), bCalcARGB, rtFrame, MAKEINTRESOURCE(_T("JPG")), hInst); }
	void CalcPARGB();
public:
	// ��ͨ����ģʽ
	bool DrawToDC(CDC *pDC, CRect rtDest, int nIndex);
	// ��ͨ������ģʽ
	bool DrawToDC(CDC *pDC, int nLeft, int nTop, int nIndex);
	// ʹ��ͼƬ�Դ�ƫ����
	bool DrawToDC(CDC *pDC, CRect rtDest, int nIndex, UINT uDrawStyle, PBGParam pParam = 0, AngleOffset angleOffset = AngleOffset(0));
	bool DrawToDC(CDC *pDC, CRect rtDest, CRect rtOffset, int nIndex, UINT uDrawStyle, PBGParam pParam = 0, AngleOffset angleOffset = AngleOffset(0));
	bool DrawFrame(CDC *pDC, CRect rtDest, CRect rtOffset, int nOffset, UINT uDrawStyle, AngleOffset angleOffset = AngleOffset(0));
	bool DrawLRToDC(CDC *pDC, CRect rtDest, int nOffsetL, int nOffsetR, int nOffset);
	// ֱ���������ȱ�ݣ�ѭ�����ƴ�������
	bool DrawLRToDCEx(CDC *pDC, CRect rtDest, int nOffsetL, int nOffsetR, int nOffset);
private:
	// ���ͼƬ�ĸ���
	int m_nImgNumber;
	// ͼƬ�߿���
	CRect m_rtOffset;
	bool LoadEx(LPCTSTR pName, bool bCalcARGB, CRect &rtFrame, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
};

// ����͸��ͨ��
inline void CLKImage::SetAlphaChannel()
{
	unsigned char *p = (unsigned char *)this; 
	p += 28 + sizeof(bool);
	int *n = (int *)p;
	bool *b = (bool *)p;
	*b = true;
	CalcPARGB();
}

// ת��ͼ�굽λͼ
inline HBITMAP CLKImage::ConvertIconToBitmap(HICON  hIcon)
{
	HBITMAP   hBmp;
	BITMAP   bmp;
	CDC   bmpDC;
	CDC   iconDC;
	ICONINFO         csII;
	int bRetValue = ::GetIconInfo(hIcon, &csII);
	if (bRetValue == FALSE)   return   NULL;
	bmpDC.Attach(::GetDC(NULL));
	iconDC.CreateCompatibleDC(&bmpDC);
	if (::GetObject(csII.hbmColor, sizeof(BITMAP), &bmp))
	{
		DWORD       dwWidth = csII.xHotspot * 2;
		DWORD       dwHeight = csII.yHotspot * 2;
		hBmp = ::CreateBitmap(dwWidth, dwHeight, bmp.bmPlanes,
			bmp.bmBitsPixel, NULL);
		iconDC.SelectObject(csII.hbmColor);
		bmpDC.SelectObject(hBmp);
		bmpDC.BitBlt(0, 0, dwWidth, dwHeight, &iconDC, 0, 0, SRCCOPY);
		return   hBmp;
	}
	return NULL;
}

// ������ɫͼ��
inline HICON CLKImage::CreateGrayIcon(HICON hSrc)
{
    ICONINFO info;
    ::GetIconInfo(hSrc, &info);
    BITMAP bmp;
    ::GetObject(info.hbmColor, sizeof(BITMAP), &bmp);
    int nSize = bmp.bmBitsPixel * bmp.bmWidth * bmp.bmHeight / 8;
    BYTE *pBmpBits = new BYTE[nSize]; 
    BYTE *p = pBmpBits;
    ::GetBitmapBits(info.hbmColor, nSize, p);
    
    for(int y = 0; y < bmp.bmHeight; y++)
    {
        for(int x = 0; x < bmp.bmWidth; x++)
        {
            *p = (BYTE)(*p * 0.299 + *(p + 1) * 0.587 + *(p + 2) * 0.114);
            *(p + 1) = *p;
            *(p + 2) = *p;
            p += bmp.bmBitsPixel / 8;
        }
    }
    ::SetBitmapBits(info.hbmColor, nSize, pBmpBits);
	//::DeleteObject(info.hbmColor);
	//::DeleteObject(info.hbmMask);
    delete pBmpBits;
    return ::CreateIconIndirect(&info);
}

// ����ͼ��
inline HICON CLKImage::CreateIconByImg()
{
    ICONINFO info;
    info.fIcon = 1;
    info.xHotspot = 8;
    info.yHotspot = 8;

    BITMAP bmp;
    bmp.bmBits = 0;
    bmp.bmBitsPixel = 1;
    bmp.bmHeight = 16;
    bmp.bmPlanes = 1;
    bmp.bmType = 0;
    bmp.bmWidth = 16;
    bmp.bmWidthBytes = 2;
    info.hbmMask = ::CreateBitmapIndirect(&bmp);

    bmp.bmBits = 0;
    bmp.bmBitsPixel = 32;
    bmp.bmHeight = 16;
    bmp.bmPlanes = 1;
    bmp.bmType = 0;
    bmp.bmWidth = 16;
    bmp.bmWidthBytes = 64;
    info.hbmColor = ::CreateBitmapIndirect(&bmp);
    int nSize = bmp.bmBitsPixel * bmp.bmWidth * bmp.bmHeight / 8;
    BYTE *pBmpBits = new BYTE[nSize]; 
    BYTE *p = pBmpBits;
    ::GetBitmapBits(info.hbmColor, nSize, p);
    BYTE *pSrc;
    //COLORREF col; 
    for(int y = 0; y < bmp.bmHeight; y++)
    {
        for(int x = 0; x < bmp.bmWidth; x++)
        {
            pSrc = (BYTE *)GetPixelAddress(x, y);
            *p = *(pSrc + 0);
            *(p + 1) = *(pSrc + 1);
            *(p + 2) = *(pSrc + 2);
            *(p + 3) = *(pSrc + 3);
            p += bmp.bmBitsPixel / 8;
        }
    }
    ::SetBitmapBits(info.hbmColor, nSize, pBmpBits);
    delete pBmpBits;

    HICON hIcon = ::CreateIconIndirect(&info);
    ::DeleteObject(info.hbmMask);
    ::DeleteObject(info.hbmColor);
    return hIcon;
}	

// ��ͼƬ�ϻ��ı�
inline void CLKImage::DrawText(CDC &memDC, CString strText, CRect rtText, UINT uFormat, COLORREF crText)
{
	// Ĭ������
	//COLORREF crText(RGB(43, 85, 128));
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(crText);
	memDC.DrawText(strText, rtText, uFormat);
}

// ��ͼƬ�ϻ��ı�
inline bool CLKImage::DrawText(CString strText, CRect rtText, CFont *pFont, UINT uFormat, COLORREF crText)
{
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*this);
			CBitmap *pOld = memDC.SelectObject(pBmp);
	        CFont *pOldFont = memDC.SelectObject(pFont);
			DrawText(memDC, strText, rtText, uFormat, crText);
	        memDC.SelectObject(pOldFont);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
			return true;
		}
	}
	return false;
}

// ���豸�������е�ͼƬ���Ƶ�������
inline bool CLKImage::DrawFromDC(CDC *pDC, CRect rtSrc, CPoint ptDest)
{
	if(pDC)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap *pBmp = CBitmap::FromHandle(*this);
		CBitmap *pOld = memDC.SelectObject(pBmp);
        memDC.BitBlt(ptDest.x, ptDest.y, rtSrc.Width(), rtSrc.Height(), pDC, rtSrc.left, rtSrc.top, SRCCOPY);
		memDC.SelectObject(pOld);
		memDC.DeleteDC();
		return true;
	}
    return false;
}

// ��ͨ����ģʽ(��͸����)
inline void CLKImage::DrawToImage(CLKImage *pDestImg, CRect rtDest, int nAlpha)
{
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			AlphaBlend(memDC.GetSafeHdc(), rtDest.left, rtDest.top, rtDest.Width(), rtDest.Height(), 0, 0, GetWidth(), GetHeight(), nAlpha);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
		}
	}
}

// ��ͨ����ģʽ(��������)
inline void CLKImage::DrawToImage(CLKImage *pDestImg, CRect rtDest)
{
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
            //::SetStretchBltMode(memDC.GetSafeHdc(), COLORONCOLOR); 
			Draw(memDC.GetSafeHdc(), rtDest.left, rtDest.top, rtDest.Width(), rtDest.Height(), 0, 0, GetWidth(), GetHeight());
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
		}
	}
}

// ��ͨ����ģʽ(��������)
inline void CLKImage::DrawToImage(CLKImage *pDestImg, int nIndex, CRect rtDest)
{
	int nHeight = GetHeight() / GetImageNumber();
	int nOffset = nHeight * nIndex;
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			Draw(memDC.GetSafeHdc(), rtDest.left, rtDest.top, rtDest.Width(), rtDest.Height(), 0, nOffset, GetWidth(), nHeight);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
		}
	}
}

// ��ͨ����ģʽ(��������)
inline void CLKImage::DrawToImage(CLKImage *pDestImg, CRect rtDest, CRect rtSrc)
{
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			Draw(memDC.GetSafeHdc(), rtDest, rtSrc);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
		}
	}
}

// ��ͨ���췽ʽ(������)
inline void CLKImage::DrawToImage(CLKImage *pDestImg, int nIndex, PBGParam pParam)
{
	if(!pDestImg || !pDestImg->IsValid()) return;
	if(!m_nImgNumber)
	{
		::AfxMessageBox(_T("���ͼƬ����Ϊ0�����Ϸ��������������"));
		return;
	}
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			int nHeight = GetHeight() / m_nImgNumber;
			int nOffset = nIndex * nHeight;
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			// �ȸ��Ʊ���
			if(pParam->pBG && pParam->pBG)
			{
				CLKImage *pBG = (CLKImage *)pParam->pBG;
				if(pBG->IsValid())
				{
					pBG->BitBlt(memDC.GetSafeHdc(), 0, 0, pDestImg->GetWidth(), pDestImg->GetHeight(), pParam->rt.left, pParam->rt.top);
				}
			}
			Draw(memDC.GetSafeHdc(), 0, 0, pDestImg->GetWidth(), pDestImg->GetHeight(), 0, nOffset, GetWidth(), nHeight);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
		}
	}
}

// ��ͨ���췽ʽ(������)
inline void CLKImage::DrawToImage(CLKImage *pDestImg, CRect rtDest, PBGParam pParam)
{
	if(!pDestImg || !pDestImg->IsValid()) return;
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			//int nHeight = GetHeight() / m_nImgNumber;
			//int nOffset = nIndex * nHeight;
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			// �ȸ��Ʊ���
			if(pParam->pBG && pParam->pBG)
			{
				CLKImage *pBG = (CLKImage *)pParam->pBG;
				if(pBG->IsValid())
				{
					pBG->BitBlt(memDC.GetSafeHdc(), 0, 0, rtDest.Width(), rtDest.Height(), pParam->rt.left, pParam->rt.top);
				}
			}
			Draw(memDC.GetSafeHdc(), rtDest.left, rtDest.top, rtDest.Width(), rtDest.Height(), 0, 0, GetWidth(), GetHeight());
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
		}
	}
}

// ��ͨ���췽ʽ
inline void CLKImage::DrawToImage(CLKImage *pDestImg, int nIndex)
{
	if(!pDestImg || !pDestImg->IsValid()) return;
	if(!m_nImgNumber)
	{
		::AfxMessageBox(_T("���ͼƬ����Ϊ0�����Ϸ��������������"));
		return;
	}
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			int nHeight = GetHeight() / m_nImgNumber;
			int nOffset = nIndex * nHeight;
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			Draw(memDC.GetSafeHdc(), 0, 0, pDestImg->GetWidth(), pDestImg->GetHeight(), 0, nOffset, GetWidth(), nHeight);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
		}
	}
}

// ���帴��ģʽ
inline bool CLKImage::DrawToImage(CLKImage *pDest)
{
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDest);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			BitBlt(memDC.GetSafeHdc(), 0, 0);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
			return true;
		}
	}
	return false;
}

// ��������ģʽ
inline void CLKImage::StretchToImage(CLKImage *pImg)
{
	//StretchBltFast(pImg, 0, 0, pImg->GetWidth(), pImg->GetHeight(), this, 0, 0, GetWidth(), GetHeight());
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			::SetStretchBltMode(memDC.GetSafeHdc(), COLORONCOLOR); 
			//::SetBrushOrgEx(memDC.GetSafeHdc(), 0, 0, NULL);
			StretchBlt(memDC.GetSafeHdc(), 0, 0, pImg->GetWidth(), pImg->GetHeight(), SRCCOPY);
			////this->Draw(memDC.GetSafeHdc(), 0, 0);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
		}
	}
}

inline bool CLKImage::DrawToImage(CLKImage *pDestImg, int nIndex, PBGParam pParam, UINT uDrawStyle)
{
	CRect rtDest(0, 0, pDestImg->GetWidth(), pDestImg->GetHeight());
	return DrawToImage(pDestImg, rtDest, m_rtOffset, nIndex, pParam, uDrawStyle);
}

inline bool CLKImage::DrawToImage(CLKImage *pDestImg, PBGParam pParam, UINT uDrawStyle, AngleOffset angleOffset)
{
	CRect rtDest(0, 0, pDestImg->GetWidth(), pDestImg->GetHeight());
	return DrawToImage(pDestImg, rtDest, m_rtOffset, 0, pParam, uDrawStyle, angleOffset);
}

inline bool CLKImage::DrawToImage(CLKImage *pDestImg, CRect rtOffset, int nIndex, PBGParam pParam, UINT uDrawStyle)
{
	CRect rtDest(0, 0, pDestImg->GetWidth(), pDestImg->GetHeight());
	return DrawToImage(pDestImg, rtDest, rtOffset, nIndex, pParam, uDrawStyle);
}

inline bool CLKImage::DrawToImage(CLKImage *pDestImg, CRect rtDest, CRect rtOffset, int nIndex, PBGParam pParam, UINT uDrawStyle, AngleOffset angleOffset)
{
	if(!pDestImg || !pDestImg->IsValid()) return false;
	if(!m_nImgNumber)
	{
		::AfxMessageBox(_T("���ͼƬ����Ϊ0�����Ϸ��������������"));
		return false;
	}
	//CRect rtDest(0, 0, pDestImg->GetWidth(), pDestImg->GetHeight());
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			//int nHeight = GetHeight() / m_nImgNumber;
			//int nOffset = nIndex * nHeight;
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			// �ȸ��Ʊ���
			if(pParam && pParam->pBG && pParam->pBG)
			{
				CLKImage *pBG = (CLKImage *)pParam->pBG;
				if(pBG->IsValid())
				{
					pBG->BitBlt(memDC.GetSafeHdc(), 0, 0, pDestImg->GetWidth(), pDestImg->GetHeight(), pParam->rt.left, pParam->rt.top);
				}
			}
			bool b = DrawToDC(&memDC, rtDest, rtOffset, nIndex, uDrawStyle, 0, angleOffset);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
			return b;
		}
	}
	return false;
}

// ˫���Բ�ֵ����
inline void CLKImage::StretchBilinear(CLKImage* pDest, int cxDest, int cyDest, int cxSrc, int cySrc)
{
	int i, j;
	LPBYTE pBitsSrc = (LPBYTE)(GetBits()); //���ݿ���ʼλ��
	LPBYTE pBitsDest = (LPBYTE)(pDest->GetBits());//���ݿ���ʼλ��
	LPBYTE pixAddrSrc = pBitsSrc;
	LPBYTE pixAddrDest = pBitsDest;

	LPBYTE pixAddrTemp[4] = { 0 };

	int strideSrc = GetPitch(); //pitch��ʱΪ��
	int strideDest = pDest->GetPitch();
	int bytesPerPixelSrc = GetBPP() / 8;
	int bytesPerPixelDest = pDest->GetBPP() / 8;

	// ȡԴͼƬ��һ�е�ַ
	int nSourceRowWidth = abs(strideSrc);
	if (strideSrc < 0)
	{
		// strideSrc < 0ʱ��pBitsSrcָ�����һ��
		pBitsSrc = pBitsSrc + strideSrc * (GetHeight() - 1);
	}

	// ȡĿ��ͼƬ��һ�е�ַ
	int nDestRowWidth = abs(strideDest);
	if (strideDest < 0)
	{
		// strideDest < 0ʱ��pBitsDestָ�����һ��
		pBitsDest = pBitsDest + strideDest * (pDest->GetHeight() - 1);
	}

	double fRatex = (double)cxSrc / cxDest;
	double fRatey = (double)cySrc / cyDest;
	if (cxDest > pDest->GetWidth())
	{
		cxDest = pDest->GetWidth();
	}
	if (cyDest > pDest->GetHeight())
	{
		cyDest = pDest->GetHeight();
	}

	for (j = 0; j < cyDest; j++)
	{
		double srcy = (j + 0.4999999) * fRatey - 0.5;
		pixAddrDest = pBitsDest + nDestRowWidth * j;
		double v = srcy - (int)srcy;
		for (i = 0; i < cxDest && i < pDest->GetWidth(); i++)
		{
			pixAddrSrc = pBitsSrc + (int)(j * fRatey) * nSourceRowWidth + (int)(i * fRatex) *  bytesPerPixelSrc;
			if (i != cxDest - 1 && i != cxDest - 2 && j != cyDest - 1 && j != cyDest - 2 && j != 0 && i != 0)
			{
				double srcx = (i + 0.4999999) * fRatex - 0.5;
				double u = srcx - (int)srcx;
				pixAddrTemp[0] = pBitsSrc + (int)srcy * nSourceRowWidth + (int)srcx *  bytesPerPixelSrc;
				pixAddrTemp[1] = pBitsSrc + ((int)srcy + 1) * nSourceRowWidth + (int)srcx *  bytesPerPixelSrc;
				pixAddrTemp[2] = pBitsSrc + (int)srcy * nSourceRowWidth + ((int)srcx + 1) *  bytesPerPixelSrc;
				pixAddrTemp[3] = pBitsSrc + ((int)srcy + 1) * nSourceRowWidth + ((int)srcx + 1) *  bytesPerPixelSrc;
				*pixAddrDest = (BYTE)(*(pixAddrTemp[0]) * (1 - u) * (1 - v) + *(pixAddrTemp[1]) * (1 - u) * v + *(pixAddrTemp[2]) * u * (1 - v) + *(pixAddrTemp[3]) * u * v);
				*(pixAddrDest + 1) = (BYTE)(*(pixAddrTemp[0] + 1) * (1 - u) * (1 - v) + *(pixAddrTemp[1] + 1) * (1 - u) * v + *(pixAddrTemp[2] + 1) * u * (1 - v) + *(pixAddrTemp[3] + 1) * u * v);
				*(pixAddrDest + 2) = (BYTE)(*(pixAddrTemp[0] + 2) * (1 - u) * (1 - v) + *(pixAddrTemp[1] + 2) * (1 - u) * v + *(pixAddrTemp[2] + 2) * u * (1 - v) + *(pixAddrTemp[3] + 2) * u * v);
				pixAddrDest += bytesPerPixelDest;
				continue;
			}
			*pixAddrDest = *pixAddrSrc;
			*(pixAddrDest + 1) = *(pixAddrSrc + 1);
			*(pixAddrDest + 2) = *(pixAddrSrc + 2);
			pixAddrDest += bytesPerPixelDest;
		}
	}
	//int i,j;
    //LPBYTE pBitsSrc = (LPBYTE)(GetBits()); //���ݿ���ʼλ��
    //LPBYTE pBitsDest = (LPBYTE)(pDest->GetBits());//���ݿ���ʼλ��
    //LPBYTE pixAddrSrc = pBitsSrc;
    //LPBYTE pixAddrDest = pBitsDest;

    //LPBYTE pixAddrTemp[4] = {0};

    //int strideSrc = GetPitch(); //pitch��ʱΪ��
    //int strideDest = pDest->GetPitch();
    //int bytesPerPixelSrc = GetBPP()/8;
    //int bytesPerPixelDest = pDest->GetBPP()/8;

    //double fRate = (double)cxSrc / cxDest;
    //
    //for (j = 0; j < cyDest && j < pDest->GetHeight(); j++)
    //{
    //    double srcy = (j + 0.4999999) * fRate - 0.5;
    //    pixAddrDest = pBitsDest + strideDest * j;
    //    double v = srcy - (int)srcy;
    //    for (i = 0; i < cxDest && i < pDest->GetWidth(); i++)
    //    {
    //        pixAddrSrc = pBitsSrc + (int)(j * fRate) * strideSrc + (int)(i * fRate) *  bytesPerPixelSrc;
    //        if(i != cxDest - 1 && i != pDest->GetWidth() - 1 && j != cyDest - 1 && j != pDest->GetHeight() - 1 && j != 0 && i != 0)
    //        {
    //            double srcx = (i + 0.4999999) * fRate - 0.5;
    //            double u = srcx - (int)srcx;
    //            pixAddrTemp[0]  = pBitsSrc + (int)srcy * strideSrc + (int)srcx *  bytesPerPixelSrc;
    //            pixAddrTemp[1]  = pBitsSrc + ((int)srcy + 1) * strideSrc + (int)srcx *  bytesPerPixelSrc;
    //            pixAddrTemp[2]  = pBitsSrc + (int)srcy * strideSrc + ((int)srcx + 1) *  bytesPerPixelSrc;
    //            pixAddrTemp[3]  = pBitsSrc + ((int)srcy + 1) * strideSrc + ((int)srcx + 1) *  bytesPerPixelSrc;
    //            *pixAddrDest = (BYTE)(*(pixAddrTemp[0]) * (1-u) * (1-v) + *(pixAddrTemp[1]) * (1 - u) * v + *(pixAddrTemp[2]) * u * (1-v) + *(pixAddrTemp[3]) * u * v);
    //            *(pixAddrDest + 1) = (BYTE)(*(pixAddrTemp[0] + 1) * (1-u) * (1-v) + *(pixAddrTemp[1] + 1) * (1 - u) * v + *(pixAddrTemp[2] + 1) * u * (1-v) + *(pixAddrTemp[3] + 1) * u * v);
    //            *(pixAddrDest + 2) = (BYTE)(*(pixAddrTemp[0] + 2) * (1-u) * (1-v) + *(pixAddrTemp[1] + 2) * (1 - u) * v + *(pixAddrTemp[2] + 2) * u * (1-v) + *(pixAddrTemp[3] + 2) * u * v);
    //            pixAddrDest += bytesPerPixelDest;
    //            continue;
    //        }
    //        *pixAddrDest = *pixAddrSrc;
    //        *(pixAddrDest + 1) = *(pixAddrSrc + 1);
    //        *(pixAddrDest + 2) = *(pixAddrSrc + 2);
    //        pixAddrDest += bytesPerPixelDest;
    //    }
    //}
}

// pImage ���ڷǼ�������
inline bool CLKImage::DrawToImage(CLKImage *pDestImg, CLKImage *pImage, int nIndex, PBGParam pParam, UINT uDrawStyle)
{
	CRect rtDest(0, 0, pDestImg->GetWidth(), pDestImg->GetHeight());
	return DrawToImage(pDestImg, pImage, rtDest, m_rtOffset, nIndex, pParam, uDrawStyle);
}

inline bool CLKImage::DrawToImage(CLKImage *pDestImg, CLKImage *pImage, CRect rtDest, CRect rtOffset, int nIndex, PBGParam pParam, UINT uDrawStyle, AngleOffset angleOffset)
{
	if(!pDestImg || !pDestImg->IsValid()) return false;
	if(!m_nImgNumber)
	{
		::AfxMessageBox(_T("���ͼƬ����Ϊ0�����Ϸ��������������"));
		return false;
	}
	//CRect rtDest(0, 0, pDestImg->GetWidth(), pDestImg->GetHeight());
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			// �ȸ��Ʊ���
			if(pParam && pParam->pBG && pParam->pBG)
			{
				CLKImage *pBG = (CLKImage *)pParam->pBG;
				if(pBG->IsValid())
				{
					pBG->BitBlt(memDC.GetSafeHdc(), 0, 0, pDestImg->GetWidth(), pDestImg->GetHeight(), pParam->rt.left, pParam->rt.top);
				}
			}
			if(pImage && pImage->IsValid())
			{
				CRect rtImg(rtDest);
				rtImg.DeflateRect(&rtOffset);
				pImage->DrawToDC(&memDC, rtImg, 0);
			}
			bool b = DrawToDC(&memDC, rtDest, rtOffset, nIndex, uDrawStyle, 0, angleOffset);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
			return b;
		}
	}
	return false;
}

inline bool CLKImage::DrawToImageFrame(CLKImage *pDestImg, CRect rtOffset, AngleOffset angleOffset)
{
	if(!pDestImg || !pDestImg->IsValid()) return false;
	CRect rtDest(0, 0, pDestImg->GetWidth(), pDestImg->GetHeight());
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			bool b = DrawToDC(&memDC, rtDest, rtOffset, 0, LKSKINBASE_DRAWBGSTYLE_FRAME, 0, angleOffset);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
			return b;
		}
	}
	return false;
}


inline bool CLKImage::DrawToImageFrame(CLKImage *pDestImg)
{
	if(!pDestImg || !pDestImg->IsValid()) return false;
	CRect rtDest(0, 0, pDestImg->GetWidth(), pDestImg->GetHeight());
	return DrawToImageFrame(pDestImg, rtDest);
}

inline bool CLKImage::DrawToImageFrame(CLKImage *pDestImg, CRect rtDest)
{
	if(!pDestImg || !pDestImg->IsValid()) return false;
	//CRect rtDest(0, 0, pDestImg->GetWidth(), pDestImg->GetHeight());
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			/*CRect rtOffset(m_rtOffset);
			rtOffset.left = 5;
			rtOffset.right = 5;*/
			bool b = DrawToDC(&memDC, rtDest, m_rtOffset, 0, LKSKINBASE_DRAWBGSTYLE_FRAME);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
			return b;
		}
	}
	return false;
}

inline bool CLKImage::DrawToImageLR(CLKImage *pDestImg, CRect rtDest)
{
	if(!pDestImg || !pDestImg->IsValid()) return false;
	CWnd *pWnd = CWnd::FromHandle(GetDesktopWindow());
	if(pWnd && pWnd->GetSafeHwnd())
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pBmp = CBitmap::FromHandle(*pDestImg);
			CBitmap *pOld = memDC.SelectObject(pBmp);
			bool b = DrawToDC(&memDC, rtDest, m_rtOffset, 0, LKSKINBASE_DRAWBGSTYLE_LEFTMIDRIGHT);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
			return b;
		}
	}
	return false;
}

// ��ͨ����ģʽ
inline bool CLKImage::DrawToDC(CDC *pDC, CRect rtDest, int nIndex)
{
	int nHeight = GetHeight() / m_nImgNumber;
	int nOffset = nIndex * nHeight;
    // !! - ��������C4800����
	return !!Draw(pDC->GetSafeHdc(), rtDest.left, rtDest.top, rtDest.Width(), rtDest.Height(), 0, nOffset, GetWidth(), nHeight);
}

// ��ͨ������ģʽ
inline bool CLKImage::DrawToDC(CDC *pDC, int nLeft, int nTop, int nIndex)
{
	int nHeight = GetHeight() / m_nImgNumber;
	int nOffset = nIndex * nHeight;
	return !!Draw(pDC->GetSafeHdc(), nLeft, nTop, GetWidth(), nHeight, 0, nOffset, GetWidth(), nHeight);
}
// ʹ��ͼƬ�Դ�ƫ����
inline bool CLKImage::DrawToDC(CDC *pDC, CRect rtDest, int nIndex, UINT uDrawStyle, PBGParam pParam, AngleOffset angleOffset)
{
	return DrawToDC(pDC, rtDest, m_rtOffset, nIndex, uDrawStyle, pParam, angleOffset);
}

inline bool CLKImage::DrawToDC(CDC *pDC, CRect rtDest, CRect rtOffset, int nIndex, UINT uDrawStyle, PBGParam pParam, AngleOffset angleOffset)
{
	int nHeight = GetHeight() / m_nImgNumber;
	int nOffset = nIndex * nHeight;
	// �ȸ��Ʊ���
	if(pParam && pParam->pBG && pParam->pBG)
	{
		CLKImage *pBG = (CLKImage *)pParam->pBG;
		if(pBG->IsValid())
		{
            pBG->BitBlt(pDC->GetSafeHdc(), 0, 0, rtDest.Width(), rtDest.Height(), pParam->rt.left, pParam->rt.top);
		}
	}
	if(LKSKINBASE_DRAWBGSTYLE_COPY & uDrawStyle)
	{
		return !!Draw(pDC->GetSafeHdc(), rtDest.left, rtDest.top, GetWidth(), nHeight, 0, nOffset, GetWidth(), nHeight);
	}else if(rtDest.Height() == nHeight)
	{
		if(rtDest.Width() == GetWidth())
		{
			return !!Draw(pDC->GetSafeHdc(), rtDest.left, rtDest.top, rtDest.Width(), rtDest.Height(), 0, nOffset, rtDest.Width(), rtDest.Height());
		}else
		{
			return DrawFrame(pDC, rtDest, rtOffset, nOffset, uDrawStyle, angleOffset);
		}
	}else
	{
		return DrawFrame(pDC, rtDest, rtOffset, nOffset, uDrawStyle, angleOffset);
	}
	return false;
}

inline bool CLKImage::DrawLRToDC(CDC *pDC, CRect rtDest, int nOffsetL, int nOffsetR, int nOffset)
{
	int nHeight = GetHeight() / GetImageNumber();
	// ����
	bool bRet = !!Draw(pDC->GetSafeHdc(), rtDest.left, rtDest.top, nOffsetL, rtDest.Height(), 0, nOffset, nOffsetL, nHeight);
	// ����
	int nWidth = GetWidth() - nOffsetL - nOffsetR;
	if(rtDest.Width() - nOffsetL - nOffsetR > 0)
	{
		if(nWidth <= 0)
		{
			nWidth = 1;
		}
		//bRet = (!bRet ? bRet : Draw(pDC->GetSafeHdc(), rtDest.left + nOffsetL, rtDest.top, rtDest.Width() - nOffsetL - nOffsetR, rtDest.Height(), nOffsetL, nOffset, nWidth, nHeight));
		CRect rtM(rtDest);
		rtM.left += nOffsetL;
		rtM.right -= nOffsetR;
		rtM.bottom = rtM.top + nHeight;
		CRect rtSrc(nOffsetL, nOffset, nOffsetL + nWidth, nOffset + nHeight);
		Draw(pDC->GetSafeHdc(), rtM, rtSrc);
	}
	// ����
	bRet = (!bRet ? bRet : !!Draw(pDC->GetSafeHdc(), rtDest.right - nOffsetR, rtDest.top, nOffsetR, rtDest.Height(), GetWidth() - nOffsetR, nOffset, nOffsetR, nHeight));
	return bRet;
}

inline bool CLKImage::DrawLRToDCEx(CDC *pDC, CRect rtDest, int nOffsetL, int nOffsetR, int nOffset)
{
	int nHeight = GetHeight() / GetImageNumber();
	// ����
	bool bRet = !!Draw(pDC->GetSafeHdc(), rtDest.left, rtDest.top, nOffsetL, rtDest.Height(), 0, nOffset, nOffsetL, nHeight);
	// ����
	int nWidth = GetWidth() - nOffsetL - nOffsetR;
	if(rtDest.Width() - nOffsetL - nOffsetR > 0)
	{
		if(nWidth <= 0)
		{
			nWidth = 1;
		}
		CRect rtM(rtDest);
		int nRight = rtDest.right - nOffsetR;
		rtM.left = nOffsetL;
		rtM.right = rtDest.left + nWidth;
		CRect rtSrc(nOffsetL, nOffset, nOffsetL + nWidth, nOffset + nHeight);
		for(;rtM.right < nRight;)
		{
			rtM.right = rtM.left + nWidth;
			if(rtM.right > nRight)
			{
				rtM.right = nRight;
			}
			Draw(pDC->GetSafeHdc(), rtM, rtSrc);
			rtM.left = rtM.right;
		}
	}
	// ����
	bRet = (!bRet ? bRet : !!Draw(pDC->GetSafeHdc(), rtDest.right - nOffsetR, rtDest.top, nOffsetR, rtDest.Height(), GetWidth() - nOffsetR, nOffset, nOffsetR, nHeight));
	return bRet;
}

inline bool CLKImage::DrawFrame(CDC *pDC, CRect rtDest, CRect rtOffset, int nOffset, UINT uDrawStyle, AngleOffset angleOffset)
{
	bool bRet(true);
	int nHeight = GetHeight() / m_nImgNumber;
	if((uDrawStyle & LKSKINBASE_DRAWBGSTYLE_TM) && (rtOffset.top > 0) && bRet)
	{
		// �ϱ߿�
		CRect rtMT(rtDest);
		rtMT.left += rtOffset.left + angleOffset.nLTWidth;
		rtMT.right -= rtOffset.right + angleOffset.nRTWidth;
		rtMT.bottom = rtMT.top + rtOffset.top;
		int nWidth = GetWidth() - rtOffset.left - rtOffset.right - angleOffset.nLTWidth - angleOffset.nRTWidth;
		if(nWidth >= 0)
		{
			if(nWidth == 0)
			{
				nWidth = 1;
			}
			bRet = !!Draw(pDC->GetSafeHdc(), rtMT.left, rtMT.top, rtMT.Width(), rtMT.Height(), rtOffset.left + angleOffset.nLTWidth, nOffset, nWidth, rtOffset.top);
		}
	}
	if((uDrawStyle & LKSKINBASE_DRAWBGSTYLE_BM) && (rtOffset.bottom > 0) && bRet)
	{
		// �±߿�
		CRect rtBM(rtDest);
		rtBM.left += rtOffset.left + angleOffset.nLBWidth;
		rtBM.right -= rtOffset.right + angleOffset.nRBWidth;
		rtBM.top = rtBM.bottom - rtOffset.bottom;
		int nWidth = GetWidth() - rtOffset.left - rtOffset.right - angleOffset.nLBWidth - angleOffset.nRBWidth;
		if(nWidth >= 0)
		{
			if(nWidth == 0)
			{
				nWidth = 1;
			}
			bRet = !!Draw(pDC->GetSafeHdc(), rtBM.left, rtBM.top, rtBM.Width(), rtBM.Height(), rtOffset.left + angleOffset.nLBWidth, nOffset + nHeight - rtOffset.bottom, nWidth, rtOffset.bottom);
		}
	}
	if((uDrawStyle & LKSKINBASE_DRAWBGSTYLE_LM) && (rtOffset.left > 0) && bRet)
	{
		// ��߿�
		CRect rtLM(rtDest);
		rtLM.right = rtLM.left + rtOffset.left;
		rtLM.top += rtOffset.top + angleOffset.nLTHeight;
		rtLM.bottom -= rtOffset.bottom + angleOffset.nLBHeight;
		int nH = nHeight - rtOffset.top - rtOffset.bottom;
		if(nH >= 0)
		{
			if(nH == 0)	nH = 1;
			bRet = !!Draw(pDC->GetSafeHdc(), rtLM.left, rtLM.top, rtLM.Width(), rtLM.Height(), 0, nOffset + rtOffset.top, rtOffset.left, nH);
		}
	}
	if((uDrawStyle & LKSKINBASE_DRAWBGSTYLE_RM) && (rtOffset.right > 0) && bRet)
	{
		// �ұ߿�
		CRect rtRM(rtDest);
		rtRM.left = rtRM.right - rtOffset.right;
		rtRM.top += rtOffset.top + angleOffset.nRTHeight;
		rtRM.bottom -= rtOffset.bottom + angleOffset.nRBHeight;
		int nH = nHeight - rtOffset.top - rtOffset.bottom;
		if(nH >= 0)
		{
			if(nH == 0)
			{
				nH = 1;
			}
			bRet = !!Draw(pDC->GetSafeHdc(), rtRM.left, rtRM.top, rtRM.Width(), rtRM.Height(), GetWidth() - rtOffset.right, nOffset + rtOffset.top, rtOffset.right, nH);
		}
	}
	if((uDrawStyle & LKSKINBASE_DRAWBGSTYLE_LT)  && bRet)
	{
		if(rtOffset.top > 0 && rtOffset.left > 0)
		{
			// ���Ͻ�
			CRect rtLT(rtDest);
			rtLT.right = rtLT.left + rtOffset.left + angleOffset.nLTWidth;
			rtLT.bottom = rtLT.top + rtOffset.top + angleOffset.nLTHeight;
			bRet = !!Draw(pDC->GetSafeHdc(), rtLT.left, rtLT.top, rtLT.Width(), rtLT.Height(), 0, nOffset, rtLT.Width(), rtLT.Height());
		}
	}
	if((uDrawStyle & LKSKINBASE_DRAWBGSTYLE_RT)  && bRet)
	{
		if(rtOffset.top > 0 && rtOffset.right > 0)
		{
			// ���Ͻ�
			CRect rtRT(rtDest);
			rtRT.left = rtRT.right - rtOffset.right - angleOffset.nRTWidth;
			rtRT.bottom = rtRT.top + rtOffset.top + angleOffset.nRTHeight;
			bRet = !!Draw(pDC->GetSafeHdc(), rtRT.left, rtRT.top, rtRT.Width(), rtRT.Height(), GetWidth() - rtRT.Width(), nOffset, rtRT.Width(), rtRT.Height());
		}
	}
	if((uDrawStyle & LKSKINBASE_DRAWBGSTYLE_LB) && bRet)
	{
		if(rtOffset.bottom > 0 && rtOffset.left > 0)
		{
			// ���½�
			CRect rtLB(rtDest);
			rtLB.right = rtLB.left + rtOffset.left + angleOffset.nLBWidth;
			rtLB.top = rtLB.bottom - rtOffset.bottom - angleOffset.nLBHeight;
			bRet = !!Draw(pDC->GetSafeHdc(), rtLB.left, rtLB.top, rtLB.Width(), rtLB.Height(), 0, nOffset + nHeight - rtLB.Height(), rtLB.Width(), rtLB.Height());
		}
	}
	if((uDrawStyle & LKSKINBASE_DRAWBGSTYLE_RB) && bRet)
	{
		if(rtOffset.right > 0 && rtOffset.bottom > 0)
		{
			// ���½�
			CRect rtRB(rtDest);
			rtRB.left = rtRB.right - rtOffset.right - angleOffset.nRBWidth;
			rtRB.top = rtRB.bottom - rtOffset.bottom - angleOffset.nRBHeight;
			bRet = !!Draw(pDC->GetSafeHdc(), rtRB.left, rtRB.top, rtRB.Width(), rtRB.Height(), GetWidth() - rtRB.Width(), nOffset + nHeight - rtRB.Height(), rtRB.Width(), rtRB.Height());
		}
	}
	// �ͻ���
	if((uDrawStyle & LKSKINBASE_DRAWBGSTYLE_MM) && bRet)
	{
		CRect rtClient(rtDest);
		rtClient.DeflateRect(rtOffset.left, rtOffset.top, rtOffset.right, rtOffset.bottom);
		int nWidth = GetWidth() - rtOffset.left - rtOffset.right;
		int nH = nHeight - rtOffset.top - rtOffset.bottom;
		if(rtClient.Width() > 0 && rtClient.Height() > 0)
		{
			if(nH <= 0)	nH = 1;
			if(nWidth <= 0)	nWidth = 1;
			//bRet = Draw(pDC->GetSafeHdc(), rtClient.left, rtClient.top, rtClient.Width(), rtClient.Height(), rtOffset.left, nOffset + rtOffset.top, nWidth, nH);
			
			//CRect rtSrc(rtOffset.left, nOffset + rtOffset.top, GetWidth()- rtOffset.right, nOffset + nHeight - rtOffset.bottom);
			CRect rtSrc(rtOffset.left, nOffset + rtOffset.top, rtOffset.left + nWidth, nOffset + rtOffset.top + nH);
			CRect rtDest(rtClient.left, rtClient.top, rtClient.left + nWidth, rtClient.top + nH);
			Draw(pDC->GetSafeHdc(), rtClient, rtSrc);
			//for()
		}
	}
	return bRet;
}

inline bool CLKImage::LoadEx(CString strFileName, bool bCalcARGB)
{
	bool bRet = false;
	if (strFileName.Right(4).CompareNoCase(L".ico") == 0)
	{
		HICON h = (HICON)::LoadImage(AfxGetApp()->m_hInstance, strFileName, IMAGE_ICON, 48, 48, LR_LOADFROMFILE | LR_DEFAULTCOLOR);
		if (h)
		{
			if (CreateImage(h))
			{
				::DeleteObject(h);
				return IsValid();
			}
		}
	}
	if (E_FAIL != CImage::Load(strFileName))
	{
		bRet = true;
	}
	if (bRet && bCalcARGB)
	{
		CalcPARGB();
	}
	return bRet;
}

inline bool CLKImage::LoadEx(HRSRC hResource, HMODULE hInst, bool bCalcARGB)
{
	bool bRet(false);

	if (!hResource)
		return false;

	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
		return false;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)
		return false;

	HGLOBAL hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (hBuffer)
	{
		void* pBuffer = ::GlobalLock(hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
			{
				if(E_FAIL != CImage::Load(pStream))
				{
					bRet = true;
				}
				pStream->Release();
			}
			::GlobalUnlock(hBuffer);
		}
		::GlobalFree(hBuffer);
	}
	if(bRet && bCalcARGB)
	{
		CalcPARGB();
	}
	return bRet;
}

inline bool CLKImage::LoadEx(LPCTSTR pName, bool bCalcARGB, CRect &rtFrame, LPCTSTR pType, HMODULE hInst)
{
	bool bRet(false);
	/*CString strIcon(_T("ICON"));
	if(strIcon.CompareNoCase(pType) == 0)
	{
		pType = RT_ICON;
	}*/
	HRSRC hResource = ::FindResource(hInst, pName, pType);
	if (!hResource)
		return false;

	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
		return false;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)
		return false;

	HGLOBAL hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (hBuffer)
	{
		void* pBuffer = ::GlobalLock(hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
			{
				if(E_FAIL != CImage::Load(pStream))
				{
					bRet = true;
				}
				pStream->Release();
			}
			::GlobalUnlock(hBuffer);
		}
		::GlobalFree(hBuffer);
	}
	if(bRet && bCalcARGB)
	{
		CalcPARGB();
	}
	m_rtOffset = rtFrame;
	return bRet;
}

inline void CLKImage::CalcPARGB()
{
	/*PNGͼƬ��͸����������һƬ��ɫ�������ŷ�������ʵ��΢��GDI+��������⣬PNGͼƬ��ARGB��ʹ��GDI+����ͼƬ��ʱ��GDI+��Ĭ���Ѿ�������Ԥʣ���㣨PARGB������ÿ���ص�ʵ��ֵ���Ѿ���ALPHAֵ��������˵Ľ����ʵ������������û����Ԥ�ˣ���ʹ��͸��ͼƬ������ALPHAͨ����ʱ��CImage�ڲ����ǵ��õ�AlphaBlend��û��Ԥ�˵�ͼ����Ԥ�˵�ͼƬ����Ľ���������൱��һ�źʹ��ױ���������Ԥʣ������ͼ�����ǳ��ְ�ɫ������
	PNGͼƬ��͸����������һƬ��ɫ�������ŷ�������ʵ��΢��GDI+��������⣬PNGͼƬ��ARGB��ʹ��GDI+����ͼƬ��ʱ��GDI+��Ĭ���Ѿ�������Ԥʣ���㣨PARGB������ÿ���ص�ʵ��ֵ���Ѿ���ALPHAֵ��������˵Ľ����ʵ������������û����Ԥ�ˣ���ʹ��͸��ͼƬ������ALPHAͨ����ʱ��CImage�ڲ����ǵ��õ�AlphaBlend��û��Ԥ�˵�ͼ����Ԥ�˵�ͼƬ����Ľ���������൱��һ�źʹ��ױ���������Ԥʣ������ͼ�����ǳ��ְ�ɫ������*/
	unsigned char pucColor[4];
	for(int i = 0; i < GetWidth(); i++)  
	{  
		for(int j = 0; j < GetHeight(); j++)  
		{  
			unsigned char* pucColorx = reinterpret_cast<unsigned char *>(GetPixelAddress(i , j));  
			memcpy(pucColor, pucColorx, 4);
			pucColor[0] = pucColor[0] * pucColor[3] / 255;  
			pucColor[1] = pucColor[1] * pucColor[3] / 255;  
			pucColor[2] = pucColor[2] * pucColor[3] / 255;  
			memcpy(pucColorx, pucColor, 4);
		}  
	} 
}

inline bool CLKImage::LoadMemImage(void *pMemData, long len)
{
	bool bRet(false);
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
	void *pData = GlobalLock(hGlobal);
	memcpy(pData, pMemData, len);
	GlobalUnlock(hGlobal);
	IStream *pStream = NULL;
	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		CImage image;
		if (SUCCEEDED(Load(pStream)))
		{
			bRet = true;
		} 
		pStream->Release();     
	} 
	GlobalFree(hGlobal);
	return bRet;
} 


// ͼƬ�б�
typedef CArray<CLKImage *, CLKImage *> ImagePointArray;

#endif