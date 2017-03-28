#pragma once

#include "GDICommon.h"
#include "..\..\Single.h"
#include "LKImage.h"

class CLKImageMgr : public CSingle<CLKImageMgr>
{
public:
	// ��ȡ����ͼƬָ���б�
	static int GetPartImageS(ImagePointArray &arrImg, int nStartIndex, int nEndIndex){CLKImageMgr *p = CLKImageMgr::GetInstance(); return p->GetPartImage(arrImg, nStartIndex, nEndIndex);}
	// ��ȡͼƬָ��
	static CLKImage *GetImageS(int nIndex){CLKImageMgr *p = CLKImageMgr::GetInstance(); return p->GetImage(nIndex);}
	// ��ȡͷ��ͼƬָ��
	static CLKImage *GetHeadS(int nHeadIndex, int nHeaderBeginIndex, int nHeaderCount, int nType);
public:
	// ��ʼ��ͼƬ�б�
	void InitImageArray(int nCount = 0);
	// ����Ϊ��
	bool SetImage(int nIndex);
	// ����һ���ļ�ͼƬ
	bool SetImage(int nIndex, CString &strFileName, CRect rtFrame, int nImgNumber, bool bCalcARGB);
	// ����һ����ԴͼƬ
	bool SetImage(int nIndex, UINT uResourceId, CString strType, CRect rtFrame, int nImgNumber, bool bCalcARGB, HMODULE hM = 0);
	// ����һ��PNG��ԴͼƬ
	bool SetPNGImage(int nIndex, UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = true, HMODULE hM = 0){return SetImage(nIndex, uResourceId, _T("PNG"), rtFrame, nImgNumber, bCalcARGB, hM);}
	// ����һ��JPG��ԴͼƬ
	bool SetJPGImage(int nIndex, UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = false, HMODULE hM = 0){return SetImage(nIndex, uResourceId, _T("JPG"), rtFrame, nImgNumber, bCalcARGB, hM);}
	// ����һ��ICO��ԴͼƬ
	bool SetICOImage(int nIndex, UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = false, HMODULE hM = 0){return SetImage(nIndex, uResourceId, _T("ICON"), rtFrame, nImgNumber, bCalcARGB, hM);}


	// ����һ���ļ�ͼƬ
	int AddImage(CString &strFileName, CRect rtFrame, int nImgNumber, bool bCalcARGB);
	// ����һ����ԴͼƬ
	int AddImage(UINT uResourceId, CString strType, CRect rtFrame, int nImgNumber, bool bCalcARGB, HMODULE hM = 0);
	// ����һ��PNG��ԴͼƬ
	int AddPNGImage(UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = true, HMODULE hM = 0){return AddImage(uResourceId, _T("PNG"), rtFrame, nImgNumber, bCalcARGB, hM);}
	// ����һ��JPG��ԴͼƬ
	int AddJPGImage(UINT uResourceId, CRect rtFrame = CRect(0, 0, 0,0), int nImgNumber = 1, bool bCalcARGB = false, HMODULE hM = 0){return AddImage(uResourceId, _T("JPG"), rtFrame, nImgNumber, bCalcARGB, hM);}

	// ��ȡ����ͼƬָ���б�
	int GetPartImage(ImagePointArray &arrImg, int nStartIndex, int nEndIndex);
	// ��ȡͼƬָ��
	CLKImage *GetImage(int nIndex);
	// ��ȡͷ��ָ��
	CLKImage *GetHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount);
	// ��ȡСͷ��ָ��
	CLKImage *GetSmallHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount);
	// ��ȡ�Ҷ�ͷ��ָ��
	CLKImage *GetGrayHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount);
	// ��ȡ�Ҷ�Сͷ��ָ��
	CLKImage *GetGraySmallHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount);
private:
	CLKImageMgr(void);
	~CLKImageMgr(void);
	
private:
	CArray<CLKImage *, CLKImage *> m_arrImage;

	friend class CSingle<CLKImageMgr>;
};

