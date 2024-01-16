#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void ShieldCountUI::OnCreate()
	{
		Sprite::OnCreate();

		const auto& stage = GetStage();
		m_player = stage->GetSharedGameObject<Player>(L"Player");
		m_gauge = stage->AddGameObject<Sprite>(L"GAUGE_TX", m_scale, m_position + Vec3(0.0f, 0.0f, 0.1f));
		m_ring = stage->AddGameObject<Sprite>(L"GAUGE_TX", m_scale, m_position + Vec3(0.0f, 0.0f, 0.1f));

		VertexData vertex;
		Utility::CircleVerticesIndices(vertex, 6);
		const Vec2 iconScale = Vec2(m_scale.x) * 2.0f;
		const Vec3 iconPos = Vec3(m_position + Vec3(0.0f, -(m_scale.y / 2.0f + m_scale.x), 0.0f));

		m_shield = stage->AddGameObject<Sprite>(L"SHIELD_TX", iconScale, iconPos + Vec3(0.0f, 0.0f, 0.2f), XM_PI);
		auto shieldDraw = m_shield.lock()->GetComponent<PCTSpriteDraw>();
		shieldDraw->CreateMesh(vertex);

		m_circle = stage->AddGameObject<Sprite>(L"SHIELD_TX", iconScale, iconPos + Vec3(0.0f, 0.0f, 0.2f), XM_PI);
		auto circleDraw = m_circle.lock()->GetComponent<PCTSpriteDraw>();
		circleDraw->CreateMesh(vertex);
	}

	void ShieldCountUI::OnUpdate()
	{
		const auto& state = GetTypeStage<GameStage>()->GetStageState();
		const bool gameNow = Utility::GetBetween(state, GameStage::GameNow, GameStage::DeathDrop);
		if (m_player.lock() && gameNow)
		{
			UpdateGauge();
			SetDrawActive(true);
			m_ring.lock()->SetDrawActive(true);
			m_gauge.lock()->SetDrawActive(true);
			m_shield.lock()->SetDrawActive(true);
			m_circle.lock()->SetDrawActive(true);
		}
		else
		{
			SetDrawActive(false);
			m_ring.lock()->SetDrawActive(false);
			m_gauge.lock()->SetDrawActive(false);
			m_shield.lock()->SetDrawActive(false);
			m_circle.lock()->SetDrawActive(false);
		}
	}

	void ShieldCountUI::UpdateGauge()
	{
		const auto& player = m_player.lock();
		const auto& gauge = m_gauge.lock();
		const auto& ring = m_ring.lock();

		const int& shieldCount = player->GetShieldCount();
		const int& shieldLimit = player->GetShieldLimit();
		const int& smallRingCount = player->GetSRingCount();
		const vector<int>& smallRingLimit = player->GetSRingLimit();

		float scaleY = m_scale.y / shieldLimit;
		float ratio = static_cast<float>(smallRingCount) / static_cast<float>(smallRingLimit.at(shieldCount));
		float nScaleY = Utility::Lerp(0.0f, scaleY, ratio);
		nScaleY = min(nScaleY, scaleY);

		Vec3 ringPos = GetPosition();
		ringPos.y += (nScaleY - m_scale.y) / 2.0f + (scaleY * shieldCount);
		ringPos.z = ring->GetPosition().z;
		ring->SetPosition(ringPos);
		ring->SetScale(Vec2(m_scale.x, nScaleY));

		Vec3 gaugePos = GetPosition();
		gaugePos.y += (scaleY * shieldCount - m_scale.y) / 2.0f;
		gaugePos.z = ring->GetPosition().z;
		gauge->SetPosition(gaugePos);
		gauge->SetScale(Vec2(m_scale.x, scaleY * shieldCount));

		ratio = nScaleY / scaleY;
		Col4 ringColor;
		Col4 gaugeColor;
		Col4 shieldColor;
		switch (shieldCount)
		{
		case 0:
			ringColor = Utility::Lerp(COL_BG, COL_BLUE, ratio);
			gaugeColor = COL_ALPHA;
			shieldColor = COL_NONE;
			break;

		case 1:
			ringColor = Utility::Lerp(COL_BLUE, COL_GREAN, ratio);
			gaugeColor = COL_BLUE;
			shieldColor = COL_BLUE;
			break;

		case 2:
			ringColor = Utility::Lerp(COL_GREAN, COL_RED, ratio);
			gaugeColor = COL_GREAN;
			shieldColor = COL_GREAN;
			break;

		case 3:
			ringColor = COL_ALPHA;
			gaugeColor = COL_RED;
			shieldColor = COL_RED;
			break;

		default:
			break;
		}

		ring->SetDiffuseColor(ringColor);
		ring->SetEmissiveColor(ringColor);
		gauge->SetDiffuseColor(gaugeColor);
		gauge->SetEmissiveColor(gaugeColor);
		m_shield.lock()->SetEmissiveColor(shieldColor);
		m_shield.lock()->SetDiffuseColor(shieldCount > 0 ? COL_WHITE : COL_GRAY);

		gaugeColor.w -= 0.75f;
		m_circle.lock()->SetDiffuseColor(gaugeColor);
	}
}