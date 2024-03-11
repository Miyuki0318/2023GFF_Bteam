/*!
@file AirJetEffect.cpp
@brief プレイヤーのエアジェットエフェクト
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void AirJetEffect::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Billboard::OnCreate();

		// 描画優先度の設定
		SetDrawLayer(3);
	}

	// エフェクトの更新処理
	void AirJetEffect::UpdateEffect()
	{
		// プレイヤーのポインタがあれば
		if (m_player.lock())
		{
			// プレイヤー型に型キャスト
			const auto& player = dynamic_pointer_cast<Player>(m_player.lock());
			if (player)
			{
				const float& acsel = player->GetAcsel();  // 加速度の取得
				const Vec2& velo = player->GetVelocity(); // 移動量の取得

				// プレイヤーの座標から移動量の方向×加速度で座標を設定
				Vec3 pos = player->GetPosition() + (Vec3(velo.x, velo.y, 0.0f).normalize() * (acsel * 1.15f));
				
				// プレイヤーモデルへのめり込みを防止
				pos.y += 0.5f;
				pos.z = -1.0f;

				// 座標とローテーションとスケールの更新
				SetPosition(pos);
				SetRotation(0.0f, 0.0f, (atan2f(velo.y, velo.x) - XM_PIDIV2));
				SetScale(Vec2((acsel - 1.0f) * 3.0f));
			}
		}
	}
}