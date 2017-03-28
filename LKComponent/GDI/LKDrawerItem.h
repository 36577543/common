#pragma once
#include "LKCtrlBase.h"

typedef struct _DrawerItemButton
{
    // ��ʶ
    int     nId;
    // ͼ��
    CLKImage *pImg;
    // ����
    CString strCaption;
    // ��ť����(0 - ��ͨ��ť; 1 - ��ѡ��ť)
    int     nType;
    // ����
    CRect   rtWnd;
    // ��ǰ״̬�����Ը�ѡ��ť��Ч, 0 - δѡ�У� 1 - ѡ�У�
    int     nSelect;
    _DrawerItemButton()
    {
        nId         = 0;
        pImg        = 0;
        strCaption  = _T("");
        nType       = 0;
        rtWnd       = CRect(0, 0, 0, 0);
        nSelect     = 0;
    }
    _DrawerItemButton(_DrawerItemButton &obj)
    {
        nId         = obj.nId;
        pImg        = obj.pImg;
        strCaption  = obj.strCaption;
        nType       = obj.nType;
        rtWnd       = obj.rtWnd;
        nSelect     = obj.nSelect;
    }
}DrawerItemButton, *PDrawerItemButton;
typedef CArray<PDrawerItemButton, PDrawerItemButton> PDIBArray;

// ����
struct DrawItemData : public CtrlDataBase
{
    CString         strCaption;
    bool            bExpand;
    PDIBArray       arrButtons;
    CtrlDataBase    *pData;
    DrawItemData()
    {
        bExpand = false;
        pData   = 0;
    }
    ~DrawItemData()
    {
        for(int i = 0; i < arrButtons.GetCount(); i++)
        {
            delete arrButtons[i];
        }
        if(pData)
        {
            delete pData;
        }
        //arrButtons.RemoveAll();
    }
};

// ����״̬��չ��; ��£; ��괦�����ϵĸ���״̬��
// CLKDrawerItem

class CLKDrawerItem : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKDrawerItem)

public:
	CLKDrawerItem(int nImageIndex = 0);
	virtual ~CLKDrawerItem();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
    // ���ñ���
    void SetCaption(CString &strCaption){m_strCaption = strCaption;}
    // ��ȡ���λ��
    void GetCurRect(CRect &rtItem){rtItem = m_bExpand ? m_rtExpand : m_rtCollect;}
    // ��ȡ���ͷ���߶�
    int GetHeadHeight(){return m_nHeadHeight;}
    // ������£λ��
    void SetCollectRect(CRect &rtItem){m_rtCollect = rtItem;}
    // ����չ��λ��
    void SetExpandRect(CRect &rtItem){m_rtExpand = rtItem;}
    // ��ȡչ��״̬
    bool GetExpandState(){return m_bExpand;}
    // ����չ��״̬
    void SetExpandState(bool bExpand = true);
    // ����һ����ť
    void AddButton(int nIndex, CString strCaption, CLKImage *pImg = 0, int nButtonType = 0, int nSelect = 0);
    void AddButton(CString strCaption, CLKImage *pImg = 0, int nButtonType = 0);
    // ���ù�������ָ��
    virtual void SetDataPtr(PCtrlDataBase pData);
    // �������ݵ��ڴ�
    virtual void SaveDataToMem(CtrlDataBase *pData);
    // ���ڴ��м�������
    virtual void LoadDataFromMem(CtrlDataBase *pData);
    // �����Ӵ������ݵ��ڴ�
    void SaveChildDataToMem();
    // ���ڴ��м������ݵ��Ӵ���
    void LoadChildDataToMem();
    // ��ȡ�Ӵ���
    CLKCtrlBase *GetChildWnd(){return m_pChildWnd;}
    // �����Ӵ���
    void SetChildWnd(CLKCtrlBase *pChildWnd);
    // �����ӿؼ�������
    void AdjustChildWndRect();
    // ȥ���ӿؼ�����
    void RemoveChildWnd(){if(m_pChildWnd){m_pChildWnd->SetParent(0);}}
    LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
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
    // ��ǰ������ָ����CDC������
    virtual void OnDrawFGToDC(BGParam *prm);
	// ���������Ϣ
    virtual bool OnEraseBG(CDC *pDC){return 1;}
private:
    // ���ָ��ĵ�ǰ��ť������-2 - ���ڵ�ǰ�ؼ���ͷ��, -1 - �ڵ�ǰ�ؼ�ͷ���ϵ����ڰ�ť�ϣ� 0 - �ڵ�һ����ť�ϡ�������������
    int     m_nCurIndex;
    // ����������ʱ�ĵ�ǰ��ť����(�뵯��ʱ��Ӧ)
    int     m_nCurDownIndex;
    // ����Ѱ���
    bool    m_bMouseDown;
    // ����
    CString m_strCaption;
    // ״̬��true - չ��; false - ��£�� 
    bool    m_bExpand;
    // ��£ʱ��λ��(����ڸ����ڵ�λ��)
    CRect   m_rtCollect;
    // չ��ʱ��λ��(����ڸ����ڵ�λ��)
    CRect   m_rtExpand;
    // ͷ���߶�
    int     m_nHeadHeight;
    // ��ť�߿հ׾���
    int     m_nMargin;
    // ��ťͼ�����ı��ľ���
    int     m_nImageTextSplit;
    // ��ť���
    int     m_nButtonSplit;
    // �ִ���
    CLKCtrlBase *m_pChildWnd;
    // ��ť����
    CLKImage    *m_pImgButton;
    // ��ť�б�
    PDIBArray   m_arrItem;
	// ���������ͼƬ���ڴ滺����
	void ParseImageToMem();
    // �������ָ���λ�û�ȡ������
    int GetItemIndexByPoint(CPoint point);
    // ����ǩ�ı�
    void DrawCaption(CDC *pDC, CRect &rtText);
    // ����ť
    void DrawButtons(CDC *pDC);
    void DrawButton(CDC *pDC, PDrawerItemButton pItem, bool bIsCur = false);
    // ���㰴ť����
    void CalcItemRect();
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL DestroyWindow();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
};


