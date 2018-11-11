
// MonitorMiddleTest.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "NVMS.h"
#include "BaseException.h"
#include "DlgUserLogin.h"
#include "LocalSettingManager.h"
#include "DlgForceExit.h"
#include "FileUtil.h"
#include "SystemSetting.h"

 //#include "vld.h"
 //#pragma comment(lib, "vld_32.lib")

#define EVENT_CONFIG_CLIENT		_T("Global\\AJB_MapClient")

// CNVMSApp
BEGIN_MESSAGE_MAP(CNVMSApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CNVMSApp::CNVMSApp()
{
	m_lGDIPlusHandle = 0;
	m_bStartup = FALSE;
}

CNVMSApp theApp;
BOOL g_bRestart = FALSE, g_bPowerOff = FALSE;
CDlgMonitor* CNVMSApp::GetCDlgMonitor()
{
	return &m_dlg;
}

void CNVMSApp::InitailizeLog(void)
{
	//初始化日志资源
	TCHAR szAppDir[MAX_PATH] = {0};
	TCHAR szLogDir[MAX_PATH] = {0};
	TCHAR szExcpDir[MAX_PATH]= {0};

	CFileUtil::GetAppPath(szAppDir,MAX_PATH);

	_tcscpy_s(szLogDir,MAX_PATH,szAppDir);
	_tcscat(szLogDir,_T("AppLog"));
	_tcscpy_s(szExcpDir,MAX_PATH,szAppDir);
	_tcscat(szExcpDir,_T("Exception"));

	CFileUtil::CreateDirectory(szLogDir);
	CFileUtil::CreateDirectory(szExcpDir);
	LOG_INITAILIZE(szLogDir);

	CSE_Exception::InitSEException(szExcpDir);
}

void CNVMSApp::UnInitailizeLog(void)
{
	//释放日志资源
	LOG_UNINITIALIZE();

	//释放语言库资源
	Language_UnLoadLanguage();
}

BOOL CNVMSApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	DuiLib::CPaintManagerUI::SetInstance(AfxGetInstanceHandle());

	InitailizeLog();
	TTS_Init();

	int nLanguageType = CLocalSettingManager::GetInstance()->GetLanguageType();
	Language_SetLanguageFilesDir((LPCTSTR)CSystemImage::GetInstance()->GetSkinFileDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER));
	switch(nLanguageType)
	{
	case -1:
		{
			LCID lcid = GetUserDefaultLCID();
			if(lcid == 0x804)
			{ 
				CString strCurLanguage = _T("简体中文");
				CSystemImage::GetInstance()->SelectResourceDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER_CHINESE);
				Language_LoadLanguage(strCurLanguage);
				CLocalSettingManager::GetInstance()->SetLanguageType(0);
			}
			else
			{
				CString strCurLanguage = _T("English");
				CSystemImage::GetInstance()->SelectResourceDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER_ENGLISH);
				Language_LoadLanguage(strCurLanguage);
				CLocalSettingManager::GetInstance()->SetLanguageType(1);
			}	
		}
		break;
	case 0 :
		{
			CString strCurLanguage = _T("简体中文");
			CSystemImage::GetInstance()->SelectResourceDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER_CHINESE);
			Language_LoadLanguage(strCurLanguage);
		}
		break;
	case 1:
		{
			CString strCurLanguage = _T("English");
			CSystemImage::GetInstance()->SelectResourceDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER_ENGLISH);
			Language_LoadLanguage(strCurLanguage);

		}
		break;
	default:
		{
			CString strCurLanguage = _T("简体中文");
			CSystemImage::GetInstance()->SelectResourceDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER_CHINESE);
			Language_LoadLanguage(strCurLanguage);
		}
		break;
	}

  	HANDLE hEvent = ::OpenEvent(EVENT_ALL_ACCESS,FALSE,EVENT_CONFIG_CLIENT);
  	if (NULL != hEvent)
  	{
  		ShowCurrentWindow();
  		return TRUE;
  	}
	m_bStartup = TRUE;

	hEvent = CreateEvent(NULL,FALSE,FALSE,EVENT_CONFIG_CLIENT);
	ASSERT(NULL != hEvent);

	AfxEnableControlContainer();
	SetRegistryKey(GetResString(IDS_VIDEO_CONFIG_CLIENT));

	GdiplusStartupInput objInput;
	Status stRet = GdiplusStartup(&m_lGDIPlusHandle, &objInput, NULL);
	if (Ok != stRet)
	{
		//AfxMessageBox(GetResString(IDS_GDI_ERROR));
		MessageBoxEx(NULL, GetResString(IDS_GDI_ERROR), _T("ConfigClient"), 
			MB_ICONWARNING | MB_OK, CLocalSettingManager::GetInstance()->GetLanguageID());
		return FALSE;
	}

#ifdef EXPIRE_CHECK
	//////////////////3个月的试用期////////////////////
	int nRet = IsExpire();
	if(-1 == nRet)//没有连接网络
	{
		MessageBoxEx(NULL, GetResString(IDS_INIT_NET_ERROR), _T("ConfigClient"), 
				MB_OK, CLocalSettingManager::GetInstance()->GetLanguageID());
		return FALSE;
	}else if (1 == nRet)//已经到期
	{
		MessageBoxEx(NULL, GetResString(IDS_INIT_OUTOF_DATE), _T("ConfigClient"), 
			MB_OK, CLocalSettingManager::GetInstance()->GetLanguageID());
		return FALSE;
	}
	//////////////////////
#endif

	while (TRUE)
	{
		CDlgUserLogin objLogin;
		objLogin.SetIpAddress(CLocalSettingManager::GetInstance()->GetCfgServerIp());
		objLogin.SetPort(CLocalSettingManager::GetInstance()->GetCfgServerPort());
		objLogin.SetLanguageType(CLocalSettingManager::GetInstance()->GetLanguageType());

		objLogin.Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
		objLogin.CenterWindow();
		if (IDCANCEL == objLogin.ShowModal())
			return FALSE;

		CString strIp = objLogin.GetIpAddress();
		UINT nPort = objLogin.GetPort();
		CUser& objUser = objLogin.GetUser();

		nLanguageType = objLogin.GetLanguageType();
		Language_SetLanguageFilesDir((LPCTSTR)CSystemImage::GetInstance()->GetSkinFileDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER));
		switch(nLanguageType)
		{
		case 0 :
			{
				CString strCurLanguage = _T("简体中文");
				CSystemImage::GetInstance()->SelectResourceDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER_CHINESE);
				Language_LoadLanguage(strCurLanguage);
			}
			break;
		case 1:
			{
				CString strCurLanguage = _T("English");
				CSystemImage::GetInstance()->SelectResourceDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER_ENGLISH);
				Language_LoadLanguage(strCurLanguage);
			}
			break;
		default:
			{
				CString strCurLanguage = _T("简体中文");
				CSystemImage::GetInstance()->SelectResourceDir(IMG_ID_CONFIG_CLIENT_SKINFOLDER_CHINESE);
				Language_LoadLanguage(strCurLanguage);
			}
			break;
		}

		CLocalSettingManager::GetInstance()->SetCfgServerIp(strIp);
		CLocalSettingManager::GetInstance()->SetCfgServerPort(nPort);
		CLocalSettingManager::GetInstance()->SetLanguageType(nLanguageType);

		CLoginInfo	objloginInfo;
		objloginInfo.SetServerIP(strIp);
		objloginInfo.SetServerPort(nPort);
		objloginInfo.SetLoginUser(objUser);
		WORD wLanguage = CLocalSettingManager::GetInstance()->GetLanguageID();

		int bIsSucc = CConfigSDK::GetInstance()->Login(objloginInfo);
		if (1 == bIsSucc)
		{
			CConfigSDK::GetInstance()->RegisterMessageCallBack(CDlgConfigClient::MessageCallBack, &m_dlg);
			CLocalSettingManager::GetInstance()->AddUser(objUser);
			CLocalSettingManager::GetInstance()->SaveConfig();

			break;
		}
		else if (AJB_ERROR_CLIENT_HASLOGINED == bIsSucc)
		{
			if (IDOK == MessageBoxEx(NULL, GetResString(IDS_FORCLOG), GetResString(IDS_WARNING), MB_OKCANCEL | MB_ICONWARNING, wLanguage))
			{
				if (1 == CConfigSDK::GetInstance()->Login(objloginInfo, TRUE))
				{
						CConfigSDK::GetInstance()->RegisterMessageCallBack(CDlgConfigClient::MessageCallBack, &m_dlg);
					CLocalSettingManager::GetInstance()->AddUser(objUser);
					CLocalSettingManager::GetInstance()->SaveConfig();

					break;
				}
				else
				{
					CString str = GetResString(IDS_LOGIN_FAIL_TIP);
					if (MessageBoxEx(NULL, GetResString(IDS_LOGIN_FAIL_TIP), GetResString(IDS_ERROR), MB_YESNO, wLanguage) != IDYES)
					{
						return FALSE;
					}
				}
			}
		}
		else
		{
			CString str = GetResString(IDS_LOGIN_FAIL_TIP);
			if (MessageBoxEx(NULL, GetResString(IDS_LOGIN_FAIL_TIP), GetResString(IDS_ERROR), MB_YESNO, wLanguage) != IDYES)
			{
				return FALSE;
			}
		}
	}

	m_pMainWnd = &m_dlg;
	m_dlg.Create(m_dlg.IDD, NULL);
	m_dlg.CenterWindow(NULL);
	m_dlg.UpdateWindow();
	m_dlg.InitData();

    if (m_dlg.IsShowAutoSvrConfig())
    {
	   SetWindowPos(m_dlg.GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
    }

	m_dlg.ShowWindow(SW_SHOWNORMAL);
   	return TRUE;
}

int CNVMSApp::ExitInstance()
{
	CAutoWaitDlg objWaitDlg(GetResString(IDS_CLOSE_SYS_WAIT));
	CWinAppEx::ExitInstance();

	// 第一个进程已经启动，启动第二个进程时候不做保存
	if (m_bStartup)
	{
		// 退出时关闭进程同步事件，防止注销用户进程无法起来
		HANDLE hEvent = ::OpenEvent(EVENT_ALL_ACCESS,FALSE,EVENT_CONFIG_CLIENT);
		if (NULL != hEvent)
		{
			CloseHandle(hEvent);
			hEvent = NULL;
		}

		if (NULL != m_dlg.m_hWnd)
		{
			m_dlg.DestroyWindow();
		}
		CLocalSettingManager::GetInstance()->SaveConfig();	

		if (m_lGDIPlusHandle > 0)
		{
			GdiplusShutdown(m_lGDIPlusHandle);
			m_lGDIPlusHandle = 0;
		}

		if (!g_bRestart && 
			CLocalSettingManager::GetInstance()->GetSystemSetting().GetExitShutdownFlag() && g_bPowerOff)
		{
			ShutDownWindows();
		}

		if(g_bRestart)
		{
			CSystemSetting::RestartSelf();
		}
	}

	UnInitailizeLog();
	TTS_UnInit();

	return 0;
}

BOOL CNVMSApp::ShowCurrentWindow()
{
	CWnd *pWndPrev;
	// Determine if a window with the class name exists...
	pWndPrev = CWnd::FindWindow(NULL, _T("MapClient"));
	if (NULL != pWndPrev)
	{
		SetForegroundWindow(pWndPrev->GetSafeHwnd());
		pWndPrev->ShowWindow(SW_SHOWMAXIMIZED);
		return TRUE;
	}

	return FALSE;
}

int CNVMSApp::IsExpire()
{
	int nRet = -1;
	struct hostent *pHost = gethostbyname("www.baidu.com"); // www.baidu.com time.windows.com
	if(pHost != NULL) // 此方法还可以判断本机是否联网
	{
		char **pptr = NULL;
		char szDestIP[128] = {0};
		for(pptr = pHost->h_addr_list ; NULL != *pptr ; ++pptr)
		{
			//::inet_ntop(pHost->h_addrtype, *pptr, szDestIP, sizeof(szDestIP)); // 此函数有的操作系统中的WS2_32.dll中不存在
			//::strcpy_s(szDestIP, inet_ntoa(*((struct in_addr*)pHost->h_addr)));
			::strcpy_s(szDestIP, inet_ntoa(*((struct in_addr *)(*pptr))));
		}

		char szBuf[4096] = {0};
		HANDLE hSession = ::libnet_connect(::ntohl(inet_addr(szDestIP)), 80);
		if (hSession != NULL)
		{
			::libnet_send_client(hSession, "GET http://time.windows.com HTTP/1.0\r\nContent-Type: text/xml;Charset=UTF-8\r\n\r\n", strlen("GET http://time.windows.com HTTP/1.0\r\nContent-Type: text/xml;Charset=UTF-8\r\n\r\n"));
			int nRecved = ::libnet_recv_client(hSession, szBuf, sizeof(szBuf));
			if (nRecved>0)
			{
				CString strTime(szBuf);
				if (AnalysisDate(strTime))
				{
					nRet = 1;//到期
				}else
				{
					nRet = 0;//在使用期内
				}
			}
		}
	}
	return nRet;
}
// 解析日期
 BOOL CNVMSApp::AnalysisDate(CString strDate)
{
	int nIndex = strDate.Find(_T("Date:"));
	if (nIndex == -1)
	{
		return FALSE;
	}
	nIndex = nIndex + 14;
	strDate = strDate.Right(strDate.GetLength()- nIndex);
	strDate = strDate.Left(22);
	//Jan 2016 07:38:16 GMT
	CString strYear = strDate.Left(9);
	CString strMonth = strDate.Left(3);
	strYear = strYear.Right(strYear.GetLength()-3);
	if (_ttoi(strYear.Trim())>2016)
	{
		return TRUE;
	}else
	{
		strMonth.Trim();

		if (strMonth == _T("Feb") || strMonth == _T("Mar") || strMonth == _T("Apr") || strMonth == _T("Jan") )
		{
			return FALSE;
		}
	}
	return TRUE;
 }

void CNVMSApp::ForceExit(WPARAM wParam,LPARAM lParam)
 {
	 CRect rtWnd;
	 CGetDlgInstance::GetDlgMonitor()->GetWindowRect(rtWnd);
	 CDlgForecExit objForceExit;
	 objForceExit.Create(CGetDlgInstance::GetDlgMonitor()->GetSafeHwnd(), NULL, UI_WNDSTYLE_DIALOG, WS_EX_TOPMOST, 0, 0, 0, 0, NULL);
	 ::MoveWindow(objForceExit.GetHWND(),rtWnd.left, rtWnd.top, rtWnd.Width(), rtWnd.Height(),FALSE);
	 objForceExit.CenterWindow();
	 objForceExit.ShowModal();
	 
	 TCHAR* strProcessName = L"MapClient.exe";
	 CFileUtil::StopProcess(strProcessName);

 }

 BOOL CNVMSApp::PreTranslateMessage(MSG* pMsg)
 {
	 // TODO: 在此添加专用代码和/或调用基类
	 if (pMsg->message == WM_FORCE_EXITEX)
	 {
		 ForceExit(pMsg->wParam, pMsg->lParam);
		 return 0;
	 }

	 return CWinAppEx::PreTranslateMessage(pMsg);
 }

 void CNVMSApp::ShutDownWindows()
 {
	 HANDLE hToken; 
	 TOKEN_PRIVILEGES tkp; 

	 // Get a token for this process. 

	 if (!OpenProcessToken(GetCurrentProcess(), 
		 TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		 return ; 

	 // Get the LUID for the shutdown privilege. 

	 LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
		 &tkp.Privileges[0].Luid); 

	 tkp.PrivilegeCount = 1; // one privilege to set 
	 tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	 // Get the shutdown privilege for this process. 

	 AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
		 (PTOKEN_PRIVILEGES)NULL, 0); 

	 if (GetLastError() != ERROR_SUCCESS) 
		 return ; 

	 // Shut down the system and force all applications to close. 

	 if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) 
		 return ; 
 }

