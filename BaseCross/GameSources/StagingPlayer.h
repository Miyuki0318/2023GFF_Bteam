#pragma once
#include "stdafx.h"
#include "Player.h"

namespace basecross
{
	class StagingPlayer : public Player
	{
	public:

		StagingPlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position)
		{
		}

		virtual ~StagingPlayer() {}

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief Aボタンが押されなくなった時に呼び出される関数
		*/
		void OnReleaseA() override;

		/*!
		@brief エアショックの軌道描画関数
		*/
		void RotateAligment() override;

		/*!
		@brief 大砲待機関数
		*/
		virtual void CannonStandby(float acsel);
	};
}