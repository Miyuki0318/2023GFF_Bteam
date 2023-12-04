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
		ptrColl->SetDrawActive(true);

		const auto& stage = GetStage();
		m_debug = stage->AddGameObject<CubeObject>(Vec3(0.0f), Vec3(0.0f), Vec3(1.15f), false);
		auto debugDraw = m_debug.lock()->AddComponent<PNTStaticDraw>();
		debugDraw->SetMeshResource(L"DEFAULT_CUBE");
		debugDraw->SetEmissive(COL_RED);

		m_search = stage->AddGameObject<CubeObject>(Vec3(0.0f), Vec3(0.0f), Vec3(1.15f), false);
		auto searchDraw = m_search.lock()->AddComponent<PNTStaticDraw>();
		searchDraw->SetMeshResource(L"DEFAULT_CUBE");
		searchDraw->SetEmissive(COL_GREAN);

		m_targetObj = stage->GetSharedGameObject<DebugObject>(L"Player");
	}

	void Rabbit::OnUpdate()
	{
		m_ptrDraw->UpdateAnimation(DELTA_TIME / 2.0f);
		
		Vec3 temp;
		if (!m_aliveBlockPos.empty())
		{
			temp = m_aliveBlockPos.at(0);
		}

		Vec3 pos = GetPosition();

		for (const auto& alive : m_aliveBlockPos)
		{
			float lengthA = (temp - pos).length();
			float lengthB = (alive - pos).length();

			if (lengthA > lengthB)
			{
				temp = alive;
			}
		}

		m_debug.lock()->SetPosition(temp);
		m_search.lock()->SetPosition(m_currentTargetPos);

		MoveRabbit();
		MoveReduction();

		Debug::Log(L"�E�T�M�̂��󒆂�", m_isAir);
		Debug::Log(L"�E�T�M�̍��W", GetPosition());
		Debug::Log(L"�E�T�M�̈ړ���", m_velocity);

		if (m_ptrDraw->IsTargetAnimeEnd() && !m_isAir)
		{
			m_ptrDraw->ChangeCurrentAnimation(L"JUMP");
			switch (m_state)
			{
			case Patrol:
				Debug::Log(L"�p�g���[���X�e�[�g");
				PatrolState();
				break;

			case Seek:
				Debug::Log(L"�V�[�N�X�e�[�g");
				SeekState();
				break;

			case LostSight:
				Debug::Log(L"���X�g�X�e�[�g");
				LostState();
				break;

			default:
				break;
			}
		}

		m_aliveBlockPos.clear();
	}

	void Rabbit::OnCollisionEnter(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block"))
		{
			// �u���b�N�̃p�����[�^���擾
			const auto& cube = dynamic_pointer_cast<CubeObject>(other);
			Vec3 objPos = cube->GetSlopePos();
			Vec3 helf = cube->GetScale() / 2.0f;
			m_aliveBlockPos.push_back(objPos);

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
	}


	void Rabbit::OnCollisionExcute(const CollisionPair& Pair)
	{
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


	void Rabbit::OnCollisionExit(const CollisionPair& Pair)
	{

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
			PlayerTargetJump(targetPos);
		}
	}

	void Rabbit::LostState()
	{
		// ���W�̎擾
		Vec3 targetPos = m_currentTargetPos;
		const Vec3& pos = GetPosition();
		if ((targetPos - pos).length() <= 5.0f)
		{
			if (SearchPlayer())
			{
				m_state = Seek;
				return;
			}
			else
			{
				m_state = Patrol;
				return;
			}
		}

		// �Փˍ��W�̃o�b�t�@�z��
		vector<Vec3> hitPos = GetHitBlockPos(targetPos);

		// �Փ˂������W����������
		if (!hitPos.empty())
		{
			BlockTargetJump(targetPos);
		}
		else
		{
			PlayerTargetJump(targetPos);
		}
	}

	bool Rabbit::SearchPlayer()
	{
		if (m_targetObj.lock())
		{
			Vec3 targetPos = m_targetObj.lock()->GetPosition();
			targetPos.y = 0.0f;
			Vec3 pos = GetPosition();
			pos.y = 0.0f;

			float length = (targetPos - pos).length();
			if (length <= m_range)
			{
				Vec3 toTarget = -(targetPos - pos);
				toTarget.normalize();

				if (m_dir == toTarget.x)
				{
					return true;
				}
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
		Vec3 alive = GetAlivePosition();

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
		// �Փˍ��W�̃o�b�t�@�z��
		vector<Vec3> hitPos = GetHitBlockPos(targetPos);
		Vec3 nearPos = hitPos.at(0);
		Vec3 pos = GetPosition();

		// �o�b�t�@�z��̒������ԋ߂����W�����߂�
		for (const auto& hit : hitPos)
		{
			float lengthA = (nearPos - pos).length();
			float lengthB = (hit - pos).length();

			// ��r
			if (lengthA > lengthB)
			{
				nearPos = hit;
			}
		}

		// �Փ˂����Őڋ߂̃u���b�N�̏�Ƀu���b�N�����邩���`�F�b�N
		// �u���b�N���������ꍇ����ɂ��̏�ɂ��邩���`�F�b�N
		// �������A5�ȏ゠�����瓞�B�ł��Ȃ��Ɣ��f���ďI��
		int loopCount = 0;
		const Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
		while (BlockCheck(nearPos + up))
		{
			nearPos += up;
			loopCount++;
			if (loopCount > 5)
			{
				m_state = Patrol;
				break;
			}
		}

		PlayerTargetJump(nearPos);
	}

	const vector<Vec3> Rabbit::GetHitBlockPos(const Vec3& targetPos)
	{
		// �o�b�t�@�z��
		vector<Vec3> posVec;

		// ���W�̎擾
		Vec3 pos = GetPosition();

		// �u���b�N�I�u�W�F�N�g�O���[�v�̎擾
		const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Stage")->GetGroupVector();
		for (const auto& gameObj : blockVec)
		{
			// �I�u�W�F�N�g�������A�܂��͔�A�N�e�B�u���̃`�F�b�N
			if (!gameObj.lock()) continue;
			if (!gameObj.lock()->GetUpdateActive()) continue;

			// �^�L���X�g���āAOBB���擾���AOBB�Ɛ����ŏՓ˔��������
			const auto& block = dynamic_pointer_cast<CubeObject>(gameObj.lock());
			if (!block) continue;
			const auto& collObb = block->GetComponent<CollisionObb>();
			if (!collObb) continue;

			// �Փ˂�����o�b�t�@�ɍ��W��ǉ�
			if (HitTest::SEGMENT_OBB(pos, targetPos, collObb->GetObb()))
			{
				posVec.push_back(block->GetSlopePos());
			}
		}

		return posVec;
	}
}