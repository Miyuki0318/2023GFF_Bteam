#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Spike::OnCreate()
	{
		Gimmick::OnCreate();
		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		ptrDraw->SetMeshResource(L"SPIKE");
		ptrDraw->SetMeshToTransformMatrix(m_bodyMat);

		AddTag(L"Spike");
	}
}