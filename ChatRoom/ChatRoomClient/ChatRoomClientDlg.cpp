
// ChatRoomClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatRoomClient.h"
#include "ChatRoomClientDlg.h"
#include "afxdialogex.h"
#include "tagHeader.h"
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


// CChatRoomClientDlg 对话框



CChatRoomClientDlg::CChatRoomClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATROOMCLIENT_DIALOG, pParent)
	, m_strName(_T(""))
	, sPort(_T(""))
	, m_Message(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatRoomClientDlg::UpdateUser()
{
	char buff[1000];
	memset(buff, 0, sizeof(buff));
	m_pSocket->Receive(buff, sizeof(buff));
	m_pSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
	CString user_info = buff;
	CString array[100];
	int b = 0;
	for (int i = 0; i < user_info.GetLength(); i++) {
		if (i != (user_info.GetLength() - 1)) {
			if (user_info[i] == '&') {
				b++;
			}
			else {
				array[b] = array[b] + user_info[i];
			}
		}
	}
	m_friendList.ResetContent();
	for (int j = 0; j < b + 1; j++) {
		m_friendList.AddString(array[j]);
	}
}

BOOL CChatRoomClientDlg::GetMsgFromRoom()
{
	char buff[1000];
	memset(buff, 0, sizeof(buff));
	m_pSocket->Receive(buff, sizeof(buff));
	m_pSocket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
	CString strtmp = buff;
	m_MessageList.AddString(strtmp);
	return 0;
}

void CChatRoomClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Control(pDX, IDC_IPADDRESS1, ServerIP);
	DDX_Text(pDX, IDC_EDIT3, sPort);
	DDX_Control(pDX, IDC_LIST1, m_friendList);
	DDX_Control(pDX, IDC_LIST2, m_MessageList);
	DDX_Text(pDX, IDC_EDIT1, m_Message);
}

BEGIN_MESSAGE_MAP(CChatRoomClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChatRoomClientDlg::OnEnter)
	ON_BN_CLICKED(IDC_BUTTON3, &CChatRoomClientDlg::OnSend)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatRoomClientDlg::OnQuit)
END_MESSAGE_MAP()


// CChatRoomClientDlg 消息处理程序

BOOL CChatRoomClientDlg::OnInitDialog()
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
	m_pSocket = NULL;
	m_Message = "";
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatRoomClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatRoomClientDlg::OnPaint()
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
HCURSOR CChatRoomClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatRoomClientDlg::OnEnter()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSocket != NULL) {
		AfxMessageBox("你当前已处于连接状态！");
		return;
	}
	UpdateData();
	m_pSocket = new CClientSocket();
	m_pSocket->GetDlg(this);
	BYTE nFile[4];
	CString sIP;
	ServerIP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);

	if (m_strName.IsEmpty()) {
		AfxMessageBox("给自己取个昵称吧！");
		return;
	}
	if (sIP.IsEmpty()) {
		AfxMessageBox("请指定IP地址！");
		return;
	}
	if (sPort.IsEmpty()) {
		AfxMessageBox("请指定连接端口！");
		return;
	}
	if (!m_pSocket->Create()) {
		AfxMessageBox("网络创建错误！！！");
		m_pSocket->Close();
		return;
	}
	if (!m_pSocket->Connect(sIP, atoi(sPort))) {
		AfxMessageBox("连接服务器失败！！！");
		m_pSocket->Close();
		delete m_pSocket;
		m_pSocket = NULL;
		return;
	}
	Header head;
	head.type = LOGIN_IO;
	head.len = m_strName.GetLength();
	m_pSocket->Send((char*)&head, sizeof(Header));
	m_pSocket->Send(m_strName, m_strName.GetLength());
	theApp.m_strName = m_strName;
	this->SetWindowTextA(m_strName + "-SelfChat");
}


void CChatRoomClientDlg::OnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSocket == NULL) {
		AfxMessageBox("请先连接聊天室IP");
		return;
	}
	UpdateData();
	if (m_Message == "") {
		AfxMessageBox("不能发送空消息！");
		return;
	}
	Header head;
	head.type = SEND_MESSAGE;
	head.len = m_Message.GetLength();
	m_pSocket->Send((char*)&head, sizeof(Header));
	if (m_pSocket->Send(m_Message, m_Message.GetLength())) {
		m_Message = "";
		UpdateData(FALSE);
		return;
	}
	else {
		AfxMessageBox("网络传输错误！");
	}
}


void CChatRoomClientDlg::OnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSocket) {
		m_pSocket->Close();
		delete m_pSocket;
		m_pSocket = NULL;
	}
	m_friendList.ResetContent();
	m_MessageList.AddString("你已退出聊天室！");
	this->SetWindowTextA("聊天室客户端-SelfChat");
}
