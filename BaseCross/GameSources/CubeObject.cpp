#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void CubeObject::OnCreate()
	{
		SetPosition(m_position);
		SetRotation(m_rotation);
		SetScale(m_scale);

		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		ptrColl->SetUpdateActive(false);

		SetAlphaActive(true);
	}

	void CubeObject::OnUpdate()
	{
		auto ptrColl = GetComponent<CollisionObb>();

		if (m_targetObj.lock())
		{
			auto targetTrans = m_targetObj.lock()->GetComponent<Transform>();
			Vec3 targetPos = targetTrans->GetPosition();

			auto ptrTrans = GetComponent<Transform>();
			Vec3 pos = ptrTrans->GetPosition();

			float length = (targetPos - pos).length();
			ptrColl->SetUpdateActive(length <= m_scale.x * 4.0f);
		}
		else
		{
			ptrColl->SetUpdateActive(false);
		}
	}
}