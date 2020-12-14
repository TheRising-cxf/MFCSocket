#pragma once
#include <afxsock.h>
class CServerSocket :
	public CSocket
{
public:
	CServerSocket();
	~CServerSocket();
	CPtrList connectList;
	virtual void OnAccept(int nErrorCode);
};

