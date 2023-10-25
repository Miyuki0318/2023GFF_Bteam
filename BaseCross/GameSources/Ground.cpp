#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Ground::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_position);
		ptrTrans->SetScale(m_scale);

		//auto ptrDraw = AddComponent<PNTStaticDraw>();
		//ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//ptrDraw->SetTextureResource(L"GROUND");
		//ptrDraw->SetOwnShadowActive(true);

		auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetDrawActive(true);
		ptrColl->SetFixed(true);
	}

	void Ground::OnUpdate()
	{

	}
}