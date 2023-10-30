#pragma once
#include "stdafx.h"
#include "DebugObject.h"

namespace basecross
{
	class CubeObject : public DebugObject
	{
		const float m_collRange;
		weak_ptr<GameObject> m_targetObj;

	public:

		CubeObject(const shared_ptr<Stage>& stagePtr) :
			DebugObject(stagePtr),
			m_collRange(4.0f)
		{
		}

		CubeObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale
		) :
			DebugObject(stagePtr, position, rotation, scale),
			m_collRange(4.0f)
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