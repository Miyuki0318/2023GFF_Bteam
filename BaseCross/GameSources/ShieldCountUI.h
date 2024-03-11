/*!
@file ShieldCountUI.h
@brief �V�[���h����UI
*/

#pragma once
#include "Sprite.h"
#include "Player.h"

namespace basecross 
{
	/*!
	@brief �V�[���h��UI
	*/
	class ShieldCountUI: public Sprite
	{
		weak_ptr<Player> m_player; // �v���C���[�̃|�C���^
		weak_ptr<Sprite> m_gauge;  // �V�[���h�Q�[�W
		weak_ptr<Sprite> m_ring;   // �����O�擾���Q�[�W
		weak_ptr<Sprite> m_shield; // �V�[���h�A�C�R��
		weak_ptr<Sprite> m_circle; // �V�[���h�A�C�R���G�t�F�N�g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �X�P�[��
		@param �|�W�V����
		*/
		ShieldCountUI(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position
		) :
			Sprite(stagePtr, L"SGAUGE_TX", scale, position)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~ShieldCountUI() {}

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