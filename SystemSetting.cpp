#include "StdAfx.h"
#include "SystemSetting.h"
#include "io.h"

CSystemSetting::CSystemSetting(void)
{
	m_bShutdownFlag = false;
	m_bShowDoorPersonInfo = false;
	m_bShowOnlyAttentionCar = false;
	m_bEnableSystemAudeio = false;

	m_bEnableRingSecurity = false;
	m_bEnableRingDoorCheckIn = false;
	m_bEnableRingDoorCall = false;
}

CSystemSetting::~CSystemSetting(void)
{
}

void CSystemSetting::SetExitShutdownFlag(bool bFlag)
{
	m_bShutdownFlag = bFlag;
}

bool CSystemSetting::GetExitShutdownFlag()const
{
	return m_bShutdownFlag;
}

void CSystemSetting::SetShowDoorPersonInfoFlag(bool bFlag)
{
	m_bShowDoorPersonInfo = bFlag;
}

bool CSystemSetting::GetShowDoorPersonInfoFlag()
{
	return m_bShowDoorPersonInfo;
}

bool CSystemSetting::RestartSelf()
{
	CHAR szAppPath[MAX_PATH] = {0}, szAppDir[MAX_PATH] = {0};
	WORD nSize = ::GetModuleFileNameA(NULL, szAppPath, _countof(szAppPath));
	if(nSize == 0)
	{		
		ATXTRACE1(LOG_LEVEL_ERROR, _T("GetModuleFileNameA Failed "));
		return false;
	}
	
	CHAR *p = strrchr(szAppPath, '\\');
	strncpy_s(szAppDir, szAppPath, p - szAppPath + 1);

	CHAR szCmdPath[MAX_PATH] = {0};
	sprintf_s(szCmdPath, _countof(szCmdPath), "%s%s", szAppDir, "restartself.cmd");
	if(::_access(szCmdPath,0)== -1)
	{
		//文件不存在
		FILE *fp = fopen(szCmdPath, "wt");
		if (fp == NULL)
		{
			ATXTRACE1(LOG_LEVEL_ERROR, _T("Open Local RestartCmdFile Failed! "));
			return false;
		}

		fwrite("ping 127.0.0.1 -n 2 -w 1000 > nul\r\n\"", strlen("ping 127.0.0.1 -n 2 -w 1000 > nul\r\n\""), 1, fp);
		fwrite(szAppPath, strlen(szAppPath), 1, fp);
		fwrite("\"\r\n", strlen("\"\r\n"), 1, fp);
		fclose(fp);
		
	}
	
	//::ShellExecuteA(NULL, NULL, szCmdPath, NULL, NULL, SW_SHOW); // 如果当前程序为控制台程序，那么会打开一个新的控制台
	::WinExec(szCmdPath, SW_HIDE); // 如果当前程序为控制台程序，那么会在当前控制台进行重启，不会打开新的控制台
	return ::TerminateProcess(::GetCurrentProcess(), 0);
}

