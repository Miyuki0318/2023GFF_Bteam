/*!
@file ShieldEffect.cpp
@brief �v���C���[�̃V�[���h�G�t�F�N�g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void ShieldEffect::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		m_ptrDraw->SetTextureResource(L"SHIELD_TX");
		m_ptrDraw->SetDiffuse(Col4(0.5f, 1.0f, 1.0f, 0.75f));

		// �A�E�g���C���I�u�W�F�N�g�̐���
		m_outLine = GetStage()->AddGameObject<ShieldOutLine>();

		SetAlphaActive(true); // �����F�̕`����\��
		SetDrawLayer(2); // �`���D��x��2�ɐݒ�
	}

	// �G�t�F�N�g�̍X�V����
	void ShieldEffect::UpdateEffect()
	{
		// �|�C���^�����݂��邩�̃G���[�`�F�b�N
		if (!m_player.lock()) return;
		if (!m_outLine.lock()) return;

		// �v���C���[�^�ɃL���X�g�ł��邩
		const auto& player = dynamic_pointer_cast<Player>(m_player.lock());
		if (!player) return;

		// �V�[���h�A�E�g���C���^�ɃL���X�g�ł��邩
		const auto& outLine = dynamic_pointer_cast<ShieldOutLine>(m_outLine.lock());
		if (!outLine) return;

		// �J�����ɑ΂��Ă̊p�x�x�N�g���̎擾
		const auto& ptrCamera = GetStage()->GetView()->GetTargetCamera(); // �J�����̎擾
		Vec3 eyePos = ptrCamera->GetEye(); // ���_�̍��W���擾
		Vec3 pos = GetPosition(); // ���g�̍��W���擾
		float angle = -atan2(pos.z - eyePos.z, pos.x - eyePos.x); // �p�x�����߂�

		// �p�x�ɑ΂��Đ��������̃x�N�g���𐶐�
		Vec3 verticalVec = Vec3(-sin(angle + XM_PIDIV2) * 2.0f, cos(angle + XM_PIDIV2), 0.0f);

		// ���W�̐ݒ�
		const Vec3& playerPos = player->GetPosition(); // �v���C���[�̍��W���擾
		const float diffY = 0.5f; // ���܂�Ȃ��ׂ�Y���̍���
		SetPosition(Vec3(playerPos.x, playerPos.y - diffY, -1.5f) + verticalVec);
		outLine->SetPosition(Vec3(playerPos.x, playerPos.y - diffY, -1.4f) + verticalVec);

		// �X�P�[���̐ݒ�
		const int& shieldCount = player->GetShieldCount(); // �V�[���h�̐����擾
		Vec2 scale = Vec2(2.5f + (0.25f * shieldCount)); // �V�[���h�̃T�C�Y�ɉ����đ傫������
		SetScale(scale);
		outLine->SetScale(scale + Vec2(0.05f));

		// �F���V�[���h�̐��ɉ����Đݒ�
		Col4 color = COL_NONE;
		switch (shieldCount)
		{
		case 1:
			color = COL_BLUE; // ��
			break;

		case 2:
			color = COL_GREAN; // ��
			break;

		case 3:
			color = COL_RED; // ��
			break;

		default:
			break;
		}

		// �A�E�g���C���̕`��R���|�[�l���g�̎擾
		const auto& outLineDraw = outLine->GetPtrDraw();

		// �F�̍X�V
		outLineDraw->SetEmissive(color); // �G�~�b�V�u�F��ݒ�

		color.w = outLineDraw->GetDiffuse().w; // �����F�����̒l�ɂ���
		outLineDraw->SetDiffuse(color); // �f�B�t���[�Y�F��ݒ�
	}

	// �`�悷�邩�̐^�U�ݒ�
	void ShieldEffect::SetDrawShield(bool b)
	{
		// �`��\����\��
		SetDrawActive(b);
		m_outLine.lock()->SetDrawActive(b);
	}

	// �������̏���
	void ShieldOutLine::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		m_ptrDraw->SetRasterizerState(RasterizerState::CullFront);
		m_ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 0.1f));
		m_ptrDraw->SetEmissive(Col4(0.0f, 0.0f, 1.0f, 1.0f));

		// �����F�`����\��
		SetAlphaActive(true);
	}
}