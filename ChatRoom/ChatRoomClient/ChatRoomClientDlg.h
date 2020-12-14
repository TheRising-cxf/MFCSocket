
// ChatRoomClientDlg.h: 头文件
//

#pragma once
#include "CClientSocket.h"

// CChatRoomClientDlg 对话框
class CChatRoomClientDlg : public CDialogEx
{
// 构造
public:
	CChatRoomClientDlg(CWnd* pParent = nullptr);	// 标准构造函数
	CClientSocket *m_pSocket;
	void UpdateUser();
	BOOL GetMsgFromRoom();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATROOMCLIENT_DIALOG };
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
	CString m_strName;
	CIPAddressCtrl ServerIP;
	CString sPort;
	CListBox m_friendList;
	CListBox m_MessageList;
	CString m_Message;
	afx_msg void OnEnter();
	afx_msg void OnSend();
	afx_msg void OnQuit();
};
