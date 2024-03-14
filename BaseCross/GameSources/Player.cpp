/*! 
@file Player.cpp
@brief プレイヤー本体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;
	using namespace Utility;
	using namespace CubeParam;
	using namespace GimmickAngle;

	// 生成時の処理
	void Player::OnCreate()
	{
		// 継承元の生成時の関数を実行する
		TemplateObject::OnCreate();

		// 腕モデル用オブジェクトの生成
		m_arm = GetStage()->AddGameObject<TemplateObject>();

		// 胴のトランスフォームの設定
		SetPosition(m_position);
		SetScale(m_scale);
		
		// 腕のトランスフォームの設定
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
		m_arrow = GetStage()->AddGameObject<ArrowEffect>(GetThis<TemplateObject>());
		m_particle = GetStage()->AddGameObject<MultiParticle>();
	}

	// 毎フレーム更新処理
	void Player::OnUpdate()
	{
		// ステージとステージステートの取得
		const auto& state = GetStageState<GameStage>();

		// 開始時の動きからゲーム中の範囲なら
		if (GetBetween(state, GameStage::StartMove, GameStage::DeathDrop))
		{
			// ゲーム中か
			if (state == GameStage::GameNow)
			{
				// エアショック発射可能でジャンプ回数が上限以下か
				if (m_status(eStatus::IsFirePossible) && m_jumpCount < m_jumpLimit)
				{
					// Aボタン入力があったら
					if (GetPushA()) OnRushA();
				}

				// 照準の回転処理
				UpdateArrow();

				// プレイヤーの回転関数
				RotatePlayer();
			}

			// 大砲待機時
			if (m_status(eStatus::IsCannonStandby))
			{
				// 大砲発射待機関数(加速度)
				CannonStandby(10.0f);
			}

			// 無敵時間中なら
			if (m_status(eStatus::IsInvincible))
			{
				// 無敵時間のタイマー
				InvincibleTimer();
			}

			// ジャンプ回数が0より大きかったら
			if (m_jumpCount > 0)
			{
				// ジャンプ回数のリカバリ
				RecoveryAirShock();
			}

			// 衝突した動く壁のポインタがあれば
			if (m_currentWall.lock())
			{
				// 動く壁から離れたかの検証
				MoveWallExit();
			}

			// 死亡時のドロップステートなら
			if (state == GameStage::DeathDrop)
			{
				// 死亡時の落下処理
				DeathDrop();
			}

			// 送風機のチェック
			BlowerBetweenCheck();

			// プレイヤーの移動関数
			MovePlayer();

			// 移動時の減少量
			MoveReduction();

			// 大砲発射後
			AftterCannon();

			// アニメーションの再生
			AnimationUpdate();

			// エフェクト描画関数
			EffectUpdate();
		}

		// 開始時の吹っ飛び演出中なら
		if (state == GameStage::StartMove)
		{
			// エアショックのエアショックを非表示
			m_airEffect.lock()->SetDrawActive(false);
		}

		// ゴール時または死亡落下中なら
		if(state == GameStage::Goal || state == GameStage::DeathDrop)
		{
			// エフェクトを非表示
			vector<Vec3> point;
			m_arrow.lock()->UpdateEffect(point);
			m_airEffect.lock()->SetDrawActive(false);
			m_shieldEffect.lock()->SetDrawShield(false);
		}

		// デバッグ文字列
		//Debug::Log(L"座標 : ", m_position);
		//Debug::Log(L"移動量 : ", m_velocity);
		//Debug::Log(L"加算移動量 : ", m_meddleVelo);
		//Debug::Log(L"加速度 : ", m_acsel);
		//Debug::Log(L"ジャンプ回数 : ", m_jumpCount);
		//Debug::Log(L"シールドの数 : ", m_shieldCount);
		//Debug::Log(m_isAir != false ? L"空中" : L"接地");
		//Debug::Log(m_firePossible != false ? L"発射可" : L"発射不可");
		//Debug::Log(m_cannonFire != false ? L"発射後" : L"通常");
		//Debug::Log(L"移動ブロックの上か : ", m_isAliveMoveBlock);
		//Debug::Log(L"左右からブロックに押されている : ", m_isHitMoveBlock);
	}

	// Aボタンを押した時
	void Player::OnRushA()
	{
		// スティック入力を取得し移動ベクトルに保持
		Vec2 stick = GetLStickValue();
		m_velocity = (IsInputLStick() ? stick.round(1) : m_deffVelo) * m_veloSpeed;

		// 地面に居るときに地面に向かって入力していたらY軸を反転
		if (!m_status(eStatus::IsAir) && stick.y > 0.0f) m_velocity.y = -stick.round(1).y * m_veloSpeed;
		
		// パラメータの設定
		m_status.Set(eStatus::IsAir) = true;
		m_status.Set(eStatus::IsCannonFire) = false;
		m_acsel = m_maxAcsel;
		m_meddleVelo.zero();
		m_jumpCount++;
		m_jumpRecoveryTime = 0.0f;

		// スティックのY軸が多めに入力されていたらハイジャンプ状態にする
		m_status.Set(eStatus::IsHighJump) = (stick.y <= -0.75f);

		// 腕のアニメーションを変更
		m_armDraw->ChangeCurrentAnimation(L"FIRE");

		// SEの再生
		StartSE(L"AIRSHOCK_SE", 0.5f);
	}

	// コリジョンに衝突したら
	void Player::OnCollisionEnter(const CollisionPair& Pair)
	{
		// 衝突したオブジェクトと衝突座標を取得
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

		// タグがブロックなら
		if (other->FindTag(L"Block"))
		{
			// ブロックと衝突した瞬間の処理を実行
			BlockEnter(other, hitPoint);
		}

		// タグが棘なら
		if (other->FindTag(L"Spike"))
		{
			// 棘と衝突した瞬間の処理を実行
			SpikeEnter(other, hitPoint);
		}

		// タグが大砲なら
		if (other->FindTag(L"Cannon"))
		{
			// 大砲と衝突した瞬間の処理を実行
			CannonEnter(other);
		}

		// タグがゴールなら
		if (other->FindTag(L"Goal"))
		{
			// ステージからカメラを取得
			const auto& stage = GetTypeStage<GameStage>();
			const auto& camera = stage->GetGameCamera();
			if (camera)
			{
				// 大砲と衝突した瞬間の処理を実行
				CannonEnter(other);

				// カメラのターゲットを外し、ゴール時のステートに切り替え
				camera->RemoveTarget();
				stage->SetStageState(GameStage::Goal);
			}
		}

		// タグがベルトコンベアなら
		if (other->FindTag(L"Convayor"))
		{
			// ベルトコンベアに衝突した瞬間の処理を実行
			ConvayorEnter(other, hitPoint);
		}

		// タグがバンパーなら
		if (other->FindTag(L"Bumper"))
		{
			// バンパーに衝突した瞬間の処理を実行
			BumperEnter(other, hitPoint);
		}
		
		// タグがリングなら
		if (other->FindTag(L"Ring"))
		{
			// リングに衝突した瞬間の処理を実行
			RingEnter(other);
		}

		// タグがウサギなら
		if (other->FindTag(L"Rabbit"))
		{
			// ウサギに衝突した瞬間の処理を実行
			RabbitEnter(other, hitPoint);
		}

		// タグが動く壁なら
		if (other->FindTag(L"MoveWall"))
		{
			// 動く壁に衝突した瞬間の処理を実行
			MoveWallEnter(other, hitPoint);

			// ブロックに衝突した瞬間の処理を実行
			BlockEnter(other, hitPoint);
		}

		// タグが死亡するコリジョンなら
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
		// 衝突したオブジェクトと衝突座標を取得
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		// タグがブロックなら
		if (other->FindTag(L"Block"))
		{
			// ブロックに衝突し続けた時の処理を実行
			BlockExcute(other, hitPoint);
		}

		// タグが棘なら
		if (other->FindTag(L"Spike"))
		{
			// 棘に衝突し続けた時の処理を実行
			SpikeExcute(other, hitPoint);
		}

		// タグがウサギなら
		if (other->FindTag(L"Rabbit"))
		{
			// ウサギに衝突し続けた時の処理を実行
			RabbitExcute(other, hitPoint);
		}

		// タグが大砲なら
		if (other->FindTag(L"Cannon"))
		{
			// 大砲発射待機時じゃないなら大砲と衝突した瞬間の処理を実行
			if (!m_status(eStatus::IsCannonStandby)) CannonEnter(other);
		}

		// タグが動く壁なら
		if (other->FindTag(L"MoveWall"))
		{
			// 動く壁に衝突し続けた時の処理を実行
			MoveWallEnter(other, hitPoint);
		}

		// タグがベルトコンベアなら
		if (other->FindTag(L"Convayor"))
		{
			// ベルトコンベアに衝突し続けた時の処理を実行
			ConvayorExcute(other, hitPoint);
		}
	}

	// コリジョンとの衝突が終わった時
	void Player::OnCollisionExit(const CollisionPair& Pair)
	{
		// 衝突し終わったオブジェクトと衝突座標を取得
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		// タグがブロックまたは動く壁なら
		if (other->FindTag(L"Block") || other->FindTag(L"MoveWall"))
		{
			// 型キャスト
			const auto& cube = dynamic_pointer_cast<CubeObject>(other);
			if (cube)
			{
				// タイプを取得
				const auto& type = cube->GetCubeType();

				// 通常タイプなら
				if (type == CubeParam::eCubeType::Normal)
				{
					// 空中かの真偽をtrueに
					m_status.Set(eStatus::IsAir) = true;
				}
				else
				{
					// スロープで、オブジェクトの位置より最後に衝突した位置が低かったら
					if (hitPoint.y <= cube->GetPosition().y)
					{
						// 空中かの真偽をtrueに
						m_status.Set(eStatus::IsAir) = true;
					}
				}
			}
		}
	}

	// コンティニュー時リセット関数
	void Player::Reset()
	{
		SetPosition(m_startPos);		// 開始座標に更新
		m_rotation.zero();				// 向きを初期化
		m_velocity = Vec2(-5.0f, 0.0f); // 移動量を横に吹っ飛ぶ値で更新
		m_meddleVelo.zero();			// 加算移動量を初期化
		m_jumpCount = 0;				// ジャンプ回数を初期化
		m_sRingCount = 0;				// 小リングの数を初期化
		m_shieldCount = 1;				// シールドを一枚の状態に更新
		m_acsel = 7.5f;					// 加速度を更新
		m_jumpRecoveryTime = 0.0f;		// ジャンプリカバリタイムを初期化
		m_status.Reset();				// 状態フラグを初期化

		// 状態フラグの「空中かに居るか?」と「エアショック発射可能か?」をオンに
		m_status.Set(eStatus::IsAir, eStatus::IsFirePossible) = true;

		// コリジョンをアクティブに
		m_ptrColl->SetUpdateActive(true);

		// ステージからカメラを取得して、カメラを初期化
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

		// 座標と体のモデルのボーンの配列を取得
		Vec3 armPos = m_position;
		const auto& bone = m_bodyDraw->GetVecLocalBones();

		// 腕と胴のモデルマトリクスのポジションy軸の差分とボーンのアニメーション量をポジションから差し引く
		armPos.y -= (m_armMat.getMajor3().y - m_bodyMat.getMajor3().y) - bone.at(1).getTranslation().y;

		// 座標の更新
		SetPosition(m_position);
		m_arm.lock()->SetPosition(armPos);
	}

	// 移動減少量
	void Player::MoveReduction()
	{
		// デルタタイムを取得
		float deltaTime = DELTA_TIME * m_timeSpeed;

		// 空中なら
		if (m_status(eStatus::IsAir))
		{
			// 加速度が1.0より大きかったら加速度分の二倍をデルタタイムで掛けた数で減算、小さかったら1.0に修正
			m_acsel > 1.0f ? m_acsel -= deltaTime * (m_acsel * 2.0f) : m_acsel = 1.0f;

			// Y軸移動ベクトルを重力とデルタタイムで掛けた数で減算
			Vec2 meddleVelo = m_meddleVelo;
			meddleVelo.x = 0.0f;

			// 送風機に当たっていなく、加算移動量Yが正負とわず0.0なら
			if (!m_status(eStatus::IsBlower) && !(meddleVelo.length() > 0.0f))
			{
				// 高く跳んでいる状態で、移動量Y軸が0.0から1.5の範囲内なら、落下量を半分にする
				float dropVal = (m_status(eStatus::IsHighJump) && GetBetween(m_velocity.y, 0.0f, 1.5f)) ? 0.5f : 1.0f;
				m_velocity.y -= m_gravity * deltaTime * dropVal;
			}
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
	}

	// エアショックのリカバリ
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
		if (m_status(eStatus::IsAir))
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
	void Player::UpdateArrow()
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
			m_arrow.lock()->UpdateEffect(points);
		}

		// 移動ガイドラインを更新
		m_arrow.lock()->SetDrawActive(stickInput && m_status(eStatus::IsFirePossible));
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
		// フラグの取得
		bool possible, cFire, cStandby;
		possible = m_status(eStatus::IsFirePossible);
		cFire = m_status(eStatus::IsCannonFire);
		cStandby = m_status(eStatus::IsCannonStandby);

		// 各エフェクトオブジェクトの更新処理
		m_shieldEffect.lock()->UpdateEffect();
		m_shieldEffect.lock()->SetDrawShield(m_shieldCount > 0 && !cStandby);
		m_airEffect.lock()->SetDrawActive(possible && !cFire);

		// 発射可能且つ大砲発射後でなければ
		if (possible && !cFire) m_airEffect.lock()->UpdateEffect();
		
		// 大砲発射後なら
		if (cFire)
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
			const auto& animeTime = cannon->GetAnimationTime();

			// 再生時間が発射時の時間を過ぎたら
			if (animeTime > fireTime)
			{
				// メンバ変数の設定
				m_acsel = acsel;
				m_status.Set(eStatus::IsAir, eStatus::IsFirePossible, eStatus::IsCannonFire) = true;
				m_status.Set(eStatus::IsCannonStandby) = false;
				m_meddleVelo.zero();

				// 大砲のZ軸をラジアンに変換し、移動量を設定する
				float rad = cannon->GetRotation().z - XM_PIDIV2;
				if (floor(RadToDeg(rad)) == 90.0f) m_acsel -= 2.5f; // 下向きの時だけ地面が貫通し易い為少し減速
				m_velocity = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				// タグがゴールなら
				if (cannon->FindTag(L"Goal"))
				{
					// クリア時のSEを再生
					StartSE(L"CLEAR_SE", 1.0f);

					// コリジョンを非アクティブに
					m_ptrColl->SetUpdateActive(false);
					return;
				}

				// 大砲発射のSEの再生
				StartSE(L"CANNON_SE", 0.75f);
			}
			else
			{
				// 自身の座標を大砲の中に設定する
				SetPosition(cannon->GetPosition());
			}
		}
	}

	// 大砲発射後
	void Player::AftterCannon() 
	{
		// 大砲発射後で、加速度が最大加速度以下になったら　
		if (m_status(eStatus::IsCannonFire) && m_acsel <= m_maxAcsel)
		{
			// 発射後状態を解除
			m_status.Set(eStatus::IsCannonFire) = false;
		}

		// 大砲のポインタが保持されていれば
		if (m_activeCannon.lock())
		{
			// 発射前の大砲なら
			if (!m_activeCannon.lock()->GetFire())
			{
				// ポインタの保持を解除
				m_activeCannon.reset();
			}
		}
	}

	// 無敵時間経過
	void Player::InvincibleTimer()
	{
		// 無敵時間でタイマーを設定
		if (SetTimer(m_invincibleTime))
		{
			// 経過時間のリセットと無敵の解除
			m_status.Set(eStatus::IsInvincible) = false;
			m_bodyDraw->SetDrawActive(true);
			m_armDraw->SetDrawActive(true);
		}

		// 無敵中なら
		if (m_status(eStatus::IsInvincible))
		{
			// 0.05f(秒)間隔で点滅させる
			if (SetTimer(0.05f))
			{
				// 描画してるかの真偽を反転して取得
				bool oppositeDraw = !m_bodyDraw->GetDrawActive();
				m_bodyDraw->SetDrawActive(oppositeDraw);
				m_armDraw->SetDrawActive(oppositeDraw);
			}
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
		const Vec3& deathPos = stage->GetGameCamera()->GetTargetPos();
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
		}
	}

	// 送風機のチェック
	void Player::BlowerBetweenCheck()
	{
		// アップデートオブジェクトグループのポインタ配列の取得
		const auto& groupVec = GetStage()->GetSharedObjectGroup(L"Gimmick")->GetGroupVector();

		// ギミックの角度と送風機の中に居るかの真偽
		eAngle angle = eAngle::Up;
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
			case eAngle::Up:
				UpMeddleVelocity();
				break;

			case eAngle::Down:
				DownMeddleVelocity();
				break;

			case eAngle::Left:
				LeftMeddleVelocity();
				break;

			case eAngle::Right:
				RightMeddleVelocity();
				break;

			default:
				break;
			}
		}	
		else
		{
			// 送風機に当たったかの真偽をリセット
			m_status.Set(eStatus::IsBlower) = false;

			// 加算移動量の減少関数
			ReductionMeddleVelocity();
		}
	}

	// 上向きの送風機に当たっている時
	void Player::UpMeddleVelocity()
	{
		// 送風機の範囲内かの真偽をtrue
		m_status.Set(eStatus::IsBlower) = true;

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
		upper = CollHitUpper(hitPos, objPos, helf) && !BlockCheck(objPos + UP_VEC);
		under = CollHitUnder(hitPos, objPos, helf) && !BlockCheck(objPos + DOWN_VEC);
		left = CollHitLeft(hitPos, objPos, helf) && !BlockCheck(objPos + LEFT_VEC);
		right = CollHitRight(hitPos, objPos, helf) && !BlockCheck(objPos + RIGHT_VEC);

		// Z軸の角度を取得でスロープかどうかを判断
		const float& deg = cube->GetDegreeAngle().z;
		if (deg != 0.0f)
		{
			// スロープの向きで上から、下からの応答処理を行う
			const auto& angle = cube->GetCubeType();
			if (GetBetween<eCubeType>(angle, eCubeType::SlopeUL, eCubeType::SlopeUR))
			{
				// 上からの応答処理を送る
				BlockUpperHit(objPos, helf);
			}
			if (GetBetween<eCubeType>(angle, eCubeType::SlopeDL, eCubeType::SlopeDR))
			{
				// 下からの応答処理を送る
				BlockUnderHit(objPos, helf);
			}
		}
		else
		{
			// コリジョンに対して上から衝突
			if (upper)
			{
				// 上からの応答処理を送る
				BlockUpperHit(objPos, helf);
				return;
			}

			// 下から衝突
			if (under)
			{
				// 下からの応答処理を送る
				BlockUnderHit(objPos, helf);
				return;
			}
		}

		// 左から衝突
		if (left)
		{
			// 左からの応答処理を送る
			BlockLeftHit(objPos, helf);

			// 動く壁じゃなければ
			const auto& wall = dynamic_pointer_cast<MoveWall>(cube);
			if (!wall)
			{
				// 左右から圧死してないかの確認処理を送る
				LeftRightCompressedDeath();
			}
			return;
		}

		// 右から衝突
		if (right)
		{
			// 右からの応答処理を送る
			BlockRightHit(objPos, helf);

			// 動く壁じゃなければ
			const auto& wall = dynamic_pointer_cast<MoveWall>(cube);
			if (!wall)
			{
				// 左右から圧死してないかの確認処理を送る
				LeftRightCompressedDeath();
			}
			return;
		}
	}

	// ブロックの上から衝突した時の応答処理
	void Player::BlockUpperHit(const Vec3& objPos, const Vec3& helf)
	{
		// 上にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x, objPos.y + (helf.y * 2.0f), 0.0f))) return;

		// エアショック使用可能にする
		m_status.Set(eStatus::IsFirePossible) = true;

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
		m_status.Set(eStatus::IsFirePossible) = true;

		// 移動量が上方向なら
		if (m_velocity.y < 0.0f)
		{
			// 反転させ、落下させる
			m_velocity.y *= -1.0f;

			if (m_acsel > 2.5f)
			{
				// バウンド時のSEを再生
				BlockBoundSE();
			}
		}

		// 下向きで圧死してないかの確認処理を送る
		UnderCompressedDeath();
	}

	// ブロックの左から衝突した時の応答処理
	void Player::BlockLeftHit(const Vec3& objPos, const Vec3& helf)
	{
		// 左にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x - (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// 移動量を半減しつつ反転させる
		m_velocity.x *= -0.5f;

		if (m_acsel > 2.5f)
		{
			// バウンド時のSEを再生
			BlockBoundSE();
		}
	}

	// ブロックの右から衝突した時の応答処理
	void Player::BlockRightHit(const Vec3& objPos, const Vec3& helf)
	{
		// 右にブロックがあるかのチェック
		if (BlockCheck(Vec3(objPos.x + (helf.x * 2.0f), objPos.y, 0.0f))) return;

		// 移動量を半減しつつ反転させる
		m_velocity.x *= -0.5f;

		if (m_acsel > 2.5f)
		{
			// バウンド時のSEを再生
			BlockBoundSE();
		}
	}

	// ブロックのバウンドのSE再生
	void Player::BlockBoundSE()
	{
		// 速度の平均を移動量速度で割った値を音量としてSEを再生
		float volume = ((m_velocity.y + m_meddleVelo.y + m_acsel) / 3.0f) / m_veloSpeed / 2.0f;
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
			m_status.Set(eStatus::IsAir) = false;

			// 通常か、スロープかで設定
			const auto& angle = cube->GetCubeType();
			switch (angle)
			{
			case CubeParam::eCubeType::Normal:
				SetPosition(GetPosition().x, objPos.y + helf.y + (GetScale().y / 2.0f), 0.0f); // 接地処理
				break;

			case CubeParam::eCubeType::SlopeUL:
				m_velocity.x = 0.175f; // 左に少し移動させる
				break;

			case CubeParam::eCubeType::SlopeUR:
				m_velocity.x = -0.175f; // 右に少し移動させる
				break;

			default:
				break;
			}
		}
	}

	// スパイクと衝突した瞬間
	void Player::SpikeEnter(const shared_ptr<GameObject>& obj, const Vec3& hitPos)
	{
		// 無敵中ならブロックとして扱う
		if (m_status(eStatus::IsInvincible))
		{
			// ブロックとの衝突処理を送る
			BlockEnter(obj, hitPos);
			return;
		}

		// スパイクオブジェクトにキャスト
		const auto& spike = dynamic_pointer_cast<Spike>(obj);

		// パラメータの取得
		Vec3 spikePos = spike->GetPosition();
		Vec3 scale = spike->GetScale();
		Vec3 helfScale = spike->GetScale() / 2.0f;

		// 衝突方向真偽
		const auto& groupVec = GetStage()->GetSharedObjectGroup(L"Update")->GetGroupVector();
		bool upper, under, left, right;
		upper = CollHitUpper(hitPos, spikePos, helfScale) && !ObjectCheck(groupVec, Vec3(spikePos + Vec3(0.0f, scale.y, 0.0f)));
		under = CollHitUnder(hitPos, spikePos, helfScale) && !ObjectCheck(groupVec, Vec3(spikePos + Vec3(0.0f, -scale.y, 0.0f)));
		left = CollHitLeft(hitPos, spikePos, helfScale) && !ObjectCheck(groupVec, Vec3(spikePos + Vec3(-scale.x, 0.0f, 0.0f)));
		right = CollHitRight(hitPos, spikePos, helfScale) && !ObjectCheck(groupVec, Vec3(spikePos + Vec3(scale.x, 0.0f, 0.0f)));

		// スパイクの方向を取得
		const auto& angle = spike->GetAngle();
		switch (angle)
		{
		case eAngle::Up: // 上向きの棘に

			// 上・左・右から衝突したら
			if (upper || left || right) 
			{
				// ダメージ処理を送る
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}

			// 下から衝突したら
			if (under)
			{
				// ブロックとの処理を送る
				BlockUnderHit(spikePos, helfScale);
				return;
			}
			break;

		case eAngle::Down: // 下向きの棘に

			// 下・左・右から衝突したら
			if (under || left || right)
			{
				// ダメージ処理を送る
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}

			// 上から衝突したら
			if (upper)
			{
				// ブロックとの処理を送る
				BlockUpperHit(spikePos, helfScale);
				return;
			}
			break;

		case eAngle::Left: // 左向きの棘に

			// 上・下・左から衝突したら
			if (upper || under || left)
			{
				// ダメージ処理を送る
				DamageKnockBack(Vec2(1.5f, -0.5f));
				return;
			}

			// 右から衝突したら
			if (right)
			{
				// ブロックとの処理を送る
				BlockRightHit(spikePos, helfScale);
				return;
			}
			break;

		case eAngle::Right: // 右向きの棘に

			// 上・下・右から衝突したら
			if (upper || under || right)
			{
				// ダメージ処理を送る
				DamageKnockBack(Vec2(-1.5f, -0.5f));
				return;
			}

			// 左から衝突したら
			if (left)
			{
				// ブロックとの処理を送る
				BlockLeftHit(spikePos, helfScale);
				return;
			}
			break;

		case eAngle::All: // 全方位の棘に
			if (upper) // 上から
			{
				DamageKnockBack(Vec2(0.9f, -1.0f));
				return;
			}
			if (under) // 下から
			{
				DamageKnockBack(Vec2(0.9f, 1.0f));
				return;
			}
			if (left) // 左から
			{
				DamageKnockBack(Vec2(1.5f, -0.5f));
				return;
			}
			if (right) // 右から
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
		if (angle == eAngle::Down)
		{
			// エアショック発射可能にする
			m_status.Set(eStatus::IsFirePossible) = true;

			// ブロックとの接地処理
			BlockExcute(obj, hitPos);
		}
		else
		{
			// 無敵時間中じゃなければ棘との衝突処理を送る
			if (!m_status(eStatus::IsInvincible))
			{
				SpikeEnter(obj, hitPos);
			}
		}
	}

	// 大砲と衝突した瞬間
	void Player::CannonEnter(const shared_ptr<GameObject>& cannon)
	{
		// 大砲型にキャスト
		const auto& ptr = dynamic_pointer_cast<Cannon>(cannon);
		if (ptr)
		{
			// 保持している大砲ポインタと違ったら
			if (m_activeCannon.lock() != ptr)
			{
				// 発射準備関数
				ptr->OnFire();

				// 乗った大砲のポインタを保持
				m_activeCannon.reset();
				m_activeCannon = ptr;

				// ステータスの設定
				m_status.Set(eStatus::IsCannonStandby) = true;
				m_status.Set(eStatus::IsAir, eStatus::IsFirePossible) = false;

				// 実質移動不可に設定
				m_velocity = Vec2(0.0f);
				m_acsel = 1.0f;
			}
		}
	}

	// ベルトコンベアと衝突した瞬間
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

			// 接地状態にする
			m_status.Set(eStatus::IsFirePossible) = true;
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

			// 向きに応じて移動量Xに速度を設定
			switch (angle)
			{
			case Convayor::eRotType::LeftRot:
				m_velocity.x = speed;
				break;

			case Convayor::eRotType::RightRot:
				m_velocity.x = -speed;
				break;

			default:
				break;
			}

			return;
		}

		// 上以外から衝突したならブロックとの衝突処理を送る
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

	// バンパーに衝突した瞬間
	void Player::BumperEnter(const shared_ptr<GameObject>& other, const Vec3& hitPos)	
	{
		// 型キャスト
		const auto& bumper = dynamic_pointer_cast<Bumper>(other);
		if (bumper)
		{
			// バンパーの座標と自身の座標から角度を取得
			const Vec3& bumperPos = bumper->GetPosition();
			float rad = -atan2f(hitPos.y - bumperPos.y, hitPos.x - bumperPos.x);

			// 角度から移動方向を設定
			Vec2 velo = Vec2(-cos(rad), sin(rad)).normalize();

			// パラメータの設定
			m_velocity = velo * m_speed; // 移動量を移動方向と速度で決定
			m_acsel = m_maxAcsel; 		 // 加速度を最大加速度に設定
			m_status.Set(eStatus::IsAir) = true; // 空中かの真偽をオンに

			// バンパーに衝突反応処理を送る
			bumper->OnHit();
		}
	}
		
	// 敵のウサギと衝突した瞬間
	void Player::RabbitEnter(const shared_ptr<GameObject>& other, const Vec3& hitPos)
	{			
		// ウサギ型にキャスト
		const auto& rabbit = dynamic_pointer_cast<Rabbit>(other);
		if (rabbit)
		{
			// 無敵時間中じゃない時
			if (!m_status(eStatus::IsInvincible))
			{
				// 大砲発射後なら
				if (m_status(eStatus::IsCannonFire))
				{
					// ウサギのステートを死亡に設定
					rabbit->SetState(Rabbit::eState::Death);
					StartSE(L"RABBIT_DEATH_SE", 0.5f);
					return;
				}

				// シールドがあるなら
				if (m_shieldCount > 0)
				{
					// ウサギのステートを死亡に設定
					rabbit->SetState(Rabbit::eState::Death);
					StartSE(L"RABBIT_DEATH_SE", 0.5f);
				}

				// 衝突処理
				float knockBackValue = (rabbit->GetPosition().x > hitPos.x) ? 1.5f : -1.5f;
				DamageKnockBack(Vec2(knockBackValue, -0.5f));
			}
		}
	}

	// 敵のウサギと衝突し続けた時
	void Player::RabbitExcute(const shared_ptr<GameObject>& rabbit, const Vec3& hitPos)
	{
		// 無敵中じゃなければ
		if (!m_status(eStatus::IsInvincible))
		{
			// ウサギと衝突した時の関数を実行
			RabbitEnter(rabbit, hitPos);
		}
	}

	// ダメージノックバック
	void Player::DamageKnockBack(const Vec2& velocity)
	{
		// パラメータの設定
		m_status.Set(eStatus::IsAir) = true;
		m_status.Set(eStatus::IsFirePossible) = false;
		m_velocity = velocity;
		m_acsel = m_damageAcsel;

		// アニメーションをダメージ状態にする
		m_bodyDraw->ChangeCurrentAnimation(L"DAMAGE");

		// 軌道の表示をオフ
		m_arrow.lock()->SetDrawActive(false);

		// 無敵中じゃなければ
		if (!m_status(eStatus::IsInvincible))
		{
			// シールドの数が1枚以上なら
			if (m_shieldCount >= 1)
			{
				// シールドを減らし、無敵時間にする
				m_shieldCount--;
				m_status.Set(eStatus::IsInvincible) = true;

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

	// リングに衝突した瞬間
	void Player::RingEnter(const shared_ptr<GameObject>& ring)
	{
		// 型キャスト
		const auto& ringPtr = dynamic_pointer_cast<Ring>(ring);
		if (ringPtr)
		{
			// リングに取得状態を送る
			ringPtr->IsGetRing();

			// リングのサイズに応じて取得処理をする
			const auto& size = ringPtr->GetRingSize();
			switch (size)
			{
			case Ring::eRingSize::Big: // 大きいリングなら
				
				AddShield(); // シールドを追加
				StartSE(L"RING_SE", 0.75f); // SEの再生
				break;

			case Ring::eRingSize::Small: // 小さいリングなら

				GetSmallRing(); // 小リング取得処理を送る
				StartSE(L"RING_SE", 0.35f); // SEの再生
				break;

			default:
				break;
			}

			// 小さいリングの取得個数が25個以上になったら
			if (m_shieldLimit > m_shieldCount && m_sRingCount >= m_sRingLimit.at(m_shieldCount))
			{
				// シールドを増やしてカウンタをリセット
				AddShield();
				m_sRingCount = 0;
			}
		}
	}

	// 動く壁に衝突した瞬間
	void Player::MoveWallEnter(const shared_ptr<GameObject>& wall, const Vec3& hitPos)
	{
		// ブロックのパラメータを取得
		const auto& moveWall = dynamic_pointer_cast<MoveWall>(wall);
		Vec3 objPos = moveWall->GetPosition();
		Vec3 helf = moveWall->GetScale() / 2.0f;

		// 動く壁オブジェクト配列等の取得
		vector<weak_ptr<GameObject>> groupVec = GetStage()->GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
		Vec3 leftPos = GetMoveWallPos(groupVec, objPos, LEFT_VEC);
		Vec3 rightPos = GetMoveWallPos(groupVec, objPos, RIGHT_VEC);
		Vec3 upperPos = GetMoveWallPos(groupVec, objPos, UP_VEC);
		Vec3 underPos = GetMoveWallPos(groupVec, objPos, DOWN_VEC);

		// 座標の設定
		Vec3 pos = GetPosition();
		Vec3 cornerUL, cornerDR;

		// 右から衝突した時の処理
		// 衝突した動く壁の左側に別の動く壁がなければ
		if (!ObjectCheck(groupVec, objPos + LEFT_VEC))
		{
			// 動く壁の角を設定
			cornerUL = Vec3(-helf.x * 1.25f, helf.y, 0.0f);
			cornerDR = Vec3(0.0f, -helf.y, 0.0f);

			// 動く壁の範囲内で衝突したなら
			if (GetBetween(hitPos, upperPos + cornerUL, underPos + cornerDR))
			{
				// 自身の高さが動くオブジェクトの高さ以下なら
				if (pos.y <= objPos.y + helf.y)
				{
					// 壁が動いていたら
					if (moveWall->GetMoveRatio() > 0.0f)
					{
						// パラメータの設定
						m_status.Set(eStatus::IsHitMoveBlock) = true;
						m_status.Set(eStatus::IsAliveMoveBlock) = false;
						m_currentWall = moveWall;
					}
				}
			}
		}

		// 左から衝突した時の処理
		// 衝突した動く壁の右側に別の動く壁がなければ
		if (!ObjectCheck(groupVec, objPos + RIGHT_VEC))
		{
			// 動く壁の角を設定
			cornerUL = Vec3(helf.x * 1.25f, helf.y, 0.0f);
			cornerDR = Vec3(0.0f, -helf.y, 0.0f);

			// 動く壁の範囲内で衝突したなら
			if (GetBetween(hitPos, upperPos + cornerUL, underPos + cornerDR))
			{
				// 自身の高さが動くオブジェクトの高さ以下なら
				if (pos.y <= objPos.y + helf.y)
				{
					// 壁が動いていたら
					if (moveWall->GetMoveRatio() > 0.0f)
					{
						// パラメータの設定
						m_status.Set(eStatus::IsHitMoveBlock) = true;
						m_status.Set(eStatus::IsAliveMoveBlock) = false;
						m_currentWall = moveWall;
					}
				}
			}
		}

		// 上から衝突した時の処理
		// 衝突した動く壁の上側に別の動く壁がなければ
		if (!ObjectCheck(groupVec, objPos + UP_VEC))
		{
			// 動く壁の角を設定
			cornerUL = Vec3(-helf.x, helf.y * 1.25f, 0.0f);
			cornerDR = Vec3(helf.x, 0.0f, 0.0f);

			// 動く壁の範囲内で衝突したなら
			if (GetBetween(hitPos, leftPos + cornerUL, rightPos + cornerDR))
			{
				// パラメータの設定
				m_velocity.y = 0.25f;
				m_acsel = 1.0f;
				m_status.Set(eStatus::IsAir) = false;

				// 接地処理
				pos.y = objPos.y + moveWall->GetScale().y;
				SetPosition(pos);

				// 左右から動く壁に衝突していなければ
				if (!m_status(eStatus::IsHitMoveBlock))
				{
					// 動く壁の上に居る状態にする
					m_status.Set(eStatus::IsAliveMoveBlock) = true;
					m_currentWall = moveWall;
				}
			}
		}

		// 下から衝突した時の処理
		// 衝突した動く壁の下側に別の動く壁がなければ
		if (!ObjectCheck(groupVec, objPos + DOWN_VEC))
		{
			// 動く壁の角を設定
			cornerUL = Vec3(-helf.x, 0.0f, 0.0f);
			cornerDR = Vec3(helf.x, -helf.y * 1.25f, 0.0f);

			// 動く壁の範囲内で衝突したなら
			if (GetBetween(hitPos, leftPos + cornerUL, rightPos + cornerDR))
			{
				if (!m_status(eStatus::IsHitMoveBlock))
				{
					UnderCompressedDeath();
				}
			}
		}

		// 圧死してないかの処理
		// 動く壁の角を設定
		cornerUL = Vec3(-helf.x, -helf.y, 0.0f);
		cornerDR = Vec3(helf.x, helf.x, 0.0f);

		// 動く壁の中に居たら
		if (GetBetween(pos, objPos + cornerUL, objPos + cornerDR))
		{
			// 圧死処理を送る
			m_status.Set(eStatus::IsAliveMoveBlock) = true;
			UnderCompressedDeath();
		}
	}

	// 動く壁から離れた時
	void Player::MoveWallExit()
	{
		bool reset = false; // リセットするかの真偽
		Vec3 wallPos = m_currentWall.lock()->GetPosition(); // 最後に衝突した動く壁の座標

		// 距離の比較
		float length = (GetPosition() - wallPos).length();
		if (length >= 2.0f)
		{
			// 衝突真偽と上に居るかの真偽をオフ
			m_status.Set(eStatus::IsHitMoveBlock, eStatus::IsAliveMoveBlock) = false;
			reset = true;
		}

		// 壁が動いているか
		const auto& wall = dynamic_pointer_cast<MoveWall>(m_currentWall.lock());
		if (wall->GetMoveRatio() <= 0.0f)
		{
			// 衝突真偽をオフ
			m_status.Set(eStatus::IsHitMoveBlock) = false;
			reset = true;
		}

		// リセットするなら
		if (reset)
		{
			// 保持を解除
			m_currentWall.reset();
		}
	}
}