/*!
@file CubeObject.cpp
@brief コリジョンOBBを使用するオブジェクトの継承元
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;
	using namespace CubeParam;

	// 生成時の処理
	void CubeObject::OnCreate()
	{
		// 継承元の生成時の関数を実行する
		TemplateObject::OnCreate();

		// 座標とスケールはそのまま設定
		SetPosition(m_position);
		SetScale(m_scale);

		// ローテーションはノーマルタイプならm_rotationを、それ以外ならCubeParamのアングルを参照
		SetRotation(m_type != eCubeType::Normal ? DegToRad(GetAngle(m_type)) : m_rotation);

		// コリジョンがアクティブなら
		if (m_collActive)
		{
			// コリジョンOBBコンポーネントを追加
			m_ptrColl = AddComponent<CollisionObb>();
			m_ptrColl->SetFixed(true);
			m_ptrColl->SetUpdateActive(false);
		}

		// 透明色の描画を可能にする
		SetAlphaActive(true);
	}

	// 毎フレーム更新処理
	void CubeObject::OnUpdate()
	{
		// コリジョンパフォーマンス関数を実行
		CollisionPerformance(m_collRange);
	}

	// コリジョンパフォーマンス
	void CubeObject::CollisionPerformance(const float range)
	{
		// コリジョンがアクティブか
		if (m_collActive)
		{
			bool achieve = false; // 条件が達成したかの真偽

			// ターゲット配列の分ループ
			for (const auto& obj : m_targetObj)
			{
				// ターゲットの取得とエラーチェック
				const auto& target = obj.lock();
				if (!target) continue;

				// ターゲットの更新処理がアクティブか
				if (target->GetUpdateActive())
				{
					// ターゲットのトランスフォームコンポーネントを取得して座標を取得
					const auto& targetTrans = target->GetComponent<Transform>();
					Vec3 targetPos = targetTrans->GetPosition();

					// 自身とターゲットの座標の距離を求める
					float length = (targetPos - m_position).length();

					// コリジョン判定距離より近ければ条件達成として終了
					achieve = (length <= range);
					if (achieve) break;
				}
			}

			// 条件を元にコリジョンをアクティブか非アクティブかにする
			m_ptrColl->SetUpdateActive(achieve);
		}
	}
}