#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Convayor::OnCreate()
	{
		Gimmick::OnCreate();
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		switch (m_type)
		{
		case Convayor::Side:
			m_ptrDraw->SetMultiMeshResource(L"SCONVAYOR");
			break;

		case Convayor::Middle:
			m_ptrDraw->SetMultiMeshResource(L"MCONVAYOR");
			break;

		default:
			break;
		}

		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");

		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);

		AddTag(L"Convayor");
	}

	void Convayor::OnUpdate()
	{
		float deltaTime = App::GetApp()->GetElapsedTime();
		m_ptrDraw->UpdateAnimation(deltaTime);
	}
}