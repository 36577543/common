/******************************************************************************
* 版权所有 (C)2011, Sandongcun开发组
*
* 文件名称：LKImageMgr.h
* 内容摘要：图片列表类
* 其它说明：
* 当前版本：v1.0
* 作    者：刘坤
* 完成日期：2011.09.17  
******************************************************************************/
//#include "StdAfx.h"
#include "LKImageMgr.h"

CLKImageMgr *CSingle<CLKImageMgr>::m_pInstance = 0;

// 初始化图片列表
void CLKImageMgr::InitImageArray(int nCount)
{
	m_arrImage.SetSize(nCount);
	//for(int i = 0; i < m_arrImage.GetCount(); i++)
	//{
	//	m_arrImage.SetAt(i, 0);
	//}
}

// 设置为空
bool CLKImageMgr::SetImage(int nIndex)
{
	if(nIndex >= 0 && nIndex < m_arrImage.GetCount())
	{
		m_arrImage.SetAt(nIndex, 0);
		return true;
	}
	return false;
}

// 设置一个文件图片
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

// 设置一个资源图片
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


// 增加一个文件图片
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

// 增加一个资源图片
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
// 描述：
//   获取部分图片指针列表
// 参数：
//   arrImg      - 目标图片指针列表对象引用
//   nStartIndex - 要获取的图片指针在源列表中的起始索引
//   nEndIndex	 - 要获取的图片指针在源列表中的结束索引(包括此索引复制到目标列表)
// 返回值：
//   > 0		 - 成功
//   0  		 - 失败
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
// 获取图片指针
CLKImage *CLKImageMgr::GetImage(int nIndex)
{
	if(nIndex >= 0 && nIndex < m_arrImage.GetCount())
	{
		return m_arrImage[nIndex];
	}
	return 0;
}
// 获取头像指针
CLKImage *CLKImageMgr::GetHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount)
{
	//if(nHeaderIndex < 0 || nHeaderIndex > LKPROJECTHEADIMAGE_HEAD_COUNT) return 0;
	//int nIndex = LKPROJECTHEADIMAGE_INDEX_BEGIN + nHeaderIndex;
	return m_arrImage[nHeaderBeginIndex];
}

// 获取小头像指针
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

// 获取灰度头像指针
CLKImage *CLKImageMgr::GetGrayHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount)
{
	return 0;
}

// 获取灰度小头像指针
CLKImage *CLKImageMgr::GetGraySmallHead(int nHeaderIndex, int nHeaderBeginIndex, int nHeaderCount)
{
	// 
	return m_arrImage[LKPROJECT_INDEX_BEGIN + 28];
}

// 获取头像图片指针
CLKImage *CLKImageMgr::GetHeadS(int nHeadIndex, int nHeaderBeginIndex, int nHeaderCount, int nType)
{
	//LKIMAGEBUTTONINDEX_COMMON_ICONNOTIFY
	//// 正常在线
	//#define USERINFO_STATE_ONLINE   1
	//// 忙碌状态
	//#define USERINFO_STATE_BUSY     2
	//// 正常离线状态
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

