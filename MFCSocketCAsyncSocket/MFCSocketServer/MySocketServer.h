#pragma once

// CClientSocket 命令目标

class CMFCSocketServerDlg;
class MySocketServer : public CAsyncSocket
{
public:
	MySocketServer();
	CMFCSocketServerDlg* m_dlg;
	virtual ~MySocketServer();
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void GetDlg(CMFCSocketServerDlg* dlg);
};


