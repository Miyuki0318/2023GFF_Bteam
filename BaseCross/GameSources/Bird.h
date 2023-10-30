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

	public:

		Bird(const shared_ptr<Stage>& stagePtr) :
			Enemy(stagePtr)
		{
			m_state = Patrol;
		}
		
		virtual ~Bird() {}

		void OnCreate() override;

		void OnUpdate() override;
	};
}