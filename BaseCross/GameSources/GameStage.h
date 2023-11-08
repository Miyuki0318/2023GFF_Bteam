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
		vector<vector<string>> data;

		// �r���[
		shared_ptr<SingleView> m_gameView;

		// ���\�[�X�̓ǂݍ���
		void CreateResourses();

		// �r���[�̍쐬
		void CreateViewLight();

		// �n�ʂ̍쐬
		void CreatePlayer();

		void CreateStage();

	public:

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