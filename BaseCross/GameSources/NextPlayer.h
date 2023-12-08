#pragma once
#include "stdafx.h"
#include "Player.h"
#include "NextStage.h"

namespace basecross
{
	class NextPlayer : public Player
	{
		float m_moveAcsel;

	public:

		NextPlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position)
		{
			m_acsel = 5.0f;
			m_moveAcsel = 5.0f;
			m_velocity = Vec2(-1.0f, 0.0f);
		}

		virtual ~NextPlayer() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief A�{�^����������Ȃ��Ȃ������ɌĂяo�����֐�
		*/
		void OnReleaseA() override;

		/*!
		@brief �v���C���[�̉�]�֐�
		*/
		void RotatePlayer() override;

		/*!
		@brief ���S���̐ݒ�֐�
		*/
		void DeathSetup() override;

		/*!
		@brief ���S���̗����֐�
		*/
		void DeathDrop() override;


		void SelectState();

		void NextStageMove();

		void UpdateParticle(const Vec2& velo);
	};
}