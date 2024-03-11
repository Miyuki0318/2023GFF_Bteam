/*!
@file Spike.cpp
@brief �X�e�[�W�M�~�b�N�̞�
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;
	using namespace GimmickAngle;

	// �������̏���
	void Spike::OnCreate()
	{
		// �p�����̐������̏��������s
		Gimmick::OnCreate();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(m_angle != eAngle::All ? L"SPIKE" : L"SPIKE_BLOCK");
		m_ptrDraw->SetMeshToTransformMatrix(m_angle != eAngle::All ? m_modelMat : m_blockMat);
		m_ptrDraw->SetEmissive(Col4(0.5f, 0.5f, 0.0f, 1.0f));
		m_ptrDraw->AddAnimation(L"ATTACK", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ATTACK");

		// �^�O��ݒ�
		AddTag(L"Spike");
	}

	// ���t���[���X�V����
	void Spike::OnUpdate()
	{
		// �p�����̖��t���[���X�V���������s
		CubeObject::OnUpdate();

		// �A�j���[�V�����̍X�V
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// �������̏���
	void MoveSpike::OnCreate()
	{
		// �p�����̐������̏��������s
		Spike::OnCreate();

		// �R���W�����̉����o�������𖳌�
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// �����^�C�v�ɉ����ă|�C���g��ݒ�
		switch (m_moveType)
		{
		case eMoveType::UpDown: // �㉺�Ȃ�
			m_movePointA += UP_VEC * m_moveLength;   // ������x�N�g���ƈړ������Őݒ�
			m_movePointB += DOWN_VEC * m_moveLength; // �������x�N�g���ƈړ������Őݒ�
			break;

		case eMoveType::LeftRight: // ���E�Ȃ�
			m_movePointA += RIGHT_VEC * m_moveLength; // �E�����x�N�g���ƈړ������Őݒ�
			m_movePointB += LEFT_VEC * m_moveLength;  // �������x�N�g���ƈړ������Őݒ�
			break;

		default:
			break;
		}
	}

	// ���t���[���X�V����
	void MoveSpike::OnUpdate()
	{
		// �p�����̖��t���[���X�V���������s
		Spike::OnUpdate();

		// �X�e�[�g�̍X�V���������s
		UpdateState();
	}

	// �X�e�[�g�̍X�V�������s
	void MoveSpike::UpdateState()
	{
		// ���x�ƈړ�������0����Ȃ����
		if (m_moveSpeed > 0.0f && m_moveLength > 0.0f)
		{
			// �X�e�[�g���ҋ@���Ȃ�
			if (m_moveState == eMoveState::StandBy)
			{
				// �ҋ@���̏��������s
				StandbyState();
			}

			// �X�e�[�g���J�n���̍��W�Ȃ�
			if (m_moveState == eMoveState::StartPos)
			{
				// �|�C���gA�Ɉړ�
				if (PointState(m_startPos, m_movePointA, 1.05f))
				{
					// �X�e�[�g�̕ύX�ƕێ�
					m_currentState = eMoveState::StartPos;
					m_moveState = eMoveState::StandBy;
				}
			}

			// �X�e�[�g���|�C���gA�Ɉړ��Ȃ�
			if (m_moveState == eMoveState::MoveA)
			{
				// �|�C���gA�Ɉړ�
				if (PointState(m_movePointB, m_movePointA, m_moveSpeed))
				{
					// �X�e�[�g�̕ύX�ƕێ�
					m_currentState = eMoveState::MoveA;
					m_moveState = eMoveState::StandBy;
				}
			}

			// �X�e�[�g���|�C���gB�Ɉړ��Ȃ�
			if (m_moveState == eMoveState::MoveB)
			{
				// �|�C���gB�Ɉړ�
				if (PointState(m_movePointA, m_movePointB, m_moveSpeed))
				{
					// �X�e�[�g�̕ύX�ƕێ�
					m_currentState = eMoveState::MoveB;
					m_moveState = eMoveState::StandBy;
				}
			}
		}
	}

	// �ҋ@���̃X�e�[�g
	void MoveSpike::StandbyState()
	{
		// 1�b�̃^�C�}�[
		if (SetTimer(1.0f))
		{
			// �O��̃X�e�[�g�ɉ����ăX�e�[�g��ύX
			switch (m_currentState)
			{
			case eMoveState::StartPos: // �J�n���̍��W�Ȃ�
				m_moveState = eMoveState::MoveB; // �|�C���gB�Ɉړ��ɐݒ�
				break;

			case eMoveState::MoveA: // �|�C���gA�Ɉړ��Ȃ�
				m_moveState = eMoveState::MoveB; // �|�C���gB�Ɉړ��ɐݒ�
				break;

			case eMoveState::MoveB: // �|�C���gB�Ɉړ��Ȃ�
				m_moveState = eMoveState::MoveA; // �|�C���gA�Ɉړ��ɐݒ�
				break;

			default:
				break;
			}
		}
	}

	// �|�C���g�Ɉړ��X�e�[�g
	bool MoveSpike::PointState(const Vec3& start, const Vec3& end, float time)
	{
		bool achiev = false; // �ړ������������̕ϐ�

		float totalTime = GetTime(time) / time; // �o�ߎ��Ԃ�0.0�`1.0�͈̔͂���
		Vec3 pos = Utility::Lerp(start, end, totalTime); // Lerp�֐��ō��W�����߂�

		// �o�ߎ��Ԃ�1.0�Ȃ�
		if (totalTime >= 1.0f)
		{
			pos = end; // ���W���I�����W��
			achiev = true; // �I���^�U��true��
		}
		else
		{
			SetTimer(time); // �^�C�}�[���J�n����
		}

		// ���W���X�V
		SetPosition(pos);

		// �^�U��Ԃ�
		return achiev;
	}
}