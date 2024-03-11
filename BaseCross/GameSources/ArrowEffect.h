/*!
@file ArrowEffect.h
@brief プレイヤーのエアジェットでの予測軌道
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief エアジェットでの予測軌道
	*/
	class ArrowEffect : public TemplateObject
	{
		VertexData m_vertex; // 頂点データ構造体
		weak_ptr<TemplateObject> m_player; // プレイヤーのポインタ
		shared_ptr<PCTStaticDraw> m_ptrDraw; // 描画コンポーネント

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param プレイヤーのポインタ
		*/
		ArrowEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<TemplateObject>& playerPtr) :
			TemplateObject(stagePtr),
			m_player(playerPtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~ArrowEffect() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief エフェクトの更新処理関数
		@param 軌道のエフェクトに必要な節の座標
		*/
		void UpdateEffect(const vector<Vec3>& points);
	};
}