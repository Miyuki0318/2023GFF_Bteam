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
		// 腕モデル用オブジェクトの生成
		m_arm = GetStage()->AddGameObject<DebugObject>();

		// トランスフォームの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(Vec3(0.0f, 2.0f, 0.0f));
		ptrTrans->SetScale(Vec3(1.0f));
		
		m_arm.lock()->SetPosition(Vec3(0.0f, 2.0f, 0.0f));
		m_arm.lock()->SetScale(Vec3(1.0f));

		// 描画の設定
		auto ptrDraw = AddComponent<PNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"ROBOT_BODY");
		ptrDraw->SetMeshToTransformMatrix(m_bodyMat);
		ptrDraw->AddAnimation(L"WALK", 0, 60, true);
		ptrDraw->AddAnimation(L"DAMAGE", 180, 30, false);
		ptrDraw->ChangeCurrentAnimation(L"WALK");

		auto armDraw = m_arm.lock()->AddComponent<PNTBoneModelDraw>();
		armDraw->SetMultiMeshResource(L"ROBOT_ARM");
		armDraw->SetMeshToTransformMatrix(m_armMat);
		armDraw->AddAnimation(L"WALK", 0, 60, true);
		armDraw->AddAnimation(L"FIRE", 120, 30, false);
		armDraw->ChangeCurrentAnimation(L"WALK");

		// コリジョンの設定
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(false);

		// 照準用オブジェクトの生成
		for (size_t i = 0; i < 10; i++)
		{
			m_aligment.push_back(GetStage()->AddGameObject<DebugSphere>());
			m_aligment.at(i).lock()->SetScale(0.15f);
			m_aligment.at(i).lock()->SetDrawActive(false);
		}

		// エフェクトオブジェクトの生成
		m_effect = GetStage()->AddGameObject<Billboard>(L"EFFECT", Vec2(0.0f), Vec3(0.0f));
		m_effect.lock()->SetDrawLayer(1);
	}

	void Player::OnUpdate()
	{

		// Aボタン入力有無での関数分岐
		if (m_acsel <= 1.7f && m_firePossible)
		{
			Input::GetPushA() == true ? OnPushA() : Input::GetReleaseA() == true ? OnReleaseA() : 0;
		}

		// 照準の回転処理
		if (m_timeSpeed == 0.1f)
		{
			RotateAligment();
		}

		// プレイヤーの移動関数
		MovePlayer();

		// 移動時の減少量
		MoveReduction();

		// プレイヤーの回転関数
		RotatePlayer();

		// アニメーションの再生
		AnimationUpdate();

		// エフェクト描画関数
		EffectUpdate();

		// デバッグ文字列
		Debug::Log(L"pos : ", m_position);
		Debug::Log(L"velo : ", m_velocity);
		Debug::Log(L"acsel : ", m_acsel);
		Debug::Log(L"isAir : ", m_isAir);
	}

	// Aボタン押した時
	void Player::OnPushA()
	{
		// 時間を遅くする
		m_timeSpeed = m_slowTime;

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
		m_timeSpeed = m_normalTime;

		// スティック入力を取得し移動ベクトルに保持
		Vec2 stick = Input::GetLStickValue();
		m_velocity = (stick.length() > 0.0f ? stick.round(1) : m_deffVelo) * 3.0f;
		
		// 腕のアニメーションを変更
		auto arm = m_arm.lock()->GetComponent<PNTBoneModelDraw>();
		arm->ChangeCurrentAnimation(L"FIRE");

		// 照準を非表示にする
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(false);
		}
	}

	// コリジョンに衝突したら
	void Player::OnCollisionEnter(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Block"))
		{
			BlockEnter(other);
		}

		if (other->FindTag(L"Spike"))
		{
			SpikeEnter(other);
		}
	}

	// コリジョンに衝突し続けたら
	void Player::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Block"))
		{
			BlockExcute(other);
		}
	}

	// コリジョンから離れたら
	void Player::OnCollisionExit(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"Block"))
		{
			BlockExit(other);
		}
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

		// 座標の更新
		ptrTrans->SetPosition(m_position);

		// 腕の座標の更新(腕と胴のモデルマトリクスのポジションy軸の差分をポジションから差し引く)
		Vec3 pos = m_position;
		auto body = GetComponent<PNTBoneModelDraw>();
		auto& bone = body->GetVecLocalBones();
		pos.y -= (m_armMat.getMajor3().y - m_bodyMat.getMajor3().y) - bone.at(1).getTranslation().y;
		m_arm.lock()->SetPosition(pos);
	}

	// 移動減少量
	void Player::MoveReduction()
	{
		// デルタタイムを取得
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

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
	}

	// プレイヤーの回転
	void Player::RotatePlayer()
	{
		// 胴と腕のrad
		float body = 0.0f, arm = 0.0f;

		// スティック入力
		const Vec2& stick = Input::GetLStickValue().round(1);

		// 空中かどうかで分岐
		if (m_isAir)
		{
			Vec2 velo = m_timeSpeed == m_normalTime ? m_velocity : stick.length() > 0.0f ? stick : m_deffVelo;
			body = (velo.x > 0.0f) * XM_PI;
			arm = atan2f(velo.y, velo.x) + XM_PIDIV2;
		}
		else
		{
			Vec2 velo = stick.length() > 0.0f ? stick : m_velocity != Vec2(0.0f) ? m_velocity : m_deffVelo;
			arm = atan2f(velo.y, velo.x) + XM_PIDIV2;
		}

		// ローテーションの更新
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetRotation(Vec3(0.0f, body, 0.0f));
		m_arm.lock()->SetRotation(Vec3(0.0f, 0.0f, arm));
	}

	// 軌道の回転描画
	void Player::RotateAligment()
	{
		// スティック入力を取得し移動ベクトルに保持
		Vec2 stick = Input::GetLStickValue().round(1);
		Vec2 velo = (stick.length() > 0.0f ? stick : m_deffVelo) / 10.0f;

		// オブジェクトの数分ループ
		float loopCount = 2.0f;
		float deltaTime = App::GetApp()->GetElapsedTime();
		for (const auto& aligment : m_aligment)
		{
			Vec2 pos = Vec2(m_position.x, m_position.y);
			pos += -velo * m_speed * m_maxAcsel * loopCount;
			velo.y -= m_gravity * 0.016f / 20.0f;
			aligment.lock()->SetPosition(pos);
			loopCount++;
		}
	}

	// アニメーションの更新
	void Player::AnimationUpdate()
	{
		// 胴と腕の描画コンポーネントの取得
		auto body = GetComponent<PNTBoneModelDraw>();
		auto arm = m_arm.lock()->GetComponent<PNTBoneModelDraw>();

		// 前フレームからのデルタタイムにゲームスピードを掛けた数を取得
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

		// アニメーションが終わってたら待機状態にする
		if (body->IsTargetAnimeEnd())
		{
			body->ChangeCurrentAnimation(L"WALK");
		}
		if (arm->IsTargetAnimeEnd())
		{
			arm->ChangeCurrentAnimation(L"WALK");
		}

		// アニメーションの更新
		body->UpdateAnimation(deltaTime);
		arm->UpdateAnimation(deltaTime);
	}

	// エフェクトの更新
	void Player::EffectUpdate()
	{
		// プレイヤーの座標に移動方向ベクトル×加速度を加算する
		Vec3 pos = m_position + (Vec3(m_velocity.x, m_velocity.y, 0.0f).normalize() * (m_acsel * 1.15f));
		pos.y += 0.5f;
		pos.z = -1.0f;

		// 更新
		m_effect.lock()->SetDrawActive(m_firePossible);
		m_effect.lock()->SetPosition(pos);
		m_effect.lock()->SetRotation(0.0f, 0.0f, (atan2f(m_velocity.y, m_velocity.x) - XM_PIDIV2));
		m_effect.lock()->SetScale(Vec2((m_acsel - 1.0f) * 3.0f));
	}

	// ブロックに衝突した瞬間
	void Player::BlockEnter(shared_ptr<GameObject>& block)
	{
		// 自身とブロックのトランスフォームの取得
		auto ptrTrans = GetComponent<Transform>();
		auto objTrans = block->GetComponent<Transform>();

		// 座標の取得
		Vec3 pos = ptrTrans->GetPosition();
		Vec3 objPos = objTrans->GetPosition();

		// ブロックのスケールの半分
		float helf = objTrans->GetScale().x / 2.0f;

		// コリジョンに対して上から衝突
		if (pos.y > objPos.y && ((pos.y - objPos.y) >= helf))
		{
			// エアショック使用可能にする
			m_firePossible = true;

			// 移動量が下方向にあり
			if (m_velocity.y > 0.0f)
			{
				// 加速度が半分より大きかったら
				if (m_acsel > 2.5f || m_velocity.y > 5.0f)
				{
					// 移動量を反転させ、半分にする
					m_velocity.y *= -0.5f;
				}
			}
			return;
		}

		// 下から衝突
		if (pos.y < objPos.y && ((pos.y - objPos.y) <= -helf))
		{
			if (m_velocity.y < 0.0f)
			{
				m_velocity.y *= -1.0f;
			}
			return;
		}

		// 横から衝突
		if (((pos.y - objPos.y) < helf) && ((pos.y - objPos.y) > -helf))
		{
			if (((pos.x - objPos.x) < helf) || ((pos.x - objPos.x) > -helf))
			{
				m_velocity.x *= -0.5f;
			}
			return;
		}
	}

	// ブロックに衝突し続けたら
	void Player::BlockExcute(shared_ptr<GameObject>& block)
	{
		// 自身とブロックのトランスフォームの取得
		auto ptrTrans = GetComponent<Transform>();
		auto objTrans = block->GetComponent<Transform>();

		// 座標の取得
		Vec3 pos = ptrTrans->GetPosition();
		Vec3 objPos = objTrans->GetPosition();

		// ブロックのスケールの半分
		float helf = objTrans->GetScale().x / 2.0f;

		// コリジョンに対して上から衝突
		if (pos.y > objPos.y && ((pos.y - objPos.y) >= helf))
		{
			// Y軸移動ベクトルを0.0にし、空中かの真偽をfalse
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
			m_isAir = false;
		}
	}

	// ブロックとの衝突が無くなったら
	void Player::BlockExit(shared_ptr<GameObject>& block)
	{
		// 接地フラグを解除
		m_isAir = true;
	}

	// スパイクとの衝突した瞬間
	void Player::SpikeEnter(shared_ptr<GameObject>& obj)
	{
		// 時間速度を通常速度で上書き
		m_timeSpeed = m_normalTime;

		// 軌道の表示をオフ
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(false);
		}

		// スパイクオブジェクトにキャスト
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// スパイクの方向に応じて処理
		const auto& angle = spike->GetAngle();
		switch (angle)
		{
		case Gimmick::Up:
			m_firePossible = false;
			m_velocity = Vec2(0.9f, -1.0f);
			m_acsel = 3.0f;
			break;

		case Gimmick::Down:
			m_firePossible = false;
			m_velocity = Vec2(0.9f, 1.0f);
			m_acsel = 3.0f;
			break;

		case Gimmick::Left:
			m_firePossible = false;
			m_velocity = Vec2(1.5f, -0.5f);
			m_acsel = 3.0f;
			break;

		case Gimmick::Right:
			m_firePossible = false;
			m_velocity = Vec2(-1.5f, -0.5f);
			m_acsel = 3.0f;
			break;

		default:
			break;
		}
	}
}