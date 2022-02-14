#include "cClientSocket.h"
#include "cClientManager.h"

cClientSocket::cClientSocket(LPVOID _lp)
{
	bConnectionFlag = false;
	p_cManager = _lp;
	__Init__();
}

cClientSocket::~cClientSocket()
{
	delete[] p_cManager;
	p_cManager = nullptr;
	closesocket(clnt);
	WSACleanup();
	cout << "cClientSocket deleted ... " << endl;
}

//##################################################################
//	���� :	Client Socket ���� 
//##################################################################
void cClientSocket::__Init__()
{
	if (!bCheckWinSock()) { return; }
	_SetSocketInfo();
	if (!bCheckValidSocket(&clnt)) { return; }
	if (!bCheckConnection(8080)) { return; }

	new cClntObj(this, &clnt);
	while (bConnectionFlag) { Sleep(1);}
}

//##################################################################
//	���� :	Ŭ���̾�Ʈ ���� ���� 
//##################################################################
void cClientSocket::_SetSocketInfo()
{
	clnt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

//##################################################################
//	���� :	WSAStartup ���� ���� Ȯ�� �� ����û
//##################################################################
bool cClientSocket::bCheckWinSock()
{
	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0)
	{
		cout << "�� bCheckWinSock() �� WSAStartup error : " << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}
	return true;
}

//##################################################################
// ���� :	INVALID_SOCKET Ȯ�� 
//##################################################################
bool cClientSocket::bCheckValidSocket(SOCKET* _sock)
{
	if (*_sock == INVALID_SOCKET)
	{
		cout << "�� _CheckValidSocket �� INVALID_SOCKET error : " << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}
	return true;
}

//##################################################################
// ���� :	������ ���� Ȯ�� 
//##################################################################
bool cClientSocket::bCheckConnection(int _nPortNum)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_nPortNum);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(clnt, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		cout << "�� bCheckConnection �� connection error : " << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}
	bConnectionFlag = true;
	return true;
}

//##################################################################
// ���� :	cClntObj���� ���� ��ȣ�� ���� �� ȣ��, cClntObj ���� 
//##################################################################
void cClientSocket::_BreakObjFunc(cClntObj* _pObj)
{
	cout << "�� _ReceiverError() " << endl;
	cClientManager* p = (cClientManager*)p_cManager;
	delete _pObj;
	_pObj = nullptr;
	bConnectionFlag = false;
	p->_DeleteSocket();
}
