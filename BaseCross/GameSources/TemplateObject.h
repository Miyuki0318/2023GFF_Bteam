#pragma once
#include "stdafx.h"

namespace basecross
{
	class TemplateObject : public GameObject
	{
	protected:

		shared_ptr<Transform> m_ptrTrans;
		Vec3 m_position;
		Vec3 m_rotation;
		Vec3 m_scale;

	public:

		TemplateObject(const shared_ptr<Stage>& stagePtr
		) :
			GameObject(stagePtr)
		{
			m_position.zero();
			m_rotation.zero();
			m_scale.zero();
		}

		TemplateObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale
		) :
			GameObject(stagePtr),
			m_position(position),
			m_rotation(rotation),
			m_scale(scale)
		{
		}

		virtual ~TemplateObject() {}

		virtual void OnCreate() override
		{
			m_ptrTrans = GetComponent<Transform>();
		}

		template <typename T>
		const typename T::eStageState& GetStageState()
		{
			return GetTypeStage<T>()->GetStageState();
		}

		template <typename T>
		void SetStageState(const typename T::eStageState& state)
		{
			return GetTypeStage<T>()->SetStageState(state);
		}

		virtual void SetPosition(const Vec3& position)
		{
			m_position = position;
			m_ptrTrans->SetPosition(m_position);
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
			return m_ptrTrans->GetPosition();
		}

		virtual void SetRotation(const Vec3& rotation)
		{
			m_rotation = rotation;
			m_ptrTrans->SetRotation(m_rotation);
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
			m_ptrTrans->SetScale(m_scale);
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

		virtual void StartSE(const wstring& seKey, float volume)
		{
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			audioPtr->Start(seKey, 0, volume);
		}

		virtual bool BlockCheck(const Vec3& checkPos)
		{
			const auto& blockVec = GetStage()->GetSharedObjectGroup(L"Stage")->GetGroupVector();

			bool check = false;

			for (const auto& ptr : blockVec)
			{
				if (!ptr.lock()) continue;

				const auto& block = dynamic_pointer_cast<TemplateObject>(ptr.lock());
				if (!block) continue;

				Vec3 pos = block->GetPosition();

				if (pos == checkPos)
				{
					check = true;
				}
			}

			return check;
		}

		virtual bool BlockCheck(const vector<weak_ptr<GameObject>>& groupVec,const Vec3& checkPos)
		{
			bool check = false;

			for (const auto& ptr : groupVec)
			{
				if (!ptr.lock()) continue;

				const auto& block = dynamic_pointer_cast<TemplateObject>(ptr.lock());
				if (!block) continue;

				Vec3 pos = block->GetPosition();

				if (pos == checkPos)
				{
					check = true;
				}
			}

			return check;
		}

		// è„Ç©ÇÁè’ìÀÇµÇΩÇ©ÇÃåüèÿ
		virtual bool CollHitUpper(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		{
			return hitPos.y > hitObjPos.y && ((hitPos.y - hitObjPos.y) >= helfScale.y);
		}

		// â∫Ç©ÇÁè’ìÀÇµÇΩÇ©ÇÃåüèÿ
		virtual bool CollHitUnder(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		{
			return hitPos.y < hitObjPos.y && ((hitPos.y - hitObjPos.y) <= -helfScale.y);
		}

		// ç∂Ç©ÇÁè’ìÀÇµÇΩÇ©ÇÃåüèÿ
		virtual bool CollHitLeft(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		{
			return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
				&& ((hitPos.x - hitObjPos.x) < helfScale.x);
		}

		// âEÇ©ÇÁè’ìÀÇµÇΩÇ©ÇÃåüèÿ
		virtual bool CollHitRight(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale)
		{
			return ((hitPos.y - hitObjPos.y) < helfScale.y && (hitPos.y - hitObjPos.y) > -helfScale.y)
				&& ((hitPos.x - hitObjPos.x) > -helfScale.x);
		}
	};
}