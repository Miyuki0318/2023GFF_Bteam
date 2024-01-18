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

	protected:

		int m_select;
		bool m_isFire;
		bool m_isRotate;
		float m_currentRot;
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
			m_select = static_cast<int>(m_fireType) % 2;
			m_isFire = false;
			m_isRotate = false;
			m_currentRot = 0.0f;

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

		virtual void OnCreate() override;

		void OnUpdate() override;

		void RotateCannon();

		void FireAnimation();

		virtual void EffectUpdate();

		void OnFire()
		{
			m_isFire = true;
			m_ptrDraw->ChangeCurrentAnimation(m_animeKey.at(m_select), 0.0f);
		}

		bool GetFire() const
		{
			return m_isFire;
		}

		const float& GetFireTime() const
		{
			return m_fireTime.at(m_select);
		}
	};

	class GoalCannon : public Cannon
	{
	public:

		GoalCannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			Cannon(stagePtr, position, scale, eAngle::Uright, eFireType::Normal)
		{
		}

		virtual ~GoalCannon() {}

		void OnCreate() override;

		void EffectUpdate() override;
	};

	class TitleCannon : public Cannon
	{
	public:

		enum eCannonType
		{
			Easy,
			Normal,
			Hard,
		};

	private:

		eCannonType m_cannonType;

	public:

		TitleCannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eCannonType& cannonType
		) :
			Cannon(stagePtr, position, scale, Gimmick::Up, eFireType::Normal),
			m_cannonType(cannonType)
		{
		}

		virtual ~TitleCannon() {}

		void OnCreate() override;
	};
}