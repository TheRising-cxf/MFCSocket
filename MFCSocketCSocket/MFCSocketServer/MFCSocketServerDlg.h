
// MFCSocketServerDlg.h: 头文件
//

#pragma once
#include "MySocketServer.h"
// CMFCSocketServerDlg 对话框
class CMFCSocketServerDlg : public CDialogEx
{
// 构造
public:
	CMFCSocketServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

	MySocketServer* m_ServerSocket;
	MySocketServer* m_ListenSocket;
	CArchive* m_archiveIn;//收到的信息的存储文件
	CArchive* m_archiveOut;//发送的信息的存储文件
	CSocketFile* m_socketFile;//用于发送和接收数据的缓冲区
	CString m_Input;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSOCKETSERVER_DIALOG };
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
	CIPAddressCtrl ServerIP;
	int sPort;
	CListBox m_ListWords;
	afx_msg void OnBnClickedListen();
	afx_msg void OnBnClickedCloselisten();
	afx_msg void OnBnClickedExit();
	CString m_sWords;
	afx_msg void OnBnClickedSendmessage();
	afx_msg void OnBnClickedDisconnect();
	void OnAccept();
	void OnReceive();
	void OnClose();
	void SocketReset();

};
