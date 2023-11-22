#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Ring::OnCreate()
	{
		Gimmick::OnCreate();

		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"RING");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 30, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");

		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		AddTag(L"Ring");
	}

	void Ring::OnUpdate()
	{
		float deltaTime = DELTA_TIME;
		
		Vec3 rot = GetRotation();
		rot.y -= deltaTime * m_rotSpeed;
		SetRotation(rot);

		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(!m_isGet);

		m_ptrDraw->UpdateAnimation(deltaTime);

		if (m_isGet)
		{
			m_totalTime += deltaTime;

			float ratio = m_totalTime / m_deleteTime;


			m_ptrDraw->SetDiffuse(Utility::Lerp(COL_WHITE, COL_ALPHA, ratio));

			if (m_deleteTime <= m_totalTime)
			{
				GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			}
		}
	}
}