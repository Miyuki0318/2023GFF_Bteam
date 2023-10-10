/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Player::OnCreate()
	{
		// トランスフォームの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(Vec3(0.0f, 2.0f, 0.0f));
		ptrTrans->SetScale(Vec3(3.0f));

		// 描画の設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		// コリジョンの設定
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(true);

		// 照準用オブジェクトの生成
		for (size_t i = 0; i < 10; i++)
		{
			m_aligment.push_back(GetStage()->AddGameObject<DebugSphere>());
			m_aligment.at(i).lock()->SetScale(1.0f);
			m_aligment.at(i).lock()->SetDrawActive(false);
		}
	}

	void Player::OnUpdate()
	{
		// Aボタン入力有無での関数分岐
		Input::GetPushA() == true ? OnPushA() : Input::GetReleaseA() == true ? OnReleaseA() : 0;

		// 照準の回転処理
		AligmentRotate();

		// プレイヤーの移動関数
		MovePlayer();

		// デバッグ文字列
		Debug::Log(L"pos : ", m_position);
		Debug::Log(L"velo : ", m_velocity);
		Debug::Log(L"acsel : ", m_acsel);
		Debug::Log(L"isAir : ", m_isAir);
	}

	// Aボタン押した時
	void Player::OnPushA()
	{
		// 時間を0.05倍にする
		m_timeSpeed = 0.1f;

		// 照準を表示する
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(true);
		}
	}

	// Aボタンを離した時
	void Player::OnReleaseA()
	{
		// メンバ変数の設定
		m_isAir = true;
		m_acsel = m_maxAcsel;
		m_timeSpeed = 2.0f;

		// スティック入力を取得し移動ベクトルに保持
		const Vec2& stick = Input::GetLStickValue();
		if (stick.length() > 0.0f)
		{
			m_velocity = stick * 3.0f;
		}

		// 照準を非表示にする
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(false);
		}
	}

	// 地面に接地したら
	void Player::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		// 移動量が下方向にあり
		if (m_velocity.y > 0.0f)
		{
			// 加速度が半分より大きかったら
			if (m_acsel > 2.5f)
			{
				// 移動量を反転させ、半分にする
				m_velocity.y *= -0.5f;
			}
		}
	}

	// 地面に接地し続けたら
	void Player::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		// Y軸移動ベクトルを0.0にし、空中かの真偽をfalse
		m_velocity.y = 0.0f;
		m_isAir = false;
	}

	// 移動関数
	void Player::MovePlayer()
	{
		// トランスフォームの取得
		auto ptrTrans = GetComponent<Transform>();

		// 前フレームからのデルタタイムを取得
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

		// 現在の座標を取得
		m_position.x = ptrTrans->GetPosition().x;
		m_position.y = ptrTrans->GetPosition().y;

		// ポジションに移動ベクトルと速度と加速度とデルタタイムで掛けた数を加算
		m_position.x += -m_velocity.x * m_speed * m_acsel * deltaTime;
		m_position.y += -m_velocity.y * m_speed * m_acsel * deltaTime;

		// 空中なら
		if (m_isAir)
		{
			// 加速度が1.0より大きかったら加速度分の二倍をデルタタイムで掛けた数で減算、小さかったら1.0に修正
			m_acsel > 1.0f ? m_acsel -= deltaTime * (m_acsel * 2.0f) : m_acsel = 1.0f;

			// Y軸移動ベクトルを重力とデルタタイムで掛けた数で減算
			m_velocity.y -= m_gravity * deltaTime;
		}
		else
		{
			// 減少量をX軸移動ベクトルの正負で決める
			float decrease = deltaTime * m_maxAcsel;
			decrease *= m_velocity.x > 0.0f ? 1.0f : -1.0f;

			// X軸移動ベクトルが0.01より大きかったら(符号問わず)
			if (m_velocity.x > 0.1f || m_velocity.x < -0.1f)
			{
				// X軸移動ベクトルを減少量で減算
				m_velocity.x -= decrease;
			}
			else
			{
				// 0.01より小さかったら0.0で修正
				m_velocity.x = 0.0f;
			}
		}

		// 座標の更新
		ptrTrans->SetPosition(m_position);
	}

	void Player::AligmentRotate()
	{
		// 移動方向用
		Vec2 velo;

		// スティック入力を取得し移動ベクトルに保持
		const Vec2& stick = Input::GetLStickValue();
		if (stick.length() > 0.0f)
		{
			velo = stick / 10.0f;
		}

		// オブジェクトの数分ループ
		float loopCount = 1.0f;
		float deltaTime = App::GetApp()->GetElapsedTime();
		for (const auto& aligment : m_aligment)
		{
			Vec2 pos = Vec2(m_position.x, m_position.y);
			pos += -velo * m_speed * m_maxAcsel * loopCount;
			velo.y -= m_gravity * deltaTime / 22.5f;
			aligment.lock()->SetPosition(pos);
			loopCount++;
		}
	}
}