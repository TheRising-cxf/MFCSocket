
// MFCSocketServerDlg.cpp: 实现文件
//
#pragma warning(disable:4996)
#include "framework.h"
#include "MFCSocketServer.h"
#include "MFCSocketServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCSocketServerDlg 对话框



CMFCSocketServerDlg::CMFCSocketServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSOCKETSERVER_DIALOG, pParent)
	, sPort(_T(""))
	, m_sWords(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, ServerIP);
	DDX_Text(pDX, IDC_EDIT1, sPort);
	DDX_Control(pDX, IDC_LIST1, m_ListWords);
	DDX_Text(pDX, IDC_EDIT2, m_sWords);
}

BEGIN_MESSAGE_MAP(CMFCSocketServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Listen, &CMFCSocketServerDlg::OnBnClickedListen)
	ON_BN_CLICKED(IDC_CloseListen, &CMFCSocketServerDlg::OnBnClickedCloselisten)
	ON_BN_CLICKED(IDC_Exit, &CMFCSocketServerDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_SendMessage, &CMFCSocketServerDlg::OnBnClickedSendmessage)
	ON_BN_CLICKED(IDC_DisConnect, &CMFCSocketServerDlg::OnBnClickedDisconnect)
END_MESSAGE_MAP()


// CMFCSocketServerDlg 消息处理程序

BOOL CMFCSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCSocketServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCSocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCSocketServerDlg::OnBnClickedListen()
{
	// TODO: 在此添加控件通知处理程序代码
	WSADATA wsd;
	WSAStartup(MAKEWORD(2, 2), &wsd);
	m_server = socket(AF_INET, SOCK_STREAM, 0);

	WSAAsyncSelect(m_server, m_hWnd, 20000, FD_ACCEPT);
	
	m_client = 0;
	BYTE nFild[4];
	CString sIP;
	UpdateData();
	if (ServerIP.IsBlank()) {
		AfxMessageBox("请设置IP地址!");
		return;
	}
	if (sPort.IsEmpty()) {
		AfxMessageBox("请设置监听端口!");
		return;
	}

	ServerIP.GetAddress(nFild[0], nFild[1], nFild[2], nFild[3]);
	sIP.Format("%d.%d.%d.%d", nFild[0], nFild[1], nFild[2], nFild[3]);
	
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = inet_addr(sIP);
	serveraddr.sin_port = htons(atoi(sPort));

	if (bind(m_server, (sockaddr*)&serveraddr, sizeof(serveraddr))) {
		MessageBox("绑定地址失败。");
		return;
	}

	listen(m_server, 5);
	m_ListWords.AddString("开始监听");
	m_ListWords.AddString("地址：" + sIP + " 端口：" + sPort);
	m_ListWords.AddString("等待客户端连接...");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}


void CMFCSocketServerDlg::OnBnClickedCloselisten()
{
	// TODO: 在此添加控件通知处理程序代码
	closesocket(m_server);
	m_ListWords.AddString("停止监听");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);

}


void CMFCSocketServerDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CMFCSocketServerDlg::OnBnClickedSendmessage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_sWords.IsEmpty()) {
		AfxMessageBox("发送消息不能为空！");
		return;
	}
	int i = send(m_client, m_sWords.GetBuffer(0), m_sWords.GetLength(), 0);
	m_ListWords.AddString("发送：" + m_sWords);
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);

}


void CMFCSocketServerDlg::OnBnClickedDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	closesocket(m_client);
	m_ListWords.AddString("与客户端断开");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}
void CMFCSocketServerDlg::ReceiveData()
{
	char buffer[1024];
	int num = recv(m_client, buffer, 1024, 0);
	buffer[num] = '\0';
	CString sTemp;
	sTemp.Format("收到：%s", buffer);
	WSAAsyncSelect(m_client, m_hWnd, 30000, FD_READ | FD_CLOSE);
	m_ListWords.AddString(sTemp);
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}

void CMFCSocketServerDlg::HandleData()
{
	sockaddr_in serveraddr;
	int len = sizeof(serveraddr);
	m_client = accept(m_server,(struct sockaddr*)&serveraddr, &len);
	WSAAsyncSelect(m_client, m_hWnd, 30000, FD_READ | FD_CLOSE);
	m_ListWords.AddString("接收到一个客户端连接请求");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}

void CMFCSocketServerDlg::CloseSock()
{
	m_ListWords.AddString("与客户端断开连接");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
	closesocket(m_client);
	WSAAsyncSelect(m_client, m_hWnd, 20000, FD_READ | FD_CLOSE);
}


BOOL CMFCSocketServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == 20000) {
		this->HandleData();
	}
	else if(pMsg->message == 30000){
		switch (pMsg->lParam)
		{
		case FD_READ:
			this->ReceiveData();
			break;
		case FD_CLOSE:
			this->CloseSock();
			break;
		default:
			break;
		}
	}
	else
		return CDialogEx::PreTranslateMessage(pMsg);
}
