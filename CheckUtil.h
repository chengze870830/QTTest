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
	static bool CheckNum(const CString& strNum);			//	判断是否为数字
	static bool CheckPositiveNum(const CString& strNum);    //  判断是否为正整数
	static bool CheckGuid(const CString& strGuid);			//	判断是否符合GUID格式
	static bool CheckIPV6(const CString& strIPv6);
	static bool CheckillegalStr(const CString& str);		//	判断是否有非法字符
	static bool RegexMatch(LPCTSTR lpctSrc, LPCTSTR lpctMatch);

	static bool CheckId(const CString& param);         // 检测身份证号是否合法
	static bool CheckMobileNum(const CString& param);  // 检测手机号码是否合法
	static bool CheckTelNum(const CString& param);     // 检测座机号码是否合法
	static bool CheckCarNum(const CString& param);     // 检测车牌号码是否合法
	static bool CheckCardNo(const CString& param);	   // 检测门禁卡号是否合法
	static bool CheckMoney(const CString& param);	   // 检测金额是否合法
	static bool CheckHaveChs(const CString& param);	   // 检测是否含有中文
	static bool CheckDateTime(const CString& param);   //检查日期字符串是否合法 yyyy-mm-dd

	static bool CheckLongitude(const CString& param);  //检查经度
	static bool CheckLatitude(const CString& param);  //检查纬度

private:
	static bool CCheckUtil::IsValidChar(char ch);
};
