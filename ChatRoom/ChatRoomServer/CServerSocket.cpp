#include "stdafx.h"
#include "CServerSocket.h"
#include "CClientSocket.h"

CServerSocket::CServerSocket()
{
}


CServerSocket::~CServerSocket()
{
}


void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CClientSocket *clientSocket = new CClientSocket(&connectList);
	Accept(*clientSocket);
	clientSocket->m_dlgServer = (CChatRoomServerDlg*)::AfxGetMainWnd();
	connectList.AddTail(clientSocket);
	CSocket::OnAccept(nErrorCode);
}

