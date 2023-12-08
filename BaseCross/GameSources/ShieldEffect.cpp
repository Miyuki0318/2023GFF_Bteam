#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void ShieldEffect::OnCreate()
	{
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		m_ptrDraw->SetTextureResource(L"SHIELD_TX");
		m_ptrDraw->SetDiffuse(Col4(0.5f, 1.0f, 1.0f, 0.75f));

		m_outLine = GetStage()->AddGameObject<ShieldOutLine>(GetThis<TemplateObject>());

		SetAlphaActive(true);
		SetDrawLayer(2);
	}

	void ShieldEffect::UpdateEffect()
	{
		if (m_ownerObject.lock())
		{
			const auto& player = dynamic_pointer_cast<Player>(m_ownerObject.lock());
			if (player)
			{
				if (m_outLine.lock())
				{
					const auto& outLine = dynamic_pointer_cast<ShieldOutLine>(m_outLine.lock());
					if (outLine)
					{
						// カメラに対しての角度ベクトルの取得
						const auto& ptrCamera = GetStage()->GetView()->GetTargetCamera();
						Vec3 eyePos = ptrCamera->GetEye();
						Vec3 pos = GetPosition();
						float angle = -atan2(pos.z - eyePos.z, pos.x - eyePos.x);
						Vec3 verticalVec = Vec3(-sin(angle + XM_PIDIV2)	* 2.0f, cos(angle + XM_PIDIV2), 0.0f);

						// 座標の設定
						const Vec3& playerPos = player->GetPosition();
						const float diffY = 0.5f;
						SetPosition(Vec3(playerPos.x, playerPos.y - diffY, -1.5f) + verticalVec);
						outLine->SetPosition(Vec3(playerPos.x, playerPos.y - diffY, -1.4f) + verticalVec);

						// スケールの設定
						const int& shieldCount = player->GetShieldCount();
						Vec2 scale = Vec2(1.5f + (0.5f * shieldCount));
						SetScale(scale);
						outLine->SetScale(scale + Vec2(0.05f));

						Col4 color = COL_NONE;
						switch (shieldCount)
						{
						case 1:
							color = Col4(0.0f, 0.0f, 1.0f, 0.25f);
							break;

						case 2:
							color = Col4(0.0f, 1.0f, 0.0f, 0.25f);
							break;

						case 3:
							color = Col4(1.0f, 0.0f, 0.0f, 0.25f);
							break;

						default:
							break;
						}

						outLine->GetComponent<PNTStaticDraw>()->SetDiffuse(color);
						color.w = 1.0f;
						outLine->GetComponent<PNTStaticDraw>()->SetEmissive(color);
					}
				}
			}
		}
	}

	void ShieldEffect::SetDrawShield(bool b)
	{
		// エラーチェック
		if (!m_outLine.lock()) return;

		// 型キャスト
		const auto& outLine = dynamic_pointer_cast<ShieldOutLine>(m_outLine.lock());

		// エラーチェック
		if (!outLine) return;

		// 描画表示非表示
		SetDrawActive(b);
		outLine->SetDrawActive(b);
	}

	void ShieldOutLine::OnCreate()
	{
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetRasterizerState(RasterizerState::CullFront);
		ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 0.25f));
		ptrDraw->SetEmissive(Col4(0.0f, 0.0f, 1.0f, 1.0f));
		SetAlphaActive(true);
	}
}