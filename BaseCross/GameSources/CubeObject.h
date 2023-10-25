#pragma once
#include "stdafx.h"
#include "DebugObject.h"

namespace basecross
{
	class CubeObject : public DebugObject
	{
		weak_ptr<GameObject> m_targetObj;

	public:

		CubeObject(const shared_ptr<Stage>& stagePtr) :
			DebugObject(stagePtr)
		{
		}

		CubeObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale
		) :
			DebugObject(stagePtr, position, rotation, scale)
		{
		}

		virtual ~CubeObject() {}

		void OnCreate() override;

		void OnUpdate() override;

		void SetTarget(const shared_ptr<GameObject>& objPtr)
		{
			m_targetObj = objPtr;
		}

		const shared_ptr<GameObject>& GetTarget() const
		{
			return m_targetObj.lock();
		}
	};
}