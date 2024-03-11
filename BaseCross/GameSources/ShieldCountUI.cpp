/*!
@file ShieldCountUI.cpp
@brief シールド枚数UI
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void ShieldCountUI::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Sprite::OnCreate();

		// ステージからプレイヤーの取得とゲージ用スプライト二つを生成
		const auto& stage = GetStage();
		m_player = stage->GetSharedGameObject<Player>(L"Player");
		m_gauge = stage->AddGameObject<Sprite>(L"GAUGE_TX", m_scale, m_position + Vec3(0.0f, 0.0f, 0.1f));
		m_ring = stage->AddGameObject<Sprite>(L"GAUGE_TX", m_scale, m_position + Vec3(0.0f, 0.0f, 0.1f));

		// 頂点データを6角形の円で作成とスケールと座標を宣言
		VertexData vertex;
		Utility::CircleVerticesIndices(vertex, 6);
		const Vec2 iconScale = Vec2(m_scale.x) * 2.0f;
		const Vec3 iconPos = Vec3(m_position + Vec3(0.0f, -(m_scale.y / 2.0f + m_scale.x), 0.0f));

		// シールドアイコンの生成、描画コンポーネントからメッシュを生成&置き換え
		m_shield = stage->AddGameObject<Sprite>(L"SHIELD_TX", iconScale, iconPos + Vec3(0.0f, 0.0f, 0.2f), XM_PI);
		auto shieldDraw = m_shield.lock()->GetComponent<PCTSpriteDraw>();
		shieldDraw->CreateMesh(vertex);

		// シールドアイコンエフェクトの生成、描画コンポーネントからメッシュを生成&置き換え
		m_circle = stage->AddGameObject<Sprite>(L"SHIELD_TX", iconScale, iconPos + Vec3(0.0f, 0.0f, 0.2f), XM_PI);
		auto circleDraw = m_circle.lock()->GetComponent<PCTSpriteDraw>();
		circleDraw->CreateMesh(vertex);
	}

	// 毎フレーム更新処理
	void ShieldCountUI::OnUpdate()
	{
		// ゲームステージステートがゲーム中から死亡時の落下中の間かの真偽を求める
		const auto& state = GetTypeStage<GameStage>()->GetStageState();
		const bool gameNow = Utility::GetBetween(state, GameStage::GameNow, GameStage::DeathDrop);

		// プレイヤーのポインタがあり、ゲーム中なら
		if (m_player.lock() && gameNow)
		{
			UpdateUI(); // UIの更新処理

			// 描画処理をアクティブに
			SetDrawActive(true);
			m_ring.lock()->SetDrawActive(true);
			m_gauge.lock()->SetDrawActive(true);
			m_shield.lock()->SetDrawActive(true);
			m_circle.lock()->SetDrawActive(true);
		}
		else
		{
			// 描画処理を非アクティブに
			SetDrawActive(false);
			m_ring.lock()->SetDrawActive(false);
			m_gauge.lock()->SetDrawActive(false);
			m_shield.lock()->SetDrawActive(false);
			m_circle.lock()->SetDrawActive(false);
		}
	}

	// UIの更新処理
	void ShieldCountUI::UpdateUI()
	{
		// シェアドポインタの取得
		const auto& player = m_player.lock();
		const auto& gauge = m_gauge.lock();
		const auto& ring = m_ring.lock();

		// プレイヤーからシールドに関するパラメータを取得
		const int& shieldCount = player->GetShieldCount();
		const int& shieldLimit = player->GetShieldLimit();
		const int& smallRingCount = player->GetSRingCount();
		const auto& smallRingLimit = player->GetSRingLimit();

		// スケールと割合を求める
		float scaleY = m_scale.y / shieldLimit;
		float ratio = static_cast<float>(smallRingCount) / static_cast<float>(smallRingLimit.at(shieldCount));
		float nScaleY = Utility::Lerp(0.0f, scaleY, ratio);
		nScaleY = min(nScaleY, scaleY);

		// リング取得数ゲージの座標とスケールの設定
		Vec3 ringPos = GetPosition();
		ringPos.y += (nScaleY - m_scale.y) / 2.0f + (scaleY * shieldCount);
		ringPos.z = ring->GetPosition().z;
		ring->SetPosition(ringPos);
		ring->SetScale(Vec2(m_scale.x, nScaleY));

		// シールド枚数ゲージの座標とスケールの設定
		Vec3 gaugePos = GetPosition();
		gaugePos.y += (scaleY * shieldCount - m_scale.y) / 2.0f;
		gaugePos.z = ring->GetPosition().z;
		gauge->SetPosition(gaugePos);
		gauge->SetScale(Vec2(m_scale.x, scaleY * shieldCount));

		// スケールの割合から色を求める
		ratio = nScaleY / scaleY;
		Col4 ringColor;
		Col4 gaugeColor;
		Col4 shieldColor;

		// シールド枚数で分岐処理
		switch (shieldCount)
		{
		case 0: // 0枚なら
			// 透明から青色に
			ringColor = Utility::Lerp(COL_BG, COL_BLUE, ratio);
			gaugeColor = COL_ALPHA; 
			shieldColor = COL_NONE;
			break;

		case 1: // 1枚なら
			// 青色から緑色に
			ringColor = Utility::Lerp(COL_BLUE, COL_GREAN, ratio);
			gaugeColor = COL_BLUE;
			shieldColor = COL_BLUE;
			break;

		case 2: // 2枚なら
			// 緑色から赤色に
			ringColor = Utility::Lerp(COL_GREAN, COL_RED, ratio);
			gaugeColor = COL_GREAN;
			shieldColor = COL_GREAN;
			break;

		case 3: // 3枚なら
			// 赤で固定
			ringColor = COL_ALPHA;
			gaugeColor = COL_RED;
			shieldColor = COL_RED;
			break;

		default:
			break;
		}

		// 色をディフューズ色とエミッシブ色に設定
		ring->SetDiffuseColor(ringColor);
		ring->SetEmissiveColor(ringColor);
		gauge->SetDiffuseColor(gaugeColor);
		gauge->SetEmissiveColor(gaugeColor);
		m_shield.lock()->SetEmissiveColor(shieldColor);
		m_shield.lock()->SetDiffuseColor(shieldCount > 0 ? COL_WHITE : COL_GRAY);

		// 透明度を下げて設定
		gaugeColor.w -= 0.75f;
		m_circle.lock()->SetDiffuseColor(gaugeColor);
	}
}