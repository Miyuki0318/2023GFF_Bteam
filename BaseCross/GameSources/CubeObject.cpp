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
			for (size_t i = 0; i < m_targetObj.size(); i++)
			{
				if (m_targetObj.at(i).lock())
				{
					auto targetTrans = m_targetObj.at(i).lock()->GetComponent<Transform>();
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