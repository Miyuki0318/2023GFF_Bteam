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
		ptrTrans->SetScale(Vec3(2.0f));

		// 描画の設定
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		// コリジョンの設定
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(true);
	}

	void Player::OnUpdate()
	{
		// Aボタン入力があったら
		if (Input::GetPushA())
		{
			// メンバ変数の設定
			m_isAir = true;
			m_acsel = 5.0f;

			// スティック入力を取得し移動ベクトルに保持
			const Vec2& stick = Input::GetLStickValue();
			if (stick.length() > 0.0f)
			{
				m_velocity = stick * 3.0f;
			}
		}

		// トランスフォームの取得
		auto ptrTrans = GetComponent<Transform>();

		// 前フレームからのデルタタイムを取得
		float deltaTime = App::GetApp()->GetElapsedTime();

		// 現在の座標を取得
		Vec2 pos;
		pos.x = ptrTrans->GetPosition().x;
		pos.y = ptrTrans->GetPosition().y;

		// ポジションに移動ベクトルと速度と加速度とデルタタイムで掛けた数を加算
		pos += -m_velocity * m_speed * m_acsel * deltaTime;

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
			float decrease = deltaTime * 5.0f;
			decrease *= m_velocity.x > 0.0f ? 1.0f : -1.0f;

			// X軸移動ベクトルが0.01より大きかったら(符号問わず)
			if (m_velocity.x > 0.01f || m_velocity.x < -0.01f)
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
		ptrTrans->SetPosition(Vec3(pos.x, pos.y, 0.0f));

		// デバッグ文字列
		Debug::Log(L"pos : ", pos);
		Debug::Log(L"velo : ", m_velocity);
		Debug::Log(L"acsel : ", m_acsel);
		Debug::Log(L"isAir : ", m_isAir);
	}

	// 地面に接地したら
	void Player::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		// Y軸移動ベクトルを0.0にし、空中かの真偽をfalse
		m_velocity.y = 0.0f;
		m_isAir = false;
	}
}