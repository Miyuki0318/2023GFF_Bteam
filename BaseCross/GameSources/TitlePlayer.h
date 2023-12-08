#pragma once
#include "stdafx.h"
#include "Player.h"

namespace basecross
{
	class TitlePlayer : public Player
	{
	public:

		TitlePlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position)
		{
		}

		virtual ~TitlePlayer() {}

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

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
		void CannonStandby(float acsel) override;
	};
}