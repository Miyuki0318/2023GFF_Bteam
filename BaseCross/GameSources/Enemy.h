#pragma once
#include "stdafx.h"

namespace basecross
{
	class Enemy : public GameObject
	{
		Vec3 m_position;
		Vec3 m_rotation;
		Vec2 m_velocity;

		const float m_speed;
		float m_acsel;

	public:

		Enemy(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_speed(0.0f)
		{
			m_position.zero();
			m_rotation.zero();
			m_velocity.zero();
			m_acsel = 1.0f;
		}

		Enemy(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation,
			const Vec2& velocity, float speed, float acsel
		) :
			GameObject(stagePtr),
			m_position(position),
			m_rotation(rotation),
			m_velocity(velocity),
			m_speed(speed),
			m_acsel(acsel)
		{
		}

		virtual ~Enemy() {}

		void OnCreate() override;

		void OnUpdate() override;
	};
}