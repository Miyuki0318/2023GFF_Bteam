#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross
{
	class Blower : public Gimmick
	{
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;
		weak_ptr<MultiParticle> m_particle;
		const float m_length;

	public:

		Blower(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const Vec3& scale,
			const eAngle& angle, const float length
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), scale, angle),
			m_length(length)
		{
			m_modelMat.affineTransformation(
				Vec3(0.4f, 2.0f, 0.4f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		virtual ~Blower() {}

		void OnCreate() override;

		void OnUpdate() override;

		void EffectUpdate();

		bool CheckBetween(const Vec3& targetPos);
	};
}