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
		@brief A�{�^����������Ȃ��Ȃ������ɌĂяo�����֐�
		*/
		void OnReleaseA() override;

		void RotateAligment() override;
	};
}