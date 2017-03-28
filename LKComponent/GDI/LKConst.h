#pragma once


// 操作系统版本号
// XP
#define	WINDOWS_XP		10
// WIN7
#define WINDOWS_7		20

// 消息分类
// 控件通用消息
#define LKCOMMON_MSG						WM_USER	+ 10
// 其它自定义消息
#define LKSELFOTHER_MSG						WM_USER + 20

// 托盘消息
#define WM_LKTRAYNOTYFY						WM_USER + 1000

// 通用消息WPARAM
// 获取背景图片
#define LKCOMMON_MSG_WPARAM_GETBGIMG			100
// 获取前景内存图片
#define LKCOMMON_MSG_WPARAM_GETMEMIMG			101
// 鼠标单击消息
#define LKCOMMON_MSG_WPARAM_CLICK				102
// 手动刷新背景消息
#define LKCOMMON_MSG_WPARAM_RESETMEM			103
// 获取Print消息时的窗口
#define LKCOMMON_MSG_WPARAM_GETPRINTDLG			104
// 将前景写到DC消息(不带背景)
#define LKCOMMON_MSG_WPARAM_DRAWDCNOBG			105

// 组合框
// 选中
#define LKCOMBOBOX_MSG_WPARAM_ITEMSELECTED		120
// 不在列表项中
#define LKCOMBOBOX_MSG_WPARAM_NOEXISTITEMS		121
// 删除
#define LKCOMBOBOX_MSG_WPARAM_ITEMDELETED		122

// 编辑框
// balloon 键盘大写键切换灯亮
#define LKEDIT_MSG_WPARAM_CAPSLK				130
// balloon 键盘大写键释放
#define LKEDIT_MSG_WPARAM_CAPSUNLK				131

// 提示窗口控件
// balloon 注销
#define LKBALLOONWND_MSG_WPARAM_CLOSE			135

// HTML窗口消息
// OnDocumentComplete完成
#define LKADDLG_MSG_WPARAM_SUCCESS				140

// 大按钮控件
// 单击删除
#define LKBUTTONEX_MSG_WPARAM_DELETECLICK		145
// 选中
#define LKBUTTONEX_MSG_WPARAM_SELECTED			146
// 删除
#define LKBUTTONEX_MSG_WPARAM_DELETE			147
// 单选按钮选中改变
#define LKBUTTONEX_MSG_WPARAM_SELECTEDCHANGEED	148
// 复选按钮选中改变
#define LKBUTTONEX_MSG_WPARAM_SELECTEDCHANGEED2	149

// LKTREECTRL
// 项改变消息
#define LKTREECTRL_MSG_WPARAM_ITEMCHANGED		150
// 项中按钮单击消息
#define LKTREECTRL_MSG_WPARAM_ITEMBTNCLICKED	151
// 项提示消息(鼠标在项上停留一定的时间触发)
#define LKTREECTRL_MSG_WPARAM_ITEMHINT	        152
// 鼠标离开控件
#define LKTREECTRL_MSG_WPARAM_MOUSELEAVE        153
// 鼠标左键在树形控件上按下
#define LKTREECTRL_MSG_WPARAM_LBUTTONDOWN       154
// 取消提示消息
#define LKTREECTRL_MSG_WPARAM_ITEMCANCELHINT    155
// 项双击消息
#define LKTREECTRL_MSG_WPARAM_ITEMDBCLICKED	    156

// LISTCTRL
// 项改变消息
#define LKLISTCTRL_MSG_WPARAM_ITEMCHANGED		200
#define LKLISTCTRL_MSG_WPARAM_HEADWIDTHCHANGED	201

// DRAWER
// 项改变消息
#define LKDRAWER_MSG_WPARAM_ITEMCHANGED		    210
// 子项按钮单击消息
#define LKDRAWER_MSG_WPARAM_ITEMBTNCLICKED	    211
// 子项复选按钮选中消息
#define LKDRAWER_MSG_WPARAM_ITEMBTNSELECTED	    212
// 子项复选按钮取消选中消息
#define LKDRAWER_MSG_WPARAM_ITEMBTNCANCEL	    213

// 垂直滚动条消息
#define LKCTRLBASEEX_MSG_WPARAM_VSCROLLBAR		300
// 水平滚动条消息
#define LKCTRLBASEEX_MSG_WPARAM_HSCROLLBAR		301
// 垂直滚动条向上滚动消息
#define LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARUP	303
// 垂直滚动条向下滚动消息
#define LKCTRLBASEEX_MSG_WPARAM_VSCROLLBARDOWN	304
// 显示滚动条消息
#define LKCTRLBASEEX_MSG_WPARAM_SHOWSCROLLBAR	305
// TAB
// 项改变消息
#define LKTAP_MSG_WPARAM_ITEMCHANGED		    400
// 项正在关闭消息
#define LKTAP_MSG_WPARAM_ITEMCLOSING	        401
// 项已经关闭消息
#define LKTAP_MSG_WPARAM_ITEMCLOSED		        402
// 项左滚动消息
#define LKTAP_MSG_WPARAM_ITEMLSCROLL		    403
// 项右滚动消息
#define LKTAP_MSG_WPARAM_ITEMRSCROLL		    404

// CLKScaleSplidWnd 控件
// 项展开
#define LKSCALESPLIDWND_MSG_WPARAM_OPEN		    500
// 项展开
#define LKSCALESPLIDWND_MSG_WPARAM_CLOSE	    501

// CLKSplit 分隔条拖动
#define LKSPLIT_MSG_DRAG                        600

// CLKRichEdit 控件消息获取ID对应的CLKImage指针
#define LKRICHEDIT_MSG_WPARAM_GETIMAGEBYID      620

// CLKContainer消息
// 项中按钮单击消息
#define LKCONTAINER_MSG_WPARAM_ITEMBTNCLICKED	650


// 控件对应的资源列表起始和结束索引
// 通用资源
#define LKIMAGELISTINDEX_COMMON_BEGIN			0
// 通用资源空图片
#define LKIMAGELISTINDEX_COMMON_NULL			0
// 列表类１像素通用边框
#define LKIMAGELISTINDEX_COMMON_CTRLFRAME1		1
// 23像素高顶部通用蒙板
#define LKIMAGELISTINDEX_COMMON_TOPMASK23		2
// 27像素高底部通用蒙板
#define LKIMAGELISTINDEX_COMMON_BOTTOMMASK27	3
// 21像素高选中和鼠标移动高亮
#define LKIMAGELISTINDEX_COMMON_SELECTANDFOCUS	4
// 消息图标
#define LKIMAGEBUTTONINDEX_COMMON_ICONMESSAGE	5
// 更换皮肤图标
#define LKIMAGEBUTTONINDEX_COMMON_ICONSKIN		6
// 业务图标
#define LKIMAGEBUTTONINDEX_COMMON_ICONBUSINESS	7
// 增加图标
#define LKIMAGEBUTTONINDEX_COMMON_ICONADD		8
// 消息通知图标
#define LKIMAGEBUTTONINDEX_COMMON_ICONNOTIFY	9
// 资料修改图标
#define LKIMAGEBUTTONINDEX_COMMON_ICONMODIFY	10
// 删除图标
#define LKIMAGEBUTTONINDEX_COMMON_ICONDELETE	11
// 滚动条图片(默认为垂直滚动条)
#define LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR		12
// 滚动条图片(默认为垂直滚动条)
#define LKIMAGEBUTTONINDEX_COMMON_VSCROLLBAR	LKIMAGEBUTTONINDEX_COMMON_SCROLLBAR
// 水平滚动条图片
#define LKIMAGEBUTTONINDEX_COMMON_HSCROLLBAR	13
// 滚动条图片宽为10(默认为垂直滚动条)
#define LKIMAGEBUTTONINDEX_COMMON_VSCROLLBAR10	14
// 删除图标
#define LKIMAGEBUTTONINDEX_COMMON_ICONDELETE15	15
// 弹出提示背景图片
#define LKIMAGEBUTTONINDEX_COMMON_DIALOGBALLOON	16
// 头像外框图片
#define LKIMAGEBUTTONINDEX_COMMON_HEADFRAME		17
// 皮肤按钮外框图片
#define LKIMAGEBUTTONINDEX_COMMON_SKINFRAME		18
// 皮肤按钮三角形删除图片
#define LKIMAGEBUTTONINDEX_COMMON_BUTTONDELETE	19
// 皮肤按钮无皮肤默认出错图片
#define LKIMAGEBUTTONINDEX_COMMON_NOSKIN		20
// 消息框提示图标
#define LKIMAGEBUTTONINDEX_COMMON_MESSAGEHINT	21
// 标签提示背景
#define LKIMAGEBUTTONINDEX_COMMON_LABELBG	    22
// 提示对话框通用背景
#define LKIMAGEHINTDIALOG_COMMON_BG	            23
// 删除图标2
#define LKIMAGEINDEX_COMMON_ICONDELETE3_15	    24
// 搜索图标
#define LKIMAGEINDEX_COMMON_ICONDSEARCH			25
// 6像素高顶部通用蒙板
#define LKIMAGELISTINDEX_COMMON_TOPMASK6		26
// 获得焦点框
#define LKIMAGELISTINDEX_COMMON_FOCUS			27

#define LKIMAGELISTINDEX_COMMON_END				49

// 对话框
#define LKIMAGELISTINDEX_DIALOG_BEGIN			50
#define LKIMAGELISTINDEX_DIALOG_END				60
// 日历
#define LKIMAGELISTINDEX_MONTHCAL_BEGIN			61
#define LKIMAGELISTINDEX_MONTHCAL_END			63
// 下拉日期时间
#define LKIMAGELISTINDEX_DTAETIME_BEGIN			64
#define LKIMAGELISTINDEX_DTAETIME_END			66
// 缩放隐藏控件
#define LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN    67
// 左开
#define LKIMAGELISTINDEX_SCALESPLIDWND_LOPEN    LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN
// 左关
#define LKIMAGELISTINDEX_SCALESPLIDWND_LCLOSE   LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN + 1
// 右开
#define LKIMAGELISTINDEX_SCALESPLIDWND_ROPEN    LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN + 2
// 右关
#define LKIMAGELISTINDEX_SCALESPLIDWND_RCLOSE   LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN + 3
#define LKIMAGELISTINDEX_SCALESPLIDWND_END      70
// 按钮
#define LKIMAGELISTINDEX_BUTTON_BEGIN			71
#define LKIMAGELISTINDEX_BUTTON_END				74
// 抽屉项
#define LKIMAGELISTINDEX_DRAWERITEM_BEGIN		75
#define LKIMAGELISTINDEX_DRAWERITEM_END			76
// 抽屉
#define LKIMAGELISTINDEX_DRAWER_BEGIN			77
#define LKIMAGELISTINDEX_DRAWER_END				78
// 编辑框
#define LKIMAGELISTINDEX_EDIT_BEGIN				80
#define LKIMAGELISTINDEX_EDIT_END				83
// 复选框
#define LKIMAGELISTINDEX_CHECKBOX_BEGIN			85
#define LKIMAGELISTINDEX_CHECKBOX_END			86

// 列表控件
#define LKIMAGELISTINDEX_LISTCTRL_BEGIN			90
#define LKIMAGELISTINDEX_LISTCTRL_END			95

// 列表列头控件
#define LKIMAGELISTINDEX_LISTCTRLHEADER_BEGIN	96
#define LKIMAGELISTINDEX_LISTCTRLHEADER_END		98

// 分页控件
#define LKIMAGELISTINDEX_TAB_BEGIN				99
#define LKIMAGELISTINDEX_TABHEADERBG			LKIMAGELISTINDEX_TAB_BEGIN + 0
#define LKIMAGELISTINDEX_TABHEADER				LKIMAGELISTINDEX_TAB_BEGIN + 1
#define LKIMAGELISTINDEX_TABHEADER2				LKIMAGELISTINDEX_TAB_BEGIN + 2
// 左箭头
#define LKIMAGELISTINDEX_TABEXLEFTARR			LKIMAGELISTINDEX_TAB_BEGIN + 3
// 右箭头
#define LKIMAGELISTINDEX_TABEXRIGHTARR			LKIMAGELISTINDEX_TAB_BEGIN + 4
#define LKIMAGELISTINDEX_TAB_END				103

// 树形控件
#define LKIMAGELISTINDEX_TREECTRL_BEGIN			105
#define LKIMAGELISTINDEX_TREECTRL_END			108

// 组合框
#define LKIMAGELISTINDEX_COMBOBOX_BEGIN			110
#define LKIMAGELISTINDEX_COMBOBOX_END			112

// 单选框
#define LKIMAGELISTINDEX_RADIOBOX_BEGIN			114
#define LKIMAGELISTINDEX_RADIOBOX_END			116

// 菜单
#define LKIMAGELISTINDEX_MENU_BEGIN			    118
#define LKIMAGELISTINDEX_MENU_END			    119
// 通用菜单背景
#define LKIMAGELISTINDEX_MENU_BG			    118

// 进度条
#define LKIMAGELISTINDEX_PROGRESSBAR_BEGIN		120
#define LKIMAGELISTINDEX_PROGRESSBAR_END		121
// 进度条背景
#define LKIMAGELISTINDEX_PROGRESSBAR_BG			120


// 项目特有图片存储区域起始索引(非共享图片)
#define LKPROJECT_INDEX_BEGIN					200
// 日志类型
// 对话框WM_PAINT
//#define LKDIALOG_PAINT_LOG 
// 基类控件WM_PAINT
//#define LKBASECTRL_PAINT_LOG
// 对话框WM_NCPAINT
//#define LKDIALOG_NCPAINT_LOG 




#define LoadAllResourceImageToImageArray(pImgMgr)\
{\
	pImgMgr->InitImageArray(400);\
	/*通用资源图片*/\
	int i = LKIMAGELISTINDEX_COMMON_BEGIN;\
	/*默认的控件非客户区通用空图片*/\
	pImgMgr->SetImage(i++);\
	/*默认的控件非客户区通用框架图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_CTRL_FRAME, CRect(2, 26, 2, 2), 1);\
	/*默认的客户区顶部蒙板图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_TOPMASK23, CRect(4, 0, 4, 0), 1);\
	/*默认的客户区底部蒙板图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_BOTTOMMASK27);\
	/*默认的选中和获取高亮图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_SELECTANDFOCUS2_21, CRect(5, 0, 6, 0), 2);\
	/*默认的消息图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_MESSAGE);\
	/*默认更换皮肤图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_SKIN);\
	/*默认业务图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_BUSINESS);\
	/*默认增加图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_ADD);\
	/*默认消息通知图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_NOTIFY);\
	/*资料修改图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_MODIIFY);\
	/*删除图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_DELETE2_16, CRect(0, 0, 0, 0), 2);\
	/*滚动条图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_SCROLLBAR4_14, CRect(0, 6, 0, 6), 4);\
	/*水平滚动条图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_HSCROLLBAR4_14, CRect(6, 0, 7, 0), 4);\
	/*滚动条图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_SCROLLBAR4_10, CRect(0, 6, 0, 6), 4);\
	/*删除图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICON_DELETE2_15, CRect(0, 0, 0, 0), 2);\
	/*弹出提示背景图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_DIALOG_BALLOON, CRect(15, 10, 5, 5), 1);\
	/*头像外框图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_HEADFRAME, CRect(6, 6, 6, 6), 1);\
	/*大按钮背景框图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_SKIN, CRect(4, 4, 4, 4), 2);\
	/*大按钮删除三角形图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_BUTTONDELETE, CRect(0, 0, 0, 0), 2);\
	/*皮肤按钮无皮肤默认出错图片*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_NOSKIN);\
	/*消息框提示图标*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_MESSAGEHINT);\
	/*标签提示背景*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_LABEL, CRect(5, 5, 5, 5), 1);\
	/*提示对话框通用背景*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_HINTDIALOG_BG, CRect(10, 35, 10, 35), 1);\
	/*删除图标2*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_ICONDELETE3_15, CRect(0, 0, 0, 0), 3); \
	/*搜索图标*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_SEARCH); \
	/*默认的客户区顶部蒙板图片6象素高*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_TOPMASK6); \
	/*获得焦点框*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_FOCUS, CRect(12, 12, 12, 12)); \
	/*通用资源图片结束*/\
	/*默认的对话框框架图片*/\
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
	/*日历图片*/\
	i = LKIMAGELISTINDEX_MONTHCAL_BEGIN;\
	pImgMgr->SetPNGImage(i++, IDB_PNG_CALENDAR_BUTTONBG3_17_16, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_CALENDAR_BG, CRect(3, 3, 3, 3), 1);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_CALENDAR_ARR2, CRect(0, 0, 0, 0), 2);\
	/*下拉日期时间图片*/\
	i = LKIMAGELISTINDEX_DTAETIME_BEGIN;\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_DTAETIME_DEL3_17_16, CRect(2, 2, 2, 2), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_DTAETIME_DOWN3_17_16, CRect(2, 2, 2, 2), 3);\
	/*默认的缩放隐藏控件图片*/\
	i = LKIMAGELISTINDEX_SCALESPLIDWND_BEGIN;\
	pImgMgr->SetPNGImage(i++, IDB_PNG_SCALESPLIDWND_IMAGELO3_11_205, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_SCALESPLIDWND_IMAGELC3_11_205, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_SCALESPLIDWND_IMAGERC3_11_205, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_SCALESPLIDWND_IMAGERO3_11_205, CRect(0, 0, 0, 0), 3);\
	/*默认的按钮图片*/\
	i = LKIMAGELISTINDEX_BUTTON_BEGIN;/*按钮图片起始索引*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_COMMON2_20, CRect(2, 2, 2, 2), 2);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_COMMON3_20, CRect(2, 2, 2, 2), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_BUTTON_COMMON4_22, CRect(2, 2, 2, 2), 4);\
	/*默认的抽屉项图片*/\
	i = LKIMAGELISTINDEX_DRAWERITEM_BEGIN;/*按钮图片起始索引*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_DRAWERHEADER, CRect(17, 0, 0, 0), 4);\
	/*默认的抽屉图片*/\
	i = LKIMAGELISTINDEX_DRAWER_BEGIN;/*按钮图片起始索引*/\
	pImgMgr->SetImage(i++);\
	/*默认的编辑框图片*/\
	i = LKIMAGELISTINDEX_EDIT_BEGIN;/*编辑框图片起始索引*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_EDIT_COMMON2_24, CRect(5, 5, 5, 5), 2);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_EDIT_COMMON2_26, CRect(20, 8, 20, 8), 2);\
	/*默认的复选框图片*/\
	i = LKIMAGELISTINDEX_CHECKBOX_BEGIN;/*复选框图片起始索引*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_CHECKBOX_COMMON4, CRect(0, 0, 0, 0), 4);\
	/*默认的列表控件图片*/\
	i = LKIMAGELISTINDEX_LISTCTRL_BEGIN;/*列表控件图片起始索引*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_LISTCTRL_BG, CRect(2, 22, 2, 2), 1);\
	/*默认的TAB控件图片*/\
	i = LKIMAGELISTINDEX_TAB_BEGIN;/*列表TAB控件图片起始索引*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TAB_HEADER, CRect(6, 0, 6, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TAB_HEADER3_32, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TABEX_LEFTARR3_13, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TABEX_RIGHTARR3_13, CRect(0, 0, 0, 0), 3);\
	/*默认的树形控件图片*/\
	i = LKIMAGELISTINDEX_TREECTRL_BEGIN;/*树形控件图片起始索引*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TREECTRL_EXPAND1, CRect(0, 0, 0, 0), 2);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_TREECTRL_EXPAND2, CRect(0, 0, 0, 0), 2);\
	/*默认的组合框控件图片*/\
	i = LKIMAGELISTINDEX_COMBOBOX_BEGIN;/*组合框控件图片起始索引*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMBOBOXCTRL_BUTTON, CRect(0, 0, 0, 0), 3);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMBOBOXCTRL_BUTTON3_26, CRect(0, 0, 0, 0), 3);\
	/*默认的复选框图片*/\
	i = LKIMAGELISTINDEX_RADIOBOX_BEGIN;/*复选框图片起始索引*/\
	pImgMgr->SetImage(i++);\
	pImgMgr->SetPNGImage(i++, IDB_PNG_RADIOBOX_COMMON4, CRect(0, 0, 0, 0), 4);\
	/*默认的菜单图片*/\
	i = LKIMAGELISTINDEX_MENU_BEGIN;/*菜单图片起始索引*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_MENUBG, CRect(31, 5, 5, 5), 1);\
	/*默认的进度条图片*/\
	i = LKIMAGELISTINDEX_PROGRESSBAR_BEGIN;/*进度条图片起始索引*/\
	pImgMgr->SetPNGImage(i++, IDB_PNG_COMMON_PROGRESSBG, CRect(5, 0, 5, 0), 1);\
}



//	pImgMgr->SetPNGImage(i++, IDB_PNG_EDIT_COMMON2_26, CRect(20, 8, 20, 8), 2);\
