/*!
@file Bumper.cpp
@brief �X�e�[�W�M�~�b�N�̓����ǂ̊J�p�{�^��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void Button::OnCreate()
	{
		// �p�����̐������̏��������s
		Gimmick::OnCreate();
		
		// �R���W�������A�N�e�B�u�ɂ��A�����������I�t�ɂ���
		m_ptrColl->SetUpdateActive(true);
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BUTTON");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"PUSH", 0, 30, false);
		m_ptrDraw->AddAnimation(L"REVERSE", 30, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"REVERSE");
	}

	// ���t���[���X�V����
	void Button::OnUpdate()
	{
		// �Փ˂��Ă邩���擾
		m_active = GetHitButton();

		// ���͎��Ԃ�ݒ�
		m_reverseTime = min(1.0f - m_ptrDraw->GetCurrentAnimationTime(), 1.0f);

		// ���͏�Ԃ��O��Ƒ���ł����
		if (m_active != m_current)
		{
			// �A�j���[�V������ύX���A���͎��Ԃ���J�n
			m_ptrDraw->ChangeCurrentAnimation(m_active ? L"PUSH" : L"REVERSE", m_reverseTime);
		}

		// �A�j���[�V�����̍X�V
		m_ptrDraw->UpdateAnimation(DELTA_TIME * 2.0f);

		// ���͏�Ԃ̕ێ�
		m_current = m_active;
	}
}