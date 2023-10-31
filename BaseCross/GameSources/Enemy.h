#pragma once
#include "stdafx.h"
#include "CubeObject.h"

namespace basecross
{
	class Enemy : public CubeObject
	{
	protected:

		Vec2 m_velocity;
		Mat4x4 m_modelMat;
		weak_ptr<GameObject> m_targetObj;

		const float m_speed;
		const float m_collRange;
		float m_acsel;

	public:

		Enemy(const shared_ptr<Stage>& stagePtr) :
			CubeObject(stagePtr),
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
			const Vec3& scale, float speed
		) :
			CubeObject(stagePtr, position, rotation, scale),
			m_speed(speed),
			m_collRange(4.0f)
		{
			m_velocity.zero();
			m_acsel = 1.0f;
		}

		virtual ~Enemy() {}

		void SetMoveValue(const Vec2& velocity, float acsel)
		{
			m_velocity = velocity;
			m_acsel = acsel < 1.0f ? 1.0f : acsel;
		}
	};
}