#pragma once
#include "stdafx.h"
#include "DebugObject.h"

namespace basecross
{
	class DebugSphere : public DebugObject
	{
	public:

		DebugSphere(const shared_ptr<Stage>& stagePtr) :
			DebugObject(stagePtr) 
		{
		}

		DebugSphere(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale
		) :
			DebugObject(stagePtr, position, rotation, scale)
		{
		}

		virtual ~DebugSphere() {}

		void OnCreate() override;
	};
}