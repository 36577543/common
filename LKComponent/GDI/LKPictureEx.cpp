#include "LKPictureEx.h"
#include <process.h>
#include "LKImage.h"

CLKPictureEx::CLKPictureEx(void)
// 父窗口
: m_pParent(0)
// 在父窗口中的显示区域
, m_rtImage(0, 0, 0, 0)
, m_hBitmap(0)
, m_hMemDC(0)
, m_hDispMemDC(0)
, m_hDispMemBM(0)
, m_hDispOldBM(0)
, m_hThread(0)
, m_bExitThread(0)
//, m_nTimer(0)
//, m_nTFrameIndex(0)
{
	m_hExitEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
}

CLKPictureEx::~CLKPictureEx(void)
{
    UnLoad();
	CloseHandle(m_hExitEvent);
}

bool CLKPictureEx::Create(CWnd *pWnd, CRect rt)
{
    m_pParent = pWnd;
    m_rtImage = rt;
    return true;
}

// 播放
void CLKPictureEx::Play()
{
	if (!m_bIsInitialized || m_hThread)
	{
		//TRACE(_T("Call one of the CLKPicture::Load() member functions before calling Draw()\n"));
		return;
	}

	if (IsAnimatedGIF())
	{
		unsigned int nDummy;
		m_hThread = (HANDLE) _beginthreadex(NULL,0,_ThreadAnimation,this, CREATE_SUSPENDED,&nDummy);
		if (m_hThread)
        {
			ResumeThread(m_hThread);
        }
	//} 
	//else
	//{
	//	if (m_pPicture)
	//	{
	//		long hmWidth;
	//		long hmHeight;
	//		m_pPicture->get_Width(&hmWidth);
	//		m_pPicture->get_Height(&hmHeight);
	//		if (m_pPicture->Render(m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, 0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
	//		{
	//			Invalidate(false);
	//			return true;
	//		}
	//	}
	}

}

// 显示一帧静态图片
void CLKPictureEx::Draw(int nFrameIndex)
{
	if (!m_bIsInitialized || m_hThread)
	{
		//TRACE(_T("Call one of the CLKPicture::Load() member functions before calling Draw()\n"));
		return;
	}

	if (IsAnimatedGIF())
	{
		// 画帧
		DrawFrame(nFrameIndex);
	} 
	else
	{
		if (m_pPicture)
		{
			long hmWidth;
			long hmHeight;
			m_pPicture->get_Width(&hmWidth);
			m_pPicture->get_Height(&hmHeight);
			if (m_pPicture->Render(m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, 0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
			{
				if(m_pParent && m_pParent->GetSafeHwnd())
				{
					m_pParent->InvalidateRect(m_rtImage, false);
				}
			}
		}
	}
}

// 更新动画到指定DC上
void CLKPictureEx::UpdateDC(CDC *pDC)
{
	LONG nPaintWidth = m_PaintRect.right-m_PaintRect.left;
    int nLeft = m_rtImage.left;
    int nTop = m_rtImage.top;

	if (nPaintWidth > 0)
	{
		LONG nPaintHeight = m_PaintRect.bottom - m_PaintRect.top;
		::BitBlt(pDC->m_hDC, m_rtImage.left, m_rtImage.top, nPaintWidth, nPaintHeight,	m_hMemDC, m_PaintRect.left, m_PaintRect.top, SRCCOPY);
	}
	else
	{
        ::BitBlt(pDC->m_hDC, m_rtImage.left, m_rtImage.top, m_PictureSize.cx, m_PictureSize.cy, m_hMemDC, 0, 0, SRCCOPY);
	}
}

void CLKPictureEx::SetBkColor(COLORREF clr)
{
	if (!m_bIsInitialized) return;

	m_clrBackground = clr;

	HBRUSH hBrush = CreateSolidBrush(clr);
	if (hBrush)
	{
		RECT rect = {0,0,m_PictureSize.cx,m_PictureSize.cy};
		FillRect(m_hMemDC,&rect,hBrush);
		DeleteObject(hBrush);
	}
}


void CLKPictureEx::UnLoad()
{
	Stop();	
	if (m_hMemDC)
	{
		SelectObject(m_hMemDC,m_hOldBitmap);
		::DeleteDC(m_hMemDC);
		::DeleteObject(m_hBitmap);
		m_hMemDC  = NULL;
		m_hBitmap = NULL;
	}

	if (m_hDispMemDC)
	{
		SelectObject(m_hDispMemDC,m_hDispOldBM);
		::DeleteDC(m_hDispMemDC);
		::DeleteObject(m_hDispMemBM);
		m_hDispMemDC  = NULL;
		m_hDispMemBM = NULL;
	}
	m_hThread		   = NULL;
    CLKPicture::UnLoad();
}

// 计算图片对应的像素宽度
void CLKPictureEx::CalcImageWidth(int hmWidth, int hmHeight)
{
    if(m_pParent && m_pParent->GetSafeHwnd()/* && !m_rtImage.IsRectEmpty()*/)
    {
	    HDC hDC = ::GetDC(m_pParent->GetSafeHwnd());        
	    m_PictureSize.cx = MulDiv(hmWidth, GetDeviceCaps(hDC,LOGPIXELSX), 2540);
	    m_PictureSize.cy = MulDiv(hmHeight, GetDeviceCaps(hDC,LOGPIXELSY), 2540);
	    ::ReleaseDC(m_pParent->GetSafeHwnd(),hDC);
    }
}

bool CLKPictureEx::PrepareDC(int nWidth, int nHeight)
{
	//SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE | SWP_NOZORDER);

    if(!m_pParent || !m_pParent->GetSafeHwnd()) return false;
	HDC hWinDC = ::GetDC(m_pParent->GetSafeHwnd());
	if (!hWinDC) return FALSE;
	
	m_hMemDC = CreateCompatibleDC(hWinDC);
	if (!m_hMemDC) 
	{
		::ReleaseDC(m_pParent->GetSafeHwnd(), hWinDC);
		return FALSE;
	}

	m_hBitmap  = CreateCompatibleBitmap(hWinDC,nWidth,nHeight);
	if (!m_hBitmap) 
	{
		::ReleaseDC(m_pParent->GetSafeHwnd(), hWinDC);
		::DeleteDC(m_hMemDC);
		return FALSE;
	}

	m_hOldBitmap = reinterpret_cast<HBITMAP>(SelectObject(m_hMemDC,m_hBitmap));
	
	// fill the background
	//m_clrBackground = GetSysColor(COLOR_3DFACE);
	m_clrBackground = RGB(255, 255, 255);
	RECT rect = {0,0,nWidth,nHeight};
	FillRect(m_hMemDC,&rect,(HBRUSH)(COLOR_WINDOW));

	::ReleaseDC(m_pParent->GetSafeHwnd(),hWinDC);
	m_bIsInitialized = TRUE;
	return TRUE;
}

bool CLKPictureEx::InnerLoad(HGLOBAL hGlobal, DWORD dwSize)
{
    bool bRet = CLKPicture::InnerLoad(hGlobal, dwSize);
    if(bRet)
    {
        bRet = PrepareDC(m_PictureSize.cx,m_PictureSize.cy);
    }
    return bRet;
}

UINT WINAPI CLKPictureEx::_ThreadAnimation(LPVOID pParam)
{
	ASSERT(pParam);
	CLKPictureEx *pPic = reinterpret_cast<CLKPictureEx *> (pParam);

	pPic->m_bIsPlaying = TRUE;
	pPic->ThreadAnimation();
	pPic->m_bIsPlaying = FALSE;

	// this thread has finished its work so we close the handle
	CloseHandle(pPic->m_hThread); 
	// and init the handle to zero (so that Stop() doesn't Wait on it)
	pPic->m_hThread = 0;
	return 0;
}

void CLKPictureEx::ThreadAnimation()
{
	// first, restore background (for stop/draw support)
	// disposal method #2
	if (m_arrFrames[m_nCurrFrame].m_nDisposal == 2)
	{
		HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
		if (hBrush)
		{
			RECT rect = {
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx + m_arrFrames[m_nCurrFrame].m_frameSize.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy + m_arrFrames[m_nCurrFrame].m_frameSize.cy };
			FillRect(m_hMemDC,&rect,hBrush);
            //::DrawText(m_hMemDC, _T("HHHHHHHHHHHHHHHHHHHHHHHHHHHHH"), 20, &rect, DT_RIGHT | DT_SINGLELINE);
			DeleteObject(hBrush);
		}
	}else
	{
		// disposal method #3
		if (m_hDispMemDC && (m_arrFrames[m_nCurrFrame].m_nDisposal == 3) )
		{
			// put it back
			BitBlt(m_hMemDC,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
				m_arrFrames[m_nCurrFrame].m_frameSize.cx,
				m_arrFrames[m_nCurrFrame].m_frameSize.cy,
				m_hDispMemDC,0,0, SRCCOPY);
			// init variables
			SelectObject(m_hDispMemDC,m_hDispOldBM);
			DeleteDC(m_hDispMemDC); m_hDispMemDC = NULL;
			DeleteObject(m_hDispMemBM); m_hDispMemBM = NULL;
		}
	}

	while (!m_bExitThread)
	{
		if (m_arrFrames[m_nCurrFrame].m_pPicture)
		{
		///////////////////////////////////////////////////////
		// Before rendering a frame we should take care of what's 
		// behind that frame. TFrame::m_nDisposal will be our guide:
		//   0 - no disposal specified (do nothing)
		//   1 - do not dispose (again, do nothing)
		//   2 - restore to background color (m_clrBackground)
		//   3 - restore to previous

			//////// disposal method #3
			if (m_arrFrames[m_nCurrFrame].m_nDisposal == 3)
			{
				// prepare a memory DC and store the background in it
				m_hDispMemDC = CreateCompatibleDC(m_hMemDC);
				m_hDispMemBM = CreateCompatibleBitmap(m_hMemDC,
							m_arrFrames[m_nCurrFrame].m_frameSize.cx,
							m_arrFrames[m_nCurrFrame].m_frameSize.cy);
				
				if (m_hDispMemDC && m_hDispMemBM)
				{
					m_hDispOldBM = reinterpret_cast<HBITMAP> (SelectObject(m_hDispMemDC,m_hDispMemBM));
					BitBlt(m_hDispMemDC,0,0,
						m_arrFrames[m_nCurrFrame].m_frameSize.cx,
						m_arrFrames[m_nCurrFrame].m_frameSize.cy,
						m_hMemDC,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
						SRCCOPY);
				}
			}
			///////////////////////

			long hmWidth;
			long hmHeight;
			m_arrFrames[m_nCurrFrame].m_pPicture->get_Width(&hmWidth);
			m_arrFrames[m_nCurrFrame].m_pPicture->get_Height(&hmHeight);

			if (m_arrFrames[m_nCurrFrame].m_pPicture->Render(m_hMemDC, 
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx, 
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy, 
				m_arrFrames[m_nCurrFrame].m_frameSize.cx, 
				m_arrFrames[m_nCurrFrame].m_frameSize.cy, 
				0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
			{
                m_pParent->InvalidateRect(&m_rtImage, false);
				//Invalidate(FALSE);
			}
			
			if (m_bExitThread) break;

			// if the delay time is too short (like in old GIFs), wait for 100ms
			if (m_arrFrames[m_nCurrFrame].m_nDelay < 5) 
				WaitForSingleObject(m_hExitEvent, 100);
			else
				WaitForSingleObject(m_hExitEvent, 10*m_arrFrames[m_nCurrFrame].m_nDelay);

			if (m_bExitThread) break;

			// disposal method #2
			if (m_arrFrames[m_nCurrFrame].m_nDisposal == 2)
			{
				HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
				if (hBrush)
				{
					RECT rect = {
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx + m_arrFrames[m_nCurrFrame].m_frameSize.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy + m_arrFrames[m_nCurrFrame].m_frameSize.cy };
					FillRect(m_hMemDC,&rect,hBrush);
					DeleteObject(hBrush);
				}
			} 
			else
				if (m_hDispMemDC && (m_arrFrames[m_nCurrFrame].m_nDisposal == 3) )
				{
					// put it back
					BitBlt(m_hMemDC,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
						m_arrFrames[m_nCurrFrame].m_frameSize.cx,
						m_arrFrames[m_nCurrFrame].m_frameSize.cy,
						m_hDispMemDC,0,0, SRCCOPY);
					// init variables
					SelectObject(m_hDispMemDC,m_hDispOldBM);
					DeleteDC(m_hDispMemDC); m_hDispMemDC = NULL;
					DeleteObject(m_hDispMemBM); m_hDispMemBM = NULL;
				}
		}
		m_nCurrFrame++;
		if (m_nCurrFrame == m_arrFrames.size())
		{
			m_nCurrFrame
				= 0; 
		// init the screen for the first frame,
			HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
			if (hBrush)
			{
				RECT rect = {0,0,m_PictureSize.cx,m_PictureSize.cy};
				FillRect(m_hMemDC,&rect,hBrush);
				DeleteObject(hBrush);
			}
		}
        //break;
	};
}

void CLKPictureEx::Stop(bool bInit)
{
	m_bIsPlaying = FALSE;
	m_bExitThread = TRUE;
	SetEvent(m_hExitEvent);
	if (m_hThread)
	{
		// we'll wait for 5 seconds then continue execution
		DWORD dwFlag = WaitForSingleObject(m_hThread,5000);
        if(WAIT_TIMEOUT == dwFlag)
        {
		    CloseHandle(m_hThread);
		    m_hThread = NULL;
        }
	}

	// make it possible to Draw() again
	ResetEvent(m_hExitEvent);
	m_bExitThread = FALSE;
	if(bInit) Draw();
}

// 画帧
void CLKPictureEx::DrawFrame(int nFrameIndex)
{
	if(nFrameIndex < 0 || nFrameIndex >= m_arrFrames.size()) return;
	HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
	if (hBrush)
	{
		RECT rect = {
			m_arrFrames[nFrameIndex].m_frameOffset.cx,
			m_arrFrames[nFrameIndex].m_frameOffset.cy,
			m_arrFrames[nFrameIndex].m_frameOffset.cx + m_arrFrames[nFrameIndex].m_frameSize.cx,
			m_arrFrames[nFrameIndex].m_frameOffset.cy + m_arrFrames[nFrameIndex].m_frameSize.cy };
		FillRect(m_hMemDC,&rect,hBrush);
        //::DrawText(m_hMemDC, _T("HHHHHHHHHHHHHHHHHHHHHHHHHHHHH"), 20, &rect, DT_RIGHT | DT_SINGLELINE);
		DeleteObject(hBrush);
	}
	if (m_arrFrames[nFrameIndex].m_pPicture)
	{
		///////////////////////////////////////////////////////
		// Before rendering a frame we should take care of what's 
		// behind that frame. TFrame::m_nDisposal will be our guide:
		//   0 - no disposal specified (do nothing)
		//   1 - do not dispose (again, do nothing)
		//   2 - restore to background color (m_clrBackground)
		//   3 - restore to previous

		//////// disposal method #3
		if (m_arrFrames[nFrameIndex].m_nDisposal == 3)
		{
			// prepare a memory DC and store the background in it
			m_hDispMemDC = CreateCompatibleDC(m_hMemDC);
			m_hDispMemBM = CreateCompatibleBitmap(m_hMemDC,
						m_arrFrames[nFrameIndex].m_frameSize.cx,
						m_arrFrames[nFrameIndex].m_frameSize.cy);
			
			if (m_hDispMemDC && m_hDispMemBM)
			{
				m_hDispOldBM = reinterpret_cast<HBITMAP> (SelectObject(m_hDispMemDC,m_hDispMemBM));
				BitBlt(m_hDispMemDC,0,0,
					m_arrFrames[nFrameIndex].m_frameSize.cx,
					m_arrFrames[nFrameIndex].m_frameSize.cy,
					m_hMemDC,
					m_arrFrames[nFrameIndex].m_frameOffset.cx,
					m_arrFrames[nFrameIndex].m_frameOffset.cy,
					SRCCOPY);
			}
		}
		///////////////////////

		long hmWidth;
		long hmHeight;
		m_arrFrames[nFrameIndex].m_pPicture->get_Width(&hmWidth);
		m_arrFrames[nFrameIndex].m_pPicture->get_Height(&hmHeight);

		if (m_arrFrames[nFrameIndex].m_pPicture->Render(m_hMemDC, 
			m_arrFrames[nFrameIndex].m_frameOffset.cx, 
			m_arrFrames[nFrameIndex].m_frameOffset.cy, 
			m_arrFrames[nFrameIndex].m_frameSize.cx, 
			m_arrFrames[nFrameIndex].m_frameSize.cy, 
			0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
		{
            m_pParent->InvalidateRect(&m_rtImage, false);
			//TRACE(_T("nFrameIndex2 %d: x-%d,y-%d,w-%d,h-%d\r\n"), nFrameIndex, m_rtImage.left, m_rtImage.top, m_rtImage.Width(), m_rtImage.Height());
		}
			
	}
}

// 更新动画到指定DC,指定区域上
void CLKPictureEx::UpdateDC(CWnd *pWnd, CDC *pDC, const CRect &rt, int nFrameIndex)
{
	if (IsAnimatedGIF())
	{
		// 画帧
		//int nIndex = m_nTFrameIndex;
		//if(m_arrFrames.size() > m_nTFrameIndex && m_nTFrameIndex >= 0)
		//{
		//	int n1 = GetTickCount();
		//	int n2 = 10 * m_arrFrames[m_nTFrameIndex].m_nDelay;
		//	if(n1 - m_nTimer > n2)
		//	{
		//		m_nTFrameIndex++;
		//		m_nTimer = n1;
		//	}
		//}
		//if(m_nTFrameIndex >= m_arrFrames.size()) m_nTFrameIndex = 0;
		//DrawFrameToDC(pWnd, pDC, rt, nIndex, nFrameIndex);
		DrawFrameToDC(pWnd, pDC, rt, nFrameIndex);
	}else
	{
		if (m_pPicture)
		{
			long hmWidth;
			long hmHeight;
			m_pPicture->get_Width(&hmWidth);
			m_pPicture->get_Height(&hmHeight);
			m_pPicture->Render(pDC->GetSafeHdc(), rt.left, rt.top, m_PictureSize.cx, m_PictureSize.cy, 0, hmHeight, hmWidth, -hmHeight, NULL);
		}
	}
}


// 画帧
void CLKPictureEx::DrawFrameToDC(CWnd *pWnd, CDC *pDC, const CRect &rt, int nFrameIndex)
{
	if(!pWnd || !pDC || nFrameIndex < 0 || nFrameIndex >= m_arrFrames.size()) return;

	CDC memDC;
	if(!memDC.CreateCompatibleDC(pDC)) return;
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rt.Width(), rt.Height());
	CBitmap *pOld = memDC.SelectObject(&bmp);
	memDC.BitBlt(0, 0, rt.Width(), rt.Height(), pDC, rt.left, rt.top, SRCCOPY);
    //CFont *pOldFont = memDC.SelectObject(pFont);
	//DrawText(memDC, strText, rtText, uFormat, crText);
    //memDC.SelectObject(pOldFont);
	//TRACE(_T("l = %d, t = %d, r = %d, b = %d\r\n"), rt.left, rt.top, rt.right, rt.bottom);
	//TRACE(_T("pDC = %d, pDC->GetSafeHdc() = %d\r\n"), pDC, pDC->GetSafeHdc());
	TFrame &fm = m_arrFrames[nFrameIndex];
	if (fm.m_pPicture)
	{
		//1、Unspecified–不定义任何处理方式，由Fireworks自己来确定每一帧采用何种过渡方式。选择Unspecified选项有可能创建最小尺寸的动画GIF文件。
		//2、None–不进行处理。选择此方式时，下一帧的内容会直接显示在当前帧的内容上面。也就是说Fireworks会根据上一帧的不同内容，将少量的不同像素增加到上一帧。
		//3、Restore to Background–恢复背景色。此选项会首先擦除当前帧的图像内容，然后恢复到背景色或背景图案（如果背景是透明的，就恢复成透明），接下来在显示下一帧的内容。此选项常用在背景透明的动画上。
		//4、Restore to Previous–恢复上一帧。此选项会擦除当前帧的内容，然后恢复上一帧的内容，接下来在显示下一帧的内容。		if (m_arrFrames[nFrameIndex].m_nDisposal == 3)
		// Before rendering a frame we should take care of what's 
		// behind that frame. TFrame::m_nDisposal will be our guide:
		//   0 - no disposal specified (do nothing)
		//   1 - do not dispose (again, do nothing)
		//   2 - restore to background color (m_clrBackground)
		//   3 - restore to previous
		int nOldIndex = (nFrameIndex + m_arrFrames.size() - 1) % m_arrFrames.size();
		TFrame &prefm = m_arrFrames[nOldIndex];
		// 擦除背景
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		// 获取背景
		if(pWnd)
		{
			//pWnd->SendMessage(
			pWnd->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
		}
		if(prm.pBG)
		{
			//
			CLKImage *pBG = (CLKImage *)prm.pBG;
			pBG->Draw(memDC.GetSafeHdc(), 0, 0, rt.Width(), rt.Height(), prm.rt.left + rt.left, prm.rt.top + rt.top, rt.Width(), rt.Height());
			//pBG->Draw(memDC.GetSafeHdc(), prm.rt.left + rt.left, prm.rt.top + rt.top);
			//memDC.MoveTo(rt.l
		}else
		{
			//擦除背景
			HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
			if (hBrush)
			{
				RECT rect = {
					fm.m_frameOffset.cx,
					fm.m_frameOffset.cy,
					fm.m_frameOffset.cx + fm.m_frameSize.cx,
					fm.m_frameOffset.cy + fm.m_frameSize.cy };
				FillRect(memDC.GetSafeHdc(), &rt, hBrush);
				DeleteObject(hBrush);
			}
		}

		// 画前面的图片
		int nOldIndex2(nFrameIndex);
		while(nOldIndex2 > 0)
		{
			int n = (nOldIndex2 + m_arrFrames.size() - 1) % m_arrFrames.size();
			if(m_arrFrames[n].m_nDisposal == 2)
			{
				break;
			}
			nOldIndex2 = n;
			// 防止无效图片死循环
			if(nOldIndex2 == nFrameIndex)
			{
				break;
			}
		}
		if(nOldIndex2 < nFrameIndex)
		{
			for(int i = nOldIndex2 ; i < nFrameIndex; i++)
			{
				long hmWidth2;
				long hmHeight2;
				m_arrFrames[i].m_pPicture->get_Width(&hmWidth2);
				m_arrFrames[i].m_pPicture->get_Height(&hmHeight2);

				if (m_arrFrames[i].m_pPicture->Render(memDC.GetSafeHdc(), 
					m_arrFrames[i].m_frameOffset.cx, 
					m_arrFrames[i].m_frameOffset.cy, 
					m_arrFrames[i].m_frameSize.cx, 
					m_arrFrames[i].m_frameSize.cy, 
					0, hmHeight2, hmWidth2, -hmHeight2, &rt) == S_OK)
				{
					//m_pParent->InvalidateRect(&m_rtImage, false);
					//TRACE(_T("nFrameIndex2 %d: x-%d,y-%d,w-%d,h-%d\r\n"), nFrameIndex, m_rtImage.left, m_rtImage.top, m_rtImage.Width(), m_rtImage.Height());
				}
			}
		}else if(nOldIndex2 > nFrameIndex)
		{
			// 画后面
			for(int i = nOldIndex2; i < m_arrFrames.size(); i++)
			{
				long hmWidth2;
				long hmHeight2;
				m_arrFrames[i].m_pPicture->get_Width(&hmWidth2);
				m_arrFrames[i].m_pPicture->get_Height(&hmHeight2);

				if (m_arrFrames[i].m_pPicture->Render(memDC.GetSafeHdc(), 
					m_arrFrames[i].m_frameOffset.cx, 
					m_arrFrames[i].m_frameOffset.cy, 
					m_arrFrames[i].m_frameSize.cx, 
					m_arrFrames[i].m_frameSize.cy, 
					0, hmHeight2, hmWidth2, -hmHeight2, &rt) == S_OK)
				{
					//m_pParent->InvalidateRect(&m_rtImage, false);
					//TRACE(_T("nFrameIndex2 %d: x-%d,y-%d,w-%d,h-%d\r\n"), nFrameIndex, m_rtImage.left, m_rtImage.top, m_rtImage.Width(), m_rtImage.Height());
				}
			}
			// 画前面
			for(int i = 0; i < nFrameIndex; i++)
			{
				long hmWidth2;
				long hmHeight2;
				m_arrFrames[i].m_pPicture->get_Width(&hmWidth2);
				m_arrFrames[i].m_pPicture->get_Height(&hmHeight2);

				if (m_arrFrames[i].m_pPicture->Render(memDC.GetSafeHdc(), 
					m_arrFrames[i].m_frameOffset.cx, 
					m_arrFrames[i].m_frameOffset.cy, 
					m_arrFrames[i].m_frameSize.cx, 
					m_arrFrames[i].m_frameSize.cy, 
					0, hmHeight2, hmWidth2, -hmHeight2, &rt) == S_OK)
				{
					//m_pParent->InvalidateRect(&m_rtImage, false);
					//TRACE(_T("nFrameIndex2 %d: x-%d,y-%d,w-%d,h-%d\r\n"), nFrameIndex, m_rtImage.left, m_rtImage.top, m_rtImage.Width(), m_rtImage.Height());
				}
			}
		}

		long hmWidth;
		long hmHeight;
		m_arrFrames[nFrameIndex].m_pPicture->get_Width(&hmWidth);
		m_arrFrames[nFrameIndex].m_pPicture->get_Height(&hmHeight);

		if (m_arrFrames[nFrameIndex].m_pPicture->Render(memDC.GetSafeHdc(), 
			m_arrFrames[nFrameIndex].m_frameOffset.cx, 
			m_arrFrames[nFrameIndex].m_frameOffset.cy, 
			m_arrFrames[nFrameIndex].m_frameSize.cx, 
			m_arrFrames[nFrameIndex].m_frameSize.cy, 
			0, hmHeight, hmWidth, -hmHeight, &rt) == S_OK)
		{
			//m_pParent->InvalidateRect(&m_rtImage, false);
			//TRACE(_T("nFrameIndex2 %d: x-%d,y-%d,w-%d,h-%d\r\n"), nFrameIndex, m_rtImage.left, m_rtImage.top, m_rtImage.Width(), m_rtImage.Height());
		}
			
	}
	pDC->BitBlt(rt.left, rt.top, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOld);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

// 是否可以刷新
bool CLKPictureEx::IsCanRedraw(int nTFrameIndex, DWORD nPreFrameTime)
{
	if (IsAnimatedGIF())
	{
		int nIndex = nTFrameIndex;
		if(m_arrFrames.size() > nIndex && nIndex >= 0)
		{
			int n1 = GetTickCount();
			int n2 = 10 * m_arrFrames[nIndex].m_nDelay;
			if(n1 - nPreFrameTime > n2)
			{
				return true;
			}
		}
	}
	return false;
}

// 获取下一帧图片
int CLKPictureEx::GetNextFrameIndex(int nTFrameIndex)
{
	if(0 == m_arrFrames.size()) return 0;
	return(nTFrameIndex + 1 + m_arrFrames.size()) % m_arrFrames.size();
}

//// 将下一帧图片作为当前图片
//int CLKPictureEx::NextFrameIndex()
//{
//	m_nTFrameIndex++;
//	if(m_nTFrameIndex >= m_arrFrames.size()) 
//	{
//		m_nTFrameIndex = 0;
//	}
//	m_nTimer = GetTickCount();
//	return m_nTFrameIndex;
//}
