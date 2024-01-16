/*!
@file DebugLog.cpp
@brief ƒfƒoƒbƒO—p•¶š—ñ•\¦
@author ¬àV”‹M
*/

#include "stdafx.h"
#include "Project.h"

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”(ˆø”–³‚µ‚Í‰üs)
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss()
{
	wstringstream wstr;
	wstr << std::endl;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param intŒ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const int str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param intŒ^”z—ñ‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const int str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param floatŒ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const float str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param floatŒ^”z—ñ‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const float str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param doubleŒ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const double str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param doubleŒ^”z—ñ‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const double str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param charŒ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const char str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param charŒ^”z—ñ‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const char str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param wchar_tŒ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const wchar_t str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param wchar_tŒ^”z—ñ‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const wchar_t str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param boolŒ^‚ğ•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const bool str)
{
	wstringstream wstr;
	wstr << (str == true ? L"true" : L"false");
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param wstringŒ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const wstring& str)
{
	return str;
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param size_tŒ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const size_t& str)
{
	wostringstream ostr;
	ostr << std::dec << str;
	return ostr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param uint16_tŒ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const uint16_t& str)
{
	wostringstream ostr;
	ostr << std::dec << str;
	return ostr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param UINTŒ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const UINT& str)
{
	wostringstream ostr;
	ostr << std::dec << str;
	return ostr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param Flt2Œ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const Flt2& str)
{
	wstringstream wstr;
	wstr << str.x << L", " << str.y;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param Flt3Œ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const Flt3& str)
{
	wstringstream wstr;
	wstr << str.x << L", " << str.y << L", " << str.z;
	return wstr.str();
}

/*!
@brief ’l‚ğwstringŒ^‚É•ÏŠ·‚µ•Ô‚·ŠÖ”
@param Flt4Œ^‚Ì•ÏŠ·‚·‚é’l
@return wstringŒ^‚É•ÏŠ·‚³‚ê‚½’l
*/
wstring Debug::GetWss(const Flt4& str)
{
	wstringstream wstr;
	wstr << str.x << L", " << str.y << L", " << str.z << L", " << str.w;
	return wstr.str();
}