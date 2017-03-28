// CommonStruct.h
#ifndef _COMMONSTRUCT_H
#define _COMMONSTRUCT_H
#include <string>
#include "datatypedef.h"

using namespace std;

// PUBWIN客户端用户信息
typedef struct _PUBWINBARCLIENT_INFO
{
	//	本机信息
	char btClientStatus;				//	机器状态

	//	用户信息
	char szUserID[21];					//	当前用户编号
	int nUserType;						//	当前用户类型
	char szCardSN[21];					//	当前用户卡号
	int nCerOriginType;					//	实名类型
	char szUserName[21];				//	用户姓名
	char szCertType[21];				//	证件类型
	char szCertID[21];					//	证件编号
}PUBWINBARCLIENT_INFO;

// 用户基本信息
class CUserInfo
{
public:
	//
	string id;
	//int nMachineId;
	string strUserId;
	// 用户姓名
	string strUserName;
	//机器名
	string strMachineName;
	// 用户昵称
	string strNickName;
	// 用户证件类型
	string strUserIDType;
	// 用户证件号码
	string strUserIDNum;
	//头像ID
	string strHeadId;
	// 用户性别
	string strUserSex;
	// 用户E-mail
	string strUserEmail;
	// 用户电话
	string strUserPhone;
	// 虚拟货币1
	int64 nMoney1 = 0;
	// 虚拟货币2
	int64 nMoney2 = 0;
	// 虚拟物品1（鲜花）
	int64 nVirtualItem1 = 0;
	// 虚拟物品2（火箭）
	int64 nVirtualItem2 = 0;
	// 是否获取到了网吧配置
	int nGetConfig = 0;
};

// {"id":"cb5e90555bc548d9861a163ad8eafde2","broadContent":"欢迎来到虎猫网咖","broadTime":"13:00","repeatType":"5","ba_1":"1"}

struct YYBBItem
{
	// id
	string id;
	// broadContent
	string broadContent;
	// broadContent2
	string broadContent2;
	// broadTime
	string broadTime;
	// repeatType
	string repeatType;
	// ba_1 状态【0删除，1打开，2关闭】;
	string ba_1;
	// ba_1 状态【0。。。。，1-置顶，2。。。。】;
	string ba_2;
	// 上次播放时间(仅对自动扫描有效)
	int64 llBBTime = 0;
};

// 网吧配置项
struct ConfigItem
{
	// 语音播报URL
	string strYYBBUrl;
	// 禁止语音播报时段(开始时间 | 结束时间)
	string strForbidTime;
	// 是否开启语音播报（1 – 开启； 0 – 关闭）
	string strYYBBSwitch;
	// 是否开启客户端呼叫（1 – 开启； 0 – 关闭）
	string strCallServerSwitch;
	// 网吧兵币
	string strSoliderMoney;
	// 网吧冻结兵币
	string strIceSoliderMoney;
	//网吧名称
	string strNetBarName;
	// TGP全路径名
	string strTGPName;
	// 是否自动启动微赛事("0" - 不自动启动微赛事； "1" - 自动启动微赛事)
	string strIsStratMatch;
	// 微赛事相对路径名
	string strMatchName;
	// LOL全路径名
	string strLOLPathName;
	//// 是否自动启动同
	//string strTGPName;
	//// TGP全路径名
	//string strTGPName;
};

// 弹幕项
struct CSSItem
{
	// {"id":"c767cfe90a564fce95bf2d35743e417d",
	string id;
	// "barrageContent":"破开鸡婆111",
	string barrageContent;
	// "barrageTime":"2:30",
	string barrageTime;
	// "barrageRange":"",
	string barrageRange;
	// "rangeNumber":"",
	string rangeNumber;
	// "repeatType":"1",
	string repeatType;
	// ba_1 状态【0删除，1打开，2关闭】;
	string ba_1;
	// 上次播放时间(仅对自动扫描有效)
	int64 llBBTime = 0;
};

// 用户游戏信息
typedef struct UserGameInfoItem
{
	UserGameInfoItem(){}
	UserGameInfoItem(const string &strTierA, const string &strLevelA, 
		const string &strWinA, 
		const string &strAreaIdA, 
		const string &strUserQQIdA, 
		const string strMachineNameA,
		const string &strNickNameA,
		string strStateA = "0")
	{
		strTier = strTierA;
		strLevel = strLevelA;
		strWin = strWinA;
		strAreaId = strAreaIdA;
		strUserQQId = strUserQQIdA;
		strNickName = strNickNameA;
		strMachineName = strMachineNameA;
		strState = strStateA;
	}
	string strTier;
	string strLevel;
	string strWin;
	string strAreaId;
	string strUserQQId;
	string strNickName;
	string strMachineName;
	// 3.游戏状态（0 - 打开游戏； 1 - 退出游戏；2 - 开始游戏；...... ） 
	string strState;
}*PUserGameInfoItem;

// 用户信息
typedef struct UserInfoItem
{
	UserInfoItem(){ bGameInfoValid = false; }
	// 用户IP-端口
	NetAddr addr;
	// 用户基本信息
	CUserInfo baseInfo;
	// 游戏信息是否有效
	bool bGameInfoValid;
	// 游戏信息
	UserGameInfoItem gameInfo;
}*PUserInfoItem;

#endif