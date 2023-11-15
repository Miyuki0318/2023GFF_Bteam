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
		if (m_target.lock())
		{
			auto targetTrans = m_target.lock()->GetComponent<Transform>();
			Vec3 targetPos = targetTrans->GetPosition();

			const float diff = 1.5f;
			const float followDistance = 0.2f;
			const float speed = 3.0f;
			bool left = (targetPos.x >= -30.5f);
			targetPos.y -= diff;

			Vec3 vec = targetPos - (m_currentAt - Vec3(0.0f, 7.5f, 0.0f));
			float distance = vec.length();

			if (distance > followDistance)
			{
				float deltaTime = App::GetApp()->GetElapsedTime();
				float move = (distance - followDistance) * (deltaTime * speed);

				m_currentEye += Vec3(left != false ? vec.normalize().x * move : 0.0f, vec.normalize().y * move, 0.0f);
				m_currentAt += Vec3(left != false ? vec.normalize().x * move : 0.0f, vec.normalize().y * move, 0.0f);
			}



			m_currentPos = targetPos;
			SetEye(m_currentEye);
			SetAt(m_currentAt);
		}
	}
}