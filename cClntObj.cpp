#include "cClntObj.h"
#include "cClientSocket.h"

cClntObj::cClntObj(LPVOID _pClientSocket, SOCKET* _pClient)
{
	cout << "[ Client Accepted ]" << endl;
	p_ClientSock = _pClientSocket;
	p_Server = nullptr;
	p_Server = _pClient;
	b_ClntObjFlag = true;
	b_MessageFlag = false;
	__ClntInit__();
}

cClntObj::~cClntObj()
{
	cout << "▶ Delete Clnt Object" << endl;
}

//##################################################################
// 설명 :	cClntObj 시작 ( Receive, Send 쓰레드 설정 ) 
//##################################################################
void cClntObj::__ClntInit__()
{
	thread RecvThread = thread(_RecvDataFrom, this);
	thread SendThread = thread(_SendDataTo, this);

	RecvThread.detach();
	SendThread.detach();
	while (b_ClntObjFlag) { Sleep(1); }
	_BreakConnection();
}

//##################################################################
//	설명 :	p_Clnt로부터 받은 데이터를 cBuffer에 저장 
//##################################################################
void cClntObj::_RecvDataFrom(LPVOID lp)
{
	cClntObj* p = (cClntObj*)lp;
	while (p->b_ClntObjFlag)
	{
		try
		{
			recv(*p->p_Server, p->cBuffer, 1024, 0);
			if (strncmp(p->cBuffer, "exit", strlen(p->cBuffer)) == 0)
			{
				cout << "[ Break Connection ]" << endl;
				p->b_ClntObjFlag = false;
			}
			else
			{
				p->qMessageQueue.push(p->cBuffer);
				p->_ReadBuffer();
			}			
		}

		catch (int e)
		{
			cout << "** Termiante connection from Server ..." << endl;
			p->b_ClntObjFlag = false;
		}
		ZeroMemory(p->cBuffer, strlen(p->cBuffer));
		Sleep(1);
	}
	ZeroMemory(p->cBuffer, strlen(p->cBuffer));
}

//##################################################################
//	설명 :	입력 받은 데이터를 p_Clnt에 전달 
//##################################################################
void cClntObj::_SendDataTo(LPVOID lp)
{
	cClntObj* p = (cClntObj*)lp;
	while (p->b_ClntObjFlag)
	{
		cout << "\n● Text your message : ";
		cin >> p->cBuffer;

		if (strncmp(p->cBuffer, "exit", strlen(p->cBuffer)) == 0)
		{
			cout << "[ Break Connection ]" << endl;
			p->b_ClntObjFlag = false;
		}
		else
		{
			send(*p->p_Server, p->cBuffer, strlen(p->cBuffer), 0);
		}
		ZeroMemory(p->cBuffer, strlen(p->cBuffer));
		Sleep(1);
	}  
	ZeroMemory(p->cBuffer, strlen(p->cBuffer));
}

//##################################################################
//	설명 :	Receive, Send에서 exit 호출 시, p_Clnt삭제 (cServerSocket)
//##################################################################
void cClntObj::_BreakConnection()
{
	cout << "▶ _BreakConnection()" << endl;
	cClientSocket* p = (cClientSocket*)p_ClientSock;
	p->_BreakObjFunc(this);
}

//##################################################################
//	설명 :	Queue에 저장된 receive data를 읽는다. 
//##################################################################
void cClntObj::_ReadBuffer()
{
	if (qMessageQueue.size() > 0)
	{
		while (!qMessageQueue.empty())
		{
			strncpy(cBuffer, qMessageQueue.front(), 1024);
			qMessageQueue.pop();
			cout << "\n○ Message From Server : " << cBuffer << endl;
			ZeroMemory(cBuffer, strlen(cBuffer));
			Sleep(1);
		}
	}
	cout << "\n● Text your message : ";
}
