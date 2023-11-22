#pragma once
#include "stdafx.h"
#include "DebugObject.h"

namespace basecross
{
	class AcselHole : public DebugObject
	{
		VertexData vertex;
	
	public:
	
		AcselHole(const shared_ptr<Stage>& stagePtr) :
			DebugObject(stagePtr)
		{
		}

		virtual ~AcselHole() {}

		void OnCreate() override;

		void OnUpdate() override;
	};
}