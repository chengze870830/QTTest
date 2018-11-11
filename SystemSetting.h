#pragma once

class CSystemSetting
{
public:
	CSystemSetting(void);
	~CSystemSetting(void);
	
public:
	void SetExitShutdownFlag(bool bFlag);
	bool GetExitShutdownFlag()const;

	void SetShowDoorPersonInfoFlag(bool bFlag);
	bool GetShowDoorPersonInfoFlag();

	void SetShowOnlyAttentionCarFlag(bool bFlag){m_bShowOnlyAttentionCar = bFlag;}
	bool GetShowOnlyAttentionCarFlag(){return m_bShowOnlyAttentionCar;} 

	bool GetEnableSystemAudiolag(){return m_bEnableSystemAudeio;}
	bool SetEnableSystemAudiolag(bool bFlag){return m_bEnableSystemAudeio = bFlag;}

	bool GetEnableRingSecurityFlag(){return m_bEnableRingSecurity;}
	bool SetEnableRingSecurityFlag(bool bFlag){return m_bEnableRingSecurity = bFlag;}

	bool GetEnableRingDoorCheckInFlag(){return m_bEnableRingDoorCheckIn;}
	bool SetEnableRingDoorCheckInFlag(bool bFlag){return m_bEnableRingDoorCheckIn = bFlag;}

	bool GetEnableRingDoorCallFlag(){return m_bEnableRingDoorCall;}
	bool SetEnableRingDoorCallFlag(bool bFlag){return m_bEnableRingDoorCall = bFlag;}

	bool GetEnableDisplayChange() const { return m_bEnableDisplayChange; }
	void SetEnableDisplayChange(bool val) { m_bEnableDisplayChange = val; }

	static bool RestartSelf();

private:
	bool m_bShutdownFlag;
	bool m_bShowDoorPersonInfo;
	bool m_bShowOnlyAttentionCar;
	bool m_bEnableSystemAudeio;

	bool m_bEnableRingSecurity;
	bool m_bEnableRingDoorCheckIn;
	bool m_bEnableRingDoorCall;
	bool m_bEnableDisplayChange;
};
