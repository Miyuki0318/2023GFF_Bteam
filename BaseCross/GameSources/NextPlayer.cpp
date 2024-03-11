/*!
@file NextPlayer.cpp
@brief 次のステージへ画面用のプレイヤー
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void NextPlayer::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Player::OnCreate();

		// 座標と体のモデルのボーンの配列を取得
		Vec3 armPos = m_position;
		const auto& bone = m_bodyDraw->GetVecLocalBones();

		// 腕と胴のモデルマトリクスのポジションy軸の差分とボーンのアニメーション量をポジションから差し引く
		armPos.y -= (m_armMat.getMajor3().y - m_bodyMat.getMajor3().y) - bone.at(1).getTranslation().y;

		// 座標の更新
		m_arm.lock()->SetPosition(armPos);

		m_arrow.lock()->SetDrawActive(false);
		m_airEffect.lock()->SetDrawActive(false);
	}

	// 毎フレーム更新処理
	void NextPlayer::OnUpdate()
	{
		// ステージとステージステートの取得
		const auto& stage = GetTypeStage<NextStage>();
		const auto& state = stage->GetStageState();

		// ステートに応じて処理
		switch (state)
		{
		case NextStage::FadeIn: // フェードインステートなら
			RotatePlayer(); // プレイヤーの回転処理を実行
			SmokeEffect(Vec2(m_velocity).normalize() * 100.0f); // 煙のエフェクトを生成
			break;

		case NextStage::Select: // セレクトステートなら
			SelectState();  // セレクトステート時の処理を実行
			RotatePlayer(); // プレイヤーの回転処理を実行
			SmokeEffect(Vec2(m_velocity).normalize() * 100.0f); // 煙のエフェクトを生成
			break;

		case NextStage::StageMove: // ステージに移動ステートなら
			MovePlayer();     // プレイヤーの移動処理を実行
			MoveReduction();  // 移動速度の減衰処理を実行
			RotatePlayer();   // プレイヤーの回転処理を実行
			NextStageCheck(); // 次のステージに移動したかの確認処理を実行
			SmokeEffect(Vec2(m_velocity.x, 0.0f).normalize() * 100.0f); // 煙のエフェクトを生成
			break;

		case NextStage::TitleBack: // タイトルに戻るステートなら
			MovePlayer();    // プレイヤーの移動処理を実行
			MoveReduction(); // 移動速度の減衰処理を実行
			TitleDrop();     // プレイヤーの死亡時の落下処理を実行
			break;

		default:
			break;
		}

		// シールドエフェクトの更新処理
		m_shieldEffect.lock()->UpdateEffect();
	}

	// Aボタンを押した時
	void NextPlayer::OnRushA()
	{
		// パラメータの設定
		m_acsel = m_moveAcsel;
		m_status.Set(eStatus::IsAir, eStatus::IsFirePossible, eStatus::IsCannonFire) = true;
		m_status.Set(eStatus::IsCannonStandby) = false;

		StartSE(L"AIRSHOCK_SE", 1.0f); // SEの再生

		// -180度を三角関数を使って移動量を設定する
		float rad = -XM_PI;
		m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;
	}

	// プレイヤーの回転処理
	void NextPlayer::RotatePlayer()
	{
		// 腕の回転
		float arm = atan2f(m_velocity.y, m_velocity.x) + XM_PIDIV2;
		m_arm.lock()->SetRotation(Vec3(0.0f, 0.0f, arm));
	}

	// 選択肢ステート
	void NextPlayer::SelectState()
	{
		const auto& stage = GetTypeStage<NextStage>(); // ステージの取得
		const auto& select = stage->GetSelect(); // ステージから選択肢の取得
		const bool& inputA = Input::GetReleaseA(); // Aボタン入力
	
		// Aボタン入力があったら
		if (inputA)
		{
			// 選択肢に応じて処理
			switch (select)
			{
			case NextStage::Next: // 次のステージになら
				OnRushA(); // Aボタン入力時の処理を実行
				stage->SetStageState(NextStage::StageMove); // ステージステートを変更
				break;

			case NextStage::Back: // タイトルに戻るなら
				DropSetup(); // 落下のセットアップを行う
				stage->SetStageState(NextStage::TitleBack); // ステージステートを変更
				break;

			default:
				break;
			}
		}
	}

	// 落下のセットアップ
	void NextPlayer::DropSetup()
	{
		// SEの再生
		StartSE(L"DAMAGE_SE", 1.0f); // SEの再生

		// 座標と移動量と加速度を設定
		m_velocity = m_deffVelo * 2.5f;
		m_acsel = m_dropAcsel;

		// コリジョンを非アクティブに
		m_ptrColl->SetUpdateActive(false);
	}

	// 落下処理
	void NextPlayer::TitleDrop()
	{
		// 中央の座標と現在の座標から長さを求める
		const auto& stage = GetTypeStage<NextStage>();
		const Vec3 defPos = Vec3(0.0f);
		float length = (defPos - GetPosition()).length();

		// Z軸回転させ続ける
		Vec3 rot = GetRotation();
		rot.z += DELTA_TIME * 5.0f;
		if (rot.z >= XM_PIDIV2) rot.z = -XM_PIDIV2;

		// ローテーションの更新
		SetRotation(rot);
		m_arm.lock()->SetRotation(rot + Vec3(0.0f, 0.0f, XM_PI));

		// 長さが画面外の距離に行ったら
		if (length >= 22.5f)
		{
			// ステートをバックフェードにし、SEを再生
			stage->SetStageState(NextStage::MetalFade);
			stage->CreateSE(L"METAL_SE", 0.75f);
		}
	}
	
	// 次のステージに移動したかのチェック(演出)
	void NextPlayer::NextStageCheck()
	{
		// 中央の座標と現在の座標から長さを求める
		const Vec3 defPos = Vec3(0.0f);
		float length = (defPos - GetPosition()).length();

		// 長さが画面外の距離に行ったら
		if (length >= 25.0f)
		{
			// ステートをフェードアウトにする
			SetStageState<NextStage>(NextStage::FadeOut);
		}
	}

	// 煙のエフェクト
	void NextPlayer::SmokeEffect(const Vec2& velo)
	{
		// 煙パーティクルの初期化
		const auto& particle = m_particle.lock()->InsertParticle(2);
		particle->SetEmitterPos(GetPosition());
		particle->SetTextureResource(L"SMOKE_TX");
		particle->SetMaxTime(3.0f);
		particle->SetDrawOption(Particle::Normal);

		// 生成したスプライトを配列で取得
		vector<ParticleSprite>& pSpriteVec = particle->GetParticleSpriteVec();

		// スプライトの数ループ
		for (auto& sprite : pSpriteVec)
		{
			// スプライトの初期化設定(移動量・スケール・ローテーション)
			sprite.m_Velocity = velo;
			sprite.m_LocalScale = Vec2(m_acsel > 2.0f ? m_acsel / 2.5f : 0.0f);
			sprite.m_LocalQt.rotationZ(DegToRad(RangeRand(360.0f, 0.0f)));
		}
	}
}