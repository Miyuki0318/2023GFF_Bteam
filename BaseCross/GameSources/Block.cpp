/*!
@file block.cpp
@brief ブロックのコリジョンと描画
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void Alpha::OnCreate()
	{
		// 継承元の開始時の処理を送る
		CubeObject::OnCreate();

		// タグを追加
		AddTag(L"Block");
	}

	// 生成時の処理
	void DeathColl::OnCreate()
	{
		// 継承元の開始時の処理を送る
		CubeObject::OnCreate();

		// タグを追加
		AddTag(L"Death");
	}

	// 生成時の処理
	void StagingColl::OnCreate()
	{
		// 継承元の開始時の処理を送る
		CubeObject::OnCreate();

		// タグを追加
		AddTag(L"Block");
	}

	// 毎フレーム更新処理
	void StagingColl::OnUpdate()
	{
		// ステージステートを取得
		const auto& stageState = GetStageState<GameStage>();

		// コリジョンを開始時の演出に応じてアクティブかを切り替える
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(stageState != GameStage::StartMove);
	}

	// 生成時の処理
	void InstanceBlock::OnCreate()
	{
		// 描画コンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		ptrDraw->SetMeshResource(L"BLOCK");
		ptrDraw->SetTextureResource(m_textures.at(toSize_t(m_type)));

		// パラメータの設定
		const float under = -98.0f;		// ステージの下限
		const float left = -49.0f;		// ステージの左限
		const float scale = 1.0f;		// スケール
		const float modelScale = 1.35f;	// モデルのスケール

		// 配列のサイズ分ループ
		for (size_t i = 0; i < m_data.size(); i++)
		{
			// 数値が0以外なら
			if (m_data.at(i) != 0)
			{
				// 座標の設定
				float x = left + i;	// 左限からループ数を足した数値
				float y = under + (m_size - m_rowNum); // 下限から行サイズ - 行数を足した数値

				Mat4x4 matrix, mtxT, mtxR, mtxS; // 行列の宣言
				
				// クォータニオンからローテーションを設定
				Quat quat;
				quat.rotationRollPitchYawFromVector(Vec3(0.0f, 0.0f, 0.0f));
				mtxR.rotation(quat);
				mtxS.scale(Vec3(modelScale)); // スケールの設定
				mtxT.translation(Vec3(x, y, 0.0f)); // 座標の設定

				// 行列の設定と追加
				matrix = mtxS * mtxR * mtxT;
				ptrDraw->AddMatrix(matrix);
			}
		}
	}

	// 生成時の処理
	void InstanceSlope::OnCreate()
	{
		// 描画コンポーネントの設定
		auto ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		ptrDraw->SetMeshResource(L"SLOPE");
		ptrDraw->SetTextureResource(m_textures.at(toSize_t(m_type)));
		
		// パラメータの設定
		const float under = -97.5f;	// ステージの下限
		const float left = -49.0f;  // ステージの左限
		const Vec3 slopeScale = Vec3(1.375f); // スロープのスケール

		// 配列のサイズ分ループ
		for (size_t i = 0; i < m_data.size(); i++)
		{
			// 数値が0以外なら
			if (m_data.at(i) != 0)
			{
				// 座標の設定
				float x = left + i;	// 左限からループ数を足した数値
				float y = under + (m_size - m_rowNum); // 下限から行サイズ - 行数を足した数値

				// スロープのタイプを取得
				CubeParam::eCubeType slope = static_cast<CubeParam::eCubeType>(m_data.at(i) + 2);

				Mat4x4 matrix, mtxT, mtxR, mtxS; // 行列の宣言
				Quat quat; // クォータニオンの宣言

				// クォータニオンからローテーションの設定
				quat.rotationRollPitchYawFromVector(DegToRad(CubeParam::GetAngle(slope)));

				mtxR.rotation(quat); // ローテーション
				mtxS.scale(slopeScale); // スケール
				mtxT.translation(Vec3(x, y, 0.0f) + CubeParam::GetSlopeDiff(slope)); // 座標

				// 行列の設定と追加
				matrix = mtxS * mtxR * mtxT;
				ptrDraw->AddMatrix(matrix);
			}
		}
	}
}