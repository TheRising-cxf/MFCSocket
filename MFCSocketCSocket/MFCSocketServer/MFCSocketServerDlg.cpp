
// MFCSocketServerDlg.cpp: 实现文件
//

#include "pch.h"
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
	, sPort(0)
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
	m_ServerSocket = NULL;
	m_ListenSocket = NULL;
	m_archiveIn = NULL;
	m_archiveOut = NULL;
	m_socketFile = NULL;

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
	if (!AfxSocketInit()) {
		MessageBox("WindowSocket init failed!", "Receive", MB_ICONSTOP);
		return;
	}
	m_ListenSocket = new MySocketServer;
	m_ListenSocket->GetDlg(this);
	BYTE nFild[4];
	CString sIP,sP;
	UpdateData();
	ServerIP.GetAddress(nFild[0], nFild[1], nFild[2], nFild[3]);
	sIP.Format("%d.%d.%d.%d", nFild[0], nFild[1], nFild[2], nFild[3]);
	sP.Format("%d", sPort);
	m_ListenSocket->Create(sPort,1,sIP);//创建服务器监听Socket
	m_ListenSocket->Listen(1);
	m_ListWords.AddString("开始监听");
	m_ListWords.AddString("地址：" + sIP + " 端口：" + sP);
	m_ListWords.AddString("等待客户端连接...");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}


void CMFCSocketServerDlg::OnBnClickedCloselisten()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_ListenSocket != NULL) {
		delete m_ListenSocket;
		m_ListenSocket = NULL;
	}
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
	*m_archiveOut << m_sWords;
	m_archiveOut->Flush();
	m_ListWords.AddString("发送：" + m_sWords);
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}


void CMFCSocketServerDlg::OnBnClickedDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	SocketReset();
	m_ListWords.AddString("与客户端断开");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}

void CMFCSocketServerDlg::OnAccept()
{
	m_ServerSocket = new MySocketServer;
	m_ServerSocket->GetDlg(this);
	m_ListenSocket->Accept(*m_ServerSocket);
	m_ServerSocket->AsyncSelect(FD_READ | FD_CLOSE);

	m_socketFile = new CSocketFile(m_ServerSocket);
	m_archiveIn = new CArchive(m_socketFile, CArchive::load);
	m_archiveOut = new CArchive(m_socketFile, CArchive::store);

	m_ListWords.AddString("接收了一个客户端的请求");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}

void CMFCSocketServerDlg::OnReceive()
{
	*m_archiveIn >> m_Input;
	m_archiveIn->Flush();
	m_ListWords.AddString("收到" + m_Input);
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}

void CMFCSocketServerDlg::OnClose()
{
	m_ListWords.AddString("与客户端断开连接");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
	SocketReset();
}

void CMFCSocketServerDlg::SocketReset()
{
	if (m_ServerSocket != NULL) {
		delete m_ServerSocket;
		m_ServerSocket = NULL;
	}
	if (m_ListenSocket != NULL) {
		delete m_ListenSocket;
		m_ListenSocket = NULL;
	}
	if (m_archiveIn != NULL) {
		delete m_archiveIn;
		m_archiveIn = NULL;
	}
	if (m_archiveOut != NULL) {
		delete m_archiveOut;
		m_archiveOut = NULL;
	}
	if (m_socketFile != NULL) {
		delete m_socketFile;
		m_socketFile = NULL;
	}
}
