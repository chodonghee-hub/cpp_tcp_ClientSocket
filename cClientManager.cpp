#include "cClientManager.h"

cClientManager::cClientManager()
{
	p_cs = new cClientSocket(this);
}

cClientManager::~cClientManager()
{
}

//##################################################################
// 설명 :	cClientSocket 삭제 
//##################################################################
void cClientManager::_DeleteSocket()
{
	cout << "▶ _DeleteSocket() " << endl;
	delete p_cs;
	p_cs = nullptr;
}
