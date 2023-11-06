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
		m_ptrTrans->SetPosition(Vec3(0.0f, 2.0f, 0.0f));
		m_ptrTrans->SetScale(Vec3(1.0f));
		
		// �r�̃g�����X�t�H�[��
		m_arm.lock()->SetPosition(Vec3(0.0f, 2.0f, 0.0f));
		m_arm.lock()->SetScale(Vec3(1.0f));

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

		// �Ə��p�I�u�W�F�N�g�̐���
		for (size_t i = 0; i < 10; i++)
		{
			m_aligment.push_back(GetStage()->AddGameObject<DebugSphere>());
			m_aligment.at(i).lock()->SetScale(0.15f);
			m_aligment.at(i).lock()->SetDrawLayer(-1);
			m_aligment.at(i).lock()->SetDrawActive(false);
		}

		// �G�t�F�N�g�I�u�W�F�N�g�̐���
		m_effect = GetStage()->AddGameObject<Billboard>(L"EFFECT", Vec2(0.0f), Vec3(0.0f));
		m_effect.lock()->SetDrawLayer(1);
	}

	void Player::OnUpdate()
	{
		// A�{�^�����͗L���ł̊֐�����
		if (m_acsel <= 1.7f && m_firePossible)
		{
			Input::GetPushA() == true ? OnPushA() : Input::GetReleaseA() == true ? OnReleaseA() : 0;
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
		Debug::Log(m_isAir != false ? L"��" : L"�ڒn");
		Debug::Log(m_firePossible != false ? L"���ˉ�" : L"���˕s��");
	}

	// A�{�^����������
	void Player::OnPushA()
	{
		//// ���Ԃ�x������
		//m_timeSpeed = m_slowTime;

		//// �Ə���\������
		//for (const auto& aligment : m_aligment)
		//{
		//	aligment.lock()->SetDrawActive(true);
		//}
	}

	// A�{�^���𗣂�����
	void Player::OnReleaseA()
	{
		// �����o�ϐ��̐ݒ�
		m_isAir = true;
		m_acsel = m_maxAcsel;
		m_timeSpeed = m_normalTime;

		// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
		Vec2 stick = Input::GetLStickValue();
		m_velocity = (stick.length() > 0.0f ? stick.round(1) : m_deffVelo) * 3.0f;
		
		// �r�̃A�j���[�V������ύX
		m_armDraw->ChangeCurrentAnimation(L"FIRE");

		//// �Ə����\���ɂ���
		//for (const auto& aligment : m_aligment)
		//{
		//	aligment.lock()->SetDrawActive(false);
		//}
	}

	// �R���W�����ɏՓ˂�����
	void Player::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Block"))
		{
			BlockEnter(other);
		}
		if (other->FindTag(L"Spike"))
		{
			SpikeEnter(other);
		}
		if (other->FindTag(L"Bird"))
		{
			BirdEnter(other);
		}
	}

	// �R���W�����ɏՓ˂���������
	void Player::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Block"))
		{
			BlockExcute(other);
		}
		if (other->FindTag(L"Spike"))
		{
			SpikeExcute(other);
		}
	}

	// �R���W�������痣�ꂽ��
	void Player::OnCollisionExit(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Block"))
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
		m_position.x = m_ptrTrans->GetPosition().x;
		m_position.y = m_ptrTrans->GetPosition().y;

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
		float loopCount = 2.0f;
		float deltaTime = App::GetApp()->GetElapsedTime();
		for (const auto& aligment : m_aligment)
		{
			Vec2 pos = Vec2(m_position.x, m_position.y);
			pos += -velo * m_speed * m_maxAcsel * loopCount;
			velo.y -= m_gravity * 0.016f / 20.0f;
			aligment.lock()->SetPosition(pos);
			aligment.lock()->SetDrawActive(Input::GetLStickValue().length() > 0.0f);
			loopCount++;
		}
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
		// �v���C���[�̍��W�Ɉړ������x�N�g���~�����x�����Z����
		Vec3 pos = m_position + (Vec3(m_velocity.x, m_velocity.y, 0.0f).normalize() * (m_acsel * 1.15f));
		pos.y += 0.5f;
		pos.z = -1.0f;

		// �X�V
		m_effect.lock()->SetDrawActive(m_firePossible);
		m_effect.lock()->SetPosition(pos);
		m_effect.lock()->SetRotation(0.0f, 0.0f, (atan2f(m_velocity.y, m_velocity.x) - XM_PIDIV2));
		m_effect.lock()->SetScale(Vec2((m_acsel - 1.0f) * 3.0f));
	}

	// �u���b�N�ɏՓ˂����u��
	void Player::BlockEnter(const shared_ptr<GameObject>& block)
	{
		// �u���b�N�̃p�����[�^���擾
		auto objTrans = block->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (CollHitUpper(objPos, helf))
		{
			// �G�A�V���b�N�g�p�\�ɂ���
			m_firePossible = true;

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
		if (CollHitUnder(objPos, helf))
		{
			// �ړ��ʂ�������Ȃ�
			if (m_velocity.y < 0.0f)
			{
				// ���]�����A����������
				m_velocity.y *= -1.0f;
			}
			return;
		}

		// ������Փ�
		if (CollHitLeft(objPos, helf) || CollHitRight(objPos, helf))
		{
			// �ړ��ʂ𔼌������]������
			m_velocity.x *= -0.5f;
			return;
		}
	}

	// �u���b�N�ɏՓ˂���������
	void Player::BlockExcute(const shared_ptr<GameObject>& block)
	{
		// �u���b�N�̃p�����[�^���擾
		auto objTrans = block->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (CollHitUpper(objPos, helf))
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

	// �X�p�C�N�Ƃ̏Փ˂����u��
	void Player::SpikeEnter(const shared_ptr<GameObject>& obj)
	{
		// �X�p�C�N�I�u�W�F�N�g�ɃL���X�g
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// �p�����[�^�̎擾
		Vec3 spikePos = spike->GetPosition();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// �X�p�C�N�̕����ɉ����ď���
		const auto& angle = spike->GetAngle();
		switch (angle)
		{
		case Gimmick::Up:
			if (CollHitUpper(spikePos, helfScale) || CollHitLeft(spikePos, helfScale) || CollHitRight(spikePos, helfScale))
			{
				DamageKnockBack(Vec2(0.9f, -1.0f));
			}
			if (CollHitUnder(spikePos, helfScale))
			{
				BlockEnter(obj);
			}
			break;

		case Gimmick::Down:
			if (CollHitUnder(spikePos, helfScale) || CollHitLeft(spikePos, helfScale) || CollHitRight(spikePos, helfScale))
			{
				DamageKnockBack(Vec2(0.9f, 1.0f));
			}
			if (CollHitUpper(spikePos, helfScale))
			{
				BlockEnter(obj);
			}
			break;

		case Gimmick::Left:
			if (CollHitLeft(spikePos, helfScale) || CollHitUpper(spikePos, helfScale) || CollHitUnder(spikePos, helfScale))
			{
				DamageKnockBack(Vec2(1.5f, -0.5f));
			}
			if (CollHitRight(spikePos, helfScale))
			{
				BlockEnter(obj);
			}
			break;

		case Gimmick::Right:
			if (CollHitRight(spikePos, helfScale) || CollHitUpper(spikePos, helfScale) || CollHitUnder(spikePos, helfScale))
			{
				DamageKnockBack(Vec2(-1.5f, -0.5f));
			}
			if (CollHitLeft(spikePos, helfScale))
			{
				BlockEnter(obj);
			}
			break;

		default:
			break;
		}
	}

	void Player::SpikeExcute(const shared_ptr<GameObject>& obj)
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
			BlockExcute(obj);
		}
	}

	void Player::BirdEnter(const shared_ptr<GameObject>& enemy)
	{
		// ���I�u�W�F�N�g�ɃL���X�g
		const auto& bird = dynamic_pointer_cast<Bird>(enemy);

		// �p�����[�^�̎擾
		Vec3 birdPos = bird->GetPosition();
		Vec3 helfScale = bird->GetScale() / 2.0f;

		// �ォ��Փ�
		if (CollHitUpper(birdPos, helfScale))
		{
			// �ړ��ʂ𔽓]�����A�����ɂ���
			m_velocity.x = -Input::GetLStickValue().round(1).x;
			m_velocity.y = -2.0f;
			m_acsel = m_damageAcsel;
			m_firePossible = true;
			return;
		}

		// ������Փ�
		if (CollHitUnder(birdPos, helfScale))
		{
			DamageKnockBack(Vec2(0.9f, 1.0f));
			return;
		}

		// ������Փ�
		if (CollHitLeft(birdPos, helfScale))
		{
			DamageKnockBack(Vec2(1.5f, -0.5f));
			return;
		}

		// �E����Փ�
		if (CollHitRight(birdPos, helfScale))
		{
			DamageKnockBack(Vec2(-1.5f, -0.5f));
			return;
		}
	}

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
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(false);
		}
	}

	bool Player::CollHitUpper(const Vec3& position, const Vec3& helfScale)
	{
		return m_position.y > position.y && ((m_position.y - position.y) >= helfScale.y);
	}

	bool Player::CollHitUnder(const Vec3& position, const Vec3& helfScale)
	{
		return m_position.y < position.y && ((m_position.y - position.y) <= -helfScale.y);
	}

	bool Player::CollHitLeft(const Vec3& position, const Vec3& helfScale)
	{
		return ((m_position.y - position.y) < helfScale.y && (m_position.y - position.y) > -helfScale.y)
			&& ((m_position.x - position.x) < helfScale.x);
	}

	bool Player::CollHitRight(const Vec3& position, const Vec3& helfScale)
	{
		return ((m_position.y - position.y) < helfScale.y && (m_position.y - position.y) > -helfScale.y)
			&& ((m_position.x - position.x) > -helfScale.x);
	}
}