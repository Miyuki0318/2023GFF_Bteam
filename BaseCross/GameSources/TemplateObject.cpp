#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TemplateObject::OnCreate()
	{
		m_ptrTrans = GetComponent<Transform>();
	}

	bool TemplateObject::SetTimer(float time, bool reset)
	{
		const auto& timer = GetTypeStage<BaseStage>()->GetTimer();
		return timer->SetTimer(ThisPtr, time, reset);
	}

	float TemplateObject::GetTime(float time)
	{
		const auto& timer = GetTypeStage<BaseStage>()->GetTimer();
		return timer->GetTime(ThisPtr, time);
	}
}