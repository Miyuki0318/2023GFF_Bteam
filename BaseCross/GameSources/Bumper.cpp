/*!
@file Bumper.cpp
@brief ステージギミックのバンパー
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void Bumper::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();
		SetTransParam(); // トランスフォームのパラメータを設定

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BUMPER");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"PUSH", 0, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"PUSH");

		// スフィアコリジョンの追加
		m_ptrColl = AddComponent<CollisionSphere>();
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// タグを設定
		AddTag(L"Bumper");
	}

	// 毎フレーム更新処理
	void Bumper::OnUpdate()
	{
		// アニメーションの更新処理
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// 生成時の処理
	void MoveBumper::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Bumper::OnCreate();

		// タイプに応じてポイントの設定を行う
		switch (m_moveType)
		{
		case eMoveType::UpDown: // 上下タイプなら
			m_movePointA += UP_VEC * m_moveLength;   // 上方向に距離を元に設定
			m_movePointB += DOWN_VEC * m_moveLength; // 下方向に距離を元に設定
			break;

		case eMoveType::LeftRight: // 左右タイプなら
			m_movePointA += RIGHT_VEC * m_moveLength; // 右方向に距離を元に設定
			m_movePointB += LEFT_VEC * m_moveLength;  // 左方向に距離を元に設定
			break;

		default:
			break;
		}
	}

	// 毎フレーム更新処理
	void MoveBumper::OnUpdate()
	{
		// 継承元の毎フレーム更新処理を実行
		Bumper::OnUpdate();

		// ステート更新を実行
		UpdateState();
	}

	// ステート更新処理
	void MoveBumper::UpdateState()
	{
		// 速度と距離が0じゃなければ
		if (m_moveSpeed > 0.0f && m_moveLength > 0.0f)
		{
			// ステートに応じて処理
			switch (m_moveState)
			{
			case eMoveState::StandBy: // 待機時なら
				
				// 待機時のステートを実行
				StandbyState();
				break;
			
			case eMoveState::StartPos: // 開始時の座標なら
				
				// 開始時の座標からポイントAに移動
				if (PointState(m_startPos, m_movePointA, 1.05f))
				{
					// ステートの変更と保持
					m_currentState = eMoveState::StartPos;
					m_moveState = eMoveState::StandBy;
				}
				break;
			
			case eMoveState::MoveA: // ポイントAに移動なら

				// ポイントAからポイントBに移動
				if (PointState(m_movePointB, m_movePointA, m_moveSpeed))
				{
					// ステートの変更と保持
					m_currentState = eMoveState::MoveA;
					m_moveState = eMoveState::StandBy;
				}
				break;
			
			case eMoveState::MoveB: // ポイントBに移動

				// ポイントBからポイントAに移動
				if (PointState(m_movePointA, m_movePointB, m_moveSpeed))
				{
					// ステートの変更と保持
					m_currentState = eMoveState::MoveB;
					m_moveState = eMoveState::StandBy;
				}
				break;
			
			default:
				break;
			}
		}
	}

	// 待機時のステート
	void MoveBumper::StandbyState()
	{
		// 1秒のタイマー
		if (SetTimer(1.0f))
		{
			// 前回のステートに応じて処理
			switch (m_currentState)
			{
			case eMoveState::StartPos: // 開始時の座標なら
				m_moveState = eMoveState::MoveB; // ステートを「ポイントBに移動」に変更
				break;

			case eMoveState::MoveA: // ポイントAに移動なら
				m_moveState = eMoveState::MoveB; // ステートを「ポイントBに移動」に変更
				break;

			case eMoveState::MoveB: // ポイントBに移動なら
				m_moveState = eMoveState::MoveA; // ステートを「ポイントAに移動」に変更
				break;

			default:
				break;
			}
		}
	}

	// 移動ステート
	bool MoveBumper::PointState(const Vec3& start, const Vec3& end, float time)
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