/*!
@file Spike.cpp
@brief ステージギミックの棘
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;
	using namespace GimmickAngle;

	// 生成時の処理
	void Spike::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Gimmick::OnCreate();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(m_angle != eAngle::All ? L"SPIKE" : L"SPIKE_BLOCK");
		m_ptrDraw->SetMeshToTransformMatrix(m_angle != eAngle::All ? m_modelMat : m_blockMat);
		m_ptrDraw->SetEmissive(Col4(0.5f, 0.5f, 0.0f, 1.0f));
		m_ptrDraw->AddAnimation(L"ATTACK", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ATTACK");

		// タグを設定
		AddTag(L"Spike");
	}

	// 毎フレーム更新処理
	void Spike::OnUpdate()
	{
		// 継承元の毎フレーム更新処理を実行
		CubeObject::OnUpdate();

		// アニメーションの更新
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// 生成時の処理
	void MoveSpike::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Spike::OnCreate();

		// コリジョンの押し出し処理を無効
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// 挙動タイプに応じてポイントを設定
		switch (m_moveType)
		{
		case eMoveType::UpDown: // 上下なら
			m_movePointA += UP_VEC * m_moveLength;   // 上方向ベクトルと移動距離で設定
			m_movePointB += DOWN_VEC * m_moveLength; // 下方向ベクトルと移動距離で設定
			break;

		case eMoveType::LeftRight: // 左右なら
			m_movePointA += RIGHT_VEC * m_moveLength; // 右方向ベクトルと移動距離で設定
			m_movePointB += LEFT_VEC * m_moveLength;  // 左方向ベクトルと移動距離で設定
			break;

		default:
			break;
		}
	}

	// 毎フレーム更新処理
	void MoveSpike::OnUpdate()
	{
		// 継承元の毎フレーム更新処理を実行
		Spike::OnUpdate();

		// ステートの更新処理を実行
		UpdateState();
	}

	// ステートの更新処理実行
	void MoveSpike::UpdateState()
	{
		// 速度と移動距離が0じゃなければ
		if (m_moveSpeed > 0.0f && m_moveLength > 0.0f)
		{
			// ステートが待機時なら
			if (m_moveState == eMoveState::StandBy)
			{
				// 待機時の処理を実行
				StandbyState();
			}

			// ステートが開始時の座標なら
			if (m_moveState == eMoveState::StartPos)
			{
				// ポイントAに移動
				if (PointState(m_startPos, m_movePointA, 1.05f))
				{
					// ステートの変更と保持
					m_currentState = eMoveState::StartPos;
					m_moveState = eMoveState::StandBy;
				}
			}

			// ステートがポイントAに移動なら
			if (m_moveState == eMoveState::MoveA)
			{
				// ポイントAに移動
				if (PointState(m_movePointB, m_movePointA, m_moveSpeed))
				{
					// ステートの変更と保持
					m_currentState = eMoveState::MoveA;
					m_moveState = eMoveState::StandBy;
				}
			}

			// ステートがポイントBに移動なら
			if (m_moveState == eMoveState::MoveB)
			{
				// ポイントBに移動
				if (PointState(m_movePointA, m_movePointB, m_moveSpeed))
				{
					// ステートの変更と保持
					m_currentState = eMoveState::MoveB;
					m_moveState = eMoveState::StandBy;
				}
			}
		}
	}

	// 待機時のステート
	void MoveSpike::StandbyState()
	{
		// 1秒のタイマー
		if (SetTimer(1.0f))
		{
			// 前回のステートに応じてステートを変更
			switch (m_currentState)
			{
			case eMoveState::StartPos: // 開始時の座標なら
				m_moveState = eMoveState::MoveB; // ポイントBに移動に設定
				break;

			case eMoveState::MoveA: // ポイントAに移動なら
				m_moveState = eMoveState::MoveB; // ポイントBに移動に設定
				break;

			case eMoveState::MoveB: // ポイントBに移動なら
				m_moveState = eMoveState::MoveA; // ポイントAに移動に設定
				break;

			default:
				break;
			}
		}
	}

	// ポイントに移動ステート
	bool MoveSpike::PointState(const Vec3& start, const Vec3& end, float time)
	{
		bool achiev = false; // 移動完了したかの変数

		float totalTime = GetTime(time) / time; // 経過時間を0.0～1.0の範囲する
		Vec3 pos = Utility::Lerp(start, end, totalTime); // Lerp関数で座標を求める

		// 経過時間が1.0なら
		if (totalTime >= 1.0f)
		{
			pos = end; // 座標を終了座標に
			achiev = true; // 終了真偽をtrueに
		}
		else
		{
			SetTimer(time); // タイマーを開始する
		}

		// 座標を更新
		SetPosition(pos);

		// 真偽を返す
		return achiev;
	}
}