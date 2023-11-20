#pragma once
#include "stdafx.h"
#include "Player.h"

namespace basecross
{
	class StagingPlayer : public Player
	{
	public:

		StagingPlayer(const shared_ptr<Stage>& stagePtr) :
			Player(stagePtr)
		{

		}

		virtual ~StagingPlayer() {}

		void OnCreate() override;

		void OnUpdate() override;
	};
}