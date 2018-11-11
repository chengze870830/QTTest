
// MonitorMiddleTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once   

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif
#include "resource.h"		// ������
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
	int IsExpire();//�Ƿ��ڣ�2016��5��1�ţ�����ʹ�ñ����, -1:û���������磬0,û�е��ڣ�1���Ѿ�����
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