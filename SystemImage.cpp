#include "StdAfx.h"
#include "SystemImage.h"
#include "Markup.h"

IMPLEMENT_SINGTON(CSystemImage)
CSystemImage::CSystemImage(void)
{
	LoadSystemImage();
}

CSystemImage::~CSystemImage(void)
{
}

BOOL CSystemImage::LoadSystemImage()
{
	TCHAR szAppPath[MAX_PATH] = {_T("")};
	TCHAR szCfgFile[MAX_PATH] = {_T("")};
	CFileUtil::GetAppPath(szAppPath,MAX_PATH);

	CString strXMLPath = szAppPath;
	strXMLPath += CONFIG_SYSTEM_IMAGE_FILE;

	if(!CFileUtil::IsFileOrDirExist(strXMLPath))
	{
		ATXTRACE1(LOG_LEVEL_ERROR,_T("Did not find system_image file[%s]."),szCfgFile);
		return FALSE;
	}

	try
	{
		CMarkup objDoc;
		if (!objDoc.Load(strXMLPath))
		{
			ATXTRACE1(LOG_LEVEL_ERROR,_T("Load system_image file[%s] failed."),strXMLPath);
			return FALSE;
		}

		if (!objDoc.FindElem(_T("resources")))
		{
			ATXTRACE1(LOG_LEVEL_ERROR,_T("Invalid system images file[%s][no imges element]."),strXMLPath);
			return FALSE;
		}

		while (objDoc.FindChildElem(TEXT("resource")))
		{
			CString strId = objDoc.GetChildAttrib(TEXT("id"));
			CString desription = objDoc.GetChildAttrib(TEXT("description"));
			CString strImgePath = objDoc.GetChildData();

			m_ImgMapTable.SetAt(strId,strImgePath);
		}
	}
	catch (...)
	{
		ATXTRACE1(LOG_LEVEL_FATAL,_T("Invalid system images file[%s] catch exception."),strXMLPath);
		return FALSE;
	}

	return TRUE;
}

CString CSystemImage::GetImagePath(CString strImgId)
{
	CString strPath;
	if (m_ImgMapTable.Lookup(strImgId,strPath))
	{
		strPath = m_strResoureceDir +  strPath;
	}
	return strPath;
}

CString CSystemImage::GetRelativePath(CString strImgId)
{
	if (strImgId == IMG_ID_CONFIG_CLIENT_SKINFOLDER_CHINESE)
	{
		int ncxFull = GetSystemMetrics(SM_CXSCREEN);
		int ncyFull = GetSystemMetrics(SM_CYSCREEN);
		if (ncxFull >= 1920)
		{
			strImgId = strImgId + _T("_1920");
		}
		else
		{
			strImgId = strImgId + _T("_1366");
		}
	}
	CString strPath;
	m_ImgMapTable.Lookup(strImgId,strPath);
	return strPath;
}

void CSystemImage::SelectResourceDir( CString strImgId )
{
	m_strResoureceDir = GetSkinFileDir(strImgId);
}

CString CSystemImage::GetSkinFileDir( CString strImgId )
{
	TCHAR szAppPath[MAX_PATH] = {_T("")};
	CFileUtil::GetAppPath(szAppPath,MAX_PATH);
	CString strSkinDir = szAppPath;
	strSkinDir += GetRelativePath(strImgId) + _T("\\");
	return strSkinDir;
}

