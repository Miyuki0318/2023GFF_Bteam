/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Player::OnCreate()
	{
		// �r���f���p�I�u�W�F�N�g�̐���
		m_arm = GetStage()->AddGameObject<DebugObject>();

		// �g�����X�t�H�[���̐ݒ�
		// ���̃g�����X�t�H�[��
		m_ptrTrans = GetComponent<Transform>();
		m_ptrTrans->SetPosition(m_position);
		m_ptrTrans->SetScale(m_scale);
		
		// �r�̃g�����X�t�H�[��
		m_arm.lock()->SetPosition(m_position);
		m_arm.lock()->SetScale(m_scale);

		// ���̕`��̐ݒ�
		m_bodyDraw = AddComponent<PNTBoneModelDraw>();
		m_bodyDraw->SetMeshResource(L"ROBOT_BODY");
		m_bodyDraw->SetMeshToTransformMatrix(m_bodyMat);
		m_bodyDraw->AddAnimation(L"WALK", 0, 60, true);
		m_bodyDraw->AddAnimation(L"DAMAGE", 180, 30, false);
		m_bodyDraw->ChangeCurrentAnimation(L"WALK");

		// �r�̕`��̐ݒ�
		m_armDraw = m_arm.lock()->AddComponent<PNTBoneModelDraw>();
		m_armDraw->SetMultiMeshResource(L"ROBOT_ARM");
		m_armDraw->SetMeshToTransformMatrix(m_armMat);
		m_armDraw->AddAnimation(L"WALK", 0, 60, true);
		m_armDraw->AddAnimation(L"FIRE", 120, 30, false);
		m_armDraw->ChangeCurrentAnimation(L"WALK");

		// �R���W�����̐ݒ�
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(false);

		// �G�t�F�N�g�I�u�W�F�N�g�̐���
		m_jetEffect = GetStage()->AddGameObject<AirJetEffect>(GetThis<DebugObject>());
		m_shieldEffect = GetStage()->AddGameObject<ShieldEffect>(GetThis<DebugObject>());
		m_aligment = GetStage()->AddGameObject<ArrowEffect>(GetThis<DebugObject>());
		m_particle = GetStage()->AddGameObject<MultiParticle>();
	}

	void Player::OnUpdate()
	{
		// A�{�^�����͗L���ł̊֐�����
		if (m_acsel <= 1.7f && m_firePossible && Input::GetReleaseA())
		{
			OnReleaseA();
		}

		// ��C�ҋ@��
		if (m_cannonStandby)
		{
			CannonStandby(10.0f);
		}

		// ���G���Ԍo��
		if (m_isInvincible)
		{
			InvincibleTimer();
		}

		// �Ə��̉�]����
		RotateAligment();

		// �v���C���[�̈ړ��֐�
		MovePlayer();

		// �ړ����̌�����
		MoveReduction();

		// �v���C���[�̉�]�֐�
		RotatePlayer();

		// �A�j���[�V�����̍Đ�
		AnimationUpdate();

		// �G�t�F�N�g�`��֐�
		EffectUpdate();

		// �f�o�b�O������
		Debug::Log(L"pos : ", m_position);
		Debug::Log(L"velo : ", m_velocity);
		Debug::Log(L"acsel : ", m_acsel);
		Debug::Log(L"shield : ", m_shieldCount);
		Debug::Log(L"���G���� : ", m_invincibleTime - m_damageTime);
		Debug::Log(m_isDeath != false ? L"���S" : L"����");
		Debug::Log(m_isAir != false ? L"��" : L"�ڒn");
		Debug::Log(m_firePossible != false ? L"���ˉ�" : L"���˕s��");
	}

	// A�{�^���𗣂�����
	void Player::OnReleaseA()
	{
		// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
		Vec2 stick = Input::GetLStickValue();
		m_velocity = (stick.length() > 0.0f ? stick.round(1) : m_deffVelo) * 3.0f;
		if (!m_isAir && stick.y > 0.0f)
		{
			m_velocity.y = -stick.round(1).y * 3.0f;
		}
		
		// �����o�ϐ��̐ݒ�
		m_isAir = true;
		m_acsel = m_maxAcsel;
		m_cannonFire = false;

		// �r�̃A�j���[�V������ύX
		m_armDraw->ChangeCurrentAnimation(L"FIRE");

		// SE�̍Đ�
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		audioPtr->Start(L"AIRSHOCK_SE", 0, 0.5f);
	}

	// �R���W�����ɏՓ˂�����
	void Player::OnCollisionEnter(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block"))
		{
			BlockEnter(other, hitPoint);
		}
		if (other->FindTag(L"Spike"))
		{
			SpikeEnter(other, hitPoint);
		}
		if (other->FindTag(L"Bird"))
		{
			BirdEnter(other, hitPoint);
		}
		if (other->FindTag(L"Cannon"))
		{
			CannonEnter(other);
		}
		if (other->FindTag(L"Convayor"))
		{
			ConvayorEnter(other, hitPoint);
		}
		if (other->FindTag(L"Ring"))
		{
			const auto& ring = dynamic_pointer_cast<Ring>(other);
			ring->IsGetRing();

			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			audioPtr->Start(L"SHIELD_C_SE", 0, 0.75f);
			
			AddShield();
		}
		if (other->FindTag(L"Death"))
		{
			m_ptrTrans->SetPosition(m_respawnPos);
			m_acsel = 1.0f;
			m_velocity = m_deffVelo;
			m_firePossible = true;
		}
	}

	// �R���W�����ɏՓ˂���������
	void Player::OnCollisionExcute(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block"))
		{
			BlockExcute(other, hitPoint);
		}
		if (other->FindTag(L"Spike"))
		{
			SpikeExcute(other, hitPoint);
		}
		if (other->FindTag(L"Cannon"))
		{
			if (!m_cannonStandby)
			{
				CannonEnter(other);
			}
		}
		if (other->FindTag(L"Convayor"))
		{
			ConvayorExcute(other, hitPoint);
		}
	}

	// �R���W�������痣�ꂽ��
	void Player::OnCollisionExit(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block") || other->FindTag(L"Spike") || other->FindTag(L"Covayor"))
		{
			BlockExit(other);
		}
	}

	// �ړ��֐�
	void Player::MovePlayer()
	{
		// �O�t���[������̃f���^�^�C�����擾
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

		// ���݂̍��W���擾
		m_position = m_ptrTrans->GetPosition();

		// �|�W�V�����Ɉړ��x�N�g���Ƒ��x�Ɖ����x�ƃf���^�^�C���Ŋ|�����������Z
		m_position.x += -m_velocity.x * m_speed * m_acsel * deltaTime;
		m_position.y += -m_velocity.y * m_speed * m_acsel * deltaTime;

		// ���W�̍X�V
		m_ptrTrans->SetPosition(m_position);

		// �r�̍��W�̍X�V(�r�Ɠ��̃��f���}�g���N�X�̃|�W�V����y���̍������|�W�V�������獷������)
		Vec3 pos = m_position;
		auto& bone = m_bodyDraw->GetVecLocalBones();
		pos.y -= (m_armMat.getMajor3().y - m_bodyMat.getMajor3().y) - bone.at(1).getTranslation().y;
		m_arm.lock()->SetPosition(pos);
	}

	// �ړ�������
	void Player::MoveReduction()
	{
		// �f���^�^�C�����擾
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

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
		}
	}

	// �v���C���[�̉�]
	void Player::RotatePlayer()
	{
		// ���Ƙr��rad
		float body = 0.0f, arm = 0.0f;

		// �X�e�B�b�N����
		const Vec2& stick = Input::GetLStickValue().round(1);

		// �󒆂��ǂ����ŕ���
		if (m_isAir)
		{
			Vec2 velo = m_timeSpeed == m_normalTime ? m_velocity : stick.length() > 0.0f ? stick : m_deffVelo;
			body = (velo.x > 0.0f) * XM_PI;
			arm = atan2f(velo.y, velo.x) + XM_PIDIV2;
		}
		else
		{
			Vec2 velo = stick.length() > 0.0f ? stick : m_velocity != Vec2(0.0f) ? m_velocity : m_deffVelo;
			arm = atan2f(velo.y, velo.x) + XM_PIDIV2;
		}

		// ���[�e�[�V�����̍X�V
		m_ptrTrans->SetRotation(Vec3(0.0f, body, 0.0f));
		m_arm.lock()->SetRotation(Vec3(0.0f, 0.0f, arm));
	}

	// �O���̉�]�`��
	void Player::RotateAligment()
	{
		// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
		Vec2 stick = Input::GetLStickValue().round(1);
		Vec2 velo = (stick.length() > 0.0f ? stick : m_deffVelo) / 10.0f;

		// �I�u�W�F�N�g�̐������[�v
		float loopCount = 1.0f;

		vector<Vec3> points;

		for (size_t i = 0; i < 40; i++)
		{
			Vec2 pos = -velo * m_speed * m_maxAcsel * loopCount;
			velo.y -= m_gravity * 0.016f / 20.0f / 4.0f;
			points.push_back(Vec3(pos.x, pos.y, 0.0f));
			loopCount += 0.25f;
		}

		m_aligment.lock()->UpdateEffect(points);
		m_aligment.lock()->SetDrawActive(stick.length() > 0.0f && m_firePossible);
	}

	// �A�j���[�V�����̍X�V
	void Player::AnimationUpdate()
	{
		// �O�t���[������̃f���^�^�C���ɃQ�[���X�s�[�h���|���������擾
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

		// �A�j���[�V�������I����Ă���ҋ@��Ԃɂ���
		if (m_bodyDraw->IsTargetAnimeEnd())
		{
			m_bodyDraw->ChangeCurrentAnimation(L"WALK");
		}
		if (m_armDraw->IsTargetAnimeEnd())
		{
			m_armDraw->ChangeCurrentAnimation(L"WALK");
		}

		// �A�j���[�V�����̍X�V
		m_bodyDraw->UpdateAnimation(deltaTime);
		m_armDraw->UpdateAnimation(deltaTime);
	}

	// �G�t�F�N�g�̍X�V
	void Player::EffectUpdate()
	{
		// �X�V
		m_shieldEffect.lock()->UpdateEffect();
		m_shieldEffect.lock()->SetDrawShield(m_shieldCount > 0 && !m_cannonStandby);
		m_jetEffect.lock()->SetDrawActive(m_firePossible && !m_cannonFire);

		if (m_firePossible && !m_cannonFire)
		{
			m_jetEffect.lock()->UpdateEffect();
		}
		if (m_cannonFire)
		{
			const auto particle = m_particle.lock()->InsertParticle(2);
			particle->SetEmitterPos(m_position);
			particle->SetTextureResource(L"SMOKE_TX");
			particle->SetMaxTime(3.0f);

			// ���������X�v���C�g��z��Ŏ擾
			vector<ParticleSprite>& pSpriteVec = particle->GetParticleSpriteVec();

			// �X�v���C�g�̐������[�v
			for (auto& sprite : pSpriteVec)
			{
				sprite.m_Velocity = Vec3(m_velocity.x, m_velocity.y, 0.0f).normalize();
				sprite.m_LocalScale = Vec2(m_acsel > 2.0f ? m_acsel / 2.5f : 0.0f);
				sprite.m_LocalQt.rotationZ(Utility::DegToRad(Utility::RangeRand(360.0f, 0.0f)));
			}
		}
	}

	// ��C���ˑҋ@��
	void Player::CannonStandby(float acsel)
	{
		if (m_activeCannon.lock())
		{
			const auto& drawPtr = m_activeCannon.lock()->GetComponent<PNTBoneModelDraw>();
			if (drawPtr->GetCurrentAnimationTime() > 1.4f)
			{
				m_acsel = acsel;
				m_isAir = true;
				m_firePossible = true;
				m_cannonFire = true;
				m_cannonStandby = false;

				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
				audioPtr->Start(L"CANNON_SE", 0, 0.75f);

				m_activeCannon.reset();
			}
			else
			{
				m_position = m_activeCannon.lock()->GetPosition();
				m_ptrTrans->SetPosition(m_position);
			}
		}
	}

	// ���G���Ԍo��
	void Player::InvincibleTimer()
	{
		float deltaTime = App::GetApp()->GetElapsedTime();

		m_damageTime += deltaTime;

		if (m_invincibleTime <= m_damageTime)
		{
			m_damageTime = 0.0f;
			m_isInvincible = false;
		}
	}

	// �u���b�N�ɏՓ˂����u��
	void Player::BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		auto objTrans = block->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// ��Ƀu���b�N�����邩�̃`�F�b�N
			if (!BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

			// �G�A�V���b�N�g�p�\�ɂ���
			m_firePossible = true;
			m_respawnPos = Vec3(objPos.x, objPos.y + (helf.y * 3.0f), 0.0f);

			// ���S���肪����Ȃ�
			if (m_isDeath)
			{
				// �R���e�B�j���[��ʂ֑J��
			}

			// �ړ��ʂ��������ɂ���
			if (m_velocity.y > 0.0f)
			{
				// �����x���������傫��������
				if (m_acsel > 2.5f || m_velocity.y > 5.0f)
				{
					// �ړ��ʂ𔽓]�����A�����ɂ���
					m_velocity.y *= -0.5f;
				}
			}
			return;
		}

		// ������Փ�
		if (CollHitUnder(hitPos, objPos, helf))
		{
			// ���Ƀu���b�N�����邩�̃`�F�b�N
			if (!BlockCheck(Vec3(objPos.x, objPos.y - (helf.y * 2.0f), 0.0f))) return;

			// �ړ��ʂ�������Ȃ�
			if (m_velocity.y < 0.0f)
			{
				// ���]�����A����������
				m_velocity.y *= -1.0f;
			}
			return;
		}

		// ������Փ�
		if (CollHitLeft(hitPos, objPos, helf))
		{
			// ���Ƀu���b�N�����邩�̃`�F�b�N
			if (!BlockCheck(Vec3(objPos.x - (helf.x * 2.0f), objPos.y, 0.0f))) return;

			// �ړ��ʂ𔼌������]������
			m_velocity.x *= -0.5f;
			return;
		}

		// �E����Փ�
		if (CollHitRight(hitPos, objPos, helf))
		{
			// �E�Ƀu���b�N�����邩�̃`�F�b�N
			if (!BlockCheck(Vec3(objPos.x + (helf.x * 2.0f), objPos.y, 0.0f))) return;

			// �ړ��ʂ𔼌������]������
			m_velocity.x *= -0.5f;
			return;
		}
	}

	// �u���b�N�ɏՓ˂���������
	void Player::BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		auto objTrans = block->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// Y���ړ��x�N�g����0.0�ɂ��A�󒆂��̐^�U��false
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
			m_isAir = false;
		}
	}

	// �u���b�N�Ƃ̏Փ˂������Ȃ�����
	void Player::BlockExit(const shared_ptr<GameObject>& block)
	{
		// �ڒn�t���O������
		m_isAir = true;
	}

	// �Փ˂����u���b�N�̏�Ƀu���b�N�����邩�̌���
	bool Player::BlockCheck(const Vec3& upperPos)
	{
		const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Stage")->GetGroupVector();

		bool check = true;

		for (const auto& ptr : blockVec)
		{
			if (!ptr.lock()) continue;

			const auto& block = dynamic_pointer_cast<CubeObject>(ptr.lock());
			if (!block) continue;

			Vec3 pos = block->GetPosition();

			if (pos == upperPos)
			{
				check = false;
			}
		}

		return check;
	}

	// �X�p�C�N�ƏՓ˂����u��
	void Player::SpikeEnter(const shared_ptr<GameObject>& obj, const Vec3& hitPos)
	{
		// �X�p�C�N�I�u�W�F�N�g�ɃL���X�g
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// �p�����[�^�̎擾
		Vec3 spikePos = spike->GetPosition();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// �Փˉ�
		bool upper, under, left, right;
		upper = CollHitUpper(hitPos, spikePos, helfScale);
		under = CollHitUnder(hitPos, spikePos, helfScale);
		left = CollHitLeft(hitPos, spikePos, helfScale);
		right = CollHitRight(hitPos, spikePos, helfScale);


		// �X�p�C�N�̕����ɉ����ď���
		const auto& angle = spike->GetAngle();
		switch (angle)
		{
		case Gimmick::Up:
			if (upper || left || right)
			{
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}
			if (under)
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::Down:
			if (under || left || right)
			{
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}
			if (upper)
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::Left:
			if (upper || under || left)
			{
				DamageKnockBack(Vec2(1.5f, -0.5f));
				return;
			}
			if (right)
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::Right:
			if (upper || under || right)
			{
				DamageKnockBack(Vec2(-1.5f, -0.5f));
				return;
			}
			if (left)
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::All:
			if (upper)
			{
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}
			if (under)
			{
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}
			if (left)
			{
				DamageKnockBack(Vec2(1.5f, -0.5f));
				return;
			}
			if (right)
			{
				DamageKnockBack(Vec2(-1.5f, -0.5f));
				return;
			}
			break;

		default:
			break;
		}
	}

	// �X�p�C�N�ƏՓ˂���������
	void Player::SpikeExcute(const shared_ptr<GameObject>& obj, const Vec3& hitPos)
	{
		// �X�p�C�N�I�u�W�F�N�g�ɃL���X�g
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// �p�����[�^�̎擾
		Vec3 spikePos = spike->GetPosition();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// �X�p�C�N�̕����ɉ����ď���
		const auto& angle = spike->GetAngle();
		if (angle == Gimmick::Down)
		{
			m_firePossible = true;
			BlockExcute(obj, hitPos);
		}
	}

	// ���ƏՓ˂�����
	void Player::BirdEnter(const shared_ptr<GameObject>& enemy, const Vec3& hitPos)
	{
		// ���I�u�W�F�N�g�ɃL���X�g
		const auto& bird = dynamic_pointer_cast<Bird>(enemy);

		// �p�����[�^�̎擾
		Vec3 birdPos = bird->GetPosition();
		Vec3 helfScale = bird->GetScale() / 2.0f;

		// �ォ��Փ�
		if (CollHitUpper(hitPos, birdPos, helfScale))
		{
			// �ړ��ʂ𔽓]�����A�����ɂ���
			m_velocity.x = -Input::GetLStickValue().round(1).x;
			m_velocity.y = -2.0f;
			m_acsel = m_damageAcsel;
			m_firePossible = true;
			return;
		}

		// ������Փ�
		if (CollHitUnder(hitPos, birdPos, helfScale))
		{
			DamageKnockBack(Vec2(0.9f, 1.0f));
			return;
		}

		// ������Փ�
		if (CollHitLeft(hitPos, birdPos, helfScale))
		{
			DamageKnockBack(Vec2(1.5f, -0.5f));
			return;
		}

		// �E����Փ�
		if (CollHitRight(hitPos, birdPos, helfScale))
		{
			DamageKnockBack(Vec2(-1.5f, -0.5f));
			return;
		}
	}

	// ��C�ƏՓ˂�����
	void Player::CannonEnter(const shared_ptr<GameObject>& cannon)
	{
		const auto& ptr = dynamic_pointer_cast<Cannon>(cannon);
		ptr->OnFire();
		m_activeCannon = ptr;

		m_isAir = false;
		m_firePossible = false;
		m_cannonStandby = true;

		m_velocity = Vec2(0.0f);
		m_acsel = 1.0f;
	}

	// �x���g�R���x�A�ƏՓ˂�����
	void Player::ConvayorEnter(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		auto objTrans = convayor->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// ��Ƀu���b�N�����邩�̃`�F�b�N
			if (!BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

			// �G�A�V���b�N�g�p�\�ɂ���
			m_firePossible = true;
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;

			// �߂荞�݂��C��
			Vec3 pos = m_ptrTrans->GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			m_ptrTrans->SetPosition(pos);

			const auto& ptr = dynamic_pointer_cast<Convayor>(convayor);
			if (!ptr) return;

			const auto& angle = ptr->GetRotate();
			switch (angle)
			{
			case Convayor::LeftRot:
				m_velocity.x = m_speed;
				break;

			case Convayor::RightRot:
				m_velocity.x = -m_speed;
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
	void Player::ConvayorExcute(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		auto objTrans = convayor->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (CollHitUpper(hitPos, objPos, helf))
		{
			Vec3 pos = m_ptrTrans->GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			m_ptrTrans->SetPosition(pos);

			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
		}
	}

	// �_���[�W�m�b�N�o�b�N
	void Player::DamageKnockBack(const Vec2& velocity)
	{
		// ���ԑ��x��ʏ푬�x�ŏ㏑��
		m_timeSpeed = m_normalTime;
		m_firePossible = false;
		m_velocity = velocity;
		m_acsel = m_damageAcsel;

		// �A�j���[�V�������_���[�W��Ԃɂ���
		m_bodyDraw->ChangeCurrentAnimation(L"DAMAGE");

		// �O���̕\�����I�t
		m_aligment.lock()->SetDrawActive(false);

		if (!m_isInvincible)
		{
			if (m_shieldCount > 0)
			{
				m_shieldCount--;
				m_isInvincible = true;

				const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
				audioPtr->Start(L"SHIELD_D_SE", 0, 0.75f);
			}
			else
			{
				m_isDeath = true;
			}
		}
	}

	// �ォ��Փ˂������̌���
	bool Player::CollHitUpper(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
	{
		return hitPos.y > hitObjPos.y && ((hitPos.y - hitObjPos.y) >= helfScale.y);
	}

	// ������Փ˂������̌���
	bool Player::CollHitUnder(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
	{
		return hitPos.y < hitObjPos.y && ((hitPos.y - hitObjPos.y) <= -helfScale.y);
	}

	// ������Փ˂������̌���
	bool Player::CollHitLeft(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
	{
		return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
			&& ((hitPos.x - hitObjPos.x) < helfScale.x);
	}

	// �E����Փ˂������̌���
	bool Player::CollHitRight(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
	{
		return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
			&& ((hitPos.x - hitObjPos.x) > -helfScale.x);
	}
}