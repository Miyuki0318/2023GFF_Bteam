#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Spike::OnCreate()
	{
		Gimmick::OnCreate();
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(m_angle != All ? L"SPIKE" : L"SPIKE_BLOCK");
		m_ptrDraw->SetMeshToTransformMatrix(m_angle != All ? m_modelMat : m_blockMat);
		m_ptrDraw->AddAnimation(L"ATTACK", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ATTACK");
		AddTag(L"Spike");
	}

	void Spike::OnUpdate()
	{
		CubeObject::OnUpdate();
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}
}