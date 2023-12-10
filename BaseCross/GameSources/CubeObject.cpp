#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void CubeObject::OnCreate()
	{
		TemplateObject::OnCreate();

		SetPosition(m_position);
		SetRotation(m_type != Normal ? Utility::DegToRad(m_angle.at(m_type)) : m_rotation);
		SetScale(m_scale);

		if (m_collActive)
		{
			m_ptrColl = AddComponent<CollisionObb>();
			m_ptrColl->SetFixed(true);
			m_ptrColl->SetUpdateActive(false);
		}

		SetAlphaActive(true);
	}

	void CubeObject::OnUpdate()
	{
		CollisionPerformance(m_collRange);
	}


	void CubeObject::CollisionPerformance(const float range)
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
					achieve = (length <= range);
					if (achieve) break;
				}
			}

			m_ptrColl->SetUpdateActive(achieve);
		}
	}
}