#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Rabbit::OnCreate()
	{
		m_ptrTrans = GetComponent<Transform>();
		SetPosition(m_position);
		SetRotation(m_rotation);
		SetScale(m_scale);

		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"RABBIT");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"JUMP", 0, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"JUMP");

		auto ptrColl = AddComponent<CollisionObb>();
		m_targetObj = GetStage()->GetSharedGameObject<DebugObject>(L"Player");

		AddTag(L"Rabbit");
	}

	void Rabbit::OnUpdate()
	{
		if (m_type == Normal)
		{
			// �X�e�[�W�ƃX�e�[�W�X�e�[�g�̎擾
			const auto& stage = GetTypeStage<GameStage>();
			const auto& state = stage->GetStageState();

			if (Utility::GetBetween(state, GameStage::GameNow, GameStage::Death))
			{
				MoveRabbit();
				MoveReduction();
				m_ptrDraw->UpdateAnimation(DELTA_TIME / 2.0f);

				bool jumpPossible = m_ptrDraw->IsTargetAnimeEnd() && !m_isAir && !m_isDeath;
				if (jumpPossible)
				{
					m_ptrDraw->ChangeCurrentAnimation(L"JUMP");
					switch (m_state)
					{
					case Patrol:
						PatrolState();
						break;

					case Seek:
						SeekState();
						break;

					case LostSight:
						LostState();
						break;

					default:
						break;
					}
				}
				if (m_state == CannonJump && !m_isDeath) CannonState();
				if (m_state == Death) DeathState();
			}
		}
		if (m_type == Wall)
		{
			if (!m_isDeath) CollisiontPerformance();
			if (m_state == Death)
			{
				DeathState();
				MoveRabbit();
				MoveReduction();
			}
		}
		m_aliveBlockPos.clear();
	}

	void Rabbit::OnCollisionEnter(const CollisionPair& Pair)
	{
		if (m_type != Normal) return;

		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block"))
		{
			if (m_isDeath) GetStage()->RemoveGameObject<Rabbit>(GetThis<Rabbit>());

			// �u���b�N�̃p�����[�^���擾
			const auto& cube = dynamic_pointer_cast<CubeObject>(other);
			Vec3 objPos = cube->GetSlopePos();
			Vec3 helf = cube->GetScale() / 2.0f;
			m_aliveBlockPos.push_back(objPos);
			m_isCannon = false;

			if (CollHitUpper(hitPoint, objPos, helf))
			{
				if (m_velocity.y > 0.0f)
				{
					// �����x���������傫��������
					if (m_acsel > 2.5f || m_velocity.y > 2.5f)
					{
						// �ړ��ʂ𔽓]�����A�����ɂ���
						m_velocity.y *= -0.5f;
					}
				}
			}

			if (CollHitUnder(hitPoint, objPos, helf))
			{
				if (m_velocity.y < 0.0f)
				{
					m_velocity.y *= -1.0f;
				}
			}

			if (CollHitLeft(hitPoint, objPos, helf) || CollHitRight(hitPoint, objPos, helf))
			{
				const auto& angle = cube->GetAngleType();
				if (angle == CubeObject::Normal)
				{
					// �ړ��ʂ𔼌������]������
					m_velocity.x *= -0.5f;
					m_dir *= -1.0f;
				}
			}
		}

		if (other->FindTag(L"Death"))
		{
			m_state = Death;
		}

		if (other->FindTag(L"Rabbit"))
		{
			// �ړ��ʂ𔼌������]������
			m_velocity.x *= -1.0f;
			m_dir *= -1.0f;
		}

		if (other->FindTag(L"Cannon"))
		{
			const auto& ptr = dynamic_pointer_cast<Cannon>(other);
			if (!ptr->GetFire() && !m_isCannon)
			{
				ptr->OnFire();
				m_activeCannon = ptr;
				m_isAir = true;
				m_isCannon = true;
				m_velocity = Vec2(0.0f);
				m_acsel = 1.0f;
				m_state = CannonJump;
				SetPosition(ptr->GetPosition());
				GetComponent<CollisionObb>()->SetUpdateActive(false);
			}
		}
	}

	void Rabbit::OnCollisionExcute(const CollisionPair& Pair)
	{
		if (m_type != Normal) return;

		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block"))
		{
			// �u���b�N�̃p�����[�^���擾
			const auto& cube = dynamic_pointer_cast<CubeObject>(other);
			Vec3 objPos = cube->GetSlopePos();
			Vec3 helf = cube->GetScale() / 2.0f;
			m_aliveBlockPos.push_back(objPos);

			if (m_velocity.y > 0.0f)
			{
				m_acsel = 1.0f;
				m_isAir = false;

				const auto& angle = cube->GetAngleType();
				if (angle == CubeObject::Normal)
				{
					SetPosition(GetPosition().x, objPos.y + helf.y + (m_scale.y / 2.0f) + 0.05f);
				}
			}
		}
	}

	void Rabbit::MoveRabbit()
	{
		// �����̕ύX
		if (m_velocity.x != 0.0f)
		{
			m_dir = m_velocity.x > 0.0f ? 1.0f : -1.0f;
			float rotY = (m_velocity.x > 0.0f) * XM_PI;
			SetRotation(Vec3(0.0f, rotY, 0.0f));
		}

		// ���݂̍��W���擾
		m_position = m_ptrTrans->GetPosition();

		// �|�W�V�����Ɉړ��x�N�g���Ƒ��x�Ɖ����x�ƃf���^�^�C���Ŋ|�����������Z
		m_position.x += -m_velocity.x * m_speed * m_acsel * DELTA_TIME;
		m_position.y += -m_velocity.y * m_speed * m_acsel * DELTA_TIME;

		// ���W�̍X�V
		m_ptrTrans->SetPosition(m_position);
	}

	void Rabbit::MoveReduction()
	{
		const float deltaTime = DELTA_TIME;

		// �󒆂Ȃ�
		if (m_isAir)
		{
			// �����x��1.0���傫������������x���̓�{���f���^�^�C���Ŋ|�������Ō��Z�A������������1.0�ɏC��
			m_acsel > 1.0f ? m_acsel -= deltaTime * (m_acsel * 2.0f) : m_acsel = 1.0f;

			// Y���ړ��x�N�g�����d�͂ƃf���^�^�C���Ŋ|�������Ō��Z
			m_velocity.y -= m_gravity * deltaTime;
		}
		else
		{
			// �����ʂ�X���ړ��x�N�g���̐����Ō��߂�
			float decrease = deltaTime * m_maxAcsel;
			decrease *= m_velocity.x > 0.0f ? 1.0f : -1.0f;

			// X���ړ��x�N�g����0.01���傫��������(������킸)
			if (m_velocity.x > 0.1f || m_velocity.x < -0.1f)
			{
				// X���ړ��x�N�g���������ʂŌ��Z
				m_velocity.x -= decrease;
			}
			else
			{
				// 0.01��菬����������0.0�ŏC��
				m_velocity.x = 0.0f;
			}

			if (m_velocity.y > 0.25f)
			{
				m_velocity.y -= deltaTime;
			}
			else
			{
				m_velocity.y = 0.25f;
			}
		}
	}

	void Rabbit::PatrolState()
	{
		if (m_targetObj.lock())
		{
			if (SearchPlayer())
			{
				m_state = Rabbit::Seek;
				return;
			}

			JumpRabbit();
		}
	}

	void Rabbit::SeekState()
	{
		// �T�[�`���|����
		SearchPlayer();

		// ���W�̎擾
		Vec3 targetPos = m_targetObj.lock()->GetPosition();

		// �Փˍ��W�̃o�b�t�@�z��
		vector<Vec3> hitPos = GetHitBlockPos(targetPos);

		// �Փ˂������W����������
		if (!hitPos.empty())
		{
			BlockTargetJump(targetPos);
		}
		else
		{
			PlayerTargetJump(targetPos + Vec3(0.0f, -1.0f, 0.0f));
		}
	}

	void Rabbit::LostState()
	{
		// ���W�̎擾
		Vec3 targetPos = m_currentTargetPos;
		const Vec3& pos = GetPosition();
		if ((targetPos - pos).length() <= m_range || m_lostJumpCount > 5)
		{
			if (SearchPlayer())
			{
				m_state = Seek;
				m_lostJumpCount = 0;
				return;
			}
			else
			{
				m_state = Patrol;
				m_lostJumpCount = 0;
				return;
			}
		}

		// �Փˍ��W�̃o�b�t�@�z��
		vector<Vec3> hitPos = GetHitBlockPos(targetPos);

		// �Փ˂������W����������
		if (!hitPos.empty())
		{
			BlockTargetJump(targetPos);
			m_lostJumpCount++;
		}
		else
		{
			PlayerTargetJump(targetPos);
			m_lostJumpCount++;
		}
	}

	void Rabbit::CannonState()
	{
		if (m_activeCannon.lock())
		{
			const float& fireTime = m_activeCannon.lock()->GetFireTime();
			const auto& drawPtr = m_activeCannon.lock()->GetComponent<PNTBoneModelDraw>();
			if (drawPtr->GetCurrentAnimationTime() > fireTime)
			{
				m_isAir = true;
				m_state = Patrol;
				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				Vec2 velo = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;
				SetMoveValue(velo, 10.0f);
				StartSE(L"CANNON_SE", 0.75f);
				GetComponent<CollisionObb>()->SetUpdateActive(true);
				m_activeCannon.reset();
			}
			else
			{
				m_velocity = Vec2(0.0f);
				SetPosition(m_activeCannon.lock()->GetPosition());
			}
		}
	}

	void Rabbit::DeathState()
	{
		if (!m_isDeath) 
		{
			m_isDeath = true;
			RemoveComponent<CollisionObb>();
			RemoveTag(L"Rabbit");
			SetMoveValue(Vec2(Utility::RangeRand(2.0f, 0.0f) - 1.0f, -1.5f), m_maxAcsel);
		}
		else
		{
			SetRotation(GetRotation() + Vec3(0.0f, 0.0f, DELTA_TIME * m_dir * 100.0f));
		}
	}

	bool Rabbit::SearchPlayer()
	{
		if (m_targetObj.lock())
		{
			Vec3 targetPos = m_targetObj.lock()->GetPosition();
			Vec3 pos = GetPosition();

			float length = (targetPos - pos).length();
			if (length <= m_range)
			{
				if (GetHitBlockPos(targetPos).empty())
				{
					targetPos.y = 0.0f;
					pos.y = 0.0f;

					Vec3 toTarget = -(targetPos - pos);
					toTarget.normalize();

					if (m_dir == toTarget.x)
					{
						StartSE(L"RABBIT_SEARCH_SE", 0.5f);
						return true;
					}
				}
				return false;
			}
			else
			{
				if (m_state == Seek)
				{
					m_state = LostSight;
					m_currentTargetPos = m_targetObj.lock()->GetPosition();
				}
				return false;
			}
		}
		return false;
	}

	void Rabbit::JumpRabbit()
	{
		Vec2 jumpVelo = m_jumpVelo;
		Vec3 alive = GetNearPosition(m_aliveBlockPos);

		if (m_dir > 0.0f)
		{
			bool check = false;
			for (const auto& jump : m_jumpTargetPos.at(0))
			{
				check = BlockCheck(alive + jump);
				if (check) break;
			}
			if (!check) m_dir *= -1.0f;

			jumpVelo.x *= m_dir;
			m_isAir = true;
			SetMoveValue(jumpVelo, m_maxAcsel);
		}
		else
		{
			bool check = false;
			for (const auto& jump : m_jumpTargetPos.at(1))
			{
				check = BlockCheck(alive + jump);
				if (check) break;
			}
			if (!check) m_dir *= -1.0f;

			jumpVelo.x *= m_dir;
			m_isAir = true;
			SetMoveValue(jumpVelo, m_maxAcsel);
		}
	}
	
	void Rabbit::PlayerTargetJump(const Vec3& targetPos)
	{
		// �p�x����ړ��ʂ�ݒ�
		const Vec3& pos = GetPosition();
		float rad = atan2f(targetPos.y - pos.y, targetPos.x - pos.x);
		Vec2 velo = Vec2(cos(rad), sin(rad)).normalize();
		velo *= -3.5f;
		velo.y += -1.0f;
		m_isAir = true;
		SetMoveValue(velo, m_maxAcsel);
	}

	void Rabbit::BlockTargetJump(const Vec3& targetPos)
	{
		// �Őڋߍ��W�̎擾
		Vec3 nearPos = GetNearPosition(GetHitBlockPos(targetPos));

		// �Փ˂����Őڋ߂̃u���b�N�̏�Ƀu���b�N�����邩���`�F�b�N
		// �u���b�N���������ꍇ����ɂ��̏�ɂ��邩���`�F�b�N
		// �������A5�ȏ゠�����瓞�B�ł��Ȃ��Ɣ��f���ďI��
		int loopCount = 0;
		const Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
		while (BlockCheck(nearPos + up))
		{
			nearPos += up;
			loopCount++;
			if (loopCount > 7)
			{
				m_state = Patrol;
				return;
			}
		}

		PlayerTargetJump(nearPos);
	}

	void Rabbit::StartJumpSE()
	{
		const Vec3& pos = GetPosition();
		const Vec3& playerPos = m_targetObj.lock()->GetPosition();

		float length = (playerPos - pos).length();
		if (length <= m_range)
		{
			float volume = (m_range - length) / 10.0f;
			StartSE(L"RABBIT_JUMP_SE", volume);
		}
	}

	void Rabbit::SetMoveValue(const Vec2& velocity, float acsel)
	{
		Enemy::SetMoveValue(velocity, acsel);
		StartJumpSE();
	}

	const vector<Vec3> Rabbit::GetHitBlockPos(const Vec3& targetPos)
	{
		// �o�b�t�@�z��
		vector<Vec3> posVec;

		// ���W�̎擾
		Vec3 pos = GetPosition();

		// �u���b�N�I�u�W�F�N�g�O���[�v�̎擾
		const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Active")->GetGroupVector();
		for (const auto& gameObj : blockVec)
		{
			// �I�u�W�F�N�g�������A�܂��͔�A�N�e�B�u���̃`�F�b�N
			if (!gameObj.lock()) continue;
			if (!gameObj.lock()->GetUpdateActive()) continue;

			// OBB���擾���AOBB�Ɛ����ŏՓ˔��������
			const auto& objPtr = gameObj.lock();
			const auto& collObb = objPtr->GetComponent<CollisionObb>();
			if (!collObb) continue;

			// �Փ˂�����o�b�t�@�ɍ��W��ǉ�
			if (HitTest::SEGMENT_OBB(pos, targetPos, collObb->GetObb()))
			{
				const auto& block = dynamic_pointer_cast<CubeObject>(objPtr);
				posVec.push_back(block->GetSlopePos());
			}
		}

		return posVec;
	}

	void Rabbit::CollisiontPerformance()
	{
		bool achieve = false;
		if (m_targetObj.lock())
		{
			const auto& target = m_targetObj.lock();
			if (target)
			{
				Vec3 targetPos = target->GetPosition();
				float length = (targetPos - m_position).length();
				achieve = (length <= 4.0f);
			}
		}

		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetUpdateActive(achieve);
	}
}