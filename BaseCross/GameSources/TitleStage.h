#pragma once
#include "stdafx.h"
#include "BaseStage.h"

namespace basecross
{
	class TitleStage : public BaseStage
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

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		TitleStage() : BaseStage() {}

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
	};
}