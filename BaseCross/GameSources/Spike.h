#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross 
{
	class Spike : public Gimmick
	{
	public:

		Spike(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, const eAngle& angle
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale), angle)
		{
			m_bodyMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f, -0.65f, 0.0f)
			);
		}

		virtual ~Spike() {}

		void OnCreate() override;

		const eAngle GetAngle() const
		{
			return m_angle;
		}
	};
}