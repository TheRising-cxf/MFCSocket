#include "stdafx.h"
#include "CClientSocket.h"
#include "ChatRoomClientDlg.h"
#include "tagHeader.h"

CClientSocket::CClientSocket()
{
}


CClientSocket::~CClientSocket()
{
}

void CClientSocket::GetDlg(CChatRoomClientDlg * dlg)
{
	m_chatDlg = dlg;
}


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	char buff[sizeof(Header)];
	memset(buff, 0, sizeof(buff));
	Receive(buff, sizeof(buff));
	this->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
	Header *header = (Header*)buff;
	int length = header->len;
	char type = header->type;
	if (type == LOGIN_IO) {
		m_chatDlg->UpdateUser();
	}
	else if (type == SEND_MESSAGE) {
		m_chatDlg->GetMsgFromRoom();
	}
	CSocket::OnReceive(nErrorCode);
}


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	AfxMessageBox("与聊天室断开");
	m_chatDlg->m_friendList.ResetContent();
	m_chatDlg->m_pSocket->Close();
	delete m_chatDlg->m_pSocket;
	m_chatDlg->m_pSocket = NULL;
	CSocket::OnClose(nErrorCode);
}
