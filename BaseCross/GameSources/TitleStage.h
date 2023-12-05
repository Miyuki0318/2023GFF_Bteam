#pragma once
#include "stdafx.h"
#include "BaseStage.h"
#include "DebugObject.h"

namespace basecross
{
	class TitleStage : public BaseStage
	{
	public:

		// �X�e�[�W�X�e�[�g�萔
		enum eStageState
		{
			FadeIn,
			PushButton,
			BackFadeOut,
			StartMove,
			ModeSelect,
			CannonStanby,
			FadeOut,
		};

	private:

		// �X�e�[�W�X�e�[�g
		eStageState m_stageState;

		// �v���C���[
		weak_ptr<DebugObject> m_player;

		// �X�v���C�g
		weak_ptr<Sprite> m_fade;
		weak_ptr<Sprite> m_titleLogo;
		weak_ptr<Sprite> m_metalLeft;
		weak_ptr<Sprite> m_metalRight;
		weak_ptr<Sprite> m_pushButton;

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

		void BackFadeState();

		void PushButtonState();

		void StartMoveState();

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