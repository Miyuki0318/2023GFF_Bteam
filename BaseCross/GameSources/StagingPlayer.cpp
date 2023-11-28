#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void StagingPlayer::OnReleaseA()
	{
		// スティック入力を取得し移動ベクトルに保持
		m_velocity = m_deffVelo * 3.0f;

		// メンバ変数の設定
		m_isAir = true;
		m_acsel = m_maxAcsel;
		m_cannonFire = false;
		m_meddleVelo.zero();

		// 腕のアニメーションを変更
		m_armDraw->ChangeCurrentAnimation(L"FIRE");

		// SEの再生
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		audioPtr->Start(L"AIRSHOCK_SE", 0, 0.5f);
	}

	void StagingPlayer::SpikeEnter(const shared_ptr<GameObject>& spike, const Vec3& hitPos)
	{

	}
}