#pragma once

class CCheckUtil
{
public:
	CCheckUtil(void);
	~CCheckUtil(void);

public:
	static bool CheckIPStr(const CString& ip);
	static bool CheckSubMaskStr(const CString& ip);
	static bool CheckPort(int nPort);
	static bool CheckPort(const CString& strPort);
	static bool SplitStr(const CString strSrc, const CString division, std::list<CString>& strList);
	static bool CheckMacStr(const CString& strMac);
	static bool CheckMail(const CString& strEmail);
	static bool CheckNum(const CString& strNum);			//	�ж��Ƿ�Ϊ����
	static bool CheckPositiveNum(const CString& strNum);    //  �ж��Ƿ�Ϊ������
	static bool CheckGuid(const CString& strGuid);			//	�ж��Ƿ����GUID��ʽ
	static bool CheckIPV6(const CString& strIPv6);
	static bool CheckillegalStr(const CString& str);		//	�ж��Ƿ��зǷ��ַ�
	static bool RegexMatch(LPCTSTR lpctSrc, LPCTSTR lpctMatch);

	static bool CheckId(const CString& param);         // ������֤���Ƿ�Ϸ�
	static bool CheckMobileNum(const CString& param);  // ����ֻ������Ƿ�Ϸ�
	static bool CheckTelNum(const CString& param);     // ������������Ƿ�Ϸ�
	static bool CheckCarNum(const CString& param);     // ��⳵�ƺ����Ƿ�Ϸ�
	static bool CheckCardNo(const CString& param);	   // ����Ž������Ƿ�Ϸ�
	static bool CheckMoney(const CString& param);	   // ������Ƿ�Ϸ�
	static bool CheckHaveChs(const CString& param);	   // ����Ƿ�������
	static bool CheckDateTime(const CString& param);   //��������ַ����Ƿ�Ϸ� yyyy-mm-dd

	static bool CheckLongitude(const CString& param);  //��龭��
	static bool CheckLatitude(const CString& param);  //���γ��

private:
	static bool CCheckUtil::IsValidChar(char ch);
};
