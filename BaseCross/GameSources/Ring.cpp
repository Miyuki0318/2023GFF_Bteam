#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Ring::OnCreate()
	{
		float size = 1.0f;
		if (m_ring == Big) size = 5.0f;
		m_scale *= size;
		Gimmick::OnCreate();

		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"RING");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 30, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");
		m_ptrDraw->SetEmissive(COL_WHITE);

		m_ptrColl->SetMakedSize(m_ring == Big ? 1.0f : 1.75f);
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		m_targetObj.clear();
		m_targetObj.push_back(GetStage()->GetSharedGameObject<Player>(L"Player"));

		AddTag(L"Ring");
	}

	void Ring::OnUpdate()
	{
		CubeObject::OnUpdate();

		const float& deltaTime = DELTA_TIME;
		
		Vec3 rot = GetRotation();
		rot.y -= deltaTime * m_rotSpeed;
		SetRotation(rot);

		m_ptrDraw->UpdateAnimation(deltaTime);

		if (m_isGet)
		{
			m_totalTime += deltaTime;

			float size = m_ring == Big ? 5.0f : 1.0f;
			float ratio = m_totalTime / m_deleteTime;

			SetScale(Utility::SinCurve(ratio * Utility::DegToRad(235.0f), -2.5f, size * 1.5f));
			m_ptrDraw->SetDiffuse(Utility::Lerp(COL_WHITE, COL_ALPHA, ratio));

			if (m_deleteTime <= m_totalTime)
			{
				GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			}
		}
	}
}