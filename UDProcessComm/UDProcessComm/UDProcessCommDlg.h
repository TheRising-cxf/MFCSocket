
// UDProcessCommDlg.h: 头文件
//

#pragma once
#pragma warning(disable:4996)
#define WM_CLIENT_READCLOSE WM_USER + 102
typedef struct MyMsg {
	char msg[100];
	int i;
}Msg;
// CUDProcessCommDlg 对话框
class CUDProcessCommDlg : public CDialogEx
{
// 构造
public:
	CUDProcessCommDlg(CWnd* pParent = nullptr);	// 标准构造函数
	LRESULT OnReadClose(WPARAM wParam, LPARAM lParam);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UDPROCESSCOMM_DIALOG };
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
	CIPAddressCtrl IPLocal;
	CString LocalPort;
	SOCKET Client;
	SOCKET ServerSocket;
	SOCKADDR_IN m_sockServerAddr;
	SOCKADDR_IN m_sockAddrto;
	int socklen;
	BOOL IsTrue;
	UINT m_sport;
	UINT m_dport;
	CIPAddressCtrl IPDest;
	CString DestPort;
	CString str;
	Msg m_Msg;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedsend();
	CListBox list;
	afx_msg void OnBnClickedStop();
};
