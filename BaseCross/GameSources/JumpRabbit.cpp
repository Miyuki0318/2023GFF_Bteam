/*!
@file JumpRabbit.cpp
@brief �W�����v���郁�J�E�T�M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void JumpRabbit::OnCreate()
	{
		// �p�����̐������̏��������s
		Rabbit::OnCreate();

		// �������̃G�t�F�N�g�𐶐�
		const auto& stage = GetStage();
		m_discovered = stage->AddGameObject<Billboard>(L"DISCOVER_TX", Vec2(2.0f), Vec3(0.0f));
		m_discovered.lock()->SetDrawActive(false);
	}

	// ���t���[���X�V����
	void JumpRabbit::OnUpdate()
	{
		MoveRabbit();    // �ړ��X�V����
		MoveReduction(); // �ړ��ʌ�������

		// �A�j���[�V�����̍X�V
		m_ptrDraw->UpdateAnimation(DELTA_TIME / 2.0f);

		// �W�����v�\���̐^�U
		bool jumpPossible = m_ptrDraw->IsTargetAnimeEnd() && !m_status(eStatus::IsAir, eStatus::IsDeath);
		if (jumpPossible)
		{
			// �A�j���[�V�����̐ݒ�
			m_ptrDraw->ChangeCurrentAnimation(L"JUMP");

			// �X�e�[�g�ŕ���
			switch (m_state)
			{
			case eState::Patrol: // �p�g���[��
				PatrolState(); // �p�g���[���X�e�[�g
				break;

			case eState::Seek: // �ǐ�
				SeekState(); // �ǐՃX�e�[�g
				break;

			case eState::LostSight: // �����Ȃ���
				LostState(); // �����Ȃ������̃X�e�[�g
				break;

			default:
				break;
			}
		}

		// ��C��
		if (m_state == eState::CannonJump && !m_status(eStatus::IsDeath))
		{
			CannonState(); // ��C�X�e�[�g
		}

		// ���S��
		if (m_state == eState::Death)
		{
			DeathState(); // ���S�X�e�[�g
		}

		// �z��̏�����
		m_aliveBlockPos.clear();
	}

	// �j�����̏���
	void JumpRabbit::OnDestroy()
	{
		// �������̃G�t�F�N�g���\��
		m_discovered.lock()->SetDrawActive(false);

		// �X�e�[�W���甭�����̃G�t�F�N�g��j��
		const auto& stage = GetStage();
		stage->RemoveGameObject<Billboard>(m_discovered.lock());
		m_discovered.reset();
	}

	// �R���W�����ɏՓ˂�����
	void JumpRabbit::OnCollisionEnter(const CollisionPair& Pair)
	{
		// �Փ˂����I�u�W�F�N�g�ƏՓˍ��W���擾
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		// �^�O���u���b�N�܂��́A���Ȃ�
		if (other->FindTag(L"Block") || other->FindTag(L"Spike"))
		{
			// �u���b�N�ƏՓ˂����u�Ԃ̏��������s
			BlockEnter(other, hitPoint);
		}

		// �^�O���x���g�R���x�A�Ȃ�
		if (other->FindTag(L"Convayor"))
		{
			// �x���g�R���x�A�ɏՓ˂����u�Ԃ̏��������s
			ConvayorEnter(other, hitPoint);
		}

		// �^�O���o���p�[�Ȃ�
		if (other->FindTag(L"Bumper"))
		{
			// �o���p�[�ɏՓ˂����u�Ԃ̏��������s
			BumperEnter(other, hitPoint);
		}

		// �^�O����C�Ȃ�
		if (other->FindTag(L"Cannon"))
		{
			// ��C�ƏՓ˂����u�Ԃ̏��������s
			CannonEnter(other);
		}

		// �^�O�����S����R���W�����Ȃ�
		if (other->FindTag(L"Death"))
		{
			// �X�e�[�g��ύX
			m_state = eState::Death;

			// ���S���Ă���Ȃ�
			if (m_status(eStatus::IsDeath))
			{
				// ���g���X�e�[�W����j������
				const auto& stage = GetStage();
				stage->RemoveGameObject<Rabbit>(GetThis<Rabbit>());
			}
		}

		// �^�O���E�T�M�Ȃ�
		if (other->FindTag(L"Rabbit"))
		{
			// �ړ��ʂ𔼌������]������
			m_velocity.x *= -1.0f;
			m_dir *= -1.0f;
		}
	}

	// �R���W�����ɏՓ˂���������
	void JumpRabbit::OnCollisionExcute(const CollisionPair& Pair)
	{
		// �Փ˂����I�u�W�F�N�g�ƏՓˍ��W���擾
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		// �^�O���u���b�N�܂��́A���Ȃ�
		if (other->FindTag(L"Block") || other->FindTag(L"Spike"))
		{			
			// �u���b�N�ɏՓ˂����������̏��������s
			BlockExcute(other, hitPoint);
		}

		// �^�O���x���g�R���x�A�Ȃ�
		if (other->FindTag(L"Convayor"))
		{
			// �x���g�R���x�A�ɏՓ˂����������̏��������s
			ConvayorExcute(other, hitPoint);
		}
	}

	// �R���W�����Ƃ̏Փ˂��I�������
	void JumpRabbit::OnCollisionExit(const CollisionPair& Pair)
	{
		// �Փ˂��I������I�u�W�F�N�g�ƏՓˍ��W���擾
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		// �^�O���u���b�N�܂��͓����ǂȂ�
		if (other->FindTag(L"Block") || other->FindTag(L"MoveWall"))
		{
			// �^�L���X�g
			const auto& cube = dynamic_pointer_cast<CubeObject>(other);
			if (cube)
			{
				// �^�C�v���擾
				const auto& type = cube->GetCubeType();

				// �ʏ�^�C�v�Ȃ�
				if (type == CubeParam::eCubeType::Normal)
				{
					// �󒆂��̐^�U��true��
					m_status.Set(eStatus::IsAir) = true;
				}
				else
				{
					// �X���[�v�ŁA�I�u�W�F�N�g�̈ʒu���Ō�ɏՓ˂����ʒu���Ⴉ������
					if (hitPoint.y <= cube->GetPosition().y)
					{
						// �󒆂��̐^�U��true��
						m_status.Set(eStatus::IsAir) = true;
					}
				}
			}
		}
	}

	// �ړ�����
	void JumpRabbit::MoveRabbit()
	{
		// �p�����̈ړ����������s
		Rabbit::MoveRabbit();

		// �G�t�F�N�g�̃r���{�[�h�\��
		m_discovered.lock()->SetPosition(m_position + Vec3(0.0f, 2.5f, 0.0f));
		m_discovered.lock()->UpdateBillboard();
	}

	// �u���b�N�ɏՓ˂����u��
	void JumpRabbit::BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// ���S���Ă���Ȃ玩�g��j��
		if (m_status(eStatus::IsDeath)) GetStage()->RemoveGameObject<Rabbit>(GetThis<Rabbit>());

		// �u���b�N�̃p�����[�^���擾
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		// �z��Ƀu���b�N�̍��W��ǉ�
		m_aliveBlockPos.push_back(objPos);

		// ��C��Ԃ�����
		m_status.Set(eStatus::IsCannon) = false;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// �ړ��ʂ��������ɂ���
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

		// ������Փ�
		if (CollHitUnder(hitPos, objPos, helf))
		{
			// �ړ��ʂ�������Ȃ�
			if (m_velocity.y < 0.0f)
			{
				// ���]�����A����������
				m_velocity.y *= -1.0f;
			}
		}

		// ���E����Փ�	
		if (CollHitLeft(hitPos, objPos, helf) || CollHitRight(hitPos, objPos, helf))
		{
			// �L���[�u�^�C�v���ʏ�u���b�N�Ȃ�
			const auto& angle = cube->GetCubeType();
			if (angle == CubeParam::eCubeType::Normal)
			{
				// �ړ��ʂ𔼌������]������
				m_velocity.x *= -0.5f;
				m_dir *= -1.0f;
			}
		}
	}

	// �u���b�N�ɏՓ˂���������
	void JumpRabbit::BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		// �z��Ƀu���b�N�̍��W��ǉ�
		m_aliveBlockPos.push_back(objPos);

		// �ړ��ʂ��������ɂ���
		if (m_velocity.y > 0.0f)
		{
			// �ڒn��Ԃɂ���
			m_acsel = 1.0f;
			m_status.Set(eStatus::IsAir) = false;

			// �L���[�u�^�C�v���ʏ�^�C�v�Ȃ�
			const auto& angle = cube->GetCubeType();
			if (angle == CubeParam::eCubeType::Normal)
			{
				// �ڒn������������
				SetPosition(GetPosition().x, objPos.y + helf.y + (m_scale.y / 2.0f) + 0.05f);
			}
		}
	}

	// �x���g�R���x�A�ƏՓ˂����u��
	void JumpRabbit::ConvayorEnter(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
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

			// �ڒn��Ԃɂ���
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

			// �����ɉ����Ĉړ���X�ɑ��x��ݒ�
			switch (angle)
			{
			case Convayor::eRotType::LeftRot:
				m_velocity.x = speed;
				break;

			case Convayor::eRotType::RightRot:
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

	// �x���g�R���x�A�ƏՓ˂���������
	void JumpRabbit::ConvayorExcute(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
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

	// �o���p�[�ɏՓ˂����u��
	void JumpRabbit::BumperEnter(const shared_ptr<GameObject>& other, const Vec3& hitPos)
	{
		// �^�L���X�g
		const auto& bumper = dynamic_pointer_cast<Bumper>(other);
		if (bumper)
		{
			// �o���p�[�̍��W�Ǝ��g�̍��W����p�x���擾
			const Vec3& bumperPos = bumper->GetPosition();
			float rad = -atan2f(hitPos.y - bumperPos.y, hitPos.x - bumperPos.x);

			// �p�x����ړ�������ݒ�
			Vec2 velo = Vec2(-cos(rad), sin(rad)).normalize();

			// �p�����[�^�̐ݒ�
			m_velocity = velo * m_speed; // �ړ��ʂ��ړ������Ƒ��x�Ō���
			m_acsel = m_maxAcsel; 		 // �����x���ő�����x�ɐݒ�
			m_status.Set(eStatus::IsAir) = true; // �󒆂��̐^�U���I����

			// �o���p�[�ɏՓ˔��������𑗂�
			bumper->OnHit();
		}
	}

	// ��C�ƏՓ˂����u��
	void JumpRabbit::CannonEnter(const shared_ptr<GameObject>& other)
	{
		// ��C�^�ɃL���X�g
		const auto& ptr = dynamic_pointer_cast<Cannon>(other);
		if (ptr)
		{
			// �ێ����Ă����C�|�C���^�ƈ������
			if (m_activeCannon.lock() != ptr)
			{
				// ���ˏ����֐�
				ptr->OnFire();

				// �������C�̃|�C���^��ێ�
				m_activeCannon.reset();
				m_activeCannon = ptr;

				// �p�����[�^�̐ݒ�
				m_status.Set(eStatus::IsAir, eStatus::IsDeath) = true;

				// �����ړ��s�ɐݒ�
				m_velocity = Vec2(0.0f);
				m_acsel = 1.0f;

				// �X�e�[�g���C�X�e�[�g�ɕύX
				m_state = eState::CannonJump;

				// ���W���C�̍��W�ōX�V
				SetPosition(ptr->GetPosition());

				// �R���W�������I�t��
				m_ptrColl->SetUpdateActive(false);
			}
		}
	}

	// �p�g���[���X�e�[�g
	void JumpRabbit::PatrolState()
	{
		// �^�[�Q�b�g�̃|�C���^�������
		if (m_targetObj.lock())
		{
			// �v���C���[�T�[�`���|����
			if (SearchPlayer())
			{
				// �ǐՃX�e�[�g�ɕύX
				m_state = eState::Seek;

				// �������̃G�t�F�N�g��\��
				m_discovered.lock()->SetDrawActive(true);
				return;
			}

			// �������̃G�t�F�N�g���\��
			m_discovered.lock()->SetDrawActive(false);
			
			// �W�����v�ړ�������
			JumpingRabbit();
		}
	}

	// �ǐՃX�e�[�g
	void JumpRabbit::SeekState()
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
			// �Փ˂����u���b�N������l�ɃW�����v����
			BlockTargetJump(targetPos);
		}
		else
		{
			// �v���C���[�Ɍ����ăW�����v����
			PlayerTargetJump(targetPos + Vec3(0.0f, -1.0f, 0.0f));
		}
	}

	// �����Ȃ������̃X�e�[�g
	void JumpRabbit::LostState()
	{
		// ���W�̎擾
		Vec3 targetPos = m_currentTargetPos;
		const Vec3& pos = GetPosition();

		// �����������W���˒��������A�܂��͌����Ȃ��Ă���̃W�����v����5���瑽��������
		if ((targetPos - pos).length() <= m_range || m_lostJumpCount > 5)
		{
			// �T�[�`���|����
			if (SearchPlayer())
			{
				// �X�e�[�g��ǐՃX�e�[�g�ɕύX
				m_state = eState::Seek;
				m_lostJumpCount = 0; // �W�����v�񐔂����Z�b�g
				return;
			}
			else
			{
				// �X�e�[�g���p�g���[���X�e�[�g�ɕύX
				m_state = eState::Patrol;
				m_lostJumpCount = 0; // �W�����v�񐔂����Z�b�g
				return;
			}
		}

		// �Փˍ��W�̃o�b�t�@�z��
		vector<Vec3> hitPos = GetHitBlockPos(targetPos);

		// �Փ˂������W����������
		if (!hitPos.empty())
		{
			// �u���b�N�������p�ɃW�����v�ړ�����
			BlockTargetJump(targetPos);
			m_lostJumpCount++; // �W�����v�񐔂����Z
		}
		else
		{
			// �^�[�Q�b�g�Ɍ����ăW�����v�ړ�����
			PlayerTargetJump(targetPos);
			m_lostJumpCount++; // �W�����v�񐔂����Z
		}
	}

	// ��C�X�e�[�g
	void JumpRabbit::CannonState()
	{
		// ��C�̃|�C���^������Ȃ�
		if (m_activeCannon.lock())
		{
			// ��C�̃V�F�A�h�|�C���^
			const auto& cannon = m_activeCannon.lock();

			// ���ˎ��̎��Ԃ̎擾
			const float& fireTime = cannon->GetFireTime();
			const auto& animeTime = cannon->GetAnimationTime();
			
			// �Đ����Ԃ����ˎ��̎��Ԃ��߂�����
			if (animeTime > fireTime)
			{
				m_status.Set(eStatus::IsAir) = true; // �󒆏�Ԃ�
				m_state = eState::Patrol; // �p�g���[���X�e�[�g�ɕύX

				// ��C��Z�������W�A���ɕϊ����A�ړ��ʂ�ݒ肷��
				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				Vec2 velo = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				// �ړ����������s
				SetMoveValue(velo, 10.0f);

				// SE���Đ�
				StartSE(L"CANNON_SE", 0.75f);

				// �R���W����������A�N�e�B�u��
				GetComponent<CollisionObb>()->SetUpdateActive(true);

				// ��C�̃|�C���^��������
				m_activeCannon.reset();
			}
			else
			{
				// �ړ��s��Ԃ�
				m_velocity = Vec2(0.0f);

				// ���W���C�̍��W�ōX�V
				SetPosition(cannon->GetPosition());
			}
		}
	}

	// ���S�X�e�[�g
	void JumpRabbit::DeathState()
	{
		// ���S���Ă��Ȃ����
		if (!m_status(eStatus::IsDeath))
		{
			// ���S��������ɂ���
			m_status.Set(eStatus::IsDeath) = true;

			// �R���W�����R���|�[�l���g�ƃ^�O���폜
			RemoveComponent<CollisionObb>();
			RemoveTag(L"Rabbit");

			// ���������p�̈ړ��ʂƉ����x�ɐݒ�
			m_velocity = Vec2(Utility::RangeRand(-1.0f, 1.0f), -1.5f);
			m_acsel = m_maxAcsel;
		}
		else
		{
			// ��]����
			SetRotation(GetRotation() + Vec3(0.0f, 0.0f, DELTA_TIME * m_dir * Utility::RangeRand(15.0f, 5.0f)));
		}
	}

	// �v���C���[�T�[�`
	bool JumpRabbit::SearchPlayer()
	{
		// �^�[�Q�b�g�̃|�C���^������Ȃ�
		if (m_targetObj.lock())
		{
			// ���W�̎擾
			Vec3 targetPos = m_targetObj.lock()->GetPosition();
			Vec3 pos = GetPosition();

			// �������˒��������Ȃ�
			float length = (targetPos - pos).length();
			if (length <= m_range)
			{
				// �v���C���[�܂łƂ̊ԂɃu���b�N���������
				if (GetHitBlockPos(targetPos).empty())
				{
					// ��r�p�ɍ��������O
					targetPos.y = 0.0f;
					pos.y = 0.0f;

					// �^�[�Q�b�g�̕��������߂�
					Vec3 toTarget = -(targetPos - pos);
					toTarget.normalize();

					// ���g�̌����ƈ�v������u�����v
					if (m_dir == toTarget.x)
					{
						// ��������SE���Đ����ďI��
						StartSE(L"RABBIT_SEARCH_SE", 0.5f);
						return true;
					}
				}
				return false;
			}
			else
			{
				// �ǐՃX�e�[�g�Ȃ�
				if (m_state == eState::Seek)
				{
					// ���������X�e�[�g�ɕύX
					m_state = eState::LostSight;

					// �����������̍��W��ێ�
					m_currentTargetPos = m_targetObj.lock()->GetPosition();
				}
				return false;
			}
		}
		return false;
	}

	// �W�����v�ړ�
	void JumpRabbit::JumpingRabbit()
	{
		// �W�����v�ړ��ʂƎ��g������Ă�u���b�N���擾
		Vec2 jumpVelo = m_jumpVelo;
		Vec3 alive = GetNearPosition(m_aliveBlockPos);

		// �����ɉ����ď���
		if (m_dir > 0.0f)
		{
			bool check = false; // ���ׂ邩�̐^�U

			// �W�����v�ړ�����W�̐����[�v
			for (const auto& jump : m_jumpTargetPos.at(0))
			{
				// ���n�悪����Ȃ�true�ɂ��ďI��
				check = BlockCheck(alive + jump);
				if (check) break;
			}

			// ���n�悪������Δ��]
			if (!check) m_dir *= -1.0f;

			jumpVelo.x *= m_dir; // �W�����v���̈ړ��ʂɌ������|����
			m_status.Set(eStatus::IsAir) = true; // �󒆏�Ԃ�

			// �ړ��ݒ�֐������s
			SetMoveValue(jumpVelo, m_maxAcsel);
		}
		else
		{
			bool check = false; // ���ׂ邩�̐^�U

			// �W�����v�ړ�����W�̐����[�v
			for (const auto& jump : m_jumpTargetPos.at(1))
			{
				// ���n�悪����Ȃ�true�ɂ��ďI��
				check = BlockCheck(alive + jump);
				if (check) break;
			}

			// ���n�悪������Δ��]
			if (!check) m_dir *= -1.0f;

			jumpVelo.x *= m_dir; // �W�����v���̈ړ��ʂɌ������|����
			m_status.Set(eStatus::IsAir) = true; // �󒆏�Ԃ�

			// �ړ��ݒ�֐������s
			SetMoveValue(jumpVelo, m_maxAcsel);
		}
	}

	// �^�[�Q�b�g�Ɍ����ăW�����v
	void JumpRabbit::PlayerTargetJump(const Vec3& targetPos)
	{
		// �p�x����ړ��ʂ�ݒ�
		const Vec3& pos = GetPosition();
		float rad = atan2f(targetPos.y - pos.y, targetPos.x - pos.x);
		Vec2 velo = Vec2(cos(rad), sin(rad)).normalize();
		velo *= -m_speed; // �ړ��ʂ𑬓x�ŏ�Z
		velo.y += -1.0f;  // �������_��

		// �󒆏�Ԃɂ���
		m_status.Set(eStatus::IsAir) = true;

		// �ړ��ݒ�֐������s
		SetMoveValue(velo, m_maxAcsel);
	}

	// �u���b�N�𒵂щz����p�ɃW�����v
	void JumpRabbit::BlockTargetJump(const Vec3& targetPos)
	{
		// �Őڋߍ��W�̎擾
		Vec3 nearPos = GetNearPosition(GetHitBlockPos(targetPos));

		// �Փ˂����Őڋ߂̃u���b�N�̏�Ƀu���b�N�����邩���`�F�b�N
		// �u���b�N���������ꍇ����ɂ��̏�ɂ��邩���`�F�b�N
		// �������A7�ȏ゠�����瓞�B�ł��Ȃ��Ɣ��f���ďI��
		int loopCount = 0;
		while (BlockCheck(nearPos + UP_VEC))
		{
			nearPos += UP_VEC;
			loopCount++;
			if (loopCount > 7)
			{
				// ���B�s�Ȃ̂Ńp�g���[���X�e�[�g��
				m_state = eState::Patrol;
				return;
			}
		}

		// ���щz����p�ɃW�����v����
		PlayerTargetJump(nearPos);
	}

	// �W�����v����SE
	void JumpRabbit::StartJumpSE()
	{
		// ���W���擾
		const Vec3& pos = GetPosition();
		const Vec3& playerPos = m_targetObj.lock()->GetPosition();

		// �������˒������Ɣ�r
		float length = (playerPos - pos).length();
		if (length <= m_range)
		{
			// �������特�ʂ�ݒ肵�čĐ�
			float volume = (m_range - length) / 10.0f;
			StartSE(L"RABBIT_JUMP_SE", volume);
		}
	}

	// �v���C���[�܂łƂ̐����ƃu���b�N���Փ˂�����Փ˂����u���b�N�̍��W��Ԃ�
	const vector<Vec3> JumpRabbit::GetHitBlockPos(const Vec3& targetPos)
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

		// �z���Ԃ�
		return posVec;
	}
}