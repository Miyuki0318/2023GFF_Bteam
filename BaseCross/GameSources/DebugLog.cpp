/*!
@file DebugLog.cpp
@brief デバッグ用文字列表示
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

/*!
@brief 値をwstring型に変換し返す関数(引数無しは改行)
@return wstring型に変換された値
*/
wstring Debug::GetWss()
{
	wstringstream wstr;
	wstr << std::endl;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param int型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const int str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param int型配列の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const int str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param float型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const float str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param float型配列の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const float str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param double型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const double str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param double型配列の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const double str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param char型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const char str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param char型配列の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const char str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param wchar_t型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const wchar_t str)
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param wchar_t型配列の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const wchar_t str[])
{
	wstringstream wstr;
	wstr << str;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param bool型を変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const bool str)
{
	wstringstream wstr;
	wstr << (str == true ? L"true" : L"false");
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param wstring型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const wstring& str)
{
	return str;
}

/*!
@brief 値をwstring型に変換し返す関数
@param size_t型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const size_t& str)
{
	wostringstream ostr;
	ostr << std::dec << str;
	return ostr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param uint16_t型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const uint16_t& str)
{
	wostringstream ostr;
	ostr << std::dec << str;
	return ostr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param UINT型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const UINT& str)
{
	wostringstream ostr;
	ostr << std::dec << str;
	return ostr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param Flt2型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const Flt2& str)
{
	wstringstream wstr;
	wstr << str.x << L", " << str.y;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param Flt3型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const Flt3& str)
{
	wstringstream wstr;
	wstr << str.x << L", " << str.y << L", " << str.z;
	return wstr.str();
}

/*!
@brief 値をwstring型に変換し返す関数
@param Flt4型の変換する値
@return wstring型に変換された値
*/
wstring Debug::GetWss(const Flt4& str)
{
	wstringstream wstr;
	wstr << str.x << L", " << str.y << L", " << str.z << L", " << str.w;
	return wstr.str();
}