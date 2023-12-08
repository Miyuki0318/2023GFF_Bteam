#pragma once
#include "stdafx.h"
#include "Player.h"

namespace basecross
{
	class TitlePlayer : public Player
	{
	public:

		TitlePlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position)
		{
		}

		virtual ~TitlePlayer() {}

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

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
		void CannonStandby(float acsel) override;
	};
}