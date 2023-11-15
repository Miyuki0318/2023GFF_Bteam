#pragma once
#include "stdafx.h"

namespace basecross
{
	class DebugObject : public GameObject
	{
	protected:

		Vec3 m_position;
		Vec3 m_rotation;
		Vec3 m_scale;

	public:

		DebugObject(const shared_ptr<Stage>& stagePtr
		) :
			GameObject(stagePtr)
		{
			m_position.zero();
			m_rotation.zero();
			m_scale.zero();
		}

		DebugObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale
		) :
			GameObject(stagePtr),
			m_position(position),
			m_rotation(rotation),
			m_scale(scale)
		{
		}

		virtual ~DebugObject() {}

		virtual void SetPosition(const Vec3& position)
		{
			m_position = position;
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetPosition(m_position);
		}

		virtual void SetPosition(const Vec2& position)
		{
			SetPosition(Vec3(position.x, position.y, 0.0f));
		}

		virtual void SetPosition(const float& x, const float& y, const float& z)
		{
			SetPosition(Vec3(x, y, z));
		}

		virtual void SetPosition(const float& x, const float& y)
		{
			SetPosition(Vec2(x, y));
		}

		virtual Vec3 GetPosition() const
		{
			return m_position;
		}

		virtual void SetRotation(const Vec3& rotation)
		{
			m_rotation = rotation;
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetRotation(m_rotation);
		}

		virtual void SetDegreeAngle(const Vec3& degree)
		{
			SetRotation(Utility::DegToRad(degree));
		}

		virtual Vec3 GetRotation() const
		{
			return m_rotation;
		}

		virtual Vec3 GetDegreeAngle() const
		{
			return Utility::RadToDeg(m_rotation);
		}

		virtual void SetScale(const Vec3& scale)
		{
			m_scale = scale;
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetScale(m_scale);
		}

		virtual void SetScale(const Vec2& scale)
		{
			SetScale(Vec3(scale.x, scale.y, 1.0f));
		}

		virtual void SetScale(const float& scale)
		{
			SetScale(Vec3(scale));
		}

		virtual Vec3 GetScale() const
		{
			return m_scale;
		}
	};
}