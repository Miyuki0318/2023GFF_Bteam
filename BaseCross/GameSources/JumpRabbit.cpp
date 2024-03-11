/*!
@file JumpRabbit.cpp
@brief ジャンプするメカウサギ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void JumpRabbit::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Rabbit::OnCreate();

		// 発見時のエフェクトを生成
		const auto& stage = GetStage();
		m_discovered = stage->AddGameObject<Billboard>(L"DISCOVER_TX", Vec2(2.0f), Vec3(0.0f));
		m_discovered.lock()->SetDrawActive(false);
	}

	// 毎フレーム更新処理
	void JumpRabbit::OnUpdate()
	{
		MoveRabbit();    // 移動更新処理
		MoveReduction(); // 移動量減衰処理

		// アニメーションの更新
		m_ptrDraw->UpdateAnimation(DELTA_TIME / 2.0f);

		// ジャンプ可能かの真偽
		bool jumpPossible = m_ptrDraw->IsTargetAnimeEnd() && !m_status(eStatus::IsAir, eStatus::IsDeath);
		if (jumpPossible)
		{
			// アニメーションの設定
			m_ptrDraw->ChangeCurrentAnimation(L"JUMP");

			// ステートで分岐
			switch (m_state)
			{
			case eState::Patrol: // パトロール
				PatrolState(); // パトロールステート
				break;

			case eState::Seek: // 追跡
				SeekState(); // 追跡ステート
				break;

			case eState::LostSight: // 見失なった
				LostState(); // 見失なった時のステート
				break;

			default:
				break;
			}
		}

		// 大砲時
		if (m_state == eState::CannonJump && !m_status(eStatus::IsDeath))
		{
			CannonState(); // 大砲ステート
		}

		// 死亡時
		if (m_state == eState::Death)
		{
			DeathState(); // 死亡ステート
		}

		// 配列の初期化
		m_aliveBlockPos.clear();
	}

	// 破棄時の処理
	void JumpRabbit::OnDestroy()
	{
		// 発見時のエフェクトを非表示
		m_discovered.lock()->SetDrawActive(false);

		// ステージから発見時のエフェクトを破棄
		const auto& stage = GetStage();
		stage->RemoveGameObject<Billboard>(m_discovered.lock());
		m_discovered.reset();
	}

	// コリジョンに衝突したら
	void JumpRabbit::OnCollisionEnter(const CollisionPair& Pair)
	{
		// 衝突したオブジェクトと衝突座標を取得
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		// タグがブロックまたは、棘なら
		if (other->FindTag(L"Block") || other->FindTag(L"Spike"))
		{
			// ブロックと衝突した瞬間の処理を実行
			BlockEnter(other, hitPoint);
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

		// タグが大砲なら
		if (other->FindTag(L"Cannon"))
		{
			// 大砲と衝突した瞬間の処理を実行
			CannonEnter(other);
		}

		// タグが死亡するコリジョンなら
		if (other->FindTag(L"Death"))
		{
			// ステートを変更
			m_state = eState::Death;

			// 死亡しているなら
			if (m_status(eStatus::IsDeath))
			{
				// 自身をステージから破棄する
				const auto& stage = GetStage();
				stage->RemoveGameObject<Rabbit>(GetThis<Rabbit>());
			}
		}

		// タグがウサギなら
		if (other->FindTag(L"Rabbit"))
		{
			// 移動量を半減しつつ反転させる
			m_velocity.x *= -1.0f;
			m_dir *= -1.0f;
		}
	}

	// コリジョンに衝突し続けたら
	void JumpRabbit::OnCollisionExcute(const CollisionPair& Pair)
	{
		// 衝突したオブジェクトと衝突座標を取得
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		// タグがブロックまたは、棘なら
		if (other->FindTag(L"Block") || other->FindTag(L"Spike"))
		{			
			// ブロックに衝突し続けた時の処理を実行
			BlockExcute(other, hitPoint);
		}

		// タグがベルトコンベアなら
		if (other->FindTag(L"Convayor"))
		{
			// ベルトコンベアに衝突し続けた時の処理を実行
			ConvayorExcute(other, hitPoint);
		}
	}

	// コリジョンとの衝突が終わった時
	void JumpRabbit::OnCollisionExit(const CollisionPair& Pair)
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

	// 移動処理
	void JumpRabbit::MoveRabbit()
	{
		// 継承元の移動処理を実行
		Rabbit::MoveRabbit();

		// エフェクトのビルボード表示
		m_discovered.lock()->SetPosition(m_position + Vec3(0.0f, 2.5f, 0.0f));
		m_discovered.lock()->UpdateBillboard();
	}

	// ブロックに衝突した瞬間
	void JumpRabbit::BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// 死亡しているなら自身を破棄
		if (m_status(eStatus::IsDeath)) GetStage()->RemoveGameObject<Rabbit>(GetThis<Rabbit>());

		// ブロックのパラメータを取得
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		// 配列にブロックの座標を追加
		m_aliveBlockPos.push_back(objPos);

		// 大砲状態を解除
		m_status.Set(eStatus::IsCannon) = false;

		// コリジョンに対して上から衝突
		if (CollHitUpper(hitPos, objPos, helf))
		{
			// 移動量が下方向にあり
			if (m_velocity.y > 0.0f)
			{
				// 加速度が半分より大きかったら
				if (m_acsel > 2.5f || m_velocity.y > 2.5f)
				{
					// 移動量を反転させ、半分にする
					m_velocity.y *= -0.5f;
				}
			}
		}

		// 下から衝突
		if (CollHitUnder(hitPos, objPos, helf))
		{
			// 移動量が上方向なら
			if (m_velocity.y < 0.0f)
			{
				// 反転させ、落下させる
				m_velocity.y *= -1.0f;
			}
		}

		// 左右から衝突	
		if (CollHitLeft(hitPos, objPos, helf) || CollHitRight(hitPos, objPos, helf))
		{
			// キューブタイプが通常ブロックなら
			const auto& angle = cube->GetCubeType();
			if (angle == CubeParam::eCubeType::Normal)
			{
				// 移動量を半減しつつ反転させる
				m_velocity.x *= -0.5f;
				m_dir *= -1.0f;
			}
		}
	}

	// ブロックに衝突し続けたら
	void JumpRabbit::BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos)
	{
		// ブロックのパラメータを取得
		const auto& cube = dynamic_pointer_cast<CubeObject>(block);
		Vec3 objPos = cube->GetSlopePos();
		Vec3 helf = cube->GetScale() / 2.0f;

		// 配列にブロックの座標を追加
		m_aliveBlockPos.push_back(objPos);

		// 移動量が下方向にあり
		if (m_velocity.y > 0.0f)
		{
			// 接地状態にする
			m_acsel = 1.0f;
			m_status.Set(eStatus::IsAir) = false;

			// キューブタイプが通常タイプなら
			const auto& angle = cube->GetCubeType();
			if (angle == CubeParam::eCubeType::Normal)
			{
				// 接地応答処理する
				SetPosition(GetPosition().x, objPos.y + helf.y + (m_scale.y / 2.0f) + 0.05f);
			}
		}
	}

	// ベルトコンベアと衝突した瞬間
	void JumpRabbit::ConvayorEnter(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
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

		// 上以外から衝突したなら
		if (CollHitUnder(hitPos, objPos, helf) || CollHitLeft(hitPos, objPos, helf) || CollHitRight(hitPos, objPos, helf))
		{
			BlockEnter(convayor, hitPos);
		}
	}

	// ベルトコンベアと衝突し続けた時
	void JumpRabbit::ConvayorExcute(const shared_ptr<GameObject>& convayor, const Vec3& hitPos)
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
	void JumpRabbit::BumperEnter(const shared_ptr<GameObject>& other, const Vec3& hitPos)
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

	// 大砲と衝突した瞬間
	void JumpRabbit::CannonEnter(const shared_ptr<GameObject>& other)
	{
		// 大砲型にキャスト
		const auto& ptr = dynamic_pointer_cast<Cannon>(other);
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

				// パラメータの設定
				m_status.Set(eStatus::IsAir, eStatus::IsDeath) = true;

				// 実質移動不可に設定
				m_velocity = Vec2(0.0f);
				m_acsel = 1.0f;

				// ステートを大砲ステートに変更
				m_state = eState::CannonJump;

				// 座標を大砲の座標で更新
				SetPosition(ptr->GetPosition());

				// コリジョンをオフに
				m_ptrColl->SetUpdateActive(false);
			}
		}
	}

	// パトロールステート
	void JumpRabbit::PatrolState()
	{
		// ターゲットのポインタがあれば
		if (m_targetObj.lock())
		{
			// プレイヤーサーチを掛ける
			if (SearchPlayer())
			{
				// 追跡ステートに変更
				m_state = eState::Seek;

				// 発見時のエフェクトを表示
				m_discovered.lock()->SetDrawActive(true);
				return;
			}

			// 発見時のエフェクトを非表示
			m_discovered.lock()->SetDrawActive(false);
			
			// ジャンプ移動させる
			JumpingRabbit();
		}
	}

	// 追跡ステート
	void JumpRabbit::SeekState()
	{
		// サーチを掛ける
		SearchPlayer();

		// 座標の取得
		Vec3 targetPos = m_targetObj.lock()->GetPosition();

		// 衝突座標のバッファ配列
		vector<Vec3> hitPos = GetHitBlockPos(targetPos);

		// 衝突した座標があったら
		if (!hitPos.empty())
		{
			// 衝突したブロック避ける様にジャンプする
			BlockTargetJump(targetPos);
		}
		else
		{
			// プレイヤーに向けてジャンプする
			PlayerTargetJump(targetPos + Vec3(0.0f, -1.0f, 0.0f));
		}
	}

	// 見失なった時のステート
	void JumpRabbit::LostState()
	{
		// 座標の取得
		Vec3 targetPos = m_currentTargetPos;
		const Vec3& pos = GetPosition();

		// 見失った座標が射程距離内、または見失なってからのジャンプ数が5回よら多かったら
		if ((targetPos - pos).length() <= m_range || m_lostJumpCount > 5)
		{
			// サーチを掛ける
			if (SearchPlayer())
			{
				// ステートを追跡ステートに変更
				m_state = eState::Seek;
				m_lostJumpCount = 0; // ジャンプ回数をリセット
				return;
			}
			else
			{
				// ステートをパトロールステートに変更
				m_state = eState::Patrol;
				m_lostJumpCount = 0; // ジャンプ回数をリセット
				return;
			}
		}

		// 衝突座標のバッファ配列
		vector<Vec3> hitPos = GetHitBlockPos(targetPos);

		// 衝突した座標があったら
		if (!hitPos.empty())
		{
			// ブロックを避ける用にジャンプ移動する
			BlockTargetJump(targetPos);
			m_lostJumpCount++; // ジャンプ回数を加算
		}
		else
		{
			// ターゲットに向けてジャンプ移動する
			PlayerTargetJump(targetPos);
			m_lostJumpCount++; // ジャンプ回数を加算
		}
	}

	// 大砲ステート
	void JumpRabbit::CannonState()
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
				m_status.Set(eStatus::IsAir) = true; // 空中状態に
				m_state = eState::Patrol; // パトロールステートに変更

				// 大砲のZ軸をラジアンに変換し、移動量を設定する
				float rad = m_activeCannon.lock()->GetRotation().z - XM_PIDIV2;
				Vec2 velo = Vec2(cos(rad), sin(rad)).normalize() * 3.5f;

				// 移動処理を実行
				SetMoveValue(velo, 10.0f);

				// SEを再生
				StartSE(L"CANNON_SE", 0.75f);

				// コリジョン判定をアクティブに
				GetComponent<CollisionObb>()->SetUpdateActive(true);

				// 大砲のポインタを初期化
				m_activeCannon.reset();
			}
			else
			{
				// 移動不可状態に
				m_velocity = Vec2(0.0f);

				// 座標を大砲の座標で更新
				SetPosition(cannon->GetPosition());
			}
		}
	}

	// 死亡ステート
	void JumpRabbit::DeathState()
	{
		// 死亡していなければ
		if (!m_status(eStatus::IsDeath))
		{
			// 死亡した判定にする
			m_status.Set(eStatus::IsDeath) = true;

			// コリジョンコンポーネントとタグを削除
			RemoveComponent<CollisionObb>();
			RemoveTag(L"Rabbit");

			// 落下処理用の移動量と加速度に設定
			m_velocity = Vec2(Utility::RangeRand(-1.0f, 1.0f), -1.5f);
			m_acsel = m_maxAcsel;
		}
		else
		{
			// 回転処理
			SetRotation(GetRotation() + Vec3(0.0f, 0.0f, DELTA_TIME * m_dir * Utility::RangeRand(15.0f, 5.0f)));
		}
	}

	// プレイヤーサーチ
	bool JumpRabbit::SearchPlayer()
	{
		// ターゲットのポインタがあるなら
		if (m_targetObj.lock())
		{
			// 座標の取得
			Vec3 targetPos = m_targetObj.lock()->GetPosition();
			Vec3 pos = GetPosition();

			// 距離が射程距離内なら
			float length = (targetPos - pos).length();
			if (length <= m_range)
			{
				// プレイヤーまでとの間にブロックが無ければ
				if (GetHitBlockPos(targetPos).empty())
				{
					// 比較用に高さを除外
					targetPos.y = 0.0f;
					pos.y = 0.0f;

					// ターゲットの方向を求める
					Vec3 toTarget = -(targetPos - pos);
					toTarget.normalize();

					// 自身の向きと一致したら「発見」
					if (m_dir == toTarget.x)
					{
						// 発見時のSEを再生して終了
						StartSE(L"RABBIT_SEARCH_SE", 0.5f);
						return true;
					}
				}
				return false;
			}
			else
			{
				// 追跡ステートなら
				if (m_state == eState::Seek)
				{
					// 見失ったステートに変更
					m_state = eState::LostSight;

					// 見失った時の座標を保持
					m_currentTargetPos = m_targetObj.lock()->GetPosition();
				}
				return false;
			}
		}
		return false;
	}

	// ジャンプ移動
	void JumpRabbit::JumpingRabbit()
	{
		// ジャンプ移動量と自身が乗ってるブロックを取得
		Vec2 jumpVelo = m_jumpVelo;
		Vec3 alive = GetNearPosition(m_aliveBlockPos);

		// 方向に応じて処理
		if (m_dir > 0.0f)
		{
			bool check = false; // 跳べるかの真偽

			// ジャンプ移動先座標の数ループ
			for (const auto& jump : m_jumpTargetPos.at(0))
			{
				// 着地先があるならtrueにして終了
				check = BlockCheck(alive + jump);
				if (check) break;
			}

			// 着地先が無ければ反転
			if (!check) m_dir *= -1.0f;

			jumpVelo.x *= m_dir; // ジャンプ時の移動量に向きを掛ける
			m_status.Set(eStatus::IsAir) = true; // 空中状態に

			// 移動設定関数を実行
			SetMoveValue(jumpVelo, m_maxAcsel);
		}
		else
		{
			bool check = false; // 跳べるかの真偽

			// ジャンプ移動先座標の数ループ
			for (const auto& jump : m_jumpTargetPos.at(1))
			{
				// 着地先があるならtrueにして終了
				check = BlockCheck(alive + jump);
				if (check) break;
			}

			// 着地先が無ければ反転
			if (!check) m_dir *= -1.0f;

			jumpVelo.x *= m_dir; // ジャンプ時の移動量に向きを掛ける
			m_status.Set(eStatus::IsAir) = true; // 空中状態に

			// 移動設定関数を実行
			SetMoveValue(jumpVelo, m_maxAcsel);
		}
	}

	// ターゲットに向けてジャンプ
	void JumpRabbit::PlayerTargetJump(const Vec3& targetPos)
	{
		// 角度から移動量を設定
		const Vec3& pos = GetPosition();
		float rad = atan2f(targetPos.y - pos.y, targetPos.x - pos.x);
		Vec2 velo = Vec2(cos(rad), sin(rad)).normalize();
		velo *= -m_speed; // 移動量を速度で乗算
		velo.y += -1.0f;  // 少し上を狙う

		// 空中状態にする
		m_status.Set(eStatus::IsAir) = true;

		// 移動設定関数を実行
		SetMoveValue(velo, m_maxAcsel);
	}

	// ブロックを跳び越える用にジャンプ
	void JumpRabbit::BlockTargetJump(const Vec3& targetPos)
	{
		// 最接近座標の取得
		Vec3 nearPos = GetNearPosition(GetHitBlockPos(targetPos));

		// 衝突した最接近のブロックの上にブロックがあるかをチェック
		// ブロックがあった場合さらにその上にあるかをチェック
		// ただし、7つ以上あったら到達できないと判断して終了
		int loopCount = 0;
		while (BlockCheck(nearPos + UP_VEC))
		{
			nearPos += UP_VEC;
			loopCount++;
			if (loopCount > 7)
			{
				// 到達不可なのでパトロールステートに
				m_state = eState::Patrol;
				return;
			}
		}

		// 跳び越える用にジャンプする
		PlayerTargetJump(nearPos);
	}

	// ジャンプ時のSE
	void JumpRabbit::StartJumpSE()
	{
		// 座標を取得
		const Vec3& pos = GetPosition();
		const Vec3& playerPos = m_targetObj.lock()->GetPosition();

		// 距離を射程距離と比較
		float length = (playerPos - pos).length();
		if (length <= m_range)
		{
			// 距離から音量を設定して再生
			float volume = (m_range - length) / 10.0f;
			StartSE(L"RABBIT_JUMP_SE", volume);
		}
	}

	// プレイヤーまでとの線分とブロックが衝突したら衝突したブロックの座標を返す
	const vector<Vec3> JumpRabbit::GetHitBlockPos(const Vec3& targetPos)
	{
		// バッファ配列
		vector<Vec3> posVec;

		// 座標の取得
		Vec3 pos = GetPosition();

		// ブロックオブジェクトグループの取得
		const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Active")->GetGroupVector();
		for (const auto& gameObj : blockVec)
		{
			// オブジェクトが無い、または非アクティブかのチェック
			if (!gameObj.lock()) continue;
			if (!gameObj.lock()->GetUpdateActive()) continue;

			// OBBを取得し、OBBと線分で衝突判定をする
			const auto& objPtr = gameObj.lock();
			const auto& collObb = objPtr->GetComponent<CollisionObb>();
			if (!collObb) continue;

			// 衝突したらバッファに座標を追加
			if (HitTest::SEGMENT_OBB(pos, targetPos, collObb->GetObb()))
			{
				const auto& block = dynamic_pointer_cast<CubeObject>(objPtr);
				posVec.push_back(block->GetSlopePos());
			}
		}

		// 配列を返す
		return posVec;
	}
}