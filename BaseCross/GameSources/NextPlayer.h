#pragma once
#include "stdafx.h"
#include "Player.h"
#include "NextStage.h"

namespace basecross
{
	class NextPlayer : public Player
	{
		float m_moveAcsel;

	public:

		NextPlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position)
		{
			m_acsel = 5.0f;
			m_moveAcsel = 5.0f;
			m_velocity = Vec2(-1.0f, 0.0f);
		}

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
		void OnReleaseA() override;

		/*!
		@brief プレイヤーの回転関数
		*/
		void RotatePlayer() override;

		/*!
		@brief 死亡時の設定関数
		*/
		void DeathSetup() override;

		/*!
		@brief 死亡時の落下関数
		*/
		void DeathDrop() override;


		void SelectState();

		void NextStageMove();

		void UpdateParticle(const Vec2& velo);
	};
}