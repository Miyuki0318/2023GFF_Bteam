/*!
@file WallRabbit.cpp
@brief 壁になってるメカウサギ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 毎フレーム更新処理
	void WallRabbit::OnUpdate()
	{
		// 死亡していなければ
		if (!m_status(eStatus::IsDeath))
		{
			// コリジョンのパフォーマンス関数を実行
			CollisiontPerformance();
		}

		// 死亡ステートなら
		if (m_state == eState::Death)
		{
			DeathState();    // 死亡時のステート処理を実行
			MoveRabbit();	 // 移動処理を実行
			MoveReduction(); // 移動量減衰処理を実行
		}
	}

	// 死亡時のステート
	void WallRabbit::DeathState()
	{
		// 死亡してなければ
		if (!m_status(eStatus::IsDeath))
		{
			// 死亡した判定にする
			m_status.Set(eStatus::IsDeath) = true;

			// 壁ウサギグループを取得
			const auto& wallVec = GetStage()->GetSharedObjectGroup(L"Wall")->GetGroupVector();

			// 自身の隣のウサギを探して「一緒に死の？」ってメンヘラ行動する
			// 自身の座標とスケールを取得
			Vec3 thisPos = GetPosition();
			Vec3 scale = GetScale();

			vector<weak_ptr<Rabbit>> rabbitVec; // 保存用配列

			// 壁ウサギの数ループ
			for (const auto& weakObj : wallVec)
			{
				// エラーチェック
				if (!weakObj.lock()) continue;

				// 型キャスト
				const auto& wall = dynamic_pointer_cast<Rabbit>(weakObj.lock());
				if (!wall) continue;
				
				// 既に死んでいたら無視
				if (wall->GetIsDeath()) continue;

				// 他の壁ウサギの座標を取得
				Vec3 pos = wall->GetPosition();

				// 自身の座標のスケールXを加算、または減算した座標に居れば
				if (pos == thisPos + Vec3(scale.x, 0.0f, 0.0f) || pos == thisPos - Vec3(scale.x, 0.0f, 0.0f))
				{
					rabbitVec.push_back(wall); // 配列に追加
				}

				// 自身の座標のスケールYを加算、または減算した座標に居れば
				if (pos == thisPos + Vec3(0.0f, scale.y, 0.0f) || pos == thisPos - Vec3(0.0f, scale.y, 0.0f))
				{
					rabbitVec.push_back(wall); // 配列に追加
				}
			}

			// 追加された壁ウサギの数ループ
			for (const auto& rabbit : rabbitVec)
			{
				// 念のためエラーチェック
				if (rabbit.lock())
				{
					// 死亡ステートに変更
					rabbit.lock()->SetState(eState::Death);
				}
			}

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

	// コリジョンパフォーマンス
	void WallRabbit::CollisiontPerformance()
	{
		bool active = false; // アクティブかの真偽

		// ターゲットのポインタがあれば
		if (m_targetObj.lock())
		{
			// 座標で距離を比較
			Vec3 targetPos = m_targetObj.lock()->GetPosition();
			float length = (targetPos - m_position).length();

			// 距離内ならアクティブにする
			active = (length <= 4.0f);
		}

		// コリジョンの設定
		m_ptrColl->SetUpdateActive(active);
	}
}