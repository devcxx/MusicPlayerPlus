#pragma once
#include <iostream>

namespace cvt {
	std::string utf8ts(const std::string& utf8);
	std::wstring	s2ws(std::string s);
	std::string ws2s(std::wstring wstr);
	unsigned char tohex(const unsigned char& x);
	std::string	urlencoder(std::string& sIn);
	std::wstring utf8tws(const std::string& utf8);
	std::string s2utf8(const std::string& str);
	void replaceSubStr(std::string& strSource, const std::string& strOld, const std::string& strNew);
	/*
	* Erase First Occurrence of given  substring from main string.
	*/
	void eraseSubStr(std::string & mainStr, const std::string & toErase);

	/*
	* Erase all Occurrences of given substring from main string.
	*/
	void eraseAllSubStr(std::string & mainStr, const std::string & toErase);
}