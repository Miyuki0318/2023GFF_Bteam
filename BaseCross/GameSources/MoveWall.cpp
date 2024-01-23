#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void MoveWall::OnCreate()
	{
		Gimmick::OnCreate();
		
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"BLOCK");
		m_ptrDraw->SetTextureResource(L"METAL_TX");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		
		m_movePoint += Vec3(0.0f, -1.0f, 0.0f) * (m_moveLength * 2.0f);

		AddTag(L"MoveWall");
	}

	void MoveWall::OnUpdate()
	{
		Gimmick::OnUpdate();

		if (m_buttons.empty())
		{
			SetTargetButton();
		}
		else
		{
			MoveWallBlock(m_startPos, m_movePoint);
		}
	}

	void MoveWall::SetTargetButton()
	{
		const auto& gimmickVec = GetStage()->GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
		for (const auto& gimmick : gimmickVec)
		{
			if (!gimmick.lock()) continue;

			const auto& button = dynamic_pointer_cast<Button>(gimmick.lock());
			if (!button) continue;

			if (button->GetButtonNumber() == m_number)
			{
				m_buttons.push_back(button);
			}
		}
	}

	void MoveWall::MoveWallBlock(const Vec3& start, const Vec3& end)
	{
		bool input = false;
		for (const auto& button : m_buttons)
		{
			if (button.lock()->GetInput())
			{
				input = true;
				break;
			}
		}

		m_moveRatio += input ? DELTA_TIME * m_moveSpeed : -DELTA_TIME;
		m_moveRatio = min(m_moveRatio, m_moveSpeed);
		m_moveRatio = max(m_moveRatio, 0.0f);

		Vec3 pos = Utility::Lerp(start, end, m_moveRatio / m_moveSpeed);
		SetPosition(pos);
	}
}