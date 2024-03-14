/*! 
@file Player.cpp
@brief �v���C���[�{��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;
	using namespace Utility;
	using namespace CubeParam;
	using namespace GimmickAngle;

	// �������̏���
	void Player::OnCreate()
	{
		// �p�����̐������̊֐������s����
		TemplateObject::OnCreate();

		// �r���f���p�I�u�W�F�N�g�̐���
		m_arm = GetStage()->AddGameObject<TemplateObject>();

		// ���̃g�����X�t�H�[���̐ݒ�
		SetPosition(m_position);
		SetScale(m_scale);
		
		// �r�̃g�����X�t�H�[���̐ݒ�
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
		m_arrow = GetStage()->AddGameObject<ArrowEffect>(GetThis<TemplateObject>());
		m_particle = GetStage()->AddGameObject<MultiParticle>();
	}

	// ���t���[���X�V����
	void Player::OnUpdate()
	{
		// �X�e�[�W�ƃX�e�[�W�X�e�[�g�̎擾
		const auto& state = GetStageState<GameStage>();

		// �J�n���̓�������Q�[�����͈̔͂Ȃ�
		if (GetBetween(state, GameStage::StartMove, GameStage::DeathDrop))
		{
			// �Q�[������
			if (state == GameStage::GameNow)
			{
				// �G�A�V���b�N���ˉ\�ŃW�����v�񐔂�����ȉ���
				if (m_status(eStatus::IsFirePossible) && m_jumpCount < m_jumpLimit)
				{
					// A�{�^�����͂���������
					if (GetPushA()) OnRushA();
				}

				// �Ə��̉�]����
				UpdateArrow();

				// �v���C���[�̉�]�֐�
				RotatePlayer();
			}

			// ��C�ҋ@��
			if (m_status(eStatus::IsCannonStandby))
			{
				// ��C���ˑҋ@�֐�(�����x)
				CannonStandby(10.0f);
			}

			// ���G���Ԓ��Ȃ�
			if (m_status(eStatus::IsInvincible))
			{
				// ���G���Ԃ̃^�C�}�[
				InvincibleTimer();
			}

			// �W�����v�񐔂�0���傫��������
			if (m_jumpCount > 0)
			{
				// �W�����v�񐔂̃��J�o��
				RecoveryAirShock();
			}

			// �Փ˂��������ǂ̃|�C���^�������
			if (m_currentWall.lock())
			{
				// �����ǂ��痣�ꂽ���̌���
				MoveWallExit();
			}

			// ���S���̃h���b�v�X�e�[�g�Ȃ�
			if (state == GameStage::DeathDrop)
			{
				// ���S���̗�������
				DeathDrop();
			}

			// �����@�̃`�F�b�N
			BlowerBetweenCheck();

			// �v���C���[�̈ړ��֐�
			MovePlayer();

			// �ړ����̌�����
			MoveReduction();

			// ��C���ˌ�
			AftterCannon();

			// �A�j���[�V�����̍Đ�
			AnimationUpdate();

			// �G�t�F�N�g�`��֐�
			EffectUpdate();
		}

		// �J�n���̐�����щ��o���Ȃ�
		if (state == GameStage::StartMove)
		{
			// �G�A�V���b�N�̃G�A�V���b�N���\��
			m_airEffect.lock()->SetDrawActive(false);
		}

		// �S�[�����܂��͎��S�������Ȃ�
		if(state == GameStage::Goal || state == GameStage::DeathDrop)
		{
			// �G�t�F�N�g���\��
			vector<Vec3> point;
			m_arrow.lock()->UpdateEffect(point);
			m_airEffect.lock()->SetDrawActive(false);
			m_shieldEffect.lock()->SetDrawShield(false);
		}

		// �f�o�b�O������
		//Debug::Log(L"���W : ", m_position);
		//Debug::Log(L"�ړ��� : ", m_velocity);
		//Debug::Log(L"���Z�ړ��� : ", m_meddleVelo);
		//Debug::Log(L"�����x : ", m_acsel);
		//Debug::Log(L"�W�����v�� : ", m_jumpCount);
		//Debug::Log(L"�V�[���h�̐� : ", m_shieldCount);
		//Debug::Log(m_isAir != false ? L"��" : L"�ڒn");
		//Debug::Log(m_firePossible != false ? L"���ˉ�" : L"���˕s��");
		//Debug::Log(m_cannonFire != false ? L"���ˌ�" : L"�ʏ�");
		//Debug::Log(L"�ړ��u���b�N�̏ォ : ", m_isAliveMoveBlock);
		//Debug::Log(L"���E����u���b�N�ɉ�����Ă��� : ", m_isHitMoveBlock);
	}

	// A�{�^������������
	void Player::OnRushA()
	{
		// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
		Vec2 stick = GetLStickValue();
		m_velocity = (IsInputLStick() ? stick.round(1) : m_deffVelo) * m_veloSpeed;

		// �n�ʂɋ���Ƃ��ɒn�ʂɌ������ē��͂��Ă�����Y���𔽓]
		if (!m_status(eStatus::IsAir) && stick.y > 0.0f) m_velocity.y = -stick.round(1).y * m_veloSpeed;
		
		// �p�����[�^�̐ݒ�
		m_status.Set(eStatus::IsAir) = true;
		m_status.Set(eStatus::IsCannonFire) = false;
		m_acsel = m_maxAcsel;
		m_meddleVelo.zero();
		m_jumpCount++;
		m_jumpRecoveryTime = 0.0f;

		// �X�e�B�b�N��Y�������߂ɓ��͂���Ă�����n�C�W�����v��Ԃɂ���
		m_status.Set(eStatus::IsHighJump) = (stick.y <= -0.75f);

		// �r�̃A�j���[�V������ύX
		m_armDraw->ChangeCurrentAnimation(L"FIRE");

		// SE�̍Đ�
		StartSE(L"AIRSHOCK_SE", 0.5f);
	}

	// �R���W�����ɏՓ˂�����
	void Player::OnCollisionEnter(const CollisionPair& Pair)
	{
		// �Փ˂����I�u�W�F�N�g�ƏՓˍ��W���擾
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

		// �^�O���u���b�N�Ȃ�
		if (other->FindTag(L"Block"))
		{
			// �u���b�N�ƏՓ˂����u�Ԃ̏��������s
			BlockEnter(other, hitPoint);
		}

		// �^�O�����Ȃ�
		if (other->FindTag(L"Spike"))
		{
			// ���ƏՓ˂����u�Ԃ̏��������s
			SpikeEnter(other, hitPoint);
		}

		// �^�O����C�Ȃ�
		if (other->FindTag(L"Cannon"))
		{
			// ��C�ƏՓ˂����u�Ԃ̏��������s
			CannonEnter(other);
		}

		// �^�O���S�[���Ȃ�
		if (other->FindTag(L"Goal"))
		{
			// �X�e�[�W����J�������擾
			const auto& stage = GetTypeStage<GameStage>();
			const auto& camera = stage->GetGameCamera();
			if (camera)
			{
				// ��C�ƏՓ˂����u�Ԃ̏��������s
				CannonEnter(other);

				// �J�����̃^�[�Q�b�g���O���A�S�[�����̃X�e�[�g�ɐ؂�ւ�
				camera->RemoveTarget();
				stage->SetStageState(GameStage::Goal);
			}
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
		
		// �^�O�������O�Ȃ�
		if (other->FindTag(L"Ring"))
		{
			// �����O�ɏՓ˂����u�Ԃ̏��������s
			RingEnter(other);
		}

		// �^�O���E�T�M�Ȃ�
		if (other->FindTag(L"Rabbit"))
		{
			// �E�T�M�ɏՓ˂����u�Ԃ̏��������s
			RabbitEnter(other, hitPoint);
		}

		// �^�O�������ǂȂ�
		if (other->FindTag(L"MoveWall"))
		{
			// �����ǂɏՓ˂����u�Ԃ̏��������s
			MoveWallEnter(other, hitPoint);

			// �u���b�N�ɏՓ˂����u�Ԃ̏��������s
			BlockEnter(other, hitPoint);
		}

		// �^�O�����S����R���W�����Ȃ�
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
		// �Փ˂����I�u�W�F�N�g�ƏՓˍ��W���擾
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		// �^�O���u���b�N�Ȃ�
		if (other->FindTag(L"Block"))
		{
			// �u���b�N�ɏՓ˂����������̏��������s
			BlockExcute(other, hitPoint);
		}

		// �^�O�����Ȃ�
		if (other->FindTag(L"Spike"))
		{
			// ���ɏՓ˂����������̏��������s
			SpikeExcute(other, hitPoint);
		}

		// �^�O���E�T�M�Ȃ�
		if (other->FindTag(L"Rabbit"))
		{
			// �E�T�M�ɏՓ˂����������̏��������s
			RabbitExcute(other, hitPoint);
		}

		// �^�O����C�Ȃ�
		if (other->FindTag(L"Cannon"))
		{
			// ��C���ˑҋ@������Ȃ��Ȃ��C�ƏՓ˂����u�Ԃ̏��������s
			if (!m_status(eStatus::IsCannonStandby)) CannonEnter(other);
		}

		// �^�O�������ǂȂ�
		if (other->FindTag(L"MoveWall"))
		{
			// �����ǂɏՓ˂����������̏��������s
			MoveWallEnter(other, hitPoint);
		}

		// �^�O���x���g�R���x�A�Ȃ�
		if (other->FindTag(L"Convayor"))
		{
			// �x���g�R���x�A�ɏՓ˂����������̏��������s
			ConvayorExcute(other, hitPoint);
		}
	}

	// �R���W�����Ƃ̏Փ˂��I�������
	void Player::OnCollisionExit(const CollisionPair& Pair)
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

	// �R���e�B�j���[�����Z�b�g�֐�
	void Player::Reset()
	{
		SetPosition(m_startPos);		// �J�n���W�ɍX�V
		m_rotation.zero();				// ������������
		m_velocity = Vec2(-5.0f, 0.0f); // �ړ��ʂ����ɐ�����Ԓl�ōX�V
		m_meddleVelo.zero();			// ���Z�ړ��ʂ�������
		m_jumpCount = 0;				// �W�����v�񐔂�������
		m_sRingCount = 0;				// �������O�̐���������
		m_shieldCount = 1;				// �V�[���h���ꖇ�̏�ԂɍX�V
		m_acsel = 7.5f;					// �����x���X�V
		m_jumpRecoveryTime = 0.0f;		// �W�����v���J�o���^�C����������
		m_status.Reset();				// ��ԃt���O��������

		// ��ԃt���O�́u�󒆂��ɋ��邩?�v�Ɓu�G�A�V���b�N���ˉ\��?�v���I����
		m_status.Set(eStatus::IsAir, eStatus::IsFirePossible) = true;

		// �R���W�������A�N�e�B�u��
		m_ptrColl->SetUpdateActive(true);

		// �X�e�[�W����J�������擾���āA�J������������
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

		// ���W�Ƒ̂̃��f���̃{�[���̔z����擾
		Vec3 armPos = m_position;
		const auto& bone = m_bodyDraw->GetVecLocalBones();

		// �r�Ɠ��̃��f���}�g���N�X�̃|�W�V����y���̍����ƃ{�[���̃A�j���[�V�����ʂ��|�W�V�������獷������
		armPos.y -= (m_armMat.getMajor3().y - m_bodyMat.getMajor3().y) - bone.at(1).getTranslation().y;

		// ���W�̍X�V
		SetPosition(m_position);
		m_arm.lock()->SetPosition(armPos);
	}

	// �ړ�������
	void Player::MoveReduction()
	{
		// �f���^�^�C�����擾
		float deltaTime = DELTA_TIME * m_timeSpeed;

		// �󒆂Ȃ�
		if (m_status(eStatus::IsAir))
		{
			// �����x��1.0���傫������������x���̓�{���f���^�^�C���Ŋ|�������Ō��Z�A������������1.0�ɏC��
			m_acsel > 1.0f ? m_acsel -= deltaTime * (m_acsel * 2.0f) : m_acsel = 1.0f;

			// Y���ړ��x�N�g�����d�͂ƃf���^�^�C���Ŋ|�������Ō��Z
			Vec2 meddleVelo = m_meddleVelo;
			meddleVelo.x = 0.0f;

			// �����@�ɓ������Ă��Ȃ��A���Z�ړ���Y�������Ƃ킸0.0�Ȃ�
			if (!m_status(eStatus::IsBlower) && !(meddleVelo.length() > 0.0f))
			{
				// ��������ł����ԂŁA�ړ���Y����0.0����1.5�͈͓̔��Ȃ�A�����ʂ𔼕��ɂ���
				float dropVal = (m_status(eStatus::IsHighJump) && GetBetween(m_velocity.y, 0.0f, 1.5f)) ? 0.5f : 1.0f;
				m_velocity.y -= m_gravity * deltaTime * dropVal;
			}
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
	}

	// �G�A�V���b�N�̃��J�o��
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
		if (m_status(eStatus::IsAir))
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
	void Player::UpdateArrow()
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
			m_arrow.lock()->UpdateEffect(points);
		}

		// �ړ��K�C�h���C�����X�V
		m_arrow.lock()->SetDrawActive(stickInput && m_status(eStatus::IsFirePossible));
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
		// �t���O�̎擾
		bool possible, cFire, cStandby;
		possible = m_status(eStatus::IsFirePossible);
		cFire = m_status(eStatus::IsCannonFire);
		cStandby = m_status(eStatus::IsCannonStandby);

		// �e�G�t�F�N�g�I�u�W�F�N�g�̍X�V����
		m_shieldEffect.lock()->UpdateEffect();
		m_shieldEffect.lock()->SetDrawShield(m_shieldCount > 0 && !cStandby);
		m_airEffect.lock()->SetDrawActive(possible && !cFire);

		// ���ˉ\����C���ˌ�łȂ����
		if (possible && !cFire) m_airEffect.lock()->UpdateEffect();
		
		// ��C���ˌ�Ȃ�
		if (cFire)
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
			const auto& animeTime = cannon->GetAnimationTime();

			// �Đ����Ԃ����ˎ��̎��Ԃ��߂�����
			if (animeTime > fireTime)
			{
				// �����o�ϐ��̐ݒ�
				m_acsel = acsel;
				m_status.Set(eStatus::IsAir, eStatus::IsFirePossible, eStatus::IsCannonFire) = true;
				m_status.Set(eStatus::IsCannonStandby) = false;
				m_meddleVelo.zero();

				// ��C��Z�������W�A���ɕϊ����A�ړ��ʂ�ݒ肷��
				float rad = cannon->GetRotation().z - XM_PIDIV2;
				if (floor(RadToDeg(rad)) == 90.0f) m_acsel -= 2.5f; // �������̎������n�ʂ��ђʂ��Ղ��׏�������
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				// �^�O���S�[���Ȃ�
				if (cannon->FindTag(L"Goal"))
				{
					// �N���A����SE���Đ�
					StartSE(L"CLEAR_SE", 1.0f);

					// �R���W�������A�N�e�B�u��
					m_ptrColl->SetUpdateActive(false);
					return;
				}

				// ��C���˂�SE�̍Đ�
				StartSE(L"CANNON_SE", 0.75f);
			}
			else
			{
				// ���g�̍��W���C�̒��ɐݒ肷��
				SetPosition(cannon->GetPosition());
			}
		}
	}

	// ��C���ˌ�
	void Player::AftterCannon() 
	{
		// ��C���ˌ�ŁA�����x���ő�����x�ȉ��ɂȂ�����@
		if (m_status(eStatus::IsCannonFire) && m_acsel <= m_maxAcsel)
		{
			// ���ˌ��Ԃ�����
			m_status.Set(eStatus::IsCannonFire) = false;
		}

		// ��C�̃|�C���^���ێ�����Ă����
		if (m_activeCannon.lock())
		{
			// ���ˑO�̑�C�Ȃ�
			if (!m_activeCannon.lock()->GetFire())
			{
				// �|�C���^�̕ێ�������
				m_activeCannon.reset();
			}
		}
	}

	// ���G���Ԍo��
	void Player::InvincibleTimer()
	{
		// ���G���ԂŃ^�C�}�[��ݒ�
		if (SetTimer(m_invincibleTime))
		{
			// �o�ߎ��Ԃ̃��Z�b�g�Ɩ��G�̉���
			m_status.Set(eStatus::IsInvincible) = false;
			m_bodyDraw->SetDrawActive(true);
			m_armDraw->SetDrawActive(true);
		}

		// ���G���Ȃ�
		if (m_status(eStatus::IsInvincible))
		{
			// 0.05f(�b)�Ԋu�œ_�ł�����
			if (SetTimer(0.05f))
			{
				// �`�悵�Ă邩�̐^�U�𔽓]���Ď擾
				bool oppositeDraw = !m_bodyDraw->GetDrawActive();
				m_bodyDraw->SetDrawActive(oppositeDraw);
				m_armDraw->SetDrawActive(oppositeDraw);
			}
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
		const Vec3& deathPos = stage->GetGameCamera()->GetTargetPos();
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
		}
	}

	// �����@�̃`�F�b�N
	void Player::BlowerBetweenCheck()
	{
		// �A�b�v�f�[�g�I�u�W�F�N�g�O���[�v�̃|�C���^�z��̎擾
		const auto& groupVec = GetStage()->GetSharedObjectGroup(L"Gimmick")->GetGroupVector();

		// �M�~�b�N�̊p�x�Ƒ����@�̒��ɋ��邩�̐^�U
		eAngle angle = eAngle::Up;
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
			case eAngle::Up:
				UpMeddleVelocity();
				break;

			case eAngle::Down:
				DownMeddleVelocity();
				break;

			case eAngle::Left:
				LeftMeddleVelocity();
				break;

			case eAngle::Right:
				RightMeddleVelocity();
				break;

			default:
				break;
			}
		}	
		else
		{
			// �����@�ɓ����������̐^�U�����Z�b�g
			m_status.Set(eStatus::IsBlower) = false;

			// ���Z�ړ��ʂ̌����֐�
			ReductionMeddleVelocity();
		}
	}

	// ������̑����@�ɓ������Ă��鎞
	void Player::UpMeddleVelocity()
	{
		// �����@�͈͓̔����̐^�U��true
		m_status.Set(eStatus::IsBlower) = true;

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
		upper = CollHitUpper(hitPos, objPos, helf) && !BlockCheck(objPos + UP_VEC);
		under = CollHitUnder(hitPos, objPos, helf) && !BlockCheck(objPos + DOWN_VEC);
		left = CollHitLeft(hitPos, objPos, helf) && !BlockCheck(objPos + LEFT_VEC);
		right = CollHitRight(hitPos, objPos, helf) && !BlockCheck(objPos + RIGHT_VEC);

		// Z���̊p�x���擾�ŃX���[�v���ǂ����𔻒f
		const float& deg = cube->GetDegreeAngle().z;
		if (deg != 0.0f)
		{
			// �X���[�v�̌����ŏォ��A������̉����������s��
			const auto& angle = cube->GetCubeType();
			if (GetBetween<eCubeType>(angle, eCubeType::SlopeUL, eCubeType::SlopeUR))
			{
				// �ォ��̉��������𑗂�
				BlockUpperHit(objPos, helf);
			}
			if (GetBetween<eCubeType>(angle, eCubeType::SlopeDL, eCubeType::SlopeDR))
			{
				// ������̉��������𑗂�
				BlockUnderHit(objPos, helf);
			}
		}
		else
		{
			// �R���W�����ɑ΂��ďォ��Փ�
			if (upper)
			{
				// �ォ��̉��������𑗂�
				BlockUpperHit(objPos, helf);
				return;
			}

			// ������Փ�
			if (under)
			{
				// ������̉��������𑗂�
				BlockUnderHit(objPos, helf);
				return;
			}
		}

		// ������Փ�
		if (left)
		{
			// ������̉��������𑗂�
			BlockLeftHit(objPos, helf);

			// �����ǂ���Ȃ����
			const auto& wall = dynamic_pointer_cast<MoveWall>(cube);
			if (!wall)
			{
				// ���E���爳�����ĂȂ����̊m�F�����𑗂�
				LeftRightCompressedDeath();
			}
			return;
		}

		// �E����Փ�
		if (right)
		{
			// �E����̉��������𑗂�
			BlockRightHit(objPos, helf);

			// �����ǂ���Ȃ����
			const auto& wall = dynamic_pointer_cast<MoveWall>(cube);
			if (!wall)
			{
				// ���E���爳�����ĂȂ����̊m�F�����𑗂�
				LeftRightCompressedDeath();
			}
			return;
		}
	}

	// �u���b�N�̏ォ��Փ˂������̉�������
	void Player::BlockUpperHit(const Vec3& objPos, const Vec3& helf)
	{
		// ��Ƀu���b�N�����邩�̃`�F�b�N
		if (BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

		// �G�A�V���b�N�g�p�\�ɂ���
		m_status.Set(eStatus::IsFirePossible) = true;

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
		m_status.Set(eStatus::IsFirePossible) = true;

		// �ړ��ʂ�������Ȃ�
		if (m_velocity.y < 0.0f)
		{
			// ���]�����A����������
			m_velocity.y *= -1.0f;

			if (m_acsel > 2.5f)
			{
				// �o�E���h����SE���Đ�
				BlockBoundSE();
			}
		}

		// �������ň������ĂȂ����̊m�F�����𑗂�
		UnderCompressedDeath();
	}

	// �u���b�N�̍�����Փ˂������̉�������
	void Player::BlockLeftHit(const Vec3& objPos, const Vec3& helf)
	{
		// ���Ƀu���b�N�����邩�̃`�F�b�N
		if (BlockCheck(Vec3(objPos.x - (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// �ړ��ʂ𔼌������]������
		m_velocity.x *= -0.5f;

		if (m_acsel > 2.5f)
		{
			// �o�E���h����SE���Đ�
			BlockBoundSE();
		}
	}

	// �u���b�N�̉E����Փ˂������̉�������
	void Player::BlockRightHit(const Vec3& objPos, const Vec3& helf)
	{
		// �E�Ƀu���b�N�����邩�̃`�F�b�N
		if (BlockCheck(Vec3(objPos.x + (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// �ړ��ʂ𔼌������]������
		m_velocity.x *= -0.5f;

		if (m_acsel > 2.5f)
		{
			// �o�E���h����SE���Đ�
			BlockBoundSE();
		}
	}

	// �u���b�N�̃o�E���h��SE�Đ�
	void Player::BlockBoundSE()
	{
		// ���x�̕��ς��ړ��ʑ��x�Ŋ������l�����ʂƂ���SE���Đ�
		float volume = ((m_velocity.y + m_meddleVelo.y + m_acsel) / 3.0f) / m_veloSpeed / 2.0f;
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
			m_status.Set(eStatus::IsAir) = false;

			// �ʏ킩�A�X���[�v���Őݒ�
			const auto& angle = cube->GetCubeType();
			switch (angle)
			{
			case CubeParam::eCubeType::Normal:
				SetPosition(GetPosition().x, objPos.y + helf.y + (GetScale().y / 2.0f), 0.0f); // �ڒn����
				break;

			case CubeParam::eCubeType::SlopeUL:
				m_velocity.x = 0.175f; // ���ɏ����ړ�������
				break;

			case CubeParam::eCubeType::SlopeUR:
				m_velocity.x = -0.175f; // �E�ɏ����ړ�������
				break;

			default:
				break;
			}
		}
	}

	// �X�p�C�N�ƏՓ˂����u��
	void Player::SpikeEnter(const shared_ptr<GameObject>& obj, const Vec3& hitPos)
	{
		// ���G���Ȃ�u���b�N�Ƃ��Ĉ���
		if (m_status(eStatus::IsInvincible))
		{
			// �u���b�N�Ƃ̏Փˏ����𑗂�
			BlockEnter(obj, hitPos);
			return;
		}

		// �X�p�C�N�I�u�W�F�N�g�ɃL���X�g
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// �p�����[�^�̎擾
		Vec3 spikePos = spike->GetPosition();
		Vec3 scale = spike->GetScale();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// �Փ˕����^�U
		const auto& groupVec = GetStage()->GetSharedObjectGroup(L"Update")->GetGroupVector();
		bool upper, under, left, right;
		upper = CollHitUpper(hitPos, spikePos, helfScale) && !ObjectCheck(groupVec, Vec3(spikePos + Vec3(0.0f, scale.y, 0.0f)));
		under = CollHitUnder(hitPos, spikePos, helfScale) && !ObjectCheck(groupVec, Vec3(spikePos + Vec3(0.0f, -scale.y, 0.0f)));
		left = CollHitLeft(hitPos, spikePos, helfScale) && !ObjectCheck(groupVec, Vec3(spikePos + Vec3(-scale.x, 0.0f, 0.0f)));
		right = CollHitRight(hitPos, spikePos, helfScale) && !ObjectCheck(groupVec, Vec3(spikePos + Vec3(scale.x, 0.0f, 0.0f)));

		// �X�p�C�N�̕������擾
		const auto& angle = spike->GetAngle();
		switch (angle)
		{
		case eAngle::Up: // ������̞���

			// ��E���E�E����Փ˂�����
			if (upper || left || right) 
			{
				// �_���[�W�����𑗂�
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}

			// ������Փ˂�����
			if (under)
			{
				// �u���b�N�Ƃ̏����𑗂�
				BlockUnderHit(spikePos, helfScale);
				return;
			}
			break;

		case eAngle::Down: // �������̞���

			// ���E���E�E����Փ˂�����
			if (under || left || right)
			{
				// �_���[�W�����𑗂�
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}

			// �ォ��Փ˂�����
			if (upper)
			{
				// �u���b�N�Ƃ̏����𑗂�
				BlockUpperHit(spikePos, helfScale);
				return;
			}
			break;

		case eAngle::Left: // �������̞���

			// ��E���E������Փ˂�����
			if (upper || under || left)
			{
				// �_���[�W�����𑗂�
				DamageKnockBack(Vec2(1.5f, -0.5f));
				return;
			}

			// �E����Փ˂�����
			if (right)
			{
				// �u���b�N�Ƃ̏����𑗂�
				BlockRightHit(spikePos, helfScale);
				return;
			}
			break;

		case eAngle::Right: // �E�����̞���

			// ��E���E�E����Փ˂�����
			if (upper || under || right)
			{
				// �_���[�W�����𑗂�
				DamageKnockBack(Vec2(-1.5f, -0.5f));
				return;
			}

			// ������Փ˂�����
			if (left)
			{
				// �u���b�N�Ƃ̏����𑗂�
				BlockLeftHit(spikePos, helfScale);
				return;
			}
			break;

		case eAngle::All: // �S���ʂ̞���
			if (upper) // �ォ��
			{
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}
			if (under) // ������
			{
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}
			if (left) // ������
			{
				DamageKnockBack(Vec2(1.5f, -0.5f));
				return;
			}
			if (right) // �E����
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
		if (angle == eAngle::Down)
		{
			// �G�A�V���b�N���ˉ\�ɂ���
			m_status.Set(eStatus::IsFirePossible) = true;

			// �u���b�N�Ƃ̐ڒn����
			BlockExcute(obj, hitPos);
		}
		else
		{
			// ���G���Ԓ�����Ȃ���Ξ��Ƃ̏Փˏ����𑗂�
			if (!m_status(eStatus::IsInvincible))
			{
				SpikeEnter(obj, hitPos);
			}
		}
	}

	// ��C�ƏՓ˂����u��
	void Player::CannonEnter(const shared_ptr<GameObject>& cannon)
	{
		// ��C�^�ɃL���X�g
		const auto& ptr = dynamic_pointer_cast<Cannon>(cannon);
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

				// �X�e�[�^�X�̐ݒ�
				m_status.Set(eStatus::IsCannonStandby) = true;
				m_status.Set(eStatus::IsAir, eStatus::IsFirePossible) = false;

				// �����ړ��s�ɐݒ�
				m_velocity = Vec2(0.0f);
				m_acsel = 1.0f;
			}
		}
	}

	// �x���g�R���x�A�ƏՓ˂����u��
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

			// �ڒn��Ԃɂ���
			m_status.Set(eStatus::IsFirePossible) = true;
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

		// ��ȊO����Փ˂����Ȃ�u���b�N�Ƃ̏Փˏ����𑗂�
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

	// �o���p�[�ɏՓ˂����u��
	void Player::BumperEnter(const shared_ptr<GameObject>& other, const Vec3& hitPos)	
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
		
	// �G�̃E�T�M�ƏՓ˂����u��
	void Player::RabbitEnter(const shared_ptr<GameObject>& other, const Vec3& hitPos)
	{			
		// �E�T�M�^�ɃL���X�g
		const auto& rabbit = dynamic_pointer_cast<Rabbit>(other);
		if (rabbit)
		{
			// ���G���Ԓ�����Ȃ���
			if (!m_status(eStatus::IsInvincible))
			{
				// ��C���ˌ�Ȃ�
				if (m_status(eStatus::IsCannonFire))
				{
					// �E�T�M�̃X�e�[�g�����S�ɐݒ�
					rabbit->SetState(Rabbit::eState::Death);
					StartSE(L"RABBIT_DEATH_SE", 0.5f);
					return;
				}

				// �V�[���h������Ȃ�
				if (m_shieldCount > 0)
				{
					// �E�T�M�̃X�e�[�g�����S�ɐݒ�
					rabbit->SetState(Rabbit::eState::Death);
					StartSE(L"RABBIT_DEATH_SE", 0.5f);
				}

				// �Փˏ���
				float knockBackValue = (rabbit->GetPosition().x > hitPos.x) ? 1.5f : -1.5f;
				DamageKnockBack(Vec2(knockBackValue, -0.5f));
			}
		}
	}

	// �G�̃E�T�M�ƏՓ˂���������
	void Player::RabbitExcute(const shared_ptr<GameObject>& rabbit, const Vec3& hitPos)
	{
		// ���G������Ȃ����
		if (!m_status(eStatus::IsInvincible))
		{
			// �E�T�M�ƏՓ˂������̊֐������s
			RabbitEnter(rabbit, hitPos);
		}
	}

	// �_���[�W�m�b�N�o�b�N
	void Player::DamageKnockBack(const Vec2& velocity)
	{
		// �p�����[�^�̐ݒ�
		m_status.Set(eStatus::IsAir) = true;
		m_status.Set(eStatus::IsFirePossible) = false;
		m_velocity = velocity;
		m_acsel = m_damageAcsel;

		// �A�j���[�V�������_���[�W��Ԃɂ���
		m_bodyDraw->ChangeCurrentAnimation(L"DAMAGE");

		// �O���̕\�����I�t
		m_arrow.lock()->SetDrawActive(false);

		// ���G������Ȃ����
		if (!m_status(eStatus::IsInvincible))
		{
			// �V�[���h�̐���1���ȏ�Ȃ�
			if (m_shieldCount >= 1)
			{
				// �V�[���h�����炵�A���G���Ԃɂ���
				m_shieldCount--;
				m_status.Set(eStatus::IsInvincible) = true;

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

	// �����O�ɏՓ˂����u��
	void Player::RingEnter(const shared_ptr<GameObject>& ring)
	{
		// �^�L���X�g
		const auto& ringPtr = dynamic_pointer_cast<Ring>(ring);
		if (ringPtr)
		{
			// �����O�Ɏ擾��Ԃ𑗂�
			ringPtr->IsGetRing();

			// �����O�̃T�C�Y�ɉ����Ď擾����������
			const auto& size = ringPtr->GetRingSize();
			switch (size)
			{
			case Ring::eRingSize::Big: // �傫�������O�Ȃ�
				
				AddShield(); // �V�[���h��ǉ�
				StartSE(L"RING_SE", 0.75f); // SE�̍Đ�
				break;

			case Ring::eRingSize::Small: // �����������O�Ȃ�

				GetSmallRing(); // �������O�擾�����𑗂�
				StartSE(L"RING_SE", 0.35f); // SE�̍Đ�
				break;

			default:
				break;
			}

			// �����������O�̎擾����25�ȏ�ɂȂ�����
			if (m_shieldLimit > m_shieldCount && m_sRingCount >= m_sRingLimit.at(m_shieldCount))
			{
				// �V�[���h�𑝂₵�ăJ�E���^�����Z�b�g
				AddShield();
				m_sRingCount = 0;
			}
		}
	}

	// �����ǂɏՓ˂����u��
	void Player::MoveWallEnter(const shared_ptr<GameObject>& wall, const Vec3& hitPos)
	{
		// �u���b�N�̃p�����[�^���擾
		const auto& moveWall = dynamic_pointer_cast<MoveWall>(wall);
		Vec3 objPos = moveWall->GetPosition();
		Vec3 helf = moveWall->GetScale() / 2.0f;

		// �����ǃI�u�W�F�N�g�z�񓙂̎擾
		vector<weak_ptr<GameObject>> groupVec = GetStage()->GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
		Vec3 leftPos = GetMoveWallPos(groupVec, objPos, LEFT_VEC);
		Vec3 rightPos = GetMoveWallPos(groupVec, objPos, RIGHT_VEC);
		Vec3 upperPos = GetMoveWallPos(groupVec, objPos, UP_VEC);
		Vec3 underPos = GetMoveWallPos(groupVec, objPos, DOWN_VEC);

		// ���W�̐ݒ�
		Vec3 pos = GetPosition();
		Vec3 cornerUL, cornerDR;

		// �E����Փ˂������̏���
		// �Փ˂��������ǂ̍����ɕʂ̓����ǂ��Ȃ����
		if (!ObjectCheck(groupVec, objPos + LEFT_VEC))
		{
			// �����ǂ̊p��ݒ�
			cornerUL = Vec3(-helf.x * 1.25f, helf.y, 0.0f);
			cornerDR = Vec3(0.0f, -helf.y, 0.0f);

			// �����ǂ͈͓̔��ŏՓ˂����Ȃ�
			if (GetBetween(hitPos, upperPos + cornerUL, underPos + cornerDR))
			{
				// ���g�̍����������I�u�W�F�N�g�̍����ȉ��Ȃ�
				if (pos.y <= objPos.y + helf.y)
				{
					// �ǂ������Ă�����
					if (moveWall->GetMoveRatio() > 0.0f)
					{
						// �p�����[�^�̐ݒ�
						m_status.Set(eStatus::IsHitMoveBlock) = true;
						m_status.Set(eStatus::IsAliveMoveBlock) = false;
						m_currentWall = moveWall;
					}
				}
			}
		}

		// ������Փ˂������̏���
		// �Փ˂��������ǂ̉E���ɕʂ̓����ǂ��Ȃ����
		if (!ObjectCheck(groupVec, objPos + RIGHT_VEC))
		{
			// �����ǂ̊p��ݒ�
			cornerUL = Vec3(helf.x * 1.25f, helf.y, 0.0f);
			cornerDR = Vec3(0.0f, -helf.y, 0.0f);

			// �����ǂ͈͓̔��ŏՓ˂����Ȃ�
			if (GetBetween(hitPos, upperPos + cornerUL, underPos + cornerDR))
			{
				// ���g�̍����������I�u�W�F�N�g�̍����ȉ��Ȃ�
				if (pos.y <= objPos.y + helf.y)
				{
					// �ǂ������Ă�����
					if (moveWall->GetMoveRatio() > 0.0f)
					{
						// �p�����[�^�̐ݒ�
						m_status.Set(eStatus::IsHitMoveBlock) = true;
						m_status.Set(eStatus::IsAliveMoveBlock) = false;
						m_currentWall = moveWall;
					}
				}
			}
		}

		// �ォ��Փ˂������̏���
		// �Փ˂��������ǂ̏㑤�ɕʂ̓����ǂ��Ȃ����
		if (!ObjectCheck(groupVec, objPos + UP_VEC))
		{
			// �����ǂ̊p��ݒ�
			cornerUL = Vec3(-helf.x, helf.y * 1.25f, 0.0f);
			cornerDR = Vec3(helf.x, 0.0f, 0.0f);

			// �����ǂ͈͓̔��ŏՓ˂����Ȃ�
			if (GetBetween(hitPos, leftPos + cornerUL, rightPos + cornerDR))
			{
				// �p�����[�^�̐ݒ�
				m_velocity.y = 0.25f;
				m_acsel = 1.0f;
				m_status.Set(eStatus::IsAir) = false;

				// �ڒn����
				pos.y = objPos.y + moveWall->GetScale().y;
				SetPosition(pos);

				// ���E���瓮���ǂɏՓ˂��Ă��Ȃ����
				if (!m_status(eStatus::IsHitMoveBlock))
				{
					// �����ǂ̏�ɋ����Ԃɂ���
					m_status.Set(eStatus::IsAliveMoveBlock) = true;
					m_currentWall = moveWall;
				}
			}
		}

		// ������Փ˂������̏���
		// �Փ˂��������ǂ̉����ɕʂ̓����ǂ��Ȃ����
		if (!ObjectCheck(groupVec, objPos + DOWN_VEC))
		{
			// �����ǂ̊p��ݒ�
			cornerUL = Vec3(-helf.x, 0.0f, 0.0f);
			cornerDR = Vec3(helf.x, -helf.y * 1.25f, 0.0f);

			// �����ǂ͈͓̔��ŏՓ˂����Ȃ�
			if (GetBetween(hitPos, leftPos + cornerUL, rightPos + cornerDR))
			{
				if (!m_status(eStatus::IsHitMoveBlock))
				{
					UnderCompressedDeath();
				}
			}
		}

		// �������ĂȂ����̏���
		// �����ǂ̊p��ݒ�
		cornerUL = Vec3(-helf.x, -helf.y, 0.0f);
		cornerDR = Vec3(helf.x, helf.x, 0.0f);

		// �����ǂ̒��ɋ�����
		if (GetBetween(pos, objPos + cornerUL, objPos + cornerDR))
		{
			// ���������𑗂�
			m_status.Set(eStatus::IsAliveMoveBlock) = true;
			UnderCompressedDeath();
		}
	}

	// �����ǂ��痣�ꂽ��
	void Player::MoveWallExit()
	{
		bool reset = false; // ���Z�b�g���邩�̐^�U
		Vec3 wallPos = m_currentWall.lock()->GetPosition(); // �Ō�ɏՓ˂��������ǂ̍��W

		// �����̔�r
		float length = (GetPosition() - wallPos).length();
		if (length >= 2.0f)
		{
			// �Փː^�U�Ə�ɋ��邩�̐^�U���I�t
			m_status.Set(eStatus::IsHitMoveBlock, eStatus::IsAliveMoveBlock) = false;
			reset = true;
		}

		// �ǂ������Ă��邩
		const auto& wall = dynamic_pointer_cast<MoveWall>(m_currentWall.lock());
		if (wall->GetMoveRatio() <= 0.0f)
		{
			// �Փː^�U���I�t
			m_status.Set(eStatus::IsHitMoveBlock) = false;
			reset = true;
		}

		// ���Z�b�g����Ȃ�
		if (reset)
		{
			// �ێ�������
			m_currentWall.reset();
		}
	}
}