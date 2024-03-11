/*!
@file TitlePlayer.h
@brief タイトル画面用のプレイヤー
*/

#pragma once
#include "Player.h"

namespace basecross
{
	/*!
	@brief タイトル画面用のプレイヤー
	*/
	class TitlePlayer : public Player
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param ポジション
		*/
		TitlePlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~TitlePlayer() {}

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief Aボタンが押されなくなった時に呼び出される関数
		*/
		void OnRushA() override;

		/*!
		@brief エアショックの軌道描画関数
		*/
		void UpdateArrow() override;

		/*!
		@brief 大砲待機関数
		*/
		void CannonStandby(float acsel) override;
	};
}