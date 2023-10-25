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

			bool left = (targetPos.x >= 0.0f);

			Vec3 eye = Vec3(m_startEye.x + (left != false ? targetPos.x : 0.0f), m_startEye.y, m_startEye.z);
			Vec3 at = Vec3(m_startAt.x + (left != false ? targetPos.x : 0.0f), m_startAt.y, m_startAt.z);

			SetEye(eye);
			SetAt(at);
		}
	}
}