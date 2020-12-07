
// MFSockteClientDlg.cpp: 实现文件
//

#include "pch.h"
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
	, sPort(0)
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
	m_ClientSocket = NULL;

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



void CMFCSockteClientDlg::OnReceive()
{
	char szTemp[200];
	int n = m_ClientSocket->Receive(szTemp, 200);
	szTemp[n] = '\0';
	CString sTemp;
	sTemp.Format("收到：%s", szTemp);
	m_ListWords.AddString(sTemp);
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
}

void CMFCSockteClientDlg::OnClose()
{
	m_ListWords.AddString("从服务器断开");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);
	m_ClientSocket->Close();
	SocketReset();
}

void CMFCSockteClientDlg::OnConnect()
{
	m_ListWords.AddString("连接服务器成功");
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);

}

void CMFCSockteClientDlg::SocketReset()
{
	if (m_ClientSocket != NULL) {
		delete m_ClientSocket;
		m_ClientSocket = NULL;
	}
}

void CMFCSockteClientDlg::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!AfxSocketInit()) {
		MessageBox("WindowSocket init failed!", "Receive", MB_ICONSTOP);
		return;
	}
	m_ClientSocket = new MySocket;
	m_ClientSocket->GetDlg(this);
	//连接服务器
	BYTE nFile[4];
	CString sIP;
	UpdateData();
	ServerIP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);
	m_ClientSocket->Create();
	m_ClientSocket->Connect(sIP, sPort);
}


void CMFCSockteClientDlg::OnBnClickedDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ClientSocket->Close();
	SocketReset();
	m_ListWords.AddString("从服务器断开");
}


void CMFCSockteClientDlg::OnBnClickedSendmessage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_ClientSocket->Send(m_sWords, m_sWords.GetLength());
	m_ListWords.AddString("发送："+ m_sWords);
	m_ListWords.SetTopIndex(m_ListWords.GetCount() - 1);

}


void CMFCSockteClientDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	SocketReset();
	CDialogEx::OnCancel();
}
