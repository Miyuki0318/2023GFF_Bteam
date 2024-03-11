/*!
@file ShieldEffect.cpp
@brief プレイヤーのシールドエフェクト
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void ShieldEffect::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		m_ptrDraw->SetTextureResource(L"SHIELD_TX");
		m_ptrDraw->SetDiffuse(Col4(0.5f, 1.0f, 1.0f, 0.75f));

		// アウトラインオブジェクトの生成
		m_outLine = GetStage()->AddGameObject<ShieldOutLine>();

		SetAlphaActive(true); // 透明色の描画を可能に
		SetDrawLayer(2); // 描画を優先度を2に設定
	}

	// エフェクトの更新処理
	void ShieldEffect::UpdateEffect()
	{
		// ポインタが存在するかのエラーチェック
		if (!m_player.lock()) return;
		if (!m_outLine.lock()) return;

		// プレイヤー型にキャストできるか
		const auto& player = dynamic_pointer_cast<Player>(m_player.lock());
		if (!player) return;

		// シールドアウトライン型にキャストできるか
		const auto& outLine = dynamic_pointer_cast<ShieldOutLine>(m_outLine.lock());
		if (!outLine) return;

		// カメラに対しての角度ベクトルの取得
		const auto& ptrCamera = GetStage()->GetView()->GetTargetCamera(); // カメラの取得
		Vec3 eyePos = ptrCamera->GetEye(); // 視点の座標を取得
		Vec3 pos = GetPosition(); // 自身の座標を取得
		float angle = -atan2(pos.z - eyePos.z, pos.x - eyePos.x); // 角度を求める

		// 角度に対して垂直方向のベクトルを生成
		Vec3 verticalVec = Vec3(-sin(angle + XM_PIDIV2) * 2.0f, cos(angle + XM_PIDIV2), 0.0f);

		// 座標の設定
		const Vec3& playerPos = player->GetPosition(); // プレイヤーの座標を取得
		const float diffY = 0.5f; // 埋まらない為のY軸の差分
		SetPosition(Vec3(playerPos.x, playerPos.y - diffY, -1.5f) + verticalVec);
		outLine->SetPosition(Vec3(playerPos.x, playerPos.y - diffY, -1.4f) + verticalVec);

		// スケールの設定
		const int& shieldCount = player->GetShieldCount(); // シールドの数を取得
		Vec2 scale = Vec2(2.5f + (0.25f * shieldCount)); // シールドのサイズに応じて大きくする
		SetScale(scale);
		outLine->SetScale(scale + Vec2(0.05f));

		// 色をシールドの数に応じて設定
		Col4 color = COL_NONE;
		switch (shieldCount)
		{
		case 1:
			color = COL_BLUE; // 青
			break;

		case 2:
			color = COL_GREAN; // 緑
			break;

		case 3:
			color = COL_RED; // 赤
			break;

		default:
			break;
		}

		// アウトラインの描画コンポーネントの取得
		const auto& outLineDraw = outLine->GetPtrDraw();

		// 色の更新
		outLineDraw->SetEmissive(color); // エミッシブ色を設定

		color.w = outLineDraw->GetDiffuse().w; // 透明色を元の値にする
		outLineDraw->SetDiffuse(color); // ディフューズ色を設定
	}

	// 描画するかの真偽設定
	void ShieldEffect::SetDrawShield(bool b)
	{
		// 描画表示非表示
		SetDrawActive(b);
		m_outLine.lock()->SetDrawActive(b);
	}

	// 生成時の処理
	void ShieldOutLine::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		m_ptrDraw->SetRasterizerState(RasterizerState::CullFront);
		m_ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 0.1f));
		m_ptrDraw->SetEmissive(Col4(0.0f, 0.0f, 1.0f, 1.0f));

		// 透明色描画を可能に
		SetAlphaActive(true);
	}
}