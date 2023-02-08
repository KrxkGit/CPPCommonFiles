#pragma once

#include<WinSock2.h>
#include<WS2tcpip.h>
#include<time.h>
#pragma comment(lib,"ws2_32.lib")

struct NTP_Packet
{
	int Control_Word;
	int root_delay;
	int root_dispersion;
	int reference_identifier;
	__int64 reference_timestamp;
	__int64 originate_timestamp;
	__int64 receive_timestamp;
	int transmit_timestamp_seconds;
	int transmit_timestamp_fractions;
};

int GetInternetTime(LPSYSTEMTIME pSt)
{
	WSADATA ws;
	int iResult = WSAStartup(MAKEWORD(2, 2), &ws);
	SOCKET ConnectSocket = INVALID_SOCKET;
	if (iResult != 0) {
		return FALSE;
	}

	ADDRINFO *result = NULL, hint;
	ZeroMemory(&hint, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_protocol = IPPROTO_UDP;

	iResult = getaddrinfo("time.windows.com", "123", &hint, &result);
	if (iResult != 0) {
		WSACleanup();
		return FALSE;
	}
	ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return FALSE;
	}
	iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		WSACleanup();
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}
	freeaddrinfo(result);
	//发送
	NTP_Packet NTP_Send, NTP_Recv;
	NTP_Send.Control_Word = htonl(0x0B000000);
	NTP_Send.root_delay = 0;
	NTP_Send.root_dispersion = 0;
	NTP_Send.reference_identifier = 0;
	NTP_Send.reference_timestamp = 0;
	NTP_Send.originate_timestamp = 0;
	NTP_Send.receive_timestamp = 0;
	NTP_Send.transmit_timestamp_seconds = 0;
	NTP_Send.transmit_timestamp_fractions = 0;
	iResult = send(ConnectSocket, (char*)&NTP_Send, sizeof(NTP_Send), 0);
	//接收
	iResult = recv(ConnectSocket, (char*)&NTP_Recv, sizeof(NTP_Recv), 0);
	//清理
	closesocket(ConnectSocket);
	WSACleanup();
	//转换时间
	struct tm *lpLocalTime = new tm;
	time_t ntp_time = NULL;

	// 获取时间服务器的时间
	ntp_time = ntohl(NTP_Recv.transmit_timestamp_seconds) - 2208988800;
	localtime_s(lpLocalTime, &ntp_time);
	if (lpLocalTime == NULL)
	{
		return FALSE;
	}

	//填充结构
	pSt->wYear = lpLocalTime->tm_year + 1900;
	pSt->wMonth = lpLocalTime->tm_mon + 1;
	pSt->wDayOfWeek = lpLocalTime->tm_wday;
	pSt->wDay = lpLocalTime->tm_mday;
	pSt->wHour = lpLocalTime->tm_hour;
	pSt->wMinute = lpLocalTime->tm_min;
	pSt->wSecond = lpLocalTime->tm_sec;

	delete lpLocalTime;
	return TRUE;
}