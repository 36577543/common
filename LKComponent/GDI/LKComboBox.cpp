// ..\..\common\LKComponent\GDI\LKComboBox.cpp : 实现文件
//

#include "LKComboBox.h"
#include "LKImageMgr.h"
#include "LKFontMgr.h"

// CLKComboBox
// 编辑框子控件标识
#define LKCOMBOBOX_CHILDID_EDIT		1

IMPLEMENT_DYNAMIC(CLKComboBox, CLKCommonCtrlEx)

LRESULT CALLBACK CLKComboBox::EditWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	CLKComboBox *pThis = (CLKComboBox *)GetWindowLong(hWnd, GWL_USERDATA);
	if(msg == WM_MOUSELEAVE)
	{
		if(pThis->GetMemImg() != pThis->m_pNormalImg)
		{
			pThis->SetMemImg(pThis->m_pNormalImg);
			if(0 == pThis->m_pNormalImg)
			{
				CRect rt;
				pThis->GetWindowRect(&rt);
				pThis->GetParent()->ScreenToClient(&rt);
				pThis->GetParent()->InvalidateRect(&rt, false);
			}else
			{
				pThis->SendMessage(WM_NCPAINT);
			}
		}
	}else if(msg == WM_MOUSEMOVE)
	{
		if(pThis->m_pHighLightImg && pThis->GetMemImg() == pThis->m_pNormalImg)
		{
			pThis->SetMemImg(pThis->m_pHighLightImg);
			TRACKMOUSEEVENT etMouse;
			etMouse.cbSize = sizeof(TRACKMOUSEEVENT);   
			etMouse.dwFlags = TME_LEAVE | TME_HOVER;   
			etMouse.dwHoverTime = NULL;   
			etMouse.hwndTrack = hWnd;  
			_TrackMouseEvent(&etMouse);   
			pThis->SendMessage(WM_NCPAINT);
		}
	}else if(WM_SETFOCUS == msg)
	{
		if(!pThis->m_strHint.IsEmpty())
		{
			CString strText;
			pThis->m_ctlEdit.GetWindowText(strText);
			if(strText.IsEmpty())
			{
				pThis->m_ctlEdit.SetWindowText(strText);
			}
		}
	}else if(WM_KEYDOWN == msg)
	{
		if(wparam == VK_BACK || wparam == VK_DELETE)
		{
			pThis->m_bCanAutoComplete = false;
		}else
		{
			pThis->m_bCanAutoComplete = true;
		}
		if(pThis->m_bCapture)
		{
			::ReleaseCapture();
		}
		//::UpdateWindow(hWnd);
	}else if(WM_KILLFOCUS == msg)
	{
		if(pThis->m_bCapture) 
		{
			//pThis->SetFocus();
			ReleaseCapture();
			//return 1;
		}
		if(!pThis->m_strHint.IsEmpty())
		{
			CString strText;
			pThis->m_ctlEdit.GetWindowText(strText);
			if(strText.IsEmpty())
			{
				pThis->m_ctlEdit.SetWindowText(pThis->m_strHint);
			}
		}
	}else if(WM_GETTEXT == msg)
	{
		if(!pThis->m_strHint.IsEmpty())
		{
			LRESULT lRet = CallWindowProc(pThis->m_pOldEditProc, hWnd, msg, wparam, lparam);
			TCHAR *p = (TCHAR *)lparam;
			CString s = p;
			if(!s.IsEmpty() && s.Compare(pThis->m_strHint) == 0)
			{
				memset((char *)lparam, 0, s.GetLength() * sizeof(TCHAR));
			}
			//static int n(0);
			//TRACE(_T("WM_GETTEXT: %d\r\n"), n++);
			return lRet;
		}
	}else if(WM_GETTEXTLENGTH == msg)
	{
		if(!pThis->m_strHint.IsEmpty())
		{
			LRESULT lRet = CallWindowProc(pThis->m_pOldEditProc, hWnd, msg, wparam, lparam);
			TCHAR *p = (TCHAR *)lparam;
			CString s = p;
			if(!s.IsEmpty() && s.Compare(pThis->m_strHint) == 0)
			{
				lRet = 0;
			}
			return lRet;
		}
	}else if(EM_SHOWBALLOONTIP == msg)
	{
		// 去掉默认的提示信息。
		return 1;
	//}else if(WM_SETFOCUS == msg)
	//{
	//	if(!pThis->m_strHint.IsEmpty())
	//	{
	//		CString strText;
	//		pThis->m_ctlEdit.GetWindowText(strText);
	//		if(strText.IsEmpty())
	//		{
	//			pThis->m_ctlEdit.SetWindowText(strText);
	//		}
	//	}
	}
	return ::CallWindowProc(pThis->m_pOldEditProc, hWnd, msg, wparam, lparam);
}


CLKComboBox::CLKComboBox(int nImageIndex, int nButtonIndex)
: CLKCommonCtrlEx(nImageIndex)
, m_nButtonIndex(nButtonIndex)
, m_colHint(RGB(160, 160, 160))
, m_pButtonImg(0)
// 列表框最大高度
, m_nMaxHeight(100)
, m_nDownHeight(30)
// 获取鼠标
, m_bCapture(false)
, m_rtOldWnd(0, 0, 0, 0)
, m_rtButton(0, 0, 0, 0)
, m_pPreWnd(0)
, m_pEditBr(0)

// 文本改变前的数据
, m_strOldText(_T(""))
// 是否可以自动完成
, m_bCanAutoComplete(true)
// 高亮选中前景图片
, m_pSelectImg(0)
// 高亮选中删除图片
, m_pDeleteImg(0)
// 文本的左边距
, m_nTextLeftMargin(4)
// 顶部距离
, m_nTopOffset(0)
// 可见元素总个数
, m_nVisibleItemCount(0)
// 可见元素起始索引
, m_nVisibleItemBeginIndex(0)
// 每一项的高度
, m_nItemHeight(22)
// 选中结点
, m_nSelectItem(0)
// 高亮结点索引(正处于鼠标移动下的)
, m_nCurItem(-1)
// 删除区域是否处于当前鼠标下
, m_bIsDeleteArea(false)
// 项之间的间距
, m_nItemSplit(1)
// 项的左边距
, m_nItemLeftMargin(0)
// 项的右边距
, m_nItemRightMargin(1)
, m_bAnimate(true)
{
	//if(nImageIndex == 1)
	//{
		m_rtClientOffset = CRect(5, 5, 5, 5);
	//}else if(2 == nImageIndex)
	//{
	//	m_rtClientOffset = CRect(5, 6, 5, 6);
	//}
	SetVScrollbarVisible();
}

CLKComboBox::~CLKComboBox()
{
	if(m_pEditBr) delete m_pEditBr;
}

BOOL CLKComboBox::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CLKCommonCtrlEx::Create(_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP, rect, pParentWnd, nID); 
	return bRet;
}

// 获取当前选中项
int CLKComboBox::GetCurSel()
{
	CString strText;
	m_ctlEdit.GetWindowText(strText);
	if(!strText.IsEmpty())
	{
		for(int i = 0; i < m_strItems.GetCount(); i++)
		{
			if(strText.Compare(m_strItems[i]) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}

CString CLKComboBox::GetCurText()
{
	CString strText;
	m_ctlEdit.GetWindowText(strText);
	return strText;
}

// 设置当前选中项
void CLKComboBox::SetCurSel(int nIndex)
{
	CString strText;
	if(nIndex >= 0 && nIndex < m_strItems.GetCount())
	{
        m_nSelectItem = nIndex;
		strText = m_strItems[nIndex];
	}
	m_ctlEdit.SetWindowText(strText);
}

// 设置提示信息
void CLKComboBox::SetHintText(CString strHint)
{
	m_strHint = strHint;
	if(GetSafeHwnd())
	{
		CString strText;
		GetWindowText(strText);
		if(strText.IsEmpty() && !m_strHint.IsEmpty())
		{
			m_ctlEdit.SetWindowText(m_strHint);
		}
	}
}
// 设置文本
void CLKComboBox::SetWindowText(LPCTSTR lpszString)
{
	m_ctlEdit.SetWindowText(lpszString);
}

// 增加项
int CLKComboBox::AddItem(CString strText)
{
	int nCount = m_strItems.Add(strText);
	CalcItemsHeight();
	return nCount;
}

// 删除项
CString CLKComboBox::DeleteItem(int nIndex)
{
    if(nIndex >= 0 && nIndex < m_strItems.GetCount())
    {
        CString strRet = m_strItems[nIndex];
        m_strItems.RemoveAt(nIndex);
        m_nVisibleItemBeginIndex = 0;
        int nCount = m_nVisibleItemCount;
	    CalcItemsHeight();
        if(nIndex == m_nSelectItem)
        {
            SetCurSel(0);
        }
        if(nCount != m_nVisibleItemCount)
        {
            FreeOperatorImg();
        }
        return strRet;
    }
    return _T("");
}


BEGIN_MESSAGE_MAP(CLKComboBox, CLKCommonCtrlEx)
	ON_MESSAGE(LKCOMMON_MSG, OnCommonMsg)
	ON_WM_CTLCOLOR()
	ON_WM_SETFOCUS()
	ON_EN_CHANGE(LKCOMBOBOX_CHILDID_EDIT, OnEnChangeEdit)
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CLKComboBox 消息处理程序
LRESULT CLKComboBox::OnCommonMsg(WPARAM wParam, LPARAM lParam)
{
	if(LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR == wParam)
	{
		// 滚动
		LKSCROLLINFO *pInfo = (LKSCROLLINFO *)lParam;
		// 滚动消息
		m_nVisibleItemBeginIndex = pInfo->nPos;
		CalcVisibleItemCount();
		CRect rtData(m_rtClient);
		GetDataRect(rtData);
		InvalidateRect(&rtData);
	}
	return CLKCtrlBaseEx::OnCommonMsg(wParam, lParam);
}

// 文本发生改变
void CLKComboBox::OnEnChangeEdit()
{
	CString strText;
	m_ctlEdit.GetWindowText(strText);
	if(strText.Compare(m_strOldText) == 0) return;
	m_strOldText = strText;
	if(m_bCanAutoComplete)	
	{
		if(!AutoCompleteText())
		{
			// 字符串不存在于列表中
			::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKCOMBOBOX_MSG_WPARAM_NOEXISTITEMS, 0);
		}else
		{
			int nIndex = FindListText();
			if(nIndex >= 0)
			{
				// 发送消息告知完成输入
				::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKCOMBOBOX_MSG_WPARAM_ITEMSELECTED, nIndex);
			}
		}
	}else
	{
		int nIndex = FindListText();
		if(nIndex >= 0)
		{
			m_ctlEdit.SetWindowText(m_strItems[nIndex]);
			int ns = m_strItems[nIndex].GetLength() - 1;
			m_ctlEdit.SetSel(m_strItems[nIndex].GetLength(), m_strItems[nIndex].GetLength());
			// 发送消息告知完成输入
			::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKCOMBOBOX_MSG_WPARAM_ITEMSELECTED, nIndex);
		}else
		{
			// 发送消息告知字符串不存在于列表中
			::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKCOMBOBOX_MSG_WPARAM_NOEXISTITEMS, nIndex);
		}
	}
	m_ctlEdit.Invalidate();
}

// 刷新前景缓存
void CLKComboBox::ReSetMem()
{
	if(m_bImgIsOwner && !m_bCapture)
	{
		ParseImageToMem();
		if(IsWindowVisible())
		{
			Invalidate();
		}
	}
}



// 初始化图片列表
void CLKComboBox::InitImageList()
{
	// 初始化图片列表
	CLKImageMgr *p = CLKImageMgr::GetInstance();
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_EDIT_BEGIN, LKIMAGELISTINDEX_EDIT_BEGIN + 2);
	p->GetPartImage(m_arrImage, LKIMAGELISTINDEX_COMBOBOX_BEGIN, LKIMAGELISTINDEX_COMBOBOX_END);
	// 下拉按钮
	m_pButtonImg = m_arrImage[m_nButtonIndex]; 
	// 高亮
	m_pSelectImg = CLKImageMgr::GetImageS(LKIMAGELISTINDEX_COMMON_SELECTANDFOCUS);
	if(m_pSelectImg)
	{
		m_nItemHeight = m_pSelectImg->GetHeight() / m_pSelectImg->GetImageNumber();
	}
	// 删除
	m_pDeleteImg = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_ICONDELETE15);
	// 滚动条
	SetScrollbarImg(CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_VSCROLLBAR10), 0);
}

// 解析出相关图片到内存缓存中
void CLKComboBox::ParseImageToMem()
{
	m_bImgIsOwner = true;
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(pImg)
	{
		// 
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);
		int nNumber = pImg->GetImageNumber();

		CRect rtWnd;
		GetWindowRect(&rtWnd);
		rtWnd.MoveToXY(0, 0);
		if(m_pNormalImg) delete m_pNormalImg;
		m_pNormalImg = new CLKImage(rtWnd.Width(), rtWnd.Height());
		pImg->DrawToImage(m_pNormalImg, 0, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);

		if(m_pHighLightImg) delete m_pHighLightImg;
		m_pHighLightImg = new CLKImage(rtWnd.Width(), rtWnd.Height());
		pImg->DrawToImage(m_pHighLightImg, 1, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);
		// 画下拉按钮
		if(m_pButtonImg)
		{
			CRect rtBtn(rtWnd);
			int nH = m_pButtonImg->GetHeight() / m_pButtonImg->GetImageNumber();
			rtBtn.DeflateRect(m_rtClientOffset.left, m_rtClientOffset.top, m_rtClientOffset.right, m_rtClientOffset.bottom);
			rtBtn.top += (rtBtn.Height() - nH) / 2;
			rtBtn.left = rtBtn.right - m_pButtonImg->GetWidth();
			rtBtn.bottom = rtBtn.top + nH;
			m_rtButton = rtBtn;
			CRect rtSrc;
			m_pButtonImg->DrawToImage(m_pNormalImg, 0, rtBtn);
			m_pButtonImg->DrawToImage(m_pHighLightImg, 1, rtBtn);
		}

		SetMemImg(m_pNormalImg);
	}
}

// 创建控件后初始化
void CLKComboBox::InitCtrlAfterCreate()
{
	CLKCommonCtrlEx::InitCtrlAfterCreate();
	CRect rtEdit(m_rtClient);
	if(GetImageIndex() == 1)
	{
		//rtEdit.top += 2;
	}else if(2 == GetImageIndex())
	{
		rtEdit.top += 2;
	}
	if(m_pButtonImg)
	{
		rtEdit.right -= m_pButtonImg->GetWidth();
	}
	m_ctlEdit.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL, rtEdit, this, LKCOMBOBOX_CHILDID_EDIT);
	SetWindowLong(m_ctlEdit.GetSafeHwnd(), GWL_USERDATA,(LONG)this);
	m_pOldEditProc = (WNDPROC)SetWindowLong(m_ctlEdit.GetSafeHwnd(), GWL_WNDPROC,(LONG)EditWndProc);
	CFont *pFtText = CLKFontMgr::GetST90();
	m_ctlEdit.SetFont(pFtText, true);
	if(!m_strHint.IsEmpty())
	{
		SetHintText(m_strHint);
	}
	// 保存自己的位置
	m_pPreWnd = GetWindow(GW_HWNDPREV);
}

// 鼠标进入
void CLKComboBox::OnMouseEnter(CPoint point)
{
	SendMessage(WM_NCPAINT, 0, 0);
}

HBRUSH CLKComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(CTLCOLOR_EDIT == nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_crText);
		if(m_bCapture && m_pButtonImg)
		{
			if(!m_pEditBr)
			{
				CreateEditBrush();
			}
			/*if(m_pOperatorImg)
			{
				CRect rt;
				pWnd->GetWindowRect(&rt);
				CRect rtp;
				GetWindowRect(&rtp);
				int nOffsetx = rt.left - rtp.left;
				int nOffsety = rt.top  - rtp.top;
				rt.MoveToXY(nOffsetx, nOffsety);
				CRect rtDest(rt);
				rtDest.MoveToXY(0, 0);
				m_pOperatorImg->Draw(pDC->GetSafeHdc(), rtDest, rt); 
			}*/
			//return (HBRUSH)0;//::GetStockObject(WHITE_BRUSH);
			return *m_pEditBr;
		}else
		{
			if(!m_strHint.IsEmpty())
			{
				CString strText;
				pWnd->GetWindowText(strText);
				if(strText.IsEmpty())
				{
					pDC->SetTextColor(m_colHint);
				}
			}
		}
		return (HBRUSH)::GetStockObject(WHITE_BRUSH);
		//return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	HBRUSH hbr = CLKCommonCtrlEx::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CLKComboBox::OnSetFocus(CWnd* pOldWnd)
{
	CLKCommonCtrlEx::OnSetFocus(pOldWnd);
	m_ctlEdit.SetFocus();
}

void CLKComboBox::OnMouseLeave()
{
	if(m_bCapture) return;
	CLKCommonCtrlEx::OnMouseLeave();
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if(!m_rtClient.PtInRect(point))
	{
		SendMessage(WM_NCPAINT, 0, 0);
	}
}

void CLKComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rtButton(m_rtButton);
	rtButton.OffsetRect(-m_rtClientOffset.left, -m_rtClientOffset.top);
	if(m_bCapture)
	{
		if(!m_rtClient.PtInRect(point) || rtButton.PtInRect(point))
		{
			ReleaseCapture();
		}else
		{
			CRect rtItem;
			if(GetItemRectByPoint(point, rtItem))
			{
                if(m_bIsDeleteArea)
                {
				    ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKCOMBOBOX_MSG_WPARAM_ITEMDELETED, m_nCurItem + m_nVisibleItemBeginIndex);
                }else
                {
				    // 选中项
				    m_ctlEdit.SetWindowText(m_strItems[m_nCurItem + m_nVisibleItemBeginIndex]);
				    ::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKCOMBOBOX_MSG_WPARAM_ITEMSELECTED, m_nCurItem + m_nVisibleItemBeginIndex);
                }
                ReleaseCapture();
			}
		}
	}else if(rtButton.PtInRect(point))
	{
		SetFocus();
		SetCapture();
		SetScrollbarCaptureFlag();
		m_bCapture = true;
		DropDown();
		if(m_pOperatorImg && GetMemImg() != m_pOperatorImg)
		{
			SetMemImg(m_pOperatorImg);
			Invalidate();
		}
	}
	CLKCtrlBaseEx::OnLButtonDown(nFlags, point);
	SendMessage(WM_NCPAINT, 0, 0);
}

void CLKComboBox::DropDown()
{
    m_nVisibleItemBeginIndex = 0;
	// 获取需要解析的图片
	CLKImage *pImg = GetParseImg();
	if(!pImg) return;
	DWORD dwFlag(0);
	if(m_bAnimate)
	{
		dwFlag = SWP_NOSIZE;
	}
	if(!m_pOperatorImg)
	{
		CWnd *pWnd = GetParent();
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		BGParam prm;
		prm.rt = CRect(0, 0, 0, 0);
		pWnd->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
		CRect rt;
		GetRectInParent(&rt);
		prm.rt.OffsetRect(rt.left, rt.top);
		int nNumber = pImg->GetImageNumber();

		rtWnd.MoveToXY(0, 0);
		m_rtOldWnd = rtWnd;
		rtWnd.bottom = GetNewWindowHeight();
		CWnd *pLastWnd = GetLastBrotherWnd();
		if(pLastWnd)
		{
			::SetWindowPos(GetSafeHwnd(), pLastWnd->GetSafeHwnd(), 0, 0, rtWnd.Width(), rtWnd.Height(), SWP_NOMOVE | dwFlag);
		}else
		{
			SetWindowPos(0, 0, 0, rtWnd.Width(), rtWnd.Height(), SWP_NOMOVE | SWP_NOZORDER | dwFlag);
		}
		m_pOperatorImg = new CLKImage(rtWnd.Width(), rtWnd.Height());
		pImg->DrawToImage(m_pOperatorImg, 1, &prm, LKSKINBASE_DRAWBGSTYLE_ALL);

		// 画下拉按钮
		if(m_pButtonImg && !m_rtButton.IsRectNull())
		{
			//m_pButtonImg->DrawToImage(m_pOperatorImg, 2, m_rtButton);
			// 画框内背景
			CRect rtClient(rtWnd);
			rtClient.DeflateRect(&m_rtClientOffset);
			//rtWnd.right -= m_pButtonImg->GetWidth();
			rtClient.bottom = rtClient.top + m_pButtonImg->GetHeight() / m_pButtonImg->GetImageNumber();
			CRect rtOffset(0, 0, m_pButtonImg->GetWidth() - 1, 0);
			m_pButtonImg->DrawToImage(m_pOperatorImg, rtClient, rtOffset, 2, 0, LKSKINBASE_DRAWBGSTYLE_RM | LKSKINBASE_DRAWBGSTYLE_MM);
		}
		// 滑出
		if(m_bAnimate)
		{
			SlideOut();
		}
	}else if(m_bCapture)
	{
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		rtWnd.MoveToXY(0, 0);
		m_rtOldWnd = rtWnd;
		int nWidth = GetNewWindowHeight();
		rtWnd.bottom = nWidth;
		CWnd *pLastWnd = GetLastBrotherWnd();
		if(pLastWnd)
		{
			::SetWindowPos(GetSafeHwnd(), pLastWnd->GetSafeHwnd(), 0, 0, rtWnd.Width(), rtWnd.Height(), SWP_NOMOVE | dwFlag);
		}else
		{
			SetWindowPos(0, 0, 0, rtWnd.Width(), rtWnd.Height(), SWP_NOMOVE | SWP_NOZORDER | dwFlag);
		}
		// 滑出
		SlideOut();
		//AnimateWindow(200, AW_BLEND | AW_VER_POSITIVE);//AW_SLIDE | AW_VER_POSITIVE); 
	}
	//m_ctlEdit.RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE );
}

void CLKComboBox::OnCaptureChanged(CWnd *pWnd)
{
	// TODO: 在此处添加消息处理程序代码
	if(pWnd != this)
	{
		m_bCapture = false;
		SetScrollbarCaptureFlag(false);
		if(!m_rtOldWnd.IsRectNull())
		{
			//AnimateWindow(2000, AW_BLEND | AW_HIDE);
			if(m_pPreWnd)
			{
				::SetWindowPos(GetSafeHwnd(), m_pPreWnd->GetSafeHwnd(), 0, 0, m_rtOldWnd.Width(), m_rtOldWnd.Height(), SWP_NOMOVE);
			}else
			{
				::SetWindowPos(GetSafeHwnd(), m_pPreWnd->GetSafeHwnd(), 0, 0, m_rtOldWnd.Width(), m_rtOldWnd.Height(), SWP_NOMOVE | SWP_NOZORDER);
			}
		}
		PostMessage(WM_MOUSELEAVE, 0, 0);
	}
	CLKCommonCtrlEx::OnCaptureChanged(pWnd);
}

void CLKComboBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bCapture)
	{
		CLKCtrlBaseEx::OnLButtonUp(nFlags, point);
		OnMouseMove(nFlags, point);
		return;
	}

	CLKCommonCtrlEx::OnLButtonUp(nFlags, point);
}

void CLKComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bCapture && !GetScrollbarDraging() && !PtInScrollbar(point)) 
	{
		//m_bIsDeleteArea = false; 
		CRect rtItem;
		int nCurIndex = GetItemIndexByPoint(point);
		if(m_nCurItem >= 0 && m_nCurItem < m_nVisibleItemCount)
		{
			if(GetItemRectByIndex(m_nCurItem, &rtItem) && m_nCurItem != nCurIndex)
			{
				InvalidateRect(&rtItem, false);
			}
		}
		int nOldIndex = m_nCurItem;
		m_nCurItem = nCurIndex;
		if(m_nCurItem >= 0 && m_nCurItem < m_nVisibleItemCount)
		{
			if(GetItemRectByIndex(m_nCurItem, &rtItem))
			{
				if(m_pDeleteImg)
				{
					CRect rtIcon(rtItem);
					int nImgHeight(m_pDeleteImg->GetHeight() / m_pDeleteImg->GetImageNumber());
					rtIcon.left = rtIcon.right - m_pDeleteImg->GetWidth() - 2;
					rtIcon.right = rtIcon.left + m_pDeleteImg->GetWidth();
					rtIcon.top += (rtIcon.Height() - nImgHeight) / 2 + 1;
					rtIcon.bottom = rtIcon.top + nImgHeight;

					if(rtIcon.PtInRect(point) && m_bIsDeleteArea == false)
					{
						m_bIsDeleteArea = true; 
						InvalidateRect(&rtIcon, false);
					}else if(!rtIcon.PtInRect(point) && m_bIsDeleteArea)
					{
						m_bIsDeleteArea = false;
						InvalidateRect(&rtIcon, false);
					}
				}
				if(nOldIndex != m_nCurItem)
				{
					InvalidateRect(&rtItem, false);
				}
			}
		}
		//return;
	}

	//if()
	CLKCommonCtrlEx::OnMouseMove(nFlags, point);
}

// 获取最后一个ＴＡＢ键顺序的兄弟控件
CWnd *CLKComboBox::GetLastBrotherWnd()
{
	CWnd *pWnd = GetWindow(GW_HWNDLAST);
	if(pWnd == this)
	{
		pWnd = 0;
	}
	return pWnd;
}

// 创建编辑框背景刷子
void CLKComboBox::CreateEditBrush()
{
	CWnd *pWnd = ::AfxGetMainWnd();
	if(pWnd && pWnd->GetSafeHwnd() && m_pOperatorImg)
	{
		CDC *pDC = pWnd->GetWindowDC();
		if(pDC)
		{
			CRect rtEdit;
			m_ctlEdit.GetWindowRect(&rtEdit);
			CRect rtp;
			GetWindowRect(&rtp);
			CRect rtSrc(rtEdit);
			rtSrc.MoveToXY(rtEdit.left - rtp.left, rtEdit.top  - rtp.top);
			rtEdit.MoveToXY(0, 0);

			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, rtEdit.Width(), rtEdit.Height());
			CBitmap *pOld = memDC.SelectObject(&bmp);

			m_pOperatorImg->Draw(memDC.GetSafeHdc(), rtEdit, rtSrc);
			memDC.SelectObject(pOld);
			memDC.DeleteDC();
			pWnd->ReleaseDC(pDC);
			m_pEditBr = new CBrush(&bmp);
		}
	}
}

// 计算滚动条位置
void CLKComboBox::CalcScrollbarRect()
{
	CRect rt(m_rtClient);
	if(rt.Height() < 3 * (m_nItemSplit + m_nItemHeight))
	{
		rt = CRect(0, 0, 0, 0);
		SetVScrollbarRect(rt);
		return;
	}
	CLKCommonCtrlEx::CalcScrollbarRect();
	if(m_pButtonImg)
	{
		GetVScrollbarRect(rt);
		int nH = m_pButtonImg->GetHeight() / m_pButtonImg->GetImageNumber();
		rt.top += nH;
		SetVScrollbarRect(rt);
		int nCount = m_strItems.GetCount();
		if(nCount > m_nVisibleItemCount)
		{
			SetVScrollbarRang(nCount, GetItemsHeight());
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 绘制客户区域
///////////////////////////////////////////////////////////////////////////////////////////////////
// 在缓存上绘图
void CLKComboBox::OnPaint(CDC *pDC)
{
	//
	//pDC->IntersectClipRect(m_nItemLeftMargin, m_nTopOffset, m_nItemRightMargin, 0);
	if(m_bCapture)
	{
		//static int n = 0;
		//TRACE(_T("OnPaint: %d\r\n"), n++);

		int nOffsety(0);
		if(3 == m_nButtonIndex)
		{
			nOffsety = 24 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
		}else
		{
			nOffsety = 26 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
		}
		CRect rtItem(m_nItemLeftMargin, nOffsety, m_rtClient.right - m_nItemRightMargin - GetVScrollbarWidth(), nOffsety + m_nItemHeight);
		for(int i = 0; i < m_nVisibleItemCount && i < m_strItems.GetCount(); i++)
		{
			DrawItem(pDC, m_strItems[m_nVisibleItemBeginIndex + i], rtItem, m_nCurItem == i, m_bIsDeleteArea);
			rtItem.OffsetRect(0, m_nItemSplit + m_nItemHeight);
		}
	}
}

// 画项
void CLKComboBox::DrawItem(CDC *pDC, CString &strText, CRect rtItem, bool bHighLight, bool bIsDelete)
{
	if(bHighLight)
	{
		// 高亮
		if(m_pSelectImg)
		{
			m_pSelectImg->DrawLRToDC(pDC, rtItem, 6, 6, 0);
		}
		// 删除
		if(m_pDeleteImg)
		{
			CRect rtIcon(rtItem);
			int nImgHeight(m_pDeleteImg->GetHeight() / m_pDeleteImg->GetImageNumber());
			rtIcon.left = rtItem.right - m_pDeleteImg->GetWidth() - 2;
			rtIcon.right = rtIcon.left + m_pDeleteImg->GetWidth();
			rtIcon.top += (rtIcon.Height() - nImgHeight) / 2 + 1;
			rtIcon.bottom = rtIcon.top + nImgHeight;
			// 高亮
			if(bIsDelete)
			{
				m_pDeleteImg->DrawToDC(pDC, rtIcon, 1);
			}else
			{
				m_pDeleteImg->DrawToDC(pDC, rtIcon, 0);
			}
		}
	}

	// 画文本
	pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(CLKFontMgr::GetST90());
	pDC->SetTextColor(m_crText);
	rtItem.left += m_nTextLeftMargin;
	pDC->DrawText(strText, &rtItem, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	pDC->SelectObject(pOldFont);

	// 画右边删除按钮
}

// 计算项总高度
void CLKComboBox::CalcItemsHeight()
{
	int nCount = CalcVisibleItemCount();//m_strItems.GetCount();
	if(nCount > 0)
	{
		if(3 == m_nButtonIndex)
		{
			m_nDownHeight = 24 + m_nTopOffset + nCount *(m_nItemHeight + m_nItemSplit) - m_nItemSplit;
		}else
		{
			m_nDownHeight = 26 + m_nTopOffset + nCount *(m_nItemHeight + m_nItemSplit) - m_nItemSplit;
		}

	}else
	{
		m_nDownHeight = 30;
	}
}

// 计算可见项个数
int CLKComboBox::CalcVisibleItemCount()
{
	int nMaxCount = (m_nMaxHeight - m_nTopOffset + m_nItemSplit) / (m_nItemSplit + m_nItemHeight);
	int nM = (m_nMaxHeight - m_nTopOffset + m_nItemSplit) % (m_nItemSplit + m_nItemHeight);
	if(nM > 0)
	{
		nMaxCount++;
	}
	m_nVisibleItemCount = m_strItems.GetCount() - m_nVisibleItemBeginIndex< nMaxCount ? m_strItems.GetCount() - m_nVisibleItemBeginIndex: nMaxCount;
	return m_nVisibleItemCount;
}

// 根据鼠标指针的位置获取项索引　－　相对于起始可见索引
int CLKComboBox::GetItemIndexByPoint(CPoint point)
{
	int nOffsety(0);
	if(3 == m_nButtonIndex)
	{
		nOffsety = 24 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
	}else
	{
		nOffsety = 26 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
	}
	CRect rtItem(m_nItemLeftMargin, nOffsety, m_rtClient.right - m_nItemRightMargin - GetVScrollbarWidth(), nOffsety + m_nItemHeight);
	for(int i = 0; i < m_nVisibleItemCount && i < m_strItems.GetCount(); i++)
	{
		if(rtItem.PtInRect(point))
		{
			return i;
		}
		rtItem.OffsetRect(0, m_nItemSplit + m_nItemHeight);
	}
	return -1;
}

// 根据可见索引获取指定项的区域
bool CLKComboBox::GetItemRectByIndex(int nIndex, LPRECT lpRect)
{
	int nOffsety(0);
	if(3 == m_nButtonIndex)
	{
		nOffsety = 24 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
	}else
	{
		nOffsety = 26 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
	}
	CRect rtItem(m_nItemLeftMargin, nOffsety, m_rtClient.right - m_nItemRightMargin - GetVScrollbarWidth(), nOffsety + m_nItemHeight);
	if(nIndex >= 0 && nIndex < m_nVisibleItemCount)
	{
		rtItem.OffsetRect(0, nIndex *(m_nItemSplit + m_nItemHeight));
		*lpRect = rtItem;
		return true;
	}
	return false;
}

// 根据鼠标指针的位置获取指定项的区域
bool CLKComboBox::GetItemRectByPoint(CPoint point, LPRECT lpRect)
{
	int nOffsety(0);
	if(3 == m_nButtonIndex)
	{
		nOffsety = 24 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
	}else
	{
		nOffsety = 26 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
	}
	CRect rtItem(m_nItemLeftMargin, nOffsety, m_rtClient.right - m_nItemRightMargin - GetVScrollbarWidth(), nOffsety + m_nItemHeight);
	for(int i = 0; i < m_nVisibleItemCount && i < m_strItems.GetCount(); i++)
	{
		if(rtItem.PtInRect(point))
		{
			*lpRect = rtItem;
			return true;
		}
		rtItem.OffsetRect(0, m_nItemSplit + m_nItemHeight);
	}
	return false;
}

// 获取垂直滚动条的宽度
int CLKComboBox::GetVScrollbarWidth()
{
	int nRet(0);
	if(IsVScrollbarValid())
	{
		CRect rt;
		GetVScrollbarRect(rt);
		nRet = rt.Width();
	}
	return nRet;
}

// 获取数据区域
void CLKComboBox::GetDataRect(CRect &rtData)
{
	rtData = m_rtClient;
	int nOffsety(0);
	if(3 == m_nButtonIndex)
	{
		nOffsety = 24 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
	}else
	{
		nOffsety = 26 + m_nTopOffset - m_rtClientOffset.top - m_rtClientOffset.bottom;
	}
	rtData.left = m_nItemLeftMargin;
	rtData.top = nOffsety;
	rtData.right = m_rtClient.right - m_nItemRightMargin - GetVScrollbarWidth();
}

// 滑出
void CLKComboBox::SlideOut(CWnd *pWnd)
{
	if(m_bCapture)
	{
		//DWORD dwCount = ::GetTickCount();
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		rtWnd.bottom += 12;
		int nWidth = GetNewWindowHeight();
		if(rtWnd.Height() > nWidth) rtWnd.bottom = rtWnd.top + nWidth;
		SetWindowPos(0, 0, 0, rtWnd.Width(), rtWnd.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
		if(rtWnd.Height() < nWidth)
		{
			SetTimer(1, 10, 0);
		}
		CRect rt(m_rtClient);
		rt.top += 24;
		SendMessage(WM_NCPAINT, 0, 1);
		InvalidateRect(rt, false);
		//RedrawWindow();
		//TRACE(_T("height timer: %d\r\n"),  GetTickCount() - dwCount);
	}
}

void CLKComboBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(nIDEvent);
	SlideOut();
	CLKCommonCtrlEx::OnTimer(nIDEvent);
}

void CLKComboBox::OnSize(UINT nType, int cx, int cy)
{
	//CLKCommonCtrlEx::OnSize(nType, cx, cy);
	//CWnd::OnSize(nType, cx, cy);
	//GetClientRect(&m_rtClient);

	CLKCtrlBaseEx::OnSize(nType, cx, cy);
	if(m_nVisibleItemCount < m_strItems.GetCount())
	{
		SetVScrollbarVisible();
	}else
	{
		SetVScrollbarVisible(false);
	}
	// TODO: 在此处添加消息处理程序代码
}


// 设置提示后续文字
bool CLKComboBox::AutoCompleteText()
{
	bool bRet = false;
	CString strText;
	m_ctlEdit.GetWindowText(strText);
	if(strText.IsEmpty()) return false;
	int nStart = strText.GetLength();
	int nFind = -1;
	for(int i = 0; i < m_strItems.GetCount(); i++)
	{
		CString strItem = m_strItems[i];
		if(!strItem.IsEmpty())
		{
			if(strItem.Find(strText) == 0)
			{
				bRet = true;
				//nIndex = i;
				if(strItem.Compare(strText) == 0)
				{
					nFind = i;
					break;
				}else if(nStart == strText.GetLength())
				{
					if(nFind < 0)
					{
						nFind = i;
					}
				}
				//break;
			}
		}
	}
	if(nFind >= 0)
	{
		CString strSel = m_strItems[nFind];
		if(strSel.Compare(strText) != 0)
		{
			m_ctlEdit.SetWindowText(strSel);
		}
		m_ctlEdit.SetSel(nStart, strSel.GetLength());
		// 发送消息告知完成输入
		//::PostMessage(GetParent()->GetSafeHwnd(), WM_LKSKINCOMBOBOX_MSG_SELECTOK, nFind, 0);
	}
	return bRet;
}

// 从列表框中查找编辑框中的文字
int CLKComboBox::FindListText()
{
	int nRet = -1;
	CString strText;
	m_ctlEdit.GetWindowText(strText);
	if(strText.IsEmpty()) return nRet;
	int nStart = strText.GetLength();
	for(int i = 0; i < m_strItems.GetCount(); i++)
	{
		CString strItem = m_strItems[i];
		if(!strItem.IsEmpty())
		{
			if(strItem.Compare(strText) == 0)
			{
				nRet = i;
				break;
			}
		}
	}
	return nRet;
}

// 释放下拉后的内存图片()
void CLKComboBox::FreeOperatorImg()
{
	if(m_pOperatorImg)
	{
        delete m_pOperatorImg;
        m_pOperatorImg = 0;
    }
}