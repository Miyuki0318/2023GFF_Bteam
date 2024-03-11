/*!
@file TitlePlayer.h
@brief �^�C�g����ʗp�̃v���C���[
*/

#pragma once
#include "Player.h"

namespace basecross
{
	/*!
	@brief �^�C�g����ʗp�̃v���C���[
	*/
	class TitlePlayer : public Player
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		*/
		TitlePlayer(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			Player(stagePtr, position)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~TitlePlayer() {}

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief A�{�^����������Ȃ��Ȃ������ɌĂяo�����֐�
		*/
		void OnRushA() override;

		/*!
		@brief �G�A�V���b�N�̋O���`��֐�
		*/
		void UpdateArrow() override;

		/*!
		@brief ��C�ҋ@�֐�
		*/
		void CannonStandby(float acsel) override;
	};
}