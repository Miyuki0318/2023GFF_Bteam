#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Rabbit::OnCreate()
	{
		TemplateObject::OnCreate();

		SetPosition(m_position);
		SetRotation(m_rotation);
		SetScale(m_scale);

		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"RABBIT");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"JUMP", 0, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"JUMP");

		auto ptrColl = AddComponent<CollisionObb>();

		const auto& stage = GetStage();
		m_targetObj = stage->GetSharedGameObject<TemplateObject>(L"Player");
		m_discovered = stage->AddGameObject<Billboard>(L"DISCOVER_TX", Vec2(2.0f), Vec3(0.0f));
		m_discovered.lock()->SetDrawActive(false);

		AddTag(L"Rabbit");
	}

	void Rabbit::OnUpdate()
	{
		if (m_type == Normal)
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

	void Rabbit::OnDestroy()
	{
		m_discovered.lock()->SetDrawActive(false);

		const auto& stage = GetStage();
		stage->RemoveGameObject<Billboard>(m_discovered.lock());
		m_discovered.reset();
	}

	void Rabbit::OnCollisionEnter(const CollisionPair& Pair)
	{
		if (m_type != Normal) return;

		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block") || other->FindTag(L"Spike"))
		{
			BlockEnter(other, hitPoint);
		}

		if (other->FindTag(L"Convayor"))
		{
			ConvayorEnter(other, hitPoint);
		}

		if (other->FindTag(L"Bumper"))
		{
			BumperEnter(other, hitPoint);
		}

		if (other->FindTag(L"Death"))
		{
			m_state = Death;
			if (m_isDeath)
			{
				const auto& stage = GetStage();
				stage->RemoveGameObject<Rabbit>(GetThis<Rabbit>());
			}
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

		if (other->FindTag(L"Block") || other->FindTag(L"Spike"))
		{
			BlockExcute(other, hitPoint);
		}

		if (other->FindTag(L"Convayor"))
		{
			ConvayorExcute(other, hitPoint);
		}
	}

	void Rabbit::OnCollisionExit(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;
		if (other->FindTag(L"Block"))
		{
			const auto& cube = dynamic_pointer_cast<CubeObject>(other);
			if (cube)
			{
				const auto& type = cube->GetAngleType();
				if (type == CubeObject::Normal)
				{
					m_isAir = true;
				}
				else
				{
					if (hitPoint.y <= cube->GetPosition().y)
					{
						m_isAir = true;
					}
				}
			}
		}
	}

	void Rabbit::BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		if (m_isDeath) GetStage()->RemoveGameObject<Rabbit>(GetThis<Rabbit>());

		// �u���b�N�̃p�����[�^���擾
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;
		m_aliveBlockPos.push_back(objPos);
		m_isCannon = false;

		if (CollHitUpper(hitPos, objPos, helf))
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

		if (CollHitUnder(hitPos, objPos, helf))
		{
			if (m_velocity.y < 0.0f)
			{
				m_velocity.y *= -1.0f;
			}
		}

		if (CollHitLeft(hitPos, objPos, helf) || CollHitRight(hitPos, objPos, helf))
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

	void Rabbit::BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
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

	void Rabbit::ConvayorEnter(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		auto objTrans = convayor->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// ��Ƀu���b�N�����邩�̃`�F�b�N
			if (BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

			// �G�A�V���b�N�g�p�\�ɂ���
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;

			// �߂荞�݂��C��
			Vec3 pos = GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			SetPosition(pos);

			// �x���g�R���x�A�^�ɃL���X�g
			const auto& ptr = dynamic_pointer_cast<Convayor>(convayor);
			if (!ptr) return;

			// ���x�ƌ������擾
			const float& speed = ptr->GetConvayorSpeed();
			const auto& angle = ptr->GetRotate();
			switch (angle)
			{
			case Convayor::LeftRot:
				m_velocity.x = speed;
				break;

			case Convayor::RightRot:
				m_velocity.x = -speed;
				break;

			default:
				break;
			}

			return;
		}

		// ��ȊO����Փ˂����Ȃ�
		if (CollHitUnder(hitPos, objPos, helf) || CollHitLeft(hitPos, objPos, helf) || CollHitRight(hitPos, objPos, helf))
		{
			BlockEnter(convayor, hitPos);
		}
	}

	void Rabbit::ConvayorExcute(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		auto objTrans = convayor->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// �߂荞�݂�����
			Vec3 pos = GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			SetPosition(pos);

			// ������ւ̈ړ��𖳂���(��葱����ׂ�)
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
		}
	}

	void Rabbit::BumperEnter(const shared_ptr<GameObject>& other, const Vec3& hitPos)
	{
		const auto& bumper = dynamic_pointer_cast<Bumper>(other);
		if (bumper)
		{
			const Vec3& bumperPos = bumper->GetPosition();
			float rad = -atan2f(hitPos.y - bumperPos.y, hitPos.x - bumperPos.x);
			Vec2 velo = Vec2(-cos(rad), sin(rad)).normalize();

			m_velocity = velo * m_speed;
			m_acsel = m_maxAcsel;
			m_isAir = true;

			bumper->OnHit();
		}
	}
	void Rabbit::MoveRabbit()
	{
		// �����̕ύX
		if (!m_isDeath && m_velocity.x != 0.0f)
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

		// �G�t�F�N�g�̃r���{�[�h�\��
		m_discovered.lock()->SetPosition(m_position + Vec3(0.0f, 2.5f, 0.0f));
		m_discovered.lock()->UpdateBillboard();
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
				m_discovered.lock()->SetDrawActive(true);
				return;
			}

			m_discovered.lock()->SetDrawActive(false);
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

			if (m_type == Wall)
			{
				const auto& wallVec = GetStage()->GetSharedObjectGroup(L"Wall")->GetGroupVector();
				
				// �ׂ̃E�T�M��T���āu�ꏏ�Ɏ��́H�v���ă����w���s������
				Vec3 thisPos = GetPosition();
				Vec3 scale = GetScale();

				vector<weak_ptr<Rabbit>> rabbitVec;

				for (const auto& weakObj : wallVec)
				{
					if (!weakObj.lock()) continue;

					const auto& wall = dynamic_pointer_cast<Rabbit>(weakObj.lock());
					if (!wall) continue;
					if (wall->GetIsDeath()) continue;

					Vec3 pos = wall->GetPosition();
					if (pos == thisPos + Vec3(scale.x, 0.0f, 0.0f) || pos == thisPos - Vec3(scale.x, 0.0f, 0.0f))
					{
						rabbitVec.push_back(wall);
					}
					if (pos == thisPos + Vec3(0.0f, scale.y, 0.0f) || pos == thisPos - Vec3(0.0f, scale.y, 0.0f))
					{
						rabbitVec.push_back(wall);
					}
				}

				for (const auto& rabbit : rabbitVec)
				{
					if (rabbit.lock())
					{
						rabbit.lock()->SetState(Death);
					}
				}
			}

			RemoveComponent<CollisionObb>();
			RemoveTag(L"Rabbit");
			m_velocity = Vec2(Utility::RangeRand(2.0f, 0.0f) - 1.0f, -1.5f);
			m_acsel = m_maxAcsel;
		}
		else
		{
			SetRotation(GetRotation() + Vec3(0.0f, 0.0f, DELTA_TIME * m_dir * Utility::RangeRand(15.0f, 5.0f)));
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