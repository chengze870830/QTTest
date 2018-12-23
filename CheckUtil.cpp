#include "StdAfx.h"
#include "CheckUtil.h"
#include "Markup.h"

using namespace std::tr1;

CCheckUtil::CCheckUtil(void)
{
}

CCheckUtil::~CCheckUtil(void)
{
}

bool CCheckUtil::CheckIPStr(const CString& ip)
{
	USES_CONVERSION;
	if (ip.IsEmpty())
		return false;

	if (ip.GetLength() > 15)
	{
		return false;
	}

	char str[20] = {0};
#if _UNICODE
	strncpy(str, T2A(ip), 20);
#else
	strncpy(str, ip, 20);
#endif
	char a;
	int dot = 0;
	int a3, a2, a1, a0, i = 0;
    char s1[5], s2[5], s3[5], s0[5];
	a3 = a2 = a1 = a0 = -1;
	// Empty string
	if (*str == '\0') 
	{
		return true;
	}

	while (a = str[i++])
	{
		if ((a == ' ') || (a == '.') || ((a >= '0') && (a <= '9')))
		{
			if (a == '.')
			{
				dot++;
			}
		}
		else
		{
			return false;
		}
	}
	if (dot != 3)
	{
		return false;
	}
	else
	{
		sscanf(str, "%[^.].%[^.].%[^.].%[^.]", &s3, &s2, &s1, &s0);
		if (s0[0] == '0' && s0[1] == '0')
		{
			return false;
		}
		a0 = atoi(s0);
        
		if (s1[0] == '0' && s1[1] == '0')
		{
			return false;
		}
		a1 = atoi(s1);

		if (s2[0] == '0' && s2[1] == '0')
		{
			return false;
		}
		a2 = atoi(s2);

		if (s3[0] == '0' && s3[1] == '0')
		{
			return false;
		}
		a3 = atoi(s3);

		if ((a0 > 254) || (a1 > 255) || (a2 > 255) || (a3 > 255))
		{
			return false;
		}
		if ((a0 < 1) || (a1 < 0) || (a2 < 0) || (a3 < 0))
		{
			return false;
		}
	}
	return true;
}

bool CCheckUtil::CheckSubMaskStr(const CString& ip)
{
	USES_CONVERSION;
	if (ip.IsEmpty())
		return false;

	if (ip.GetLength() > 15)
	{
		return false;
	}

	char str[20] = {0};
#if _UNICODE
	strncpy(str, T2A(ip), 20);
#else
	strncpy(str, ip, 20);
#endif
	char a;
	int dot = 0;
	int a3, a2, a1, a0, i = 0;
	char s1[5], s2[5], s3[5], s0[5];
	a3 = a2 = a1 = a0 = -1;
	// Empty string
	if (*str == '\0') 
	{
		return true;
	}

	while (a = str[i++])
	{
		if ((a == ' ') || (a == '.') || ((a >= '0') && (a <= '9')))
		{
			if (a == '.')
			{
				dot++;
			}
		}
		else
		{
			return false;
		}
	}
	if (dot != 3)
	{
		return false;
	}
	else
	{
		sscanf(str, "%[^.].%[^.].%[^.].%[^.]", &s3, &s2, &s1, &s0);
		if (s0[0] == '0' && s0[1] == '0')
		{
			return false;
		}
		a0 = atoi(s0);

		if (s1[0] == '0' && s1[1] == '0')
		{
			return false;
		}
		a1 = atoi(s1);

		if (s2[0] == '0' && s2[1] == '0')
		{
			return false;
		}
		a2 = atoi(s2);

		if (s3[0] == '0' && s3[1] == '0')
		{
			return false;
		}
		a3 = atoi(s3);

		if ((a0 > 255) || (a1 > 255) || (a2 > 255) || (a3 > 255))
		{
			return false;
		}
		if ((a0 < 0) || (a1 < 0) || (a2 < 0) || (a3 < 0))
		{
			return false;
		}
	}
	return true;
}

bool CCheckUtil::SplitStr(const CString strSrc, const CString division, std::list<CString>& strList)
{
	if (strSrc.IsEmpty())
	{
		return false;
	}

	if (division.IsEmpty())
	{
		return false;
	}

	strList.empty();

	int nPos = 0;
	CString strTempSrc = strSrc;
	CString strTemp;
	while(-1 != nPos)
	{
		nPos = strTempSrc.Find(division);
		if (-1 != nPos)
		{
			strTemp = strTempSrc.Left(nPos);
			strList.push_back(strTemp);
			strTempSrc = strTempSrc.Right(strTempSrc.GetLength()-nPos-1);
		}
		else
		{
			return true;
		}
	}
	strList.push_back(strTempSrc);
	return true;
}

bool CCheckUtil::CheckPort(int nPort)
{
	if (nPort <= 0 || nPort > 65535)
		return false;

	return true;
}

bool CCheckUtil::CheckPort(const CString& strPort)
{
	if (strPort.IsEmpty())
		return false;

	if(!CheckNum(strPort))
		return FALSE;

	int nPort = _ttoi(strPort);
	return CheckPort(nPort);
}

bool CCheckUtil::CheckMacStr(const CString& strMac)
{
	if (strMac.IsEmpty())
		return false;

	if (strMac.GetLength() != 17)
		return false;

	int nIndex = 0;
	int nValue = 2;
	while(nIndex < 17)
	{
		if (nIndex > 0 && nIndex % nValue == 0 )
		{
			if (strMac[nIndex] != ':')
				return false;

			nValue += 3;
			nIndex++;
			continue;
		}

		char c = strMac[nIndex];
		if (!((strMac[nIndex] >= 48 && strMac[nIndex] <= 57) || (strMac[nIndex] >= 'a' && strMac[nIndex] >= 'f')))
		{
			return false;
		}

		nIndex++;
	}

	return true;
}

bool CCheckUtil::CheckMail(const CString& strEmail)
{
	if( strEmail.GetLength() < 5 ) 
		return false;

	char ch = strEmail[0];
	if( ((ch>=97) && (ch<=122)) || ((ch>=65) && (ch<=90)) )
	{
		int atCount =0;
		int atPos = 0;
		int dotCount = 0;
		for(int i=1;i<strEmail.GetLength();i++) 
		{
			ch = strEmail[i];
			if(IsValidChar(ch))
			{
				if(ch==64) 
				{
					atCount ++;
					atPos = i;
				}
				else if( (atCount>0) && (ch==46) )
					dotCount ++;
			}
			else
				return false;
		}

		if( ch == 46 )
			return false;
		
		if( (atCount!=1) || (dotCount<1) || (dotCount>3) )
			return false;
		
		int x,y;
		x = strEmail.Find(_T("@."));
		y = strEmail.Find(_T(".@"));

		if( x>0 || y>0 )
			return false;

		return true;
	}

	return false;
}

bool CCheckUtil::IsValidChar(char ch)
{
	if( (ch>=97) && (ch<=122) ) 
		return true;

	if( (ch>=65) && (ch<=90) ) 
		return true;

	if((ch>=48) && (ch<=57)) 
		return true;

	if( ch==95 || ch==45 || ch==46 || ch==64 ) 
		return true;

	return false;
}

bool CCheckUtil::CheckNum(const CString& strNum)
{
	int nLen = strNum.GetLength();
	for(int i = 0; i < nLen; i++)
	{
		if(strNum.GetAt(i) >= '0' && strNum.GetAt(i) <= '9')
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CCheckUtil::CheckPositiveNum(const CString& strNum)
{
	return CCheckUtil::RegexMatch(strNum, _T("^[1-9][0-9]*$"));
}

bool CCheckUtil::CheckGuid(const CString& strGuid)
{
	int nLen = strGuid.GetLength();
	for(int i = 0 ; i < nLen ; i++)
	{
		if(strGuid.GetAt(i) >= '0' && strGuid.GetAt(i) <= '9')
		{
			continue;
		}
		else if(strGuid.GetAt(i) >= 'a' && strGuid.GetAt(i) <= 'z')
		{
			continue;
		}
		else if(strGuid.GetAt(i) >= 'A' && strGuid.GetAt(i) <= 'Z')
		{
			continue;
		}
		else if (strGuid.GetAt(i)=='-')
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CCheckUtil::CheckIPV6(const CString& strIPv6)
{
	int nLen = strIPv6.GetLength();
	int nCount = 0;
	for(int i = 0 ; i < nLen ; i++)
	{
		if(strIPv6.GetAt(i) == ':')
		{
			nCount ++;
			continue;
		}
		else if(strIPv6.GetAt(i) >= '0' && strIPv6.GetAt(i) <= '9')
		{
			continue;
		}
		else if(strIPv6.GetAt(i) >= 'a' && strIPv6.GetAt(i) <= 'z')
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	bool bRet = false;
	nCount == 7? bRet = true : false;
	return bRet;
}

bool CCheckUtil::CheckillegalStr(const CString& str)
{
	if (str.IsEmpty())
		return false;

	for (int nIndex=0; nIndex<str.GetLength(); ++nIndex)
	{
		int n = str[nIndex];
		if (!(str[nIndex]>=33 && str[nIndex]<=122))
			return false;
	}

	return true;
}

bool CCheckUtil::RegexMatch( LPCTSTR lpctSrc, LPCTSTR lpctMatch )
{
#if defined(UNICODE) || defined(_UNICODE)
	wcmatch match;
	wregex _regex(lpctMatch, std::tr1::regex_constants::icase);
#else
	cmatch match;
	regex _regex(lpctMatch, std::tr1::regex_constants::icase);
#endif
	return ::regex_match(lpctSrc, /*match, */_regex);
}

bool CCheckUtil::CheckId( const CString& param )
{
	bool bRet=CCheckUtil::RegexMatch(param,_T("(\\d{6})(((1|2)\\d{3})((0[1-9])|(1[0-2]))((0[1-9])|(1[0-9])|(2[0-9])|(3[0-1])))(\\d{3}(\\d|x))"));
	if(!bRet)
		return false;
	CString strDate=param.Mid(6,8);
	int year=_ttoi(strDate.Mid(0,4).GetBuffer(0));
	CTime now=CTime::GetCurrentTime();
	if(year>now.GetYear())
		return false;
	
	
	return true;
}

bool CCheckUtil::CheckMobileNum( const CString& param )
{
	bool bRet=CCheckUtil::RegexMatch(param,_T("^1[3|4|5|6|7|8]\\d{9}$"));
	//bool bRet = CCheckUtil::RegexMatch(param,_T("^(13[0-9])|(15[^4,\\d])|(18[0,5-9]))\\d{8}$"));
	return bRet;
}

bool CCheckUtil::CheckTelNum( const CString& param )
{
	//bool bRet=CCheckUtil::RegexMatch(param,_T("\\+?\\d{2,4}(-?\\d+)(-?\\d+)(-?\\d+)"));
	bool bRet = CCheckUtil::RegexMatch(param,_T("^(0\\d{2,3}-?|\\(0\\d{2,3}\\))?[1-9]\\d{4,7}(-\\d{1,8})?$"));
	
	return bRet;
}

bool CCheckUtil::CheckCarNum( const CString& param )
{
	bool bRet=CCheckUtil::RegexMatch(param,_T("^[\u4e00-\u9fa5]{1}[A-Z]{1}[A-Z_0-9]{5}$"));
	return bRet;
}

bool CCheckUtil::CheckCardNo(const CString& param)
{
	bool bRet=CCheckUtil::RegexMatch(param,_T("^\\d{1,16}$"));
	return bRet;
}

bool CCheckUtil::CheckMoney(const CString& param)
{
	bool bRet = CCheckUtil::RegexMatch(param, _T("^(-?\\d+)(\.\\d+)?$"));
	return bRet;
}
bool CCheckUtil::CheckHaveChs(const CString& param)
{
	bool bRet = CCheckUtil::RegexMatch(param,_T("[^\u4e00-\u9fa5]*"));
	return !bRet;
}

bool CCheckUtil::CheckDateTime(const CString& param)
{
	//2012-01-01
	bool bRet = CCheckUtil::RegexMatch(param, _T("(([0-9]{3}[1-9]|[0-9]{2}[1-9][0-9]{1}|[0-9]{1}[1-9][0-9]{2}|[1-9][0-9]{3})-(((0[13578]|1[02])-(0[1-9]|[12][0-9]|3[01]))|((0[469]|11)-(0[1-9]|[12][0-9]|30))|(02-(0[1-9]|[1][0-9]|2[0-8]))))|((([0-9]{2})(0[48]|[2468][048]|[13579][26])|((0[48]|[2468][048]|[3579][26])00))-02-29)"));
	return bRet;
}

bool CCheckUtil::CheckLongitude(const CString& param)
{
	bool bRet = CCheckUtil::RegexMatch(param, _T("^[-|+]?(((\\d|([1-9]\\d)|(1[0-7]\\d))(\\.\\d{1,6})?)|(180(\\.0{1,6})?))$"));
	return bRet;
}

bool CCheckUtil::CheckLatitude(const CString& param)
{
	bool bRet = CCheckUtil::RegexMatch(param, _T("^[-|+]?(((\\d|([1-8]\\d))(\\.\\d{1,6})?)|(90(\\.0{1,6})?))$"));
	return bRet;
}
