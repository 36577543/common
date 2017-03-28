#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include "windows.h"

// �ص�����ָ������
typedef void(WINAPI *PSTARTUDPSERVERCALLBACKFUN)(void *pCallbackParam, char *pRecvBuf, int nBufLen, const char *pRemoteIp, int nRemotePort);
// �ص�����ָ������
// ������
//   pCallbackParam - �����ص��������ϲ������
//   pConnect		- �����Ӷ���ָ�룻
//   pRemoteIp		- �������е�Զ��Ip��
//   nRemotePort	- �������е�Զ�̶˿ڣ�
typedef void(WINAPI *PSTARTTCPSERVERCALLBACKFUN)(void *pCallbackParam, void *pConnect, const char *pRemoteIp, int nRemotePort);
// �ص�����ָ������
// ������
//   pCallbackParam - �����ص��������ϲ������
//   pSreamSocket	- ������SreamSocket����ָ�룻
//   pRemoteIp		- �������е�Զ��Ip��
//   nRemotePort	- �������е�Զ�̶˿ڣ�
typedef void(WINAPI *PSTARTTCPCLIENTCALLBACKFUN)(void *pCallbackParam, void *pSreamSocket, char *pSendBuf, int nBufLen);

// �ص�����ָ������
// ������
//   pCallbackParam - �����ص��������ϲ������
//   pWebSocketClient - ������SreamSocket����ָ�룻
typedef void(WINAPI *PWEBSOCKCLIENTCALLBACKFUN)(const void *pCallbackParam, const void *pWebSocketClient);
// �ص�����ָ������
// ������
//   pCallbackParam - �����ص��������ϲ������
//   pWebSocketClient - ������SreamSocket����ָ�룻
typedef void(WINAPI *PWEBSOCKCLIENTONRECV)(const void *pCallbackParam, const void *pBuf, int nLen);

// ����Http����
int WINAPI RequestHttpData(const char *sUrl, const char *strSource, int nSourceLen, const char *strContentType, char *strRet, int nSize, int &nState, bool bPost = true, bool bIsParamutf8 = false, bool bIsResultutf8 = false);
// �ϴ�Http����
int WINAPI PostHttpData(const char *sUrl, const char *strSource, int nSourceLen, const char *strContentType, char *strRet, int nSize, int &nState);
// �ϴ�Http����
int WINAPI PostHttpDataToUtf8(const char *sUrl, const char *strSource, int nSourceLen, const char *strContentType, char *strRet, int nSize, int &nState);
//// �ϴ�Https��Https���ݲ������ر����strSourceת��ΪUtf8
//int WINAPI PostDataToServer(const char *sUrl, const char *strSource, int nSourceLen, char *strRet, int nSize, int &nState);
// ��ȡHttp��������
int WINAPI GetHttpData(const char *sUrl, char *strRet, int nSize, int &nState);
//// ��ȡHttps��������
//int WINAPI GetHttpsData(const char *sUrl, char *strRet, int nSize, int &nState);
// ��ȡHttp��������
int WINAPI GetHttpDataFromUtf8(const char *sUrl, char *strRet, int nSize, int &nState);
//// ��ȡHttps��������
//int WINAPI GetHttpsDataFromUtf8(const char *sUrl, char *strRet, int nSize, int &nState);
// ��ȡHttp��Https�ķ������ݲ���Utf8����Ϊ���ر���
int WINAPI GetDataFromServer(const char *sUrl, char *strRet, int nSize, int &nState);

// ����UDP����
int WINAPI StartUdpServer(int nPort, PSTARTUDPSERVERCALLBACKFUN pFun, void *pCallbackParam);
// ֹͣUDP����
void WINAPI StopUdpServer();
// ����UDP����
int WINAPI SendUdpData(char *sRemoteIp, int nRemotePort, void *pBuf, int nBufLen);
// ����Ŀ¼
int WINAPI DownloadDirByFtp(const char *sUrl, const char *sLocalDir, const char *sUsername, const char *sPassword);
// �����ļ�
int WINAPI DownloadFileByFtpAndUnZip(const char *sUrl, const char *sLocalDir, const char *sUsername, const char *sPassword, bool bUnZip);

// ����TCP����
int WINAPI StartTcpServer(int nPort, PSTARTTCPSERVERCALLBACKFUN pFun, void *pCallbackParam);
// ֹͣTCP����
void WINAPI StopTcpServer();
// ͨ������˷���TCP����(���̰߳�ȫ��ע�ⲻҪ�ٲ�ͬ���߳������)
int WINAPI SendTcpDataByServerConnection(void *pConnect, void *pBuf, int nBufLen);
// ͨ������˽���TCP����(���̰߳�ȫ��ע�ⲻҪ�ٲ�ͬ���߳������)
int WINAPI RecvTcpDataByServerConnection(void *pConnect, void *pBuf, int nBufLen);

// ͨ���ͻ��˷���TCP����(���̰߳�ȫ��ע�ⲻҪ�ٲ�ͬ���߳������)
int WINAPI SendTcpDataByClient(void *pCallbackParam, char *pRemoteIp, int nRemotePort, PSTARTTCPCLIENTCALLBACKFUN pFun, void *pBuf, int nBufLen);
// ͨ���ͻ��˷���TCP����(���̰߳�ȫ��ע�ⲻҪ�ٲ�ͬ���߳������)
int WINAPI SendTcpDataByClientConnection(void *pSreamSocket, void *pBuf, int nBufLen);
// ͨ���ͻ��˽���TCP����(���̰߳�ȫ��ע�ⲻҪ�ٲ�ͬ���߳������)
int WINAPI RecvTcpDataByClientConnection(void *pSreamSocket, void *pBuf, int nBufLen);

// Ping
int WINAPI PingByClient(const char *szIp);
// �������ѹ���ļ�
int WINAPI UnzipFile(const char *szBuf, int nBufLen, const char *szFileName, char *szDestFileName, int nDestFileLen);

#endif