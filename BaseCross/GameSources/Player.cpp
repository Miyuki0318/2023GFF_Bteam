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
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(Vec3(0.0f, 2.0f, 0.0f));
		ptrTrans->SetScale(Vec3(1.0f));
		
		m_arm.lock()->SetPosition(Vec3(0.0f, 2.0f, 0.0f));
		m_arm.lock()->SetScale(Vec3(1.0f));

		// �`��̐ݒ�
		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"ROBOT_BODY");
		ptrDraw->SetMeshToTransformMatrix(m_bodyMat);
		ptrDraw->AddAnimation(L"WALK", 0, 60, true);
		ptrDraw->AddAnimation(L"DAMAGE", 180, 30, false);
		ptrDraw->ChangeCurrentAnimation(L"WALK");

		auto armDraw = m_arm.lock()->AddComponent<PNTBoneModelDraw>();
		armDraw->SetMultiMeshResource(L"ROBOT_ARM");
		armDraw->SetMeshToTransformMatrix(m_armMat);
		armDraw->AddAnimation(L"WALK", 0, 60, true);
		armDraw->AddAnimation(L"FIRE", 120, 30, false);
		armDraw->ChangeCurrentAnimation(L"WALK");

		// �R���W�����̐ݒ�
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(false);

		// �Ə��p�I�u�W�F�N�g�̐���
		for (size_t i = 0; i < 10; i++)
		{
			m_aligment.push_back(GetStage()->AddGameObject<DebugSphere>());
			m_aligment.at(i).lock()->SetScale(0.15f);
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
		if (m_timeSpeed == 0.1f)
		{
			RotateAligment();
		}

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
		Debug::Log(L"isAir : ", m_isAir);
	}

	// A�{�^����������
	void Player::OnPushA()
	{
		// ���Ԃ�x������
		m_timeSpeed = m_slowTime;

		// �Ə���\������
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(true);
		}
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
		auto arm = m_arm.lock()->GetComponent<PNTBoneModelDraw>();
		arm->ChangeCurrentAnimation(L"FIRE");

		// �Ə����\���ɂ���
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(false);
		}
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
	}

	// �R���W�����ɏՓ˂���������
	void Player::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Block"))
		{
			BlockExcute(other);
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
		// �g�����X�t�H�[���̎擾
		auto ptrTrans = GetComponent<Transform>();

		// �O�t���[������̃f���^�^�C�����擾
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

		// ���݂̍��W���擾
		m_position.x = ptrTrans->GetPosition().x;
		m_position.y = ptrTrans->GetPosition().y;

		// �|�W�V�����Ɉړ��x�N�g���Ƒ��x�Ɖ����x�ƃf���^�^�C���Ŋ|�����������Z
		m_position.x += -m_velocity.x * m_speed * m_acsel * deltaTime;
		m_position.y += -m_velocity.y * m_speed * m_acsel * deltaTime;

		// ���W�̍X�V
		ptrTrans->SetPosition(m_position);

		// �r�̍��W�̍X�V(�r�Ɠ��̃��f���}�g���N�X�̃|�W�V����y���̍������|�W�V�������獷������)
		Vec3 pos = m_position;
		auto body = GetComponent<PNTBoneModelDraw>();
		auto& bone = body->GetVecLocalBones();
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
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetRotation(Vec3(0.0f, body, 0.0f));
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
			loopCount++;
		}
	}

	// �A�j���[�V�����̍X�V
	void Player::AnimationUpdate()
	{
		// ���Ƙr�̕`��R���|�[�l���g�̎擾
		auto body = GetComponent<PNTBoneModelDraw>();
		auto arm = m_arm.lock()->GetComponent<PNTBoneModelDraw>();

		// �O�t���[������̃f���^�^�C���ɃQ�[���X�s�[�h���|���������擾
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

		// �A�j���[�V�������I����Ă���ҋ@��Ԃɂ���
		if (body->IsTargetAnimeEnd())
		{
			body->ChangeCurrentAnimation(L"WALK");
		}
		if (arm->IsTargetAnimeEnd())
		{
			arm->ChangeCurrentAnimation(L"WALK");
		}

		// �A�j���[�V�����̍X�V
		body->UpdateAnimation(deltaTime);
		arm->UpdateAnimation(deltaTime);
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
	void Player::BlockEnter(shared_ptr<GameObject>& block)
	{
		// ���g�ƃu���b�N�̃g�����X�t�H�[���̎擾
		auto ptrTrans = GetComponent<Transform>();
		auto objTrans = block->GetComponent<Transform>();

		// ���W�̎擾
		Vec3 pos = ptrTrans->GetPosition();
		Vec3 objPos = objTrans->GetPosition();

		// �u���b�N�̃X�P�[���̔���
		float helf = objTrans->GetScale().x / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (pos.y > objPos.y && ((pos.y - objPos.y) >= helf))
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
		if (pos.y < objPos.y && ((pos.y - objPos.y) <= -helf))
		{
			if (m_velocity.y < 0.0f)
			{
				m_velocity.y *= -1.0f;
			}
			return;
		}

		// ������Փ�
		if (((pos.y - objPos.y) < helf) && ((pos.y - objPos.y) > -helf))
		{
			if (((pos.x - objPos.x) < helf) || ((pos.x - objPos.x) > -helf))
			{
				m_velocity.x *= -0.5f;
			}
			return;
		}
	}

	// �u���b�N�ɏՓ˂���������
	void Player::BlockExcute(shared_ptr<GameObject>& block)
	{
		// ���g�ƃu���b�N�̃g�����X�t�H�[���̎擾
		auto ptrTrans = GetComponent<Transform>();
		auto objTrans = block->GetComponent<Transform>();

		// ���W�̎擾
		Vec3 pos = ptrTrans->GetPosition();
		Vec3 objPos = objTrans->GetPosition();

		// �u���b�N�̃X�P�[���̔���
		float helf = objTrans->GetScale().x / 2.0f;

		// �R���W�����ɑ΂��ďォ��Փ�
		if (pos.y > objPos.y && ((pos.y - objPos.y) >= helf))
		{
			// Y���ړ��x�N�g����0.0�ɂ��A�󒆂��̐^�U��false
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
			m_isAir = false;
		}
	}

	// �u���b�N�Ƃ̏Փ˂������Ȃ�����
	void Player::BlockExit(shared_ptr<GameObject>& block)
	{
		// �ڒn�t���O������
		m_isAir = true;
	}

	// �X�p�C�N�Ƃ̏Փ˂����u��
	void Player::SpikeEnter(shared_ptr<GameObject>& obj)
	{
		// ���ԑ��x��ʏ푬�x�ŏ㏑��
		m_timeSpeed = m_normalTime;

		// �O���̕\�����I�t
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(false);
		}

		// �X�p�C�N�I�u�W�F�N�g�ɃL���X�g
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// �X�p�C�N�̕����ɉ����ď���
		const auto& angle = spike->GetAngle();
		switch (angle)
		{
		case Gimmick::Up:
			m_firePossible = false;
			m_velocity = Vec2(0.9f, -1.0f);
			m_acsel = 3.0f;
			break;

		case Gimmick::Down:
			m_firePossible = false;
			m_velocity = Vec2(0.9f, 1.0f);
			m_acsel = 3.0f;
			break;

		case Gimmick::Left:
			m_firePossible = false;
			m_velocity = Vec2(1.5f, -0.5f);
			m_acsel = 3.0f;
			break;

		case Gimmick::Right:
			m_firePossible = false;
			m_velocity = Vec2(-1.5f, -0.5f);
			m_acsel = 3.0f;
			break;

		default:
			break;
		}
	}
}