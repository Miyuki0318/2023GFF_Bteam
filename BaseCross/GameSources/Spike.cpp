#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Spike::OnCreate()
	{
		Gimmick::OnCreate();
		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		ptrDraw->SetMeshResource(m_angle != All ? L"SPIKE" : L"SPIKE_BLOCK");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);

		AddTag(L"Spike");
	}
}