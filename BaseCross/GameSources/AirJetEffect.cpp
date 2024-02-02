#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void AirJetEffect::OnCreate()
	{
		Billboard::OnCreate();
		SetDrawLayer(3);
	}

	void AirJetEffect::UpdateEffect()
	{
		if (m_ownerObject.lock())
		{
			const auto& player = dynamic_pointer_cast<Player>(m_ownerObject.lock());
			if (player)
			{
				const float& acsel = player->GetAcsel();
				const Vec2& velo = player->GetVelocity();
				Vec3 pos = player->GetPosition() + (Vec3(velo.x, velo.y, 0.0f).normalize() * (acsel * 1.15f));
				pos.y += 0.5f;
				pos.z = -1.0f;

				SetPosition(pos);
				SetRotation(0.0f, 0.0f, (atan2f(velo.y, velo.x) - XM_PIDIV2));
				SetScale(Vec2((acsel - 1.0f) * 3.0f));
			}
		}
	}
}