/*!
@file JumpCountUI.cpp
@brief �W�����v�\��UI
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void JumpCountUI::OnCreate()
	{
		// �p�����̐������̏��������s
		Sprite::OnCreate();
		
		// �X�e�[�W����v���C���[�̎擾�ƃQ�[�W�p�X�v���C�g�𐶐�
		const auto& stage = GetStage();
		m_player = stage->GetSharedGameObject<Player>(L"Player");
		m_gauge = stage->AddGameObject<Sprite>(L"GAUGE_TX", m_scale, m_position + Vec3(0.0f, 0.0f, 0.1f));

		// �A�C�R���̃X�P�[���ƍ��W���G�A�V���b�N�A�C�R���ɓn���Đ���
		const Vec2 iconScale = Vec2(m_scale.x) * 2.0f;
		const Vec3 iconPos = Vec3(m_position + Vec3(0.0f, -(m_scale.y / 2.0f + m_scale.x), 0.0f));
		m_airShock = stage->AddGameObject<Sprite>(L"WHITE_TX", iconScale, iconPos + Vec3(0.0f, 0.0f, 0.2f), XM_PI);
	}

	// ���t���[���X�V����
	void JumpCountUI::OnUpdate()
	{
		// �Q�[���X�e�[�W�X�e�[�g���Q�[�������玀�S���̗������̊Ԃ��̐^�U�����߂�
		const auto& state = GetTypeStage<GameStage>()->GetStageState();
		const bool gameNow = Utility::GetBetween(state, GameStage::GameNow, GameStage::DeathDrop);
		
		// �v���C���[�̃|�C���^������A�Q�[�����Ȃ�
		if (m_player.lock() && gameNow)
		{
			UpdateUI(); // UI�̍X�V����

			// �`�揈�����A�N�e�B�u��
			SetDrawActive(true);
			m_gauge.lock()->SetDrawActive(true);
			m_airShock.lock()->SetDrawActive(true);
		}
		else
		{
			// �`�揈�����A�N�e�B�u��
			SetDrawActive(false);
			m_gauge.lock()->SetDrawActive(false);
			m_airShock.lock()->SetDrawActive(false);
		}
	}

	// UI�̍X�V����
	void JumpCountUI::UpdateUI()
	{
		// �v���C���[�ƃQ�[�W�̃V�F�A�h�|�C���^���擾
		const auto& player = m_player.lock();
		const auto& gauge = m_gauge.lock();

		// �v���C���[����W�����v�Ɋւ���p�����[�^���擾
		const int& jumpCount = player->GetJumpCount();
		const int& jumpLimit = player->GetJumpLimit();
		const float& recoveryTime = player->GetJumpRecoveryTime();
		const float& recoveryLimit = player->GetJumpRecoveryLimit();

		// �W�����v�\��
		int jumpPossible = jumpLimit - jumpCount;
		
		// �X�P�[���Ɗ���
		float scaleY = m_scale.y / jumpLimit;
		float ratio = recoveryTime / recoveryLimit;
		float value = Utility::Lerp(0.0f, scaleY, ratio);
		float nScaleY = (scaleY * jumpPossible) + value;

		// �l�̏C���ƃQ�[�W�̃X�P�[����ݒ�
		nScaleY = min(nScaleY, m_scale.y);
		gauge->SetScale(Vec2(m_scale.x, nScaleY));
		
		// ���W���X�P�[�������������������Z���Đݒ�
		Vec3 pos = GetPosition();
		pos.y += (nScaleY - m_scale.y) / 2.0f;
		pos.z = gauge->GetPosition().z;
		gauge->SetPosition(pos);

		// ��������Q�[�W�̐F��Lerp�Őݒ�
		ratio = nScaleY / m_scale.y;
		Col4 color = Utility::Lerp(COL_BG, COL_BLUE, ratio);
		gauge->SetDiffuseColor(color);
		gauge->SetEmissiveColor(COL_BLUE);

		// �W�����v�\���̐^�U����A�C�R���̃e�N�X�`����ݒ�
		bool isPosshible = jumpCount < jumpLimit;
		m_airShock.lock()->SetTexture(isPosshible ? L"EFFECT_TX" : L"N_EFFECT_TX");
	}
}