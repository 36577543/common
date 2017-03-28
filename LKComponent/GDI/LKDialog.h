#pragma once
#include "GDICommon.h"
#include "LKImage.h"

// ״̬��Ĭ�ϸ߶�
#define DEFAULT_STATUSBAR_HEIGHT    5


// ͼƬ����
// �رհ�ť
#define LKGDIBASEDIALOG_IMAGEINDEX_CLOSE	0
// ��С����ť
#define LKGDIBASEDIALOG_IMAGEINDEX_MIN		1
// ��󻯰�ť
#define LKGDIBASEDIALOG_IMAGEINDEX_MAX		2
// �ָ���ť
#define LKGDIBASEDIALOG_IMAGEINDEX_RESTORE	3
// ϵͳ��ť
#define LKGDIBASEDIALOG_IMAGEINDEX_SYS		4
// �Զ��尴ť
#define LKGDIBASEDIALOG_IMAGEINDEX_SELF		5
// ����
#define LKGDIBASEDIALOG_IMAGEINDEX_BG		6
// �������
#define LKGDIBASEDIALOG_IMAGEINDEX_BGFRAME	7
// ϵͳ��ťICO
//#define LKGDIBASEDIALOG_IMAGEINDEX_SYSICON	8

// ͼƬ����
#define LKGDIBASEDIALOG_IMAGEINDEX_COUNT	8

// �Ի����ݼ�ע��ID
// ALT + S
#define LKDIALOG_HOTKEY_ALTS	1001
// ALT + C
#define LKDIALOG_HOTKEY_ALTC	1002

// ����ڷǿͻ���λ�ñ�ʶ
enum MOUSENCPOS
{
    // ����С��ť��
    mnp_sys = 0,
    // ����С��ť��
    mnp_min,
    // �����ť��
	mnp_max,
    // �ڹرհ�ť��
    mnp_close,
	// ���Զ��尴ť��
    mnp_menu,
	// �ϱ߽�
	mnp_top,
	// �����Զ�������
	mnp_other,
    // δ֪λ��
    mnp_unknow
};

// ͼƬ����
enum BITMAPSTYLE
{
    // ����
    bs_normal = 0,
    // ����ƶ�
    bs_mousemove,
    // ��갴��
    bs_mousepress
};

struct StatusbarItem
{
    // ����ʶ��(ͨ�������Ϊ����)
    int		nId;
    CString	strCaption;
    CRect	rtCaption;
    //CxImage	imgIcon;
    CRect	rtIcon;
    UINT	uFormat;      // text-drawing options
    CFont   ftCaption;
};

// CLKDialog �Ի���

class CLKDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLKDialog)

public:
	CLKDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLKDialog();
    virtual BOOL OnInitDialog();

public:
	// ���ö���ƫ��
	void SetTopOffset(int nTopOffset){ m_nTopOffset = nTopOffset; }
	// �����ұ�ƫ��
	void SetRightOffset(int nRightOffset){ m_nRightOffset = nRightOffset; }
	// ����ϵͳͼ��
	void SetSysIcon(HICON hIcon){ m_hSysIcon = hIcon; }
	// ���ñ�������
	void SetBGBmp(CLKImage *pBGBmp);
	// �ı䱳��
	void ChangeBGImg(CLKImage *pImg = 0);
	// ��ȡ�������߶�
	int GetTitleBarHeight(){return m_rtClientOffset.top;}
	// ��ȡ��߿���
	int GetLeftFrameWidth(){return m_rtClientOffset.left;}
	// ��ȡ�ұ߿���
	int GetRightFrameWidth(){return m_rtClientOffset.right;}
	// ��ȡ״̬���߶�
	int GetStatusBarHeight(){return m_rtClientOffset.bottom;}
	// ���÷ǿͻ�����
	void SetNCClientRect(const CRect &rtNc){m_rtClientOffset = rtNc;}
	// ����״̬���߶�
	void SetStatusBarHeight(int nHeight){m_rtClientOffset.bottom = nHeight;}
    // ��ȡ�Զ���˵���ť������(�������Ļ)
    void GetSelfRectByScreen(LPRECT lpRect){*lpRect = m_rtMenuWnd; ((CRect *)lpRect)->OffsetRect(m_rtWnd.left, m_rtWnd.top);}
	// ����������ɫ
	void SetTextCol(COLORREF colText){m_crText = colText;}
	// ��ȡ������ɫ
	COLORREF GetTextCol(){return m_crText;}
    // ������ʾ����
    void SetToolTip(CToolTipCtrl *pToolTip){if(m_pToolTip){delete m_pToolTip;} m_pToolTip = pToolTip;m_pToolTip->Activate(TRUE);m_pToolTip->AddTool(this, _T(""));}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT OnCommonMsg(WPARAM wParam, LPARAM lParam);
	// ��Ӧ��ȡ����ͼƬ����Ϣ
	void OnGetBGImgMsg(PBGParam p);
	// ��Ӧ��ȡǰ���ڴ�ͼƬ����Ϣ
	void OnGetMemImgMsg(PBGParam p);
protected:
    // ͨ�ô�ͼ��
	static HICON m_hCommonIcon;
    // ͨ��Сͼ��
	static HICON m_hCommonSmallIcon;
    // ��ʾ����
    CToolTipCtrl    *m_pToolTip;
    // �ؼ���ʾ����
    CToolTipCtrl    *m_pChildToolTip;
	// ��С���ߴ�
	CPoint			m_bMinSize;
	// ͼƬ�б�
	ImagePointArray	m_arrImage;
	// ϵͳͼ��(ICO��ʽ)
	HICON			m_hSysIcon;
	// �������С�ߴ�
	CPoint			m_ptMinWidthHeight;
	// �ı�����
    CFont			m_ftText;
	// �ı���ɫ
    COLORREF		m_crText;
    // �洢ʵʱ�����С
    CRect			m_rtWnd;
    // �洢ʵʱ�رհ�ť�ľ���λ��
    CRect			m_rtCloseWnd;
    // �洢ʵʱ��󻯰�ť(�ָ���ť)�ľ���λ��
    CRect			m_rtMaxWnd;
    // �洢ʵʱ��С����ť�ľ���λ��
    CRect			m_rtMinWnd;
    // �洢�Զ���˵���ť�ľ���λ��
    CRect			m_rtMenuWnd;
	// ��ȡ����ǰ��
	CLKImage *GetMemImage(){return m_pMemBmp;}
	// ��ȡ���汳��
	CLKImage *GetBGImage(){return m_pBGBmp;}
	// ���û��汳��
	void SetBGImage(CLKImage *pImg, bool bIsOwnerBGBmp = false){m_pBGBmp = pImg; m_bIsOwnerBGBmp = bIsOwnerBGBmp;}
	// ����״̬��λ��
	void CalcStatusbarRect();
    // ����رհ�ťλ��
    void CalcCloseRect();
    // ����ϵͳ��ťλ��
    void CalcSysRect();
	// �ͷű�������
	void FreeBGImage(){if(m_bIsOwnerBGBmp && m_pBGBmp){delete m_pBGBmp;}m_pBGBmp = 0;m_bIsOwnerBGBmp = false;}
	// ������������
	void CreateBGImage(){FreeBGImage();m_pBGBmp = new CLKImage(SCREEN_WIDTH, SCREEN_HEIGHT);m_bIsOwnerBGBmp = true;}
	// ������
	void DrawNCBG();
	// ��ʼ����������
	virtual void InitBGBmp();
    // ��ȡ�ṩ����DC�Ĵ��ھ��
    virtual HWND GetProvideBGHwnd(){return GetParent()->GetSafeHwnd();}
	// ��ʼ������ǰ��ͼƬ
	virtual void OnInitMemImg(){}
	// �޶����������С��
	virtual void OnSetMinMaxInfo(MINMAXINFO *lpMMI);
    // �¼�
    virtual BOOL OnEnterPress(HWND hCrtl, LPARAM lparam);
    // �¼�
	virtual BOOL OnEscPress(HWND hCrtl, LPARAM lparam){return true;}
    // �¼�
    virtual void OnShowHint(MSG *pMsg);
    virtual void OnChildShowHint(MSG *pMsg);
	// ��ʼ��ͼƬ�б�
	virtual void InitImageList();
    // ������С����ťλ��
    virtual void CalcMinRect();
    // ������󻯰�ťλ��
	virtual void CalcMaxRect();
    // �����Զ���˵���ťλ��
    virtual void CalcSelfMenuRect();
    // �ж�����Ƿ�����С��ť��Χ�� 
    virtual bool IsInMinbox(CPoint point);
    // �ж�����Ƿ������ť��Χ��
    virtual bool IsInMaxbox(CPoint point);
    // �ж�����Ƿ����Զ���˵���ť��Χ��
    virtual bool IsInMenubox(CPoint point);
    // �ж�����Ƿ��������Զ�������Χ��
	virtual bool IsInOther(CPoint point){return false;}
    // �ж�����Ƿ����ϵİ�ť��Χ��
    virtual bool IsInOldbox(CPoint point);
    // ϵͳ��ť������
	virtual void OnSysClick(){}
    // �رհ�ť������
    virtual void OnCloseClick();
    // ��С����ť������
    virtual void OnMinClick();
    // ��󻯰�ť������ 
    virtual void OnMaxClick();
    // �Զ���˵���ť������
	virtual void OnMenuClick(){}
    // �Զ������򱻰���
	virtual void OnOtherClick(){}
	// �˵�ѡ����Ϣ
	virtual void OnMenuItemSelected(int nId){}
	// �ػ��ǿͻ����Զ�����������
	virtual void OnDrawOther(CDC *pDC, BITMAPSTYLE bs, bool bErase = true){}
    // ����ǿͻ����������ƶ��¼� 
    virtual bool OnNCMouseMoveDef(UINT nHitTest, CPoint point); 
    // ����ǿͻ��������������¼� 
    virtual bool OnNCLMouseDownDef(UINT nHitTest, CPoint point); 
    // ����ǿͻ����������ͷ��¼� 
    virtual bool OnNCLMouseUpDef(UINT nHitTest, CPoint point);
    // ����ǿͻ�������Ҽ������¼� 
	virtual bool OnNCRMouseDownDef(UINT nHitTest, CPoint point){return true;}
	// ����ǿͻ�������¼� 
	virtual bool OnHitTest(CPoint pt, LRESULT &lr){return false;}
    // ������wm_print��Ϣ
    virtual void OnPrint(CDC *pDC){}
    // ������wm_paint��Ϣ
    virtual void OnPaint(CDC *pDC){}
	// ������ͻ���
	virtual void DrawClientArea();
    // ����ǿͻ����ߴ��¼�
    virtual void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    // ����λ��״̬�ı�
    virtual void OnWindowPosChange(PWINDOWPOS p){}
	// �򿪴���
	//virtual void OpenMainDlg();
	// �޸Ĵ�������
	virtual void ModiDialogRectRgn(CRgn &rgn){}
	//// ��ȡӦ�ó���ָ������ͼ������
	//BOOL FindOutPositionOfIconDirectly(const HWND a_hWndOwner, const int a_iButtonID, CRect& a_rcIcon);
private:
	// ����ƫ��λ��(�رհ�ť����ƫ��)
	int				m_nTopOffset = 0;
	// �ұ�ƫ��λ��(�رհ�ť�ұ�ƫ��)
	int				m_nRightOffset = 0;
	// ���
	bool			m_bIsMax;
	// �ڴ汳������
	CLKImage		*m_pBGBmp;
	// �Ƿ��Ƕ�����������
	bool			m_bIsOwnerBGBmp;
	// �ڴ滺��
	CLKImage		*m_pMemBmp;
	// �ͻ���ƫ����
	CRect			m_rtClientOffset;
	// ״̬�����б�
    CPtrList		m_ltStatusItem;
	// ״̬������ʹ�õĿ��
	int				m_nUsedWidth;
    // �߿�Ŀ��
    int				m_nBordWidth;
	// �رհ�ť���ұ߾�
	int				m_nCloseBtnRight;
    // �洢ʵʱϵͳ��ť�ľ���λ��
    CRect			m_rtSysWnd;
    // �洢ʵʱϵͳ��ť�ľ���λ��
    CPoint			m_ptTitle;
    // ���λ�ñ��
    MOUSENCPOS		m_eMouseNCPos;
    // ������һ�ΰ���ʱλ�ñ��
    MOUSENCPOS		m_eMouseNCPressPos;
    // ����ƶ�λ�õ����������Ƿ����仯�����Ƿ��һ�������ƶ�����һ������
    bool			m_bIsMouseChanged;
	// ��һ���϶�����ʱ��(��������������ĺ�����)
	DWORD			m_dwMouseDrag;
    void DrawNCIcon(CDC *pDC, MOUSENCPOS mnp, BITMAPSTYLE bs= bs_normal, bool bErase = true);
    // ���ǿͻ���
    void DrawNClientArea(CDC * pDC, bool bFocus = false);
    // ���߿�
    void DrawNClientFrame();
    // ���������ı�
    void DrawTitleCaption(CDC *pDC);
    // �ػ��ǿͻ���ϵͳͼ��(��ͼƬģʽ)
    void DrawSys(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
    // �ػ��ǿͻ����رհ�ťͼ��(��ͼƬģʽ)
    void DrawClose(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
    // �ػ��ǿͻ�����С����ťͼ��(��ͼƬģʽ)
    void DrawMin(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
	// �ػ��ǿͻ�����󻯻�ָ���ťͼ��(��ͼƬģʽ)
	void DrawMax(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
	// �ػ��ǿͻ����Զ���˵���ťͼ��(��ͼƬģʽ)
	void DrawMenu(CDC *pDC, BITMAPSTYLE bs, bool bErase = true);
    // ��ʧȥ�����ɰ�
    void DrawKillFocusFrame(CDC *pDC);
    // ��������ͼ��
    void DrawTitleBarIcon(CDC *pDC, bool IsNcPaint = true);
	// �ػ��ǿͻ���ͨ�ð�ť(��̬ͼƬģʽ)
	void DrawButton(CDC *pDC, CLKImage *pImage, CRect &rtImg, BITMAPSTYLE bs, bool bErase);
	// ��������
	void DrawTitlebar();
	// �������߿�
	void DrawFrameEdge();
	// ������������
	/*void DrawTitleBarBkEx(CDC * pDC);*/

};
