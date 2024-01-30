#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void GameCamera::OnCreate()
	{
		SetEye(m_startEye);
		SetAt(m_startAt);
	}

	void GameCamera::OnUpdate()
	{
		Vec3 targetPos;
		if (m_target.lock())
		{
			auto targetTrans = m_target.lock()->GetComponent<Transform>();
			targetPos = targetTrans->GetPosition();
			m_currentPos = targetPos;
		}
		else
		{
			targetPos = m_currentPos;
		}

		const float diff = 1.5f;
		const float followDistance = 0.2f;
		const float speed = 4.0f;
		bool left = (targetPos.x <= -30.5f);
		bool down = (targetPos.y <= -87.5f);
		targetPos.y -= diff;
		if (left) targetPos.x = -30.5f;
		if (down) targetPos.y = -87.5f;

		Vec3 vec = targetPos - (m_currentAt - Vec3(10.0f, 7.5f, 0.0f));
		float distance = vec.length();

		if (distance > followDistance)
		{
			float move = (distance - followDistance) * (DELTA_TIME * speed);

			m_currentEye += Vec3(vec.normalize().x * move, vec.normalize().y * move, 0.0f);
			m_currentAt += Vec3(vec.normalize().x * move, vec.normalize().y * move, 0.0f);
		}

		SetEye(m_currentEye);
		SetAt(m_currentAt);
	}
}