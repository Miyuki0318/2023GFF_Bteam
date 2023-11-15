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
		// 胴のトランスフォーム
		m_ptrTrans = GetComponent<Transform>();
		m_ptrTrans->SetPosition(m_position);
		m_ptrTrans->SetScale(m_scale);
		
		// 腕のトランスフォーム
		m_arm.lock()->SetPosition(m_position);
		m_arm.lock()->SetScale(m_scale);

		// 胴の描画の設定
		m_bodyDraw = AddComponent<PNTBoneModelDraw>();
		m_bodyDraw->SetMeshResource(L"ROBOT_BODY");
		m_bodyDraw->SetMeshToTransformMatrix(m_bodyMat);
		m_bodyDraw->AddAnimation(L"WALK", 0, 60, true);
		m_bodyDraw->AddAnimation(L"DAMAGE", 180, 30, false);
		m_bodyDraw->ChangeCurrentAnimation(L"WALK");

		// 腕の描画の設定
		m_armDraw = m_arm.lock()->AddComponent<PNTBoneModelDraw>();
		m_armDraw->SetMultiMeshResource(L"ROBOT_ARM");
		m_armDraw->SetMeshToTransformMatrix(m_armMat);
		m_armDraw->AddAnimation(L"WALK", 0, 60, true);
		m_armDraw->AddAnimation(L"FIRE", 120, 30, false);
		m_armDraw->ChangeCurrentAnimation(L"WALK");

		// コリジョンの設定
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(false);

		// 照準用オブジェクトの生成
		for (size_t i = 0; i < 10; i++)
		{
			m_aligment.push_back(GetStage()->AddGameObject<DebugSphere>());
			m_aligment.at(i).lock()->SetScale(0.15f);
			m_aligment.at(i).lock()->SetDrawLayer(-1);
			m_aligment.at(i).lock()->SetDrawActive(false);
			m_aligment.at(i).lock()->GetComponent<PNTStaticDraw>()->SetEmissive(COL_RED);
		}

		// エフェクトオブジェクトの生成
		m_effect = GetStage()->AddGameObject<Billboard>(L"EFFECT", Vec2(0.0f), Vec3(0.0f));
		m_effect.lock()->SetDrawLayer(1);
		m_shield = GetStage()->AddGameObject<DebugSphere>(m_position, Vec3(0.0f), Vec3(3.0f));
		m_shield.lock()->SetDrawLayer(2);
		m_shield.lock()->GetComponent<PNTStaticDraw>()->SetTextureResource(L"SHIELD");
		m_particle = GetStage()->AddGameObject<MultiParticle>();
	}

	void Player::OnUpdate()
	{
		// Aボタン入力有無での関数分岐
		if (m_acsel <= 1.7f && m_firePossible && Input::GetReleaseA())
		{
			OnReleaseA();
		}

		if (m_cannonStandby)
		{
			CannonStandby();
		}

		// 照準の回転処理
		RotateAligment();

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
		Debug::Log(L"shield : ", m_shieldCount);
		Debug::Log(m_isDeath != false ? L"死亡" : L"生存");
		Debug::Log(m_isAir != false ? L"空中" : L"接地");
		Debug::Log(m_firePossible != false ? L"発射可" : L"発射不可");
	}

	// Aボタンを離した時
	void Player::OnReleaseA()
	{
		// スティック入力を取得し移動ベクトルに保持
		Vec2 stick = Input::GetLStickValue();
		m_velocity = (stick.length() > 0.0f ? stick.round(1) : m_deffVelo) * 3.0f;
		if (!m_isAir && stick.y > 0.0f)
		{
			m_velocity.y = -stick.round(1).y * 3.0f;
		}
		
		// メンバ変数の設定
		m_isAir = true;
		m_acsel = m_maxAcsel;
		m_cannonFire = false;

		// 腕のアニメーションを変更
		m_armDraw->ChangeCurrentAnimation(L"FIRE");

		// SEの再生
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		audioPtr->Start(L"AIRSHOCK_SE", 0, 0.5f);
	}

	// コリジョンに衝突したら
	void Player::OnCollisionEnter(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block"))
		{
			BlockEnter(other, hitPoint);
		}
		if (other->FindTag(L"Spike"))
		{
			SpikeEnter(other, hitPoint);
		}
		if (other->FindTag(L"Bird"))
		{
			BirdEnter(other, hitPoint);
		}
		if (other->FindTag(L"Cannon"))
		{
			CannonEnter(other);
		}
		if (other->FindTag(L"Convayor"))
		{
			ConvayorEnter(other, hitPoint);
		}
		if (other->FindTag(L"Ring"))
		{
			const auto& ring = dynamic_pointer_cast<Ring>(other);
			ring->IsGetRing();
			AddShield();
		}
		if (other->FindTag(L"Death"))
		{
			m_ptrTrans->SetPosition(m_respawnPos);
			m_acsel = 1.0f;
			m_velocity = m_deffVelo;
			m_firePossible = true;
		}
	}

	// コリジョンに衝突し続けたら
	void Player::OnCollisionExcute(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block"))
		{
			BlockExcute(other, hitPoint);
		}
		if (other->FindTag(L"Spike"))
		{
			SpikeExcute(other, hitPoint);
		}
		if (other->FindTag(L"Cannon"))
		{
			if (!m_cannonStandby)
			{
				CannonEnter(other);
			}
		}
		if (other->FindTag(L"Convayor"))
		{
			ConvayorExcute(other, hitPoint);
		}
	}

	// コリジョンから離れたら
	void Player::OnCollisionExit(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block") || other->FindTag(L"Spike") || other->FindTag(L"Covayor"))
		{
			BlockExit(other);
		}
	}

	// 移動関数
	void Player::MovePlayer()
	{
		// 前フレームからのデルタタイムを取得
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

		// 現在の座標を取得
		m_position = m_ptrTrans->GetPosition();

		// ポジションに移動ベクトルと速度と加速度とデルタタイムで掛けた数を加算
		m_position.x += -m_velocity.x * m_speed * m_acsel * deltaTime;
		m_position.y += -m_velocity.y * m_speed * m_acsel * deltaTime;

		// 座標の更新
		m_ptrTrans->SetPosition(m_position);

		// 腕の座標の更新(腕と胴のモデルマトリクスのポジションy軸の差分をポジションから差し引く)
		Vec3 pos = m_position;
		auto& bone = m_bodyDraw->GetVecLocalBones();
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
		m_ptrTrans->SetRotation(Vec3(0.0f, body, 0.0f));
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
			aligment.lock()->SetDrawActive(Input::GetLStickValue().length() > 0.0f);
			loopCount++;
		}
	}

	// アニメーションの更新
	void Player::AnimationUpdate()
	{
		// 前フレームからのデルタタイムにゲームスピードを掛けた数を取得
		float deltaTime = App::GetApp()->GetElapsedTime() * m_timeSpeed;

		// アニメーションが終わってたら待機状態にする
		if (m_bodyDraw->IsTargetAnimeEnd())
		{
			m_bodyDraw->ChangeCurrentAnimation(L"WALK");
		}
		if (m_armDraw->IsTargetAnimeEnd())
		{
			m_armDraw->ChangeCurrentAnimation(L"WALK");
		}

		// アニメーションの更新
		m_bodyDraw->UpdateAnimation(deltaTime);
		m_armDraw->UpdateAnimation(deltaTime);
	}

	// エフェクトの更新
	void Player::EffectUpdate()
	{
		// 更新
		m_shield.lock()->SetPosition(m_position.x, m_position.y + 0.5f, -1.5f);
		m_shield.lock()->SetDrawActive(m_shieldCount > 0);
		m_shield.lock()->GetComponent<PNTStaticDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.5f + (0.1f * m_shieldCount)));

		m_effect.lock()->SetDrawActive(m_firePossible && !m_cannonFire);

		if (m_firePossible && !m_cannonFire)
		{
			// プレイヤーの座標に移動方向ベクトル×加速度を加算する
			Vec3 pos = m_position + (Vec3(m_velocity.x, m_velocity.y, 0.0f).normalize() * (m_acsel * 1.15f));
			pos.y += 0.5f;
			pos.z = -1.0f;

			m_effect.lock()->SetPosition(pos);
			m_effect.lock()->SetRotation(0.0f, 0.0f, (atan2f(m_velocity.y, m_velocity.x) - XM_PIDIV2));
			m_effect.lock()->SetScale(Vec2((m_acsel - 1.0f) * 3.0f));
		}
		if (m_cannonFire)
		{
			const auto particle = m_particle.lock()->InsertParticle(2);
			particle->SetEmitterPos(m_position);
			particle->SetTextureResource(L"SMOKE_TX");
			particle->SetMaxTime(3.0f);

			// 生成したスプライトを配列で取得
			vector<ParticleSprite>& pSpriteVec = particle->GetParticleSpriteVec();

			// スプライトの数分ループ
			for (auto& sprite : pSpriteVec)
			{
				sprite.m_Velocity = Vec3(m_velocity.x, m_velocity.y, 0.0f).normalize();
				sprite.m_LocalScale = Vec2(m_acsel > 2.0f ? m_acsel / 2.5f : 0.0f);
				sprite.m_LocalQt.rotationZ(Utility::DegToRad(Utility::RangeRand(360.0f, 0.0f)));
			}
		}
	}

	// 大砲発射待機時
	void Player::CannonStandby()
	{
		if (m_activeCannon.lock())
		{
			const auto& drawPtr = m_activeCannon.lock()->GetComponent<PNTBoneModelDraw>();
			if (drawPtr->GetCurrentAnimationTime() > 0.4f)
			{
				m_acsel = 10.0f;
				m_isAir = true;
				m_firePossible = true;
				m_cannonFire = true;
				m_cannonStandby = false;

				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
				audioPtr->Start(L"CANNON_SE", 0, 0.75f);

				m_activeCannon.reset();
			}
			else
			{
				m_position = m_activeCannon.lock()->GetPosition();
				m_ptrTrans->SetPosition(m_position);
			}
		}
	}

	// ブロックに衝突した瞬間
	void Player::BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// ブロックのパラメータを取得
		auto objTrans = block->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// コリジョンに対して上から衝突
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// 上にブロックがあるかのチェック
			if (!BlockUpperCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

			// エアショック使用可能にする
			m_firePossible = true;
			m_respawnPos = Vec3(objPos.x, objPos.y + (helf.y * 3.0f), 0.0f);

			// 死亡判定があるなら
			if (m_isDeath)
			{
				// コンティニュー画面へ遷移
			}

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
		if (CollHitUnder(hitPos, objPos, helf))
		{
			// 下にブロックがあるかのチェック
			if (!BlockUnderCheck(Vec3(objPos.x, objPos.y - (helf.y * 2.0f), 0.0f))) return;

			// 移動量が上方向なら
			if (m_velocity.y < 0.0f)
			{
				// 反転させ、落下させる
				m_velocity.y *= -1.0f;
			}
			return;
		}

		// 横から衝突
		if (CollHitLeft(hitPos, objPos, helf) || CollHitRight(hitPos, objPos, helf))
		{
			// 移動量を半減しつつ反転させる
			m_velocity.x *= -0.5f;
			return;
		}
	}

	// ブロックに衝突し続けたら
	void Player::BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// ブロックのパラメータを取得
		auto objTrans = block->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// コリジョンに対して上から衝突
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// Y軸移動ベクトルを0.0にし、空中かの真偽をfalse
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
			m_isAir = false;
		}
	}

	// ブロックとの衝突が無くなったら
	void Player::BlockExit(const shared_ptr<GameObject>& block)
	{
		// 接地フラグを解除
		m_isAir = true;
	}

	// 衝突したブロックの上にブロックがあるかの検証
	bool Player::BlockUpperCheck(const Vec3& upperPos)
	{
		const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Stage")->GetGroupVector();

		bool check = true;

		for (const auto& ptr : blockVec)
		{
			if (!ptr.lock()) continue;

			const auto& block = dynamic_pointer_cast<CubeObject>(ptr.lock());
			if (!block) continue;

			Vec3 pos = block->GetPosition();

			if (pos == upperPos)
			{
				check = false;
			}
		}

		return check;
	}

	// 衝突したブロックの下にブロックがあるかの検証
	bool Player::BlockUnderCheck(const Vec3& underPos)
	{
		const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Stage")->GetGroupVector();

		bool check = true;

		for (const auto& ptr : blockVec)
		{
			if (!ptr.lock()) continue;

			const auto& block = dynamic_pointer_cast<CubeObject>(ptr.lock());
			if (!block) continue;

			Vec3 pos = block->GetPosition();

			if (pos == underPos)
			{
				check = false;
			}
		}

		return check;
	}

	// スパイクと衝突した瞬間
	void Player::SpikeEnter(const shared_ptr<GameObject>& obj, const Vec3& hitPos)
	{
		// スパイクオブジェクトにキャスト
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// パラメータの取得
		Vec3 spikePos = spike->GetPosition();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// スパイクの方向に応じて処理
		const auto& angle = spike->GetAngle();
		switch (angle)
		{
		case Gimmick::Up:
			if (CollHitUpper(hitPos ,spikePos, helfScale) || CollHitLeft(hitPos, spikePos, helfScale) || CollHitRight(hitPos, spikePos, helfScale))
			{
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}
			if (CollHitUnder(hitPos, spikePos, helfScale))
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::Down:
			if (CollHitUnder(hitPos, spikePos, helfScale) || CollHitLeft(hitPos, spikePos, helfScale) || CollHitRight(hitPos, spikePos, helfScale))
			{
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}
			if (CollHitUpper(hitPos, spikePos, helfScale))
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::Left:
			if (CollHitLeft(hitPos, spikePos, helfScale) || CollHitUpper(hitPos, spikePos, helfScale) || CollHitUnder(hitPos, spikePos, helfScale))
			{
				DamageKnockBack(Vec2(1.5f, -0.5f));
				return;
			}
			if (CollHitRight(hitPos, spikePos, helfScale))
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::Right:
			if (CollHitRight(hitPos, spikePos, helfScale) || CollHitUpper(hitPos, spikePos, helfScale) || CollHitUnder(hitPos, spikePos, helfScale))
			{
				DamageKnockBack(Vec2(-1.5f, -0.5f));
				return;
			}
			if (CollHitLeft(hitPos, spikePos, helfScale))
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::All:
			if (CollHitUpper(hitPos, spikePos, helfScale))
			{
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}
			if (CollHitUnder(hitPos, spikePos, helfScale))
			{
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}
			if (CollHitLeft(hitPos, spikePos, helfScale))
			{
				DamageKnockBack(Vec2(1.5f, -0.5f));
				return;
			}
			if (CollHitRight(hitPos, spikePos, helfScale))
			{
				DamageKnockBack(Vec2(-1.5f, -0.5f));
				return;
			}
			break;

		default:
			break;
		}
	}

	// スパイクと衝突し続けた時
	void Player::SpikeExcute(const shared_ptr<GameObject>& obj, const Vec3& hitPos)
	{
		// スパイクオブジェクトにキャスト
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// パラメータの取得
		Vec3 spikePos = spike->GetPosition();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// スパイクの方向に応じて処理
		const auto& angle = spike->GetAngle();
		if (angle == Gimmick::Down)
		{
			m_firePossible = true;
			BlockExcute(obj, hitPos);
		}
	}

	// 鳥と衝突した時
	void Player::BirdEnter(const shared_ptr<GameObject>& enemy, const Vec3& hitPos)
	{
		// 鳥オブジェクトにキャスト
		const auto& bird = dynamic_pointer_cast<Bird>(enemy);

		// パラメータの取得
		Vec3 birdPos = bird->GetPosition();
		Vec3 helfScale = bird->GetScale() / 2.0f;

		// 上から衝突
		if (CollHitUpper(hitPos, birdPos, helfScale))
		{
			// 移動量を反転させ、半分にする
			m_velocity.x = -Input::GetLStickValue().round(1).x;
			m_velocity.y = -2.0f;
			m_acsel = m_damageAcsel;
			m_firePossible = true;
			return;
		}

		// 下から衝突
		if (CollHitUnder(hitPos, birdPos, helfScale))
		{
			DamageKnockBack(Vec2(0.9f, 1.0f));
			return;
		}

		// 左から衝突
		if (CollHitLeft(hitPos, birdPos, helfScale))
		{
			DamageKnockBack(Vec2(1.5f, -0.5f));
			return;
		}

		// 右から衝突
		if (CollHitRight(hitPos, birdPos, helfScale))
		{
			DamageKnockBack(Vec2(-1.5f, -0.5f));
			return;
		}
	}

	// 大砲と衝突した時
	void Player::CannonEnter(const shared_ptr<GameObject>& cannon)
	{
		const auto& ptr = dynamic_pointer_cast<Cannon>(cannon);
		ptr->OnFire();
		m_activeCannon = ptr;

		m_isAir = false;
		m_firePossible = false;
		m_cannonStandby = true;

		m_velocity = Vec2(0.0f);
		m_acsel = 1.0f;
	}

	// ベルトコンベアと衝突した時
	void Player::ConvayorEnter(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
	{
		// ブロックのパラメータを取得
		auto objTrans = convayor->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// コリジョンに対して上から衝突
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// 上にブロックがあるかのチェック
			if (!BlockUpperCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

			// エアショック使用可能にする
			m_firePossible = true;
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;

			// めり込みを修正
			Vec3 pos = m_ptrTrans->GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			m_ptrTrans->SetPosition(pos);

			const auto& ptr = dynamic_pointer_cast<Convayor>(convayor);
			if (!ptr) return;

			const auto& angle = ptr->GetRotate();
			switch (angle)
			{
			case Convayor::LeftRot:
				m_velocity.x = m_speed;
				break;

			case Convayor::RightRot:
				m_velocity.x = -m_speed;
				break;

			default:
				break;
			}

			return;
		}

		// 上以外から衝突したなら
		if (CollHitUnder(hitPos, objPos, helf) || CollHitLeft(hitPos, objPos, helf) || CollHitRight(hitPos, objPos, helf))
		{
			BlockEnter(convayor, hitPos);
		}
	}

	// ベルトコンベアと衝突し続けた時
	void Player::ConvayorExcute(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
	{
		// ブロックのパラメータを取得
		auto objTrans = convayor->GetComponent<Transform>();
		Vec3 objPos = objTrans->GetPosition();
		Vec3 helf = objTrans->GetScale() / 2.0f;

		// コリジョンに対して上から衝突
		if (CollHitUpper(hitPos, objPos, helf))
		{
			Vec3 pos = m_ptrTrans->GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			m_ptrTrans->SetPosition(pos);

			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
		}
	}

	// ダメージノックバック
	void Player::DamageKnockBack(const Vec2& velocity)
	{
		// 時間速度を通常速度で上書き
		m_timeSpeed = m_normalTime;
		m_firePossible = false;
		m_velocity = velocity;
		m_acsel = m_damageAcsel;

		// アニメーションをダメージ状態にする
		m_bodyDraw->ChangeCurrentAnimation(L"DAMAGE");

		// 軌道の表示をオフ
		for (const auto& aligment : m_aligment)
		{
			aligment.lock()->SetDrawActive(false);
		}

		if (m_shieldCount > 0)
		{
			m_shieldCount--;
		}
		else
		{
			m_isDeath = true;
		}
	}

	// 上から衝突したかの検証
	bool Player::CollHitUpper(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
	{
		return hitPos.y > hitObjPos.y && ((hitPos.y - hitObjPos.y) >= helfScale.y);
	}

	// 下から衝突したかの検証
	bool Player::CollHitUnder(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
	{
		return hitPos.y < hitObjPos.y && ((hitPos.y - hitObjPos.y) <= -helfScale.y);
	}

	// 左から衝突したかの検証
	bool Player::CollHitLeft(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
	{
		return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
			&& ((hitPos.x - hitObjPos.x) < helfScale.x);
	}

	// 右から衝突したかの検証
	bool Player::CollHitRight(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
	{
		return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
			&& ((hitPos.x - hitObjPos.x) > -helfScale.x);
	}
}