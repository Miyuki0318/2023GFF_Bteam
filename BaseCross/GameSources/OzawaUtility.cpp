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
	@brief シンプルな頂点データと頂点インデックスを作成
	@param (vertices) 頂点データ
	@param (indices) 頂点インデックス
	*/
	void SimpleVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices)
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
	void SimpleVerticesIndices(VertexData& vertex)
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
	void SimpleVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Col4& color)
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
	void SimpleVerticesIndices(VertexData& vertex, const Col4& color)
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
	void NumberVerticesIndices(vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const int number)
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
	void NumberVerticesIndices(VertexData& vertex, const int number)
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
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices, const Vec3& axisVec, float weight, int texLoop)
	{
		const int& size = static_cast<int>(point.size());
		if (size == 0) return false;

		vertices.clear();

		for (int i = 0; i < size; i++)
		{
			const int loop = texLoop > 0 ? texLoop : 1;
			int front = (i - 1) < 0 ? size - 1 : i - 1;
			int rear = (i + 1) % size;
			Vec3 dir = point.at(front) - point.at(rear);
			Vec3 cross = dir.cross(axisVec).normalize();
		
			Vec3 pos = point.at(i) - cross * weight;
			VertexPositionColorTexture vertexLeft(pos, COL_WHITE, Vec2(0.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexLeft);

			pos = point.at(i) + cross * weight;
			VertexPositionColorTexture vertexRight(pos, COL_WHITE, Vec2(1.0f, static_cast<float>(i) / size * loop));
			vertices.push_back(vertexRight);
		}

		const vector<uint16_t> baseIndices = {
			0, 1, 2,
			2, 1, 3
		};

		indices.clear();

		for (int i = 0; i < point.size() - 1; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				indices.push_back(baseIndices.at(j) + (2 * i));
			}
		}

		return true;
	}

	/*!
	@brief リボン型頂点データと頂点インデックスを作成
	@param (point) 節目の座標配列
	@param (vertex) 頂点データと頂点インデックス
	@param (axisVec) 直行軸ベクトル
	@param (weight) 幅
	@param (texLoop) テクスチャループ
	*/
	bool RibonVerticesIndices(const vector<Vec3>& point, VertexData& vertex, const Vec3& axisVec, float weight, int texLoop)
	{
		return RibonVerticesIndices(point, vertex.vertices, vertex.indices, axisVec, weight, texLoop);
	}

	/*!
	@brief ワールド座標をスクリーン座標に変換(BaseCrossの座標用、それ以外はDirectX付属の関数を備考)
	@param (viewPtr) viewのshared_ptr
	@param (position) ワールド座標
	@return 変換されたスクリーン座標
	*/
	Vec3 ConvertToWorldPosition(const shared_ptr<ViewBase>& viewPtr, const Vec3& position)
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
	@brief ディグリー角をラジアン角に変換
	@param (deg) ディグリー角
	@return ラジアン角に変換されたディグリー角
	*/
	float DegToRad(const float& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	Vec2 DegToRad(const Vec2& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	Vec3 DegToRad(const Vec3& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	Vec4 DegToRad(const Vec4& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	/*!
	@brief ラジアン角をディグリー角に変換
	@param (rad) ラジアン角
	@return ディグリー角に変換されたラジアン角
	*/
	float RadToDeg(const float& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	Vec2 RadToDeg(const Vec2& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	Vec3 RadToDeg(const Vec3& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	Vec4 RadToDeg(const Vec4& rad)
	{
		return rad * (180.0f / XM_PI);
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
	bool GetBetween(int value, int a, int b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	bool GetBetween(float value, float a, float b)
	{
		return a > b ? value <= a && value >= b : value <= b && value >= a;
	}

	bool GetBetween(const Vec2& value, const Vec2& a, const Vec2& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y);
	}

	bool GetBetween(const Vec3& value, const Vec3& a, const Vec3& b)
	{
		return GetBetween(value.x, a.x, b.x) && GetBetween(value.y, a.y, b.y) && GetBetween(value.z, a.z, b.z);
	}
}