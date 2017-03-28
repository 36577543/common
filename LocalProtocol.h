#ifndef _LOCALPROTOCOL_H
#define _LOCALPROTOCOL_H

#pragma once
#include <string>

using namespace std;

/// 动作代码
/// 退出线程
#define PIPEPACKETHEADER_CODE_EXITTHREAD	1

/// 注册客户端
#define CS_PIPE_CODE_REGEDIT   		2
/// 执行指定代码
#define CS_PIPE_CODE_EXECCODE		3
/// 发送数据到平台
#define CS_PIPE_CODE_SENDDATA  		4

/// 注册客户端
#define SC_PIPE_CODE_REGEDIT   		102
/// 执行指定代码
#define SC_PIPE_CODE_EXECCODE		103
/// 发送数据到平台
#define SC_PIPE_CODE_SENDDATA  		104

/// 要执行的函数格式
typedef int(__stdcall *PFun)(void *);

#pragma pack(1)
/// 管道内容格式
struct PipePacketHeader
{
	// 应用接口Id
	int nCChannelId;
	/// 动作代码
	unsigned short nCode;
	/// 数据长度(全长)
	int nPacketLen;
	/// 数据内容
	void InitHeader(int nChannelIdA, unsigned short nCodeA, int nPacketLenA){ nCChannelId = nChannelIdA; nCode = nCodeA; nPacketLen = nPacketLenA; }
};

// 注册应用报文
struct CRegeditPipePacket : public PipePacketHeader
{
	wchar_t szChannelName[1];
	static PipePacketHeader *CreatPacket(const wstring &strChannelName)
	{
		PipePacketHeader *pRet(0);
		int nLen = sizeof(PipePacketHeader)+strChannelName.size() * sizeof(wchar_t)+2;
		pRet = (PipePacketHeader *)new char[nLen];
		memset(pRet, 0, nLen);
		pRet->InitHeader(0, CS_PIPE_CODE_REGEDIT, nLen);
		memcpy((char *)(pRet + 1), strChannelName.c_str(), strChannelName.size() * sizeof(wchar_t));
		return pRet;
	}
};

// 注册应用返回报文
struct CReRegeditPipePacket : public PipePacketHeader
{
	int nChannelId;
	void Init(int nChannelIdA){ InitHeader(0, CS_PIPE_CODE_REGEDIT, sizeof(*this)); nChannelId = nChannelIdA; }
};

// 注册应用报文
struct CSendDataPipePacket : public PipePacketHeader
{
	//
};

struct CReSendDataPipePacket : public PipePacketHeader
{
	char szChannelName[1];
	//
	static PipePacketHeader *CreatePacket(void *pData, int nLenA)
	{
		PipePacketHeader *pRet(0);
		int nLen = sizeof(PipePacketHeader)+nLenA + 1;
		pRet = (PipePacketHeader *)new char[nLen];
		memset(pRet, 0, nLen);
		pRet->InitHeader(0, SC_PIPE_CODE_SENDDATA, nLen);
		memcpy((char *)(pRet + 1), pData, nLenA);
		return pRet;
	}
};

/// 执行函数内容格式
struct PipePacketExec
{
	PFun pFun;
	void *pParam;
};
#pragma pack()



#endif