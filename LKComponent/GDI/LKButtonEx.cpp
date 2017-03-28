// ..\..\common\LKComponent\GDI\LKButtonEx.cpp : 实现文件
//

#include "LKButtonEx.h"
#include "LKImageMgr.h"

// CLKButtonEx

IMPLEMENT_DYNAMIC(CLKButtonEx, CLKButton)

CLKButtonEx::CLKButtonEx(CLKImage *pImg, CString strImage)
: CLKButton(2)
, m_bSelected(false)
, m_pImg(pImg)
, m_bDeletedMouseDown(false)
, m_strImage(strImage)
, m_bDrag(false)
{
	m_pDeleteImg = CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_BUTTONDELETE);
}

CLKButtonEx::~CLKButtonEx()
{
}


BEGIN_MESSAGE_MAP(CLKButtonEx, CLKButton)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CLKButtonEx 消息处理程序

// 在缓存上绘图
void CLKButtonEx::OnPaint(CDC *pDC)
{
	if(m_pDeleteImg && m_pDeleteImg->IsValid() && GetNormalMemImg() != GetMemImg())
	{
		CRect rtDelete(0, 0, m_pDeleteImg->GetWidth(), m_pDeleteImg->GetHeight() / m_pDeleteImg->GetImageNumber());
		rtDelete.MoveToXY(m_rtClient.Width() - rtDelete.Width(), m_rtClient.Height() - rtDelete.Height());
		if(m_bDeletedMouseDown)
		{
			m_pDeleteImg->DrawToDC(pDC, rtDelete, 1);
		}else //if(GetHighLightMemImg() == GetMemImg())
		{
			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);
			if(m_rtClient.PtInRect(point))
			{
				m_pDeleteImg->DrawToDC(pDC, rtDelete, 0);
			}
		}
	}
}

// 解析出相关图片到内存缓存中
void CLKButtonEx::ParseImageToMem()
{
	// 
	int nType(0);
	CLKImage *pTemp = GetMemImg();
	if(GetNormalMemImg() == pTemp)
	{
		nType = 0;
	}else if(pTemp == GetHighLightMemImg())
	{
		nType = 1;
	}else if(pTemp == GetOperatorMemImg())
	{
		nType = 2;
	}
	BGParam prm;
	prm.rt = CRect(0, 0, 0, 0);
	GetParent()->SendMessage(LKCOMMON_MSG, LKCOMMON_MSG_WPARAM_GETMEMIMG, (LPARAM)&prm);
	CRect rt;
	GetRectInParent(&rt);
	prm.rt.OffsetRect(rt.left, rt.top);
	CreateNormalImg(0, prm, m_pImg);
	CreateHighLightImg(1, prm, m_pImg);
	CreateOperatorImg(1, prm, m_pImg);
	if(1 == nType)
	{
		SetMemImg(GetHighLightMemImg());
	}else if(2 == nType)
	{
		SetMemImg(GetOperatorMemImg());
	}else
	{
		SetMemImg(GetNormalMemImg());
	}
}

// 获取需要解析的图片
CLKImage *CLKButtonEx::GetParseImg()
{
	return CLKImageMgr::GetImageS(LKIMAGEBUTTONINDEX_COMMON_SKINFRAME);
}

void CLKButtonEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bDrag && !m_bDeletedMouseDown)
    {
        m_bDrag = false;
        return;
    }
    m_bDrag = false;
	CRect rtDelete(0, 0, m_pDeleteImg->GetWidth(), m_pDeleteImg->GetHeight() / m_pDeleteImg->GetImageNumber());
	rtDelete.MoveToXY(m_rtClient.Width() - rtDelete.Width(), m_rtClient.Height() - rtDelete.Height());
	if(rtDelete.PtInRect(point))
	{
		
		::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKBUTTONEX_MSG_WPARAM_DELETECLICK, GetDlgCtrlID());
	}else
	{
		::PostMessage(GetParent()->GetSafeHwnd(), LKCOMMON_MSG, LKBUTTONEX_MSG_WPARAM_SELECTED, GetDlgCtrlID());
		m_bSelected = true;
	}
	m_bDeletedMouseDown = false;
	CLKButton::OnLButtonUp(nFlags, point);
	if(GetHighLightMemImg() != GetMemImg())
	{
		SetMemImg(GetHighLightMemImg());
	}
}

void CLKButtonEx::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bDrag = false;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CLKButton::OnLButtonDown(nFlags, point);
	//SetMemImg(GetHighLightMemImg());
	m_bDeletedMouseDown = false;
	if(m_pDeleteImg && m_pDeleteImg->IsValid())
	{
		CRect rtDelete(0, 0, m_pDeleteImg->GetWidth(), m_pDeleteImg->GetHeight() / m_pDeleteImg->GetImageNumber());
		rtDelete.MoveToXY(m_rtClient.Width() - rtDelete.Width(), m_rtClient.Height() - rtDelete.Height());
		if(rtDelete.PtInRect(point))
		{
			m_bDeletedMouseDown = true;
		}
	}
}

void CLKButtonEx::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

    m_bDrag = false;
	m_bDeletedMouseDown = false;
	CLKButton::OnMouseLeave();
	if(m_bSelected)
	{
		SetMemImg(GetOperatorMemImg());
	}else
    {
		SetMemImg(GetNormalMemImg());
    }
}

// 设置选中状态
void CLKButtonEx::SetSelected(bool bSelected)
{
	if(m_bSelected != bSelected)
	{
		m_bSelected = bSelected;
		if(!m_bSelected)
		{
			SetMemImg(GetNormalMemImg());
		}else
        {
			SetMemImg(GetOperatorMemImg());
        }
		Invalidate();
	}
}

void CLKButtonEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    if(MK_LBUTTON == (MK_LBUTTON & nFlags))
    {
        m_bDrag = true;
    }
	CLKButton::OnMouseMove(nFlags, point);
	SetMemImg(GetHighLightMemImg());
}
