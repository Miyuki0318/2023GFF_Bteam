#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Bumper::OnCreate()
	{
		TemplateObject::OnCreate();
		SetTransParam();

		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BUMPER");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"PUSH", 0, 30, false);
		m_ptrDraw->ChangeCurrentAnimation(L"PUSH");

		m_ptrColl = AddComponent<CollisionSphere>();
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		AddTag(L"Bumper");
	}

	void Bumper::OnUpdate()
	{
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	void MoveBumper::OnCreate()
	{
		Bumper::OnCreate();

		switch (m_moveType)
		{
		case MoveBumper::UpDown:
			m_movePointA += UP_VEC * m_moveLength;
			m_movePointB += DOWN_VEC * m_moveLength;
			break;

		case MoveBumper::LeftRight:
			m_movePointA += RIGHT_VEC * m_moveLength;
			m_movePointB += LEFT_VEC * m_moveLength;
			break;

		default:
			break;
		}
	}

	void MoveBumper::OnUpdate()
	{
		Bumper::OnUpdate();
		UpdateMove();
	}

	void MoveBumper::UpdateMove()
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

	void MoveBumper::StandbyState()
	{
		if (SetTimer(1.0f))
		{
			switch (m_currentState)
			{
			case MoveBumper::StartPos:
				m_moveState = MoveB;
				break;

			case MoveBumper::MoveA:
				m_moveState = MoveB;
				break;

			case MoveBumper::MoveB:
				m_moveState = MoveA;
				break;

			default:
				break;
			}
		}
	}

	bool MoveBumper::PointState(const Vec3& start, const Vec3& end, float time)
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