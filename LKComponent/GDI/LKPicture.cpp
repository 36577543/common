#include "LKPicture.h"

//////////////////////////////////////////////////////////////////////
// Nested structures member functions
//////////////////////////////////////////////////////////////////////

inline int CLKPicture::TGIFControlExt::GetPackedValue(enum ControlExtValues Value)
{
	int nRet = (int)m_cPacked;
	switch (Value)
	{
	case GCX_PACKED_DISPOSAL:
		nRet = (nRet & 28) >> 2;
		break;

	case GCX_PACKED_USERINPUT:
		nRet = (nRet & 2) >> 1;
		break;

	case GCX_PACKED_TRANSPCOLOR:
		nRet &= 1;
		break;
	};

	return nRet;
}

inline int CLKPicture::TGIFLSDescriptor::GetPackedValue(enum LSDPackedValues Value)
{
	int nRet = (int)m_cPacked;

	switch (Value)
	{
	case LSD_PACKED_GLOBALCT:
		nRet = nRet >> 7;
		break;

	case LSD_PACKED_CRESOLUTION:
		nRet = ((nRet & 0x70) >> 4) + 1;
		break;

	case LSD_PACKED_SORT:
		nRet = (nRet & 8) >> 3;
		break;

	case LSD_PACKED_GLOBALCTSIZE:
		nRet &= 7;
		break;
	};

	return nRet;
}

inline int CLKPicture::TGIFImageDescriptor::GetPackedValue(enum IDPackedValues Value)
{
	int nRet = (int)m_cPacked;

	switch (Value)
	{
	case ID_PACKED_LOCALCT:
		nRet >>= 7;
		break;

	case ID_PACKED_INTERLACE:
		nRet = ((nRet & 0x40) >> 6);
		break;

	case ID_PACKED_SORT:
		nRet = (nRet & 0x20) >> 5;
		break;

	case ID_PACKED_LOCALCTSIZE:
		nRet &= 7;
		break;
	};

	return nRet;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLKPicture::CLKPicture(void)
{
	ASSERT(sizeof(TGIFImageDescriptor) == 10);
	ASSERT(sizeof(TGIFAppExtension)    == 14);
	ASSERT(sizeof(TGIFPlainTextExt)    == 15);
	ASSERT(sizeof(TGIFLSDescriptor)    ==  7);
	ASSERT(sizeof(TGIFControlExt)	   ==  8);
	ASSERT(sizeof(TGIFCommentExt)	   ==  2);
	ASSERT(sizeof(TGIFHeader)		   ==  6);

	m_pGIFLSDescriptor = NULL;
	m_pGIFHeader	   = NULL;
	m_pPicture		   = NULL;
	m_pRawData		   = NULL;

	m_bIsInitialized   = false;
	m_bIsPlaying       = false;
	m_bIsGIF		   = false;
	m_clrBackground    = RGB(255,255,255); // white by default
	m_nGlobalCTSize    = 0;
	m_nCurrOffset	   = 0;
	m_nCurrFrame	   = 0;
	m_nDataSize		   = 0;
	m_PictureSize.cx = m_PictureSize.cy = 0;
	SetRect(&m_PaintRect,0,0,0,0);
}

// 析构函数中调用虚函数有问题。
CLKPicture::~CLKPicture(void)
{
	//UnLoad();
}

void CLKPicture::UnLoad()
{
	if (m_pPicture)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
	};
	
	std::vector<TFrame>::iterator it;
	for (it=m_arrFrames.begin();it<m_arrFrames.end();it++)
		(*it).m_pPicture->Release();
	m_arrFrames.clear();

	//if (m_hMemDC)
	//{
	//	SelectObject(m_hMemDC,m_hOldBitmap);
	//	::DeleteDC(m_hMemDC);
	//	::DeleteObject(m_hBitmap);
	//	m_hMemDC  = NULL;
	//	m_hBitmap = NULL;
	//};

	//if (m_hDispMemDC)
	//{
	//	SelectObject(m_hDispMemDC,m_hDispOldBM);
	//	::DeleteDC(m_hDispMemDC);
	//	::DeleteObject(m_hDispMemBM);
	//	m_hDispMemDC  = NULL;
	//	m_hDispMemBM = NULL;
	//};

	SetRect(&m_PaintRect,0,0,0,0);
	m_pGIFLSDescriptor = NULL;
	m_pGIFHeader	   = NULL;
	m_pRawData		   = NULL;
	m_bIsInitialized   = false;
	m_bIsGIF		   = false;
	m_clrBackground    = RGB(255,255,255); // white by default
	m_nGlobalCTSize	   = 0;
	m_nCurrOffset	   = 0;
	m_nCurrFrame	   = 0;
	m_nDataSize		   = 0;
}

int CLKPicture::GetFrameCount() const
{
	if (!IsAnimatedGIF())
		return 0;

	return m_arrFrames.size();
}

bool CLKPicture::Load(LPCTSTR szFileName)
{
	ASSERT(szFileName);
	
	CFile file;
	HGLOBAL hGlobal;
	DWORD dwSize;

	if (!file.Open(szFileName,
				CFile::modeRead | 
				CFile::shareDenyWrite) )
	{
		TRACE(_T("Load (file): Error opening file %s\n"),szFileName);
		return false;
	};

	dwSize = file.GetLength();
	hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD,dwSize);
	if (!hGlobal)
	{
		TRACE(_T("Load (file): Error allocating memory\n"));
		return false;
	};
	
	char *pData = reinterpret_cast<char*>(GlobalLock(hGlobal));
	if (!pData)
	{
		TRACE(_T("Load (file): Error locking memory\n"));
		GlobalFree(hGlobal);
		return false;
	};

	TRY
	{
		file.Read(pData,dwSize);
	}
	CATCH(CFileException, e);                                          
	{
		TRACE(_T("Load (file): An exception occured while reading the file %s\n"),
			szFileName);
		GlobalFree(hGlobal);
		e->Delete();
		file.Close();
		return false;
	}
	END_CATCH
	GlobalUnlock(hGlobal);
	file.Close();

	bool bRetValue = InnerLoad(hGlobal,dwSize);
	GlobalFree(hGlobal);
	return bRetValue;
}

bool CLKPicture::Load(LPCTSTR szResourceName, LPCTSTR szResourceType)
{
	ASSERT(szResourceName);
	ASSERT(szResourceType);

	HRSRC hPicture = FindResource(AfxGetResourceHandle(),szResourceName,szResourceType);
	HGLOBAL hResData;
	if (!hPicture || !(hResData = LoadResource(AfxGetResourceHandle(),hPicture)))
	{
		TRACE(_T("Load (resource): Error loading resource %s\n"),szResourceName);
		return false;
	};
	DWORD dwSize = SizeofResource(AfxGetResourceHandle(),hPicture);

	// hResData is not the real HGLOBAL (we can't lock it)
	// let's make it real

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD,dwSize);
	if (!hGlobal)
	{
		TRACE(_T("Load (resource): Error allocating memory\n"));
		FreeResource(hResData);
		return false;
	};
	
	char *pDest = reinterpret_cast<char *> (GlobalLock(hGlobal));
	char *pSrc = reinterpret_cast<char *> (LockResource(hResData));
	if (!pSrc || !pDest)
	{
		TRACE(_T("Load (resource): Error locking memory\n"));
		GlobalFree(hGlobal);
		FreeResource(hResData);
		return false;
	};
	CopyMemory(pDest,pSrc,dwSize);
	FreeResource(hResData);
	GlobalUnlock(hGlobal);

	bool bRetValue = InnerLoad(hGlobal,dwSize);
	GlobalFree(hGlobal);
	return bRetValue;
}
int CLKPicture::GetNextBlockLen() const
{
	GIFBlockTypes nBlock = GetNextBlock();

	int nTmp;

	switch(nBlock)
	{
	case BLOCK_UNKNOWN:
		return -1;
		break;

	case BLOCK_TRAILER:
		return 1;
		break;

	case BLOCK_APPEXT:
		nTmp = GetSubBlocksLen(m_nCurrOffset+sizeof(TGIFAppExtension));
		if (nTmp > 0)
			return sizeof(TGIFAppExtension)+nTmp;
		break;

	case BLOCK_COMMEXT:
		nTmp = GetSubBlocksLen(m_nCurrOffset+sizeof(TGIFCommentExt));
		if (nTmp > 0)
			return sizeof(TGIFCommentExt)+nTmp;
		break;

	case BLOCK_CONTROLEXT:
		return sizeof(TGIFControlExt);
		break;

	case BLOCK_PLAINTEXT:
		nTmp = GetSubBlocksLen(m_nCurrOffset+sizeof(TGIFPlainTextExt));
		if (nTmp > 0)
			return sizeof(TGIFPlainTextExt)+nTmp;
		break;

	case BLOCK_IMAGE:
		TGIFImageDescriptor *pIDescr = 
			reinterpret_cast<TGIFImageDescriptor *> (&m_pRawData[m_nCurrOffset]);
		int nLCTSize = (int)
			(pIDescr->GetPackedValue(ID_PACKED_LOCALCT)*3*
			(1 << (pIDescr->GetPackedValue(ID_PACKED_LOCALCTSIZE)+1)));

		int nTmp = GetSubBlocksLen(m_nCurrOffset+
			sizeof(TGIFImageDescriptor) + nLCTSize + 1);
		if (nTmp > 0)
			return sizeof(TGIFImageDescriptor) + nLCTSize + 1 + nTmp;
		break;
	};

	return 0;
}

bool CLKPicture::SkipNextBlock()
{
	if (!m_pRawData) return false;

	int nLen = GetNextBlockLen();
	if ((nLen <= 0) || ((m_nCurrOffset+nLen) > m_nDataSize))
		return false;

	m_nCurrOffset += nLen;
	return true;
}

bool CLKPicture::SkipNextGraphicBlock()
{
	if (!m_pRawData) return false;

	// GIF header + LSDescriptor [+ GCT] [+ Control block] + Data

	enum GIFBlockTypes nBlock;

	nBlock = GetNextBlock();

	while ((nBlock != BLOCK_CONTROLEXT) &&
		   (nBlock != BLOCK_IMAGE) &&
		   (nBlock != BLOCK_PLAINTEXT) &&
		   (nBlock != BLOCK_UNKNOWN) &&
		   (nBlock != BLOCK_TRAILER) )
	{
		if (!SkipNextBlock()) return NULL;
		nBlock = GetNextBlock();
	};

	if ((nBlock == BLOCK_UNKNOWN) ||
		(nBlock == BLOCK_TRAILER))
		return false;

	// it's either a control ext.block, an image or a plain text

	if (GetNextBlockLen() <= 0) return false;

	if (nBlock == BLOCK_CONTROLEXT)
	{
		if (!SkipNextBlock()) return false;
		nBlock = GetNextBlock();

		// skip everything until we meet an image block or a plain-text block
		while ((nBlock != BLOCK_IMAGE) &&
			   (nBlock != BLOCK_PLAINTEXT) &&
			   (nBlock != BLOCK_UNKNOWN) &&
			   (nBlock != BLOCK_TRAILER) )
		{
			if (!SkipNextBlock()) return NULL;
			nBlock = GetNextBlock();
		};

		if ((nBlock == BLOCK_UNKNOWN) ||
			(nBlock == BLOCK_TRAILER))
			return false;
	};

	// skip the found data block (image or plain-text)
	if (!SkipNextBlock()) return false;

	return true;
}

void CLKPicture::ResetDataPointer()
{
	// skip header and logical screen descriptor
	m_nCurrOffset = 
		sizeof(TGIFHeader)+sizeof(TGIFLSDescriptor)+m_nGlobalCTSize;
}

enum CLKPicture::GIFBlockTypes CLKPicture::GetNextBlock() const
{
	switch(m_pRawData[m_nCurrOffset])
	{
	case 0x21:
	// extension block
		switch(m_pRawData[m_nCurrOffset+1])
		{
		case 0x01:
		// plain text extension
			return BLOCK_PLAINTEXT;
			break;

		case 0xF9:
		// graphic control extension
			return BLOCK_CONTROLEXT;
			break;

		case 0xFE:
		// comment extension
			return BLOCK_COMMEXT;
			break;

		case 0xFF:
		// application extension
			return BLOCK_APPEXT;
			break;
		};
		break;
	
	case 0x3B:
	// trailer
		return BLOCK_TRAILER;
		break;

	case 0x2C:
	// image data
		return BLOCK_IMAGE;
		break;
	};

	return BLOCK_UNKNOWN;
}

UINT CLKPicture::GetSubBlocksLen(UINT nStartingOffset) const
{
	UINT nRet = 0;
	UINT nCurOffset = nStartingOffset;
	
	while (m_pRawData[nCurOffset] != 0)
	{
		nRet += m_pRawData[nCurOffset]+1;
		nCurOffset += m_pRawData[nCurOffset]+1;
	};

	return nRet+1;
}

HGLOBAL CLKPicture::GetNextGraphicBlock(UINT *pBlockLen, 
	UINT *pDelay, SIZE *pBlockSize, SIZE *pBlockOffset, 
	UINT *pDisposal)
{
	if (!m_pRawData) return NULL;

	// GIF header + LSDescriptor [+ GCT] [+ Control block] + Data

	*pDisposal = 0;
	enum GIFBlockTypes nBlock;
	nBlock = GetNextBlock();

	while (
		(nBlock != BLOCK_CONTROLEXT) &&
		(nBlock != BLOCK_IMAGE) &&
		(nBlock != BLOCK_PLAINTEXT) &&
		(nBlock != BLOCK_UNKNOWN) &&
		(nBlock != BLOCK_TRAILER)
		)
	{
		if (!SkipNextBlock()) return NULL;
		nBlock = GetNextBlock();
	};

	if ((nBlock == BLOCK_UNKNOWN) ||
		(nBlock == BLOCK_TRAILER))
		return NULL;

	// it's either a control ext.block, an image or a plain text

	int nStart = m_nCurrOffset;
	int nBlockLen = GetNextBlockLen();

	if (nBlockLen <= 0) return NULL;

	if (nBlock == BLOCK_CONTROLEXT)
	{
		// get the following data
		TGIFControlExt *pControl = 
			reinterpret_cast<TGIFControlExt *> (&m_pRawData[m_nCurrOffset]);
		// store delay time
		*pDelay = pControl->m_wDelayTime;
		// store disposal method
		*pDisposal = pControl->GetPackedValue(GCX_PACKED_DISPOSAL);

		if (!SkipNextBlock()) return NULL;
		nBlock = GetNextBlock();
		
		// skip everything until we find data to display 
		// (image block or plain-text block)
		
		while (
			(nBlock != BLOCK_IMAGE) &&
			(nBlock != BLOCK_PLAINTEXT) &&
			(nBlock != BLOCK_UNKNOWN) &&
			(nBlock != BLOCK_TRAILER)
			)
		{
			if (!SkipNextBlock()) return NULL;
			nBlock = GetNextBlock();
			nBlockLen += GetNextBlockLen();
		};

		if ((nBlock == BLOCK_UNKNOWN) || (nBlock == BLOCK_TRAILER))
			return NULL;
		nBlockLen += GetNextBlockLen();
	}
	else
		*pDelay = -1; // to indicate that there was no delay value

	if (nBlock == BLOCK_IMAGE)
	{
		// store size and offsets
		TGIFImageDescriptor *pImage = 
			reinterpret_cast<TGIFImageDescriptor *> (&m_pRawData[m_nCurrOffset]);
		pBlockSize->cx = pImage->m_wWidth;
		pBlockSize->cy = pImage->m_wHeight;
		pBlockOffset->cx = pImage->m_wLeftPos;
		pBlockOffset->cy = pImage->m_wTopPos;
	};

	if (!SkipNextBlock()) return NULL;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED,
		sizeof(TGIFHeader) +
		sizeof(TGIFLSDescriptor) +
		m_nGlobalCTSize +
		nBlockLen + 
		1);  // for the trailer

	if (!hGlobal) return NULL;

	int nOffset = 0; 

	// GMEM_FIXED means we get a pointer
	unsigned char *pGlobal = reinterpret_cast<unsigned char *> (hGlobal);

	CopyMemory(pGlobal,m_pRawData, 
		sizeof(TGIFHeader)+sizeof(TGIFLSDescriptor)+m_nGlobalCTSize);
	nOffset += sizeof(TGIFHeader)+sizeof(TGIFLSDescriptor)+m_nGlobalCTSize;

	CopyMemory(pGlobal + nOffset,&m_pRawData[nStart], nBlockLen);
	nOffset += nBlockLen;

	pGlobal[nOffset] = 0x3B; // trailer
	nOffset++;

	*pBlockLen = nOffset;

	return hGlobal;
}


bool CLKPicture::InnerLoad(HGLOBAL hGlobal, DWORD dwSize)
{
	IStream *pStream = NULL;
	UnLoad();

	if (!(m_pRawData = reinterpret_cast<unsigned char*> (GlobalLock(hGlobal))) )
	{
		TRACE(_T("Load: Error locking memory\n"));
		return false;
	};

	m_nDataSize = dwSize;
	m_pGIFHeader = reinterpret_cast<TGIFHeader *> (m_pRawData);

	if ((memcmp(&m_pGIFHeader->m_cSignature,"GIF",3) != 0) &&
		((memcmp(&m_pGIFHeader->m_cVersion,"87a",3) != 0) ||
		 (memcmp(&m_pGIFHeader->m_cVersion,"89a",3) != 0)) )
	{
	// it's neither GIF87a nor GIF89a
	// do the default processing

		// clear GIF variables
		m_pRawData = NULL;
		GlobalUnlock(hGlobal);

		// don't delete memory on object's release
		if (CreateStreamOnHGlobal(hGlobal,false,&pStream) != S_OK)
			return false;

		if (OleLoadPicture(pStream,dwSize,false,IID_IPicture,
			reinterpret_cast<LPVOID *>(&m_pPicture)) != S_OK)
		{
			pStream->Release();
			return false;
		};
		pStream->Release();

		// store picture's size

		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

        // 计算图片对应的像素宽度
        CalcImageWidth(hmWidth, hmHeight);
		//HDC hDC = ::GetDC(m_hWnd);
		//m_PictureSize.cx = MulDiv(hmWidth, GetDeviceCaps(hDC,LOGPIXELSX), 2540);
		//m_PictureSize.cy = MulDiv(hmHeight, GetDeviceCaps(hDC,LOGPIXELSY), 2540);
		//::ReleaseDC(m_hWnd,hDC);
	}
	else
	{
		// it's a GIF
		m_bIsGIF = true;
		m_pGIFLSDescriptor = reinterpret_cast<TGIFLSDescriptor *>
			(m_pRawData + sizeof(TGIFHeader));
		if (m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCT) == 1)
		{
			// calculate the globat color table size
			m_nGlobalCTSize = static_cast<int>
				(3* (1 << (m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCTSIZE)+1)));
			// get the background color if GCT is present
			unsigned char *pBkClr = m_pRawData + sizeof(TGIFHeader) + 
				sizeof(TGIFLSDescriptor) + 3*m_pGIFLSDescriptor->m_cBkIndex;
			m_clrBackground = RGB(pBkClr[0],pBkClr[1],pBkClr[2]);
		};

		// store the picture's size
		m_PictureSize.cx = m_pGIFLSDescriptor->m_wWidth;
		m_PictureSize.cy = m_pGIFLSDescriptor->m_wHeight;

		// determine frame count for this picture
		UINT nFrameCount=0;
		ResetDataPointer();
		while (SkipNextGraphicBlock())
			nFrameCount++;

		if (nFrameCount == 0) // it's an empty GIF!
		{
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);
			return false;
		};

		// now check the frame count
		// if there's only one frame, no need to animate this GIF
		// therefore, treat it like any other pic

		if (nFrameCount == 1)
		{
			// clear GIF variables
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);

			// don't delete memory on object's release
			if (CreateStreamOnHGlobal(hGlobal,false,&pStream) != S_OK)
				return false;

			if (OleLoadPicture(pStream,dwSize,false,IID_IPicture,
				(LPVOID *)&m_pPicture) != S_OK)
			{
				pStream->Release();
				return false;
			};

			pStream->Release();
		}
		else
		{
		// if, on the contrary, there are several frames
		// then store separate frames in an array

			TFrame frame;
			UINT nBlockLen;
			HGLOBAL hFrameData;
			UINT nCurFrame = 0;

			ResetDataPointer();
			while (hFrameData = GetNextGraphicBlock(&nBlockLen,
				&frame.m_nDelay, &frame.m_frameSize,
				&frame.m_frameOffset, &frame.m_nDisposal) )
			{
				IStream *pStream = NULL;

				// delete memory on object's release
				if (CreateStreamOnHGlobal(hFrameData,true,&pStream) != S_OK)
				{
					GlobalFree(hFrameData);
					continue;
				};

				if (OleLoadPicture(pStream,nBlockLen,false,
					IID_IPicture,
					reinterpret_cast<LPVOID *>(&frame.m_pPicture)) != S_OK)
				{
					pStream->Release();
					continue;
				};
				pStream->Release();
			
				// everything went well, add this frame
				m_arrFrames.push_back(frame);
			};

			// clean after ourselves
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);

			if (m_arrFrames.empty()) // couldn't load any frames
				return false;
		};
	}; // if (!IsGIF...

	//return PrepareDC(m_PictureSize.cx,m_PictureSize.cy);
    return true;
}

