#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Cannon::OnCreate()
	{
		Gimmick::OnCreate();
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"CANNON");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"FIRE", 0, 30, false);

		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		AddTag(L"Cannon");
	}

	void Cannon::OnUpdate()
	{
		float deltaTime = App::GetApp()->GetElapsedTime() * 1.5f;

		if (m_type == Rotate)
		{
			Vec3 rot = GetRotation();
			rot.z -= deltaTime;
			SetRotation(rot);
		}

		if (m_isFire)
		{
			if (m_ptrDraw->IsTargetAnimeEnd())
			{
				m_isFire = false;
			}

			m_ptrDraw->UpdateAnimation(deltaTime);
		}
	}
}	