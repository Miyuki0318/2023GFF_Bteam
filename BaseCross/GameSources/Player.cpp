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
		// �g�����X�t�H�[���̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(Vec3(0.0f, 2.0f, 0.0f));
		ptrTrans->SetScale(Vec3(3.0f));

		// �`��̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		// �R���W�����̐ݒ�
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(true);

		// �Ə��p�I�u�W�F�N�g�̐���
		for (size_t i = 0; i < 10; i++)
		{
			m_aligment.push_back(GetStage()->AddGameObject<DebugSphere>());
			m_aligment.at(i).lock()->SetScale(1.0f);
			m_aligment.at(i).lock()->SetDrawActive(false);
		}
	}

	void Player::OnUpdate()
	{
		// A�{�^�����͗L���ł̊֐�����
		Input::GetPushA() == true ? OnPushA() : Input::GetReleaseA() == true ? OnReleaseA() : 0;

		// �Ə��̉�]����
		AligmentRotate();

		// �v���C���[�̈ړ��֐�
		MovePlayer();

		// �f�o�b�O������
		Debug::Log(L"pos : ", m_position);
		Debug::Log(L"velo : ", m_velocity);
		Debug::Log(L"acsel : ", m_acsel);
		Debug::Log(L"isAir : ", m_isAir);
	}

	// A�{�^����������
	void Player::OnPushA()
	{
		// ���Ԃ�0.05�{�ɂ���
		m_timeSpeed = 0.1f;

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
		m_timeSpeed = 2.0f;

		// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
		const Vec2& stick = Input::GetLStickValue();
		if (stick.length() > 0.0f)
		{
			m_velocity = stick * 3.0f;
		}

		// �Ə����\���ɂ���
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(false);
		}
	}

	// �n�ʂɐڒn������
	void Player::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		// �ړ��ʂ��������ɂ���
		if (m_velocity.y > 0.0f)
		{
			// �����x���������傫��������
			if (m_acsel > 2.5f)
			{
				// �ړ��ʂ𔽓]�����A�����ɂ���
				m_velocity.y *= -0.5f;
			}
		}
	}

	// �n�ʂɐڒn����������
	void Player::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		// Y���ړ��x�N�g����0.0�ɂ��A�󒆂��̐^�U��false
		m_velocity.y = 0.0f;
		m_isAir = false;
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

		// ���W�̍X�V
		ptrTrans->SetPosition(m_position);
	}

	void Player::AligmentRotate()
	{
		// �ړ������p
		Vec2 velo;

		// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
		const Vec2& stick = Input::GetLStickValue();
		if (stick.length() > 0.0f)
		{
			velo = stick / 10.0f;
		}

		// �I�u�W�F�N�g�̐������[�v
		float loopCount = 1.0f;
		float deltaTime = App::GetApp()->GetElapsedTime();
		for (const auto& aligment : m_aligment)
		{
			Vec2 pos = Vec2(m_position.x, m_position.y);
			pos += -velo * m_speed * m_maxAcsel * loopCount;
			velo.y -= m_gravity * deltaTime / 22.5f;
			aligment.lock()->SetPosition(pos);
			loopCount++;
		}
	}
}