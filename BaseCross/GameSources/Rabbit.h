#pragma once
#include "stdafx.h"
#include "Enemy.h"

namespace basecross
{
	class Rabbit : public Enemy
	{
		enum eState
		{
			Patrol,
			Seek,
			LostSight,
		};

		eState m_state;
		Vec3 m_currentTargetPos;
		vector<Vec3> m_aliveBlockPos;
		vector<vector<Vec3>> m_jumpTargetPos;
		const Vec2 m_jumpVelo;

		bool m_isAir;
		float m_dir;
		float m_gravity;

		weak_ptr<CubeObject> m_debug;
		weak_ptr<CubeObject> m_search;

	public:

		Rabbit(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			Enemy(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), 3.0f, 3.0f, 15.0f),
			m_jumpVelo(1.0f, -2.0f)
		{
			m_state = Patrol;
			m_isAir = true;
			m_dir = -1.0f;
			m_gravity = -5.0f;
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
		@brief �Փ˂����u�ԂɌĂяo�����֐�
		*/
		void OnCollisionEnter(const CollisionPair& Pair) override;

		/*!
		@brief �Փ˂��Ă���ԌĂяo�����֐�
		*/
		void OnCollisionExcute(const CollisionPair& Pair) override;

		/*!
		@brief �Փ˂��Ă���ԌĂяo�����֐�
		*/
		void OnCollisionExit(const CollisionPair& Pair) override;

		void MoveRabbit();

		void MoveReduction();

		void PatrolState();

		void SeekState();

		void LostState();

		void CannonState();

		bool SearchPlayer();

		void JumpRabbit();

		void PlayerTargetJump(const Vec3& targetPos);

		void BlockTargetJump(const Vec3& targetPos);

		const vector<Vec3> GetHitBlockPos(const Vec3& targetPos);

		const Vec3 GetAlivePosition() const
		{
			Vec3 temp;
			Vec3 pos = GetPosition();

			if (!m_aliveBlockPos.empty())
			{
				temp = m_aliveBlockPos.at(0);
				for (const auto& alive : m_aliveBlockPos)
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
	};
}