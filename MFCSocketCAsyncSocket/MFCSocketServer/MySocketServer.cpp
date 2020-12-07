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
	if (nErrorCode) {
		AfxMessageBox("连接失败，请重试");
		return;
	}
	m_dlg->OnAccept();
	CAsyncSocket::OnAccept(nErrorCode);

}

void MySocketServer::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_dlg->OnReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocketServer::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_dlg->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}

void MySocketServer::GetDlg(CMFCSocketServerDlg * dlg)
{
	m_dlg = dlg;
}
