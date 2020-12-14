
// ChatRoomDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatRoomServer.h"
#include "ChatRoomServerDlg.h"
#include "afxdialogex.h"
#include "ChatRoomServer.h"
#include "CClientSocket.h"
#include "CServerSocket.h"
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


// CChatRoomServerDlg 对话框



CChatRoomServerDlg::CChatRoomServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATROOM_DIALOG, pParent)
	, m_strName(_T(""))
	, sPort(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatRoomServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Control(pDX, IDC_IPADDRESS1, ServerIP);
	DDX_Text(pDX, IDC_EDIT1, sPort);
	DDX_Control(pDX, IDC_LIST2, m_UserList);
	DDX_Control(pDX, IDC_LIST1, m_MessageList);
}

BEGIN_MESSAGE_MAP(CChatRoomServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChatRoomServerDlg::OnStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatRoomServerDlg::OnStop)
	ON_BN_CLICKED(IDCANCEL, &CChatRoomServerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CChatRoomServerDlg 消息处理程序

BOOL CChatRoomServerDlg::OnInitDialog()
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
	m_pServerSocket = NULL;
	m_pClientSocket = NULL;
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatRoomServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatRoomServerDlg::OnPaint()
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
HCURSOR CChatRoomServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatRoomServerDlg::UpdateUser(CClientSocket * pSocket)
{
	m_UserList.ResetContent();
	m_UserList.AddString(theApp.m_strName);
	CString user_info;
	user_info = theApp.m_strName + "(管理员)" + "&";
	if (pSocket != NULL) {
		CClientSocket *pSock = NULL;
		POSITION pos = pSocket->m_cList->GetHeadPosition();
		while (pos != NULL) {
			pSock = (CClientSocket*)pSocket->m_cList->GetNext(pos);
			m_UserList.AddString(pSock->m_strName);
			user_info += (pSock->m_strName + "&");
		}
		Header head;
		head.type = LOGIN_IO;
		head.len = user_info.GetLength();
		POSITION po = pSocket->m_cList->GetHeadPosition();
		while (po != NULL) {
			pSock = (CClientSocket*)pSocket->m_cList->GetNext(po);
			pSock->Send((char*)&head, sizeof(Header));
			pSock->Send((LPCTSTR)user_info, user_info.GetLength());
		}

	}
}

void CChatRoomServerDlg::OnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pServerSocket != NULL) {
		MessageBox("你已配置并连接过服务器端IP!");
		return;
	}
	UpdateData();
	m_pServerSocket = new CServerSocket();
	BYTE nFile[4];
	CString sIP;
	ServerIP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);
	CTime time = CTime::GetCurrentTime();
	if (m_strName.IsEmpty()) {
		AfxMessageBox("请先登记管理员名字！");
		return;
	}
	if (sIP.IsEmpty()) {
		AfxMessageBox("请先配置聊天室IP！");
		return;
	}
	if (sPort.IsEmpty()) {
		AfxMessageBox("请先配置要开放的端口！");
		return;
	}
	if (m_pServerSocket->Create(atoi(sPort), 1, sIP)){
		m_MessageList.AddString("聊天室开启成功");
		m_MessageList.SetTopIndex(m_MessageList.GetCount() - 1);
		CString t = time.Format("%Y-%m-%d");
		m_MessageList.AddString("日期：" + t);
		t = time.Format("%H:%M:%S");
		theApp.m_strName = m_strName;
		m_MessageList.AddString("管理员" + m_strName + "开放聊天室");
		if (m_pServerSocket->Listen()) {
			m_MessageList.AddString("等待成员加入...");
		}
		m_UserList.ResetContent();
		m_UserList.AddString(theApp.m_strName + "(管理员)");
		this->SetWindowTextA("管理员:" + m_strName + "-SelfRoom");
	}
}


void CChatRoomServerDlg::OnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	CClientSocket* pSock = NULL;
	POSITION po = m_pServerSocket->connectList.GetHeadPosition();
	while (po != NULL) {
		pSock = (CClientSocket*)m_pServerSocket->connectList.GetNext(po);
		pSock->Close();
	}
	m_pServerSocket->connectList.RemoveAll();
	m_pServerSocket->Close();
	m_pServerSocket = NULL;
	CTime time = CTime::GetCurrentTime();
	CString t = time.Format("%H:%M:%S");
	m_UserList.ResetContent();
	m_MessageList.ResetContent();
	this->SetWindowTextA("聊天室管理-selfRoom");
}


void CChatRoomServerDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
