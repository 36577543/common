#pragma once

#include "GDICommon.h"
#include "..\..\Single.h"
#include "LKImage.h"

class CLKImageMgr : public CSingle<CLKImageMgr>
{
public:
	// 获取部分图片指针列表
	static int GetPartImageS(ImagePointArray &arrImg, int nStartIndex, int nEndIndex){CLKImageMgr *p = CLKImageMgr::GetInstance(); return p->GetPartImage(arrImg, nStartIndex, nEndIndex);}
	// 获取图片指针
	static CLKImage *GetImageS(int nIndex){CLKImageMgr *p = CLKImageMgr::GetInstance(); return p->GetImage(nIndex);}
	// 获取头像图片指针
	static CLKImage *GetHeadS(int nHeadIndex, int nHeaderBeginIndex, int nHeaderCount, int nType);
public:
	// 初始化图片列表
	void InitImageArray(int nCount = 0);
	// 设置为空
	bool SetImage(int nIndex);
	// 设置一个文件图片
	bool SetImage(int nIndex, CString &strFileName, CRect rtFrame, int nImgNumber, bool bCalcARGB);
	// 设置一个资源图片
	bool SetImage(int nIndex, UINT uResourceId, CString strType, CRect rtFrame, int nImgNumber, bool bCalcARGB, HMODULE hM = 0);
	// 设置一个PNG资源图片
	bool SetPNGImage(int nIndex, UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = true, HMODULE hM = 0){return SetImage(nIndex, uResourceId, _T("PNG"), rtFrame, nImgNumber, bCalcARGB, hM);}
	// 设置一个JPG资源图片
	bool SetJPGImage(int nIndex, UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = false, HMODULE hM = 0){return SetImage(nIndex, uResourceId, _T("JPG"), rtFrame, nImgNumber, bCalcARGB, hM);}
	// 设置一个ICO资源图片
	bool SetICOImage(int nIndex, UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = false, HMODULE hM = 0){return SetImage(nIndex, uResourceId, _T("ICON"), rtFrame, nImgNumber, bCalcARGB, hM);}


	// 增加一个文件图片
	int AddImage(CString &strFileName, CRect rtFrame, int nImgNumber, bool bCalcARGB);
	// 增加一个资源图片
	int AddImage(UINT uResourceId, CString strType, CRect rtFrame, int nImgNumber, bool bCalcARGB, HMODULE hM = 0);
	// 增加一个PNG资源图片
	int AddPNGImage(UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = true, HMODULE hM = 0){return AddImage(uResourceId, _T("PNG"), rtFrame, nImgNumber, bCalcARGB, hM);}
	// 增加一个JPG资源图片
	int AddJPGImage(UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = false, HMODULE hM = 0){return AddImage(uResourceId, _T("JPG"), rtFrame, nImgNumber, bCalcARGB, hM);}

	// 获取部分图片指针列表
	int GetPartImage(ImagePointArray &arrImg, int nStartIndex, int nEndIndex);
	// 获取图片指针
	CLKImage *GetImage(int nIndex);
	// 获取头像指针
	CLKImage *GetHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount);
	// 获取小头像指针
	CLKImage *GetSmallHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount);
	// 获取灰度头像指针
	CLKImage *GetGrayHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount);
	// 获取灰度小头像指针
	CLKImage *GetGraySmallHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount);
private:
	CLKImageMgr(void);
	~CLKImageMgr(void);
	
private:
	CArray<CLKImage *, CLKImage *> m_arrImage;

	friend class CSingle<CLKImageMgr>;
};

