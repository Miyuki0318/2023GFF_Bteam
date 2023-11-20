#pragma once
#include "stdafx.h"
#include "DebugSphere.h"

namespace basecross
{
	class ShieldEffect : public DebugSphere
	{
		weak_ptr<DebugObject> m_outLine;
		weak_ptr<DebugObject> m_ownerObject;
		shared_ptr<PNTStaticDraw> m_ptrDraw;

	public:

		ShieldEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<DebugObject>& ownerPtr) :
			DebugSphere(stagePtr, Vec3(0.0f), Vec3(0.0f), Vec3(2.0f)),
			m_ownerObject(ownerPtr)
		{
		}

		virtual ~ShieldEffect() {}

		void OnCreate() override;

		void UpdateEffect();
	};

	class ShieldOutLine : public DebugObject
	{
		weak_ptr<DebugSphere> m_ownerObject;

	public:

		ShieldOutLine(const shared_ptr<Stage>& stagePtr, const shared_ptr<DebugSphere>& ownerPtr) :
			DebugObject(stagePtr),
			m_ownerObject(ownerPtr)
		{
		}

		virtual ~ShieldOutLine() {}

		void OnCreate() override;
	};
}