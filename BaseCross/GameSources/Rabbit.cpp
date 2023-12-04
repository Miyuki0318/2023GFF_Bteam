#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Rabbit::OnCreate()
	{
		m_ptrTrans = GetComponent<Transform>();
		SetPosition(m_position);
		SetRotation(m_rotation);
		SetScale(m_scale);

		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"RABBIT");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"JUMP", 0, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"JUMP");

		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetDrawActive(true);

		const auto& stage = GetStage();
		m_debug = stage->AddGameObject<CubeObject>(Vec3(0.0f), Vec3(0.0f), Vec3(1.15f), false);
		auto debugDraw = m_debug.lock()->AddComponent<PNTStaticDraw>();
		debugDraw->SetMeshResource(L"DEFAULT_CUBE");
		debugDraw->SetEmissive(COL_RED);

		m_search = stage->AddGameObject<CubeObject>(Vec3(0.0f), Vec3(0.0f), Vec3(1.15f), false);
		auto searchDraw = m_search.lock()->AddComponent<PNTStaticDraw>();
		searchDraw->SetMeshResource(L"DEFAULT_CUBE");
		searchDraw->SetEmissive(COL_GREAN);

		m_targetObj = stage->GetSharedGameObject<DebugObject>(L"Player");
	}

	void Rabbit::OnUpdate()
	{
		m_ptrDraw->UpdateAnimation(DELTA_TIME / 2.0f);
		
		Vec3 temp;
		if (!m_aliveBlockPos.empty())
		{
			temp = m_aliveBlockPos.at(0);
		}

		Vec3 pos = GetPosition();

		for (const auto& alive : m_aliveBlockPos)
		{
			float lengthA = (temp - pos).length();
			float lengthB = (alive - pos).length();

			if (lengthA > lengthB)
			{
				temp = alive;
			}
		}

		m_debug.lock()->SetPosition(temp);
		m_search.lock()->SetPosition(m_currentTargetPos);

		MoveRabbit();
		MoveReduction();

		Debug::Log(L"ウサギのが空中か", m_isAir);
		Debug::Log(L"ウサギの座標", GetPosition());
		Debug::Log(L"ウサギの移動量", m_velocity);

		if (m_ptrDraw->IsTargetAnimeEnd() && !m_isAir)
		{
			m_ptrDraw->ChangeCurrentAnimation(L"JUMP");
			switch (m_state)
			{
			case Patrol:
				Debug::Log(L"パトロールステート");
				PatrolState();
				break;

			case Seek:
				Debug::Log(L"シークステート");
				SeekState();
				break;

			case LostSight:
				Debug::Log(L"ロストステート");
				LostState();
				break;

			default:
				break;
			}
		}

		m_aliveBlockPos.clear();
	}

	void Rabbit::OnCollisionEnter(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block"))
		{
			// ブロックのパラメータを取得
			const auto& cube = dynamic_pointer_cast<CubeObject>(other);
			Vec3 objPos = cube->GetSlopePos();
			Vec3 helf = cube->GetScale() / 2.0f;
			m_aliveBlockPos.push_back(objPos);

			if (CollHitUpper(hitPoint, objPos, helf))
			{
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

			if (CollHitLeft(hitPoint, objPos, helf) || CollHitRight(hitPoint, objPos, helf))
			{
				const auto& angle = cube->GetAngleType();
				if (angle == CubeObject::Normal)
				{
					// 移動量を半減しつつ反転させる
					m_velocity.x *= -0.5f;
					m_dir *= -1.0f;
				}
			}
		}
	}


	void Rabbit::OnCollisionExcute(const CollisionPair& Pair)
	{
		const shared_ptr<GameObject>& other = Pair.m_Dest.lock()->GetGameObject();
		const Vec3& hitPoint = Pair.m_CalcHitPoint;

		if (other->FindTag(L"Block"))
		{
			// ブロックのパラメータを取得
			const auto& cube = dynamic_pointer_cast<CubeObject>(other);
			Vec3 objPos = cube->GetSlopePos();
			Vec3 helf = cube->GetScale() / 2.0f;
			m_aliveBlockPos.push_back(objPos);

			if (m_velocity.y > 0.0f)
			{
				m_acsel = 1.0f;
				m_isAir = false;

				const auto& angle = cube->GetAngleType();
				if (angle == CubeObject::Normal)
				{
					SetPosition(GetPosition().x, objPos.y + helf.y + (m_scale.y / 2.0f) + 0.05f);
				}
			}
		}
	}


	void Rabbit::OnCollisionExit(const CollisionPair& Pair)
	{

	}


	void Rabbit::MoveRabbit()
	{
		// 向きの変更
		if (m_velocity.x != 0.0f)
		{
			m_dir = m_velocity.x > 0.0f ? 1.0f : -1.0f;
			float rotY = (m_velocity.x > 0.0f) * XM_PI;
			SetRotation(Vec3(0.0f, rotY, 0.0f));
		}

		// 現在の座標を取得
		m_position = m_ptrTrans->GetPosition();

		// ポジションに移動ベクトルと速度と加速度とデルタタイムで掛けた数を加算
		m_position.x += -m_velocity.x * m_speed * m_acsel * DELTA_TIME;
		m_position.y += -m_velocity.y * m_speed * m_acsel * DELTA_TIME;

		// 座標の更新
		m_ptrTrans->SetPosition(m_position);
	}

	void Rabbit::MoveReduction()
	{
		const float deltaTime = DELTA_TIME;

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

			if (m_velocity.y > 0.25f)
			{
				m_velocity.y -= deltaTime;
			}
			else
			{
				m_velocity.y = 0.25f;
			}
		}
	}

	void Rabbit::PatrolState()
	{
		if (m_targetObj.lock())
		{
			if (SearchPlayer())
			{
				m_state = Rabbit::Seek;
				return;
			}

			JumpRabbit();
		}
	}

	void Rabbit::SeekState()
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
			BlockTargetJump(targetPos);
		}
		else
		{
			PlayerTargetJump(targetPos);
		}
	}

	void Rabbit::LostState()
	{
		// 座標の取得
		Vec3 targetPos = m_currentTargetPos;
		const Vec3& pos = GetPosition();
		if ((targetPos - pos).length() <= 5.0f)
		{
			if (SearchPlayer())
			{
				m_state = Seek;
				return;
			}
			else
			{
				m_state = Patrol;
				return;
			}
		}

		// 衝突座標のバッファ配列
		vector<Vec3> hitPos = GetHitBlockPos(targetPos);

		// 衝突した座標があったら
		if (!hitPos.empty())
		{
			BlockTargetJump(targetPos);
		}
		else
		{
			PlayerTargetJump(targetPos);
		}
	}

	bool Rabbit::SearchPlayer()
	{
		if (m_targetObj.lock())
		{
			Vec3 targetPos = m_targetObj.lock()->GetPosition();
			targetPos.y = 0.0f;
			Vec3 pos = GetPosition();
			pos.y = 0.0f;

			float length = (targetPos - pos).length();
			if (length <= m_range)
			{
				Vec3 toTarget = -(targetPos - pos);
				toTarget.normalize();

				if (m_dir == toTarget.x)
				{
					return true;
				}
			}
			else
			{
				if (m_state == Seek)
				{
					m_state = LostSight;
					m_currentTargetPos = m_targetObj.lock()->GetPosition();
				}
				return false;
			}
		}
		return false;
	}

	void Rabbit::JumpRabbit()
	{
		Vec2 jumpVelo = m_jumpVelo;
		Vec3 alive = GetAlivePosition();

		if (m_dir > 0.0f)
		{
			bool check = false;
			for (const auto& jump : m_jumpTargetPos.at(0))
			{
				check = BlockCheck(alive + jump);
				if (check) break;
			}
			if (!check) m_dir *= -1.0f;

			jumpVelo.x *= m_dir;
			m_isAir = true;
			SetMoveValue(jumpVelo, m_maxAcsel);
		}
		else
		{
			bool check = false;
			for (const auto& jump : m_jumpTargetPos.at(1))
			{
				check = BlockCheck(alive + jump);
				if (check) break;
			}
			if (!check) m_dir *= -1.0f;

			jumpVelo.x *= m_dir;
			m_isAir = true;
			SetMoveValue(jumpVelo, m_maxAcsel);
		}
	}
	
	void Rabbit::PlayerTargetJump(const Vec3& targetPos)
	{
		// 角度から移動量を設定
		const Vec3& pos = GetPosition();
		float rad = atan2f(targetPos.y - pos.y, targetPos.x - pos.x);
		Vec2 velo = Vec2(cos(rad), sin(rad)).normalize();
		velo *= -3.5f;
		velo.y += -1.0f;
		m_isAir = true;
		SetMoveValue(velo, m_maxAcsel);
	}

	void Rabbit::BlockTargetJump(const Vec3& targetPos)
	{
		// 衝突座標のバッファ配列
		vector<Vec3> hitPos = GetHitBlockPos(targetPos);
		Vec3 nearPos = hitPos.at(0);
		Vec3 pos = GetPosition();

		// バッファ配列の中から一番近い座標を求める
		for (const auto& hit : hitPos)
		{
			float lengthA = (nearPos - pos).length();
			float lengthB = (hit - pos).length();

			// 比較
			if (lengthA > lengthB)
			{
				nearPos = hit;
			}
		}

		// 衝突した最接近のブロックの上にブロックがあるかをチェック
		// ブロックがあった場合さらにその上にあるかをチェック
		// ただし、5つ以上あったら到達できないと判断して終了
		int loopCount = 0;
		const Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
		while (BlockCheck(nearPos + up))
		{
			nearPos += up;
			loopCount++;
			if (loopCount > 5)
			{
				m_state = Patrol;
				break;
			}
		}

		PlayerTargetJump(nearPos);
	}

	const vector<Vec3> Rabbit::GetHitBlockPos(const Vec3& targetPos)
	{
		// バッファ配列
		vector<Vec3> posVec;

		// 座標の取得
		Vec3 pos = GetPosition();

		// ブロックオブジェクトグループの取得
		const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Stage")->GetGroupVector();
		for (const auto& gameObj : blockVec)
		{
			// オブジェクトが無い、または非アクティブかのチェック
			if (!gameObj.lock()) continue;
			if (!gameObj.lock()->GetUpdateActive()) continue;

			// 型キャストして、OBBを取得し、OBBと線分で衝突判定をする
			const auto& block = dynamic_pointer_cast<CubeObject>(gameObj.lock());
			if (!block) continue;
			const auto& collObb = block->GetComponent<CollisionObb>();
			if (!collObb) continue;

			// 衝突したらバッファに座標を追加
			if (HitTest::SEGMENT_OBB(pos, targetPos, collObb->GetObb()))
			{
				posVec.push_back(block->GetSlopePos());
			}
		}

		return posVec;
	}
}