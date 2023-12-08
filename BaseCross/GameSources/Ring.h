#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross
{
	class Ring : public Gimmick
	{
	public:
		enum eRing
		{
			Big,
			Small,
		};

	private:

		shared_ptr<PNTBoneModelDraw> m_ptrDraw;
		eRing m_ring;
		bool m_isGet;
		float m_rotSpeed;
		float m_totalTime;
		float m_deleteTime;

	public:

		Ring(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale, eRing ring
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale, scale, scale), Up),
			m_ring(ring)
		{
			m_isGet = false;
			m_rotSpeed = 3.0f;
			m_totalTime = 0.0f;
			m_deleteTime = 1.0f;
			m_modelMat.affineTransformation(
				Vec3(0.475f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.75f, 0.0f)
			);
		}

		virtual ~Ring() {}

		void OnCreate() override;

		void OnUpdate() override;

		void IsGetRing()
		{
			m_isGet = true;
			m_rotSpeed = 12.0f;
			m_ptrColl->SetUpdateActive(false);
		}

		const eRing& GetRingSize() const
		{
			return m_ring;
		}
	};
}