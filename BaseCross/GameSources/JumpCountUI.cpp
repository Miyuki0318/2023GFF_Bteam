#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void JumpCountUI::OnCreate()
	{
		Sprite::OnCreate();
		
		const auto& stage = GetStage();
		m_player = stage->GetSharedGameObject<Player>(L"Player");
		m_gauge = stage->AddGameObject<Sprite>(L"GAUGE_TX", m_scale, m_position + Vec3(0.0f, 0.0f, 0.1f));

		const Vec2 iconScale = Vec2(m_scale.x) * 2.0f;
		const Vec3 iconPos = Vec3(m_position + Vec3(0.0f, -(m_scale.y / 2.0f + m_scale.x), 0.0f));
		m_airShock = stage->AddGameObject<Sprite>(L"WHITE_TX", iconScale, iconPos + Vec3(0.0f, 0.0f, 0.2f), XM_PI);
	}

	void JumpCountUI::OnUpdate()
	{
		const auto& state = GetTypeStage<GameStage>()->GetStageState();
		const bool gameNow = Utility::GetBetween(state, GameStage::GameNow, GameStage::DeathDrop);
		if (m_player.lock() && gameNow)
		{
			UpdateGauge();
			SetDrawActive(true);
			m_gauge.lock()->SetDrawActive(true);
			m_airShock.lock()->SetDrawActive(true);
		}
		else
		{
			SetDrawActive(false);
			m_gauge.lock()->SetDrawActive(false);
			m_airShock.lock()->SetDrawActive(false);
		}
	}

	void JumpCountUI::UpdateGauge()
	{
		const auto& player = m_player.lock();
		const auto& gauge = m_gauge.lock();

		const int& jumpCount = player->GetJumpCount();
		const int& jumpLimit = player->GetJumpLimit();
		const float& recoveryTime = player->GetJumpRecoveryTime();
		const float& recoveryLimit = player->GetJumpRecoveryLimit();

		int jumpPossible = jumpLimit - jumpCount;
		float scaleY = m_scale.y / jumpLimit;
		float ratio = recoveryTime / recoveryLimit;
		float value = Utility::Lerp(0.0f, scaleY, ratio);
		float nScaleY = (scaleY * jumpPossible) + value;

		nScaleY = min(nScaleY, m_scale.y);
		gauge->SetScale(Vec2(m_scale.x, nScaleY));
		
		Vec3 pos = GetPosition();
		pos.y += (nScaleY - m_scale.y) / 2.0f;
		pos.z = gauge->GetPosition().z;
		gauge->SetPosition(pos);

		ratio = nScaleY / m_scale.y;
		Col4 color = Utility::Lerp(COL_BG, COL_BLUE, ratio);
		gauge->SetDiffuseColor(color);
		gauge->SetEmissiveColor(COL_BLUE);

		bool isPosshible = jumpCount < jumpLimit;
		m_airShock.lock()->SetTexture(isPosshible ? L"EFFECT_TX" : L"N_EFFECT_TX");
	}
}