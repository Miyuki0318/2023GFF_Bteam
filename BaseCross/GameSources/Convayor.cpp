/*!
@file Convayor.cpp
@brief ステージギミックのベルトコンベア
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void Convayor::OnCreate()
	{
		// 継承元の生成時の関数を実行する
		Gimmick::OnCreate();

		// 描画コンポーネントの取得
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();

		// ベルトタイプに応じてメッシュの設定
		switch (m_type)
		{
		case eBeltType::Middle: // 内側なら
			m_ptrDraw->SetMultiMeshResource(L"MCONVAYOR"); // 内側用のメッシュキーを
			break;

		case eBeltType::Side: // 外側なら 
			m_ptrDraw->SetMultiMeshResource(L"SCONVAYOR"); // 外側用のメッシュキーを
			break;

		default:
			break;
		}

		// 描画コンポーネントの設定
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(m_animeKey.at(toSize_t(eRotType::LeftRot)), 0, 60, true);
		m_ptrDraw->AddAnimation(m_animeKey.at(toSize_t(eRotType::RightRot)), 70, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(m_animeKey.at(toSize_t(m_rotate)));

		// コリジョンコンポーネントの設定
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);

		// タグを設定
		AddTag(L"Convayor");
	}

	// 毎フレーム更新処理
	void Convayor::OnUpdate()
	{
		// 継承元の更新処理を実行
		CubeObject::OnUpdate();

		// アニメーションの更新
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// 生成時の処理
	void ConvayorGuide::OnCreate()
	{
		// 手前に移動させる
		m_position.z = -1.0f;

		// 継承元の生成時の処理を実行
		Billboard::OnCreate();

		// テクスチャの移動方向を設定
		SetVelocity(-1.0f, 0.0f);

		// 回転方向に応じて設定
		switch (m_rotate)
		{
		case Convayor::eRotType::LeftRot: // 左方向なら
			SetRotation(0.0f, 0.0f, XM_PI); // Z軸で180度回転
			SetVerticesColor(Col4(0.2f, 0.4f, 1.0f, 1.0f)); // 頂点色を水色に設定
			break;
		
		case  Convayor::eRotType::RightRot: // 右方向なら
			SetVerticesColor(Col4(1.0f, 0.65f, 0.15f, 1.0f)); // 頂点色を橙色に設定
			break;

		default:
			break;
		}
	}

	// 毎フレーム更新処理
	void ConvayorGuide::OnUpdate()
	{
		// テクスチャを回転処理
		Billboard::RotateTexture();
	}
}