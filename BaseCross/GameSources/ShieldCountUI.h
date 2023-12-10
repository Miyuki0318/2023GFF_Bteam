#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Player.h"

namespace basecross 
{
	class ShieldCountUI: public Sprite
	{
		weak_ptr<Player> m_player;
		weak_ptr<Sprite> m_gauge;
		weak_ptr<Sprite> m_ring;
		weak_ptr<Sprite> m_shield;
		weak_ptr<Sprite> m_circle;

	public:

		ShieldCountUI(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position
		) :
			Sprite(stagePtr, L"SGAUGE_TX", scale, position)
		{
		}

		virtual ~ShieldCountUI() {}

		void OnCreate() override;

		void OnUpdate() override;

		void UpdateGauge();
	};
}