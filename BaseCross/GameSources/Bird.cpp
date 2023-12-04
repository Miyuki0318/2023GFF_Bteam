#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Bird::OnCreate()
	{
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"BIRD");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"WALK", 0, 30, true);
		m_ptrDraw->ChangeCurrentAnimation(L"WALK");

		AddTag(L"Bird");
	}

	void Bird::OnUpdate()
	{
		GetComponent<CollisionObb>()->SetDrawActive(true);

		float deltaTime = App::GetApp()->GetElapsedTime();
		m_ptrDraw->UpdateAnimation(deltaTime);
	}
}