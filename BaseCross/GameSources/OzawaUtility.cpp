/*!
@file OzawaUtility.cpp
@brief ユーティリティ
@author 小澤博貴
*/

#include "stdafx.h"
#include "OzawaUtility.h"

namespace Utility
{
	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(int value, int a, int b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(float value, float a, float b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

#if BASECROSS

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices)
	{
		// 半分のサイズとデフォルトの色
		const float HALF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// 頂点データ
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(1.0f, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(1.0f, 1.0f)},
		};

		// 頂点の並び順
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex)
	{
		// 半分のサイズとデフォルトの色
		const float HALF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// 頂点データ
		vertex.vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(1.0f, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(1.0f, 1.0f)},
		};

		// 頂点の並び順
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (color) 固定色
	*/
	void SimpleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color)
	{
		// 半分のサイズとデフォルトの色
		const float HELF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = color;

		// 頂点データ
		vertices = {
			{ Vec3(-HELF_SIZE, HELF_SIZE, 0), DEFAULT_COLOR, Vec2(0.0f, 0.0f)},
			{ Vec3(HELF_SIZE, HELF_SIZE, 0), DEFAULT_COLOR , Vec2(1.0f, 0.0f)},
			{ Vec3(-HELF_SIZE, -HELF_SIZE, 0), DEFAULT_COLOR, Vec2(0.0f, 1.0f)},
			{ Vec3(HELF_SIZE, -HELF_SIZE, 0), DEFAULT_COLOR, Vec2(1.0f, 1.0f)},
		};

		// 頂点の並び順
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	*/
	void SimpleVerticesIndices(basecross::VertexData& vertex, const Col4& color)
	{
		// 半分のサイズとデフォルトの色
		const float HALF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = color;

		// 頂点データ
		vertex.vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(1.0f, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(0.0f, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(1.0f, 1.0f)},
		};

		// 頂点の並び順
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void NumberVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number)
	{
		// 半分のサイズとデフォルトの色
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// 頂点データ
		vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// 頂点の並び順
		indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief 数字用頂点データと頂点インデックスを作成
	@param (vertex) 頂点データと頂点インデックス構造体
	@param (number) 数字
	*/
	void NumberVerticesIndices(basecross::VertexData& vertex, const int number)
	{
		// 半分のサイズとデフォルトの色
		const float HALF_SIZE = 0.5f;
		const float UV_LEFT = 1.0f / 10.0f * number;
		const float UV_RIGHT = 1.0f / 10.0f * (number + 1);
		const Col4 DEFAULT_COLOR = Col4(1.0f);

		// 頂点データ
		vertex.vertices = {
			{ Vec3(-HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_LEFT, 0.0f)},
			{ Vec3(HALF_SIZE, HALF_SIZE, 0.0f), DEFAULT_COLOR , Vec2(UV_LEFT, 0.0f)},
			{ Vec3(-HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
			{ Vec3(HALF_SIZE, -HALF_SIZE, 0.0f), DEFAULT_COLOR, Vec2(UV_RIGHT, 1.0f)},
		};

		// 頂点の並び順
		vertex.indices = {
			0, 1, 2,
			1, 3, 2
		};
	}

	/*!
	@brief リボン型頂点データと頂点インデックスを作成
	@param (point) 節目の座標配列
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (axisVec) 直行軸ベクトル
	@param (weight) 幅
	@param (texLoop) テクスチャループ
	@return 生成出来たかの真偽
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Vec3& axisVec, float weight, int texLoop)
	{
		// 節目の数が0なら終了
		const int& size = static_cast<int>(point.size()); 
		if (size == 0) return false;

		vertices.clear(); // 頂点座標の初期化

		// 節目の数ループ
		for (int i = 0; i < size; i++)
		{
			// テクスチャをループするか
			const int loop = texLoop > 0 ? texLoop : 1;

			int front = (i - 1) < 0 ? size - 1 : i - 1; // 手前
			int rear = (i + 1) % size; // 後方

			// 方向から外積を求める
			Vec3 dir = point.at(front) - point.at(rear);
			Vec3 cross = dir.cross(axisVec).normalize();
		
			// 幅分を差し引いて座標を頂点として追加
			Vec3 pos = point.at(i) - cross * weight;
			basecross::VertexPositionColorTexture vertexLeft(pos, COL_WHITE, Vec2(0.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexLeft);

			// 幅分を加算した座標を頂点として追加
			pos = point.at(i) + cross * weight;
			basecross::VertexPositionColorTexture vertexRight(pos, COL_WHITE, Vec2(1.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexRight);
		}

		// 頂点インデックスの元
		const vector<uint16_t> baseIndices = {
			0, 1, 2,
			2, 1, 3
		};

		indices.clear(); // インデックスの初期化

		// サイズ分より一回少ない数ループ
		for (int i = 0; i < size - 1; i++)
		{
			// 元サイズ分ループ
			for (int j = 0; j < baseIndices.size(); j++)
			{
				// 元のインデックスに2の倍数を加算させて追加
				indices.push_back(baseIndices.at(j) + (2 * i));
			}
		}

		return true; // 生成出来たのでtrueを返す
	}

	/*!
	@brief リボン型頂点データと頂点インデックスを作成
	@param (point) 節目の座標配列
	@param (vertex) 頂点データと頂点インデックス
	@param (axisVec) 直行軸ベクトル
	@param (weight) 幅
	@param (texLoop) テクスチャループ
	@return 生成出来たかの真偽
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, basecross::VertexData& vertex, const Vec3& axisVec, float weight, int texLoop)
	{
		// 頂点データ構造体を分解して生成関数を実行
		return RibonVerticesIndices(point, vertex.vertices, vertex.indices, axisVec, weight, texLoop);
	}

	/*!
	@brief 円型用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void CircleVerticesIndices(vector<basecross::VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int squareNum)
	{
		const float HELF_SIZE = 0.5f;
		const Col4 DEFAULT_COLOR = Col4(1.0f);
		
		vertices.clear();
		indices.clear();

		basecross::VertexPositionColorTexture first(Vec3(0.0f), DEFAULT_COLOR, Vec2(HELF_SIZE));
		vertices.push_back(first);

		for (int i = 0; i < squareNum + 1; i++)
		{
			float rad = (XM_2PI / squareNum * i);
			float x = cosf(rad) * HELF_SIZE;
			float y = sinf(rad) * HELF_SIZE;
			Vec3 position = Vec3(x, y, 0.0f);
			Vec2 uvPos = Vec2(HELF_SIZE) + Vec2(x, -y);

			basecross::VertexPositionColorTexture v(position, DEFAULT_COLOR, uvPos);
			vertices.push_back(v);
		}

		for (int i = 0; i < squareNum; i++)
		{
			// 3の倍数を基準に設定
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}
	}

	/*!
	@brief 円型用頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	@param (number) 数字
	*/
	void CircleVerticesIndices(basecross::VertexData& vertex, const int squareNum)
	{
		CircleVerticesIndices(vertex.vertices, vertex.indices, squareNum);
	}

	/*!
	@brief ワールド座標をスクリーン座標に変換(BaseCrossの座標用、それ以外はDirectX付属の関数を備考)
	@param (viewPtr) viewのshared_ptr
	@param (position) ワールド座標
	@return 変換されたスクリーン座標
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<basecross::ViewBase>& viewPtr, const Vec3& position)
	{
		// ビューからカメラとビューポートを取得
		const auto& ptrCamera = viewPtr->GetTargetCamera();
		const auto& viewPort = viewPtr->GetTargetViewport();

		// マトリックスの定義
		const Mat4x4 viewMat = ptrCamera->GetViewMatrix();
		const Mat4x4 projMat = ptrCamera->GetProjMatrix();
		Mat4x4 worldMat;
		worldMat.affineTransformation(
			Vec3(1.0f),
			Vec3(0.0f),
			Vec3(0.0f),
			Vec3(0.0f)
		);

		// 3D座標を2D座標に変換(左上0,0基準)
		XMVECTOR xmWinPos = DirectX::XMVector3Project(
			position,
			viewPort.TopLeftX,
			viewPort.TopLeftY,
			viewPort.Width,
			viewPort.Height,
			viewPort.MinDepth,
			viewPort.MaxDepth,
			projMat,
			viewMat,
			worldMat
		);

		// BaseCross用に変換(中央0,0基準)
		Vec3 windowPos = Vec3(xmWinPos);
		windowPos.x -= static_cast<float>(viewPort.Width) / 2.0f;
		windowPos.y -= static_cast<float>(viewPort.Height) / 2.0f;
		windowPos.y *= -1.0f;

		// 変換された座標
		return windowPos;
	}

	/*!
	@brief ビルボードをクォータニオンで作成し返す
	@param (Line)　カメラの注視点 - カメラの位置
	@return 作成されたクォータニオン
	*/
	Quat GetBillboardQuat(const Vec3& Line)
	{
		// 線分のコピー
		Vec3 temp = Line;

		// マトリックスの宣言
		Mat4x4 rotMatrix;

		// 回転軸の設定
		Vec3 defUp(0, 1.0f, 0);

		// 線分をVec2に変換
		Vec2 tempVec(temp.x, temp.z);

		// 前後左右の長さが0.1f以下なら
		if (tempVec.length() < 0.1f)
		{
			// 回転軸を変更
			defUp = Vec3(0, 0, 1.0f);
		}
		
		// 線分の正規化
		temp.normalize();

		// マトリックスの計算
		rotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), temp, defUp);
		rotMatrix.inverse();

		// クォータニオンに変換し、正規化
		Quat qt = rotMatrix.quatInMatrix();
		qt.normalize();
		return qt;
	}

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(const Vec2& value, const Vec2& a, const Vec2& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y);
	}

	/*!
	@brief 値が範囲内かを返す
	@param (value)　確認する値
	@param (a)　範囲の値１
	@param (b)　範囲の値２
	@return 範囲内かの真偽
	*/
	bool GetBetween(const Vec3& value, const Vec3& a, const Vec3& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y) && GetBetween(value.z, a.z, b.z);
	}
#endif
}