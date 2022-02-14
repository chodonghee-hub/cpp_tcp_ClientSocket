#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <queue>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;


class cClntObj
{
private:
	LPVOID		p_ClientSock;
	SOCKET*		p_Server;
	mutex		mtx;
	bool		b_ClntObjFlag;
	bool		b_MessageFlag;
	char		cBuffer[1024];
	queue<char*>qMessageQueue;

public:
	cClntObj(LPVOID _pServerSocket, SOCKET* _pSocket);
	~cClntObj();

	void __ClntInit__();
	void _BreakConnection();
	void _ReadBuffer();
	static void _RecvDataFrom(LPVOID lp);
	static void _SendDataTo(LPVOID lp);
};