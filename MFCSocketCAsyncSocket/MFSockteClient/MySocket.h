#pragma once

// CClientSocket 命令目标

class CMFCSockteClientDlg;
class MySocket : public CAsyncSocket
{
public:
	MySocket();
	CMFCSockteClientDlg* m_dlg;
	virtual ~MySocket();
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void GetDlg(CMFCSockteClientDlg* dlg);
};


