/*!
@file MoveWall.cpp
@brief ステージギミックの動く壁
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void MoveWall::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Gimmick::OnCreate();
		
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"BLOCK");
		m_ptrDraw->SetTextureResource(L"METAL_TX");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		
		// 動く方向に応じて設定
		switch (m_moveType)
		{
		// 下方向なら
		case MoveWall::eMoveType::Down:
		case MoveWall::eMoveType::DownSE:

			// 下方向に開閉距離分の座標を設定
			m_movePoint += DOWN_VEC * (m_moveLength * 2.0f);
			break;

		// 上方向なら
		case MoveWall::eMoveType::Up:
		case MoveWall::eMoveType::UpSE:

			// 上方向に開閉距離分の座標を設定
			m_movePoint += UP_VEC * (m_moveLength * 2.0f);
			break;

		// 左方向なら
		case MoveWall::eMoveType::Left:
		case MoveWall::eMoveType::LeftSE:

			// 左方向に開閉距離分の座標を設定
			m_movePoint += LEFT_VEC * (m_moveLength * 2.0f);
			break;

		// 右方向なら
		case MoveWall::eMoveType::Right:
		case MoveWall::eMoveType::RightSE:

			// 右方向に開閉距離分の座標を設定
			m_movePoint += RIGHT_VEC * (m_moveLength * 2.0f);
			break;

		default:
			break;
		}

		// タグを設定
		AddTag(L"MoveWall");
	}

	// 毎フレーム更新処理
	void MoveWall::OnUpdate()
	{
		// 継承元の毎フレーム更新処理を実行
		Gimmick::OnUpdate();

		// 開閉用のボタンが未登録なら
		if (m_buttons.empty())
		{
			// ボタンの登録を行う
			SetTargetButton();
		}
		else
		{
			// 開閉処理を実行
			MoveWallBlock(m_startPos, m_movePoint);
		}

		// 動くタイプがSE付きなら
		if (m_moveType >= eMoveType::DownSE)
		{
			// ゲーム中じゃないなら
			if (GetStageState<GameStage>() != GameStage::GameNow)
			{
				StopSE(L"METAL_SE"); // SEの停止
			}
		}
	}

	// 更新処理を行うかの設定
	void MoveWall::SetUpdateActive(bool b)
	{
		// 継承元の更新処理を行うかの設定を実行
		TemplateObject::SetUpdateActive(b);

		// 更新処理を停止するならSEも停止させる
		if (!b) StopSE(L"METAL_SE");
	}

	// 開閉用のボタン登録
	void MoveWall::SetTargetButton()
	{
		// ギミックグループの配列を取得
		const auto& gimmickVec = GetStage()->GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
		
		// オブジェクトの数ループ
		for (const auto& gimmick : gimmickVec)
		{
			// エラーチェック
			if (!gimmick.lock()) continue;

			// ボタン型にキャスト
			const auto& button = dynamic_pointer_cast<Button>(gimmick.lock());
			if (!button) continue;

			// ボタンの識別ナンバーと一致したら
			if (button->GetButtonNumber() == m_number)
			{
				// ボタン配列に追加
				m_buttons.push_back(button);
			}
		}
	}

	// 開閉時の移動関数
	void MoveWall::MoveWallBlock(const Vec3& start, const Vec3& end)
	{
		bool input = false; // 開閉用ボタンの入力があるかの真偽

		// ボタンオブジェクトの数ループ
		for (const auto& button : m_buttons)
		{
			// ボタンの入力があれば
			if (button.lock()->GetInput())
			{
				// 入力真偽をtrueにしてループを終了
				input = true;
				break;
			}
		}

		// 移動量を入力の有無で設定
		m_moveRatio += input ? DELTA_TIME * m_moveSpeed : -DELTA_TIME;
		m_moveRatio = min(m_moveRatio, m_moveSpeed);
		m_moveRatio = max(m_moveRatio, 0.0f);

		// SE付きなら
		if (m_moveType >= eMoveType::DownSE)
		{
			// 移動量が前回と相違なら
			if (m_moveRatio != m_currentRatio)
			{
				// 移動量が開始値または終了値なら
				if (m_moveRatio == 0.0f || m_moveRatio == m_moveSpeed)
				{
					// 移動時のSEを停止し、停止時のSEを再生
					StopSE(L"METAL_SE");
					StartSE(L"METAL_STOP_SE", 0.75f);
				}
			}

			// ボタン入力が前回と相違なら
			if (m_currentInput != input)
			{
				// 移動時のSEを再生し直す
				StopSE(L"METAL_SE");
				StartSE(L"METAL_SE", 0.2f);
			}
		}

		// 移動量と入力真偽を保持
		m_currentRatio = m_moveRatio;
		m_currentInput = input;

		// 座標をLerpで設定し更新
		Vec3 pos = Utility::Lerp(start, end, m_moveRatio / m_moveSpeed);
		SetPosition(pos);
	}
}