#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross
{
	class Cannon : public Gimmick
	{
	public:

		enum eFireType
		{
			Normal,
			Rapid,
			NRotate,
			RRotate,
		};

	private:

		bool m_isFire;
		const float m_particleTime;
		const eFireType m_fireType;
		vector<float> m_fireTime;
		vector<wstring> m_animeKey;
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;
		weak_ptr<MultiParticle> m_particle;

	public:

		Cannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, 
			const eAngle& angle, const eFireType& fire
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale), angle),
			m_fireType(fire),
			m_particleTime(0.04f)
		{
			m_isFire = false;

			m_fireTime = {
				1.4f,
				0.4f,
			};

			m_animeKey = {
				L"FIRE",
				L"RAPID",
			};

			m_modelMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, -1.0f, 0.0f)
			);
		}

		virtual ~Cannon() {}

		void OnCreate() override;

		void OnUpdate() override;

		void EffectUpdate();

		void OnFire()
		{
			m_isFire = true;
			int type = static_cast<int>(m_fireType) % 2;
			m_ptrDraw->ChangeCurrentAnimation(m_animeKey.at(type), 0.0f);
		}

		const float& GetFireTime() const
		{
			int type = static_cast<int>(m_fireType) % 2;
			return m_fireTime.at(type);
		}
	};
}