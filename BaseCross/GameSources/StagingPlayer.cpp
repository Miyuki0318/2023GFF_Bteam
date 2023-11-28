#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void StagingPlayer::OnReleaseA()
	{
		Vec2 stick = Input::GetLStickValue().round(1);

		if (Input::GetLStickValue().length() > 0.0f)
		{
			stick.y = 0.0f;
			m_velocity = m_deffVelo * 3.0f;
			m_velocity.x = stick.length() > 0.5f ? stick.x > 0.0f ? 2.25f : -2.25f : 0.0f;

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
	}

	void StagingPlayer::RotateAligment()
	{
		Vec2 stick = Input::GetLStickValue().round(1);
		Vec2 velo = m_deffVelo / 10.0f;
		stick.y = 0.0f;
		velo.x = stick.length() > 0.5f ? stick.x > 0.0f ? 0.05f : -0.05f : 0.0f;

		// オブジェクトの数分ループ
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
		m_aligment.lock()->SetDrawActive(Input::GetLStickValue().length() > 0.0f);
	}
}