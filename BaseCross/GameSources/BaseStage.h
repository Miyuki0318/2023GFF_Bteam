#pragma once
#include "stdafx.h"

namespace basecross
{
	class BaseStage : public Stage
	{
	protected:

		// �r���[
		shared_ptr<SingleView> m_gameView;

		// BGM
		weak_ptr<SoundItem> m_bgm;

		/*!
		@brief ���\�[�X�ǂݍ��݊֐�
		*/
		virtual void CreateResourses();

		/*!
		@brief �r���[�ƃ��C�g�̐����֐�
		*/
		virtual void CreateViewLight();

		/*!
		@brief BGM�̍Đ��֐�
		*/
		virtual void CreateBGM();

		/*!
		@brief �v���C���[�̐����֐�
		*/
		virtual void CreatePlayer();

		/*!
		@brief �X�e�[�W�u���b�N�ƃM�~�b�N�̐����֐�
		*/
		void CreateStage(const string& fileName);
		void CreateInstanceBlock(const string& fileName);

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		BaseStage() :Stage() {}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~BaseStage() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnDestroy() override;

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