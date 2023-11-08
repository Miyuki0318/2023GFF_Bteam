#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void CubeObject::OnCreate()
	{
		SetPosition(m_position);
		SetRotation(m_type != Normal ? Utility::DegToRad(m_angle.at(m_type)) : m_rotation);
		SetScale(m_scale);

		if (m_collActive)
		{
			auto ptrColl = AddComponent<CollisionObb>();
			ptrColl->SetFixed(true);
			ptrColl->SetUpdateActive(false);
		}

		SetAlphaActive(true);
	}

	void CubeObject::OnUpdate()
	{
		if (m_targetObj.lock())
		{
			auto targetTrans = m_targetObj.lock()->GetComponent<Transform>();
			Vec3 targetPos = targetTrans->GetPosition();
			Vec3 pos = GetPosition();

			float length = (targetPos - pos).length();
			SetDrawActive(length <= 55.0f);

			if (m_collActive)
			{
				auto ptrColl = GetComponent<CollisionObb>();
				ptrColl->SetUpdateActive(length <= m_scale.x * m_collRange);
			}
		}
	}
}