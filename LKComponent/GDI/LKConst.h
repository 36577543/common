#pragma once


// ����ϵͳ�汾��
// XP
#define	WINDOWS_XP		10
// WIN7
#define WINDOWS_7		20

// ��Ϣ����
// �ؼ�ͨ����Ϣ
#define LKCOMMON_MSG						WM_USER	+ 10
// �����Զ�����Ϣ
#define LKSELFOTHER_MSG						WM_USER + 20

// ������Ϣ
#define WM_LKTRAYNOTYFY						WM_USER + 1000

// ͨ����ϢWPARAM
// ��ȡ����ͼƬ
#define LKCOMMON_MSG_WPARAM_GETBGIMG			100
// ��ȡǰ���ڴ�ͼƬ
#define LKCOMMON_MSG_WPARAM_GETMEMIMG			101
// ��굥����Ϣ
#define LKCOMMON_MSG_WPARAM_CLICK				102
// �ֶ�ˢ�±�����Ϣ
#define LKCOMMON_MSG_WPARAM_RESETMEM			103
// ��ȡPrint��Ϣʱ�Ĵ���
#define LKCOMMON_MSG_WPARAM_GETPRINTDLG			104
// ��ǰ��д��DC��Ϣ(��������)
#define LKCOMMON_MSG_WPARAM_DRAWDCNOBG			105

// ��Ͽ�
// ѡ��
#define LKCOMBOBOX_MSG_WPARAM_ITEMSELECTED		120
// �����б�����
#define LKCOMBOBOX_MSG_WPARAM_NOEXISTITEMS		121
// ɾ��
#define LKCOMBOBOX_MSG_WPARAM_ITEMDELETED		122

// �༭��
// balloon ���̴�д���л�����
#define LKEDIT_MSG_WPARAM_CAPSLK				130
// balloon ���̴�д���ͷ�
#define LKEDIT_MSG_WPARAM_CAPSUNLK				131

// ��ʾ���ڿؼ�
// balloon ע��
#define LKBALLOONWND_MSG_WPARAM_CLOSE			135

// HTML������Ϣ
// OnDocumentComplete���
#define LKADDLG_MSG_WPARAM_SUCCESS				140

// ��ť�ؼ�
// ����ɾ��
#define LKBUTTONEX_MSG_WPARAM_DELETECLICK		145
// ѡ��
#define LKBUTTONEX_MSG_WPARAM_SELECTED			146
// ɾ��
#define LKBUTTONEX_MSG_WPARAM_DELETE			147
// ��ѡ��ťѡ�иı�
#define LKBUTTONEX_MSG_WPARAM_SELECTEDCHANGEED	148
// ��ѡ��ťѡ�иı�
#define LKBUTTONEX_MSG_WPARAM_SELECTEDCHANGEED2	149

// LKTREECTRL
// ��ı���Ϣ
#define LKTREECTRL_MSG_WPARAM_ITEMCHANGED		150
// ���а�ť������Ϣ
#define LKTREECTRL_MSG_WPARAM_ITEMBTNCLICKED	151
// ����ʾ��Ϣ(���������ͣ��һ����ʱ�䴥��)
#define LKTREECTRL_MSG_WPARAM_ITEMHINT	        152
// ����뿪�ؼ�
#define LKTREECTRL_MSG_WPARAM_MOUSELEAVE        153
// �����������οؼ��ϰ���
#define LKTREECTRL_MSG_WPARAM_LBUTTONDOWN       154
// ȡ����ʾ��Ϣ
#define LKTREECTRL_MSG_WPARAM_ITEMCANCELHINT    155
// ��˫����Ϣ
#define LKTREECTRL_MSG_WPARAM_ITEMDBCLICKED	    156

// LISTCTRL
// ��ı���Ϣ
#define LKLISTCTRL_MSG_WPARAM_ITEMCHANGED		200
#define LKLISTCTRL_MSG_WPARAM_HEADWIDTHCHANGED	201

// DRAWER
// ��ı���Ϣ
#define LKDRAWER_MSG_WPARAM_ITEMCHANGED		    210
// ���ť������Ϣ
#define LKDRAWER_MSG_WPARAM_ITEMBTNCLICKED	    211
// ���ѡ��ťѡ����Ϣ
#define LKDRAWER_MSG_WPARAM_ITEMBTNSELECTED	    212
// ���ѡ��ťȡ��ѡ����Ϣ
#define LKDRAWER_MSG_WPARAM_ITEMBTNCANCEL	    213

// ��ֱ��������Ϣ
#define LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR		300
// ˮƽ��������Ϣ
#define LKCTRLBASEEX_MSG_WPARAM_HSCROLLBAR		301
// ��ֱ���������Ϲ�����Ϣ
#define LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARUP	303
// ��ֱ���������¹�����Ϣ
#define LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARDOWN	304
// ��ʾ��������Ϣ
#define LKCTRLBASEEX_MSG_WPARAM_SHOWSCROLLBAR	305
// TAB
// ��ı���Ϣ
#define LKTAP_MSG_WPARAM_ITEMCHANGED		    400
// �����ڹر���Ϣ
#define LKTAP_MSG_WPARAM_ITEMCLOSING	        401
// ���Ѿ��ر���Ϣ
#define LKTAP_MSG_WPARAM_ITEMCLOSED		        402
// ���������Ϣ
#define LKTAP_MSG_WPARAM_ITEMLSCROLL		    403
// ���ҹ�����Ϣ
#define LKTAP_MSG_WPARAM_ITEMRSCROLL		    404

// CLKScaleSplidWnd �ؼ�
// ��չ��
#define LKSCALESPLIDWND_MSG_WPARAM_OPEN		    500
// ��չ��
#define LKSCALESPLIDWND_MSG_WPARAM_CLOSE	    501

// CLKSplit �ָ����϶�
#define LKSPLIT_MSG_DRAG                        600

// CLKRichEdit �ؼ���Ϣ��ȡID��Ӧ��CLKImageָ��
#define LKRICHEDIT_MSG_WPARAM_GETIMAGEBYID      620

// CLKContainer��Ϣ
// ���а�ť������Ϣ
#define LKCONTAINER_MSG_WPARAM_ITEMBTNCLICKED	650


// �ؼ���Ӧ����Դ�б���ʼ�ͽ�������
// ͨ����Դ
#define LKIMAGELISTINDEX_COMMON_BEGIN			0
// ͨ����Դ��ͼƬ
#define LKIMAGELISTINDEX_COMMON_NULL			0
// �б��ࣱ����ͨ�ñ߿�
#define LKIMAGELISTINDEX_COMMON_CTRLFRAME1		1
// 23���ظ߶���ͨ���ɰ�
#define LKIMAGELISTINDEX_COMMON_TOPMASK23		2
// 27���ظߵײ�ͨ���ɰ�
#define LKIMAGELISTINDEX_COMMON_BOTTOMMASK27	3
// 21���ظ�ѡ�к�����ƶ�����
#define LKIMAGELISTINDEX_COMMON_SELECTANDFOCUS	4
// ��Ϣͼ��
#define LKIMAGEBUTTONINDEX_COMMON_ICONMESSAGE	5
// ����Ƥ��ͼ��
#define LKIMAGEBUTTONINDEX_COMMON_ICONSKIN		6
// ҵ��ͼ��
#define LKIMAGEBUTTONINDEX_COMMON_ICONBUSINESS	7
// ����ͼ��
#define LKIMAGEBUTTONINDEX_COMMON_ICONADD		8
// ��Ϣ֪ͨͼ��
#define LKIMAGEBUTTONINDEX_COMMON_ICONNOTIFY	9
// �����޸�ͼ��
#define LKIMAGEBUTTONINDEX_COMMON_ICONMODIFY	10
// ɾ��ͼ��
#define LKIMAGEBUTTONINDEX_COMMON_ICONDELETE	11
// ������ͼƬ(Ĭ��Ϊ��ֱ������)
#define LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR		12
// ������ͼƬ(Ĭ��Ϊ��ֱ������)
#define LKIMAGEBUTTONINDEX_COMMON_VSCROLLBAR	LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR
// ˮƽ������ͼƬ
#define LKIMAGEBUTTONINDEX_COMMON_HSCROLLBAR	13
// ������ͼƬ��Ϊ10(Ĭ��Ϊ��ֱ������)
#define LKIMAGEBUTTONINDEX_COMMON_VSCROLLBAR10	14
// ɾ��ͼ��
#define LKIMAGEBUTTONINDEX_COMMON_ICONDELETE15	15
// ������ʾ����ͼƬ
#define LKIMAGEBUTTONINDEX_COMMON_DIALOGBALLOON	16
// ͷ�����ͼƬ
#define LKIMAGEBUTTONINDEX_COMMON_HEADFRAME		17
// Ƥ����ť���ͼƬ
#define LKIMAGEBUTTONINDEX_COMMON_SKINFRAME		18
// Ƥ����ť������ɾ��ͼƬ
#define LKIMAGEBUTTONINDEX_COMMON_BUTTONDELETE	19
// Ƥ����ť��Ƥ��Ĭ�ϳ���ͼƬ
#define LKIMAGEBUTTONINDEX_COMMON_NOSKIN		20
// ��Ϣ����ʾͼ��
#define LKIMAGEBUTTONINDEX_COMMON_MESSAGEHINT	21
// ��ǩ��ʾ����
#define LKIMAGEBUTTONINDEX_COMMON_LABELBG	    22
// ��ʾ�Ի���ͨ�ñ���
#define LKIMAGEHINTDIALOG_COMMON_BG	            23
// ɾ��ͼ��2
#define LKIMAGEINDEX_COMMON_ICONDELETE3_15	    24
// ����ͼ��
#define LKIMAGEINDEX_COMMON_ICONDSEARCH			25
// 6���ظ߶���ͨ���ɰ�
#define LKIMAGELISTINDEX_COMMON_TOPMASK6		26
// ��ý����
#define LKIMAGELISTINDEX_COMMON_FOCUS			27

#define LKIMAGELISTINDEX_COMMON_END				49

// �Ի���
#define LKIMAGELISTINDEX_DIALOG_BEGIN			50
#define LKIMAGELISTINDEX_DIALOG_END				60
// ����
#define LKIMAGELISTINDEX_MONTHCAL_BEGIN			61
#define LKIMAGELISTINDEX_MONTHCAL_END			63
// ��������ʱ��
#define LKIMAGELISTINDEX_DTAETIME_BEGIN			64
#define LKIMAGELISTINDEX_DTAETIME_END			66
// �������ؿؼ�
#define LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN    67
// ��
#define LKIMAGELISTINDEX_SCALESPLIDWND_LOPEN    LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN
// ���
#define LKIMAGELISTINDEX_SCALESPLIDWND_LCLOSE   LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN + 1
// �ҿ�
#define LKIMAGELISTINDEX_SCALESPLIDWND_ROPEN    LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN + 2
// �ҹ�
#define LKIMAGELISTINDEX_SCALESPLIDWND_RCLOSE   LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN + 3
#define LKIMAGELISTINDEX_SCALESPLIDWND_END      70
// ��ť
#define LKIMAGELISTINDEX_BUTTON_BEGIN			71
#define LKIMAGELISTINDEX_BUTTON_END				74
// ������
#define LKIMAGELISTINDEX_DRAWERITEM_BEGIN		75
#define LKIMAGELISTINDEX_DRAWERITEM_END			76
// ����
#define LKIMAGELISTINDEX_DRAWER_BEGIN			77
#define LKIMAGELISTINDEX_DRAWER_END				78
// �༭��
#define LKIMAGELISTINDEX_EDIT_BEGIN				80
#define LKIMAGELISTINDEX_EDIT_END				83
// ��ѡ��
#define LKIMAGELISTINDEX_CHECKBOX_BEGIN			85
#define LKIMAGELISTINDEX_CHECKBOX_END			86

// �б�ؼ�
#define LKIMAGELISTINDEX_LISTCTRL_BEGIN			90
#define LKIMAGELISTINDEX_LISTCTRL_END			95

// �б���ͷ�ؼ�
#define LKIMAGELISTINDEX_LISTCTRLHEADER_BEGIN	96
#define LKIMAGELISTINDEX_LISTCTRLHEADER_END		98

// ��ҳ�ؼ�
#define LKIMAGELISTINDEX_TAB_BEGIN				99
#define LKIMAGELISTINDEX_TABHEADERBG			LKIMAGELISTINDEX_TAB_BEGIN + 0
#define LKIMAGELISTINDEX_TABHEADER				LKIMAGELISTINDEX_TAB_BEGIN + 1
#define LKIMAGELISTINDEX_TABHEADER2				LKIMAGELISTINDEX_TAB_BEGIN + 2
// ���ͷ
#define LKIMAGELISTINDEX_TABEXLEFTARR			LKIMAGELISTINDEX_TAB_BEGIN + 3
// �Ҽ�ͷ
#define LKIMAGELISTINDEX_TABEXRIGHTARR			LKIMAGELISTINDEX_TAB_BEGIN + 4
#define LKIMAGELISTINDEX_TAB_END				103

// ���οؼ�
#define LKIMAGELISTINDEX_TREECTRL_BEGIN			105
#define LKIMAGELISTINDEX_TREECTRL_END			108

// ��Ͽ�
#define LKIMAGELISTINDEX_COMBOBOX_BEGIN			110
#define LKIMAGELISTINDEX_COMBOBOX_END			112

// ��ѡ��
#define LKIMAGELISTINDEX_RADIOBOX_BEGIN			114
#define LKIMAGELISTINDEX_RADIOBOX_END			116

// �˵�
#define LKIMAGELISTINDEX_MENU_BEGIN			    118
#define LKIMAGELISTINDEX_MENU_END			    119
// ͨ�ò˵�����
#define LKIMAGELISTINDEX_MENU_BG			    118

// ������
#define LKIMAGELISTINDEX_PROGRESSBAR_BEGIN		120
#define LKIMAGELISTINDEX_PROGRESSBAR_END		121
// ����������
#define LKIMAGELISTINDEX_PROGRESSBAR_BG			120


// ��Ŀ����ͼƬ�洢������ʼ����(�ǹ���ͼƬ)
#define LKPROJECT_INDEX_BEGIN					200
// ��־����
// �Ի���WM_PAINT
//#define LKDIALOG_PAINT_LOG 
// ����ؼ�WM_PAINT
//#define LKBASECTRL_PAINT_LOG
// �Ի���WM_NCPAINT
//#define LKDIALOG_NCPAINT_LOG 




#define LoadAllResourceImageToImageArray(pImgMgr)\
{\
	pImgMgr->InitImageArray(400);\
	/*ͨ����ԴͼƬ*/\
	int i = LKIMAGELISTINDEX_COMMON_BEGIN;\
	/*Ĭ�ϵĿؼ��ǿͻ���ͨ�ÿ�ͼƬ*/\
	pImgMgr->SetImage(i++);\
	/*Ĭ�ϵĿؼ��ǿͻ���ͨ�ÿ��ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_CTRL_FRAME, CRect(2, 26, 2, 2), 1);\
	/*Ĭ�ϵĿͻ��������ɰ�ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_TOPMASK23, CRect(4, 0, 4, 0), 1);\
	/*Ĭ�ϵĿͻ����ײ��ɰ�ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_BOTTOMMASK27);\
	/*Ĭ�ϵ�ѡ�кͻ�ȡ����ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_SELECTANDFOCUS2_21, CRect(5, 0, 6, 0), 2);\
	/*Ĭ�ϵ���ϢͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_MESSAGE);\
	/*Ĭ�ϸ���Ƥ��ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_SKIN);\
	/*Ĭ��ҵ��ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_BUSINESS);\
	/*Ĭ������ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_ADD);\
	/*Ĭ����Ϣ֪ͨͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_NOTIFY);\
	/*�����޸�ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_MODIIFY);\
	/*ɾ��ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_DELETE2_16, CRect(0, 0, 0, 0), 2);\
	/*������ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_SCROLLBAR4_14, CRect(0, 6, 0, 6), 4);\
	/*ˮƽ������ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_HSCROLLBAR4_14, CRect(6, 0, 7, 0), 4);\
	/*������ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_SCROLLBAR4_10, CRect(0, 6, 0, 6), 4);\
	/*ɾ��ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_DELETE2_15, CRect(0, 0, 0, 0), 2);\
	/*������ʾ����ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_DIALOG_BALLOON, CRect(15, 10, 5, 5), 1);\
	/*ͷ�����ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_HEADFRAME, CRect(6, 6, 6, 6), 1);\
	/*��ť������ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_SKIN, CRect(4, 4, 4, 4), 2);\
	/*��ťɾ��������ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_BUTTONDELETE, CRect(0, 0, 0, 0), 2);\
	/*Ƥ����ť��Ƥ��Ĭ�ϳ���ͼƬ*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_NOSKIN);\
	/*��Ϣ����ʾͼ��*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_MESSAGEHINT);\
	/*��ǩ��ʾ����*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_LABEL, CRect(5, 5, 5, 5), 1);\
	/*��ʾ�Ի���ͨ�ñ���*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_HINTDIALOG_BG, CRect(10, 35, 10, 35), 1);\
	/*ɾ��ͼ��2*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICONDELETE3_15, CRect(0, 0, 0, 0), 3); \
	/*����ͼ��*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_SEARCH); \
	/*Ĭ�ϵĿͻ��������ɰ�ͼƬ6���ظ�*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_TOPMASK6); \
	/*��ý����*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_FOCUS, CRect(12, 12, 12, 12)); \
	/*ͨ����ԴͼƬ����*/\
	/*Ĭ�ϵĶԻ�����ͼƬ*/\
	i = LKIMAGELISTINDEX_DIALOG_BEGIN;\
	pImgMgr->SetPNGImage(i++, IDB_PNG_DLG_CLOSE);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_DLG_MIN);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_DLG_MAX);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_DLG_RESTORE);\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_DLG_SELF);\
	pImgMgr->SetJPGImage(i++, IDB_JPG_DLG_BG);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_DLG_FRAME, CRect(35, 29, 5, 32));\
	pImgMgr->SetPNGImage(i++, IDB_PNG_DLG_LOGINBG, CRect(0, 0, 0, 0), 1, false);\
	/*����ͼƬ*/\
	i = LKIMAGELISTINDEX_MONTHCAL_BEGIN;\
	pImgMgr->SetPNGImage(i++, IDB_PNG_CALENDAR_BUTTONBG3_17_16, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_CALENDAR_BG, CRect(3, 3, 3, 3), 1);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_CALENDAR_ARR2, CRect(0, 0, 0, 0), 2);\
	/*��������ʱ��ͼƬ*/\
	i = LKIMAGELISTINDEX_DTAETIME_BEGIN;\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_DTAETIME_DEL3_17_16, CRect(2, 2, 2, 2), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_DTAETIME_DOWN3_17_16, CRect(2, 2, 2, 2), 3);\
	/*Ĭ�ϵ��������ؿؼ�ͼƬ*/\
	i = LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN;\
	pImgMgr->SetPNGImage(i++, IDB_PNG_SCALESPLIDWND_IMAGELO3_11_205, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_SCALESPLIDWND_IMAGELC3_11_205, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_SCALESPLIDWND_IMAGERC3_11_205, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_SCALESPLIDWND_IMAGERO3_11_205, CRect(0, 0, 0, 0), 3);\
	/*Ĭ�ϵİ�ťͼƬ*/\
	i = LKIMAGELISTINDEX_BUTTON_BEGIN;/*��ťͼƬ��ʼ����*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_COMMON2_20, CRect(2, 2, 2, 2), 2);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_COMMON3_20, CRect(2, 2, 2, 2), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_COMMON4_22, CRect(2, 2, 2, 2), 4);\
	/*Ĭ�ϵĳ�����ͼƬ*/\
	i = LKIMAGELISTINDEX_DRAWERITEM_BEGIN;/*��ťͼƬ��ʼ����*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_DRAWERHEADER, CRect(17, 0, 0, 0), 4);\
	/*Ĭ�ϵĳ���ͼƬ*/\
	i = LKIMAGELISTINDEX_DRAWER_BEGIN;/*��ťͼƬ��ʼ����*/\
	pImgMgr->SetImage(i++);\
	/*Ĭ�ϵı༭��ͼƬ*/\
	i = LKIMAGELISTINDEX_EDIT_BEGIN;/*�༭��ͼƬ��ʼ����*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_EDIT_COMMON2_24, CRect(5, 5, 5, 5), 2);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_EDIT_COMMON2_26, CRect(20, 8, 20, 8), 2);\
	/*Ĭ�ϵĸ�ѡ��ͼƬ*/\
	i = LKIMAGELISTINDEX_CHECKBOX_BEGIN;/*��ѡ��ͼƬ��ʼ����*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_CHECKBOX_COMMON4, CRect(0, 0, 0, 0), 4);\
	/*Ĭ�ϵ��б�ؼ�ͼƬ*/\
	i = LKIMAGELISTINDEX_LISTCTRL_BEGIN;/*�б�ؼ�ͼƬ��ʼ����*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_LISTCTRL_BG, CRect(2, 22, 2, 2), 1);\
	/*Ĭ�ϵ�TAB�ؼ�ͼƬ*/\
	i = LKIMAGELISTINDEX_TAB_BEGIN;/*�б�TAB�ؼ�ͼƬ��ʼ����*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TAB_HEADER, CRect(6, 0, 6, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TAB_HEADER3_32, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TABEX_LEFTARR3_13, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TABEX_RIGHTARR3_13, CRect(0, 0, 0, 0), 3);\
	/*Ĭ�ϵ����οؼ�ͼƬ*/\
	i = LKIMAGELISTINDEX_TREECTRL_BEGIN;/*���οؼ�ͼƬ��ʼ����*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TREECTRL_EXPAND1, CRect(0, 0, 0, 0), 2);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TREECTRL_EXPAND2, CRect(0, 0, 0, 0), 2);\
	/*Ĭ�ϵ���Ͽ�ؼ�ͼƬ*/\
	i = LKIMAGELISTINDEX_COMBOBOX_BEGIN;/*��Ͽ�ؼ�ͼƬ��ʼ����*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMBOBOXCTRL_BUTTON, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMBOBOXCTRL_BUTTON3_26, CRect(0, 0, 0, 0), 3);\
	/*Ĭ�ϵĸ�ѡ��ͼƬ*/\
	i = LKIMAGELISTINDEX_RADIOBOX_BEGIN;/*��ѡ��ͼƬ��ʼ����*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_RADIOBOX_COMMON4, CRect(0, 0, 0, 0), 4);\
	/*Ĭ�ϵĲ˵�ͼƬ*/\
	i = LKIMAGELISTINDEX_MENU_BEGIN;/*�˵�ͼƬ��ʼ����*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_MENUBG, CRect(31, 5, 5, 5), 1);\
	/*Ĭ�ϵĽ�����ͼƬ*/\
	i = LKIMAGELISTINDEX_PROGRESSBAR_BEGIN;/*������ͼƬ��ʼ����*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_PROGRESSBG, CRect(5, 0, 5, 0), 1);\
}



//	pImgMgr->SetPNGImage(i++, IDB_PNG_EDIT_COMMON2_26, CRect(20, 8, 20, 8), 2);\
