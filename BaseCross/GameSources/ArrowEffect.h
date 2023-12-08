#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross
{
	class ArrowEffect : public TemplateObject
	{
		VertexData vertex;
		weak_ptr<TemplateObject> m_ownerObject;
		shared_ptr<PCTStaticDraw> m_ptrDraw;

	public:

		ArrowEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<TemplateObject>& ownerPtr) :
			TemplateObject(stagePtr),
			m_ownerObject(ownerPtr)
		{
		}

		virtual ~ArrowEffect() {}

		void OnCreate() override;

		void UpdateEffect(const vector<Vec3>& points);
	};
}