#include "stdafx.h"
#include "CClientSocket.h"
#include "tagHeader.h"
#include "afxdialogex.h"
CClientSocket::CClientSocket()
{
}

CClientSocket::CClientSocket(CPtrList * list):m_dlgServer(NULL)
{
	m_cList= list;
}


CClientSocket::~CClientSocket()
{

}


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	char buff1[sizeof(Header)];
	memset(buff1, 0, sizeof(buff1));
	Receive(buff1, sizeof(buff1));
	this->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
	Header *header = (Header*)buff1;
	int length = header->len;
	char type = header->type;
	if (type == LOGIN_IO) {
		char buff[1000];
		memset(buff, 0, sizeof(buff));
		Receive(buff, sizeof(buff));
		this->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
		m_dlgServer->UpdateData();
		CTime time = CTime::GetCurrentTime();
		CString t = time.Format("%H:%M:%S");
		CString strTmp = t + " " + CString(buff) + "进入聊天室";
		m_dlgServer->m_MessageList.AddString(strTmp);
		m_strName = buff;

		Header head;
		head.type = SEND_MESSAGE;
		head.len = strTmp.GetLength();
	
		CClientSocket * curr = NULL;
		POSITION pos = m_cList->GetHeadPosition();
		while (pos != NULL) {
			curr = (CClientSocket*)m_cList->GetNext(pos);
			if (curr->m_strName == m_strName) {
				CString str = m_strName + ",欢迎你的加入。";
				curr->Send((char*)&head, sizeof(Header));
				curr->Send(str, str.GetLength());
			}
			else {
				curr->Send((char*)&head, sizeof(Header));
				curr->Send(strTmp, strTmp.GetLength());
			}
		}
			m_dlgServer->UpdateUser(this);

	}
	if (type == SEND_MESSAGE) {
		char buff[1000];
		memset(buff, 0, sizeof(buff));
		Receive(buff, sizeof(buff));
		this->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
		CTime time = CTime::GetCurrentTime();
		CString t = time.Format("%H:%M:%S");
		CString nikeName = this->m_strName;
		CString strTmp = t + " " + nikeName + " 说：" + CString(buff);
		m_dlgServer->m_MessageList.AddString(strTmp);
		CClientSocket* curr = NULL;
		POSITION po = m_cList->GetHeadPosition();
		while (po != NULL) {
			curr = (CClientSocket*)m_cList->GetNext(po);
			curr->Send((char*)header, sizeof(Header));
			curr->Send(strTmp, strTmp.GetLength());
		}
	}
	CSocket::OnReceive(nErrorCode);
}


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	POSITION pos = m_cList->Find(this);
	if (pos != NULL) {
		m_cList->RemoveAt(pos);
		CTime time = CTime::GetCurrentTime();
		CString t = time.Format("%H:%M:%S");
		CString strTmp = t + " " + this->m_strName + "离开聊天室";
		m_dlgServer->m_MessageList.AddString(strTmp);
		Header head;
		head.type = SEND_MESSAGE;
		head.len = strTmp.GetLength();
		CClientSocket *curr = NULL;
		POSITION pos = m_cList->GetHeadPosition();
		while (pos != NULL) {
			curr = (CClientSocket*)m_cList->GetNext(pos);
			curr->Send((char*)&head, sizeof(Header));
			curr->Send(strTmp, strTmp.GetLength());

		}
		m_dlgServer->UpdateUser(this);
		this->Close();
		delete this;
	}
	CSocket::OnClose(nErrorCode);
}
