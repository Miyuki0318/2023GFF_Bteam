/*!
@file OzawaUtility.h
@brief ユーティリティ
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#define BASECROSS __has_include("common.h") // BaseCrossのライブラリをインクルードしているかのマクロ

namespace Utility
{
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
	int GetLowerDigit(const T& value)
	{
		// float/double型以外の場合は0を返して関数を終了する
		if (typeid(value) != typeid(float) && typeid(value) != typeid(double))
		{
			return 0;
		}

		// 文字列で小数点を精査する為、文字列型の変数に変換
		wstring str = to_wstring(value);

		// 計測用カウンタ
		int integer = 0;
		int decimal = 0;
		int zeroCount = -1;
		bool errorCheck = false;

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

		// 文字列の長さ文ループ
		for (int i = integer; i < str.size(); i++)
		{
			if (str[i] == '0')
			{
				errorCheck = true;
				zeroCount++;
			}

			if (str[i] != '.' && str[i] != '0')
			{
				errorCheck = false;
				zeroCount = 0;
			}
		}

		// 小数点が見つからなかったら
		if (integer != 0)
		{
			// 小数点以下の桁数を返すカウンタを計算する
			if (errorCheck)
			{
				return decimal = static_cast<int>(str.size()) - integer - zeroCount - 1;
			}

			return decimal = static_cast<int>(str.size()) - integer - 1;
		}

		// 小数点以下を返すカウンタは0にする
		return decimal = 1;
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
	@param (digit) 小数点以下の桁数指定
	@return 範囲内で生成された乱数
	*/
	template <class T>
	T RangeRand(const T& min, const T& max, int digit)
	{
		T r = min + static_cast<T>(rand()) * static_cast<T>(max - min) / RAND_MAX;
		return static_cast<T>(basecross::bsm::floor(static_cast<float>(r), digit));
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

		return RangeRand(min, max, digit);
	}

	/*!
	@brief ディグリー角をラジアン角に変換
	@param (deg) ディグリー角
	@return ラジアン角に変換されたディグリー角
	*/
	template <class T>
	T DegToRad(const T& deg)
	{
		return deg * (3.141592654f / 180.0f);
	}

	/*!
	@brief ラジアン角をディグリー角に変換
	@param (rad) ラジアン角
	@return ディグリー角に変換されたラジアン角
	*/
	template <class T>
	T RadToDeg(const T& rad)
	{
		return rad * (180.0f / 3.141592654f);
	}

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(int value, int a, int b);
	bool GetBetween(float value, float a, float b);

	/*!
	@brief 列挙型の値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	template <class T>
	bool GetBetween(T value, T a, T b)
	{
		// 列挙型かどうかのチェック
		if (is_enum<T>())
		{
			// 列挙型なのでint型にキャストする
			int iv, ia, ib;
			iv = static_cast<int>(value);
			ia = static_cast<int>(a);
			ib = static_cast<int>(b);

			// int型でのGetBetween()を実行する
			return GetBetween(iv, ia, ib);
		}
		else
		{
#if BASECROSS

			// エラーメッセージを送る
			throw basecross::BaseException(
				L"列挙型以外は指定できません",
				L"列挙型が指定されてるか確認してください",
				L"bool GetBetween(T value, T a, T b)"
			);
#endif
			return false;
		}
	}


	/*!
	@brief enum列挙子をsize_tにキャスト
	@return キャストされたenum
	*/
	template <typename Enum>
	const size_t toSize_t(Enum index)
	{
		// 列挙型じゃなければ
		if (!is_enum<Enum>())
		{
#if BASECROSS

			// エラーメッセージを送る
			throw basecross::BaseException(
				L"enum以外をsize_tにキャストできません",
				L"enumを指定してください",
				L"toSize_t(Enum index)"
			);
#endif
			return 0;
		}

		// size_tにキャストして返す
		return static_cast<size_t>(index);
	}

	/*!
	@brief 第一引数の値に一番近い値を返す (中間値がある場合は最短で出た値が優先される)
	@param (num) 元の数
	@param (a...) 比べる数(可変長引数)
	@return numに一番近い数値
	*/
	template <class Ty1, class... Ty2>
	Ty1 GetClosest(const Ty1& num, Ty2... a)
	{
		size_t size = sizeof...(a);
		size_t elem = 0;
		Ty1 ret = (numeric_limits<Ty1>::max)();
		vector<Ty1> nums = { static_cast<Ty1>(a)... };

		for (size_t i = 0; i < size; i++)
		{
			Ty1 value = num - nums.at(i);
			if (value < 0) value *= -1;

			if (ret > value)
			{
				ret = value;
				elem = i;
			}
		}

		return nums.at(elem);
	}


#if BASECROSS

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
#define COL_BG		Col4(0.0f, 1.0f, 1.0f, 1.0f)

	 /*!
	 @brief 灰色
	 */
#define COL_GRAY	Col4(0.5f, 0.5f, 0.5f, 1.0f)

	 /*!
	 @brief デルタタイム
	 @return App::GetApp()->GetElapsedTime()
	 */
#define DELTA_TIME basecross::App::GetApp()->GetElapsedTime()

	 /*!
	 @brief ウィンドウの幅
	 @return App::GetApp()->GetGameWidth()
	 */
#define WINDOW_WIDTH static_cast<float>(basecross::App::GetApp()->GetGameWidth())

	 /*!
	 @brief ウィンドウの高さ
	 @return App::GetApp()->GetGameHeight()
	 */
#define WINDOW_HEIGHT static_cast<float>(basecross::App::GetApp()->GetGameHeight())

	 /*!
	 @brief ウィンドウのサイズ
	 @return Vec2(WINDOW_WIDTH, WINDOW_HEIGHT)
	 */
#define WINDOW_SIZE Vec2(WINDOW_WIDTH, WINDOW_HEIGHT)

	 /*!
	 @brief Thisポインタ
	 @return static_cast<void*>(ObjectInterface::shared_from_this().get())
	 */
#define ThisPtr static_cast<void*>(shared_from_this().get())

	 /*!
	 @brief 上方向Vec3
	 @return Vec3(0.0f, 1.0f, 0.0f)
	 */
#define UP_VEC		Vec3(0.0f, 1.0f, 0.0f)

	 /*!
	 @brief 下方向Vec3
	 @return Vec3(0.0f, -1.0f, 0.0f)
	 */
#define DOWN_VEC	Vec3(0.0f, -1.0f, 0.0f)

	 /*!
	 @brief 左方向Vec3
	 @return Vec3(-1.0f, 0.0f, 0.0f)
	 */
#define LEFT_VEC	Vec3(-1.0f, 0.0f, 0.0f)

	 /*!
	 @brief 右方向Vec3
	 @return Vec3(1.0f, 0.0f, 0.0f)
	 */
#define RIGHT_VEC	Vec3(1.0f, 0.0f, 0.0f)

	 /*!
	 @brief シンプルな頂点データと頂点インデックスを作成
	 @param (vertices) 頂点データ
	 @param (indices) 頂点インデックス
	 */
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertex) 頂点データと頂点インデックス構造体
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (color) 固定色
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color);

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (color) 固定色
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex, const Col4& color);

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void NumberVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number);

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertex) 頂点データと頂点インデックス構造体
	@param (number) 数字
	*/
	void NumberVerticesIndices(basecross::VertexData& vertex, const int number);

	/*!
	@brief リボン用頂点データと頂点インデックスを作成
	@param (point) 節目の座標配列
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (axisVec) 軸ベクトル
	@param (weight) 幅のサイズ
	@param (texLoop) テクスチャのループ数
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Vec3& axisVec, float weight, int texLoop);

	/*!
	@brief リボン用頂点データと頂点インデックスを作成
	@param (point) 節目の座標配列
	@param (vertex) 頂点データと頂点インデックス構造体
	@param (axisVec) 軸ベクトル
	@param (weight) 幅のサイズ
	@param (texLoop) テクスチャのループ数
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, basecross::VertexData& vertex, const Vec3& axisVec, float weight, int texLoop);

	/*!
	@brief 円型用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void CircleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int squareNum);

	/*!
	@brief 円型用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void CircleVerticesIndices(basecross::VertexData& vertex, const int squareNum);

	/*!
	@brief ワールド座標をスクリーン座標に変換(BaseCrossの座標用、それ以外はDirectX付属の関数を備考)
	@param (viewPtr) viewのshared_ptr
	@param (position) ワールド座標
	@return 変換されたスクリーン座標
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<basecross::ViewBase>& viewPtr, const Vec3& position);

	/*!
	@brief ビルボードをクォータニオンで作成し返す
	@param (Line)　カメラの注視点 - カメラの位置
	@return 作成されたクォータニオン
	*/
	Quat GetBillboardQuat(const Vec3& Line);

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(const Vec2& value, const Vec2& a, const Vec2& b);
	bool GetBetween(const Vec3& value, const Vec3& a, const Vec3& b);
#endif
}