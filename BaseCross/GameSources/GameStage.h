/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "BaseStage.h"
#include "GameCamera.h"

namespace basecross
{
	/*!
	@brief �Q�[�����̃X�e�[�W
	*/
	class GameStage : public BaseStage
	{
	public:

		// �X�e�[�W�X�e�[�g�萔
		enum eStageState
		{
			FadeIn,		// �t�F�[�h�C��
			StartMove,	// �J�n���̈ړ����o
			GameNow,	// �Q�[����
			Goal,		// �S�[����
			DeathDrop,	// ���S���̗���
			Death,		// ���S����
			Select,		// �I����
			Metal,		// ���^���E�B���h�E
			Reset,		// �R���e�B�j���[���Z�b�g
			FadeOut,	// �t�F�[�h�A�E�g
		};

		// �Z���N�g�^�C�v
		enum eSelect
		{
			Continue,	// �R���e�B�j���[
			TitleBack,	// �^�C�g���ɖ߂�
		};

	private:

		float m_totalTime;		// �o�ߎ���
		bool m_currentStickX;	// �O�t���[���̃X�e�B�b�N����X

		// �X�e�[�W�X�e�[�g
		eStageState m_stageState;

		// �Z���N�g�^�C�v
		eSelect m_select;

		// �X�v���C�g
		weak_ptr<Sprite> m_fade;
		weak_ptr<Sprite> m_metalLeft;
		weak_ptr<Sprite> m_metalRight;
		weak_ptr<Sprite> m_gameOver;
		weak_ptr<Sprite> m_continue;
		weak_ptr<Sprite> m_titleBack;

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
		@brief UI�̐����֐�
		*/
		void CreateUI();

		/*!
		@brief ���S���̃t�F�[�h�X�e�[�g
		*/
		void DeathFadeState();

		/*!
		@brief �Z���N�g�X�e�[�g
		*/
		void SelectState();

		/*!
		@brief ���^���E�B���h�E�X�e�[�g
		*/
		void MetalState();

		/*!
		@brief �R���e�B�j���[���Z�b�g�X�e�[�g
		*/
		void ResetState();

		/*!
		@brief �t�F�[�h�A�E�g�X�e�[�g
		@param �t�F�[�h����
		*/
		void FadeOutState(float fadeTime);

		/*!
		@brief �X�e�[�W�̃��Z�b�g
		*/
		void ResetStage();

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W��CSV�̃t�@�C����
		*/
		GameStage(const string& stageName) :BaseStage() 
		{
			m_select = Continue;
			m_stageState = FadeIn;
			m_stagePath = stageName;
			m_totalTime = 0.0f;
			m_isClear = false;
			m_currentStickX = false;
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