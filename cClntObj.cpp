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
	cout << "�� Delete Clnt Object" << endl;
}

//##################################################################
// ���� :	cClntObj ���� ( Receive, Send ������ ���� ) 
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
//	���� :	p_Clnt�κ��� ���� �����͸� cBuffer�� ���� 
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
//	���� :	�Է� ���� �����͸� p_Clnt�� ���� 
//##################################################################
void cClntObj::_SendDataTo(LPVOID lp)
{
	cClntObj* p = (cClntObj*)lp;
	while (p->b_ClntObjFlag)
	{
		cout << "\n�� Text your message : ";
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
//	���� :	Receive, Send���� exit ȣ�� ��, p_Clnt���� (cServerSocket)
//##################################################################
void cClntObj::_BreakConnection()
{
	cout << "�� _BreakConnection()" << endl;
	cClientSocket* p = (cClientSocket*)p_ClientSock;
	p->_BreakObjFunc(this);
}

//##################################################################
//	���� :	Queue�� ����� receive data�� �д´�. 
//##################################################################
void cClntObj::_ReadBuffer()
{
	if (qMessageQueue.size() > 0)
	{
		while (!qMessageQueue.empty())
		{
			strncpy(cBuffer, qMessageQueue.front(), 1024);
			qMessageQueue.pop();
			cout << "\n�� Message From Server : " << cBuffer << endl;
			ZeroMemory(cBuffer, strlen(cBuffer));
			Sleep(1);
		}
	}
	cout << "\n�� Text your message : ";
}
