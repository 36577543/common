#ifndef _LOCALPROTOCOL_H
#define _LOCALPROTOCOL_H

#pragma once
#include <string>

using namespace std;

/// ��������
/// �˳��߳�
#define PIPEPACKETHEADER_CODE_EXITTHREAD	1

/// ע��ͻ���
#define CS_PIPE_CODE_REGEDIT   		2
/// ִ��ָ������
#define CS_PIPE_CODE_EXECCODE		3
/// �������ݵ�ƽ̨
#define CS_PIPE_CODE_SENDDATA  		4

/// ע��ͻ���
#define SC_PIPE_CODE_REGEDIT   		102
/// ִ��ָ������
#define SC_PIPE_CODE_EXECCODE		103
/// �������ݵ�ƽ̨
#define SC_PIPE_CODE_SENDDATA  		104

/// Ҫִ�еĺ�����ʽ
typedef int(__stdcall *PFun)(void *);

#pragma pack(1)
/// �ܵ����ݸ�ʽ
struct PipePacketHeader
{
	// Ӧ�ýӿ�Id
	int nCChannelId;
	/// ��������
	unsigned short nCode;
	/// ���ݳ���(ȫ��)
	int nPacketLen;
	/// ��������
	void InitHeader(int nChannelIdA, unsigned short nCodeA, int nPacketLenA){ nCChannelId = nChannelIdA; nCode = nCodeA; nPacketLen = nPacketLenA; }
};

// ע��Ӧ�ñ���
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

// ע��Ӧ�÷��ر���
struct CReRegeditPipePacket : public PipePacketHeader
{
	int nChannelId;
	void Init(int nChannelIdA){ InitHeader(0, CS_PIPE_CODE_REGEDIT, sizeof(*this)); nChannelId = nChannelIdA; }
};

// ע��Ӧ�ñ���
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

/// ִ�к������ݸ�ʽ
struct PipePacketExec
{
	PFun pFun;
	void *pParam;
};
#pragma pack()



#endif