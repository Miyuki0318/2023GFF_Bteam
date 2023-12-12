#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TitlePlayer::OnUpdate()
	{
		// ステージステートの取得
		const auto& state = GetStageState<TitleStage>();

		// ステージステートがスタートムーブ以上なら
		if (state >= TitleStage::StartMove)
		{
			// 大砲待機時
			if (m_cannonStandby)
			{
				CannonStandby(10.0f);
			}

			// モードセレクト時なら
			if (state == TitleStage::ModeSelect)
			{
				// Aボタン入力有無での関数分岐
				if (m_firePossible && Input::GetReleaseA())
				{
					OnRushA();
				}
			}

			// プレイヤーの移動関数
			MovePlayer();

			// 移動時の減少量
			MoveReduction();

			// プレイヤーの回転関数
			RotatePlayer();
			
			// 照準の回転処理
			RotateAligment();

			// アニメーションの再生
			AnimationUpdate();

			// エフェクト描画関数
			EffectUpdate();
		}

		// デバッグ文字列
		Debug::Log(L"pos : ", m_position);
		Debug::Log(L"velo : ", m_velocity);
		Debug::Log(L"acsel : ", m_acsel);
		Debug::Log(m_isAir != false ? L"空中" : L"接地");
		Debug::Log(m_firePossible != false ? L"発射可" : L"発射不可");
	}

	void TitlePlayer::OnRushA()
	{
		Vec2 stick = Input::GetLStickValue().round(1);

		if (Input::GetLStickValue().length() > 0.0f)
		{
			// 移動量の設定
			stick.y = 0.0f;
			m_velocity = m_deffVelo * 3.0f;
			m_velocity.x = stick.length() > 0.5f ? stick.x > 0.0f ? 2.25f : -2.25f : 0.0f;

			// メンバ変数の設定
			m_isAir = true;
			m_acsel = m_maxAcsel;
			m_cannonFire = false;
			m_meddleVelo.zero();

			// 腕のアニメーションを変更
			m_armDraw->ChangeCurrentAnimation(L"FIRE");

			// SEの再生
			StartSE(L"AIRSHOCK_SE", 0.5f);

			// ステージの分岐
			const auto& scene = App::GetApp()->GetScene<Scene>();
			if (m_velocity.x < 0.0f) scene->SetCurrentStage("Normal");
			if (m_velocity.x == 0.0f) scene->SetCurrentStage("Easy");

			// ステージステートを大砲待機に設定
			SetStageState<TitleStage>(TitleStage::CannonStanby);
		}
	}

	void TitlePlayer::RotateAligment()
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

		bool input = Input::GetLStickValue().length() > 0.0f;
		bool state = GetStageState<TitleStage>() == TitleStage::ModeSelect;
		m_aligment.lock()->SetDrawActive(input && state && m_firePossible);
	}

	void TitlePlayer::CannonStandby(float acsel)
	{
		if (m_activeCannon.lock())
		{
			// 大砲のシェアドポインタ
			const auto& cannon = m_activeCannon.lock();

			// 発射時の時間の取得
			const float& fireTime = cannon->GetFireTime();

			// 描画コンポーネントからアニメーションの再生時間を取得
			const auto& drawPtr = cannon->GetComponent<PNTBoneModelDraw>();
			if (drawPtr->GetCurrentAnimationTime() > fireTime)
			{
				m_acsel = m_activeCannon.lock()->GetAngle() == Gimmick::Up ? acsel * 2.0f : acsel;
				m_isAir = true;
				m_firePossible = true;
				m_cannonFire = true;
				m_cannonStandby = false;
				m_meddleVelo.zero();

				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				StartSE(L"CANNON_SE", 0.75f);

				const auto& titleStage = GetTypeStage<TitleStage>();
				if (titleStage->GetStageState() == TitleStage::CannonStanby)
				{
					titleStage->SetStageState(TitleStage::FadeOut);
				}
			}
			else
			{
				m_position = m_activeCannon.lock()->GetPosition();
				SetPosition(m_position);
			}
		}
	}
}