/*!
@file JumpRabbit.h
@brief �W�����v���郁�J�E�T�M
*/

#pragma once
#include "Rabbit.h"

namespace basecross
{
	/*!
	@brief �G�L�����̃W�����v�E�T�M
	*/
	class JumpRabbit : public Rabbit
	{
		const array<array<Vec3, 6>, 2> m_jumpTargetPos; // �W�����v�������n��

		weak_ptr<Cannon> m_activeCannon;  // ��C�̃|�C���^
		weak_ptr<Billboard> m_discovered; // �������̃G�t�F�N�g

		Vec3 m_currentTargetPos; // �����������̍��W
		vector<Vec3> m_aliveBlockPos; // �ڒn���Ă�u���b�N

		int m_lostJumpCount; // �����Ȃ������̃W�����v�񐔃J�E���^

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		*/
		JumpRabbit(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			Rabbit(stagePtr, position, scale),
			m_jumpTargetPos{{
				{
					Vec3(-2.0f, 0.0f, 0.0f),
					Vec3(-2.5f, 1.5f, 0.0f),
					Vec3(-2.5f, 2.5f, 0.0f),
					Vec3(-1.5f, 2.5f, 0.0f),
					Vec3(-2.5f, -1.5f, 0.0f),
					Vec3(-1.0f, 1.0f, 0.0f)
				},
				{
					Vec3(2.0f, 0.0f, 0.0f),
					Vec3(2.5f, 1.5f, 0.0f),
					Vec3(2.5f, 2.5f, 0.0f),
					Vec3(1.5f, 2.5f, 0.0f),
					Vec3(2.5f, -1.5f, 0.0f),
					Vec3(1.0f, 1.0f, 0.0f)
				}
			}}
		{
			m_lostJumpCount = 0;
			m_currentTargetPos.zero();
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~JumpRabbit() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �j�����ꂽ���ɌĂяo�����֐�
		*/
		void OnDestroy() override;

		/*!
		@brief �Փ˂����u�ԂɌĂяo�����֐�
		*/
		void OnCollisionEnter(const CollisionPair& Pair) override;

		/*!
		@brief �Փ˂��Ă���ԌĂяo�����֐�
		*/
		void OnCollisionExcute(const CollisionPair& Pair) override;

		/*!
		@brief �Փ˂��Ă���ԌĂяo�����֐�
		@param �R���W�����y�A(�R���W�������)
		*/
		void OnCollisionExit(const CollisionPair& Pair) override;

		/*!
		@brief �E�T�M�̈ړ��֐�
		*/
		void MoveRabbit() override;

		/*!
		@brief �u���b�N�ɏՓ˂�����
		@param �u���b�N�̃|�C���^
		@param �Փˍ��W
		*/
		void BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		/*!
		@brief �u���b�N�ɏՓ˂���������
		@param �u���b�N�̃|�C���^
		@param �Փˍ��W
		*/
		void BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		/*!
		@brief �x���g�R���x�A�ɏՓ˂�����
		@param �x���g�R���x�A�̃|�C���^
		@param �Փˍ��W
		*/
		void ConvayorEnter(const shared_ptr<GameObject>& convayor, const Vec3& hitPos);

		/*!
		@brief �x���g�R���x�A�ɏՓ˂���������
		@param �x���g�R���x�A�̃|�C���^
		@param �Փˍ��W
		*/
		void ConvayorExcute(const shared_ptr<GameObject>& convayor, const Vec3& hitPos);

		/*!
		@brief �o���p�[�ɏՓ˂�����
		@param �o���p�[�̃|�C���^
		@param �Փˍ��W
		*/
		void BumperEnter(const shared_ptr<GameObject>& bumper, const Vec3& hitPos);

		/*!
		@brief ��C�ɏՓ˂�����
		@param ��C�̃|�C���^
		*/
		void CannonEnter(const shared_ptr<GameObject>& cannon);

		/*!
		@brief ���S���̃X�e�[�g�֐�
		*/
		void DeathState();

		/*!
		@brief �p�g���[���X�e�[�g�֐�
		*/
		void PatrolState();

		/*!
		@brief �ǐՎ��̃X�e�[�g�֐�
		*/
		void SeekState();

		/*!
		@brief �����Ȃ������̃X�e�[�g�֐�
		*/
		void LostState();

		/*!
		@brief ��C�ҋ@���̃X�e�[�g�֐�
		*/
		void CannonState();

		/*!
		@brief �v���C���[�T�[�`�֐�
		*/
		bool SearchPlayer();

		/*!
		@brief �W�����v�֐�
		*/
		void JumpingRabbit();

		/*!
		@brief �W�����v����SE�֐�
		*/
		void StartJumpSE();

		/*!
		@brief �v���C���[�Ɍ����ăW�����v����֐�
		@param �^�[�Q�b�g���W
		*/
		void PlayerTargetJump(const Vec3& targetPos);

		/*!
		@brief �u���b�N�Ɍ����ăW�����v����֐�
		@param �^�[�Q�b�g���W
		*/
		void BlockTargetJump(const Vec3& targetPos);

		/*!
		@brief �^�[�Q�b�g�܂ł̐����ƏՓ˂���u���b�N��Ԃ��֐�
		@param �^�[�Q�b�g���W
		@return �Փ˂����u���b�N�z��
		*/
		const vector<Vec3> GetHitBlockPos(const Vec3& targetPos);

		/*!
		@brief ���W�z��̒�����Őڋ߂�Ԃ��֐�
		@param ���W�z��
		@return �Őڋ߂̍��W
		*/
		const Vec3 GetNearPosition(const vector<Vec3>& posVec) const
		{
			Vec3 temp; // �ۑ��p�ϐ�
			Vec3 pos = GetPosition(); // ���g�̍��W

			// ���W�z�񂪋󂶂�Ȃ����
			if (!posVec.empty())
			{
				temp = posVec.at(0); // �z��̈�ԍŏ��̍��W����

				// ���W�̐����[�v
				for (const auto& alive : posVec)
				{
					// �������r
					float lengthA = (temp - pos).length();
					float lengthB = (alive - pos).length();

					// �������Z�����
					if (lengthA > lengthB)
					{
						temp = alive; // �ۑ�
					}
				}
			}

			return temp;
		}

		/*!
		@brief �ړ��ݒ�֐�
		@param �ړ���
		@param �����x
		*/
		void SetMoveValue(const Vec2& velocity, float acsel) override
		{
			// �p�����̈ړ��ݒ�֐������s
			Rabbit::SetMoveValue(velocity, acsel);

			// �W�����v����SE���Đ�
			StartJumpSE();
		}
	};
}