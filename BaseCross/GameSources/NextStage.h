/*!
@file NextStage.h
@brief �u���̃X�e�[�W�ցv�X�e�[�W
*/

#pragma once
#include "BaseStage.h"
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �u���̃X�e�[�W�ցv�X�e�[�W
	*/
	class NextStage : public BaseStage
	{
	public :

		// �X�e�[�W�X�e�[�g�萔
		enum eStageState
		{
			FadeIn,		// �t�F�[�h�C��
			Select,		// �I����
			StageMove,	// �X�e�[�W�ֈړ�
			TitleBack,	// �^�C�g���ɖ߂�
			MetalFade,	// �^�C�g���ɖ߂�p�t�F�[�h
			FadeOut,	// �t�F�[�h�A�E�g
		};

		// �Z���N�g�^�C�v
		enum eSelect
		{
			Next,	// ���̃X�e�[�W��
			Back,	// �^�C�g���ɖ߂�
		};

	private:

		float m_totalTime;		// �o�ߎ���
		bool m_currentStickX;	// �O�t���[���̃X�e�B�b�N����X

		// �Z���N�g�^�C�v
		eSelect m_select;

		// �X�e�[�W�X�e�[�g
		eStageState m_stageState;

		// �X�v���C�g
		weak_ptr<Sprite> m_fade;		// �t�F�[�h�p
		weak_ptr<Sprite> m_logo;		// ���S
		weak_ptr<Sprite> m_next;		// ����
		weak_ptr<Sprite> m_back;		// �߂�
		weak_ptr<Sprite> m_metalLeft;	// ���^���E�B���h�E��
		weak_ptr<Sprite> m_metalRight;	// ���^���E�B���h�E�E

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
		@brief �w�i�X�N���[�������֐�
		*/
		void SlideBackGround();

		/*!
		@brief �Z���N�g�X�e�[�g
		*/
		void SelectState();

		/*!
		@brief �^�C�g���ɖ߂�t�F�[�h�X�e�[�g
		*/
		void MetalState();

		/*!
		@brief �t�F�[�h�C���X�e�[�g
		*/
		void FadeInState();

		/*!
		@brief �t�F�[�h�A�E�g�X�e�[�g
		*/
		void FadeOutState();

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		NextStage() : BaseStage()
		{
			m_select = Next;
			m_stageState = eStageState::FadeIn;
			m_totalTime = 0.0f;
			m_currentStickX = false;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~NextStage() {}

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

		/*!
		@brief �Z���N�g�^�C�v�擾�֐�
		@return m_select
		*/
		const eSelect& GetSelect() const
		{
			return m_select;
		}
	};
}