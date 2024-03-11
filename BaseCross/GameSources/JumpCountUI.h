/*!
@file JumpCountUI.h
@brief �W�����v�\��UI
*/

#pragma once
#include "Sprite.h"
#include "Player.h"

namespace basecross
{
	/*!
	@brief �W�����v��UI
	*/
	class JumpCountUI : public Sprite
	{
		weak_ptr<Player> m_player;   // �v���C���[�|�C���^
		weak_ptr<Sprite> m_gauge;    // �Q�[�W�p�X�v���C�g
		weak_ptr<Sprite> m_airShock; // �G�A�V���b�N�A�C�R���X�v���C�g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �X�P�[��
		@param �|�W�V����
		*/
		JumpCountUI(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position
		) :
			Sprite(stagePtr, L"JGAUGE_TX", scale, position)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~JumpCountUI() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief UI�̍X�V�����֐�
		*/
		void UpdateUI();
	};
}