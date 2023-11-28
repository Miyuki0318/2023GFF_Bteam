/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public BaseStage 
	{
		/*!
		@brief ���\�[�X�ǂݍ��݊֐�
		*/
		void CreateResourses() override;

		/*!
		@brief �r���[�ƃ��C�g�̐����֐�
		*/
		void CreateViewLight() override;

		/*!
		@brief BGM�̍Đ��֐�
		*/
		void CreateBGM() override;

		/*!
		@brief �v���C���[�̐����֐�
		*/
		void CreatePlayer() override;

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameStage() :BaseStage() {}

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
	};
}