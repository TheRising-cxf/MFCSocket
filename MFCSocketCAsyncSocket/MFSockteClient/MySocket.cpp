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


// CClientSocket 成员函数
void MySocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode) {
		AfxMessageBox("连接失败，请重试");
		return;
	}
	m_dlg->OnConnect();
	CAsyncSocket::OnConnect(nErrorCode);
	
}

void MySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_dlg->OnReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_dlg->OnClose();
	CAsyncSocket::OnClose(nErrorCode);	
}

void MySocket::GetDlg(CMFCSockteClientDlg * dlg)
{
	m_dlg = dlg;
}
