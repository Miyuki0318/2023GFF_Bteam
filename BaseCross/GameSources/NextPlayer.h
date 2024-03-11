/*!
@file NextPlayer.h
@brief 次のステージへ画面用のプレイヤー
*/

#pragma once
#include "Player.h"
#include "NextStage.h"

namespace basecross
{
	/*!
	@brief 「次のステージへ」中のプレイヤー
	*/
	class NextPlayer : public Player
	{
		const float m_moveAcsel; // 移動時の加速度
		const float m_dropAcsel; // 落下時の加速度

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		NextPlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position),
			m_moveAcsel(5.0f),
			m_dropAcsel(2.5f)
		{
			m_velocity = Vec2(-1.0f, 0.0f);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~NextPlayer() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief Aボタンが押されなくなった時に呼び出される関数
		*/
		void OnRushA() override;

		/*!
		@brief プレイヤーの回転関数
		*/
		void RotatePlayer() override;

		/*!
		@brief 死亡時の設定関数
		*/
		void DropSetup();

		/*!
		@brief 死亡時の落下関数
		*/
		void TitleDrop();

		/*!
		@brief 選択肢ステート関数
		*/
		void SelectState();

		/*!
		@brief 次のステージに移動したかチェックする関数
		*/
		void NextStageCheck();

		/*!
		@brief 煙のエフェクト処理関数
		@param 移動量
		*/
		void SmokeEffect(const Vec2& velo);
	};
}