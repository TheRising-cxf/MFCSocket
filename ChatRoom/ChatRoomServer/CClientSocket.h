#pragma once
#include <afxsock.h>
#include "ChatRoomServerDlg.h"
class CClientSocket :
	public CSocket
{
public:
	CClientSocket();
	CClientSocket(CPtrList * list);//����һ��socket����
	~CClientSocket();
public:
	CString m_strName;//�ͻ����ǳ�
	CPtrList * m_cList;//Socket�������ݽṹ
	CChatRoomServerDlg* m_dlgServer;//�Ի���ָ��
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

