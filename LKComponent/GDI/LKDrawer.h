#pragma once
//#include "LKCtrlBase.h"
#include "LKDrawerItem.h"

// ����
struct DrawData : public CtrlDataBase
{
    // ��ǰѡ����
    int                                     nCurIndex;
    // ���б�
    CArray<DrawItemData *, DrawItemData *>  arrItem;
    ~DrawData()
    {
        for(int i = 0; i < arrItem.GetCount(); i++)
        {
            delete arrItem[i];
        }
    }
};

// CLKDrawer

class CLKDrawer : public CLKCtrlBase
{
	DECLARE_DYNAMIC(CLKDrawer)

public:
	CLKDrawer(int nImageIndex = 0);
	virtual ~CLKDrawer();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Create(CString strCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateHide(const RECT& rect, CWnd* pParentWnd, UINT nID);

    // ����һ��
    void Add(CString strText = _T(""));
    // ѡ��һ��
    void SelectItem(int nIndex);
    // ��ȡѡ���������
    int GetSelectedIndex(){return m_nSelectedIndex;}
    // ����һ����ť��ָ��������
    void AddButton(int nIndex, CString strCaption, CLKImage *pImg = 0, int nButtonType = 0);
    // ��ȡ�Ӵ���
    CLKCtrlBase *GetChildWnd(int nIndex);
    // ��ȡ����
    CWnd *GetItemWnd(int nIndex){if(nIndex >= 0 && nIndex < m_arrItem.GetCount()){return m_arrItem[nIndex];}return 0;}
    // �����Ӵ���
    void SetChildWnd(int nIndex, CLKCtrlBase *pChildWnd);
    void SetChildWnd(CLKCtrlBase *pChildWnd);
    // ���ù�������ָ��
    void SetDataPtr(PCtrlDataBase pData);
    // �������ݵ��ڴ�
    virtual void SaveDataToMem(CtrlDataBase *pData);
    // ���ڴ��м�������
    virtual void LoadDataFromMem(CtrlDataBase *pData);
	// �Ƿ��ȡ������(���߸�ǰ��)
	void SetChildGetParentBG(bool b = true);
	//// ���ñ���
 //   void ResetBGDC();
protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	// ���������ͼƬ���ڴ滺����
	virtual void ParseImageToMem();
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
	// ˢ��ǰ������
	virtual void ReSetMem();
	// ���������Ϣ
    virtual bool OnEraseBG(CDC *pDC){return 1;}
    // ��print����
    virtual int OnPrint(WPARAM wParam, LPARAM lParam);
private:
    // ��ǰѡ���������
    int     m_nSelectedIndex;
    // ������
    CArray<CLKDrawerItem *, CLKDrawerItem *> m_arrItem; 
    // ������
    void CreateItem(CLKDrawerItem *pItem, int nId);
    void CreateItems();
    // �����������λ��
    void CalcItemsRect();
    // ��������ʱ����λ��
    void CalcItemsCollectRect();
    // ������λ��
    void AdjustItemPos();
    void AdjustItemPos(int nOldIndex);
public:
    afx_msg void OnSetFocus(CWnd* pOldWnd);
//    afx_msg void OnMove(int x, int y);
};


