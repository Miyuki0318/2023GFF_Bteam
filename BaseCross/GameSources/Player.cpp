/*! 
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

using namespace Input;
using namespace Utility;

namespace basecross
{
	void Player::OnCreate()
	{
		// 継承元の生成時の関数を実行する
		TemplateObject::OnCreate();

		// 腕モデル用オブジェクトの生成
		m_arm = GetStage()->AddGameObject<TemplateObject>();

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
		m_ptrColl = AddComponent<CollisionSphere>();
		m_ptrColl->SetDrawActive(false);

		// エフェクトオブジェクトの生成
		m_airEffect = GetStage()->AddGameObject<AirJetEffect>(GetThis<TemplateObject>());
		m_shieldEffect = GetStage()->AddGameObject<ShieldEffect>(GetThis<TemplateObject>());
		m_aligment = GetStage()->AddGameObject<ArrowEffect>(GetThis<TemplateObject>());
		m_particle = GetStage()->AddGameObject<MultiParticle>();
	}


	void Player::OnUpdate()
	{
		// ステージとステージステートの取得
		const auto& state = GetStageState<GameStage>();

		// 開始時の動きからゲーム中の範囲なら
		if (GetBetween(state, GameStage::StartMove, GameStage::DeathDrop))
		{
			// Aボタン入力有無での関数分岐
			if (state == GameStage::GameNow)
			{
				if (m_firePossible && m_jumpCount < m_jumpLimit)
				{
					if (GetPushA()) OnRushA();
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

			// ジャンプ回数が0より大きかったら
			if (m_jumpCount > 0)
			{
				RecoveryAirShock();
			}

			// 死亡時のドロップステートなら
			if (state == GameStage::DeathDrop)
			{
				DeathDrop();
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
		}

		if (state == GameStage::StartMove)
		{
			m_airEffect.lock()->SetDrawActive(false);
		}

		// ゴールステートなら
		if(state == GameStage::Goal || state == GameStage::DeathDrop)
		{
			vector<Vec3> point;
			m_aligment.lock()->UpdateEffect(point);
			m_airEffect.lock()->SetDrawActive(false);
			m_shieldEffect.lock()->SetDrawShield(false);
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
		Debug::Log(m_cannonFire != false ? L"発射後" : L"通常");
	}

	// Aボタンを離した時
	void Player::OnRushA()
	{
		// スティック入力を取得し移動ベクトルに保持
		Vec2 stick = GetLStickValue();
		m_velocity = (IsInputLStick() ? stick.round(1) : m_deffVelo) * m_veloSpeed;
		if (!m_isAir && stick.y > 0.0f) m_velocity.y = -stick.round(1).y * m_veloSpeed;
		
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

		// ステージとステージステートの取得
		const auto& stage = GetTypeStage<GameStage>(false);
		if (stage)
		{
			// ステートが開始演出移動なら
			const auto& state = stage->GetStageState();
			if (state == GameStage::StartMove)
			{
				// ステートをゲーム中に設定し、カメラにターゲット設定する
				stage->SetStageState(GameStage::GameNow);
				stage->GetGameCamera()->SetTargetObject(GetThis<Player>());
			}
		}

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
			RingEnter(other);
		}
		if (other->FindTag(L"Rabbit"))
		{
			RabbitEnter(other, hitPoint);
		}
		if (other->FindTag(L"Death"))
		{
			// 死亡時の設定をする
			m_shieldCount = 0;
			StartSE(L"SHIELD_D_SE", 1.5f);
			DeathSetup();
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
		if (other->FindTag(L"Rabbit"))
		{
			RabbitExcute(other, hitPoint);
		}
		if (other->FindTag(L"Cannon"))
		{
			if (!m_cannonStandby) CannonEnter(other);
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
				else
				{
					if (hitPoint.y <= cube->GetPosition().y)
					{
						m_isAir = true;
					}
				}
			}
		}
	}

	void Player::Reset()
	{
		SetPosition(m_startPos);
		m_rotation.zero();
		m_velocity = Vec2(-5.0f, 0.0f);
		m_meddleVelo.zero();
		m_jumpCount = 0;
		m_sRingCount = 0;
		m_shieldCount = 1;
		m_damageTime = 0.0f;
		m_acsel = 7.5f;
		m_jumpRecoveryTime = 0.0f;
		m_isAir = true;
		m_isBlower = false;
		m_isInvincible = false;
		m_firePossible = true;
		m_cannonFire = false;
		m_cannonStandby = false;
		m_ptrColl->SetUpdateActive(true);

		const auto& stage = GetTypeStage<GameStage>();
		const auto& camera = stage->GetGameCamera();
		camera->ResetCamera();
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

			// 送風機に当たっていなく、加算移動量Yが正負とわず0.0より大きかったら
			if (!m_isBlower && !(tempVelo.length() > 0.0f)) m_velocity.y -= m_gravity * deltaTime;
		}
		else
		{
			// 減少量をX軸移動ベクトルの正負で決める
			float decrease = deltaTime * m_maxAcsel;
			decrease *= m_velocity.x > 0.0f ? 1.0f : -1.0f;

			// X軸移動ベクトルが0.01より大きかったら(符号問わず)
			if (m_velocity.x > 0.1f || m_velocity.x < -0.1f) m_velocity.x -= decrease;
			else m_velocity.x = 0.0f;

			// Y軸移動ベクトルが0.25より大きかったら
			if (m_velocity.y > 0.25f) m_velocity.y -= DELTA_TIME;
			else m_velocity.y = 0.25f;
		}

		// 大砲発射後で、加速度が最大加速度以下になったら　
		if (m_cannonFire && m_acsel <= m_maxAcsel) 
		{
			// 発射後状態を解除
			m_cannonFire = false;
		}

		if (m_activeCannon.lock())
		{
			if (!m_activeCannon.lock()->GetFire())
			{
				// ポインタの保持を解除
				m_activeCannon.reset();
			}
		}
	}

	void Player::RecoveryAirShock()
	{
		// ジャンプ回数が0なら終了
		if (m_jumpCount == 0) return;

		// リカバリー経過時間がリカバリー時間以上になったら
		if (m_jumpRecoveryTime >= m_jumpRecoveryLimit)
		{
			m_jumpCount--;
			m_jumpRecoveryTime = 0.0f;
		}

		// リカバリー経過時間をデルタタイムで加算
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
			// スティック入力があるかで向きを設定
			Vec2 dir = IsInputLStick() ? stick : m_velocity;

			// 胴はX軸の正負かで、腕は向きをradianに変換
			body = (dir.x > 0.0f) * XM_PI;
			arm = atan2f(dir.y, dir.x) + XM_PIDIV2;
		}
		else
		{
			// スティック入力の有無で設定(有：スティック入力,無：移動量があるかで設定(有：移動量,無：デフォルト移動量))
			Vec2 dir = IsInputLStick() ? stick : m_velocity != Vec2(0.0f) ? m_velocity : m_deffVelo;

			// スティック入力の有無で設定(有：向きをラジアンに変換,無：デフォルト移動量をラジアンに変換)
			arm = IsInputLStick() ? atan2f(dir.y, dir.x) + XM_PIDIV2 : atan2f(m_deffVelo.x, -m_deffVelo.y);
		} 

		// ローテーションの更新
		SetRotation(Vec3(0.0f, body, 0.0f));
		m_arm.lock()->SetRotation(Vec3(0.0f, 0.0f, arm));
	}

	// 軌道の回転描画
	void Player::RotateAligment()
	{
		// スティック入力があるかの真偽
		const bool& stickInput = IsInputLStick();
		if (stickInput)
		{
			// フレームレート(デルタタイムだと変動する為)
			const float perSecond = 1.0f / 60.0f;

			// スティック入力を取得し移動ベクトルに保持
			const float decrease = 10.0f;
			Vec2 velo = GetLStickValue().round(1) / decrease;

			// 節目と節目のカウンタ
			const int mileNum = 40;
			float mileCount = 1.0f;

			// 節目座標配列
			vector<Vec3> points;

			// 節目の数ループして節目の座標を設定
			for (size_t i = 0; i < mileNum; i++)
			{
				Vec2 pos = -velo * m_speed * m_maxAcsel * mileCount;
				velo.y -= m_gravity * perSecond / (decrease * 2.0f) / (mileNum / 10.0f);
				points.push_back(Vec3(pos.x, pos.y, 0.0f));
				mileCount += (decrease / mileNum);
			}
			m_aligment.lock()->UpdateEffect(points);
		}

		// 移動ガイドラインを更新
		m_aligment.lock()->SetDrawActive(stickInput && m_firePossible);
	}

	// アニメーションの更新
	void Player::AnimationUpdate()
	{
		// 前フレームからのデルタタイムにゲームスピードを掛けた数を取得
		float deltaTime = DELTA_TIME * m_timeSpeed;

		// アニメーションが終わってたら待機状態にする
		if (m_bodyDraw->IsTargetAnimeEnd()) m_bodyDraw->ChangeCurrentAnimation(L"WALK");
		if (m_armDraw->IsTargetAnimeEnd()) m_armDraw->ChangeCurrentAnimation(L"WALK");

		// アニメーションの更新
		m_bodyDraw->UpdateAnimation(deltaTime);
		m_armDraw->UpdateAnimation(deltaTime);
	}

	// エフェクトの更新
	void Player::EffectUpdate()
	{
		// 各エフェクトオブジェクトの更新処理
		m_shieldEffect.lock()->UpdateEffect();
		m_shieldEffect.lock()->SetDrawShield(m_shieldCount > 0 && !m_cannonStandby);
		m_airEffect.lock()->SetDrawActive(m_firePossible && !m_cannonFire);

		// 発射可能且つ大砲発射後でなければ
		if (m_firePossible && !m_cannonFire) m_airEffect.lock()->UpdateEffect();
		
		// 大砲発射後なら
		if (m_cannonFire)
		{
			// 煙パーティクルの初期化
			const auto& particle = m_particle.lock()->InsertParticle(2);
			particle->SetEmitterPos(m_position);
			particle->SetTextureResource(L"SMOKE_TX");
			particle->SetMaxTime(3.0f);
			particle->SetDrawOption(Particle::Normal);

			// 生成したスプライトを配列で取得
			vector<ParticleSprite>& pSpriteVec = particle->GetParticleSpriteVec();

			// スプライトの数ループ
			for (auto& sprite : pSpriteVec)
			{
				// スプライトの初期化設定(移動量・スケール・ローテーション)
				sprite.m_Velocity = Vec3(m_velocity.x, m_velocity.y, 0.0f).normalize();
				sprite.m_LocalScale = Vec2(m_acsel > 2.0f ? m_acsel / 2.5f : 0.0f);
				sprite.m_LocalQt.rotationZ(Utility::DegToRad(Utility::RangeRand(360.0f, 0.0f)));
			}
		}
	}

	// 大砲発射待機時
	void Player::CannonStandby(float acsel)
	{
		// 大砲のポインタがあるなら
		if (m_activeCannon.lock())
		{
			// 大砲のシェアドポインタ
			const auto& cannon = m_activeCannon.lock();

			// 発射時の時間の取得
			const float& fireTime = cannon->GetFireTime();

			// 描画コンポーネントからアニメーションの再生時間を取得
			const auto& drawPtr = cannon->GetComponent<PNTBoneModelDraw>();

			// 再生時間が発射時の時間を過ぎたら
			if (drawPtr->GetCurrentAnimationTime() > fireTime)
			{
				// メンバ変数の設定
				m_acsel = acsel;
				m_isAir = true;
				m_firePossible = true;
				m_cannonFire = true;
				m_cannonStandby = false;
				m_meddleVelo.zero();

				// 大砲のZ軸をラジアンに変換し、移動量を設定する
				float rad = cannon->GetRotation().z - XM_PIDIV2;
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				// SEの再生
				StartSE(L"CANNON_SE", 0.75f);
			}
			else
			{
				// 自身の座標を大砲の中に設定する
				SetPosition(cannon->GetPosition());
			}
		}
	}

	// 無敵時間経過
	void Player::InvincibleTimer()
	{
		// ダメージからの経過時間をデルタタイムで加算
		m_damageTime += DELTA_TIME;

		// 経過時間が無敵時間以上になったら
		if (m_invincibleTime <= m_damageTime)
		{
			// 経過時間のリセットと無敵の解除
			m_damageTime = 0.0f;
			m_isInvincible = false;
		}
	}

	// 死亡時の設定
	void Player::DeathSetup()
	{
		// ステージステートがゲーム中なら
		const auto& stage = GetTypeStage<GameStage>();
		if (stage->GetStageState() == GameStage::GameNow)
		{
			// SEの再生
			StartSE(L"DAMAGE_SE", 0.75f);

			// ステートを死亡時の落下にし、カメラのターゲットを解除
			stage->SetStageState(GameStage::DeathDrop);
			stage->GetGameCamera()->RemoveTarget();

			// 座標と移動量と加速度を設定
			SetPosition(GetPosition() + Vec3(0.0f, 0.0f, -1.5f));
			m_velocity = m_deffVelo * 2.5f;
			m_acsel = 2.5f;

			// コリジョンを非アクティブに
			m_ptrColl->SetUpdateActive(false);
		}
	}

	// 死亡時の落下
	void Player::DeathDrop()
	{
		// 死亡時の座標と現在の座標から長さを求める
		const auto& stage = GetTypeStage<GameStage>();
		const Vec3& deathPos = stage->GetGameCamera()->GetCurrentPos();
		float length = (deathPos - GetPosition()).length();

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
			// ステートを死亡にし、SEを再生
			stage->SetStageState(GameStage::Death);
			stage->CreateSE(L"METAL_SE", 0.75f);
		}
	}

	// 送風機のチェック
	void Player::BlowerBetweenCheck()
	{
		// アップデートオブジェクトグループのポインタ配列の取得
		const auto& groupVec = GetStage()->GetSharedObjectGroup(L"Gimmick")->GetGroupVector();

		// ギミックの角度と送風機の中に居るかの真偽
		Gimmick::eAngle angle = Gimmick::Up;
		bool check = false;

		// グループ配列のオブジェクトの数ループ
		for (const auto& ptr : groupVec)
		{
			// 送風機に型キャスト出来たら
			const auto& blower = dynamic_pointer_cast<Blower>(ptr.lock());
			if (blower)
			{
				// プレイヤーが送風機の送風範囲に居るか
				check = blower->CheckBetween(GetPosition());
				if (check)
				{
					// ギミックの角度の取得
					angle = blower->GetAngle();
					break;
				}
			}
		}

		// 送風範囲に居たら
		if (check)
		{
			// 角度事に加算移動量を設定する
			switch (angle)
			{
			case Gimmick::Up:
				Debug::Log(L"送風機上");
				UpMeddleVelocity();
				break;

			case Gimmick::Down:
				Debug::Log(L"送風機下");
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
			// 送風機に当たったかの真偽をリセット
			m_isBlower = false;

			// 加算移動量の減少関数
			ReductionMeddleVelocity();
		}
	}

	// 上向きの送風機に当たっている時
	void Player::UpMeddleVelocity()
	{
		// 送風機の範囲内かの真偽をtrue
		m_isBlower = true;

		// 減少量を移動量Y軸の正負に応じてデルタタイムを設定する
		float decrease = DELTA_TIME;
		decrease *= m_velocity.y > 0.0f ? 1.0f : -1.0f;

		// 移動量Y軸が正負問わず減少量より大きかったら減らす
		bool amountVelo = (m_velocity.y > decrease || m_velocity.y < decrease);
		if (amountVelo) m_velocity.y -= decrease * m_maxAcsel;
		else m_velocity.y = decrease;

		// 加算移動量が速度より大きかったら加算移動量を減らす(移動量がマイナスである程上に上昇)
		if (m_meddleVelo.y > -m_speed) m_meddleVelo.y -= DELTA_TIME * m_maxAcsel;
	}

	// 下向きの送風機に当たっている時
	void Player::DownMeddleVelocity()
	{
		// 移動量Y軸を増やす
		m_velocity.y += DELTA_TIME * m_maxAcsel;
	}

	// 左向きの送風機に当たっている時
	void Player::LeftMeddleVelocity()
	{
		// 加算移動量X軸を増やす
		m_meddleVelo.x += DELTA_TIME * m_maxAcsel * m_speed;
	}

	// 右向きの送風機に当たっている時
	void Player::RightMeddleVelocity()
	{
		// 加算移動量X軸を減らす
		m_meddleVelo.x -= DELTA_TIME * m_maxAcsel * m_speed;
	}

	// 加算移動量の減少
	void Player::ReductionMeddleVelocity()
	{
		// 減少量を加算移動量X軸の正負に応じて設定
		float decrease = DELTA_TIME;
		decrease *= m_meddleVelo.x > 0.0f ? 1.0f : -1.0f;

		// 加算移動量X軸が正負問わず0.1より大きかったら減らす
		bool amountVelo = (m_meddleVelo.x > 0.1f || m_meddleVelo.x < -0.1f);
		if (amountVelo) m_meddleVelo.x -= decrease;
		else m_meddleVelo.x = 0.0f;

		// 減少量を加算移動量Y軸の正負に応じて設定
		decrease = DELTA_TIME;
		decrease *= m_meddleVelo.y > 0.0f ? 1.0f : -1.0f;

		// 加算移動量Y軸が正負問わず0.1より大きかったら減らす
		amountVelo = (m_meddleVelo.y > 0.1f || m_meddleVelo.y < -0.1f);
		if (amountVelo) m_meddleVelo.y -= decrease * 5.0f;
		else m_meddleVelo.y = 0.0f;
	}

	// ブロックに衝突した瞬間
	void Player::BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// ブロックのパラメータを取得
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		// 衝突方向真偽
		bool upper, under, left, right;
		upper = CollHitUpper(hitPos, objPos, helf) && !BlockCheck(objPos + Vec3(0.0f, 1.0f, 0.0f));
		under = CollHitUnder(hitPos, objPos, helf) && !BlockCheck(objPos + Vec3(0.0f, -1.0f, 0.0f));
		left = CollHitLeft(hitPos, objPos, helf) && !BlockCheck(objPos + Vec3(1.0f, 0.0f, 0.0f));
		right = CollHitRight(hitPos, objPos, helf) && !BlockCheck(objPos + Vec3(-1.0f, 0.0f, 0.0f));

		// Z軸の角度を取得でスロープかどうかを判断
		const float& deg = cube->GetDegreeAngle().z;
		if (deg != 0.0f)
		{
			// スロープの向きで上から、下からの応答処理を行う
			const auto& angle = cube->GetAngleType();
			if (Utility::GetBetween<CubeObject::eType>(angle, CubeObject::SlopeUL, CubeObject::SlopeUR))
			{
				BlockUpperHit(objPos, helf);
			}
			if (Utility::GetBetween<CubeObject::eType>(angle, CubeObject::SlopeDL, CubeObject::SlopeDR))
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

	// ブロックの上から衝突した時の応答処理
	void Player::BlockUpperHit(const Vec3& objPos, const Vec3& helf)
	{
		// 上にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

		// エアショック使用可能にする
		m_firePossible = true;

		// 移動量が下方向にあり
		if (m_velocity.y > 0.0f)
		{
			// 加速度が半分より大きかったら
			if (m_acsel > 2.5f || m_velocity.y > 2.5f)
			{
				// 移動量を反転させ、半分にする
				m_velocity.y *= -0.5f;

				// バウンド時のSEを再生
				BlockBoundSE();
			}
		}
	}

	// ブロックの下から衝突した時の応答処理
	void Player::BlockUnderHit(const Vec3& objPos, const Vec3& helf)
	{
		// 下にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x, objPos.y - (helf.y * 2.0f), 0.0f))) return;

		// エアショック使用可能にする
		m_firePossible = true;

		// 移動量が上方向なら
		if (m_velocity.y < 0.0f)
		{
			// 反転させ、落下させる
			m_velocity.y *= -1.0f;

			// バウンド時のSEを再生
			BlockBoundSE();
		}
	}

	// ブロックの左から衝突した時の応答処理
	void Player::BlockLeftHit(const Vec3& objPos, const Vec3& helf)
	{
		// 左にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x - (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// 移動量を半減しつつ反転させる
		m_velocity.x *= -0.5f;

		// バウンド時のSEを再生
		BlockBoundSE();
	}

	// ブロックの右から衝突した時の応答処理
	void Player::BlockRightHit(const Vec3& objPos, const Vec3& helf)
	{
		// 右にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x + (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// 移動量を半減しつつ反転させる
		m_velocity.x *= -0.5f;

		// バウンド時のSEを再生
		BlockBoundSE();
	}

	// ブロックのバウンドのSE再生
	void Player::BlockBoundSE()
	{
		// 速度の平均を移動量速度で割った値を音量としてSEを再生
		float volume = ((m_velocity.y + m_meddleVelo.y + m_acsel) / 3.0f) / m_veloSpeed;
		StartSE(L"ROBOT_BOUND_SE", volume);
	}

	// ブロックに衝突し続けたら
	void Player::BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// ブロックのパラメータを取得
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		// 上から衝突していたら
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// パラメータの設定
			m_velocity.y = 0.25f;
			m_acsel = 1.0f;
			m_isAir = false;

			// 通常か、スロープかで設定
			const auto& angle = cube->GetAngleType();
			switch (angle)
			{
			case CubeObject::Normal:
				SetPosition(GetPosition().x, objPos.y + helf.y + (GetScale().y / 2.0f), 0.0f);
				break;

			case CubeObject::SlopeUL:
				m_velocity.x = 0.175f;
				break;

			case CubeObject::SlopeUR:
				m_velocity.x = -0.175f;
				break;

			default:
				break;
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
		Vec3 scale = spike->GetScale();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// 衝突方向真偽
		const auto& groupVec = GetStage()->GetSharedObjectGroup(L"Update")->GetGroupVector();
		bool upper, under, left, right;
		upper = CollHitUpper(hitPos, spikePos, helfScale) && !BlockCheck(groupVec, Vec3(spikePos + Vec3(0.0f, scale.y, 0.0f)));
		under = CollHitUnder(hitPos, spikePos, helfScale) && !BlockCheck(groupVec, Vec3(spikePos + Vec3(0.0f, -scale.y, 0.0f)));
		left = CollHitLeft(hitPos, spikePos, helfScale) && !BlockCheck(groupVec, Vec3(spikePos + Vec3(-scale.x, 0.0f, 0.0f)));
		right = CollHitRight(hitPos, spikePos, helfScale) && !BlockCheck(groupVec, Vec3(spikePos + Vec3(scale.x, 0.0f, 0.0f)));

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
				DamageKnockBack(Vec2(1.5f, -0.5f));
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

		// スパイクの方向が下向きなら地面と同じ処理
		const auto& angle = spike->GetAngle();
		if (angle == Gimmick::Down)
		{
			m_firePossible = true;
			BlockExcute(obj, hitPos);
		}
		else
		{
			if (!m_isInvincible)
			{
				SpikeEnter(obj, hitPos);
			}
		}
	}

	// 大砲と衝突した時
	void Player::CannonEnter(const shared_ptr<GameObject>& cannon)
	{
		// 大砲型にキャスト
		const auto& ptr = dynamic_pointer_cast<Cannon>(cannon);
		if (ptr)
		{
			if (m_activeCannon.lock() != ptr)
			{
				// 発射準備関数
				ptr->OnFire();

				// 乗った大砲のポインタを保持
				m_activeCannon.reset();
				m_activeCannon = ptr;

				// パラメータの設定
				m_isAir = false;
				m_firePossible = false;
				m_cannonStandby = true;

				// 実質移動不可に設定
				m_velocity = Vec2(0.0f);
				m_acsel = 1.0f;
			}
		}
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

			// ベルトコンベア型にキャスト
			const auto& ptr = dynamic_pointer_cast<Convayor>(convayor);
			if (!ptr) return;

			// 速度と向きを取得
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
			// めり込みを解消
			Vec3 pos = GetPosition();
			pos.y = objPos.y + objTrans->GetScale().y + 0.1f;
			SetPosition(pos);

			// 上方向への移動を無くす(乗り続ける為に)
			m_velocity.y = 0.0f;
			m_acsel = 1.0f;
		}
	}

	// 敵のウサギと衝突した時
	void Player::RabbitEnter(const shared_ptr<GameObject>& other, const Vec3& hitPos)
	{			
		// ウサギ型にキャスト
		const auto& rabbit = dynamic_pointer_cast<Rabbit>(other);
		if (rabbit)
		{
			if (!m_isInvincible)
			{
				// シールドが二枚以上あるなら
				if (m_cannonFire)
				{
					// ウサギのステートを死亡に設定
					rabbit->SetState(Rabbit::Death);
					return;
				}
				if (m_shieldCount >= 2)
				{
					// ウサギのステートを死亡に設定
					rabbit->SetState(Rabbit::Death);
					m_shieldCount--;
				}
				else
				{
					// 衝突方向真偽
					Vec3 helfScale = rabbit->GetScale() / 2.0f;

					if (CollHitLeft(hitPos, hitPos, helfScale))
					{
						DamageKnockBack(Vec2(1.5f, -0.5f));
						return;
					}
					if (CollHitRight(hitPos, hitPos, helfScale))
					{
						DamageKnockBack(Vec2(-1.5f, -0.5f));
						return;
					}
				}
			}
		}
	}

	// 敵のウサギと衝突し続けた時
	void Player::RabbitExcute(const shared_ptr<GameObject>& rabbit, const Vec3& hitPos)
	{
		// 無敵中じゃなければ
		if (!m_isInvincible)
		{
			// ウサギと衝突した時の関数を実行
			RabbitEnter(rabbit, hitPos);
		}
	}

	// ダメージノックバック
	void Player::DamageKnockBack(const Vec2& velocity)
	{
		// 時間速度を通常速度で上書き
		m_firePossible = false;
		m_velocity = velocity;
		m_acsel = m_damageAcsel;
		m_isAir = true;

		// アニメーションをダメージ状態にする
		m_bodyDraw->ChangeCurrentAnimation(L"DAMAGE");

		// 軌道の表示をオフ
		m_aligment.lock()->SetDrawActive(false);

		// 無敵中じゃなければ
		if (!m_isInvincible)
		{
			// シールドの数が1枚以上なら
			if (m_shieldCount >= 1)
			{
				// シールドを減らし、無敵時間にする
				m_shieldCount--;
				m_isInvincible = true;

				// SEの再生
				StartSE(L"SHIELD_D_SE", 1.5f);
			}
			else
			{
				// 死亡時の設定をする
				DeathSetup();
			}
		}
	}

	// リングに衝突時のチェック関数
	void Player::RingEnter(const shared_ptr<GameObject>& ring)
	{
		// リングに型キャストする
		const auto& ringPtr = dynamic_pointer_cast<Ring>(ring);
		if (ringPtr)
		{
			// リングに取得状態を送る
			ringPtr->IsGetRing();

			// リングのサイズに応じて取得処理をする
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

			// 小さいリングの取得個数が25個以上になったら
			if (m_sRingCount >= m_sRingLimit)
			{
				// シールドを増やしてカウンタをリセット
				AddShield();
				m_sRingCount = 0;
			}
		}
	}
}