#pragma once
#include "stdafx.h"

namespace basecross
{
	class ShieldEffect : public TemplateObject
	{
		weak_ptr<TemplateObject> m_outLine;
		weak_ptr<TemplateObject> m_ownerObject;
		shared_ptr<PNTStaticDraw> m_ptrDraw;

	public:

		ShieldEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<TemplateObject>& ownerPtr) :
			TemplateObject(stagePtr, Vec3(0.0f), Vec3(0.0f), Vec3(2.0f)),
			m_ownerObject(ownerPtr)
		{
		}

		virtual ~ShieldEffect() {}

		void OnCreate() override;

		void UpdateEffect();

		void SetDrawShield(bool b);
	};

	class ShieldOutLine : public TemplateObject
	{
		weak_ptr<TemplateObject> m_ownerObject;

	public:

		ShieldOutLine(const shared_ptr<Stage>& stagePtr, const shared_ptr<TemplateObject>& ownerPtr) :
			TemplateObject(stagePtr),
			m_ownerObject(ownerPtr)
		{
		}

		virtual ~ShieldOutLine() {}

		void OnCreate() override;
	};
}