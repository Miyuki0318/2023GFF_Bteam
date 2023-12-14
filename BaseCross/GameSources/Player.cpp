/*! 
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

using namespace Input;
using namespace Utility;

namespace basecross
{
	void Player::OnCreate()
	{
		// �p�����̐������̊֐������s����
		TemplateObject::OnCreate();

		// �r���f���p�I�u�W�F�N�g�̐���
		m_arm = GetStage()->AddGameObject<TemplateObject>();

		// �g�����X�t�H�[���̐ݒ�
		// ���̃g�����X�t�H�[��
		SetPosition(m_position);
		SetScale(m_scale);
		
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
		m_ptrColl = AddComponent<CollisionSphere>();
		m_ptrColl->SetDrawActive(false);

		// �G�t�F�N�g�I�u�W�F�N�g�̐���
		m_airEffect = GetStage()->AddGameObject<AirJetEffect>(GetThis<TemplateObject>());
		m_shieldEffect = GetStage()->AddGameObject<ShieldEffect>(GetThis<TemplateObject>());
		m_aligment = GetStage()->AddGameObject<ArrowEffect>(GetThis<TemplateObject>());
		m_particle = GetStage()->AddGameObject<MultiParticle>();
	}


	void Player::OnUpdate()
	{
		// �X�e�[�W�ƃX�e�[�W�X�e�[�g�̎擾
		const auto& state = GetStageState<GameStage>();

		// �J�n���̓�������Q�[�����͈̔͂Ȃ�
		if (GetBetween(state, GameStage::StartMove, GameStage::DeathDrop))
		{
			// A�{�^�����͗L���ł̊֐�����
			if (state == GameStage::GameNow)
			{
				if (m_firePossible && m_jumpCount < m_jumpLimit)
				{
					if (GetPushA()) OnRushA();
				}

				// �Ə��̉�]����
				RotateAligment();

				// �v���C���[�̉�]�֐�
				RotatePlayer();
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

			// �W�����v�񐔂�0���傫��������
			if (m_jumpCount > 0)
			{
				RecoveryAirShock();
			}

			// ���S���̃h���b�v�X�e�[�g�Ȃ�
			if (state == GameStage::DeathDrop)
			{
				DeathDrop();
			}

			// �����@�̃`�F�b�N
			BlowerBetweenCheck();

			// �v���C���[�̈ړ��֐�
			MovePlayer();

			// �ړ����̌�����
			MoveReduction();

			// �A�j���[�V�����̍Đ�
			AnimationUpdate();

			// �G�t�F�N�g�`��֐�
			EffectUpdate();
		}

		if (state == GameStage::StartMove)
		{
			m_airEffect.lock()->SetDrawActive(false);
		}

		// �S�[���X�e�[�g�Ȃ�
		if(state == GameStage::Goal || state == GameStage::DeathDrop)
		{
			vector<Vec3> point;
			m_aligment.lock()->UpdateEffect(point);
			m_airEffect.lock()->SetDrawActive(false);
			m_shieldEffect.lock()->SetDrawShield(false);
		}


		// �f�o�b�O������
		Debug::Log(L"���W : ", m_position);
		Debug::Log(L"�ړ��� : ", m_velocity);
		Debug::Log(L"���Z�ړ��� : ", m_meddleVelo);
		Debug::Log(L"�����x : ", m_acsel);
		Debug::Log(L"�W�����v�� : ", m_jumpCount);
		Debug::Log(L"�V�[���h�̐� : ", m_shieldCount);
		Debug::Log(L"���G���� : ", m_invincibleTime - m_damageTime);
		Debug::Log(L"�X���[�������O : ", m_sRingCount);
		Debug::Log(m_isAir != false ? L"��" : L"�ڒn");
		Debug::Log(m_firePossible != false ? L"���ˉ�" : L"���˕s��");
		Debug::Log(m_cannonFire != false ? L"���ˌ�" : L"�ʏ�");
	}

	// A�{�^���𗣂�����
	void Player::OnRushA()
	{
		// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
		Vec2 stick = GetLStickValue();
		m_velocity = (IsInputLStick() ? stick.round(1) : m_deffVelo) * m_veloSpeed;
		if (!m_isAir && stick.y > 0.0f) m_velocity.y = -stick.round(1).y * m_veloSpeed;
		
		// �����o�ϐ��̐ݒ�
		m_isAir = true;
		m_acsel = m_maxAcsel;
		m_cannonFire = false;
		m_meddleVelo.zero();
		m_jumpCount++;
		m_jumpRecoveryTime = 0.0f;

		// �r�̃A�j���[�V������ύX
		m_armDraw->ChangeCurrentAnimation(L"FIRE");

		// SE�̍Đ�
		StartSE(L"AIRSHOCK_SE", 0.5f);
	}

	// �R���W�����ɏՓ˂�����
	void Player::OnCollisionEnter(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		// �X�e�[�W�ƃX�e�[�W�X�e�[�g�̎擾
		const auto& stage = GetTypeStage<GameStage>(false);
		if (stage)
		{
			// �X�e�[�g���J�n���o�ړ��Ȃ�
			const auto& state = stage->GetStageState();
			if (state == GameStage::StartMove)
			{
				// �X�e�[�g���Q�[�����ɐݒ肵�A�J�����Ƀ^�[�Q�b�g�ݒ肷��
				stage->SetStageState(GameStage::GameNow);
				stage->GetGameCamera()->SetTargetObject(GetThis<Player>());
			}
		}

		if (other->FindTag(L"Block"))
		{
			BlockEnter(other, hitPoint);
		}
		if (other->FindTag(L"Spike"))
		{
			SpikeEnter(other, hitPoint);
		}
		if (other->FindTag(L"Cannon"))
		{
			CannonEnter(other);
		}
		if (other->FindTag(L"Goal"))
		{
			const auto& stage = GetTypeStage<GameStage>();
			const auto& camera = stage->GetGameCamera();
			if (camera)
			{
				CannonEnter(other);
				camera->RemoveTarget();
				stage->SetStageState(GameStage::Goal);
			}
		}
		if (other->FindTag(L"Convayor"))
		{
			ConvayorEnter(other, hitPoint);
		}
		if (other->FindTag(L"Ring"))
		{
			RingEnter(other);
		}
		if (other->FindTag(L"Rabbit"))
		{
			RabbitEnter(other, hitPoint);
		}
		if (other->FindTag(L"Death"))
		{
			// ���S���̐ݒ������
			m_shieldCount = 0;
			StartSE(L"SHIELD_D_SE", 1.5f);
			DeathSetup();
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
		if (other->FindTag(L"Rabbit"))
		{
			RabbitExcute(other, hitPoint);
		}
		if (other->FindTag(L"Cannon"))
		{
			if (!m_cannonStandby) CannonEnter(other);
		}
		if (other->FindTag(L"Convayor"))
		{
			ConvayorExcute(other, hitPoint);
		}
	}


	void Player::OnCollisionExit(const CollisionPair& Pair)
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

	void Player::Reset()
	{
		SetPosition(m_startPos);
		m_rotation.zero();
		m_velocity = Vec2(-5.0f, 0.0f);
		m_meddleVelo.zero();
		m_jumpCount = 0;
		m_sRingCount = 0;
		m_shieldCount = 1;
		m_damageTime = 0.0f;
		m_acsel = 7.5f;
		m_jumpRecoveryTime = 0.0f;
		m_isAir = true;
		m_isBlower = false;
		m_isInvincible = false;
		m_firePossible = true;
		m_cannonFire = false;
		m_cannonStandby = false;
		m_ptrColl->SetUpdateActive(true);

		const auto& stage = GetTypeStage<GameStage>();
		const auto& camera = stage->GetGameCamera();
		camera->ResetCamera();
	}

	// �ړ��֐�
	void Player::MovePlayer()
	{
		// �O�t���[������̃f���^�^�C�����擾
		float deltaTime = DELTA_TIME * m_timeSpeed;

		// ���݂̍��W���擾
		m_position = GetPosition();

		// �|�W�V�����Ɉړ��x�N�g���Ƒ��x�Ɖ����x�ƃf���^�^�C���Ŋ|�����������Z
		m_position.x += -(m_velocity.x + m_meddleVelo.x) * m_speed * m_acsel * deltaTime;
		m_position.y += -(m_velocity.y + m_meddleVelo.y) * m_speed * m_acsel * deltaTime;

		// ���W�̍X�V
		SetPosition(m_position);

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
		float deltaTime = DELTA_TIME * m_timeSpeed;

		// �󒆂Ȃ�
		if (m_isAir)
		{
			// �����x��1.0���傫������������x���̓�{���f���^�^�C���Ŋ|�������Ō��Z�A������������1.0�ɏC��
			m_acsel > 1.0f ? m_acsel -= deltaTime * (m_acsel * 2.0f) : m_acsel = 1.0f;

			// Y���ړ��x�N�g�����d�͂ƃf���^�^�C���Ŋ|�������Ō��Z
			Vec2 tempVelo = m_meddleVelo;
			tempVelo.x = 0.0f;

			// �����@�ɓ������Ă��Ȃ��A���Z�ړ���Y�������Ƃ킸0.0���傫��������
			if (!m_isBlower && !(tempVelo.length() > 0.0f)) m_velocity.y -= m_gravity * deltaTime;
		}
		else
		{
			// �����ʂ�X���ړ��x�N�g���̐����Ō��߂�
			float decrease = deltaTime * m_maxAcsel;
			decrease *= m_velocity.x > 0.0f ? 1.0f : -1.0f;

			// X���ړ��x�N�g����0.01���傫��������(������킸)
			if (m_velocity.x > 0.1f || m_velocity.x < -0.1f) m_velocity.x -= decrease;
			else m_velocity.x = 0.0f;

			// Y���ړ��x�N�g����0.25���傫��������
			if (m_velocity.y > 0.25f) m_velocity.y -= DELTA_TIME;
			else m_velocity.y = 0.25f;
		}

		// ��C���ˌ�ŁA�����x���ő�����x�ȉ��ɂȂ�����@
		if (m_cannonFire && m_acsel <= m_maxAcsel) 
		{
			// ���ˌ��Ԃ�����
			m_cannonFire = false;
		}

		if (m_activeCannon.lock())
		{
			if (!m_activeCannon.lock()->GetFire())
			{
				// �|�C���^�̕ێ�������
				m_activeCannon.reset();
			}
		}
	}

	void Player::RecoveryAirShock()
	{
		// �W�����v�񐔂�0�Ȃ�I��
		if (m_jumpCount == 0) return;

		// ���J�o���[�o�ߎ��Ԃ����J�o���[���Ԉȏ�ɂȂ�����
		if (m_jumpRecoveryTime >= m_jumpRecoveryLimit)
		{
			m_jumpCount--;
			m_jumpRecoveryTime = 0.0f;
		}

		// ���J�o���[�o�ߎ��Ԃ��f���^�^�C���ŉ��Z
		m_jumpRecoveryTime += DELTA_TIME;
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
			// �X�e�B�b�N���͂����邩�Ō�����ݒ�
			Vec2 dir = IsInputLStick() ? stick : m_velocity;

			// ����X���̐������ŁA�r�͌�����radian�ɕϊ�
			body = (dir.x > 0.0f) * XM_PI;
			arm = atan2f(dir.y, dir.x) + XM_PIDIV2;
		}
		else
		{
			// �X�e�B�b�N���̗͂L���Őݒ�(�L�F�X�e�B�b�N����,���F�ړ��ʂ����邩�Őݒ�(�L�F�ړ���,���F�f�t�H���g�ړ���))
			Vec2 dir = IsInputLStick() ? stick : m_velocity != Vec2(0.0f) ? m_velocity : m_deffVelo;

			// �X�e�B�b�N���̗͂L���Őݒ�(�L�F���������W�A���ɕϊ�,���F�f�t�H���g�ړ��ʂ����W�A���ɕϊ�)
			arm = IsInputLStick() ? atan2f(dir.y, dir.x) + XM_PIDIV2 : atan2f(m_deffVelo.x, -m_deffVelo.y);
		} 

		// ���[�e�[�V�����̍X�V
		SetRotation(Vec3(0.0f, body, 0.0f));
		m_arm.lock()->SetRotation(Vec3(0.0f, 0.0f, arm));
	}

	// �O���̉�]�`��
	void Player::RotateAligment()
	{
		// �X�e�B�b�N���͂����邩�̐^�U
		const bool& stickInput = IsInputLStick();
		if (stickInput)
		{
			// �t���[�����[�g(�f���^�^�C�����ƕϓ������)
			const float perSecond = 1.0f / 60.0f;

			// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
			const float decrease = 10.0f;
			Vec2 velo = GetLStickValue().round(1) / decrease;

			// �ߖڂƐߖڂ̃J�E���^
			const int mileNum = 40;
			float mileCount = 1.0f;

			// �ߖڍ��W�z��
			vector<Vec3> points;

			// �ߖڂ̐����[�v���Đߖڂ̍��W��ݒ�
			for (size_t i = 0; i < mileNum; i++)
			{
				Vec2 pos = -velo * m_speed * m_maxAcsel * mileCount;
				velo.y -= m_gravity * perSecond / (decrease * 2.0f) / (mileNum / 10.0f);
				points.push_back(Vec3(pos.x, pos.y, 0.0f));
				mileCount += (decrease / mileNum);
			}
			m_aligment.lock()->UpdateEffect(points);
		}

		// �ړ��K�C�h���C�����X�V
		m_aligment.lock()->SetDrawActive(stickInput && m_firePossible);
	}

	// �A�j���[�V�����̍X�V
	void Player::AnimationUpdate()
	{
		// �O�t���[������̃f���^�^�C���ɃQ�[���X�s�[�h���|���������擾
		float deltaTime = DELTA_TIME * m_timeSpeed;

		// �A�j���[�V�������I����Ă���ҋ@��Ԃɂ���
		if (m_bodyDraw->IsTargetAnimeEnd()) m_bodyDraw->ChangeCurrentAnimation(L"WALK");
		if (m_armDraw->IsTargetAnimeEnd()) m_armDraw->ChangeCurrentAnimation(L"WALK");

		// �A�j���[�V�����̍X�V
		m_bodyDraw->UpdateAnimation(deltaTime);
		m_armDraw->UpdateAnimation(deltaTime);
	}

	// �G�t�F�N�g�̍X�V
	void Player::EffectUpdate()
	{
		// �e�G�t�F�N�g�I�u�W�F�N�g�̍X�V����
		m_shieldEffect.lock()->UpdateEffect();
		m_shieldEffect.lock()->SetDrawShield(m_shieldCount > 0 && !m_cannonStandby);
		m_airEffect.lock()->SetDrawActive(m_firePossible && !m_cannonFire);

		// ���ˉ\����C���ˌ�łȂ����
		if (m_firePossible && !m_cannonFire) m_airEffect.lock()->UpdateEffect();
		
		// ��C���ˌ�Ȃ�
		if (m_cannonFire)
		{
			// ���p�[�e�B�N���̏�����
			const auto& particle = m_particle.lock()->InsertParticle(2);
			particle->SetEmitterPos(m_position);
			particle->SetTextureResource(L"SMOKE_TX");
			particle->SetMaxTime(3.0f);
			particle->SetDrawOption(Particle::Normal);

			// ���������X�v���C�g��z��Ŏ擾
			vector<ParticleSprite>& pSpriteVec = particle->GetParticleSpriteVec();

			// �X�v���C�g�̐����[�v
			for (auto& sprite : pSpriteVec)
			{
				// �X�v���C�g�̏������ݒ�(�ړ��ʁE�X�P�[���E���[�e�[�V����)
				sprite.m_Velocity = Vec3(m_velocity.x, m_velocity.y, 0.0f).normalize();
				sprite.m_LocalScale = Vec2(m_acsel > 2.0f ? m_acsel / 2.5f : 0.0f);
				sprite.m_LocalQt.rotationZ(Utility::DegToRad(Utility::RangeRand(360.0f, 0.0f)));
			}
		}
	}

	// ��C���ˑҋ@��
	void Player::CannonStandby(float acsel)
	{
		// ��C�̃|�C���^������Ȃ�
		if (m_activeCannon.lock())
		{
			// ��C�̃V�F�A�h�|�C���^
			const auto& cannon = m_activeCannon.lock();

			// ���ˎ��̎��Ԃ̎擾
			const float& fireTime = cannon->GetFireTime();

			// �`��R���|�[�l���g����A�j���[�V�����̍Đ����Ԃ��擾
			const auto& drawPtr = cannon->GetComponent<PNTBoneModelDraw>();

			// �Đ����Ԃ����ˎ��̎��Ԃ��߂�����
			if (drawPtr->GetCurrentAnimationTime() > fireTime)
			{
				// �����o�ϐ��̐ݒ�
				m_acsel = acsel;
				m_isAir = true;
				m_firePossible = true;
				m_cannonFire = true;
				m_cannonStandby = false;
				m_meddleVelo.zero();

				// ��C��Z�������W�A���ɕϊ����A�ړ��ʂ�ݒ肷��
				float rad = cannon->GetRotation().z - XM_PIDIV2;
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				// SE�̍Đ�
				StartSE(L"CANNON_SE", 0.75f);
			}
			else
			{
				// ���g�̍��W���C�̒��ɐݒ肷��
				SetPosition(cannon->GetPosition());
			}
		}
	}

	// ���G���Ԍo��
	void Player::InvincibleTimer()
	{
		// �_���[�W����̌o�ߎ��Ԃ��f���^�^�C���ŉ��Z
		m_damageTime += DELTA_TIME;

		// �o�ߎ��Ԃ����G���Ԉȏ�ɂȂ�����
		if (m_invincibleTime <= m_damageTime)
		{
			// �o�ߎ��Ԃ̃��Z�b�g�Ɩ��G�̉���
			m_damageTime = 0.0f;
			m_isInvincible = false;
		}
	}

	// ���S���̐ݒ�
	void Player::DeathSetup()
	{
		// �X�e�[�W�X�e�[�g���Q�[�����Ȃ�
		const auto& stage = GetTypeStage<GameStage>();
		if (stage->GetStageState() == GameStage::GameNow)
		{
			// SE�̍Đ�
			StartSE(L"DAMAGE_SE", 0.75f);

			// �X�e�[�g�����S���̗����ɂ��A�J�����̃^�[�Q�b�g������
			stage->SetStageState(GameStage::DeathDrop);
			stage->GetGameCamera()->RemoveTarget();

			// ���W�ƈړ��ʂƉ����x��ݒ�
			SetPosition(GetPosition() + Vec3(0.0f, 0.0f, -1.5f));
			m_velocity = m_deffVelo * 2.5f;
			m_acsel = 2.5f;

			// �R���W�������A�N�e�B�u��
			m_ptrColl->SetUpdateActive(false);
		}
	}

	// ���S���̗���
	void Player::DeathDrop()
	{
		// ���S���̍��W�ƌ��݂̍��W���璷�������߂�
		const auto& stage = GetTypeStage<GameStage>();
		const Vec3& deathPos = stage->GetGameCamera()->GetCurrentPos();
		float length = (deathPos - GetPosition()).length();

		// Z����]����������
		Vec3 rot = GetRotation();
		rot.z += DELTA_TIME * 5.0f;
		if (rot.z >= XM_PIDIV2) rot.z = -XM_PIDIV2;

		// ���[�e�[�V�����̍X�V
		SetRotation(rot);
		m_arm.lock()->SetRotation(rot + Vec3(0.0f, 0.0f, XM_PI));

		// ��������ʊO�̋����ɍs������
		if (length >= 22.5f) 
		{
			// �X�e�[�g�����S�ɂ��ASE���Đ�
			stage->SetStageState(GameStage::Death);
			stage->CreateSE(L"METAL_SE", 0.75f);
		}
	}

	// �����@�̃`�F�b�N
	void Player::BlowerBetweenCheck()
	{
		// �A�b�v�f�[�g�I�u�W�F�N�g�O���[�v�̃|�C���^�z��̎擾
		const auto& groupVec = GetStage()->GetSharedObjectGroup(L"Gimmick")->GetGroupVector();

		// �M�~�b�N�̊p�x�Ƒ����@�̒��ɋ��邩�̐^�U
		Gimmick::eAngle angle = Gimmick::Up;
		bool check = false;

		// �O���[�v�z��̃I�u�W�F�N�g�̐����[�v
		for (const auto& ptr : groupVec)
		{
			// �����@�Ɍ^�L���X�g�o������
			const auto& blower = dynamic_pointer_cast<Blower>(ptr.lock());
			if (blower)
			{
				// �v���C���[�������@�̑����͈͂ɋ��邩
				check = blower->CheckBetween(GetPosition());
				if (check)
				{
					// �M�~�b�N�̊p�x�̎擾
					angle = blower->GetAngle();
					break;
				}
			}
		}

		// �����͈͂ɋ�����
		if (check)
		{
			// �p�x���ɉ��Z�ړ��ʂ�ݒ肷��
			switch (angle)
			{
			case Gimmick::Up:
				Debug::Log(L"�����@��");
				UpMeddleVelocity();
				break;

			case Gimmick::Down:
				Debug::Log(L"�����@��");
				DownMeddleVelocity();
				break;

			case Gimmick::Left:
				LeftMeddleVelocity();
				break;

			case Gimmick::Right:
				RightMeddleVelocity();
				break;

			default:
				break;
			}
		}	
		else
		{
			// �����@�ɓ����������̐^�U�����Z�b�g
			m_isBlower = false;

			// ���Z�ړ��ʂ̌����֐�
			ReductionMeddleVelocity();
		}
	}

	// ������̑����@�ɓ������Ă��鎞
	void Player::UpMeddleVelocity()
	{
		// �����@�͈͓̔����̐^�U��true
		m_isBlower = true;

		// �����ʂ��ړ���Y���̐����ɉ����ăf���^�^�C����ݒ肷��
		float decrease = DELTA_TIME;
		decrease *= m_velocity.y > 0.0f ? 1.0f : -1.0f;

		// �ړ���Y����������킸�����ʂ��傫�������猸�炷
		bool amountVelo = (m_velocity.y > decrease || m_velocity.y < decrease);
		if (amountVelo) m_velocity.y -= decrease * m_maxAcsel;
		else m_velocity.y = decrease;

		// ���Z�ړ��ʂ����x���傫����������Z�ړ��ʂ����炷(�ړ��ʂ��}�C�i�X�ł������ɏ㏸)
		if (m_meddleVelo.y > -m_speed) m_meddleVelo.y -= DELTA_TIME * m_maxAcsel;
	}

	// �������̑����@�ɓ������Ă��鎞
	void Player::DownMeddleVelocity()
	{
		// �ړ���Y���𑝂₷
		m_velocity.y += DELTA_TIME * m_maxAcsel;
	}

	// �������̑����@�ɓ������Ă��鎞
	void Player::LeftMeddleVelocity()
	{
		// ���Z�ړ���X���𑝂₷
		m_meddleVelo.x += DELTA_TIME * m_maxAcsel * m_speed;
	}

	// �E�����̑����@�ɓ������Ă��鎞
	void Player::RightMeddleVelocity()
	{
		// ���Z�ړ���X�������炷
		m_meddleVelo.x -= DELTA_TIME * m_maxAcsel * m_speed;
	}

	// ���Z�ړ��ʂ̌���
	void Player::ReductionMeddleVelocity()
	{
		// �����ʂ����Z�ړ���X���̐����ɉ����Đݒ�
		float decrease = DELTA_TIME;
		decrease *= m_meddleVelo.x > 0.0f ? 1.0f : -1.0f;

		// ���Z�ړ���X����������킸0.1���傫�������猸�炷
		bool amountVelo = (m_meddleVelo.x > 0.1f || m_meddleVelo.x < -0.1f);
		if (amountVelo) m_meddleVelo.x -= decrease;
		else m_meddleVelo.x = 0.0f;

		// �����ʂ����Z�ړ���Y���̐����ɉ����Đݒ�
		decrease = DELTA_TIME;
		decrease *= m_meddleVelo.y > 0.0f ? 1.0f : -1.0f;

		// ���Z�ړ���Y����������킸0.1���傫�������猸�炷
		amountVelo = (m_meddleVelo.y > 0.1f || m_meddleVelo.y < -0.1f);
		if (amountVelo) m_meddleVelo.y -= decrease * 5.0f;
		else m_meddleVelo.y = 0.0f;
	}

	// �u���b�N�ɏՓ˂����u��
	void Player::BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		// �Փ˕����^�U
		bool upper, under, left, right;
		upper = CollHitUpper(hitPos, objPos, helf) && !BlockCheck(objPos + Vec3(0.0f, 1.0f, 0.0f));
		under = CollHitUnder(hitPos, objPos, helf) && !BlockCheck(objPos + Vec3(0.0f, -1.0f, 0.0f));
		left = CollHitLeft(hitPos, objPos, helf) && !BlockCheck(objPos + Vec3(1.0f, 0.0f, 0.0f));
		right = CollHitRight(hitPos, objPos, helf) && !BlockCheck(objPos + Vec3(-1.0f, 0.0f, 0.0f));

		// Z���̊p�x���擾�ŃX���[�v���ǂ����𔻒f
		const float& deg = cube->GetDegreeAngle().z;
		if (deg != 0.0f)
		{
			// �X���[�v�̌����ŏォ��A������̉����������s��
			const auto& angle = cube->GetAngleType();
			if (Utility::GetBetween<CubeObject::eType>(angle, CubeObject::SlopeUL, CubeObject::SlopeUR))
			{
				BlockUpperHit(objPos, helf);
			}
			if (Utility::GetBetween<CubeObject::eType>(angle, CubeObject::SlopeDL, CubeObject::SlopeDR))
			{
				BlockUnderHit(objPos, helf);
			}
		}
		else
		{
			// �R���W�����ɑ΂��ďォ��Փ�
			if (upper)
			{
				BlockUpperHit(objPos, helf);
				return;
			}

			// ������Փ�
			if (under)
			{
				BlockUnderHit(objPos, helf);
				return;
			}
		}

		// ������Փ�
		if (left)
		{
			BlockLeftHit(objPos, helf);
			return;
		}

		// �E����Փ�
		if (right)
		{
			BlockRightHit(objPos, helf);
			return;
		}
	}

	// �u���b�N�̏ォ��Փ˂������̉�������
	void Player::BlockUpperHit(const Vec3& objPos, const Vec3& helf)
	{
		// ��Ƀu���b�N�����邩�̃`�F�b�N
		if (BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

		// �G�A�V���b�N�g�p�\�ɂ���
		m_firePossible = true;

		// �ړ��ʂ��������ɂ���
		if (m_velocity.y > 0.0f)
		{
			// �����x���������傫��������
			if (m_acsel > 2.5f || m_velocity.y > 2.5f)
			{
				// �ړ��ʂ𔽓]�����A�����ɂ���
				m_velocity.y *= -0.5f;

				// �o�E���h����SE���Đ�
				BlockBoundSE();
			}
		}
	}

	// �u���b�N�̉�����Փ˂������̉�������
	void Player::BlockUnderHit(const Vec3& objPos, const Vec3& helf)
	{
		// ���Ƀu���b�N�����邩�̃`�F�b�N
		if (BlockCheck(Vec3(objPos.x, objPos.y - (helf.y * 2.0f), 0.0f))) return;

		// �G�A�V���b�N�g�p�\�ɂ���
		m_firePossible = true;

		// �ړ��ʂ�������Ȃ�
		if (m_velocity.y < 0.0f)
		{
			// ���]�����A����������
			m_velocity.y *= -1.0f;

			// �o�E���h����SE���Đ�
			BlockBoundSE();
		}
	}

	// �u���b�N�̍�����Փ˂������̉�������
	void Player::BlockLeftHit(const Vec3& objPos, const Vec3& helf)
	{
		// ���Ƀu���b�N�����邩�̃`�F�b�N
		if (BlockCheck(Vec3(objPos.x - (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// �ړ��ʂ𔼌������]������
		m_velocity.x *= -0.5f;

		// �o�E���h����SE���Đ�
		BlockBoundSE();
	}

	// �u���b�N�̉E����Փ˂������̉�������
	void Player::BlockRightHit(const Vec3& objPos, const Vec3& helf)
	{
		// �E�Ƀu���b�N�����邩�̃`�F�b�N
		if (BlockCheck(Vec3(objPos.x + (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// �ړ��ʂ𔼌������]������
		m_velocity.x *= -0.5f;

		// �o�E���h����SE���Đ�
		BlockBoundSE();
	}

	// �u���b�N�̃o�E���h��SE�Đ�
	void Player::BlockBoundSE()
	{
		// ���x�̕��ς��ړ��ʑ��x�Ŋ������l�����ʂƂ���SE���Đ�
		float volume = ((m_velocity.y + m_meddleVelo.y + m_acsel) / 3.0f) / m_veloSpeed;
		StartSE(L"ROBOT_BOUND_SE", volume);
	}

	// �u���b�N�ɏՓ˂���������
	void Player::BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		// �ォ��Փ˂��Ă�����
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// �p�����[�^�̐ݒ�
			m_velocity.y = 0.25f;
			m_acsel = 1.0f;
			m_isAir = false;

			// �ʏ킩�A�X���[�v���Őݒ�
			const auto& angle = cube->GetAngleType();
			switch (angle)
			{
			case CubeObject::Normal:
				SetPosition(GetPosition().x, objPos.y + helf.y + (GetScale().y / 2.0f), 0.0f);
				break;

			case CubeObject::SlopeUL:
				m_velocity.x = 0.175f;
				break;

			case CubeObject::SlopeUR:
				m_velocity.x = -0.175f;
				break;

			default:
				break;
			}
		}
	}

	// �X�p�C�N�ƏՓ˂����u��
	void Player::SpikeEnter(const shared_ptr<GameObject>& obj, const Vec3& hitPos)
	{
		// �X�p�C�N�I�u�W�F�N�g�ɃL���X�g
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// �p�����[�^�̎擾
		Vec3 spikePos = spike->GetPosition();
		Vec3 scale = spike->GetScale();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// �Փ˕����^�U
		const auto& groupVec = GetStage()->GetSharedObjectGroup(L"Update")->GetGroupVector();
		bool upper, under, left, right;
		upper = CollHitUpper(hitPos, spikePos, helfScale) && !BlockCheck(groupVec, Vec3(spikePos + Vec3(0.0f, scale.y, 0.0f)));
		under = CollHitUnder(hitPos, spikePos, helfScale) && !BlockCheck(groupVec, Vec3(spikePos + Vec3(0.0f, -scale.y, 0.0f)));
		left = CollHitLeft(hitPos, spikePos, helfScale) && !BlockCheck(groupVec, Vec3(spikePos + Vec3(-scale.x, 0.0f, 0.0f)));
		right = CollHitRight(hitPos, spikePos, helfScale) && !BlockCheck(groupVec, Vec3(spikePos + Vec3(scale.x, 0.0f, 0.0f)));

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

		// �X�p�C�N�̕������������Ȃ�n�ʂƓ�������
		const auto& angle = spike->GetAngle();
		if (angle == Gimmick::Down)
		{
			m_firePossible = true;
			BlockExcute(obj, hitPos);
		}
		else
		{
			if (!m_isInvincible)
			{
				SpikeEnter(obj, hitPos);
			}
		}
	}

	// ��C�ƏՓ˂�����
	void Player::CannonEnter(const shared_ptr<GameObject>& cannon)
	{
		// ��C�^�ɃL���X�g
		const auto& ptr = dynamic_pointer_cast<Cannon>(cannon);
		if (ptr)
		{
			if (m_activeCannon.lock() != ptr)
			{
				// ���ˏ����֐�
				ptr->OnFire();

				// �������C�̃|�C���^��ێ�
				m_activeCannon.reset();
				m_activeCannon = ptr;

				// �p�����[�^�̐ݒ�
				m_isAir = false;
				m_firePossible = false;
				m_cannonStandby = true;

				// �����ړ��s�ɐݒ�
				m_velocity = Vec2(0.0f);
				m_acsel = 1.0f;
			}
		}
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
			if (BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

			// �G�A�V���b�N�g�p�\�ɂ���
			m_firePossible = true;
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
			// �߂荞�݂�����
			Vec3 pos = GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			SetPosition(pos);

			// ������ւ̈ړ��𖳂���(��葱����ׂ�)
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
		}
	}

	// �G�̃E�T�M�ƏՓ˂�����
	void Player::RabbitEnter(const shared_ptr<GameObject>& other, const Vec3& hitPos)
	{			
		// �E�T�M�^�ɃL���X�g
		const auto& rabbit = dynamic_pointer_cast<Rabbit>(other);
		if (rabbit)
		{
			if (!m_isInvincible)
			{
				// �V�[���h���񖇈ȏ゠��Ȃ�
				if (m_cannonFire)
				{
					// �E�T�M�̃X�e�[�g�����S�ɐݒ�
					rabbit->SetState(Rabbit::Death);
					return;
				}
				if (m_shieldCount >= 2)
				{
					// �E�T�M�̃X�e�[�g�����S�ɐݒ�
					rabbit->SetState(Rabbit::Death);
					m_shieldCount--;
				}
				else
				{
					// �Փ˕����^�U
					Vec3 helfScale = rabbit->GetScale() / 2.0f;

					if (CollHitLeft(hitPos, hitPos, helfScale))
					{
						DamageKnockBack(Vec2(1.5f, -0.5f));
						return;
					}
					if (CollHitRight(hitPos, hitPos, helfScale))
					{
						DamageKnockBack(Vec2(-1.5f, -0.5f));
						return;
					}
				}
			}
		}
	}

	// �G�̃E�T�M�ƏՓ˂���������
	void Player::RabbitExcute(const shared_ptr<GameObject>& rabbit, const Vec3& hitPos)
	{
		// ���G������Ȃ����
		if (!m_isInvincible)
		{
			// �E�T�M�ƏՓ˂������̊֐������s
			RabbitEnter(rabbit, hitPos);
		}
	}

	// �_���[�W�m�b�N�o�b�N
	void Player::DamageKnockBack(const Vec2& velocity)
	{
		// ���ԑ��x��ʏ푬�x�ŏ㏑��
		m_firePossible = false;
		m_velocity = velocity;
		m_acsel = m_damageAcsel;
		m_isAir = true;

		// �A�j���[�V�������_���[�W��Ԃɂ���
		m_bodyDraw->ChangeCurrentAnimation(L"DAMAGE");

		// �O���̕\�����I�t
		m_aligment.lock()->SetDrawActive(false);

		// ���G������Ȃ����
		if (!m_isInvincible)
		{
			// �V�[���h�̐���1���ȏ�Ȃ�
			if (m_shieldCount >= 1)
			{
				// �V�[���h�����炵�A���G���Ԃɂ���
				m_shieldCount--;
				m_isInvincible = true;

				// SE�̍Đ�
				StartSE(L"SHIELD_D_SE", 1.5f);
			}
			else
			{
				// ���S���̐ݒ������
				DeathSetup();
			}
		}
	}

	// �����O�ɏՓˎ��̃`�F�b�N�֐�
	void Player::RingEnter(const shared_ptr<GameObject>& ring)
	{
		// �����O�Ɍ^�L���X�g����
		const auto& ringPtr = dynamic_pointer_cast<Ring>(ring);
		if (ringPtr)
		{
			// �����O�Ɏ擾��Ԃ𑗂�
			ringPtr->IsGetRing();

			// �����O�̃T�C�Y�ɉ����Ď擾����������
			const auto& size = ringPtr->GetRingSize();
			switch (size)
			{
			case Ring::Big:
				AddShield();
				StartSE(L"RING_SE", 0.75f);
				break;

			case Ring::Small:
				m_sRingCount++;
				StartSE(L"RING_SE", 0.35f);
				break;

			default:
				break;
			}

			// �����������O�̎擾����25�ȏ�ɂȂ�����
			if (m_sRingCount >= m_sRingLimit)
			{
				// �V�[���h�𑝂₵�ăJ�E���^�����Z�b�g
				AddShield();
				m_sRingCount = 0;
			}
		}
	}
}