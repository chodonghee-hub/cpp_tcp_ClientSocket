#pragma once
#include "cClientSocket.h"
class cClientManager
{
private :
	cClientSocket* p_cs;

public :
	cClientManager();
	~cClientManager();
	void _DeleteSocket();
};

