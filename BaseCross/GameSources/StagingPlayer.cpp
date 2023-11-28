#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void StagingPlayer::OnReleaseA()
	{
		// �X�e�B�b�N���͂��擾���ړ��x�N�g���ɕێ�
		m_velocity = m_deffVelo * 3.0f;

		// �����o�ϐ��̐ݒ�
		m_isAir = true;
		m_acsel = m_maxAcsel;
		m_cannonFire = false;
		m_meddleVelo.zero();

		// �r�̃A�j���[�V������ύX
		m_armDraw->ChangeCurrentAnimation(L"FIRE");

		// SE�̍Đ�
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		audioPtr->Start(L"AIRSHOCK_SE", 0, 0.5f);
	}

	void StagingPlayer::SpikeEnter(const shared_ptr<GameObject>& spike, const Vec3& hitPos)
	{

	}
}