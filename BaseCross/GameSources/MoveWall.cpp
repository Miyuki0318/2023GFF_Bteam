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
		
		switch (m_moveType)
		{
		case MoveWall::eMoveType::Down:
		case MoveWall::eMoveType::DownSE:
			m_movePoint += DOWN_VEC * (m_moveLength * 2.0f);
			break;

		case MoveWall::eMoveType::Up:
		case MoveWall::eMoveType::UpSE:
			m_movePoint += UP_VEC * (m_moveLength * 2.0f);
			break;

		case MoveWall::eMoveType::Left:
		case MoveWall::eMoveType::LeftSE:
			m_movePoint += LEFT_VEC * (m_moveLength * 2.0f);
			break;

		case MoveWall::eMoveType::Right:
		case MoveWall::eMoveType::RightSE:
			m_movePoint += RIGHT_VEC * (m_moveLength * 2.0f);
			break;

		default:
			break;
		}

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

		if (m_moveType >= DownSE)
		{
			if (GetStageState<GameStage>() != GameStage::GameNow)
			{
				StopSE(L"METAL_SE");
			}
		}
	}

	void MoveWall::SetUpdateActive(bool b)
	{
		TemplateObject::SetUpdateActive(b);

		if (!b) StopSE(L"METAL_SE");
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

		if (m_moveType >= DownSE)
		{
			if (m_moveRatio != m_currentRatio)
			{
				if (m_moveRatio == 0.0f || m_moveRatio == m_moveSpeed)
				{
					StopSE(L"METAL_SE");
					StartSE(L"METAL_STOP_SE", 0.75f);
				}
			}
			if (m_currentInput != input)
			{
				StopSE(L"METAL_SE");
				StartSE(L"METAL_SE", 0.2f);
			}
		}

		m_currentRatio = m_moveRatio;
		m_currentInput = input;

		Vec3 pos = Utility::Lerp(start, end, m_moveRatio / m_moveSpeed);
		SetPosition(pos);
	}
}