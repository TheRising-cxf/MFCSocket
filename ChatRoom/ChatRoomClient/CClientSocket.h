#pragma once
#include <afxsock.h>
class CChatRoomClientDlg;
class CClientSocket :
	public CSocket
{
public:
	CClientSocket();
	~CClientSocket();
public:
	CChatRoomClientDlg *m_chatDlg;
	CString m_strName;
	void GetDlg(CChatRoomClientDlg *dlg);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

