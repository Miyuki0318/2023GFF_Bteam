#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Enemy::OnCreate()
	{
		SetPosition(m_position);
		SetRotation(m_rotation);
		SetScale(m_scale);

		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetUpdateActive(false);

		SetAlphaActive(true);
	}

	void Enemy::OnUpdate()
	{
		auto ptrColl = GetComponent<CollisionSphere>();

		if (m_targetObj.lock())
		{
			auto targetTrans = m_targetObj.lock()->GetComponent<Transform>();
			Vec3 targetPos = targetTrans->GetPosition();
			Vec3 pos = GetPosition();

			float length = (targetPos - pos).length();
			ptrColl->SetUpdateActive(length <= m_scale.x * m_collRange);
		}
		else
		{
			ptrColl->SetUpdateActive(false);
		}
	}
}