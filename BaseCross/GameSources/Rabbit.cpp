/*!
@file Rabbit.cpp
@brief メカウサギ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void Rabbit::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();

		// トランスフォームの設定
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"RABBIT");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"JUMP", 0, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"JUMP");

		// コリジョンコンポーネントの追加
		m_ptrColl = AddComponent<CollisionObb>();
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// ステージからプレイヤーを取得し、ターゲットとして設定
		m_targetObj = GetStage()->GetSharedGameObject<TemplateObject>(L"Player");

		// タグを設定
		AddTag(L"Rabbit");
	}

	// 移動処理
	void Rabbit::MoveRabbit()
	{
		// 死んでおらず、移動量X軸が0以外であれば
		if (!m_status(eStatus::IsDeath) && m_velocity.x != 0.0f)
		{
			// 移動量X軸から向きを設定
			m_dir = m_velocity.x > 0.0f ? 1.0f : -1.0f;

			// 向きを元に、ローテーションY軸を設定
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

	// 移動量減衰処理
	void Rabbit::MoveReduction()
	{
		// デルタタイムを取得
		const float deltaTime = DELTA_TIME;

		// 空中なら
		if (m_status(eStatus::IsAir))
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

			// Y軸移動ベクトルが0.25より大きかったら
			if (m_velocity.y > 0.25f)
			{
				// Y軸移動ベクトルをデルタタイムで減算
				m_velocity.y -= deltaTime;
			}
			else
			{
				// 0.25より小さかったら0.25で修正
				m_velocity.y = 0.25f;
			}
		}
	}
}