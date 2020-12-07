#pragma once
#include "Afxsock.h" //使用CSocket 类
// CClientSocket 命令目标

class CMFCSocketServerDlg;
class MySocketServer : public CSocket
{
public:
	MySocketServer();
	CMFCSocketServerDlg* m_dlg;
	virtual ~MySocketServer();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	void GetDlg(CMFCSocketServerDlg* dlg);
};


