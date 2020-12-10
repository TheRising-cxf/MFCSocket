
// MFSockteClientDlg.cpp: 实现文件
// 
#pragma warning(disable:4996)
#include "framework.h"
#include "MFCSockteClient.h"
#include "MFCSockteClientDlg.h"
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


// CMFSockteClientDlg 对话框



CMFCSockteClientDlg::CMFCSockteClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFSOCKTECLIENT_DIALOG, pParent)
	, sPort(_T(""))
	, m_sWords(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSockteClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, ServerIP);
	DDX_Text(pDX, IDC_EDIT5, sPort);
	DDX_Text(pDX, IDC_EDIT3, m_sWords);
	DDX_Control(pDX, IDC_LIST1, m_ListWords);
}

BEGIN_MESSAGE_MAP(CMFCSockteClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Connect, &CMFCSockteClientDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_Disconnect, &CMFCSockteClientDlg::OnBnClickedDisconnect)
	ON_BN_CLICKED(IDC_SendMessage, &CMFCSockteClientDlg::OnBnClickedSendmessage)
	ON_BN_CLICKED(IDCANCEL, &CMFCSockteClientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMFSockteClientDlg 消息处理程序

BOOL CMFCSockteClientDlg::OnInitDialog()
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

void CMFCSockteClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCSockteClientDlg::OnPaint()
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
HCURSOR CMFCSockteClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCSockteClientDlg::ReceiveData()
{
	char buffer[1024];
	int num = recv(m_client, buffer, 1024, 0);
	buffer[num] = '\0';
	CString sTemp;
	sTemp.Format("收到：%s", buffer);
	WSAAsyncSelect(m_client, m_hWnd, 10000, FD_READ | FD_CLOSE);
	m_ListWords.AddString(sTemp);
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}

void CMFCSockteClientDlg::CloseSock()
{
	m_ListWords.AddString("从服务器断开");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
	closesocket(m_client);
}


void CMFCSockteClientDlg::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	WSADATA wsd;
	WSAStartup(MAKEWORD(2, 2), &wsd);
	m_client = socket(AF_INET, SOCK_STREAM, 0);

	//服务器地址
	sockaddr_in serveraddr;
	UpdateData();
	if (ServerIP.IsBlank()) {
		AfxMessageBox("请指定服务器IP!");
		return;
	}
	if (sPort.IsEmpty()) {
		AfxMessageBox("请指定端口!");
		return;
	}
	BYTE nFild[4];
	CString sIP;
	UpdateData();
	ServerIP.GetAddress(nFild[0], nFild[1], nFild[2], nFild[3]);
	sIP.Format("%d.%d.%d.%d", nFild[0], nFild[1], nFild[2], nFild[3]);
	
	//设置服务器地址结构
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = inet_addr(sIP);
	serveraddr.sin_port = htons(atoi(sPort));

	//发起连接须指名要访问的服务器进程地址
	if (connect(m_client, (sockaddr*)&serveraddr, sizeof(serveraddr)) != 0) {
		MessageBox("连接失败");
		return;
	}
	else {
		m_ListWords.AddString("连接服务器成功");
		m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);

	}
	WSAAsyncSelect(m_client, m_hWnd, 10000, FD_READ | FD_CLOSE);
}


void CMFCSockteClientDlg::OnBnClickedDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	closesocket(m_client);
	m_ListWords.AddString("从服务器断开");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);

}


void CMFCSockteClientDlg::OnBnClickedSendmessage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_sWords.IsEmpty()) {
		AfxMessageBox("发送消息不能为空！");
		return;
	}
	int i = send(m_client, m_sWords.GetBuffer(0), m_sWords.GetLength(), 0);
	m_ListWords.AddString("发送："+m_sWords);
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);

}


void CMFCSockteClientDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CMFCSockteClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == 10000) {
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
