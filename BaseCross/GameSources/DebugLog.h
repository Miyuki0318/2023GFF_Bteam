/*!
@file DebugLog.h
@brief デバッグ用文字列表示
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

// Debug::Log用クラス(Unityの様に記述し、画面左上に文字列を表示するクラス)
class Debug
{
private:

	/*!
	@brief コンストラクタ
	*/
	Debug();

	/*!
	@brief デストラクタ
	*/
	~Debug() {}

	/*!
	@brief 値をwstring型に変換し返す関数
	@param template型の変換する値
	@return wstring型に変換された値
	*/
	static wstring GetWss();
	static wstring GetWss(const bool str);
	static wstring GetWss(const int str);
	static wstring GetWss(const int str[]);
	static wstring GetWss(const float str);
	static wstring GetWss(const float str[]);
	static wstring GetWss(const double str);
	static wstring GetWss(const double str[]);
	static wstring GetWss(const char str);
	static wstring GetWss(const char str[]);
	static wstring GetWss(const wchar_t str);
	static wstring GetWss(const wchar_t str[]);
	static wstring GetWss(const wstring& str);
	static wstring GetWss(const size_t& str);
	static wstring GetWss(const uint16_t& str);
	static wstring GetWss(const UINT& str);
	static wstring GetWss(const Flt2& str);
	static wstring GetWss(const Flt3& str);
	static wstring GetWss(const Flt4& str);

	/*!
	@brief 値をwstringstream型に変換し返す関数
	@param template型の変換する値
	@return wstringstream型に変換された値
	*/
	template <class T>
	static wstringstream SetWss(const T& str)
	{
		wstringstream wstr;
		wstr << Debug::GetWss(str);
		return wstr;
	}

	/*!
	@brief 値をwstringstream型に変換し返す関数
	@param template型の変換する値二つ
	@return wstringstream型に変換された値
	*/
	template <class T, class U>
	static wstringstream SetWss(const T& str1, const U& str2)
	{
		wstringstream wstr;
		wstr << Debug::GetWss(str1) << Debug::GetWss(str2);
		return wstr;
	}

	/*!
	@brief 値をwstringstream型に変換し返す関数
	@param template型の変換する値三つ
	@return wstringstream型に変換された値
	*/
	template <class T, class U, class V>
	static wstringstream SetWss(const T& str1, const U& str2, const V& str3)
	{
		wstringstream wstr;
		wstr << Debug::GetWss(str1) << Debug::GetWss(str2) << Debug::GetWss(str3);
		return wstr;
	}

public:

	/*!
	@brief 値をwstringstream型に変換し、左上に文字列を表示する
	@param template型の変換する値
	*/
	template <class T>
	static void Log(const T& param)
	{
		wstringstream wss = Debug::SetWss(param);
		wss << std::endl;
		auto scene = basecross::App::GetApp()->GetScene<basecross::Scene>();
		auto dstr = scene->GetDebugString();
		scene->SetDebugString(dstr + wss.str());
	}

	/*!
	@brief 値をwstringstream型に変換し、左上に文字列を表示する
	@param template型の変換する値二つ
	*/
	template <class T, class U>
	static void Log(const T& param1, const U& param2)
	{
		wstringstream wss = Debug::SetWss(param1, param2);
		wss << std::endl;
		auto scene = basecross::App::GetApp()->GetScene<basecross::Scene>();
		auto dstr = scene->GetDebugString();
		scene->SetDebugString(dstr + wss.str());
	}

	/*!
	@brief 値をwstringstream型に変換し、左上に文字列を表示する
	@param template型の変換する値三つ
	*/
	template <class T, class U, class V>
	static void Log(const T& param1, const U& param2, const V& param3)
	{
		wstringstream wss = Debug::SetWss(param1, param2, param3);
		wss << std::endl;
		auto scene = basecross::App::GetApp()->GetScene<basecross::Scene>();
		auto dstr = scene->GetDebugString();
		scene->SetDebugString(dstr + wss.str());
	}
};