#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TemplateObject::OnCreate()
	{
		m_ptrTrans = GetComponent<Transform>();
	}

	void TemplateObject::StartSE(const wstring& seKey, float volume)
	{
		GetTypeStage<BaseStage>()->CreateSE(seKey, volume, ThisPtr);
	}

	void TemplateObject::StopSE(const wstring& seKey)
	{
		GetTypeStage<BaseStage>()->StopSE(seKey, ThisPtr);
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