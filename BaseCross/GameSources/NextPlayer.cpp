#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void NextPlayer::OnCreate()
	{
		Player::OnCreate();

		Vec3 pos = m_position;
		auto& bone = m_bodyDraw->GetVecLocalBones();
		pos.y -= (m_armMat.getMajor3().y - m_bodyMat.getMajor3().y) - bone.at(1).getTranslation().y;
		m_arm.lock()->SetPosition(pos);
	}

	void NextPlayer::OnUpdate()
	{
		const auto& stage = GetTypeStage<NextStage>();
		const auto& state = stage->GetStageState();

		switch (state)
		{
		case NextStage::FadeIn:
			RotatePlayer();
			UpdateParticle(Vec2(m_velocity).normalize() * 100.0f);
			break;

		case NextStage::Select:
			SelectState();
			RotatePlayer();
			UpdateParticle(Vec2(m_velocity).normalize() * 100.0f);
			break;

		case NextStage::StageMove:
			MovePlayer();
			MoveReduction();
			RotatePlayer();
			NextStageMove();
			UpdateParticle(Vec2(m_velocity.x, 0.0f).normalize() * 100.0f);
			break;

		case NextStage::TitleBack:
			MovePlayer();
			MoveReduction();
			DeathDrop();
			break;

		default:
			break;
		}

		vector<Vec3> point;
		m_shieldEffect.lock()->UpdateEffect();
		m_aligment.lock()->UpdateEffect(point);
		m_airEffect.lock()->SetDrawActive(false);
	}

	void NextPlayer::OnRushA()
	{
		// �����o�ϐ��̐ݒ�
		m_acsel = m_moveAcsel;
		m_isAir = true;
		m_firePossible = true;
		m_cannonFire = true;
		m_cannonStandby = false;

		// ��C��Z�������W�A���ɕϊ����A�ړ��ʂ�ݒ肷��
		float rad = -XM_PI;
		m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;
	}

	void NextPlayer::RotatePlayer()
	{
		// �r�̉�]
		float arm = atan2f(m_velocity.y, m_velocity.x) + XM_PIDIV2;
		m_arm.lock()->SetRotation(Vec3(0.0f, 0.0f, arm));
	}

	void NextPlayer::SelectState()
	{
		const auto& stage = GetTypeStage<NextStage>();
		const auto& select = stage->GetSelect();
		const bool& inputA = Input::GetReleaseA();
		switch (select)
		{
		case NextStage::Next:
			if (inputA)
			{
				OnRushA();
				StartSE(L"AIRSHOCK_SE", 1.0f);
				stage->SetStageState(NextStage::StageMove);
			}
			break;

		case NextStage::Back:
			if (inputA)
			{
				DeathSetup();
				StartSE(L"DAMAGE_SE", 1.0f);
				stage->SetStageState(NextStage::TitleBack);
			}
			break;

		default:
			break;
		}
	}

	void NextPlayer::DeathSetup()
	{
		// SE�̍Đ�
		StartSE(L"DAMAGE_SE", 0.75f);

		// ���W�ƈړ��ʂƉ����x��ݒ�
		m_velocity = m_deffVelo * 2.5f;
		m_acsel = 2.5f;

		// �R���W�������A�N�e�B�u��
		m_ptrColl->SetUpdateActive(false);
	}

	void NextPlayer::DeathDrop()
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
			stage->SetStageState(NextStage::BackFade);
			stage->CreateSE(L"METAL_SE", 0.75f);
		}
	}
	
	void NextPlayer::NextStageMove()
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

	void NextPlayer::UpdateParticle(const Vec2& velo)
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
			sprite.m_LocalQt.rotationZ(Utility::DegToRad(Utility::RangeRand(360.0f, 0.0f)));
		}
	}
}