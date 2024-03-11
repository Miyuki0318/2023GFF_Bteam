/*!
@file NextPlayer.h
@brief ���̃X�e�[�W�։�ʗp�̃v���C���[
*/

#pragma once
#include "Player.h"
#include "NextStage.h"

namespace basecross
{
	/*!
	@brief �u���̃X�e�[�W�ցv���̃v���C���[
	*/
	class NextPlayer : public Player
	{
		const float m_moveAcsel; // �ړ����̉����x
		const float m_dropAcsel; // �������̉����x

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		NextPlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position),
			m_moveAcsel(5.0f),
			m_dropAcsel(2.5f)
		{
			m_velocity = Vec2(-1.0f, 0.0f);
		}

		/*!
		@brief �f�X�g���N�^
		*/
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
		void OnRushA() override;

		/*!
		@brief �v���C���[�̉�]�֐�
		*/
		void RotatePlayer() override;

		/*!
		@brief ���S���̐ݒ�֐�
		*/
		void DropSetup();

		/*!
		@brief ���S���̗����֐�
		*/
		void TitleDrop();

		/*!
		@brief �I�����X�e�[�g�֐�
		*/
		void SelectState();

		/*!
		@brief ���̃X�e�[�W�Ɉړ��������`�F�b�N����֐�
		*/
		void NextStageCheck();

		/*!
		@brief ���̃G�t�F�N�g�����֐�
		@param �ړ���
		*/
		void SmokeEffect(const Vec2& velo);
	};
}