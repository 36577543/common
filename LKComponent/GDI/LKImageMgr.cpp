/******************************************************************************
* ��Ȩ���� (C)2011, Sandongcun������
*
* �ļ����ƣ�LKImageMgr.h
* ����ժҪ��ͼƬ�б���
* ����˵����
* ��ǰ�汾��v1.0
* ��    �ߣ�����
* ������ڣ�2011.09.17  
******************************************************************************/
//#include "StdAfx.h"
#include "LKImageMgr.h"

CLKImageMgr *CSingle<CLKImageMgr>::m_pInstance = 0;

// ��ʼ��ͼƬ�б�
void CLKImageMgr::InitImageArray(int nCount)
{
	m_arrImage.SetSize(nCount);
	//for(int i = 0; i < m_arrImage.GetCount(); i++)
	//{
	//	m_arrImage.SetAt(i, 0);
	//}
}

// ����Ϊ��
bool CLKImageMgr::SetImage(int nIndex)
{
	if(nIndex >= 0 && nIndex < m_arrImage.GetCount())
	{
		m_arrImage.SetAt(nIndex, 0);
		return true;
	}
	return false;
}

// ����һ���ļ�ͼƬ
bool CLKImageMgr::SetImage(int nIndex, CString &strFileName, CRect rtFrame, int nImgNumber, bool bCalcARGB)
{
	if(nIndex >= 0 && nIndex < m_arrImage.GetCount())
	{
		CLKImage *p = new CLKImage;
		if(p->Load(strFileName))
		{
			if(bCalcARGB)
			{
				p->CalcPARGB();
			}
			p->SetImageNumber(nImgNumber);
			p->SetFrameRect(rtFrame);
			m_arrImage.SetAt(nIndex, p);
			return true;
		}
		delete p;
	}
	return false;
}

// ����һ����ԴͼƬ
bool CLKImageMgr::SetImage(int nIndex, UINT uResourceId, CString strType, CRect rtFrame, int nImgNumber, bool bCalcARGB, HMODULE hM)
{
	if(nIndex >= 0 && nIndex < m_arrImage.GetCount())
	{
        if(m_arrImage[nIndex])
        {
            delete m_arrImage[nIndex];
        }
		CLKImage *p = new CLKImage;
		if(p->LoadEx(uResourceId, bCalcARGB, rtFrame, strType, hM))
		{
			p->SetImageNumber(nImgNumber);
			m_arrImage.SetAt(nIndex, p);
			return true;
		}
		delete p;
	}
	return false;
}


// ����һ���ļ�ͼƬ
int CLKImageMgr::AddImage(CString &strFileName, CRect rtFrame, int nImgNumber, bool bCalcARGB)
{
	CLKImage *p = new CLKImage;
	if(p->Load(strFileName))
	{
		if(bCalcARGB)
		{
			p->CalcPARGB();
		}
		p->SetImageNumber(nImgNumber);
		p->SetFrameRect(rtFrame);
		m_arrImage.Add(p);
		return m_arrImage.GetCount();
	}
	delete p;
	return 0;
}

// ����һ����ԴͼƬ
int CLKImageMgr::AddImage(UINT uResourceId, CString strType, CRect rtFrame, int nImgNumber, bool bCalcARGB, HMODULE hM)
{
	//
	CLKImage *p = new CLKImage;
	if(p->LoadEx(uResourceId, bCalcARGB, rtFrame, strType, hM))
	{
		p->SetImageNumber(nImgNumber);
		m_arrImage.Add(p);
		return m_arrImage.GetCount();
	}
	delete p;
	return 0;
}

//-------------------------------------------------------------------------------------------------
// ������
//   ��ȡ����ͼƬָ���б�
// ������
//   arrImg      - Ŀ��ͼƬָ���б��������
//   nStartIndex - Ҫ��ȡ��ͼƬָ����Դ�б��е���ʼ����
//   nEndIndex	 - Ҫ��ȡ��ͼƬָ����Դ�б��еĽ�������(�������������Ƶ�Ŀ���б�)
// ����ֵ��
//   > 0		 - �ɹ�
//   0  		 - ʧ��
//-------------------------------------------------------------------------------------------------
int CLKImageMgr::GetPartImage(ImagePointArray &arrImg, int nStartIndex, int nEndIndex)
{
	//arrImg.RemoveAll();
	if(nEndIndex >= nStartIndex && nStartIndex >= 0 && nEndIndex < m_arrImage.GetCount())
	{
		//arrImg.SetSize(nEndIndex - nStartIndex + 1);
		for(int i = nStartIndex; i <= nEndIndex; i++)
		{
			arrImg.Add(m_arrImage[i]);
		}
		return arrImg.GetCount();
	}
	return 0;
}
// ��ȡͼƬָ��
CLKImage *CLKImageMgr::GetImage(int nIndex)
{
	if(nIndex >= 0 && nIndex < m_arrImage.GetCount())
	{
		return m_arrImage[nIndex];
	}
	return 0;
}
// ��ȡͷ��ָ��
CLKImage *CLKImageMgr::GetHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount)
{
	//if(nHeaderIndex < 0 || nHeaderIndex > LKPROJECTHEADIMAGE_HEAD_COUNT) return 0;
	//int nIndex = LKPROJECTHEADIMAGE_INDEX_BEGIN + nHeaderIndex;
	return m_arrImage[nHeaderBeginIndex];
}

// ��ȡСͷ��ָ��
CLKImage *CLKImageMgr::GetSmallHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount)
{
	//if(nHeaderIndex < 0 || nHeaderIndex > LKPROJECTHEADIMAGE_HEAD_COUNT) return 0;
	//int nIndex = LKPROJECTHEADIMAGE_INDEX_BEGIN + LKPROJECTHEADIMAGE_HEAD_COUNT + nHeaderIndex;
	//if(!m_arrImage[nIndex])
	//{
	//	m_arrImage[nIndex] = 0;
	//}
	//return m_arrImage[nIndex];
	return m_arrImage[nHeaderBeginIndex];
}

// ��ȡ�Ҷ�ͷ��ָ��
CLKImage *CLKImageMgr::GetGrayHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount)
{
	return 0;
}

// ��ȡ�Ҷ�Сͷ��ָ��
CLKImage *CLKImageMgr::GetGraySmallHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount)
{
	// 
	return m_arrImage[LKPROJECT_INDEX_BEGIN + 28];
}

// ��ȡͷ��ͼƬָ��
CLKImage *CLKImageMgr::GetHeadS(int nHeadIndex, int nHeaderBeginIndex, int nHeaderCount, int nType)
{
	//LKIMAGEBUTTONINDEX_COMMON_ICONNOTIFY
	//// ��������
	//#define USERINFO_STATE_ONLINE   1
	//// æµ״̬
	//#define USERINFO_STATE_BUSY     2
	//// ��������״̬
	//#define USERINFO_STATE_OFFLINE  0

	CLKImageMgr *p = CLKImageMgr::GetInstance();
	if(0 == nType)
	{
		return p->GetSmallHead(nHeadIndex, nHeaderBeginIndex, nHeaderCount);
	}
	return p->GetGraySmallHead(nHeadIndex, nHeaderBeginIndex, nHeaderCount);
}

CLKImageMgr::CLKImageMgr(void)
{
}

CLKImageMgr::~CLKImageMgr(void)
{
	for(int i = 0; i < m_arrImage.GetCount(); i++)
	{
		CLKImage *p = m_arrImage[i];
		if(p)
		{
			delete p;
		}
	}
}

