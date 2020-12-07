// CClientSocket.cpp: 实现文件
//

#include "pch.h"
#include "MFCSockteClient.h"
#include "MySocket.h"
#include "MFCSockteClientDlg.h"

// CClientSocket

MySocket::MySocket()
{

}

MySocket::~MySocket()
{
}


void MySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_dlg->OnReceive();
	AsyncSelect(FD_CLOSE | FD_WRITE);
	CSocket::OnReceive(nErrorCode);
}

void MySocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_dlg->OnClose();
	CSocket::OnClose(nErrorCode);
}

void MySocket::GetDlg(CMFCSockteClientDlg * dlg)
{
	m_dlg = dlg;
}
