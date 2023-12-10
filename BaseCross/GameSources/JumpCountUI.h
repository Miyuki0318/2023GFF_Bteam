#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Player.h"

namespace basecross
{
	class JumpCountUI : public Sprite
	{
		weak_ptr<Player> m_player;
		weak_ptr<Sprite> m_gauge;
		weak_ptr<Sprite> m_airShock;

	public:

		JumpCountUI(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position
		) :
			Sprite(stagePtr, L"JGAUGE_TX", scale, position)
		{
		}

		virtual ~JumpCountUI() {}

		void OnCreate() override;

		void OnUpdate() override;

		void UpdateGauge();
	};
}