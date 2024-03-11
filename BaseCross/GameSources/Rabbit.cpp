/*!
@file Rabbit.cpp
@brief ���J�E�T�M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void Rabbit::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();

		// �g�����X�t�H�[���̐ݒ�
		SetTransParam();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"RABBIT");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"JUMP", 0, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"JUMP");

		// �R���W�����R���|�[�l���g�̒ǉ�
		m_ptrColl = AddComponent<CollisionObb>();
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// �X�e�[�W����v���C���[���擾���A�^�[�Q�b�g�Ƃ��Đݒ�
		m_targetObj = GetStage()->GetSharedGameObject<TemplateObject>(L"Player");

		// �^�O��ݒ�
		AddTag(L"Rabbit");
	}

	// �ړ�����
	void Rabbit::MoveRabbit()
	{
		// ����ł��炸�A�ړ���X����0�ȊO�ł����
		if (!m_status(eStatus::IsDeath) && m_velocity.x != 0.0f)
		{
			// �ړ���X�����������ݒ�
			m_dir = m_velocity.x > 0.0f ? 1.0f : -1.0f;

			// ���������ɁA���[�e�[�V����Y����ݒ�
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

	// �ړ��ʌ�������
	void Rabbit::MoveReduction()
	{
		// �f���^�^�C�����擾
		const float deltaTime = DELTA_TIME;

		// �󒆂Ȃ�
		if (m_status(eStatus::IsAir))
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

			// Y���ړ��x�N�g����0.25���傫��������
			if (m_velocity.y > 0.25f)
			{
				// Y���ړ��x�N�g�����f���^�^�C���Ō��Z
				m_velocity.y -= deltaTime;
			}
			else
			{
				// 0.25��菬����������0.25�ŏC��
				m_velocity.y = 0.25f;
			}
		}
	}
}