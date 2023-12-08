/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"
#include "GameCamera.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public BaseStage 
	{
	public:

		// �X�e�[�W�X�e�[�g�萔
		enum eStageState
		{
			FadeIn,
			StartMove,
			GameNow,
			Goal,
			DeathDrop,
			Death,
			Continue,
			FadeOut,
		};

	private:

		// �X�e�[�W�X�e�[�g
		eStageState m_stageState;

		// �X�v���C�g
		weak_ptr<Sprite> m_fade;
		weak_ptr<Sprite> m_metalLeft;
		weak_ptr<Sprite> m_metalRight;

		// �X�e�[�W�p�X
		string m_stagePath;

		// �N���A�������̐^�U
		bool m_isClear;

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
		@brief ���S���̃t�F�[�h�X�e�[�g
		*/
		void DeathFadeState();

		/*!
		@brief �t�F�[�h�A�E�g�X�e�[�g
		@param �t�F�[�h����
		*/
		void FadeOutState(float fadeTime);

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameStage(const string& stageName) :BaseStage() 
		{
			m_stageState = FadeIn;
			m_stagePath = stageName;
			m_isClear = false;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GameStage() {}

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
		@brief �Q�[���J�����̎擾�֐�
		@return dynamic_pointer_cast<GameCamera>(m_gameView->GetCamera())
		*/
		const shared_ptr<GameCamera> GetGameCamera() const
		{
			return dynamic_pointer_cast<GameCamera>(m_gameView->GetCamera());
		}
	};
}