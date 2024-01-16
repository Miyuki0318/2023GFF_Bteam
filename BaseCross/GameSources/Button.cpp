#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Button::OnCreate()
	{
		Gimmick::OnCreate();
		
		m_ptrColl->SetDrawActive(true);
		m_ptrColl->SetUpdateActive(true);
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BUTTON");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"PUSH", 0, 30, false);
		m_ptrDraw->AddAnimation(L"REVERSE", 30, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"REVERSE");
	}

	void Button::OnUpdate()
	{
		m_active = GetHitButton();
		m_reverseTime = min(1.0f - m_ptrDraw->GetCurrentAnimationTime(), 1.0f);

		if (m_active != m_current)
		{
			m_ptrDraw->ChangeCurrentAnimation(m_active ? L"PUSH" : L"REVERSE", m_reverseTime);
		}

		m_ptrDraw->UpdateAnimation(DELTA_TIME * 2.0f);
		m_current = m_active;
	}
}