
// P2PClientDlg.h: 头文件
//

#pragma once
#include "rgstdUsr.h"
#include "MyMsg.h"
#include "stdafx.h"
// CP2PClientDlg 对话框
class CP2PClientDlg : public CDialogEx
{
// 构造
public:
	CP2PClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_P2PCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	SOCKET m_SelfSendSocket;//本机进程用于发送消息
	SOCKET m_SelfReceiveSocket;//本机用于接收信息
	SOCKADDR_IN m_sockSelfSendAddr;
	SOCKADDR_IN m_sockSelfReceiveAddr;
	SOCKADDR_IN m_socketAddrTo;
	RgstUsr m_registerUser[100];
	int m_registerUserCont;
	int m_onlineUserCont;
	Msg m_msgBuf;
	CString m_registerAddr;
	CString m_onlineUserInfo;
	CString m_p2pSrvRequest;
	CString m_p2pSrvResponse;
	CString MsgTalktoPeer;
	int socklen;
	void EnrolUsr(CString strMsgData);
	void UpdateUser(CString strMsgData);
	void PtoPsrvProvdr(CString strMsgData);
	void SendMsgtoPer(CString strMsgData);
	void ShowMsgfrmPer(CString strMsgData);
	void StartSendSocket(CString msgToSend, CString sIP, CString sPort);
	LRESULT OnReadMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStart();
	CIPAddressCtrl LocalP;
	CEdit localPort;
	CString sPort;
	CListBox m_MessageList;
	CString m_strUserName;
	CIPAddressCtrl ServerIP;
	CString ServerPort;
	afx_msg void OnEnter();
	afx_msg void OnLogout();
	CListBox m_userList;
	afx_msg void OnStop();
	afx_msg void OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult);
	CString strMessage;
	afx_msg void OnSend();
};
