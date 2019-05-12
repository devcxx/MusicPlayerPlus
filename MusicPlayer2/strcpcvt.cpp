#include "stdafx.h"
#include "strcpcvt.h"

std::string cvt::utf8ts(const std::string& utf8)
{
	// convert an utf8 string to widechar

	__int32 nLen = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
	wchar_t* lpszW = NULL;
	lpszW = new wchar_t[nLen];
	__int32 nRtn = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), lpszW, nLen);
	if (nRtn != nLen)
	{
		delete[] lpszW;
		return "";
	}

	// convert widechar string to multibyte
	__int32 mbLen = ::WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);
	if (mbLen <= 0)
	{
		return "";
	}

	std::string _str;
	_str.resize(mbLen);
	nRtn = ::WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, (LPSTR)_str.c_str(), mbLen, NULL, NULL);
	if (nRtn != mbLen)
	{
		delete[] lpszW;
		return "";
	}

	delete[] lpszW;
	return _str;
}

std::wstring cvt::s2ws(std::string s)
{
	std::wstring ws;
#ifdef _MSC_VER
	int iLen = ::MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
	if (iLen > 0)
	{
		wchar_t* pwszDst = new wchar_t[iLen];
		if (pwszDst)
		{
			::MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, pwszDst, iLen);
			pwszDst[iLen - 1] = 0;
			ws = pwszDst;
			delete[]pwszDst;
		}
	}
#else
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs");
	size_t _Dsize = 2 * (wcslen(wstr) + 1);
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, wstr, _Dsize);
	ws = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, curLocale.c_str());
#endif
	return (ws);
}

std::string cvt::ws2s(std::wstring wstr)
{
	std::string str;
#ifdef _MSC_VER
	int iLen = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (iLen > 0)
	{
		char* pszDst = new char[iLen];
		if (pszDst)
		{
			::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pszDst, iLen, NULL, NULL);
			pszDst[iLen - 1] = 0;
			str = pszDst;
			delete[] pszDst;
		}
	}
#else
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs");
	size_t _Dsize = 2 * (wcslen(wstr) + 1);
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, wstr, _Dsize);
	str = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, curLocale.c_str());
#endif
	return (str);
}

unsigned char cvt::tohex(const unsigned char& x)
{
	return x > 9 ? x + 55 : x + 48;
}

std::string cvt::urlencoder(std::string& sIn)
{
	std::string sOut;
	for (unsigned int ix = 0; ix < sIn.size(); ix++)
	{
		unsigned char buf[4];
		memset(buf, 0, 4);
		if (isalnum((unsigned char)sIn[ix]))
		{
			buf[0] = sIn[ix];
		}
		else if (isspace((unsigned char)sIn[ix]))
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = tohex((unsigned char)sIn[ix] >> 4);
			buf[2] = tohex((unsigned char)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
}

std::wstring cvt::utf8tws(const std::string& utf8)
{
	// convert an utf8 string to widechar
	__int32 nLen = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
	std::wstring _str;
	_str.resize(nLen);
	__int32 nRtn = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), (LPWSTR)_str.c_str(), nLen);
	return _str;
}

std::string cvt::s2utf8(const std::string& str)
{
	std::string _utf8(str);
	__int32 nLen = ::MultiByteToWideChar(CP_ACP, 0, _utf8.c_str(), _utf8.size(), NULL, 0);
	wchar_t* lpszW = NULL;
	lpszW = new wchar_t[nLen];
	__int32 nRtn = ::MultiByteToWideChar(CP_ACP, 0, _utf8.c_str(), _utf8.size(), lpszW, nLen);
	if (nRtn != nLen)
	{
		delete[] lpszW;
		return "";
	}
	// convert widechar string to utf-8
	__int32 utf8Len = ::WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);
	if (utf8Len <= 0)
		return "";
	_utf8.resize(utf8Len);
	nRtn = ::WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, (LPSTR)_utf8.c_str(), utf8Len, NULL, NULL);
	delete[] lpszW;
	return _utf8;
}

void cvt::replaceSubStr(std::string& strSource, const std::string& strOld, const std::string& strNew)
{
	int nPos = 0;
	while ((nPos = strSource.find(strOld, nPos)) != strSource.npos)
	{
		strSource.replace(nPos, strOld.length(), strNew);
		nPos += strNew.length();
	}
}

void cvt::eraseSubStr(std::string & mainStr, const std::string & toErase)
{
	// Search for the substring in string
	size_t pos = mainStr.find(toErase);
	if (pos != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}

void cvt::eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
	size_t pos = std::string::npos;

	// Search for the substring in string in a loop untill nothing is found
	while ((pos = mainStr.find(toErase)) != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}

