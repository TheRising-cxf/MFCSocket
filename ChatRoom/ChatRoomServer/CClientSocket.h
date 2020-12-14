#pragma once
#include <afxsock.h>
#include "ChatRoomServerDlg.h"
class CClientSocket :
	public CSocket
{
public:
	CClientSocket();
	CClientSocket(CPtrList * list);//生成一个socket队列
	~CClientSocket();
public:
	CString m_strName;//客户端昵称
	CPtrList * m_cList;//Socket队列数据结构
	CChatRoomServerDlg* m_dlgServer;//对话框指针
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

