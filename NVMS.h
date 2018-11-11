
// MonitorMiddleTest.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once   

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif
#include "resource.h"		// 主符号
#include "DlgMonitor.h"
#include "libnet_rs.h"
#pragma comment(lib, "libnet_rs.lib")

class CNVMSApp : public CWinAppEx
{
public:
	CNVMSApp();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	CDlgMonitor* GetCDlgMonitor();
	BOOL ShowCurrentWindow();
	void InitailizeLog(void);
	void UnInitailizeLog(void);
	void ForceExit(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	int IsExpire();//是否到期，2016年5月1号，不能使用本软件, -1:没有连接网络，0,没有到期，1：已经到期
	BOOL AnalysisDate(CString strDate);
	void ShutDownWindows();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CDlgMonitor m_dlg;
	u_long		m_lGDIPlusHandle;
	int			nLanguageType;
	BOOL		m_bStartup;
};

extern CNVMSApp theApp;
extern BOOL g_bRestart, g_bPowerOff;