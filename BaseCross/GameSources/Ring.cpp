/*!
@file Ring.cpp
@brief ステージギミックのリング
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void Ring::OnCreate()
	{
		// サイズ倍率の設定
		float size = 1.0f;
		if (m_ringSize == eRingSize::Big) size = 5.0f;
		m_scale *= size;

		// 継承元の生成時の処理を実行
		Gimmick::OnCreate();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"RING");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 30, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");
		m_ptrDraw->SetEmissive(COL_WHITE);

		// コリジョンコンポーネントの設定
		m_ptrColl->SetMakedSize(m_ringSize == eRingSize::Big ? 1.0f : 1.75f);
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// タグを設定
		AddTag(L"Ring");
	}

	// 毎フレーム更新処理
	void Ring::OnUpdate()
	{
		// デルタタイムと取得時のアニメーションスケールの最大値を設定
		const float& deltaTime = DELTA_TIME;
		const float maxSize = m_ringSize == eRingSize::Big ? 7.5f : 1.5f;

		// ローテーションZ軸で回転
		Vec3 rot = GetRotation();
		rot.y -= deltaTime * m_rotSpeed;
		SetRotation(rot);

		// アニメーションの更新処理
		m_ptrDraw->UpdateAnimation(deltaTime);

		// 取得したなら
		if (m_isGet)
		{
			// 経過時間をデルタタイムで加算
			m_totalTime += deltaTime;

			// 経過時間と削除時間の割合
			float ratio = m_totalTime / m_deleteTime;

			// スケールをサインカーブで設定
			SetScale(SinCurve(ratio * DegToRad(235.0f), -2.5f, maxSize));

			// ディフューズ色をLerpで設定
			m_ptrDraw->SetDiffuse(Utility::Lerp(COL_WHITE, COL_ALPHA, ratio));

			// 経過時間が削除時間時間以上になったら
			if (m_deleteTime <= m_totalTime)
			{
				// ステージと自身のポインタを取得
				const auto& stage = GetStage();
				const auto& thisPtr = GetThis<Ring>();

				// 収集物グループから除外してオブジェクトを破棄
				stage->GetSharedObjectGroup(L"Collect")->OutoGroup(thisPtr);
				stage->RemoveGameObject<Ring>(thisPtr);
			}
		}
		else
		{
			// コリジョンのパフォーマンス関数を実行
			CollisionPerformance(maxSize);
		}
	}
}