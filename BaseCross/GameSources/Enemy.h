#pragma once
#include "stdafx.h"
#include "CubeObject.h"

namespace basecross
{
	class Enemy : public TemplateObject
	{
	protected:

		Vec2 m_velocity;
		Mat4x4 m_modelMat;
		weak_ptr<TemplateObject> m_targetObj;
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

		const float m_speed;
		const float m_maxAcsel;
		const float m_range;
		float m_acsel;

	public:

		Enemy(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, 
			const Vec3& scale, float speed, float acsel, float range
		) :
			TemplateObject(stagePtr, position, rotation, scale),
			m_speed(speed),
			m_maxAcsel(acsel),
			m_range(range)
		{
			m_velocity.zero();
			m_acsel = 1.0f;
		}

		virtual ~Enemy() {}

		virtual void SetMoveValue(const Vec2& velocity, float acsel)
		{
			m_velocity = velocity;
			m_acsel = acsel < 1.0f ? 1.0f : acsel;
		}
	};
}