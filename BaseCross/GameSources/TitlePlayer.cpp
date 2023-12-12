#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TitlePlayer::OnUpdate()
	{
		// �X�e�[�W�X�e�[�g�̎擾
		const auto& state = GetStageState<TitleStage>();

		// �X�e�[�W�X�e�[�g���X�^�[�g���[�u�ȏ�Ȃ�
		if (state >= TitleStage::StartMove)
		{
			// ��C�ҋ@��
			if (m_cannonStandby)
			{
				CannonStandby(10.0f);
			}

			// ���[�h�Z���N�g���Ȃ�
			if (state == TitleStage::ModeSelect)
			{
				// A�{�^�����͗L���ł̊֐�����
				if (m_firePossible && Input::GetReleaseA())
				{
					OnRushA();
				}
			}

			// �v���C���[�̈ړ��֐�
			MovePlayer();

			// �ړ����̌�����
			MoveReduction();

			// �v���C���[�̉�]�֐�
			RotatePlayer();
			
			// �Ə��̉�]����
			RotateAligment();

			// �A�j���[�V�����̍Đ�
			AnimationUpdate();

			// �G�t�F�N�g�`��֐�
			EffectUpdate();
		}

		// �f�o�b�O������
		Debug::Log(L"pos : ", m_position);
		Debug::Log(L"velo : ", m_velocity);
		Debug::Log(L"acsel : ", m_acsel);
		Debug::Log(m_isAir != false ? L"��" : L"�ڒn");
		Debug::Log(m_firePossible != false ? L"���ˉ�" : L"���˕s��");
	}

	void TitlePlayer::OnRushA()
	{
		Vec2 stick = Input::GetLStickValue().round(1);

		if (Input::GetLStickValue().length() > 0.0f)
		{
			// �ړ��ʂ̐ݒ�
			stick.y = 0.0f;
			m_velocity = m_deffVelo * 3.0f;
			m_velocity.x = stick.length() > 0.5f ? stick.x > 0.0f ? 2.25f : -2.25f : 0.0f;

			// �����o�ϐ��̐ݒ�
			m_isAir = true;
			m_acsel = m_maxAcsel;
			m_cannonFire = false;
			m_meddleVelo.zero();

			// �r�̃A�j���[�V������ύX
			m_armDraw->ChangeCurrentAnimation(L"FIRE");

			// SE�̍Đ�
			StartSE(L"AIRSHOCK_SE", 0.5f);

			// �X�e�[�W�̕���
			const auto& scene = App::GetApp()->GetScene<Scene>();
			if (m_velocity.x < 0.0f) scene->SetCurrentStage("Normal");
			if (m_velocity.x == 0.0f) scene->SetCurrentStage("Easy");

			// �X�e�[�W�X�e�[�g���C�ҋ@�ɐݒ�
			SetStageState<TitleStage>(TitleStage::CannonStanby);
		}
	}

	void TitlePlayer::RotateAligment()
	{
		Vec2 stick = Input::GetLStickValue().round(1);
		Vec2 velo = m_deffVelo / 10.0f;
		stick.y = 0.0f;
		velo.x = stick.length() > 0.5f ? stick.x > 0.0f ? 0.05f : -0.05f : 0.0f;

		// �I�u�W�F�N�g�̐������[�v
		float loopCount = 1.0f;

		vector<Vec3> points;

		for (size_t i = 0; i < 40; i++)
		{
			Vec2 pos = -velo * m_speed * m_maxAcsel * loopCount;
			velo.y -= m_gravity * 0.016f / 20.0f / 4.0f;
			points.push_back(Vec3(pos.x, pos.y, 0.0f));
			loopCount += 0.25f;
		}

		m_aligment.lock()->UpdateEffect(points);

		bool input = Input::GetLStickValue().length() > 0.0f;
		bool state = GetStageState<TitleStage>() == TitleStage::ModeSelect;
		m_aligment.lock()->SetDrawActive(input && state && m_firePossible);
	}

	void TitlePlayer::CannonStandby(float acsel)
	{
		if (m_activeCannon.lock())
		{
			// ��C�̃V�F�A�h�|�C���^
			const auto& cannon = m_activeCannon.lock();

			// ���ˎ��̎��Ԃ̎擾
			const float& fireTime = cannon->GetFireTime();

			// �`��R���|�[�l���g����A�j���[�V�����̍Đ����Ԃ��擾
			const auto& drawPtr = cannon->GetComponent<PNTBoneModelDraw>();
			if (drawPtr->GetCurrentAnimationTime() > fireTime)
			{
				m_acsel = m_activeCannon.lock()->GetAngle() == Gimmick::Up ? acsel * 2.0f : acsel;
				m_isAir = true;
				m_firePossible = true;
				m_cannonFire = true;
				m_cannonStandby = false;
				m_meddleVelo.zero();

				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				StartSE(L"CANNON_SE", 0.75f);

				const auto& titleStage = GetTypeStage<TitleStage>();
				if (titleStage->GetStageState() == TitleStage::CannonStanby)
				{
					titleStage->SetStageState(TitleStage::FadeOut);
				}
			}
			else
			{
				m_position = m_activeCannon.lock()->GetPosition();
				SetPosition(m_position);
			}
		}
	}
}