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
		SetPosition(m_position);
		SetScale(m_scale);
		
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

		// エフェクトオブジェクトの生成
		m_jetEffect = GetStage()->AddGameObject<AirJetEffect>(GetThis<DebugObject>());
		m_shieldEffect = GetStage()->AddGameObject<ShieldEffect>(GetThis<DebugObject>());
		m_aligment = GetStage()->AddGameObject<ArrowEffect>(GetThis<DebugObject>());
		m_particle = GetStage()->AddGameObject<MultiParticle>();
	}


	void Player::OnUpdate()
	{
		// ステージとステージステートの取得
		const auto& stage = GetTypeStage<GameStage>();
		const auto& state = stage->GetStageState();

		// 開始時の動きからゲーム中の範囲なら
		if (Utility::GetBetween(state, GameStage::StartMove, GameStage::DeathDrop))
		{
			// Aボタン入力有無での関数分岐
			if (state == GameStage::GameNow)
			{
				if (m_acsel <= 1.7f && m_firePossible && m_jumpCount < m_jumpLimit)
				{
					if (Input::GetReleaseA()) OnReleaseA();
				}

				// 照準の回転処理
				RotateAligment();

				// プレイヤーの回転関数
				RotatePlayer();
			}

			// 大砲待機時
			if (m_cannonStandby)
			{
				CannonStandby(10.0f);
			}

			// 無敵時間経過
			if (m_isInvincible)
			{
				InvincibleTimer();
			}

			if (m_jumpCount > 0)
			{
				RecoveryAirShock();
			}

			// 送風機のチェック
			BlowerBetweenCheck();

			// プレイヤーの移動関数
			MovePlayer();

			// 移動時の減少量
			MoveReduction();

			// アニメーションの再生
			AnimationUpdate();

			// エフェクト描画関数
			EffectUpdate();

			if (state == GameStage::DeathDrop)
			{
				DeathDrop();
			}
		}


		// デバッグ文字列
		Debug::Log(L"座標 : ", m_position);
		Debug::Log(L"移動量 : ", m_velocity);
		Debug::Log(L"加算移動量 : ", m_meddleVelo);
		Debug::Log(L"加速度 : ", m_acsel);
		Debug::Log(L"ジャンプ回数 : ", m_jumpCount);
		Debug::Log(L"シールドの数 : ", m_shieldCount);
		Debug::Log(L"無敵時間 : ", m_invincibleTime - m_damageTime);
		Debug::Log(L"スモールリング : ", m_sRingCount);
		Debug::Log(m_isAir != false ? L"空中" : L"接地");
		Debug::Log(m_firePossible != false ? L"発射可" : L"発射不可");
	}

	// Aボタンを離した時
	void Player::OnReleaseA()
	{
		// スティック入力を取得し移動ベクトルに保持
		Vec2 stick = Input::GetLStickValue();
		m_velocity = (stick.length() > 0.0f ? stick.round(1) : m_deffVelo) * m_veloSpeed;
		if (!m_isAir && stick.y > 0.0f)
		{
			m_velocity.y = -stick.round(1).y * m_veloSpeed;
		}
		
		// メンバ変数の設定
		m_isAir = true;
		m_acsel = m_maxAcsel;
		m_cannonFire = false;
		m_meddleVelo.zero();
		m_jumpCount++;
		m_jumpRecoveryTime = 0.0f;

		// 腕のアニメーションを変更
		m_armDraw->ChangeCurrentAnimation(L"FIRE");

		// SEの再生
		StartSE(L"AIRSHOCK_SE", 0.5f);
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
		if (other->FindTag(L"Cannon"))
		{
			CannonEnter(other);
		}
		if (other->FindTag(L"Goal"))
		{
			const auto& stage = GetTypeStage<GameStage>();
			const auto& camera = stage->GetGameCamera();
			if (camera)
			{
				CannonEnter(other);
				camera->RemoveTarget();
				stage->SetStageState(GameStage::Goal);
			}
		}
		if (other->FindTag(L"Convayor"))
		{
			ConvayorEnter(other, hitPoint);
		}
		if (other->FindTag(L"Ring"))
		{
			RingCheck(other);
		}
		if (other->FindTag(L"Rabbit"))
		{
			RabbitEnter(other);
		}
		if (other->FindTag(L"Death"))
		{
			SetPosition(m_respawnPos);
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


	void Player::OnCollisionExit(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;
		if (other->FindTag(L"Block"))
		{
			const auto& cube = dynamic_pointer_cast<CubeObject>(other);
			if (cube)
			{
				const auto& type = cube->GetAngleType();
				if (type == CubeObject::Normal)
				{
					m_isAir = true;
				}
			}
		}
	}

	// 移動関数
	void Player::MovePlayer()
	{
		// 前フレームからのデルタタイムを取得
		float deltaTime = DELTA_TIME * m_timeSpeed;

		// 現在の座標を取得
		m_position = GetPosition();

		// ポジションに移動ベクトルと速度と加速度とデルタタイムで掛けた数を加算
		m_position.x += -(m_velocity.x + m_meddleVelo.x) * m_speed * m_acsel * deltaTime;
		m_position.y += -(m_velocity.y + m_meddleVelo.y) * m_speed * m_acsel * deltaTime;

		// 座標の更新
		SetPosition(m_position);

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
		float deltaTime = DELTA_TIME * m_timeSpeed;

		// 空中なら
		if (m_isAir)
		{
			// 加速度が1.0より大きかったら加速度分の二倍をデルタタイムで掛けた数で減算、小さかったら1.0に修正
			m_acsel > 1.0f ? m_acsel -= deltaTime * (m_acsel * 2.0f) : m_acsel = 1.0f;

			// Y軸移動ベクトルを重力とデルタタイムで掛けた数で減算
			Vec2 tempVelo = m_meddleVelo;
			tempVelo.x = 0.0f;
			if (!m_isBlower && !(tempVelo.length() > 0.0f))
			{
				m_velocity.y -= m_gravity * deltaTime;
			}
		}
		else
		{
			// 減少量をX軸移動ベクトルの正負で決める
			float decrease = deltaTime * m_maxAcsel;
			decrease *= m_velocity.x > 0.0f ? 1.0f : -1.0f;

			// X軸移動ベクトルが0.01より大きかったら(符号問わず)
			if (m_velocity.x > 0.1f || m_velocity.x < -0.1f)
			{
				m_velocity.x -= decrease;
			}
			else
			{
				m_velocity.x = 0.0f;
			}

			if (m_velocity.y > 0.25f)
			{
				m_velocity.y -= DELTA_TIME;
			}
			else
			{
				m_velocity.y = 0.25f;
			}
		}
	}

	void Player::RecoveryAirShock()
	{
		if (m_jumpCount == 0) return;

		if (m_jumpRecoveryTime >= m_jumpRecoveryLimit)
		{
			m_jumpCount--;
			m_jumpRecoveryTime = 0.0f;
		}

		m_jumpRecoveryTime += DELTA_TIME;
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
			arm = stick.length() > 0.0f ? atan2f(velo.y, velo.x) + XM_PIDIV2 : atan2f(m_deffVelo.x, -m_deffVelo.y);
		} 

		// ローテーションの更新
		SetRotation(Vec3(0.0f, body, 0.0f));
		m_arm.lock()->SetRotation(Vec3(0.0f, 0.0f, arm));
	}

	// 軌道の回転描画
	void Player::RotateAligment()
	{
		// スティック入力を取得し移動ベクトルに保持
		Vec2 stick = Input::GetLStickValue().round(1);
		Vec2 velo = (stick.length() > 0.0f ? stick : m_deffVelo) / 10.0f;

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
		m_aligment.lock()->SetDrawActive(stick.length() > 0.0f && m_firePossible);
	}

	// アニメーションの更新
	void Player::AnimationUpdate()
	{
		// 前フレームからのデルタタイムにゲームスピードを掛けた数を取得
		float deltaTime = DELTA_TIME * m_timeSpeed;

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
		m_shieldEffect.lock()->UpdateEffect();
		m_shieldEffect.lock()->SetDrawShield(m_shieldCount > 0 && !m_cannonStandby);
		m_jetEffect.lock()->SetDrawActive(m_firePossible && !m_cannonFire);

		if (m_firePossible && !m_cannonFire)
		{
			m_jetEffect.lock()->UpdateEffect();
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
	void Player::CannonStandby(float acsel)
	{
		if (m_activeCannon.lock())
		{
			const float& fireTime = m_activeCannon.lock()->GetFireTime();
			const auto& drawPtr = m_activeCannon.lock()->GetComponent<PNTBoneModelDraw>();
			if (drawPtr->GetCurrentAnimationTime() > fireTime)
			{
				m_acsel = acsel;
				m_isAir = true;
				m_firePossible = true;
				m_cannonFire = true;
				m_cannonStandby = false;
				m_meddleVelo.zero();

				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				StartSE(L"CANNON_SE", 0.75f);

				m_activeCannon.reset();
			}
			else
			{
				m_position = m_activeCannon.lock()->GetPosition();
				SetPosition(m_position);
			}
		}
	}

	// 無敵時間経過
	void Player::InvincibleTimer()
	{
		m_damageTime += DELTA_TIME;

		if (m_invincibleTime <= m_damageTime)
		{
			m_damageTime = 0.0f;
			m_isInvincible = false;
		}
	}

	void Player::DeathSetup()
	{
		const auto& stage = GetTypeStage<GameStage>();
		if (stage->GetStageState() == GameStage::GameNow)
		{
			StartSE(L"DAMAGE_SE", 0.75f);

			stage->SetStageState(GameStage::DeathDrop);
			stage->GetGameCamera()->RemoveTarget();

			Vec3 pos = GetPosition();
			pos.z = -1.5f;
			SetPosition(pos);
			m_velocity = m_deffVelo * 2.5f;
			m_acsel = 2.5f;

			auto ptrColl = GetComponent<CollisionSphere>();
			ptrColl->SetUpdateActive(false);
		}
	}

	void Player::DeathDrop()
	{
		const auto& stage = GetTypeStage<GameStage>();
		const Vec3& deathPos = stage->GetGameCamera()->GetCurrentPos();
		float length = (deathPos - GetPosition()).length();
		Vec3 rad = GetRotation();
		rad.z += DELTA_TIME * 5.0f;
		if (rad.z >= XM_PIDIV2) rad.z = -XM_PIDIV2;

		SetRotation(rad);
		m_arm.lock()->SetRotation(rad + Vec3(0.0f, 0.0f, XM_PI));

		if (length >= 22.5f) 
		{
			stage->SetStageState(GameStage::Death);
			stage->CreateSE(L"METAL_SE", 0.75f);
		}
	}

	// 送風機のチェック
	void Player::BlowerBetweenCheck()
	{
		const auto& groupVec = GetStage()->GetSharedObjectGroup(L"Update")->GetGroupVector();

		Gimmick::eAngle angle = Gimmick::Up;
		bool check = false;

		for (const auto& ptr : groupVec)
		{
			const auto& blower = dynamic_pointer_cast<Blower>(ptr.lock());
			if (blower)
			{
				check = blower->CheckBetween(GetPosition());
				if (check)
				{
					angle = blower->GetAngle();
					break;
				}
			}
		}

		if (check)
		{
			switch (angle)
			{
			case Gimmick::Up:
				UpMeddleVelocity();
				break;

			case Gimmick::Down:
				DownMeddleVelocity();
				break;

			case Gimmick::Left:
				LeftMeddleVelocity();
				break;

			case Gimmick::Right:
				RightMeddleVelocity();
				break;

			default:
				break;
			}
		}	
		else
		{
			m_isBlower = false;
			ReductionMeddleVelocity();
		}
	}


	void Player::UpMeddleVelocity()
	{
		m_isBlower = true;

		float decrease = DELTA_TIME;
		decrease *= m_velocity.y > 0.0f ? 1.0f : -1.0f;

		if (m_velocity.y > decrease || m_velocity.y < decrease)
		{
			m_velocity.y -= decrease * m_maxAcsel;
		}
		else
		{
			m_velocity.y = decrease;
		}

		if (m_meddleVelo.y > -m_speed)
		{
			m_meddleVelo.y -= DELTA_TIME * m_maxAcsel;
		}
	}


	void Player::DownMeddleVelocity()
	{
		m_velocity.y += DELTA_TIME * m_maxAcsel;
	}


	void Player::LeftMeddleVelocity()
	{
		m_meddleVelo.x += DELTA_TIME * m_maxAcsel * m_speed;
	}


	void Player::RightMeddleVelocity()
	{
		m_meddleVelo.x -= DELTA_TIME * m_maxAcsel * m_speed;
	}


	void Player::ReductionMeddleVelocity()
	{
		float decrease = DELTA_TIME;
		decrease *= m_meddleVelo.x > 0.0f ? 1.0f : -1.0f;

		if (m_meddleVelo.x > 0.1f || m_meddleVelo.x < -0.1f)
		{
			m_meddleVelo.x -= decrease;
		}
		else
		{
			m_meddleVelo.x = 0.0f;
		}

		decrease = DELTA_TIME;
		decrease *= m_meddleVelo.y > 0.0f ? 1.0f : -1.0f;

		if (m_meddleVelo.y > 0.1f || m_meddleVelo.y < -0.1f)
		{
			m_meddleVelo.y -= decrease * 5.0f;
		}
		else
		{
			m_meddleVelo.y = 0.0f;
		}
	}

	// ブロックに衝突した瞬間
	void Player::BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// ステージとステージステートの取得
		const auto& stage = GetTypeStage<GameStage>(false);
		if (stage)
		{
			const auto& state = stage->GetStageState();

			if (state == GameStage::StartMove)
			{
				stage->SetStageState(GameStage::GameNow);
				stage->GetGameCamera()->SetTargetObject(GetThis<Player>());
			}
		}

		// ブロックのパラメータを取得
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		bool upper, under, left, right;
		upper = CollHitUpper(hitPos, objPos, helf);
		under = CollHitUnder(hitPos, objPos, helf);
		left = CollHitLeft(hitPos, objPos, helf);
		right = CollHitRight(hitPos, objPos, helf);

		const float& deg = cube->GetDegreeAngle().z;
		if (deg != 0.0f)
		{
			if (Utility::GetBetween<CubeObject::eType>(cube->GetAngleType(), CubeObject::SlopeUL, CubeObject::SlopeUR))
			{
				BlockUpperHit(objPos, helf);
			}
			if (Utility::GetBetween<CubeObject::eType>(cube->GetAngleType(), CubeObject::SlopeDL, CubeObject::SlopeDR))
			{
				BlockUnderHit(objPos, helf);
			}
		}
		else
		{
			// コリジョンに対して上から衝突
			if (upper)
			{
				BlockUpperHit(objPos, helf);
				return;
			}

			// 下から衝突
			if (under)
			{
				BlockUnderHit(objPos, helf);
				return;
			}
		}

		// 左から衝突
		if (left)
		{
			BlockLeftHit(objPos, helf);
			return;
		}

		// 右から衝突
		if (right)
		{
			BlockRightHit(objPos, helf);
			return;
		}
	}


	void Player::BlockUpperHit(const Vec3& objPos, const Vec3& helf)
	{
		// 上にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

		// エアショック使用可能にする
		m_firePossible = true;
		m_respawnPos = Vec3(objPos.x, objPos.y + (helf.y * 3.0f), 0.0f);

		// 移動量が下方向にあり
		if (m_velocity.y > 0.0f)
		{
			// 加速度が半分より大きかったら
			if (m_acsel > 2.5f || m_velocity.y > 2.5f)
			{
				// 移動量を反転させ、半分にする
				m_velocity.y *= -0.5f;
				float volume = ((m_velocity.y + m_meddleVelo.y + m_acsel) / 3.0f) /2.5f;
				StartSE(L"ROBOT_BOUND_SE", volume);
			}
		}
	}


	void Player::BlockUnderHit(const Vec3& objPos, const Vec3& helf)
	{
		// 下にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x, objPos.y - (helf.y * 2.0f), 0.0f))) return;

		// 移動量が上方向なら
		if (m_velocity.y < 0.0f)
		{
			// 反転させ、落下させる
			m_velocity.y *= -1.0f;
			StartSE(L"ROBOT_BOUND_SE", 0.25f);
		}
	}


	void Player::BlockLeftHit(const Vec3& objPos, const Vec3& helf)
	{
		// 左にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x - (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// 移動量を半減しつつ反転させる
		m_velocity.x *= -0.5f;
	}


	void Player::BlockRightHit(const Vec3& objPos, const Vec3& helf)
	{
		// 右にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x + (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// 移動量を半減しつつ反転させる
		m_velocity.x *= -0.5f;
	}

	// ブロックに衝突し続けたら
	void Player::BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// ブロックのパラメータを取得
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		// Y軸移動ベクトルを0.0にし、空中かの真偽をfalse
		if (CollHitUpper(hitPos, objPos, helf))
		{
			m_velocity.y = 0.25f;
			m_acsel = 1.0f;
			m_isAir = false;

			const auto& angle = cube->GetAngleType();
			if (angle == CubeObject::Normal)
			{
				if (CollHitUpper(hitPos, objPos, helf))
				{
					SetPosition(GetPosition().x, objPos.y + helf.y + (GetScale().y / 2.0f), 0.0f);
				}
			}

			if (angle == CubeObject::SlopeUL)
			{
				m_velocity.x = 0.175f;
			}

			if (angle == CubeObject::SlopeUR)
			{
				m_velocity.x = -0.175f;
			}
		}
	}

	// スパイクと衝突した瞬間
	void Player::SpikeEnter(const shared_ptr<GameObject>& obj, const Vec3& hitPos)
	{
		// スパイクオブジェクトにキャスト
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// パラメータの取得
		Vec3 spikePos = spike->GetPosition();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// 衝突可否
		bool upper, under, left, right;
		upper = CollHitUpper(hitPos, spikePos, helfScale);
		under = CollHitUnder(hitPos, spikePos, helfScale);
		left = CollHitLeft(hitPos, spikePos, helfScale);
		right = CollHitRight(hitPos, spikePos, helfScale);


		// スパイクの方向に応じて処理
		const auto& angle = spike->GetAngle();
		switch (angle)
		{
		case Gimmick::Up:
			if (upper || left || right)
			{
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}
			if (under)
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::Down:
			if (under || left || right)
			{
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}
			if (upper)
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::Left:
			if (upper || under || left)
			{
				DamageKnockBack(Vec2(1.5f, -0.5f));
				return;
			}
			if (right)
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::Right:
			if (upper || under || right)
			{
				DamageKnockBack(Vec2(-1.5f, -0.5f));
				return;
			}
			if (left)
			{
				BlockEnter(obj, hitPos);
				return;
			}
			break;

		case Gimmick::All:
			if (upper)
			{
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}
			if (under)
			{
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}
			if (left)
			{
				DamageKnockBack(Vec2(1.0f, 0.5f));
				return;
			}
			if (right)
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
			if (BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

			// エアショック使用可能にする
			m_firePossible = true;
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;

			// めり込みを修正
			Vec3 pos = GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			SetPosition(pos);

			const auto& ptr = dynamic_pointer_cast<Convayor>(convayor);
			if (!ptr) return;

			const float& speed = ptr->GetConvayorSpeed();
			const auto& angle = ptr->GetRotate();
			switch (angle)
			{
			case Convayor::LeftRot:
				m_velocity.x = speed;
				break;

			case Convayor::RightRot:
				m_velocity.x = -speed;
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
			Vec3 pos = GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			SetPosition(pos);

			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
		}
	}

	void Player::RabbitEnter(const shared_ptr<GameObject>& other)
	{
		if (m_shieldCount > 1)
		{
			const auto& rabbit = dynamic_pointer_cast<Rabbit>(other);
			if (rabbit) rabbit->SetState(Rabbit::Death);
		}

		DamageKnockBack(m_deffVelo);
	}

	// ダメージノックバック
	void Player::DamageKnockBack(const Vec2& velocity)
	{
		// 時間速度を通常速度で上書き
		m_timeSpeed = m_normalTime;
		m_firePossible = false;
		m_velocity = velocity;
		m_acsel = m_damageAcsel;
		m_isAir = true;

		// アニメーションをダメージ状態にする
		m_bodyDraw->ChangeCurrentAnimation(L"DAMAGE");

		// 軌道の表示をオフ
		m_aligment.lock()->SetDrawActive(false);

		if (!m_isInvincible)
		{
			if (m_shieldCount > 0)
			{
				m_shieldCount--;
				m_isInvincible = true;

				StartSE(L"SHIELD_D_SE", 1.5f);
			}
			else
			{
				DeathSetup();
			}
		}
	}

	void Player::RingCheck(const shared_ptr<GameObject>& ring)
	{
		const auto& ringPtr = dynamic_pointer_cast<Ring>(ring);
		if (ringPtr)
		{
			ringPtr->IsGetRing();
			const auto& size = ringPtr->GetRingSize();
			switch (size)
			{
			case Ring::Big:
				AddShield();
				StartSE(L"RING_SE", 0.75f);
				break;

			case Ring::Small:
				m_sRingCount++;
				StartSE(L"RING_SE", 0.35f);
				break;

			default:
				break;
			}

			if (m_sRingCount >= 25)
			{
				AddShield();
				m_sRingCount = 0;
			}
		}
	}

}