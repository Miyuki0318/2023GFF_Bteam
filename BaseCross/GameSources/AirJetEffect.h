/*!
@file AirJetEffect.h
@brief プレイヤーのエアジェットエフェクト
*/

#pragma once
#include "Billboard.h"

namespace basecross
{
	/*!
	@brief エアジェットエフェクト
	*/
	class AirJetEffect : public Billboard
	{
		// プレイヤーオブジェクトのポインタ
		weak_ptr<TemplateObject> m_player;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param プレイヤーのポインタ
		*/
		AirJetEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<TemplateObject>& playerPtr) :
			Billboard(stagePtr, L"EFFECT_TX", Vec2(0.0f), Vec3(0.0f)),
			m_player(playerPtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~AirJetEffect() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief エフェクトの更新処理関数
		*/
		void UpdateEffect();
	};
}