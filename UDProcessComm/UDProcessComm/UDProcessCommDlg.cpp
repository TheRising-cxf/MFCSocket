
// UDProcessCommDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "UDProcessComm.h"
#include "UDProcessCommDlg.h"
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


// CUDProcessCommDlg 对话框



CUDProcessCommDlg::CUDProcessCommDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UDPROCESSCOMM_DIALOG, pParent)
	, LocalPort(_T(""))
	, DestPort(_T(""))
	, str(_T(""))
{
	str = _T("");
	m_sport = 0;
	m_dport = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	IsTrue = FALSE;
	Client = INVALID_SOCKET;
}

LRESULT CUDProcessCommDlg::OnReadClose(WPARAM wParam, LPARAM lParam)
{
	CString str;
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		if (recvfrom(ServerSocket, (char *)&m_Msg, sizeof(m_Msg), 0, (LPSOCKADDR)&m_sockServerAddr, (int *)&socklen) == SOCKET_ERROR){
			list.AddString("发送失败！对方主机或应用程序未启动");
			return 0;
        }
		str.Format("%s", m_Msg.msg);
		list.AddString(str);
		break;
	default:
		break;
	}
}

void CUDProcessCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, IPLocal);
	DDX_Text(pDX, IDC_EDIT1, LocalPort);
	DDX_Control(pDX, IDC_IPADDRESS2, IPDest);
	DDX_Text(pDX, IDC_EDIT2, DestPort);
	DDX_Text(pDX, IDC_EDIT3, str);
	DDX_Control(pDX, IDC_LIST1, list);
}

BEGIN_MESSAGE_MAP(CUDProcessCommDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Start, &CUDProcessCommDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_send, &CUDProcessCommDlg::OnBnClickedsend)
	ON_BN_CLICKED(IDC_Stop, &CUDProcessCommDlg::OnBnClickedStop)
	ON_MESSAGE(WM_CLIENT_READCLOSE,OnReadClose)
END_MESSAGE_MAP()


// CUDProcessCommDlg 消息处理程序

BOOL CUDProcessCommDlg::OnInitDialog()
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

void CUDProcessCommDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUDProcessCommDlg::OnPaint()
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
HCURSOR CUDProcessCommDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDProcessCommDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (IPLocal.IsBlank()) {
		AfxMessageBox("请指定主机地址！");
		return;
	}
	if (LocalPort.IsEmpty()) {
		AfxMessageBox("请指定端口！");
		return;
	}
	WSADATA wsaData;
	int isErrorCode;
	if (WSAStartup(MAKEWORD(2, 1), &wsaData)) {
		list.AddString("Windosck 无法初始化！");
		WSACleanup();
		return;
	}
	list.AddString("开始创建socket...");
	ServerSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (ServerSocket == INVALID_SOCKET) {
		list.AddString("创建socket失败！");
		return;
	}
	BYTE nFile[4];
	CString sIP;
	IPLocal.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);
	m_sockServerAddr.sin_family = AF_INET;
	m_sockServerAddr.sin_addr.S_un.S_addr = inet_addr(sIP);
	m_sockServerAddr.sin_port = htons(atoi(LocalPort));

	socklen = sizeof(m_sockServerAddr);
	if (bind(ServerSocket, (LPSOCKADDR)&m_sockServerAddr, sizeof(m_sockServerAddr)) == SOCKET_ERROR){
		list.AddString("绑定失败！");
		return;
    }
	isErrorCode = WSAAsyncSelect(ServerSocket, m_hWnd,WM_CLIENT_READCLOSE, FD_READ);
	if (isErrorCode == SOCKET_ERROR) {
		list.AddString("WSAAsyncSelect 设定失败");
		return;
	}
	list.AddString("本机进程启动成功");
	list.AddString("地址 " + sIP + " 端口" + LocalPort);
	this->SetWindowTextA("本机应用进程（" + sIP + ":" + LocalPort + ")-UDProcsComm");
}


void CUDProcessCommDlg::OnBnClickedsend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (IPDest.IsBlank()) {
		AfxMessageBox("请指定目标进程主机地址！");
		return;
	}
	if (DestPort.IsEmpty()) {
		AfxMessageBox("请指定目标进程端口！");
		return;
	}
	BYTE nFile[4];
	CString sIP;
	IPDest.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);
	m_sockAddrto.sin_family = AF_INET;
	m_sockAddrto.sin_addr.S_un.S_addr = inet_addr(sIP);
	m_sockAddrto.sin_port = htons(atoi(DestPort));
	if (str.IsEmpty()) {
		AfxMessageBox("发送的消息不能为空！");
		return;
	}
	strcpy(m_Msg.msg, (LPCTSTR)str);
	m_Msg.i = 0;
	if (sendto(ServerSocket, (char*)&m_Msg,sizeof(m_Msg), 0, (LPSOCKADDR)&m_sockAddrto, sizeof(m_sockAddrto)) == SOCKET_ERROR) {
		list.AddString("发送数据失败！");
	}
	str.Empty();
	UpdateData(FALSE);
}


void CUDProcessCommDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	list.AddString("正在关闭Socket...");
	closesocket(ServerSocket);
	WSACleanup();
	list.AddString("本机进程停止运行！");

}
