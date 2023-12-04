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
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief A�{�^����������Ȃ��Ȃ������ɌĂяo�����֐�
		*/
		void OnReleaseA() override;

		/*!
		@brief �G�A�V���b�N�̋O���`��֐�
		*/
		void RotateAligment() override;

		/*!
		@brief ��C�ҋ@�֐�
		*/
		virtual void CannonStandby(float acsel);
	};
}