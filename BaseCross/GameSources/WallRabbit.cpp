/*!
@file WallRabbit.cpp
@brief �ǂɂȂ��Ă郁�J�E�T�M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ���t���[���X�V����
	void WallRabbit::OnUpdate()
	{
		// ���S���Ă��Ȃ����
		if (!m_status(eStatus::IsDeath))
		{
			// �R���W�����̃p�t�H�[�}���X�֐������s
			CollisiontPerformance();
		}

		// ���S�X�e�[�g�Ȃ�
		if (m_state == eState::Death)
		{
			DeathState();    // ���S���̃X�e�[�g���������s
			MoveRabbit();	 // �ړ����������s
			MoveReduction(); // �ړ��ʌ������������s
		}
	}

	// ���S���̃X�e�[�g
	void WallRabbit::DeathState()
	{
		// ���S���ĂȂ����
		if (!m_status(eStatus::IsDeath))
		{
			// ���S��������ɂ���
			m_status.Set(eStatus::IsDeath) = true;

			// �ǃE�T�M�O���[�v���擾
			const auto& wallVec = GetStage()->GetSharedObjectGroup(L"Wall")->GetGroupVector();

			// ���g�ׂ̗̃E�T�M��T���āu�ꏏ�Ɏ��́H�v���ă����w���s������
			// ���g�̍��W�ƃX�P�[�����擾
			Vec3 thisPos = GetPosition();
			Vec3 scale = GetScale();

			vector<weak_ptr<Rabbit>> rabbitVec; // �ۑ��p�z��

			// �ǃE�T�M�̐����[�v
			for (const auto& weakObj : wallVec)
			{
				// �G���[�`�F�b�N
				if (!weakObj.lock()) continue;

				// �^�L���X�g
				const auto& wall = dynamic_pointer_cast<Rabbit>(weakObj.lock());
				if (!wall) continue;
				
				// ���Ɏ���ł����疳��
				if (wall->GetIsDeath()) continue;

				// ���̕ǃE�T�M�̍��W���擾
				Vec3 pos = wall->GetPosition();

				// ���g�̍��W�̃X�P�[��X�����Z�A�܂��͌��Z�������W�ɋ����
				if (pos == thisPos + Vec3(scale.x, 0.0f, 0.0f) || pos == thisPos - Vec3(scale.x, 0.0f, 0.0f))
				{
					rabbitVec.push_back(wall); // �z��ɒǉ�
				}

				// ���g�̍��W�̃X�P�[��Y�����Z�A�܂��͌��Z�������W�ɋ����
				if (pos == thisPos + Vec3(0.0f, scale.y, 0.0f) || pos == thisPos - Vec3(0.0f, scale.y, 0.0f))
				{
					rabbitVec.push_back(wall); // �z��ɒǉ�
				}
			}

			// �ǉ����ꂽ�ǃE�T�M�̐����[�v
			for (const auto& rabbit : rabbitVec)
			{
				// �O�̂��߃G���[�`�F�b�N
				if (rabbit.lock())
				{
					// ���S�X�e�[�g�ɕύX
					rabbit.lock()->SetState(eState::Death);
				}
			}

			// �R���W�����R���|�[�l���g�ƃ^�O���폜
			RemoveComponent<CollisionObb>();
			RemoveTag(L"Rabbit");

			// ���������p�̈ړ��ʂƉ����x�ɐݒ�
			m_velocity = Vec2(Utility::RangeRand(-1.0f, 1.0f), -1.5f);
			m_acsel = m_maxAcsel;
		}
		else
		{
			// ��]����
			SetRotation(GetRotation() + Vec3(0.0f, 0.0f, DELTA_TIME * m_dir * Utility::RangeRand(15.0f, 5.0f)));
		}
	}

	// �R���W�����p�t�H�[�}���X
	void WallRabbit::CollisiontPerformance()
	{
		bool active = false; // �A�N�e�B�u���̐^�U

		// �^�[�Q�b�g�̃|�C���^�������
		if (m_targetObj.lock())
		{
			// ���W�ŋ������r
			Vec3 targetPos = m_targetObj.lock()->GetPosition();
			float length = (targetPos - m_position).length();

			// �������Ȃ�A�N�e�B�u�ɂ���
			active = (length <= 4.0f);
		}

		// �R���W�����̐ݒ�
		m_ptrColl->SetUpdateActive(active);
	}
}