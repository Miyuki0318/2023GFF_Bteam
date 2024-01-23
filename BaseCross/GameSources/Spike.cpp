#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Spike::OnCreate()
	{
		Gimmick::OnCreate();
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(m_angle != All ? L"SPIKE" : L"SPIKE_BLOCK");
		m_ptrDraw->SetMeshToTransformMatrix(m_angle != All ? m_modelMat : m_blockMat);
		m_ptrDraw->SetEmissive(Col4(0.5f, 0.5f, 0.0f, 1.0f));
		m_ptrDraw->AddAnimation(L"ATTACK", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ATTACK");
		AddTag(L"Spike");
	}

	void Spike::OnUpdate()
	{
		CubeObject::OnUpdate();
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	void MoveSpike::OnCreate()
	{
		Spike::OnCreate();
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		switch (m_moveType)
		{
		case MoveSpike::UpDown:
			m_movePointA += Vec3(0.0f, 1.0f, 0.0f) * m_moveLength;
			m_movePointB += Vec3(0.0f, -1.0f, 0.0f) * m_moveLength;
			break;

		case MoveSpike::LeftRight:
			m_movePointA += Vec3(1.0f, 0.0f, 0.0f) * m_moveLength;
			m_movePointB += Vec3(-1.0f, 0.0f, 0.0f) * m_moveLength;
			break;

		default:
			break;
		}
	}

	void MoveSpike::OnUpdate()
	{
		Spike::OnUpdate();
		UpdateMove();
	}

	void MoveSpike::UpdateMove()
	{
		if (m_moveSpeed > 0.0f && m_moveLength > 0.0f)
		{
			if (m_moveState == StandBy)
			{
				StandbyState();
			}

			if (m_moveState == StartPos)
			{
				if (PointState(m_startPos, m_movePointA, 1.05f))
				{
					m_currentState = StartPos;
					m_moveState = StandBy;
				}
			}

			if (m_moveState == MoveA)
			{
				if (PointState(m_movePointB, m_movePointA, m_moveSpeed))
				{
					m_currentState = MoveA;
					m_moveState = StandBy;
				}
			}

			if (m_moveState == MoveB)
			{
				if (PointState(m_movePointA, m_movePointB, m_moveSpeed))
				{
					m_currentState = MoveB;
					m_moveState = StandBy;
				}
			}
		}
	}

	void MoveSpike::StandbyState()
	{
		if (SetTimer(1.0f))
		{
			switch (m_currentState)
			{
			case MoveSpike::StartPos:
				m_moveState = MoveB;
				break;

			case MoveSpike::MoveA:
				m_moveState = MoveB;
				break;

			case MoveSpike::MoveB:
				m_moveState = MoveA;
				break;

			default:
				break;
			}
		}
	}

	bool MoveSpike::PointState(const Vec3& start, const Vec3& end, float time)
	{
		bool achiev = false;
		float totalTime = GetTime(time) / time;
		Vec3 pos = Utility::Lerp(start, end, totalTime);
		if (totalTime >= 1.0f)
		{
			pos = end;
			achiev = true;
		}
		else
		{
			SetTimer(time);
		}

		SetPosition(pos);
		return achiev;
	}
}