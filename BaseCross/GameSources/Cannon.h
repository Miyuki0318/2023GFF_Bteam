#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross
{
	class Cannon : public Gimmick
	{
	public:

		enum eType
		{
			Normal,
			Rotate,
		};

	private:

		bool m_isFire;
		const eType m_type;
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;
		weak_ptr<MultiParticle> m_particle;

	public:

		Cannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, const eAngle& angle, const eType& type
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale), angle),
			m_type(type)
		{
			m_isFire = false;

			m_modelMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -1.0f, 0.0f)
			);
		}

		virtual ~Cannon() {}

		void OnCreate() override;

		void OnUpdate() override;

		void OnFire()
		{
			m_isFire = true;
			m_ptrDraw->ChangeCurrentAnimation(L"FIRE", 0.0f);
		}
	};
}