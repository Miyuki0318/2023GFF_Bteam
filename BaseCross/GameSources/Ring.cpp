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

		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		AddTag(L"Ring");
	}

	void Ring::OnUpdate()
	{
		Vec3 rot = GetRotation();
		rot.y -= App::GetApp()->GetElapsedTime() * m_rotSpeed;
		SetRotation(rot);

		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(!m_isGet);
	}
}