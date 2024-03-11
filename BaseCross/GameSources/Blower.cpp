/*!
@file Blower.cpp
@brief ステージギミックの送風機
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;
	using namespace GimmickAngle;

	// 生成時の処理
	void Blower::OnCreate()
	{
		// 継承元の生成時の関数を実行する
		Gimmick::OnCreate();
		
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BLOWER");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");

		// タグの設定
		AddTag(L"Blower");
	}

	// 毎フレーム更新処理
	void Blower::OnUpdate()
	{
		// アニメーションの更新
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// ターゲットが範囲内に居るかの取得関数
	bool Blower::CheckBetween(const Vec3& targetPos) const
	{
		// 座標とスケールの取得
		Vec3 pos = GetPosition();
		Vec3 scale = GetScale();
		Vec3 helf = GetScale() / 2.0f;

		Vec3 left, right; // 範囲測定用の変数

		// 角度に応じて範囲を求める
		switch (m_angle)
		{
		case eAngle::Up: // 上向きなら
			left = Vec3(pos.x - scale.x, pos.y - helf.y, 0.0f); // 左端の座標を設定
			right = Vec3(pos.x + scale.x, pos.y + helf.y + m_range, 0.0f); // 右端の座標を設定
			break;
		
		case eAngle::Down: // 下向きなら
			left = Vec3(pos.x - scale.x, pos.y - helf.y - m_range, 0.0f); // 左端の座標を設定
			right = Vec3(pos.x + scale.x, pos.y + helf.y, 0.0f); // 右端の座標を設定
			break;
		
		case eAngle::Left: // 左向きなら
			left = Vec3(pos.x - helf.x - m_range, pos.y - scale.y, 0.0f); // 左端の座標を設定
			right = Vec3(pos.x + helf.x, pos.y + scale.y, 0.0f); // 右端の座標を設定
			break;
		
		case eAngle::Right: // 右向きなら
			left = Vec3(pos.x - helf.x, pos.y - scale.y, 0.0f); // 左端の座標を設定
			right = Vec3(pos.x + helf.x + m_range, pos.y + scale.y, 0.0f); // 右端の座標を設定
			break;

		default:
			break;
		}

		// 右端と左端からターゲットが範囲内に居るかを真偽
		return GetBetween(targetPos, left, right);
	}
}