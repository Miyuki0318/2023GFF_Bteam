#pragma once
#include "stdafx.h"
#include "DebugObject.h"

namespace basecross
{
	class ArrowEffect : public DebugObject
	{
		VertexData vertex;
		weak_ptr<DebugObject> m_ownerObject;
		shared_ptr<PCTStaticDraw> m_ptrDraw;

	public:

		ArrowEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<DebugObject>& ownerPtr) :
			DebugObject(stagePtr),
			m_ownerObject(ownerPtr)
		{
		}

		virtual ~ArrowEffect() {}

		void OnCreate() override;

		void UpdateEffect(const vector<Vec3>& points);
	};
}