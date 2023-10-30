#pragma once
#include "stdafx.h"
#include "DebugObject.h"

namespace basecross
{
	class Enemy : public DebugObject
	{
		Vec2 m_velocity;
		weak_ptr<GameObject> m_targetObj;

		const float m_speed;
		const float m_collRange;
		float m_acsel;

	public:

		Enemy(const shared_ptr<Stage>& stagePtr) :
			DebugObject(stagePtr),
			m_speed(0.0f),
			m_collRange(4.0f)
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
			DebugObject(stagePtr, position, rotation, Vec3(1.0f)),
			m_velocity(velocity),
			m_speed(speed),
			m_acsel(acsel),
			m_collRange(4.0f)
		{
		}

		virtual ~Enemy() {}

		void OnCreate() override;

		void OnUpdate() override;

		void SetMoveValue(const Vec2& velocity, float acsel)
		{
			m_velocity = velocity;
			m_acsel = acsel < 1.0f ? 1.0f : acsel;
		}

		void SetTarget(const shared_ptr<GameObject>& objPtr)
		{
			m_targetObj = objPtr;
		}

		const shared_ptr<GameObject>& GetTarget() const
		{
			return m_targetObj.lock();
		}
	};
}