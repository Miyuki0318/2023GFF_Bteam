#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross
{
	class Ring : public Gimmick
	{
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;
		bool m_isGet;
		float m_rotSpeed;
		float m_totalTime;
		float m_deleteTime;

	public:

		Ring(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale / 10.0f, scale, scale), Up)
		{
			m_isGet = false;
			m_rotSpeed = 3.0f;
			m_totalTime = 0.0f;
			m_deleteTime = 3.0f;
			m_modelMat.affineTransformation(
				Vec3(4.75f, 0.475f, 0.475f),
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
		}
	};
}