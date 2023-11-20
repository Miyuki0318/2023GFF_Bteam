#pragma once
#include "stdafx.h"

namespace basecross
{
	class TitleStage : public Stage
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
		void CreateInstanceBlock();

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		TitleStage() : Stage() {}

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
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;
	};
}