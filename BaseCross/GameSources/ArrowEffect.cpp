/*!
@file ArrowEffect.cpp
@brief プレイヤーのエアジェットでの予測軌道
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void ArrowEffect::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();

		// プレイヤーのポインタがあれば
		if (m_player.lock())
		{
			// プレイヤーの座標に自身の座標を設定
			SetPosition(m_player.lock()->GetPosition());
			SetScale(1.0f); // スケールの設定

			// 頂点データの設定(リボン型メッシュ)
			VertexData tempVertex; // 
 			vector<Vec3> points; // 座標群
			points.resize(40); // 節目を40個に設定する為
			RibonVerticesIndices(points, tempVertex, Vec3(0.0, 0.0f, 1.0f), 0.25f, 1);

			// 描画コンポーネントの設定
			m_ptrDraw = AddComponent<PCTStaticDraw>();
			m_ptrDraw->SetOriginalMeshUse(true);
			m_ptrDraw->CreateOriginalMesh(tempVertex);
			m_ptrDraw->SetTextureResource(L"ARROW_TX");
			m_ptrDraw->SetSamplerState(SamplerState::LinearWrap);
			m_ptrDraw->SetEmissive(COL_WHITE);
			m_ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.5f));

			SetDrawActive(false); // 描画をオフ
			SetAlphaActive(true); // 透明色の描画を可能に
		}
	}

	// エフェクトの更新処理
	void ArrowEffect::UpdateEffect(const vector<Vec3>& points)
	{
		// プレイヤーのポインタがあれば
		if (m_player.lock())
		{
			// プレイヤー型にキャスト
			const auto& player = dynamic_pointer_cast<Player>(m_player.lock());
			if (player)
			{
				// プレイヤーの座標に自身の座標を設定
				SetPosition(player->GetPosition());

				// リボン型メッシュの再設定をできれば
				if (RibonVerticesIndices(points, m_vertex, Vec3(0.0, 0.0f, 1.0f), 0.35f, 1))
				{
					SetDrawActive(true); // 描画をオン
					m_ptrDraw->UpdateVertices(m_vertex.vertices); // 頂点データの更新
				}
				else
				{
					SetDrawActive(false); // 描画をオフ
				}
			}
		}
	}
}