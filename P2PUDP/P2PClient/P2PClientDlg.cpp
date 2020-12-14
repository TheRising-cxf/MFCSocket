
// P2PClientDlg.cpp: 实现文件
//
#pragma warning(disable:4996)
#include "stdafx.h"
#include "P2PClient.h"
#include "P2PClientDlg.h"
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


// CP2PClientDlg 对话框



CP2PClientDlg::CP2PClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_P2PCLIENT_DIALOG, pParent)
	, sPort(_T(""))
	, m_strUserName(_T(""))
	, ServerPort(_T(""))
	, strMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CP2PClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS2, LocalP);
	DDX_Control(pDX, IDC_EDIT4, localPort);
	DDX_Text(pDX, IDC_EDIT4, sPort);
	DDX_Control(pDX, IDC_LIST1, m_MessageList);
	DDX_Text(pDX, IDC_EDIT2, m_strUserName);
	DDX_Control(pDX, IDC_IPADDRESS1, ServerIP);
	DDX_Text(pDX, IDC_EDIT3, ServerPort);
	DDX_Control(pDX, IDC_LIST2, m_userList);
	DDX_Text(pDX, IDC_EDIT1, strMessage);
}

BEGIN_MESSAGE_MAP(CP2PClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_CLIENT_READMSG, OnReadMsg)
	ON_BN_CLICKED(IDC_BUTTON5, &CP2PClientDlg::OnStart)
	ON_BN_CLICKED(IDC_BUTTON3, &CP2PClientDlg::OnEnter)
	ON_BN_CLICKED(IDC_BUTTON2, &CP2PClientDlg::OnLogout)
	ON_BN_CLICKED(IDC_BUTTON4, &CP2PClientDlg::OnStop)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS2, &CP2PClientDlg::OnIpnFieldchangedIpaddress2)
	ON_BN_CLICKED(IDC_BUTTON1, &CP2PClientDlg::OnSend)
END_MESSAGE_MAP()


// CP2PClientDlg 消息处理程序

BOOL CP2PClientDlg::OnInitDialog()
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
	m_registerUserCont = 0;
	m_onlineUserCont = 0;
	strMessage = "";
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CP2PClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CP2PClientDlg::OnPaint()
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
HCURSOR CP2PClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CP2PClientDlg::EnrolUsr(CString strMsgData)
{
	CString userName, ipFiled1, ipFiled2, ipFiled3, ipFiled4,port;
	int n;
	n = strMsgData.Find(",");
	userName  = strMsgData.Left(n);
	strMsgData = strMsgData.Right(strMsgData.GetLength() - (n + 1));
	n = strMsgData.Find(".");
	ipFiled1 = strMsgData.Left(n);
	strMsgData = strMsgData.Right(strMsgData.GetLength() - (n + 1));
	n = strMsgData.Find(".");
	ipFiled2 = strMsgData.Left(n);
	strMsgData = strMsgData.Right(strMsgData.GetLength() - (n + 1));
	n = strMsgData.Find(".");
	ipFiled3 = strMsgData.Left(n);
	strMsgData = strMsgData.Right(strMsgData.GetLength() - (n + 1));
	n = strMsgData.Find(":");
	ipFiled4 = strMsgData.Left(n);
	port = strMsgData.Right(strMsgData.GetLength() - (n + 1));

	BOOL BePresnc = false;
	for (int i = 0; i < m_registerUserCont; i++) {
		if (CString(m_registerUser[i].userName) == userName) {
			BePresnc = true;
			if (m_registerUser[i].onLStat) {
				m_registerUser[i].onLStat = false;
				m_onlineUserCont--;
			}
			else{
				m_registerUser[i].ipFild[0] = _ttoi(ipFiled1);
				m_registerUser[i].ipFild[1] = _ttoi(ipFiled2);
				m_registerUser[i].ipFild[2] = _ttoi(ipFiled3);
				m_registerUser[i].ipFild[3] = _ttoi(ipFiled4);
				m_registerUser[i].port = atoi((LPCTSTR)port);
				m_registerUser[i].onLStat = true;
				m_onlineUserCont++;
			}
		}
	}
	if (!BePresnc) {
		strcpy(m_registerUser[m_registerUserCont].userName, (LPCTSTR)userName);
		m_registerUser[m_registerUserCont].ipFild[0] = _ttoi(ipFiled1);
		m_registerUser[m_registerUserCont].ipFild[1] = _ttoi(ipFiled2);
		m_registerUser[m_registerUserCont].ipFild[2] = _ttoi(ipFiled3);
		m_registerUser[m_registerUserCont].ipFild[3] = _ttoi(ipFiled4);
		m_registerUser[m_registerUserCont].port = atoi((LPCTSTR)port);
		m_registerUser[m_registerUserCont].onLStat = true;
		m_onlineUserCont++;
		m_registerUserCont++;
	}
	m_userList.ResetContent();
	m_MessageList.SetWindowTextA("");
	m_onlineUserInfo = "UPDT_USR;";
	for (int i = 0; i < m_registerUserCont; i++) {
		if (m_registerUser[i].onLStat) {
			m_onlineUserInfo += CString(m_registerUser[i].userName) + "&";
			m_userList.AddString(CString(m_registerUser[i].userName));
			CString sIP, sp, onlUsrStateView;
			sIP.Format("%d.%d.%d.%d", m_registerUser[i].ipFild[0], m_registerUser[i].ipFild[1], m_registerUser[i].ipFild[2], m_registerUser[i].ipFild[3]);
			sp.Format("%d", m_registerUser[i].port);
			onlUsrStateView = CString(m_registerUser[i].userName)+ " -IP";
			onlUsrStateView += sIP + " 端口：" + sp;
			m_MessageList.AddString(onlUsrStateView);
		}
	}
	for (int i = 0; i < m_registerUserCont; i++) {
		if (m_registerUser[i].onLStat) {
			CString sIP, sp;
			sIP.Format("%d.%d.%d.%d", m_registerUser[i].ipFild[0], m_registerUser[i].ipFild[1], m_registerUser[i].ipFild[2], m_registerUser[i].ipFild[3]);
			sp.Format("%d", m_registerUser[i].port);
			this->StartSendSocket(m_onlineUserInfo, sIP, sp);
		}
	}
}

void CP2PClientDlg::UpdateUser(CString strMsgData)
{
	CString array[100];
	int b = 0;
	for (int i = 0; i < strMsgData.GetLength(); i++) {
		if (i != (strMsgData.GetLength() - 1)) {
			if (strMsgData[i] == '&') {
				b++;
			}
			else {
				array[b] = array[b] + strMsgData[i];
			}
		}
	}
	m_userList.ResetContent();
	for (int j = 0; j < b + 1; j++) {
		m_userList.AddString(array[j]);
	}
}

void CP2PClientDlg::PtoPsrvProvdr(CString strMsgData)
{
	CString userName,ip, port,perUserAddr;
	int n = strMsgData.Find(",");
	userName = strMsgData.Left(n);
	strMsgData = strMsgData.Right(strMsgData.GetLength() - (n + 1));
	for (int i = 0; i < m_registerUserCont; i++) {
		if (CString(m_registerUser[i].userName) == userName) {
			CString perIP, perPort;
			perIP.Format("%d.%d.%d.%d", m_registerUser[i].ipFild[0], m_registerUser[i].ipFild[1], m_registerUser[i].ipFild[2], m_registerUser[i].ipFild[3]);
			perPort.Format("%d", m_registerUser[i].port);
			perUserAddr = userName + ",";
			perUserAddr += perIP + ":";
			perUserAddr += perPort;
			m_p2pSrvResponse = "RSP_ADDR;" + perUserAddr;
			break;
		}
	}
	n = strMsgData.Find(":");
	ip = strMsgData.Left(n);
	port = strMsgData.Right(strMsgData.GetLength() - (n + 1));
	this->StartSendSocket(m_p2pSrvResponse, ip, port);


}

void CP2PClientDlg::SendMsgtoPer(CString strMsgData)
{
	CString ip, port;
	int n = strMsgData.Find(",");
	strMsgData = strMsgData.Right(strMsgData.GetLength() - (n + 1));
	n = strMsgData.Find(":");
	ip = strMsgData.Left(n);
	port = strMsgData.Right(strMsgData.GetLength() - (n + 1));
	UpdateData();
	if (strMessage == "") {
		AfxMessageBox("不能发送空消息");
	}
	CTime time = CTime::GetCurrentTime();
	CString t = time.Format("%H:%M:%S");
	MsgTalktoPeer = "SND_MSG;" + m_strUserName + ",";
	MsgTalktoPeer += t + "\r\n    " + strMessage + "\r\n";
	this->StartSendSocket(MsgTalktoPeer, ip, port);
	m_MessageList.AddString(m_strUserName + " " + t + " " + strMessage);
	strMessage = "";
	UpdateData(FALSE);
}

void CP2PClientDlg::ShowMsgfrmPer(CString strMsgData)
{
	CString userName, MsgFromPer, str;
	int n;
	n = strMsgData.Find(",");
	userName = strMsgData.Left(n);
	MsgFromPer = strMsgData.Right(strMsgData.GetLength() - (n + 1));
	m_MessageList.AddString(userName + " " + MsgFromPer);
}

void CP2PClientDlg::StartSendSocket(CString msgToSend, CString sIP, CString sPort)
{
	m_SelfSendSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (m_SelfSendSocket == INVALID_SOCKET) {
		m_MessageList.AddString("创建socket失败");
		return;
	}
	m_sockSelfSendAddr.sin_family = AF_INET;
	m_sockSelfSendAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	m_sockSelfSendAddr.sin_port = htons(8888);
	if (bind(m_SelfSendSocket, (LPSOCKADDR)&m_sockSelfSendAddr, sizeof(m_sockSelfSendAddr)) == SOCKET_ERROR) {
		m_MessageList.AddString("绑定失败！");
		return;
	}
	m_socketAddrTo.sin_family = AF_INET;
	m_socketAddrTo.sin_addr.S_un.S_addr = inet_addr(sIP);
	m_socketAddrTo.sin_port = htons(atoi(sPort));
	strcpy(m_msgBuf.msg, (LPCTSTR)msgToSend);
	m_msgBuf.i = 0;
	if (sendto(m_SelfSendSocket, (char*)&m_msgBuf, sizeof(m_msgBuf), 0,
		(LPSOCKADDR)&m_socketAddrTo, sizeof(m_socketAddrTo)) == SOCKET_ERROR) {
		m_MessageList.AddString("发送数据失败！");
	}
	closesocket(m_SelfSendSocket);


}

LRESULT CP2PClientDlg::OnReadMsg(WPARAM wParam, LPARAM lParam)
{
	CString str, strHead, strData;
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		socklen = sizeof(m_sockSelfReceiveAddr);
		recvfrom(m_SelfReceiveSocket, (char*)&m_msgBuf, sizeof(m_msgBuf), 0,
			(LPSOCKADDR)&m_sockSelfReceiveAddr, (int*)&socklen);
		WSAAsyncSelect(m_SelfReceiveSocket, m_hWnd, WM_CLIENT_READMSG, FD_READ);
		str.Format("%s", m_msgBuf.msg);
		int n = str.ReverseFind(';');
		strHead = str.Left(n);
		strData = str.Right(str.GetLength() - (n + 1));
		if (strHead == "RGST_ADDR") {
			this->EnrolUsr(strData);
		}
		if (strHead == "UPDT_USR") {
			this->UpdateUser(strData);
		}
		if (strHead == "REQ_ADDR") {
			this->PtoPsrvProvdr(strData);
		}
		if (strHead == "RSP_ADDR") {
			this->SendMsgtoPer(strData);
		}
		if (strHead == "SND_MSG") {
			this->ShowMsgfrmPer(strData);
		}
		break;
	}
	return 0L;
}


void CP2PClientDlg::OnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (LocalP.IsBlank()) {
		AfxMessageBox("请设置本机IP地址！");
		return;
	}
	if (sPort.IsEmpty()) {
		AfxMessageBox("请指定通信端口！");
		return;
	}
	WSADATA wsaData;
	int iErrorCode;
	if (WSAStartup(MAKEWORD(2, 1), &wsaData)) {
		m_MessageList.AddString("Winsock 无法初始化！");
		WSACleanup();
		return;
	}
	m_MessageList.AddString("开始创建Socket..");
	m_SelfReceiveSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (m_SelfReceiveSocket == INVALID_SOCKET) {
		m_MessageList.AddString("创建socket失败！");
		return;
	}
	BYTE nFile[4];
	CString sIP;
	LocalP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);
    m_sockSelfReceiveAddr.sin_family = AF_INET;
	m_sockSelfReceiveAddr.sin_addr.S_un.S_addr = inet_addr(sIP);
	m_sockSelfReceiveAddr.sin_port = htons(atoi(sPort));

	socklen = sizeof(m_sockSelfReceiveAddr);
	if (bind(m_SelfReceiveSocket, (LPSOCKADDR)&m_sockSelfReceiveAddr, sizeof(m_sockSelfReceiveAddr)) == SOCKET_ERROR) {
		m_MessageList.AddString("绑定失败！");
		return;
	}
	int isErrorCode = WSAAsyncSelect(m_SelfReceiveSocket, m_hWnd, WM_CLIENT_READMSG, FD_READ);
	if (isErrorCode == SOCKET_ERROR) {
		m_MessageList.AddString("WSAAsyncSelect 设定失败");
		return;
	}
	m_MessageList.AddString("本机进程启动成功");
	m_MessageList.AddString("地址 " + sIP + " 端口" + sPort);
	this->SetWindowTextA("本机应用进程（" + sIP + ":" + sPort + ")-正在运行...-P2PTalker");
	return;
}


void CP2PClientDlg::OnEnter()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_strUserName.IsEmpty()) {
		AfxMessageBox("给自己取个名字");
		return;
	}
	if (ServerIP.IsBlank()) {
		AfxMessageBox("请设置服务器IP地址！");
		return;
	}
	if (ServerPort.IsEmpty()) {
		AfxMessageBox("请设置服务器端口！");
		return;
	}
	BYTE nFile[4];
	CString sIP;
	LocalP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);
	m_registerAddr = "RGST_ADDR;";
	m_registerAddr += m_strUserName + "," + sIP;
	m_registerAddr +=  ":" + sPort;

	ServerIP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);

	this->StartSendSocket(m_registerAddr, sIP, ServerPort);

	this->SetWindowTextA("已登录P2P服务器（" + sIP + ":" + sPort + ")-P2PTalker");
}


void CP2PClientDlg::OnLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	BYTE nFile[4];
	CString sIP;
	LocalP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);
	m_registerAddr = "RGST_ADDR;";
	m_registerAddr += m_strUserName + "," + sIP;
	m_registerAddr += ":" + sPort;

	ServerIP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);

	this->StartSendSocket(m_registerAddr, sIP, ServerPort);
	m_userList.ResetContent();
	this->SetWindowTextA("已登录P2P服务器（" + sIP + ":" + sPort + ")-P2PTalker");

}


void CP2PClientDlg::OnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MessageList.AddString("正在关闭socket...");
	closesocket(m_SelfReceiveSocket);
	WSACleanup();
	m_MessageList.AddString("本机停止运行");
	this->SetWindowTextA("P2PTalker");
}


void CP2PClientDlg::OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}




void CP2PClientDlg::OnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel;
	nSel = m_userList.GetCurSel();
	if (nSel == LB_ERR) {
		AfxMessageBox("请先从在吸纳用户列表中选择对方网名！");
		return;
	}
	CString UserSel;
	m_userList.GetText(nSel, UserSel);
	UpdateData();
	if (UserSel == m_strUserName) {
		AfxMessageBox("不能向自身发消息！");
		return;
	}
	BYTE nFile[4];
	CString sIP;
	LocalP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);
	m_p2pSrvRequest = "REQ_ADDR;";
	m_p2pSrvRequest += UserSel + "," + sIP;
	m_p2pSrvRequest += ":" + sPort;

	ServerIP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
	sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);

	this->StartSendSocket(m_p2pSrvRequest, sIP, ServerPort);

}
