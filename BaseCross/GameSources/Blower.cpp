#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Blower::OnCreate()
	{
		Gimmick::OnCreate();
		
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BLOWER");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");

		m_particle = GetStage()->AddGameObject<MultiParticle>();

		AddTag(L"Blower");
	}

	void Blower::OnUpdate()
	{
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	void Blower::EffectUpdate()
	{
	}

	bool Blower::CheckBetween(const Vec3& targetPos)
	{
		Vec3 pos = GetPosition();
		Vec3 scale = GetScale();
		Vec3 helf = GetScale() / 2.0f;

		Vec3 left, right;

		switch (m_angle)
		{
		case Gimmick::Up:
			left = Vec3(pos.x - scale.x, pos.y + helf.y, 0.0f);
			right = Vec3(pos.x + scale.x, pos.y + helf.y + m_length, 0.0f);
			break;
		
		case Gimmick::Down:
			left = Vec3(pos.x - scale.x, pos.y - helf.y - m_length, 0.0f);
			right = Vec3(pos.x + scale.x, pos.y - helf.y, 0.0f);
			break;
		
		case Gimmick::Left:
			left = Vec3(pos.x - helf.x - m_length, pos.y - scale.y, 0.0f);
			right = Vec3(pos.x - helf.x, pos.y + scale.y, 0.0f);
			break;
		
		case Gimmick::Right:
			left = Vec3(pos.x + helf.x, pos.y - scale.y, 0.0f);
			right = Vec3(pos.x + helf.x + m_length, pos.y + scale.y, 0.0f);
			break;

		default:
			break;
		}

		return Utility::GetBetween(targetPos, left, right);
	}
}