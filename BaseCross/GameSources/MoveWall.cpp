/*!
@file MoveWall.cpp
@brief �X�e�[�W�M�~�b�N�̓�����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void MoveWall::OnCreate()
	{
		// �p�����̐������̏��������s
		Gimmick::OnCreate();
		
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"BLOCK");
		m_ptrDraw->SetTextureResource(L"METAL_TX");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		
		// ���������ɉ����Đݒ�
		switch (m_moveType)
		{
		// �������Ȃ�
		case MoveWall::eMoveType::Down:
		case MoveWall::eMoveType::DownSE:

			// �������ɊJ�������̍��W��ݒ�
			m_movePoint += DOWN_VEC * (m_moveLength * 2.0f);
			break;

		// ������Ȃ�
		case MoveWall::eMoveType::Up:
		case MoveWall::eMoveType::UpSE:

			// ������ɊJ�������̍��W��ݒ�
			m_movePoint += UP_VEC * (m_moveLength * 2.0f);
			break;

		// �������Ȃ�
		case MoveWall::eMoveType::Left:
		case MoveWall::eMoveType::LeftSE:

			// �������ɊJ�������̍��W��ݒ�
			m_movePoint += LEFT_VEC * (m_moveLength * 2.0f);
			break;

		// �E�����Ȃ�
		case MoveWall::eMoveType::Right:
		case MoveWall::eMoveType::RightSE:

			// �E�����ɊJ�������̍��W��ݒ�
			m_movePoint += RIGHT_VEC * (m_moveLength * 2.0f);
			break;

		default:
			break;
		}

		// �^�O��ݒ�
		AddTag(L"MoveWall");
	}

	// ���t���[���X�V����
	void MoveWall::OnUpdate()
	{
		// �p�����̖��t���[���X�V���������s
		Gimmick::OnUpdate();

		// �J�p�̃{�^�������o�^�Ȃ�
		if (m_buttons.empty())
		{
			// �{�^���̓o�^���s��
			SetTargetButton();
		}
		else
		{
			// �J���������s
			MoveWallBlock(m_startPos, m_movePoint);
		}

		// �����^�C�v��SE�t���Ȃ�
		if (m_moveType >= eMoveType::DownSE)
		{
			// �Q�[��������Ȃ��Ȃ�
			if (GetStageState<GameStage>() != GameStage::GameNow)
			{
				StopSE(L"METAL_SE"); // SE�̒�~
			}
		}
	}

	// �X�V�������s�����̐ݒ�
	void MoveWall::SetUpdateActive(bool b)
	{
		// �p�����̍X�V�������s�����̐ݒ�����s
		TemplateObject::SetUpdateActive(b);

		// �X�V�������~����Ȃ�SE����~������
		if (!b) StopSE(L"METAL_SE");
	}

	// �J�p�̃{�^���o�^
	void MoveWall::SetTargetButton()
	{
		// �M�~�b�N�O���[�v�̔z����擾
		const auto& gimmickVec = GetStage()->GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
		
		// �I�u�W�F�N�g�̐����[�v
		for (const auto& gimmick : gimmickVec)
		{
			// �G���[�`�F�b�N
			if (!gimmick.lock()) continue;

			// �{�^���^�ɃL���X�g
			const auto& button = dynamic_pointer_cast<Button>(gimmick.lock());
			if (!button) continue;

			// �{�^���̎��ʃi���o�[�ƈ�v������
			if (button->GetButtonNumber() == m_number)
			{
				// �{�^���z��ɒǉ�
				m_buttons.push_back(button);
			}
		}
	}

	// �J���̈ړ��֐�
	void MoveWall::MoveWallBlock(const Vec3& start, const Vec3& end)
	{
		bool input = false; // �J�p�{�^���̓��͂����邩�̐^�U

		// �{�^���I�u�W�F�N�g�̐����[�v
		for (const auto& button : m_buttons)
		{
			// �{�^���̓��͂������
			if (button.lock()->GetInput())
			{
				// ���͐^�U��true�ɂ��ă��[�v���I��
				input = true;
				break;
			}
		}

		// �ړ��ʂ���̗͂L���Őݒ�
		m_moveRatio += input ? DELTA_TIME * m_moveSpeed : -DELTA_TIME;
		m_moveRatio = min(m_moveRatio, m_moveSpeed);
		m_moveRatio = max(m_moveRatio, 0.0f);

		// SE�t���Ȃ�
		if (m_moveType >= eMoveType::DownSE)
		{
			// �ړ��ʂ��O��Ƒ���Ȃ�
			if (m_moveRatio != m_currentRatio)
			{
				// �ړ��ʂ��J�n�l�܂��͏I���l�Ȃ�
				if (m_moveRatio == 0.0f || m_moveRatio == m_moveSpeed)
				{
					// �ړ�����SE���~���A��~����SE���Đ�
					StopSE(L"METAL_SE");
					StartSE(L"METAL_STOP_SE", 0.75f);
				}
			}

			// �{�^�����͂��O��Ƒ���Ȃ�
			if (m_currentInput != input)
			{
				// �ړ�����SE���Đ�������
				StopSE(L"METAL_SE");
				StartSE(L"METAL_SE", 0.2f);
			}
		}

		// �ړ��ʂƓ��͐^�U��ێ�
		m_currentRatio = m_moveRatio;
		m_currentInput = input;

		// ���W��Lerp�Őݒ肵�X�V
		Vec3 pos = Utility::Lerp(start, end, m_moveRatio / m_moveSpeed);
		SetPosition(pos);
	}
}