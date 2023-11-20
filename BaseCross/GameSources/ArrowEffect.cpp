#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void ArrowEffect::OnCreate()
	{
		if (m_ownerObject.lock())
		{
			SetPosition(m_ownerObject.lock()->GetPosition());
			SetScale(1.0f);

			VertexData tempVertex;
			vector<Vec3> points;
			points.resize(40);
			Utility::RibonVerticesIndices(points, tempVertex, Vec3(0.0, 0.0f, 1.0f), 0.25f, 1);
			m_ptrDraw = AddComponent<PCTStaticDraw>();
			m_ptrDraw->SetOriginalMeshUse(true);
			m_ptrDraw->CreateOriginalMesh(tempVertex);
			m_ptrDraw->SetTextureResource(L"ARROW_TX");
			m_ptrDraw->SetSamplerState(SamplerState::LinearWrap);

			SetDrawActive(false);
			SetAlphaActive(true);
		}
	}

	void ArrowEffect::UpdateEffect(const vector<Vec3>& points)
	{
		if (m_ownerObject.lock())
		{
			const auto& player = dynamic_pointer_cast<Player>(m_ownerObject.lock());
			if (player)
			{
				SetPosition(player->GetPosition());

				Utility::RibonVerticesIndices(points, vertex, Vec3(0.0, 0.0f, 1.0f), 0.25f, 1);
				m_ptrDraw->UpdateVertices(vertex.vertices);
			}
		}
	}
}