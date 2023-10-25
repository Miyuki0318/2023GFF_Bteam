/*!
@file BillboardNumber.h
@brief ビルボード表示する数字オブジェクト
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#include "Billboard.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	BillboardNumber : public Billboard ビルボード表示する数字オブジェクト
	// ----------------------------------------------------------------------------------
	class BillboardNumber : public Billboard
	{
		int m_number; // 数字

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param スケール
		*/
		BillboardNumber(const shared_ptr<Stage>& stagePtr) :
			Billboard(stagePtr)
		{
			m_number = 0;
			Utility::NumberVerticesIndices(m_vertex, m_number);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~BillboardNumber() {};

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 表示する数値を設定する関数
		@param 設定する数値
		*/
		void SetNumber(const int& number)
		{
			// 数値の保持
			m_number = number;

			// 数値に応じてUV座標を変更
			m_vertex.vertices[0].textureCoordinate.x = m_number / 10.0f;
			m_vertex.vertices[1].textureCoordinate.x = m_number / 10.0f + 0.1f;
			m_vertex.vertices[2].textureCoordinate.x = m_number / 10.0f;
			m_vertex.vertices[3].textureCoordinate.x = m_number / 10.0f + 0.1f;

			// 頂点データの更新
			auto ptrMesh = GetComponent<PCTStaticDraw>();
			ptrMesh->UpdateVertices(m_vertex.vertices);
		}
	};
}