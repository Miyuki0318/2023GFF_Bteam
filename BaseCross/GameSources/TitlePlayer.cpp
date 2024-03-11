/*!
@file TitlePlayer.cpp
@brief タイトル画面用のプレイヤー
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;

	void TitlePlayer::OnUpdate()
	{
		// ステージステートの取得
		const auto& state = GetStageState<TitleStage>();

		// ステージステートがスタートムーブ以上なら
		if (state >= TitleStage::StartMove)
		{
			// 大砲待機時
			if (m_status(eStatus::IsCannonStandby))
			{
				CannonStandby(10.0f);
			}

			// モードセレクト時なら
			if (state == TitleStage::ModeSelect)
			{
				// Aボタン入力有無での関数分岐
				if (m_status(eStatus::IsFirePossible) && GetReleaseA())
				{
					OnRushA();
				}
			}

			// 無敵時間経過
			if (m_status(eStatus::IsInvincible))
			{
				InvincibleTimer();
			}

			// プレイヤーの移動関数
			MovePlayer();

			// 移動時の減少量
			MoveReduction();

			// プレイヤーの回転関数
			RotatePlayer();
			
			// 照準の回転処理
			UpdateArrow();

			// アニメーションの再生
			AnimationUpdate();

			// エフェクト描画関数
			EffectUpdate();
		}

		//// デバッグ文字列
		//Debug::Log(L"pos : ", m_position);
		//Debug::Log(L"velo : ", m_velocity);
		//Debug::Log(L"acsel : ", m_acsel);
		//Debug::Log(m_isAir != false ? L"空中" : L"接地");
		//Debug::Log(m_firePossible != false ? L"発射可" : L"発射不可");
	}

	// Aボタンを押した時
	void TitlePlayer::OnRushA()
	{
		Vec2 stick = GetLStickValue().round(1); // Lスティックの入力量を取得

		// Lスティックの入力があれば
		if (IsInputLStick())
		{
			// 移動量の設定
			stick.y = 0.0f;
			m_velocity = m_deffVelo * 3.0f;

			// X軸の入力量が正負を問わず0.5より大きかったら正負に応じて移動量を設定
			m_velocity.x = stick.length() > 0.5f ? stick.x > 0.0f ? 2.25f : -2.25f : 0.0f;

			// メンバ変数の設定
			m_status.Set(eStatus::IsAir) = true;
			m_status.Set(eStatus::IsCannonFire) = false;
			m_acsel = m_maxAcsel;
			m_meddleVelo.zero();

			// 腕のアニメーションを変更
			m_armDraw->ChangeCurrentAnimation(L"FIRE");

			// SEの再生
			StartSE(L"AIRSHOCK_SE", 0.5f);

			// ステージの分岐
			const auto& scene = App::GetApp()->GetScene<Scene>();
			if (m_velocity.x < 0.0f) scene->SetCurrentStage("ExtraNormal");
			if (m_velocity.x > 0.0f) scene->SetCurrentStage("Bornus");
			if (m_velocity.x == 0.0f) scene->SetCurrentStage("ExtraEasy");

			// ステージステートを大砲待機に設定
			SetStageState<TitleStage>(TitleStage::CannonStanby);
		}
	}

	// 軌道の回転描画
	void TitlePlayer::UpdateArrow()
	{
		// スティックの入力量から移動量を設定
		Vec2 stick = GetLStickValue().round(1);
		Vec2 velo = m_deffVelo / 10.0f;
		stick.y = 0.0f;
		velo.x = stick.length() > 0.5f ? stick.x > 0.0f ? 0.05f : -0.05f : 0.0f;

		// 節目と節目のカウンタ
		const int mileNum = 40;
		float mileCount = 1.0f;

		// 節目座標配列
		vector<Vec3> points;

		// 節目の数ループして節目の座標を設定
		for (size_t i = 0; i < mileNum; i++)
		{
			Vec2 pos = -velo * m_speed * m_maxAcsel * mileCount;
			velo.y -= m_gravity * 0.016f / 20.0f / 4.0f;
			points.push_back(Vec3(pos.x, pos.y, 0.0f));
			mileCount += 0.25f;
		}
		m_arrow.lock()->UpdateEffect(points);

		// スティックの入力がありモードセレクトステート中で、エアショック発射可能であれば描画する
		bool state = GetStageState<TitleStage>() == TitleStage::ModeSelect;
		m_arrow.lock()->SetDrawActive(IsInputLStick() && state && m_status(eStatus::IsFirePossible));
	}

	// 大砲待機時
	void TitlePlayer::CannonStandby(float acsel)
	{
		// 大砲のポインタがあるなら
		if (m_activeCannon.lock())
		{
			// 大砲のシェアドポインタ
			const auto& cannon = m_activeCannon.lock();

			// 発射時の時間の取得
			const float& fireTime = cannon->GetFireTime();
			const auto& drawPtr = cannon->GetComponent<PNTBoneModelDraw>();

			// 再生時間が発射時の時間を過ぎたら
			if (drawPtr->GetCurrentAnimationTime() > fireTime)
			{
				// メンバ変数の設定
				m_acsel = m_activeCannon.lock()->GetAngle() == GimmickAngle::eAngle::Up ? acsel * 2.0f : acsel;
				m_status.Set(eStatus::IsAir, eStatus::IsFirePossible, eStatus::IsCannonFire) = true;
				m_status.Set(eStatus::IsCannonStandby) = false;
				m_meddleVelo.zero();

				// 大砲のZ軸をラジアンに変換し、移動量を設定する
				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				// SEの再生
				StartSE(L"CANNON_SE", 0.75f);

				// ステージステートが大砲待機時なら
				const auto& titleStage = GetTypeStage<TitleStage>();
				if (titleStage->GetStageState() == TitleStage::CannonStanby)
				{
					// ステージステートをフェードアウトに変更
					titleStage->SetStageState(TitleStage::FadeOut);
				}
			}
			else
			{
				// 自身の座標を大砲の中に設定する
				SetPosition(cannon->GetPosition());
			}
		}
	}
}