/*!
@file ShieldCountUI.cpp
@brief �V�[���h����UI
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void ShieldCountUI::OnCreate()
	{
		// �p�����̐������̏��������s
		Sprite::OnCreate();

		// �X�e�[�W����v���C���[�̎擾�ƃQ�[�W�p�X�v���C�g��𐶐�
		const auto& stage = GetStage();
		m_player = stage->GetSharedGameObject<Player>(L"Player");
		m_gauge = stage->AddGameObject<Sprite>(L"GAUGE_TX", m_scale, m_position + Vec3(0.0f, 0.0f, 0.1f));
		m_ring = stage->AddGameObject<Sprite>(L"GAUGE_TX", m_scale, m_position + Vec3(0.0f, 0.0f, 0.1f));

		// ���_�f�[�^��6�p�`�̉~�ō쐬�ƃX�P�[���ƍ��W��錾
		VertexData vertex;
		Utility::CircleVerticesIndices(vertex, 6);
		const Vec2 iconScale = Vec2(m_scale.x) * 2.0f;
		const Vec3 iconPos = Vec3(m_position + Vec3(0.0f, -(m_scale.y / 2.0f + m_scale.x), 0.0f));

		// �V�[���h�A�C�R���̐����A�`��R���|�[�l���g���烁�b�V���𐶐�&�u������
		m_shield = stage->AddGameObject<Sprite>(L"SHIELD_TX", iconScale, iconPos + Vec3(0.0f, 0.0f, 0.2f), XM_PI);
		auto shieldDraw = m_shield.lock()->GetComponent<PCTSpriteDraw>();
		shieldDraw->CreateMesh(vertex);

		// �V�[���h�A�C�R���G�t�F�N�g�̐����A�`��R���|�[�l���g���烁�b�V���𐶐�&�u������
		m_circle = stage->AddGameObject<Sprite>(L"SHIELD_TX", iconScale, iconPos + Vec3(0.0f, 0.0f, 0.2f), XM_PI);
		auto circleDraw = m_circle.lock()->GetComponent<PCTSpriteDraw>();
		circleDraw->CreateMesh(vertex);
	}

	// ���t���[���X�V����
	void ShieldCountUI::OnUpdate()
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
			m_ring.lock()->SetDrawActive(true);
			m_gauge.lock()->SetDrawActive(true);
			m_shield.lock()->SetDrawActive(true);
			m_circle.lock()->SetDrawActive(true);
		}
		else
		{
			// �`�揈�����A�N�e�B�u��
			SetDrawActive(false);
			m_ring.lock()->SetDrawActive(false);
			m_gauge.lock()->SetDrawActive(false);
			m_shield.lock()->SetDrawActive(false);
			m_circle.lock()->SetDrawActive(false);
		}
	}

	// UI�̍X�V����
	void ShieldCountUI::UpdateUI()
	{
		// �V�F�A�h�|�C���^�̎擾
		const auto& player = m_player.lock();
		const auto& gauge = m_gauge.lock();
		const auto& ring = m_ring.lock();

		// �v���C���[����V�[���h�Ɋւ���p�����[�^���擾
		const int& shieldCount = player->GetShieldCount();
		const int& shieldLimit = player->GetShieldLimit();
		const int& smallRingCount = player->GetSRingCount();
		const auto& smallRingLimit = player->GetSRingLimit();

		// �X�P�[���Ɗ��������߂�
		float scaleY = m_scale.y / shieldLimit;
		float ratio = static_cast<float>(smallRingCount) / static_cast<float>(smallRingLimit.at(shieldCount));
		float nScaleY = Utility::Lerp(0.0f, scaleY, ratio);
		nScaleY = min(nScaleY, scaleY);

		// �����O�擾���Q�[�W�̍��W�ƃX�P�[���̐ݒ�
		Vec3 ringPos = GetPosition();
		ringPos.y += (nScaleY - m_scale.y) / 2.0f + (scaleY * shieldCount);
		ringPos.z = ring->GetPosition().z;
		ring->SetPosition(ringPos);
		ring->SetScale(Vec2(m_scale.x, nScaleY));

		// �V�[���h�����Q�[�W�̍��W�ƃX�P�[���̐ݒ�
		Vec3 gaugePos = GetPosition();
		gaugePos.y += (scaleY * shieldCount - m_scale.y) / 2.0f;
		gaugePos.z = ring->GetPosition().z;
		gauge->SetPosition(gaugePos);
		gauge->SetScale(Vec2(m_scale.x, scaleY * shieldCount));

		// �X�P�[���̊�������F�����߂�
		ratio = nScaleY / scaleY;
		Col4 ringColor;
		Col4 gaugeColor;
		Col4 shieldColor;

		// �V�[���h�����ŕ��򏈗�
		switch (shieldCount)
		{
		case 0: // 0���Ȃ�
			// ��������F��
			ringColor = Utility::Lerp(COL_BG, COL_BLUE, ratio);
			gaugeColor = COL_ALPHA; 
			shieldColor = COL_NONE;
			break;

		case 1: // 1���Ȃ�
			// �F����ΐF��
			ringColor = Utility::Lerp(COL_BLUE, COL_GREAN, ratio);
			gaugeColor = COL_BLUE;
			shieldColor = COL_BLUE;
			break;

		case 2: // 2���Ȃ�
			// �ΐF����ԐF��
			ringColor = Utility::Lerp(COL_GREAN, COL_RED, ratio);
			gaugeColor = COL_GREAN;
			shieldColor = COL_GREAN;
			break;

		case 3: // 3���Ȃ�
			// �ԂŌŒ�
			ringColor = COL_ALPHA;
			gaugeColor = COL_RED;
			shieldColor = COL_RED;
			break;

		default:
			break;
		}

		// �F���f�B�t���[�Y�F�ƃG�~�b�V�u�F�ɐݒ�
		ring->SetDiffuseColor(ringColor);
		ring->SetEmissiveColor(ringColor);
		gauge->SetDiffuseColor(gaugeColor);
		gauge->SetEmissiveColor(gaugeColor);
		m_shield.lock()->SetEmissiveColor(shieldColor);
		m_shield.lock()->SetDiffuseColor(shieldCount > 0 ? COL_WHITE : COL_GRAY);

		// �����x�������Đݒ�
		gaugeColor.w -= 0.75f;
		m_circle.lock()->SetDiffuseColor(gaugeColor);
	}
}