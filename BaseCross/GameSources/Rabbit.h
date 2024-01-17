#pragma once
#include "stdafx.h"
#include "Enemy.h"

namespace basecross
{
	class Rabbit : public Enemy
	{
	public:

		enum eType
		{
			Normal,
			Wall,
		};

		enum eState
		{
			Patrol,
			Seek,
			LostSight,
			CannonJump,
			Death,
		};

	private:

		eType m_type;
		eState m_state;
		const Vec2 m_jumpVelo;
		Vec3 m_currentTargetPos;
		vector<Vec3> m_aliveBlockPos;		
		weak_ptr<Cannon> m_activeCannon;
		weak_ptr<Billboard> m_discovered;
		vector<vector<Vec3>> m_jumpTargetPos;

		int m_lostJumpCount;
		float m_dir;
		float m_gravity;
		bool m_isAir;
		bool m_isDeath;
		bool m_isCannon;

	public:

		Rabbit(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale, eType type
		) :
			Enemy(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f, XM_PI, 0.0f), Vec3(scale), 3.0f, 5.0f, 15.0f),
			m_type(type),
			m_jumpVelo(1.0f, -2.0f)
		{
			m_velocity = Vec2(-1.0f, 0.0f);
			m_state = Patrol;
			m_isAir = true;
			m_isCannon = false;
			m_dir = -1.0f;
			m_gravity = -5.0f;
			m_lostJumpCount = 0;
			m_currentTargetPos.zero();
			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);

			m_jumpTargetPos = {
				{
					Vec3(-2.0f, 0.0f, 0.0f),
					Vec3(-2.5f, 1.5f, 0.0f),
					Vec3(-2.5f, 2.5f, 0.0f),
					Vec3(-1.5f, 2.5f, 0.0f),
					Vec3(-2.5f, -1.5f, 0.0f),
					Vec3(-1.0f, 1.0f, 0.0f),
				},
				{
					Vec3(2.0f, 0.0f, 0.0f),
					Vec3(2.5f, 1.5f, 0.0f),
					Vec3(2.5f, 2.5f, 0.0f),
					Vec3(1.5f, 2.5f, 0.0f),
					Vec3(2.5f, -1.5f, 0.0f),
					Vec3(1.0f, 1.0f, 0.0f),
				},
			};
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Rabbit() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;
		
		/*!
		@brief �j������鎞�ɌĂяo�����֐�
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

		void MoveRabbit();

		void MoveReduction();

		void PatrolState();

		void SeekState();

		void LostState();

		void CannonState();

		void DeathState();

		bool SearchPlayer();

		void JumpRabbit();

		void PlayerTargetJump(const Vec3& targetPos);

		void BlockTargetJump(const Vec3& targetPos);

		void StartJumpSE();

		void SetMoveValue(const Vec2& velocity, float acsel) override;

		void CollisiontPerformance();

		const vector<Vec3> GetHitBlockPos(const Vec3& targetPos);

		const Vec3 GetNearPosition(const vector<Vec3>& posVec) const
		{
			Vec3 temp;
			Vec3 pos = GetPosition();

			if (!posVec.empty())
			{
				temp = posVec.at(0);
				for (const auto& alive : posVec)
				{
					float lengthA = (temp - pos).length();
					float lengthB = (alive - pos).length();

					if (lengthA > lengthB)
					{
						temp = alive;
					}
				}
			}

			return temp;
		}

		const eState& GetState() const
		{
			return m_state;
		}

		void SetState(const eState& state)
		{
			m_state = state;
		}

		bool GetIsDeath() const
		{
			return m_isDeath;
		}
	};
}