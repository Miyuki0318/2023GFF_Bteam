/*!
@file Bumper.cpp
@brief ステージギミックの動く壁の開閉用ボタン
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void Button::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Gimmick::OnCreate();
		
		// コリジョンをアクティブにし、応答処理をオフにする
		m_ptrColl->SetUpdateActive(true);
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BUTTON");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"PUSH", 0, 30, false);
		m_ptrDraw->AddAnimation(L"REVERSE", 30, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"REVERSE");
	}

	// 毎フレーム更新処理
	void Button::OnUpdate()
	{
		// 衝突してるかを取得
		m_active = GetHitButton();

		// 入力時間を設定
		m_reverseTime = min(1.0f - m_ptrDraw->GetCurrentAnimationTime(), 1.0f);

		// 入力状態が前回と相違であれば
		if (m_active != m_current)
		{
			// アニメーションを変更し、入力時間から開始
			m_ptrDraw->ChangeCurrentAnimation(m_active ? L"PUSH" : L"REVERSE", m_reverseTime);
		}

		// アニメーションの更新
		m_ptrDraw->UpdateAnimation(DELTA_TIME * 2.0f);

		// 入力状態の保持
		m_current = m_active;
	}
}