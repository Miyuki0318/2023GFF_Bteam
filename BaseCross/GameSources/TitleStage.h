/*!
@file TitleStage.h
@brief �^�C�g���p�X�e�[�W
*/

#pragma once
#include "BaseStage.h"
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �^�C�g���p�̃X�e�[�W
	*/
	class TitleStage : public BaseStage
	{
	public:

		// �X�e�[�W�X�e�[�g�萔
		enum eStageState
		{
			FadeIn,		  // �t�F�[�h�C��
			PushButton,	  // �v�b�V���{�^��
			MetalFadeOut, // ���^���E�B���h�E
			StartMove,	  // �J�n���̈ړ����o
			ModeSelect,   // ��Փx�I��
			CannonStanby, // ��C�ҋ@
			FadeOut,      // �t�F�[�h�A�E�g
		};

	private:

		// �X�e�[�W�X�e�[�g
		eStageState m_stageState;

		// �v���C���[
		weak_ptr<TemplateObject> m_player;

		// �X�v���C�g
		weak_ptr<Sprite> m_fade;		// �t�F�[�h�p�X�v���C�g
		weak_ptr<Sprite> m_titleLogo;	// �^�C�g�����S
		weak_ptr<Sprite> m_metalLeft;	// ���^���E�B���h�E��
		weak_ptr<Sprite> m_metalRight;	// ���^���E�B���h�E�E
		weak_ptr<Sprite> m_pushButton;	// �v�b�V���{�^��

		/*!
		@brief ���\�[�X�ǂݍ��݊֐�
		*/
		void CreateResourses() override;

		/*!
		@brief �r���[�ƃ��C�g�̐����֐�
		*/
		void CreateViewLight() override;

		/*!
		@brief �r���[�ƃ��C�g�̐����֐�
		*/
		void CreateBackGround();

		/*!
		@brief �v���C���[�̐����֐�
		*/
		void CreatePlayer() override;

		/*!
		@brief �X�v���C�g�̐����֐�
		*/
		void CreateSprites();

		/*!
		@brief ���^���E�B���h�E�X�e�[�g
		*/
		void MetalFadeState();

		/*!
		@brief �v�b�V���{�^���X�e�[�g
		*/
		void PushButtonState();

		/*!
		@brief �J�n���̈ړ����o�X�e�[�g
		*/
		void StartMoveState();

		/*!
		@brief �t�F�[�h�A�E�g�X�e�[�g
		*/
		void FadeOutState();

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		TitleStage() : BaseStage() 
		{
			m_stageState = eStageState::FadeIn;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~TitleStage() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief �X�e�[�W�X�e�[�g�ݒ�֐�
		@param eStageState
		*/
		void SetStageState(const eStageState& state)
		{
			m_stageState = state;
		}

		/*!
		@brief �X�e�[�W�X�e�[�g�擾�֐�
		@return m_stageState
		*/
		const eStageState& GetStageState() const
		{
			return m_stageState;
		}
	};
}