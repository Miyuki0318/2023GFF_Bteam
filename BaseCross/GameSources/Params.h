/*!
@file Params.h
@brief 多用しそうなデータ型構造体の定義
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"

using namespace basecross;

namespace Params {

	/*!
	@brief TransformのVec3三種
	@param position
	@param rotation
	@param scale
	*/
	struct TransData
	{
		bsm::Vec3 position;	// 座標
		bsm::Vec3 rotation;	// 向き
		bsm::Vec3 scale;		// 規模

		TransData()
		{
			position.zero();
			rotation.zero();
			scale.zero();
		}

		TransData(const bsm::Vec3& pos, const bsm::Vec3& rot, const bsm::Vec3& sc)
		{
			position = pos;
			rotation = rot;
			scale = sc;
		}
	};

	/*!
	@brief 頂点データ
	@param vertices
	@param indices
	*/
	struct VertexData
	{
		vector<VertexPositionColorTexture> vertices; // 頂点
		vector<uint16_t> indices; // インデックス
	};
}