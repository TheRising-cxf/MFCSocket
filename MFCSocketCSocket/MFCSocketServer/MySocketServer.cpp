// CServerSocket.cpp: 实现文件
//

#include "pch.h"
#include "MFCSocketServer.h"
#include "MySocketServer.h"
#include "MFCSocketServerDlg.h"

// CServerSocket

MySocketServer::MySocketServer()
{

}

MySocketServer::~MySocketServer()
{
}


// CClientSocket 成员函数
void MySocketServer::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_dlg->m_ServerSocket == NULL) {
		m_dlg->OnAccept();
		CSocket::OnAccept(nErrorCode);
	}

}

void MySocketServer::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_dlg->OnReceive();
	AsyncSelect(FD_CLOSE | FD_WRITE);
	CSocket::OnReceive(nErrorCode);
}

void MySocketServer::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_dlg->OnClose();
	CSocket::OnClose(nErrorCode);
}

void MySocketServer::GetDlg(CMFCSocketServerDlg * dlg)
{
	m_dlg = dlg;
}
