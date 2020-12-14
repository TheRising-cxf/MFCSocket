
// ChatRoomDlg.h: 头文件
//

#pragma once


// CChatRoomServerDlg 对话框
class CServerSocket;
class CClientSocket;
class CChatRoomServerDlg : public CDialogEx
{
// 构造
public:
	CChatRoomServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATROOM_DIALOG };
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
	CListBox m_MessageList;
	CListBox m_UserList;
	CServerSocket *m_pServerSocket;
	CClientSocket *m_pClientSocket;
	void UpdateUser(CClientSocket* pSocket);
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnBnClickedCancel();
};
