/*!
@file OzawaUtility.h
@brief ユーティリティ
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

using namespace basecross;

namespace Utility
{
/*! 
@brief 無色透明
 */
#define COL_NONE	Col4(0.0f)

/*!
@brief 有色透明
*/
#define COL_ALPHA	Col4(1.0f,1.0f,1.0f,0.0f)

/*!
@brief 白色
*/
#define COL_WHITE	Col4(1.0f)

/*!
@brief 黒色
*/
#define COL_BLACK	Col4(0.0f,0.0f,0.0f,1.0f)

/*!
@brief 赤色
*/
#define COL_RED		Col4(1.0f,0.0f,0.0f,1.0f)

/*!
@brief 緑色
*/
#define COL_GREAN	Col4(0.0f, 1.0f, 0.0f, 1.0f)

/*!
@brief 青色
*/
#define COL_BLUE	Col4(0.0f,0.0f,1.0f,1.0f)

/*!
@brief 黄色
*/
#define COL_YELOW	Col4(1.0f, 1.0f, 0.0f, 1.0f)

/*!
@brief 青緑色
*/
#define COL_BG	Col4(0.0f, 1.0f, 1.0f, 1.0f)

/*!
@brief 灰色
*/
#define COL_GRAY	Col4(0.5f, 0.5f, 0.5f, 1.0f)

	/*!
	@brief 線形補間を行う関数
	@param (start) 線形補間の始点の値
	@param (end) 線形補間の終点の値
	@param (time) 線形補間の割合
	@return 割合に応じて補間された値
	*/
	template <class T>
	inline T Lerp(const T& start, const T& end, float time)
	{
		time = time < 0.0f ? 0.0f : time;
		time = time > 1.0f ? 1.0f : time;
		return start + (end - start) * time;
	}

	/*!
	@brief 範囲を指定したサインカーブを生成
	@param (time) 経過時間
	@param (min) カーブの下限値
	@param (max) カーブの上限値
	@return 範囲内で生成されたサインカーブ
	*/
	template <class T>
	inline T SinCurve(const float& time, const T& min, const T& max)
	{
		T length = max - min;
		T helfLength = length / 2.0f;
		T curve = sin(time) * helfLength + helfLength;
		return curve + min;
	}

	/*!
	@brief 小数点以下の桁数を返す関数
	@param (value)小数の桁数を調べたい値
	@return 小数点以下の桁数をint型で返す
	*/
	template <class T>
	inline int GetLowerDigit(const T& value)
	{
		// float/double型以外の場合は0を返して関数を終了する
		if (typeid(value) != typeid(float) && typeid(value) != typeid(double))
		{
			return 0;
		}

		// 文字列で小数点を精査する為、文字列型の変数に変換
		wstringstream stream;
		stream << value;
		wstring str = stream.str();

		// 計測用カウンタ
		int integer = 0;
		int decimal = 0;

		// 文字列の長さ文ループ
		for (int i = 0; i < str.size(); i++)
		{
			// 小数点が見つかったら
			if (str[i] == '.')
			{
				// 配列の要素数を保持してfor文を停止
				integer = i;
				break;
			}
		}

		// 小数点が見つからなかったら
		if (integer == 0)
		{
			// 小数点以下を返すカウンタは0にする
			decimal = 0;
		}
		else
		{
			// 小数点以下の桁数を返すカウンタを計算する
			decimal = static_cast<int>(str.size()) - integer - 1;
		}

		// 小数点以下の桁数を返す
		return decimal;
	}

	/*!
	@brief 整数の桁数を返す関数
	@param (value)整数の桁数を調べたい値
	@return 整数のの桁数をint型で返す
	*/
	template <class T>
	inline int GetUpperDigit(const T& value)
	{
		// float/double型以外の場合は0を返して関数を終了する
		if (typeid(value) != typeid(float) && typeid(value) != typeid(double) && typeid(value) != typeid(int))
		{
			return 0;
		}

		// 文字列で小数点を精査する為、文字列型の変数に変換
		wstringstream stream;
		stream << value;
		wstring str = stream.str();

		// 計測用カウンタ
		int integer = 0;

		// 文字列の長さ文ループ
		for (int i = 0; i < str.size(); i++)
		{
			// 小数点が見つかったらfor文を停止
			if (str[i] == '.' || (integer == 0 && str[i] == '0')) break;
			integer++;
		}

		// 小数点以上の桁数を返す
		return integer;
	}

	/*!
	@brief 範囲を指定した乱数を生成(小数点以下も対応)
	@param (max) 乱数の上限値
	@param (min) 乱数の下限値
	@return 範囲内で生成された乱数
	*/
	template <class T>
	T RangeRand(const T& max, const T& min)
	{
		// 浮動小数型の場合は小数の桁数を計算する
		int maxDigit = GetLowerDigit(max);
		int minDigit = GetLowerDigit(min);
		int digit = maxDigit > minDigit ? maxDigit : minDigit;

		// 小数を求める為に扱う桁数を10のべき乗で計算
		T divisionDigit = static_cast<T>(std::pow(10.0f, digit));

		// 小数を一度整数に戻す
		int maxNum = static_cast<int>(max) * static_cast<int>(divisionDigit);
		int minNum = static_cast<int>(min) * static_cast<int>(divisionDigit);

		// 整数を小数に戻す(rand関数が浮動小数を扱えない為)
		return static_cast<T>(static_cast<float>((rand() % (maxNum - minNum + 1)) + minNum) / static_cast<float>(divisionDigit));
	}

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	*/
	void SimpleVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertex) 頂点データと頂点インデックス構造体
	*/
	void SimpleVerticesIndices(VertexData& vertex);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (color) 固定色
	*/
	void SimpleVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (color) 固定色
	*/
	void SimpleVerticesIndices(VertexData& vertex, const Col4& color);

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void NumberVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number);

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertex) 頂点データと頂点インデックス構造体
	@param (number) 数字
	*/
	void NumberVerticesIndices(VertexData& vertex, const int number);

	/*!
	@brief ワールド座標をスクリーン座標に変換(BaseCrossの座標用、それ以外はDirectX付属の関数を備考)
	@param (viewPtr) viewのshared_ptr
	@param (position) ワールド座標
	@return 変換されたスクリーン座標
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<ViewBase>& viewPtr, const Vec3& position);
	
	/*!
	@brief ディグリー角をラジアン角に変換
	@param (deg) ディグリー角
	@return ラジアン角に変換されたディグリー角
	*/
	template<class T>
	T DegToRad(const T& deg)
	{
		return deg * (XM_PI / 180.0f);
	}
	float DegToRad(const float& deg);
	Vec2 DegToRad(const Vec2& deg);
	Vec3 DegToRad(const Vec3& deg);
	Vec4 DegToRad(const Vec4& deg);


	/*!
	@brief ラジアン角をディグリー角に変換
	@param (rad) ラジアン角
	@return ディグリー角に変換されたラジアン角
	*/
	template<class T>
	T RadToDeg(const T& deg)
	{
		return rad * (180.0f / XM_PI);
	}
	float RadToDeg(const float& rad);
	Vec2 RadToDeg(const Vec2& rad);
	Vec3 RadToDeg(const Vec3& rad);
	Vec4 RadToDeg(const Vec4& rad);

	/*!
	@brief ビルボードをクォータニオンで作成し返す
	@param (Line)　カメラの注視点 - カメラの位置
	@return 作成されたクォータニオン
	*/
	Quat Billboard(const Vec3& Line);
}