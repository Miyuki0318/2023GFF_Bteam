#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Bumper::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BUMPER");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"PUSH", 0, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"PUSH");

		m_ptrColl = AddComponent<CollisionSphere>();
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		AddTag(L"Bumper");
	}

	void Bumper::OnUpdate()
	{
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}
}