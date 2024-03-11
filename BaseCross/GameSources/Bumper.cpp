/*!
@file Bumper.cpp
@brief �X�e�[�W�M�~�b�N�̃o���p�[
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void Bumper::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();
		SetTransParam(); // �g�����X�t�H�[���̃p�����[�^��ݒ�

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BUMPER");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"PUSH", 0, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"PUSH");

		// �X�t�B�A�R���W�����̒ǉ�
		m_ptrColl = AddComponent<CollisionSphere>();
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// �^�O��ݒ�
		AddTag(L"Bumper");
	}

	// ���t���[���X�V����
	void Bumper::OnUpdate()
	{
		// �A�j���[�V�����̍X�V����
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// �������̏���
	void MoveBumper::OnCreate()
	{
		// �p�����̐������̏��������s
		Bumper::OnCreate();

		// �^�C�v�ɉ����ă|�C���g�̐ݒ���s��
		switch (m_moveType)
		{
		case eMoveType::UpDown: // �㉺�^�C�v�Ȃ�
			m_movePointA += UP_VEC * m_moveLength;   // ������ɋ��������ɐݒ�
			m_movePointB += DOWN_VEC * m_moveLength; // �������ɋ��������ɐݒ�
			break;

		case eMoveType::LeftRight: // ���E�^�C�v�Ȃ�
			m_movePointA += RIGHT_VEC * m_moveLength; // �E�����ɋ��������ɐݒ�
			m_movePointB += LEFT_VEC * m_moveLength;  // �������ɋ��������ɐݒ�
			break;

		default:
			break;
		}
	}

	// ���t���[���X�V����
	void MoveBumper::OnUpdate()
	{
		// �p�����̖��t���[���X�V���������s
		Bumper::OnUpdate();

		// �X�e�[�g�X�V�����s
		UpdateState();
	}

	// �X�e�[�g�X�V����
	void MoveBumper::UpdateState()
	{
		// ���x�Ƌ�����0����Ȃ����
		if (m_moveSpeed > 0.0f && m_moveLength > 0.0f)
		{
			// �X�e�[�g�ɉ����ď���
			switch (m_moveState)
			{
			case eMoveState::StandBy: // �ҋ@���Ȃ�
				
				// �ҋ@���̃X�e�[�g�����s
				StandbyState();
				break;
			
			case eMoveState::StartPos: // �J�n���̍��W�Ȃ�
				
				// �J�n���̍��W����|�C���gA�Ɉړ�
				if (PointState(m_startPos, m_movePointA, 1.05f))
				{
					// �X�e�[�g�̕ύX�ƕێ�
					m_currentState = eMoveState::StartPos;
					m_moveState = eMoveState::StandBy;
				}
				break;
			
			case eMoveState::MoveA: // �|�C���gA�Ɉړ��Ȃ�

				// �|�C���gA����|�C���gB�Ɉړ�
				if (PointState(m_movePointB, m_movePointA, m_moveSpeed))
				{
					// �X�e�[�g�̕ύX�ƕێ�
					m_currentState = eMoveState::MoveA;
					m_moveState = eMoveState::StandBy;
				}
				break;
			
			case eMoveState::MoveB: // �|�C���gB�Ɉړ�

				// �|�C���gB����|�C���gA�Ɉړ�
				if (PointState(m_movePointA, m_movePointB, m_moveSpeed))
				{
					// �X�e�[�g�̕ύX�ƕێ�
					m_currentState = eMoveState::MoveB;
					m_moveState = eMoveState::StandBy;
				}
				break;
			
			default:
				break;
			}
		}
	}

	// �ҋ@���̃X�e�[�g
	void MoveBumper::StandbyState()
	{
		// 1�b�̃^�C�}�[
		if (SetTimer(1.0f))
		{
			// �O��̃X�e�[�g�ɉ����ď���
			switch (m_currentState)
			{
			case eMoveState::StartPos: // �J�n���̍��W�Ȃ�
				m_moveState = eMoveState::MoveB; // �X�e�[�g���u�|�C���gB�Ɉړ��v�ɕύX
				break;

			case eMoveState::MoveA: // �|�C���gA�Ɉړ��Ȃ�
				m_moveState = eMoveState::MoveB; // �X�e�[�g���u�|�C���gB�Ɉړ��v�ɕύX
				break;

			case eMoveState::MoveB: // �|�C���gB�Ɉړ��Ȃ�
				m_moveState = eMoveState::MoveA; // �X�e�[�g���u�|�C���gA�Ɉړ��v�ɕύX
				break;

			default:
				break;
			}
		}
	}

	// �ړ��X�e�[�g
	bool MoveBumper::PointState(const Vec3& start, const Vec3& end, float time)
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