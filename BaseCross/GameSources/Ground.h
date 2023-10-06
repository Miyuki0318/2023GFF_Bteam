#pragma once
#include "stdafx.h"

namespace basecross
{
	class Ground : public GameObject
	{
		const Vec3 m_position;
		const Vec3 m_scale;

	public:

		Ground(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& scale
		) :
			GameObject(stagePtr),
			m_position(position),
			m_scale(scale)
		{
		}

		virtual ~Ground() {}

		void OnCreate() override;

		void OnUpdate() override;
	};
}