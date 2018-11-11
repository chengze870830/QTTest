#pragma once
#include "UIlib.h"
using namespace DuiLib;

class CDlgUserLogin : public WindowImplBase
{
public:
	CDlgUserLogin(void);
	virtual ~CDlgUserLogin(void);

public:
	CString GetIpAddress();
	void SetIpAddress(CString strIP);

	UINT GetPort();
	void SetPort(UINT nPort);

	int GetLanguageType();
	void SetLanguageType(int nLanguageType);

	void InitUsers();
	CUser& GetUser();

public:
	virtual void InitWindow();
	virtual void OnFinalMessage( HWND hWnd );
	virtual LPCTSTR GetWindowClassName() const;

	virtual CDuiString GetSkinFolder() ;
	virtual CDuiString GetZIPFileName() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFile() ;

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	void SetWindowShape();

protected:
	void OnLogin();

public:
	CUser			m_objUser;
	CString			m_strIpAddress;
	UINT			m_nServerPort;
	INT				m_nWndWidth;
	int				m_nMinHeight;
	int				m_nMaxHeight;
	BOOL			m_bShowConfig;
	int				m_nLanguageType;//0 为中文 1为英文 OBJ_STRING_NAME_EDIT_INT

private:
	CEditUI*		m_pEditUsr;
	CEditUI*		m_pEditPass;
	CEditUI*		m_pEditIp;
	CEditUI*		m_pEditPort;
	COptionUI*		m_pBtnCheck;
	CComboUI*       m_pCombLanguage;
	CComboUI*       m_pUserList;
	CLabelUI*		m_pLabLog;
	CLabelUI*		m_pLabServerAddress;
	CButtonUI*      m_pBtnLogin;

protected:
	DUI_DECLARE_MESSAGE_MAP();
	afx_msg void OnClick(TNotifyUI& msg);
	afx_msg void OnCombChange(TNotifyUI& msg);
};
