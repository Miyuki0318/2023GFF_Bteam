#pragma once
#include "stdafx.h"
#include "Billboard.h"

namespace basecross
{
	class AirJetEffect : public Billboard
	{
		weak_ptr<DebugObject> m_ownerObject;

	public:

		AirJetEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<DebugObject>& ownerPtr) :
			Billboard(stagePtr, L"EFFECT_TX", Vec2(0.0f), Vec3(0.0f)),
			m_ownerObject(ownerPtr)
		{
		}

		virtual ~AirJetEffect() {}

		void OnCreate() override;

		void UpdateEffect();
	};
}