#pragma once
#include "stdafx.h"
#include "BaseStage.h"
#include "TemplateObject.h"

namespace basecross
{
	class NextStage : public BaseStage
	{
	public :

		// �X�e�[�W�X�e�[�g�萔
		enum eStageState
		{
			FadeIn,
			Select,
			StageMove,
			TitleBack,
			BackFade,
			FadeOut,
		};

		// �Z���N�g�^�C�v
		enum eSelect
		{
			Next,
			Back,
		};

	private:

		float m_totalTime;		// �o�ߎ���
		bool m_currentStickX;	// �O�t���[���̃X�e�B�b�N����X

		// �Z���N�g�^�C�v
		eSelect m_select;

		// �X�e�[�W�X�e�[�g
		eStageState m_stageState;

		// �X�v���C�g
		weak_ptr<Sprite> m_fade;
		weak_ptr<Sprite> m_logo;
		weak_ptr<Sprite> m_next;
		weak_ptr<Sprite> m_back;
		weak_ptr<Sprite> m_metalLeft;
		weak_ptr<Sprite> m_metalRight;

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

		void SlideBackGround();
		
		void SelectState();

		void BackFadeState();

		void FadeInState();

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