#pragma once
#include "LKCtrlBaseEx.h"
#include <list>
#include "DynDialogEx.h"
#include "LKCommonCtrl.h"

using namespace std;

// CLKContainerItem
typedef struct CLKContainerItem
{
	int nId;
	// ǰ��ͼƬ
	CLKImage *pImg = 0;
	// ����
	CString strText;
	// ��־
	CString strFlag;
	// ����
	void     *pData = 0;
}*PLKContainerItem;

// CLKContainer
class CLKContainer : public CLKCtrlBaseEx
{
	DECLARE_DYNAMIC(CLKContainer)

public:
	CLKContainer(int nImageIndex = 0);
	virtual ~CLKContainer();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	// ���ؽ����
	void HideDlg();
	// ����һ���ؼ�����
	bool InsertItem(int nId, CString strItem, CString strFlag, CLKImage *pImg, void *pData = 0, bool bCalcVisible = false);
	// ɾ�����пؼ�����
	void DeleteAllItem();
	// �ؼ������������ϣ��ң���ƫ��ֵ
	CRect GetSubItemsOffest(){ return m_rtSubItemsOffest; }
	// �ؼ�����ļ��
	int GetItemSplit(){ return m_nItemSplit; }
	// �ؼ�����Ŀ�
	int GetItemWidth(){ return m_nItemWidth; }
	// �ؼ�����ĸ�
	int GetItemHeight(){ return m_nItemWidth; }

protected:
	DECLARE_MESSAGE_MAP()
protected:
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// �����ؼ����ʼ��
	virtual void InitCtrlAfterCreate();
    // ��print����
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
	// �ڻ����ϻ�ͼ
	virtual void OnPaint(CDC *pDC);
	// ���������Ϣ
	virtual bool OnEraseBG(CDC *pDC){ return 1; }
private:
	// �ܵ�ҳ��
	int m_nPageCount = 0;
	// ��ǰҳ������
	int m_nCurPageIndex = 0;
	// ��ǰ������������
	int m_nCurItemIndex = -1;
	// ��ǰѡ�е���
	CLKContainerItem  *m_pCurItem = 0;
	// ���״̬
	EMState		m_emsCommon;
	// 
	CDynDialogEx *m_pDialog = 0;
	// ����״̬���ͼƬ
	CLKImage *m_pItemStateImag = 0; 
	// ���ͷ״̬ͼƬ
	CLKImage *m_pItemLeftArrStateImag = 0;
	// ���ͷ״̬
	int m_nLeftArrState = 0;
	// �Ҽ�ͷ״̬ͼƬ
	CLKImage *m_pItemRightArrStateImag = 0;
	// �Ҽ�ͷ״̬
	int m_nRightArrState = 0;
	// ������̬ͼƬ(�ڴ滺��)
	CLKImage			*m_pNormalImg;
	// �ؼ��б�
	list<CLKContainerItem *>   m_ltCtrl;
	// �ɼ��ؼ��б�
	list<CLKContainerItem *>   m_ltCurCtrl;
	// �ؼ�����ı���ͼƬ
	CImage *m_pItemBGImg = 0;
	// �ؼ������������ϣ��ң���ƫ��ֵ
	CRect m_rtSubItemsOffest = CRect(10, 10, 10, 24);
	// �ؼ�����ļ��
	int m_nItemSplit = 10;
	// �ؼ�����Ŀ�
	int m_nItemWidth = 468; //138;
	// �ؼ�����ĸ�
	int m_nItemHeight = 243;// 203;
	// ��һ���Ƽ�����
	CLKContainerItem  *m_pVisibleItemBeginItem = 0;
	// ��������
	void DrawItem(CDC *pDC, PLKContainerItem pItem, CRect rtCtrl);
	// ����ҳ��ť
	void DrawButton(CDC *pDC);
	// ����������Ǹ�������
	bool GetRectByPoint(CRect &r, CPoint &point, int &nIndex);
	// ��ҳ��ť״̬�Ƿ�ı�
	bool IsPageBtnStateChange(CPoint &point);
	// ��������ҳ��ť��
	bool PointInLeftPageBtn(CPoint &point);
	// ������ҷ�ҳ��ť��
	bool PointInRightPageBtn(CPoint &point);
	// ����ɼ��ؼ��б�
	void CalcVisibleCtrl();
	void ProcPageBtnState(CPoint point);
	// ��ǰ��ҳ
	void PagePrev();
	// ���ҳ
	void PageNext();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


