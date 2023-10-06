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
		ptrTrans->SetScale(Vec3(2.0f));

		// �`��̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		// �R���W�����̐ݒ�
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(true);
	}

	void Player::OnUpdate()
	{
		// A�{�^�����͂���������
		Input::GetPushA() == true ? OnPushA() : Input::GetReleaseA() == true ? OnReleaseA() : 0;

		// �v���C���[�̈ړ��֐�
		MovePlayer();

		// �f�o�b�O������
		Debug::Log(L"pos : ", m_position);
		Debug::Log(L"velo : ", m_velocity);
		Debug::Log(L"acsel : ", m_acsel);
		Debug::Log(L"isAir : ", m_isAir);
	}



	void Player::OnPushA()
	{
		m_timeSpeed = 0.05f;
	}

	void Player::OnReleaseA()
	{
		// �����o�ϐ��̐ݒ�
		m_isAir = true;
		m_acsel = 5.0f;
		m_timeSpeed = 1.0f;

		// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
		const Vec2& stick = Input::GetLStickValue();
		if (stick.length() > 0.0f)
		{
			m_velocity = stick * 3.0f;
		}
	}

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
			float decrease = deltaTime * 5.0f;
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

	// �n�ʂɐڒn������
	void Player::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		// Y���ړ��x�N�g����0.0�ɂ��A�󒆂��̐^�U��false
		m_velocity.y = 0.0f;
		m_isAir = false;
	}
}