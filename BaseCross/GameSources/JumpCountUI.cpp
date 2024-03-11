/*!
@file JumpCountUI.cpp
@brief ジャンプ可能回数UI
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void JumpCountUI::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Sprite::OnCreate();
		
		// ステージからプレイヤーの取得とゲージ用スプライトを生成
		const auto& stage = GetStage();
		m_player = stage->GetSharedGameObject<Player>(L"Player");
		m_gauge = stage->AddGameObject<Sprite>(L"GAUGE_TX", m_scale, m_position + Vec3(0.0f, 0.0f, 0.1f));

		// アイコンのスケールと座標をエアショックアイコンに渡して生成
		const Vec2 iconScale = Vec2(m_scale.x) * 2.0f;
		const Vec3 iconPos = Vec3(m_position + Vec3(0.0f, -(m_scale.y / 2.0f + m_scale.x), 0.0f));
		m_airShock = stage->AddGameObject<Sprite>(L"WHITE_TX", iconScale, iconPos + Vec3(0.0f, 0.0f, 0.2f), XM_PI);
	}

	// 毎フレーム更新処理
	void JumpCountUI::OnUpdate()
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
			m_gauge.lock()->SetDrawActive(true);
			m_airShock.lock()->SetDrawActive(true);
		}
		else
		{
			// 描画処理を非アクティブに
			SetDrawActive(false);
			m_gauge.lock()->SetDrawActive(false);
			m_airShock.lock()->SetDrawActive(false);
		}
	}

	// UIの更新処理
	void JumpCountUI::UpdateUI()
	{
		// プレイヤーとゲージのシェアドポインタを取得
		const auto& player = m_player.lock();
		const auto& gauge = m_gauge.lock();

		// プレイヤーからジャンプに関するパラメータを取得
		const int& jumpCount = player->GetJumpCount();
		const int& jumpLimit = player->GetJumpLimit();
		const float& recoveryTime = player->GetJumpRecoveryTime();
		const float& recoveryLimit = player->GetJumpRecoveryLimit();

		// ジャンプ可能回数
		int jumpPossible = jumpLimit - jumpCount;
		
		// スケールと割合
		float scaleY = m_scale.y / jumpLimit;
		float ratio = recoveryTime / recoveryLimit;
		float value = Utility::Lerp(0.0f, scaleY, ratio);
		float nScaleY = (scaleY * jumpPossible) + value;

		// 値の修正とゲージのスケールを設定
		nScaleY = min(nScaleY, m_scale.y);
		gauge->SetScale(Vec2(m_scale.x, nScaleY));
		
		// 座標をスケール分引いた半分を加算して設定
		Vec3 pos = GetPosition();
		pos.y += (nScaleY - m_scale.y) / 2.0f;
		pos.z = gauge->GetPosition().z;
		gauge->SetPosition(pos);

		// 割合からゲージの色をLerpで設定
		ratio = nScaleY / m_scale.y;
		Col4 color = Utility::Lerp(COL_BG, COL_BLUE, ratio);
		gauge->SetDiffuseColor(color);
		gauge->SetEmissiveColor(COL_BLUE);

		// ジャンプ可能かの真偽からアイコンのテクスチャを設定
		bool isPosshible = jumpCount < jumpLimit;
		m_airShock.lock()->SetTexture(isPosshible ? L"EFFECT_TX" : L"N_EFFECT_TX");
	}
}