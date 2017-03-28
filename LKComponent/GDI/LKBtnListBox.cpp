// ..\..\common\LKComponent\GDI\LKBtnListBox.cpp : ʵ���ļ�
//
#include "LKBtnListBox.h"
#include "LKImageMgr.h"


// CLKBtnListBox

IMPLEMENT_DYNAMIC(CLKBtnListBox, CLKCtrlBaseEx)

CLKBtnListBox::CLKBtnListBox()
: CLKCtrlBaseEx(0)
// ��Ŀ��
, m_nItemWidth(85)
// ��ĸ߶�
, m_nItemHeight(58)
// �������
, m_nMaxCol(0)
// ���ɼ������
, m_nMaxVisibleCount(0)
// �ɼ�������
, m_nVisibleIndex(0)
// ��ǰѡ������
, m_nSelectIndex(-1)
{
	// ������
	SetScrollbarImg(CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_VSCROLLBAR), 0);
}

CLKBtnListBox::~CLKBtnListBox()
{
	for(int i = 0; i < m_ltItems.GetCount(); i++)
	{
		CLKImage *pImg = m_ltItems[i]->GetButtonImg();
		if(pImg && CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_NOSKIN) != pImg)
		{
			delete pImg;
		}
		m_ltItems[i]->DestroyWindow();
		delete m_ltItems[i];
	}
}

BOOL CLKBtnListBox::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCtrlBase::Create(_T(""), WS_CHILD | WS_VISIBLE/* | WS_CLIPCHILDREN*/, rect, pParentWnd, nID); 
	return bRet;
}

// ����һ��
void CLKBtnListBox::AddItem(CLKImage *pImg, CString strImage)
{
	//
	CLKButtonEx *pBtn = new CLKButtonEx(pImg, strImage);
	CRect rt(0, 0, m_nItemWidth, m_nItemHeight);
	pBtn->CreateHide(rt, this, m_ltItems.GetCount());
	m_ltItems.Add(pBtn);
	if(GetSafeHwnd())
	{
		// �������ɼ�����
		CalcMaxVisibleCount();
		// ������ťλ��
		AdjustButton();
	}
	pBtn->Invalidate(false);
}

void CLKBtnListBox::AddItem(CString strImage)
{
	CLKImage *pImg = new CLKImage;
	if(pImg->Load(strImage) != E_FAIL)
	{
		AddItem(pImg);
	}else
	{
		delete pImg;
		AddItem(CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_NOSKIN));
	}
}

// �ֶ�ˢ��
void CLKBtnListBox::UpdateCtrl()
{
	for(int i = 0; i < m_ltItems.GetCount(); i++)
	{
		//m_ltItems[i]->Invalidate();
	}
}

// ��ȡָ�����ͼƬ·��
CString CLKBtnListBox::GetButtonImgPath(int nIndex)
{
    if(nIndex >= 0 && nIndex < m_ltItems.GetCount())
    {
        return m_ltItems[nIndex]->GetButtonImgPath();
    }
    return _T("");
}

BEGIN_MESSAGE_MAP(CLKBtnListBox, CLKCtrlBaseEx)
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
END_MESSAGE_MAP()



//=================================================================================================
// CLKBtnListBox ��Ϣ�������
LRESULT CLKBtnListBox::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR == wParam)
	{
		// ˮƽ����
		LKSCROLLINFO *pInfo = (LKSCROLLINFO *)lParam;
		// ������Ϣ
		m_nVisibleIndex = pInfo->nPos * m_nMaxCol;
		// �������ɼ�����
		CalcMaxVisibleCount();
		// ������ťλ��
		AdjustButton(false);
		//CRect rtData;
		//GetDataRect(rtData);
		//rtData.right -= 2;
		//InvalidateRect(&rtData, false);
		Invalidate(false);
	}else if(LKBUTTONEX_MSG_WPARAM_DELETECLICK == wParam)
	{
		//
		//CLKMessageBox dlg;
		//if(dlg.DoModal() == IDOK)
		//{
			PostMessage(LKCOMMON_MSG, LKBUTTONEX_MSG_WPARAM_DELETE, lParam);
		//}
		return 1;
	}else if(LKBUTTONEX_MSG_WPARAM_SELECTED == wParam)
	{
		int nId = lParam;
		int nIndex = GetItemIndexById(nId);
		if(m_nSelectIndex != nIndex)
		{
			if(m_nSelectIndex >= 0 && m_nSelectIndex < m_ltItems.GetCount())
			{
				m_ltItems[m_nSelectIndex]->SetSelected(false);
			}
			m_nSelectIndex = nIndex;
			if(m_nSelectIndex >= 0 && m_nSelectIndex < m_ltItems.GetCount())
			{
				CString strPath = m_ltItems[m_nSelectIndex]->GetButtonImgPath();
				::SendMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKBUTTONEX_MSG_WPARAM_SELECTED, (LPARAM)&strPath);
			}
		}
		return 1;
	}else if(LKBUTTONEX_MSG_WPARAM_DELETE == wParam)
	{
		int nId = lParam;
		int nIndex = GetItemIndexById(nId);
		if(nIndex >= 0)
		{
            DeleteItemParam para;
            para.nIndex = nIndex;
            para.bDel = false;
			::SendMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKBUTTONEX_MSG_WPARAM_DELETE, (LPARAM)&para);
            if(para.bDel)
            {
			    CLKButtonEx *pButton = m_ltItems[nIndex];
			    m_ltItems.RemoveAt(nIndex);
			    pButton->DestroyWindow();
			    delete pButton;
			    CalcMaxVisibleCount();
			    AdjustButton();
			    if(m_nSelectIndex > nIndex)
			    {
				    m_nSelectIndex--;
			    }else if(m_nSelectIndex == nIndex)
			    {
				    m_nSelectIndex = -1;
			    }
			    Invalidate(false);
            }
		}
		return 1;
	}
	return CLKCtrlBaseEx::OnCommonMsg(wParam, lParam);
}



// ˢ��ǰ������
void CLKBtnListBox::ReSetMem()
{
	// �����������
	CalcMaxColumn();
	// �������ɼ�����
	CalcMaxVisibleCount();
	// ������ťλ��
	AdjustButton();
}

// ��ȡ��������
void CLKBtnListBox::GetDataRect(CRect &rt)
{
	rt = m_rtClient;
	if(IsVScrollbarValid())
	{
		rt.right -= GetVScrollbarWidth();
	}
}

// �����������
void CLKBtnListBox::CalcMaxColumn()
{
	CRect rt;
	GetDataRect(rt);
	m_nMaxCol = rt.Width() / m_nItemWidth;
}

// �������ɼ�����
void CLKBtnListBox::CalcMaxVisibleCount()
{
	CRect rt;
	GetDataRect(rt);
	int m = rt.Height() % m_nItemHeight;
	//if(m)
	//{
	//	m_nMaxVisibleCount = (rt.Height() / m_nItemHeight + 1) * m_nMaxCol;
	//}else
	//{
		m_nMaxVisibleCount = (rt.Height() / m_nItemHeight) * m_nMaxCol;
	//}

	if(m_nMaxVisibleCount > m_ltItems.GetCount() - m_nVisibleIndex)
	{
		m_nMaxVisibleCount = m_ltItems.GetCount() - m_nVisibleIndex;
	}
}

// ������ťλ��
void CLKBtnListBox::AdjustButton(bool bAdjustVScrollbar)
{
	int n = 0;
	int nCol(0);
	int nRow(0);
	//HDWP h = BeginDeferWindowPos(m_ltItems.GetCount());
	for(int i = 0; i < m_ltItems.GetCount(); i++)
	{
		CLKButtonEx *pItem = m_ltItems[i];
		if(i < m_nVisibleIndex || i >= m_nVisibleIndex + m_nMaxVisibleCount)
		{
			pItem->ShowWindow(SW_HIDE);
			continue;
		}
		pItem->SetWindowPos(0, nCol * m_nItemWidth, nRow * m_nItemHeight, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
		//h = DeferWindowPos(h, pItem->GetSafeHwnd(), 0, nCol * m_nItemWidth, nRow * m_nItemHeight, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
		n++;
		nRow = n / m_nMaxCol;
		nCol = n % m_nMaxCol;
	}
	//EndDeferWindowPos(h);
	if(!bAdjustVScrollbar) return;
	int nMaxRow = GetMaxRow();
	if(GetVisibleRow() < nMaxRow)
	{
		SetVScrollbarVisible();
		SetVScrollbarRang(nMaxRow, GetRowsHeight(), true);
	}else
	{
		SetVScrollbarVisible(false);
	}

}

// ��ȡָ�������������
CRect CLKBtnListBox::GetRectByIndex(int nIndex)
{
	CRect rt(0, 0, 0, 0);
	if(nIndex >= m_nVisibleIndex && nIndex < m_nVisibleIndex + m_nMaxVisibleCount)
	{
		int nRow = nIndex / m_nMaxCol;
		int nCol = nIndex % m_nMaxCol;

		rt = CRect(nCol * m_nItemWidth, nRow * m_nItemHeight, (nCol + 1) * m_nItemWidth, (nRow + 1) * m_nItemHeight);
	}
	return rt;
}

// ������ɣĻ�ȡָ��������
int CLKBtnListBox::GetItemIndexById(int nId)
{
	for(int i = 0; i < m_ltItems.GetCount(); i++)
	{
		if(m_ltItems[i]->GetDlgCtrlID() == nId)
		{
			return i;
		}
	}
	return -1;
}

// ������������ȡָ����ɣ�
int CLKBtnListBox::GetItemIdByIndex(int nIndex)
{
	if(nIndex >= 0 && nIndex < m_ltItems.GetCount())
	{
		return m_ltItems[nIndex]->GetDlgCtrlID();
	}
	return -1;
}

// ��ȡ�������
int CLKBtnListBox::GetMaxRow()
{
	int nCol = (0 == m_nMaxCol ? 1 : m_nMaxCol);
	int nRow = m_ltItems.GetCount() / nCol;
	if(m_ltItems.GetCount() % nCol) nRow++;
	return nRow;
}

// ��ȡ�ɼ�����
int CLKBtnListBox::GetVisibleRow()
{
	int nCol = (0 == m_nMaxCol ? 1 : m_nMaxCol);
	int nRow = m_nMaxVisibleCount / nCol;
	if(m_nMaxVisibleCount % nCol) nRow++;
	return nRow;
}

// ��ȡ�����и�
int CLKBtnListBox::GetRowsHeight()
{
	return m_nItemHeight * GetMaxRow();
}

// ���ѡ��״̬
void CLKBtnListBox::ClearSelectItem()
{
	if(m_nSelectIndex >= 0 && m_nSelectIndex < m_ltItems.GetCount())
	{
		m_ltItems[m_nSelectIndex]->SetSelected(false);
	}
	m_nSelectIndex = -1;
}

// ���õ�ǰѡ����
void CLKBtnListBox::SetSelectItem(int nIndex)
{
    if(m_nSelectIndex != nIndex)
    {
	    if(m_nSelectIndex >= 0 && m_nSelectIndex < m_ltItems.GetCount())
	    {
		    m_ltItems[m_nSelectIndex]->SetSelected(false);
	    }
        m_nSelectIndex = nIndex;
	    if(m_nSelectIndex >= 0 && m_nSelectIndex < m_ltItems.GetCount())
	    {
		    m_ltItems[m_nSelectIndex]->SetSelected();
            //if(m_ltItems[m_nSelectIndex]->IsWindowVisible())
            //{
            //    m_ltItems[m_nSelectIndex]->Invalidate();
            //}
	    }
    }
}
void CLKBtnListBox::SetSelectItem(CString &strFileName)
{
    for(int i = 0; i < m_ltItems.GetCount(); i++)
    {
        if(strFileName.Compare(m_ltItems[i]->GetButtonImgPath()) == 0)
        {
            SetSelectItem(i);
            return;
        }
    }
}

//���Ÿ���
void StretchBltFast(CImage* pDest, int xDest, int yDest, int cxDest, int cyDest, 
    CImage* pSrc, int xSrc, int ySrc, int cxSrc, int cySrc)
{
    int i,j,k;
    LPBYTE pBitsSrc = (LPBYTE)(pSrc->GetBits()); //���ݿ���ʼλ��
    LPBYTE pBitsDest = (LPBYTE)(pDest->GetBits());//���ݿ���ʼλ��
    LPBYTE pixAddrSrc = pBitsSrc;
    LPBYTE pixAddrDest = pBitsDest;

    int strideSrc = pSrc->GetPitch(); //pitch��ʱΪ��
    int strideDest = pDest->GetPitch();
    int bytesPerPixelSrc = pSrc->GetBPP()/8;
    int bytesPerPixelDest = pDest->GetBPP()/8;
    
    for (j = 0; j < cyDest; j++)
    {
        for (i = 0; i < cxDest; i++)
        {
            pixAddrSrc = pBitsSrc + (j * cySrc / cyDest) * strideSrc + (i * cxSrc / cxDest) *  bytesPerPixelSrc;
            pixAddrDest = pBitsDest + strideDest * j  + i  * bytesPerPixelDest;

            //���Ƶ�ǰ����
            for (k = 0; k < bytesPerPixelDest; k++, pixAddrDest++)
            {
                *pixAddrDest = *pixAddrSrc;

                //�Ƿ�����ƶ�����һ��ͨ����
                if(k < bytesPerPixelSrc - 1) pixAddrSrc++;
            }
        }
    }
}


			//const int oldx = GetWidth();
			//const int oldy = GetHeight();

			//int newx = pImg->GetWidth();
			//int newy = pImg->GetHeight();
			//int accuCellSize = 4;




			//unsigned int *accu = new unsigned int[newx*accuCellSize];      //array for suming pixels... one pixel for every destination column
			//unsigned int *accuPtr;                              //pointer for walking through accu
			////each cell consists of blue, red, green component and count of pixels summed in this cell
			//memset(accu, 0, newx * accuCellSize * sizeof(unsigned int));  //clear accu

			//if (!IsIndexed()) 
			//{
			//	//RGB24 version with pointers
			//	BYTE *destPtr, *srcPtr, *destPtrS, *srcPtrS;        //destination and source pixel, and beginnings of current row
			//	srcPtrS=(BYTE*)GetBits();
			//	srcPtrS=(BYTE*)GetPixelAddress(0, 0);
			//	destPtrS=(BYTE*)pImg->GetBits();
			//	int dwSrcEffWidth = (BYTE *)GetPixelAddress(0, 1) - (BYTE *)GetPixelAddress(0, 0);
			//	int dwDestEffWidth = (BYTE *)pImg->GetPixelAddress(0, 1) - (BYTE *)pImg->GetPixelAddress(0, 0);

			//	int ex=0, ey=0;                                               //ex and ey replace division... 
			//	int dy=0;
			//	//(we just add pixels, until by adding newx or newy we get a number greater than old size... then
			//	// it's time to move to next pixel)
		 //       
			//	for(int y=0; y<oldy; y++)
			//	{                                    //for all source rows
			//		//nProgress = (long)(100*y/oldy); if (nEscape) break;
			//		ey += newy;                                                   
			//		ex = 0;                                                       //restart with ex = 0
			//		accuPtr=accu;                                                 //restart from beginning of accu
			//		srcPtr=srcPtrS;                                               //and from new source line

			//		for(int x=0; x<oldx; x++){                                    //for all source columns
			//			ex += newx;
			//			*accuPtr     += *(srcPtr++);                                  //add current pixel to current accu slot
			//			*(accuPtr+1) += *(srcPtr++);
			//			*(accuPtr+2) += *(srcPtr++);
			//			(*(accuPtr+3)) ++;
			//			if (ex>oldx) {                                                //when we reach oldx, it's time to move to new slot
			//				accuPtr += accuCellSize;
			//				ex -= oldx;                                                   //(substract oldx from ex and resume from there on)
			//			}//if (ex overflow)
			//		}//for x

			//		if (ey>=oldy)                                                  //now when this happens
			//		{	
			//			ey -= oldy;                                                     //it's time to move to new destination row
			//			destPtr = destPtrS;                                             //reset pointers to proper initial values
			//			accuPtr = accu;
			//			for (int k=0; k<newx; k++)                                     //copy accu to destination row (divided by number of pixels in each slot)
			//			{	
			//				int xx = *(accuPtr+3);
			//				*(destPtr++) = (BYTE)(*(accuPtr) / (xx ? xx : 1));
			//				*(destPtr++) = (BYTE)(*(accuPtr+1) / (xx ? xx : 1));
			//				*(destPtr++) = (BYTE)(*(accuPtr+2) / (xx ? xx : 1));
			//				accuPtr += accuCellSize;
			//			}//for k
			//			memset(accu, 0, newx * accuCellSize * sizeof(unsigned int));                   //clear accu
			//			destPtrS += dwDestEffWidth;
			//		}//if (ey overflow)

			//		srcPtrS += dwSrcEffWidth;                                     //next round we start from new source row
			//	}//for y
			//}else
			//{
			//	//RGB24 version with pointers
			//	int ex=0, ey=0;                                               //ex and ey replace division... 
			//	int dy=0;
			//	//(we just add pixels, until by adding newx or newy we get a number greater than old size... then
			//	// it's time to move to next pixel)
			//	RGBQUAD rgb;
			//	      
			//	for(int y=0; y<oldy; y++){                                    //for all source rows
			//		//info.nProgress = (long)(100*y/oldy); if (info.nEscape) break;
			//		ey += newy;                                                   
			//		ex = 0;                                                       //restart with ex = 0
			//		accuPtr=accu;                                                 //restart from beginning of accu
			//		for(int x=0; x<oldx; x++){                                    //for all source columns
			//			ex += newx;
			//			COLORREF r = GetPixel(x, y);
			//			rgb.rgbRed = GetRValue(r);
			//			rgb.rgbGreen = GetGValue(r);
			//			rgb.rgbBlue = GetBValue(r);

			//			*accuPtr     += rgb.rgbBlue;                                  //add current pixel to current accu slot
			//			*(accuPtr+1) += rgb.rgbRed;
			//			*(accuPtr+2) += rgb.rgbGreen;
			//			(*(accuPtr+3)) ++;
			//			if (ex>oldx) {                                                //when we reach oldx, it's time to move to new slot
			//				accuPtr += accuCellSize;
			//				ex -= oldx;                                                   //(substract oldx from ex and resume from there on)
			//			}//if (ex overflow)
			//		}//for x

			//		if (ey>=oldy) {                                                 //now when this happens
			//			ey -= oldy;                                                     //it's time to move to new destination row
			//			accuPtr = accu;
			//			for (int dx=0; dx<newx; dx++)                                  //copy accu to destination row (divided by number of pixels in each slot)
			//			{	
			//				int xx = *(accuPtr+3);
			//				rgb.rgbBlue = (BYTE)(*(accuPtr) / (xx ? xx : 1));
			//				rgb.rgbRed  = (BYTE)(*(accuPtr+1) / (xx ? xx : 1));
			//				rgb.rgbGreen= (BYTE)(*(accuPtr+2) / (xx ? xx : 1));
			//				//newImage.SetPixelColor(dx, dy, rgb, pAlpha!=0);
			//				pImg->SetPixelRGB(dx, dy, rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
			//				accuPtr += accuCellSize;
			//			}//for dx
			//			memset(accu, 0, newx * accuCellSize * sizeof(unsigned int));                   //clear accu
			//			dy++;
			//		}//if (ey overflow)
			//	}//for y
			//}

			//delete [] accu;                                                 //delete helper array



