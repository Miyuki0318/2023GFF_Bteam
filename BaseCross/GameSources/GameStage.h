/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage 
	{
		// �r���[
		shared_ptr<SingleView> m_gameView;

		// BGM
		weak_ptr<SoundItem> m_bgm;

		// ���\�[�X�̓ǂݍ���
		void CreateResourses();

		// �r���[�̍쐬
		void CreateViewLight();

		// BGM�̍Đ�
		void CreateBGM();

		// �n�ʂ̍쐬
		void CreatePlayer();

		// �X�e�[�W�̍쐬
		void CreateStage();

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameStage() :Stage() {}

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
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;

		/*!
		@brief �Q�[���r���[�擾�֐�
		@return const shared_ptr<SingleView>
		*/
		const shared_ptr<SingleView>& GetGameView() const
		{
			return m_gameView;
		}
	};
}