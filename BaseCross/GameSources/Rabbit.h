#pragma once
#include "stdafx.h"
#include "Enemy.h"

namespace basecross
{
	class Rabbit : public Enemy
	{
	public:

		enum eState
		{
			Patrol,
			Seek,
			LostSight,
			CannonJump,
			Death,
		};

	private:

		eState m_state;
		const Vec2 m_jumpVelo;
		Vec3 m_currentTargetPos;
		vector<Vec3> m_aliveBlockPos;		
		weak_ptr<Cannon> m_activeCannon;
		vector<vector<Vec3>> m_jumpTargetPos;

		int m_lostJumpCount;
		float m_dir;
		float m_gravity;
		bool m_isAir;
		bool m_isDeath;
		bool m_isCannon;

	public:

		Rabbit(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			Enemy(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), 3.0f, 5.0f, 15.0f),
			m_jumpVelo(1.0f, -2.0f)
		{
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
		@brief デストラクタ
		*/
		virtual ~Rabbit() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 衝突した瞬間に呼び出される関数
		*/
		void OnCollisionEnter(const CollisionPair& Pair) override;

		/*!
		@brief 衝突している間呼び出される関数
		*/
		void OnCollisionExcute(const CollisionPair& Pair) override;

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
	};
}