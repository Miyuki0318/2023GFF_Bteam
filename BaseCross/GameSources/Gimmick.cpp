#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Gimmick::OnCreate()
	{
		CubeObject::OnCreate();
		SetRotation(Utility::DegToRad(Vec3(0.0f, 0.0f, m_angles.at(m_angle))));
		AddTag(L"Gimmick");
		SetAlphaActive(true);
	}
}