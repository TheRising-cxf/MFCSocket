// MFSockteClientDlg.h: 头文件
//

#pragma once
#include "MySocket.h"
// CMFSockteClientDlg 对话框
class CMFCSockteClientDlg : public CDialogEx
{
// 构造
public:
	CMFCSockteClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFSOCKTECLIENT_DIALOG };
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
	MySocket* m_ClientSocket;
	CIPAddressCtrl ServerIP;
	void  OnReceive();
	void  OnClose();
	void  OnConnect();
	void  SocketReset();
	int sPort;
	CString m_sWords;
	CListBox m_ListWords;
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnBnClickedSendmessage();
	afx_msg void OnBnClickedCancel();
};
