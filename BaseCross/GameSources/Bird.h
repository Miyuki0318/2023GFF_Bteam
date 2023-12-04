#pragma once
#include "stdafx.h"
#include "Enemy.h"

namespace basecross
{
	class Bird : public Enemy
	{
	public:

		enum eState
		{
			Patrol,
			Chase,
			FlyAway
		};

	private:

		eState m_state;
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

	public:
		
		Bird(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		):
			Enemy(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f, XM_PIDIV2, 0.0f), Vec3(scale, scale, scale * 2.0f), 3.0f, 3.0f),
			m_state(Patrol)
		{
			m_modelMat.affineTransformation(
				Vec3(1.0f, 1.0f, 0.5f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}
		
		virtual ~Bird() {}

		void OnCreate() override;

		void OnUpdate() override;
	};
}