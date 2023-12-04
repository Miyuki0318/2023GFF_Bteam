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
		if (m_collActive)
		{
			bool achieve = false;
			for (const auto& obj : m_targetObj)
			{
				const auto& target = obj.lock();
				if (target && target->GetUpdateActive())
				{
					auto targetTrans = target->GetComponent<Transform>();
					Vec3 targetPos = targetTrans->GetPosition();
					float length = (targetPos - m_position).length();
					achieve = (length <= m_collRange);
					if (achieve) break;
				}
			}

			auto ptrColl = GetComponent<CollisionObb>();
			ptrColl->SetUpdateActive(achieve);
		}
	}
}