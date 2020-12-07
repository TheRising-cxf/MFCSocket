#pragma once
#include "Afxsock.h" //使用CSocket 类
// CClientSocket 命令目标

class CMFCSockteClientDlg;
class MySocket : public CSocket
{
public:
	MySocket();
	CMFCSockteClientDlg* m_dlg;
	virtual ~MySocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void GetDlg(CMFCSockteClientDlg* dlg);
};


