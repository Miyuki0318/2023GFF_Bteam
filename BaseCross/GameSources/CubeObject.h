#pragma once
#include "stdafx.h"
#include "DebugObject.h"

namespace basecross
{
	class CubeObject : public DebugObject
	{
	public:

		enum eType
		{
			Normal,
			Left,
			Right,
			SlopeUL,
			SlopeUR,
			SlopeDL,
			SlopeDR,
		};

	protected:

		eType m_type;
		vector<Vec3> m_angle;
		const float m_collRange;
		const bool m_collActive;
		weak_ptr<GameObject> m_targetObj;

	public:

		CubeObject(const shared_ptr<Stage>& stagePtr) :
			DebugObject(stagePtr),
			m_collRange(5.0f),
			m_collActive(true)
		{
			m_type = Normal;
			m_angle = {
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 45.0f),
				Vec3(0.0f, 0.0f, -45.0f),
				Vec3(0.0f, 0.0f, 45.0f),
				Vec3(0.0f, 180.0f, 45.0f),
				Vec3(0.0f, 0.0f, 135.0f),
				Vec3(0.0f, 180.0f, 135.0f),
			};
		}

		CubeObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale, const bool active
		) :
			DebugObject(stagePtr, position, rotation, scale),
			m_collRange(4.0f),
			m_collActive(active)
		{
			m_type = Normal;
			m_angle = {
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 45.0f),
				Vec3(0.0f, 0.0f, -45.0f),
				Vec3(0.0f, 0.0f, 45.0f),
				Vec3(0.0f, 180.0f, 45.0f),
				Vec3(0.0f, 0.0f, 135.0f),
				Vec3(0.0f, 180.0f, 135.0f),
			};
		}

		CubeObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& scale, const eType& type, const bool active
		) :
			DebugObject(stagePtr, position, Vec3(0.0f), scale),
			m_type(type),
			m_collRange(4.0f),
			m_collActive(active)
		{
			m_angle = {
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 45.0f),
				Vec3(0.0f, 0.0f, -45.0f),
				Vec3(0.0f, 0.0f, 45.0f),
				Vec3(0.0f, 180.0f, 45.0f),
				Vec3(0.0f, 0.0f, 135.0f),
				Vec3(0.0f, 180.0f, 135.0f),
			};
		}

		virtual ~CubeObject() {}

		void OnCreate() override;

		void OnUpdate() override;

		const eType& GetAngleType() const
		{
			return m_type;
		}

		void SetTarget(const shared_ptr<GameObject>& objPtr)
		{
			m_targetObj = objPtr;
		}

		const shared_ptr<GameObject>& GetTarget() const
		{
			return m_targetObj.lock();
		}

		const Vec3& GetSlopePos() const
		{
			if (Utility::GetBetween(m_type, SlopeUL, SlopeDR))
			{
				Vec3 slope;

				switch (m_type)
				{
				case SlopeUL:
					slope = Vec3(0.5f, -0.5f, 0.0f);
					break;

				case SlopeUR:
					slope = Vec3(-0.5f, -0.5f, 0.0f);
					break;

				case SlopeDL:
					slope = Vec3(0.5f, 0.5f, 0.0f);
					break;

				case SlopeDR:
					slope = Vec3(-0.5f, 0.5f, 0.0f);
					break;

				default:
					break;
				}

				return m_position + slope;
			}
			else
			{
				return m_position;
			}
		}
	};
}