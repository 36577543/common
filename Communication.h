#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include "windows.h"

// 回调函数指针类型
typedef void(WINAPI *PSTARTUDPSERVERCALLBACKFUN)(void *pCallbackParam, char *pRecvBuf, int nBufLen, const char *pRemoteIp, int nRemotePort);
// 回调函数指针类型
// 参数：
//   pCallbackParam - 传给回调函数的上层参数；
//   pConnect		- 此连接对象指针；
//   pRemoteIp		- 此连接中的远程Ip；
//   nRemotePort	- 此连接中的远程端口；
typedef void(WINAPI *PSTARTTCPSERVERCALLBACKFUN)(void *pCallbackParam, void *pConnect, const char *pRemoteIp, int nRemotePort);
// 回调函数指针类型
// 参数：
//   pCallbackParam - 传给回调函数的上层参数；
//   pSreamSocket	- 此连接SreamSocket对象指针；
//   pRemoteIp		- 此连接中的远程Ip；
//   nRemotePort	- 此连接中的远程端口；
typedef void(WINAPI *PSTARTTCPCLIENTCALLBACKFUN)(void *pCallbackParam, void *pSreamSocket, char *pSendBuf, int nBufLen);

// 回调函数指针类型
// 参数：
//   pCallbackParam - 传给回调函数的上层参数；
//   pWebSocketClient - 此连接SreamSocket对象指针；
typedef void(WINAPI *PWEBSOCKCLIENTCALLBACKFUN)(const void *pCallbackParam, const void *pWebSocketClient);
// 回调函数指针类型
// 参数：
//   pCallbackParam - 传给回调函数的上层参数；
//   pWebSocketClient - 此连接SreamSocket对象指针；
typedef void(WINAPI *PWEBSOCKCLIENTONRECV)(const void *pCallbackParam, const void *pBuf, int nLen);

// 请求Http数据
int WINAPI RequestHttpData(const char *sUrl, const char *strSource, int nSourceLen, const char *strContentType, char *strRet, int nSize, int &nState, bool bPost = true, bool bIsParamutf8 = false, bool bIsResultutf8 = false);
// 上传Http数据
int WINAPI PostHttpData(const char *sUrl, const char *strSource, int nSourceLen, const char *strContentType, char *strRet, int nSize, int &nState);
// 上传Http数据
int WINAPI PostHttpDataToUtf8(const char *sUrl, const char *strSource, int nSourceLen, const char *strContentType, char *strRet, int nSize, int &nState);
//// 上传Https或Https数据并将本地编码的strSource转码为Utf8
//int WINAPI PostDataToServer(const char *sUrl, const char *strSource, int nSourceLen, char *strRet, int nSize, int &nState);
// 获取Http返回数据
int WINAPI GetHttpData(const char *sUrl, char *strRet, int nSize, int &nState);
//// 获取Https返回数据
//int WINAPI GetHttpsData(const char *sUrl, char *strRet, int nSize, int &nState);
// 获取Http返回数据
int WINAPI GetHttpDataFromUtf8(const char *sUrl, char *strRet, int nSize, int &nState);
//// 获取Https返回数据
//int WINAPI GetHttpsDataFromUtf8(const char *sUrl, char *strRet, int nSize, int &nState);
// 获取Http或Https的返回数据并将Utf8解码为本地编码
int WINAPI GetDataFromServer(const char *sUrl, char *strRet, int nSize, int &nState);

// 启动UDP服务
int WINAPI StartUdpServer(int nPort, PSTARTUDPSERVERCALLBACKFUN pFun, void *pCallbackParam);
// 停止UDP服务
void WINAPI StopUdpServer();
// 发送UDP数据
int WINAPI SendUdpData(char *sRemoteIp, int nRemotePort, void *pBuf, int nBufLen);
// 下载目录
int WINAPI DownloadDirByFtp(const char *sUrl, const char *sLocalDir, const char *sUsername, const char *sPassword);
// 下载文件
int WINAPI DownloadFileByFtpAndUnZip(const char *sUrl, const char *sLocalDir, const char *sUsername, const char *sPassword, bool bUnZip);

// 启动TCP服务
int WINAPI StartTcpServer(int nPort, PSTARTTCPSERVERCALLBACKFUN pFun, void *pCallbackParam);
// 停止TCP服务
void WINAPI StopTcpServer();
// 通过服务端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
int WINAPI SendTcpDataByServerConnection(void *pConnect, void *pBuf, int nBufLen);
// 通过服务端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
int WINAPI RecvTcpDataByServerConnection(void *pConnect, void *pBuf, int nBufLen);

// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
int WINAPI SendTcpDataByClient(void *pCallbackParam, char *pRemoteIp, int nRemotePort, PSTARTTCPCLIENTCALLBACKFUN pFun, void *pBuf, int nBufLen);
// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
int WINAPI SendTcpDataByClientConnection(void *pSreamSocket, void *pBuf, int nBufLen);
// 通过客户端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
int WINAPI RecvTcpDataByClientConnection(void *pSreamSocket, void *pBuf, int nBufLen);

// Ping
int WINAPI PingByClient(const char *szIp);
// 将缓存解压到文件
int WINAPI UnzipFile(const char *szBuf, int nBufLen, const char *szFileName, char *szDestFileName, int nDestFileLen);

#endif