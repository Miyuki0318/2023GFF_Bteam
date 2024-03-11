/*!
@file NextPlayer.cpp
@brief ���̃X�e�[�W�։�ʗp�̃v���C���[
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void NextPlayer::OnCreate()
	{
		// �p�����̐������̏��������s
		Player::OnCreate();

		// ���W�Ƒ̂̃��f���̃{�[���̔z����擾
		Vec3 armPos = m_position;
		const auto& bone = m_bodyDraw->GetVecLocalBones();

		// �r�Ɠ��̃��f���}�g���N�X�̃|�W�V����y���̍����ƃ{�[���̃A�j���[�V�����ʂ��|�W�V�������獷������
		armPos.y -= (m_armMat.getMajor3().y - m_bodyMat.getMajor3().y) - bone.at(1).getTranslation().y;

		// ���W�̍X�V
		m_arm.lock()->SetPosition(armPos);

		m_arrow.lock()->SetDrawActive(false);
		m_airEffect.lock()->SetDrawActive(false);
	}

	// ���t���[���X�V����
	void NextPlayer::OnUpdate()
	{
		// �X�e�[�W�ƃX�e�[�W�X�e�[�g�̎擾
		const auto& stage = GetTypeStage<NextStage>();
		const auto& state = stage->GetStageState();

		// �X�e�[�g�ɉ����ď���
		switch (state)
		{
		case NextStage::FadeIn: // �t�F�[�h�C���X�e�[�g�Ȃ�
			RotatePlayer(); // �v���C���[�̉�]���������s
			SmokeEffect(Vec2(m_velocity).normalize() * 100.0f); // ���̃G�t�F�N�g�𐶐�
			break;

		case NextStage::Select: // �Z���N�g�X�e�[�g�Ȃ�
			SelectState();  // �Z���N�g�X�e�[�g���̏��������s
			RotatePlayer(); // �v���C���[�̉�]���������s
			SmokeEffect(Vec2(m_velocity).normalize() * 100.0f); // ���̃G�t�F�N�g�𐶐�
			break;

		case NextStage::StageMove: // �X�e�[�W�Ɉړ��X�e�[�g�Ȃ�
			MovePlayer();     // �v���C���[�̈ړ����������s
			MoveReduction();  // �ړ����x�̌������������s
			RotatePlayer();   // �v���C���[�̉�]���������s
			NextStageCheck(); // ���̃X�e�[�W�Ɉړ��������̊m�F���������s
			SmokeEffect(Vec2(m_velocity.x, 0.0f).normalize() * 100.0f); // ���̃G�t�F�N�g�𐶐�
			break;

		case NextStage::TitleBack: // �^�C�g���ɖ߂�X�e�[�g�Ȃ�
			MovePlayer();    // �v���C���[�̈ړ����������s
			MoveReduction(); // �ړ����x�̌������������s
			TitleDrop();     // �v���C���[�̎��S���̗������������s
			break;

		default:
			break;
		}

		// �V�[���h�G�t�F�N�g�̍X�V����
		m_shieldEffect.lock()->UpdateEffect();
	}

	// A�{�^������������
	void NextPlayer::OnRushA()
	{
		// �p�����[�^�̐ݒ�
		m_acsel = m_moveAcsel;
		m_status.Set(eStatus::IsAir, eStatus::IsFirePossible, eStatus::IsCannonFire) = true;
		m_status.Set(eStatus::IsCannonStandby) = false;

		StartSE(L"AIRSHOCK_SE", 1.0f); // SE�̍Đ�

		// -180�x���O�p�֐����g���Ĉړ��ʂ�ݒ肷��
		float rad = -XM_PI;
		m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;
	}

	// �v���C���[�̉�]����
	void NextPlayer::RotatePlayer()
	{
		// �r�̉�]
		float arm = atan2f(m_velocity.y, m_velocity.x) + XM_PIDIV2;
		m_arm.lock()->SetRotation(Vec3(0.0f, 0.0f, arm));
	}

	// �I�����X�e�[�g
	void NextPlayer::SelectState()
	{
		const auto& stage = GetTypeStage<NextStage>(); // �X�e�[�W�̎擾
		const auto& select = stage->GetSelect(); // �X�e�[�W����I�����̎擾
		const bool& inputA = Input::GetReleaseA(); // A�{�^������
	
		// A�{�^�����͂���������
		if (inputA)
		{
			// �I�����ɉ����ď���
			switch (select)
			{
			case NextStage::Next: // ���̃X�e�[�W�ɂȂ�
				OnRushA(); // A�{�^�����͎��̏��������s
				stage->SetStageState(NextStage::StageMove); // �X�e�[�W�X�e�[�g��ύX
				break;

			case NextStage::Back: // �^�C�g���ɖ߂�Ȃ�
				DropSetup(); // �����̃Z�b�g�A�b�v���s��
				stage->SetStageState(NextStage::TitleBack); // �X�e�[�W�X�e�[�g��ύX
				break;

			default:
				break;
			}
		}
	}

	// �����̃Z�b�g�A�b�v
	void NextPlayer::DropSetup()
	{
		// SE�̍Đ�
		StartSE(L"DAMAGE_SE", 1.0f); // SE�̍Đ�

		// ���W�ƈړ��ʂƉ����x��ݒ�
		m_velocity = m_deffVelo * 2.5f;
		m_acsel = m_dropAcsel;

		// �R���W�������A�N�e�B�u��
		m_ptrColl->SetUpdateActive(false);
	}

	// ��������
	void NextPlayer::TitleDrop()
	{
		// �����̍��W�ƌ��݂̍��W���璷�������߂�
		const auto& stage = GetTypeStage<NextStage>();
		const Vec3 defPos = Vec3(0.0f);
		float length = (defPos - GetPosition()).length();

		// Z����]����������
		Vec3 rot = GetRotation();
		rot.z += DELTA_TIME * 5.0f;
		if (rot.z >= XM_PIDIV2) rot.z = -XM_PIDIV2;

		// ���[�e�[�V�����̍X�V
		SetRotation(rot);
		m_arm.lock()->SetRotation(rot + Vec3(0.0f, 0.0f, XM_PI));

		// ��������ʊO�̋����ɍs������
		if (length >= 22.5f)
		{
			// �X�e�[�g���o�b�N�t�F�[�h�ɂ��ASE���Đ�
			stage->SetStageState(NextStage::MetalFade);
			stage->CreateSE(L"METAL_SE", 0.75f);
		}
	}
	
	// ���̃X�e�[�W�Ɉړ��������̃`�F�b�N(���o)
	void NextPlayer::NextStageCheck()
	{
		// �����̍��W�ƌ��݂̍��W���璷�������߂�
		const Vec3 defPos = Vec3(0.0f);
		float length = (defPos - GetPosition()).length();

		// ��������ʊO�̋����ɍs������
		if (length >= 25.0f)
		{
			// �X�e�[�g���t�F�[�h�A�E�g�ɂ���
			SetStageState<NextStage>(NextStage::FadeOut);
		}
	}

	// ���̃G�t�F�N�g
	void NextPlayer::SmokeEffect(const Vec2& velo)
	{
		// ���p�[�e�B�N���̏�����
		const auto& particle = m_particle.lock()->InsertParticle(2);
		particle->SetEmitterPos(GetPosition());
		particle->SetTextureResource(L"SMOKE_TX");
		particle->SetMaxTime(3.0f);
		particle->SetDrawOption(Particle::Normal);

		// ���������X�v���C�g��z��Ŏ擾
		vector<ParticleSprite>& pSpriteVec = particle->GetParticleSpriteVec();

		// �X�v���C�g�̐����[�v
		for (auto& sprite : pSpriteVec)
		{
			// �X�v���C�g�̏������ݒ�(�ړ��ʁE�X�P�[���E���[�e�[�V����)
			sprite.m_Velocity = velo;
			sprite.m_LocalScale = Vec2(m_acsel > 2.0f ? m_acsel / 2.5f : 0.0f);
			sprite.m_LocalQt.rotationZ(DegToRad(RangeRand(360.0f, 0.0f)));
		}
	}
}