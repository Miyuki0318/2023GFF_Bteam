#pragma once
#include "stdafx.h"

namespace basecross
{
	class GameCamera : public Camera
	{
		weak_ptr<GameObject> m_target;

		const Vec3 m_startEye;
		const Vec3 m_startAt;
		Vec3 m_currentPos;
		Vec3 m_currentEye;
		Vec3 m_currentAt;

	public:

		GameCamera() :
			m_startEye(Vec3(-20.5f, -82.3f, -33.0f)),
			m_startAt(Vec3(-20.5f, -82.3f, 0.0f))
		{
			m_currentPos = Vec3(-35.0f ,-88.5f, 0.0f);
			m_currentEye = m_startEye;
			m_currentAt = m_startAt;
		}

		virtual ~GameCamera() {}

		void OnCreate() override;

		void OnUpdate() override;

		void SetTargetObject(const shared_ptr<GameObject>& object)
		{
			if (m_target.lock() != object)
			{
				m_target = object;
			}
		}

		void RemoveTarget()
		{
			m_currentPos = m_target.lock()->GetComponent<Transform>()->GetPosition();
			m_target.reset();
		}
	};
}