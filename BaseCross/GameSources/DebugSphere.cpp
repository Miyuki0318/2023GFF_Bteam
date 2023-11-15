#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void DebugSphere::OnCreate()
	{
		SetPosition(m_position);
		SetRotation(m_rotation);
		SetScale(m_scale);
		SetAlphaActive(true);
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
	}
}