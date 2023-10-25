/*!
@file Number.cpp
@brief 数字を表示するスプライト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	/*!
	@brief 生成時に一度だけ呼び出される関数
	*/
	void Number::OnCreate()
	{
		// 継承元の生成時の関数を実行
		Sprite::OnCreate();

		// 頂点を初期化
		m_vertex.vertices.clear();

		// 数字表示用の頂点に変更
		Utility::NumberVerticesIndices(m_vertex, 0);

		// 頂点を更新
		SetNumber(0);
	}

	/*!
	@brief 表示する数字を変更する関数
	@param 数字
	*/
	void Number::SetNumber(const int number)
	{
		m_number = number;

		// 数値に応じてUV座標を変更
		m_vertex.vertices[0].textureCoordinate.x = m_number / 10.0f;
		m_vertex.vertices[1].textureCoordinate.x = m_number / 10.0f + 0.1f;
		m_vertex.vertices[2].textureCoordinate.x = m_number / 10.0f;
		m_vertex.vertices[3].textureCoordinate.x = m_number / 10.0f + 0.1f;

		// 頂点データの更新
		auto ptrMesh = GetComponent<PCTSpriteDraw>();
		ptrMesh->UpdateVertices(m_vertex.vertices);
	}
}