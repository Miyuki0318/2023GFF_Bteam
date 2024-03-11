/*!
@file WallRabbit.h
@brief �ǂɂȂ��Ă郁�J�E�T�M
*/

#pragma once
#include "Rabbit.h"

namespace basecross
{
	/*!
	@brief �G�L�����̕ǃE�T�M
	*/
	class WallRabbit : public Rabbit
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		*/
		WallRabbit(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			Rabbit(stagePtr, position, scale)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~WallRabbit() {}

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief ���S���̃X�e�[�g�֐�
		*/
		void DeathState();

		/*!
		@brief �R���W�����̃p�t�H�[�}���X�Ǘ��֐�
		*/
		void CollisiontPerformance();
	};
}