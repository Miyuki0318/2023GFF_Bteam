/*!
@file Convayor.cpp
@brief �X�e�[�W�M�~�b�N�̃x���g�R���x�A
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void Convayor::OnCreate()
	{
		// �p�����̐������̊֐������s����
		Gimmick::OnCreate();

		// �`��R���|�[�l���g�̎擾
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();

		// �x���g�^�C�v�ɉ����ă��b�V���̐ݒ�
		switch (m_type)
		{
		case eBeltType::Middle: // �����Ȃ�
			m_ptrDraw->SetMultiMeshResource(L"MCONVAYOR"); // �����p�̃��b�V���L�[��
			break;

		case eBeltType::Side: // �O���Ȃ� 
			m_ptrDraw->SetMultiMeshResource(L"SCONVAYOR"); // �O���p�̃��b�V���L�[��
			break;

		default:
			break;
		}

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(m_animeKey.at(toSize_t(eRotType::LeftRot)), 0, 60, true);
		m_ptrDraw->AddAnimation(m_animeKey.at(toSize_t(eRotType::RightRot)), 70, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(m_animeKey.at(toSize_t(m_rotate)));

		// �R���W�����R���|�[�l���g�̐ݒ�
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);

		// �^�O��ݒ�
		AddTag(L"Convayor");
	}

	// ���t���[���X�V����
	void Convayor::OnUpdate()
	{
		// �p�����̍X�V���������s
		CubeObject::OnUpdate();

		// �A�j���[�V�����̍X�V
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// �������̏���
	void ConvayorGuide::OnCreate()
	{
		// ��O�Ɉړ�������
		m_position.z = -1.0f;

		// �p�����̐������̏��������s
		Billboard::OnCreate();

		// �e�N�X�`���̈ړ�������ݒ�
		SetVelocity(-1.0f, 0.0f);

		// ��]�����ɉ����Đݒ�
		switch (m_rotate)
		{
		case Convayor::eRotType::LeftRot: // �������Ȃ�
			SetRotation(0.0f, 0.0f, XM_PI); // Z����180�x��]
			SetVerticesColor(Col4(0.2f, 0.4f, 1.0f, 1.0f)); // ���_�F�𐅐F�ɐݒ�
			break;
		
		case  Convayor::eRotType::RightRot: // �E�����Ȃ�
			SetVerticesColor(Col4(1.0f, 0.65f, 0.15f, 1.0f)); // ���_�F���F�ɐݒ�
			break;

		default:
			break;
		}
	}

	// ���t���[���X�V����
	void ConvayorGuide::OnUpdate()
	{
		// �e�N�X�`������]����
		Billboard::RotateTexture();
	}
}