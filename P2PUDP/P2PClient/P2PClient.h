﻿
// P2PClient.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CP2PClientApp:
// 有关此类的实现，请参阅 P2PClient.cpp
//

class CP2PClientApp : public CWinApp
{
public:
	CP2PClientApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CP2PClientApp theApp;
