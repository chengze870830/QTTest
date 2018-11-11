#include "StdAfx.h"
#include "DlgUserLogin.h"
#include "CheckUtil.h"
#include "LocalSettingManager.h"

DUI_BEGIN_MESSAGE_MAP(CDlgUserLogin, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnCombChange)
DUI_END_MESSAGE_MAP()

CDlgUserLogin::CDlgUserLogin(void)
{
	m_strIpAddress = _T("127.0.0.1");
	m_nServerPort = 9000;
	m_nLanguageType = 0 ;

	m_nWndWidth = 748;
	m_nMinHeight = 228;
	m_nMaxHeight = 373;
	m_bShowConfig = FALSE;

	m_pEditUsr = NULL;
	m_pEditPass = NULL;
	m_pEditIp = NULL;
	m_pEditPort = NULL;
	m_pBtnCheck = NULL;
	m_pCombLanguage = NULL;
	m_pLabLog = NULL;
	m_pLabServerAddress = NULL;
	m_pBtnLogin = NULL;
	m_pUserList = NULL;
}

CDlgUserLogin::~CDlgUserLogin(void)
{
}

CString CDlgUserLogin::GetIpAddress()
{
	return m_strIpAddress;
}

void CDlgUserLogin::SetIpAddress(CString strIP)
{
	m_strIpAddress = strIP;
}

UINT CDlgUserLogin::GetPort()
{
	return m_nServerPort;
}

void CDlgUserLogin::SetPort(UINT nPort)
{
	m_nServerPort = nPort;
}

int CDlgUserLogin::GetLanguageType()
{
	return m_nLanguageType;
}

void CDlgUserLogin::SetLanguageType(int nLanguageType)
{
	m_nLanguageType = nLanguageType;
}

void CDlgUserLogin::InitUsers()
{
	if(NULL == m_pUserList)
		return ;

	m_pUserList->RemoveAll();

	list<CUser>& objUsers = CLocalSettingManager::GetInstance()->GetRecentLoginUsers();
	for (list<CUser>::iterator it = objUsers.begin(); it != objUsers.end(); ++it)
	{
	   CListLabelElementUI* pLabel = new CListLabelElementUI();
	   m_pUserList->Add(pLabel);
	   pLabel->SetText(it->GetUserName());
	   pLabel->SetAttribute(_T("font"), _T("1"));
	}

	if (objUsers.size() > 0)
	{
		m_pUserList->SelectItem(0);
	}
}

CUser& CDlgUserLogin::GetUser()
{
	return m_objUser;
}

void CDlgUserLogin::InitWindow()
{
	CRect rtNormal;
	GetClientRect(m_hWnd, &rtNormal);

	m_pEditUsr = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("username")));
	m_pEditPass = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("password")));
	m_pEditIp = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("serverip")));
	m_pEditPort = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("serverport")));
	m_pBtnCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("rememberpass")));
	m_pCombLanguage = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("language")));
	m_pUserList = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("userlist")));
	CHorizontalLayoutUI* pFixPartUI = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("fixpart")));
	m_pLabServerAddress = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("serveraddress")));
	m_pBtnLogin = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("loginbtn")));

	ASSERT(NULL != m_pEditUsr);
	ASSERT(NULL != m_pEditPass);
	ASSERT(NULL != m_pEditIp);
	ASSERT(NULL != m_pEditPort);
	ASSERT(NULL != m_pBtnCheck);
	ASSERT(NULL != pFixPartUI);
	ASSERT(NULL != m_pCombLanguage);
	ASSERT(NULL != m_pLabServerAddress);
	ASSERT(NULL != m_pBtnLogin);
	ASSERT(NULL != m_pUserList);

	CString strPort;
	strPort.Format(_T("%d"), m_nServerPort);

	m_pEditIp->SetText(m_strIpAddress);
	m_pEditPort->SetText(strPort);
	m_pCombLanguage->SelectItem(m_nLanguageType);

	m_nWndWidth = rtNormal.Width();
	m_nMaxHeight = rtNormal.Height();
	m_nMinHeight = pFixPartUI->GetFixedHeight();

	InitUsers();

	m_pBtnLogin->SetFocus();
	SetWindowShape();
	SetWindowPos(m_hWnd, NULL, 0, 0, m_nWndWidth, m_nMinHeight, SWP_NOZORDER|SWP_NOMOVE);
}

void CDlgUserLogin::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
}

LPCTSTR CDlgUserLogin::GetWindowClassName() const
{
	return _T("DlgUserLogin");
}

CDuiString CDlgUserLogin::GetSkinFolder() 
{
#if 1
	CDuiString strSkinFolder ;
	if(m_nLanguageType == 0)
	{
		strSkinFolder = (LPCTSTR)CSystemImage::GetInstance()->GetRelativePath(IMG_ID_CONFIG_CLIENT_SKINFOLDER_CHINESE);
	}
	else if (m_nLanguageType == 1)
	{
		strSkinFolder = (LPCTSTR)CSystemImage::GetInstance()->GetRelativePath(IMG_ID_CONFIG_CLIENT_SKINFOLDER_ENGLISH);
	}

	strSkinFolder += _T("/");
	return strSkinFolder;
#endif 
	return CLocalSettingManager::GetInstance()->GetSkinFolder();
}

CDuiString CDlgUserLogin::GetZIPFileName() const
{
	return CLocalSettingManager::GetInstance()->GetZipFile();
}

UILIB_RESOURCETYPE CDlgUserLogin::GetResourceType() const
{
#if 1
	return  UILIB_FILE;
#endif
	return UILIB_ZIP;
}

CDuiString CDlgUserLogin::GetSkinFile() 
{
	CDuiString strLoginSkinFile = (LPCTSTR)CSystemImage::GetInstance()->GetRelativePath(IMG_ID_LOGIN_SKIN_FILE);
	return strLoginSkinFile;
}

void CDlgUserLogin::OnLogin()
{
	if(NULL != m_pCombLanguage)
	{
		m_nLanguageType = m_pCombLanguage->GetCurSel();
	}

	CLocalSettingManager::GetInstance()->SetLanguageType(m_nLanguageType);

	WORD wLanguage = CLocalSettingManager::GetInstance()->GetLanguageID();
	if (NULL != m_pEditUsr)
	{
		m_objUser.SetUserName((LPCTSTR)m_pEditUsr->GetText());
		if (m_objUser.GetUserName().IsEmpty())
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_USER_NAME_NOT_NULL),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditUsr->SetFocus();
			return ;
		}

		if(-1 != m_objUser.GetUserName().Find(_T(" ")))
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_USER_NAME_ILLEGAL),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditUsr->SetFocus();
			return ;
		}

		if (!CCheckUtil::CheckillegalStr(m_objUser.GetUserName()))
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_USER_NAME_ILLEGAL),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditUsr->SetFocus();
			return ;
		}
	}

	if (NULL != m_pEditPass)
	{
		m_objUser.SetPassWord((LPCTSTR)m_pEditPass->GetText());
		if (m_objUser.GetPassWord().IsEmpty())
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_PASS_NOT_NULL),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditPass->SetFocus();
			return ;
		}

		if(-1 != m_objUser.GetPassWord().Find(_T(" ")))
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_PASS_WORD_ILLEGAL),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditPass->SetFocus();
			return ;
		}

		if (!CCheckUtil::CheckillegalStr(m_objUser.GetPassWord()))
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_PASS_WORD_ILLEGAL),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditPass->SetFocus();
			return ;
		}
	}

	if (NULL != m_pEditIp)
	{
		m_strIpAddress = m_pEditIp->GetText();
		if (m_strIpAddress.IsEmpty())
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_SERVER_ADDRESS_IS_NULL),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditIp->SetFocus();
			return ;
		}

		if (!CCheckUtil::CheckIPStr(m_strIpAddress))
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_IP_ERROR),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditIp->SetFocus();
			return ;
		}
	}

	if (NULL != m_pEditPort)
	{
		CString strPort = m_pEditPort->GetText();
		if (strPort.IsEmpty())
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_SERVER_PORT_IS_NULL),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditPort->SetFocus();
			return ;
		}

		if (!CCheckUtil::CheckillegalStr(strPort))
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_INVALIDPORT),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditPort->SetFocus();
			return ;
		}

		m_nServerPort = _ttoi(strPort);
		if (m_nServerPort <= 0 || m_nServerPort > 65535)
		{
			MessageBoxEx(m_hWnd, GetResString(IDS_INVALIDPORT),  GetResString(IDS_ERROR), MB_OK|MB_ICONERROR, wLanguage);
			m_pEditPort->SetFocus();
			return ;
		}
	}
	if (NULL != m_pBtnCheck)
	{
		m_objUser.IsRememberPwd() = m_pBtnCheck->IsSelected();
	}

	Close(IDOK);
}

void CDlgUserLogin::OnClick(TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("configbtn"))
	{
		if (!m_bShowConfig)
		{
			SetWindowPos(m_hWnd, NULL, 0, 0, m_nWndWidth, m_nMaxHeight, SWP_NOZORDER|SWP_NOMOVE);
		}
		else
		{
			SetWindowPos(m_hWnd, NULL, 0, 0, m_nWndWidth, m_nMinHeight, SWP_NOZORDER|SWP_NOMOVE);
		}
		m_bShowConfig = !m_bShowConfig;
	}
	else if (msg.pSender->GetName() == _T("loginbtn"))
	{
		OnLogin();
	}
	else if (msg.pSender->GetName() == _T("closebtn"))
	{
		Close(IDCANCEL);
	}
}

void CDlgUserLogin::OnCombChange(TNotifyUI& msg)
{
	if(msg.pSender->GetName() == _T("language"))
	{
		int nType = m_pCombLanguage->GetCurSel();
		if(nType == -1)
			return;
		switch(nType)
		{
		case 0:
			{
				//m_pLabLog->SetText(_T("     视频管理客户端"));
				//m_pLabLog->SetFont(1);
				m_pBtnCheck->SetText(_T("保存密码"));
				m_pLabServerAddress->SetText(_T("服务器地址:"));
				Language_LoadLanguage(_T("简体中文"));
				CDuiString strTemp = CSystemImage::GetInstance()->GetRelativePath(IMG_ID_CONFIG_CLIENT_SKINFOLDER_CHINESE);
				CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + strTemp);
				CPaintManagerUI::ReloadSkin();
			}
			break;
		case 1:
			{
				//m_pLabLog->SetText(_T("Configuration Client"));
				//m_pLabLog->SetFont(2);
				m_pBtnCheck->SetText(_T("Remember"));
				m_pLabServerAddress->SetText(_T("Server Address:"));
				Language_LoadLanguage(_T("English"));
				CDuiString strTemp = CSystemImage::GetInstance()->GetRelativePath(IMG_ID_CONFIG_CLIENT_SKINFOLDER_ENGLISH);
				CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + strTemp);
				CPaintManagerUI::ReloadSkin();
			}
			break;
		}
	}
	else if (msg.pSender->GetName() == _T("userlist"))
	{
		CString strUser = m_pUserList->GetItemAt(m_pUserList->GetCurSel())->GetText();
		m_pUserList->SelectItem(-1);
		m_objUser.SetUserName(strUser);
		if (CLocalSettingManager::GetInstance()->GetUser(m_objUser))
		{
			m_pEditUsr->SetText(m_objUser.GetUserName());
			m_pBtnCheck->Selected(m_objUser.IsRememberPwd());
			if (m_objUser.IsRememberPwd())
			{
				m_pEditPass->SetText(m_objUser.GetPassWord());
			}
			else
			{
				m_pEditPass->SetText(_T(""));
			}
		}
	}
}

LRESULT CDlgUserLogin::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (WM_KEYDOWN == uMsg && VK_RETURN == wParam)
	{
		bHandled = TRUE;

		OnLogin();
	}
	return WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

LRESULT CDlgUserLogin::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}
	if (uMsg == WM_SYSKEYDOWN && wParam == VK_F4 ) 	//屏蔽ALT+F4
	{
		return TRUE;
	}

	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDlgUserLogin::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (WM_KEYDOWN == uMsg && VK_ESCAPE == wParam)
	{
		bHandled = TRUE;
		Close(IDCANCEL);
	}

	if (WM_KEYDOWN == uMsg && VK_SPACE == wParam)
	{
		bHandled = TRUE;
	}

	return 0;
}

void CDlgUserLogin::SetWindowShape() 
{ 
	// 区域多边形轮廓各顶点坐标（逆时针排列）
	CPoint points[] = {CPoint(25 , 0),CPoint(0, 59),CPoint(124, 227),CPoint(124 , 346),CPoint(152 , 376),CPoint(669 , 376),
	                   CPoint(697 , 346),CPoint(697, 227),CPoint(747 , 109),CPoint(654, 0),CPoint(25 , 0)};
	int nSize = sizeof(points)/sizeof(CPoint);

	HRGN hRgn = CreatePolygonRgn(points, nSize, ALTERNATE);
	SetWindowRgn(m_hWnd, hRgn, TRUE);
	::DeleteObject(hRgn);
	hRgn = NULL;
}
