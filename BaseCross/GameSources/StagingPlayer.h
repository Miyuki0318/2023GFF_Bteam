#pragma once
#include "stdafx.h"
#include "Player.h"

namespace basecross
{
	class StagingPlayer : public Player
	{
	public:

		StagingPlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position)
		{
		}

		virtual ~StagingPlayer() {}

		/*!
		@brief Aƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚È‚­‚È‚Á‚½‚ÉŒÄ‚Ño‚³‚ê‚éŠÖ”
		*/
		void OnReleaseA() override;

		void RotateAligment() override;
	};
}