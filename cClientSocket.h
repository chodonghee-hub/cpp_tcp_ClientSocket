#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include "cClntObj.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class cClientSocket
{
private :
	WSADATA		wsaData;
	SOCKET		clnt;
	SOCKADDR_IN	addr;
	LPVOID		p_cManager;
	bool		bConnectionFlag;

public :
	cClientSocket(LPVOID _lp);
	~cClientSocket();

	void __Init__();
	void _SetSocketInfo();
	bool bCheckWinSock();
	bool bCheckValidSocket(SOCKET* _sock);
	bool bCheckConnection(int _nPortNum);
	void _BreakObjFunc(cClntObj* _pObj);
};

