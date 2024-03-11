/*!
@file TitlePlayer.cpp
@brief �^�C�g����ʗp�̃v���C���[
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;

	void TitlePlayer::OnUpdate()
	{
		// �X�e�[�W�X�e�[�g�̎擾
		const auto& state = GetStageState<TitleStage>();

		// �X�e�[�W�X�e�[�g���X�^�[�g���[�u�ȏ�Ȃ�
		if (state >= TitleStage::StartMove)
		{
			// ��C�ҋ@��
			if (m_status(eStatus::IsCannonStandby))
			{
				CannonStandby(10.0f);
			}

			// ���[�h�Z���N�g���Ȃ�
			if (state == TitleStage::ModeSelect)
			{
				// A�{�^�����͗L���ł̊֐�����
				if (m_status(eStatus::IsFirePossible) && GetReleaseA())
				{
					OnRushA();
				}
			}

			// ���G���Ԍo��
			if (m_status(eStatus::IsInvincible))
			{
				InvincibleTimer();
			}

			// �v���C���[�̈ړ��֐�
			MovePlayer();

			// �ړ����̌�����
			MoveReduction();

			// �v���C���[�̉�]�֐�
			RotatePlayer();
			
			// �Ə��̉�]����
			UpdateArrow();

			// �A�j���[�V�����̍Đ�
			AnimationUpdate();

			// �G�t�F�N�g�`��֐�
			EffectUpdate();
		}

		//// �f�o�b�O������
		//Debug::Log(L"pos : ", m_position);
		//Debug::Log(L"velo : ", m_velocity);
		//Debug::Log(L"acsel : ", m_acsel);
		//Debug::Log(m_isAir != false ? L"��" : L"�ڒn");
		//Debug::Log(m_firePossible != false ? L"���ˉ�" : L"���˕s��");
	}

	// A�{�^������������
	void TitlePlayer::OnRushA()
	{
		Vec2 stick = GetLStickValue().round(1); // L�X�e�B�b�N�̓��͗ʂ��擾

		// L�X�e�B�b�N�̓��͂������
		if (IsInputLStick())
		{
			// �ړ��ʂ̐ݒ�
			stick.y = 0.0f;
			m_velocity = m_deffVelo * 3.0f;

			// X���̓��͗ʂ��������킸0.5���傫�������琳���ɉ����Ĉړ��ʂ�ݒ�
			m_velocity.x = stick.length() > 0.5f ? stick.x > 0.0f ? 2.25f : -2.25f : 0.0f;

			// �����o�ϐ��̐ݒ�
			m_status.Set(eStatus::IsAir) = true;
			m_status.Set(eStatus::IsCannonFire) = false;
			m_acsel = m_maxAcsel;
			m_meddleVelo.zero();

			// �r�̃A�j���[�V������ύX
			m_armDraw->ChangeCurrentAnimation(L"FIRE");

			// SE�̍Đ�
			StartSE(L"AIRSHOCK_SE", 0.5f);

			// �X�e�[�W�̕���
			const auto& scene = App::GetApp()->GetScene<Scene>();
			if (m_velocity.x < 0.0f) scene->SetCurrentStage("ExtraNormal");
			if (m_velocity.x > 0.0f) scene->SetCurrentStage("Bornus");
			if (m_velocity.x == 0.0f) scene->SetCurrentStage("ExtraEasy");

			// �X�e�[�W�X�e�[�g���C�ҋ@�ɐݒ�
			SetStageState<TitleStage>(TitleStage::CannonStanby);
		}
	}

	// �O���̉�]�`��
	void TitlePlayer::UpdateArrow()
	{
		// �X�e�B�b�N�̓��͗ʂ���ړ��ʂ�ݒ�
		Vec2 stick = GetLStickValue().round(1);
		Vec2 velo = m_deffVelo / 10.0f;
		stick.y = 0.0f;
		velo.x = stick.length() > 0.5f ? stick.x > 0.0f ? 0.05f : -0.05f : 0.0f;

		// �ߖڂƐߖڂ̃J�E���^
		const int mileNum = 40;
		float mileCount = 1.0f;

		// �ߖڍ��W�z��
		vector<Vec3> points;

		// �ߖڂ̐����[�v���Đߖڂ̍��W��ݒ�
		for (size_t i = 0; i < mileNum; i++)
		{
			Vec2 pos = -velo * m_speed * m_maxAcsel * mileCount;
			velo.y -= m_gravity * 0.016f / 20.0f / 4.0f;
			points.push_back(Vec3(pos.x, pos.y, 0.0f));
			mileCount += 0.25f;
		}
		m_arrow.lock()->UpdateEffect(points);

		// �X�e�B�b�N�̓��͂����胂�[�h�Z���N�g�X�e�[�g���ŁA�G�A�V���b�N���ˉ\�ł���Ε`�悷��
		bool state = GetStageState<TitleStage>() == TitleStage::ModeSelect;
		m_arrow.lock()->SetDrawActive(IsInputLStick() && state && m_status(eStatus::IsFirePossible));
	}

	// ��C�ҋ@��
	void TitlePlayer::CannonStandby(float acsel)
	{
		// ��C�̃|�C���^������Ȃ�
		if (m_activeCannon.lock())
		{
			// ��C�̃V�F�A�h�|�C���^
			const auto& cannon = m_activeCannon.lock();

			// ���ˎ��̎��Ԃ̎擾
			const float& fireTime = cannon->GetFireTime();
			const auto& drawPtr = cannon->GetComponent<PNTBoneModelDraw>();

			// �Đ����Ԃ����ˎ��̎��Ԃ��߂�����
			if (drawPtr->GetCurrentAnimationTime() > fireTime)
			{
				// �����o�ϐ��̐ݒ�
				m_acsel = m_activeCannon.lock()->GetAngle() == GimmickAngle::eAngle::Up ? acsel * 2.0f : acsel;
				m_status.Set(eStatus::IsAir, eStatus::IsFirePossible, eStatus::IsCannonFire) = true;
				m_status.Set(eStatus::IsCannonStandby) = false;
				m_meddleVelo.zero();

				// ��C��Z�������W�A���ɕϊ����A�ړ��ʂ�ݒ肷��
				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				// SE�̍Đ�
				StartSE(L"CANNON_SE", 0.75f);

				// �X�e�[�W�X�e�[�g����C�ҋ@���Ȃ�
				const auto& titleStage = GetTypeStage<TitleStage>();
				if (titleStage->GetStageState() == TitleStage::CannonStanby)
				{
					// �X�e�[�W�X�e�[�g���t�F�[�h�A�E�g�ɕύX
					titleStage->SetStageState(TitleStage::FadeOut);
				}
			}
			else
			{
				// ���g�̍��W���C�̒��ɐݒ肷��
				SetPosition(cannon->GetPosition());
			}
		}
	}
}