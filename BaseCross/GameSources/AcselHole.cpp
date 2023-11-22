#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void AcselHole::OnCreate()
	{
		SetPosition(Vec3(-22.0f, 15.0f, 0.0f));
		SetScale(Vec3(3.0f, 0.25f, 3.0f));


		int segment = 120;
		vector<Vec3> points;

		for (int i = 0; i < segment + 1; i++)
		{
			float rad = Utility::DegToRad(360.0f / segment * i);
			Vec3 pos = Vec3(sin(rad), 0.0f, cos(rad));
			points.push_back(pos);
		}

		Utility::RibonVerticesIndices(points, vertex, Vec3(0.0f, 0.0f, 1.0f), 0.5f, 1);

		auto ptrDraw = AddComponent<PCTStaticDraw>();
		ptrDraw->SetOriginalMeshUse(true);
		ptrDraw->CreateOriginalMesh(vertex);
		//ptrDraw->SetTextureResource(L"WHITE_BAR_TX");
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);
		
		SetAlphaActive(true);
	}

	void AcselHole::OnUpdate()
	{
	}
}