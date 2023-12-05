#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Alpha::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Block");
	}

	void DeathColl::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Death");
	}

	void StagingColl::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Block");
	}

	void StagingColl::OnUpdate()
	{
		const auto& stageState = GetTypeStage<GameStage>()->GetStageState();
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(stageState != GameStage::StartMove);
	}

	void InstanceBlock::OnCreate()
	{
		auto ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		ptrDraw->SetMeshResource(L"BLOCK");
		ptrDraw->SetTextureResource(m_textures.at(m_type));

		const float under = -98.0f;
		const float left = -49.0f;
		const float scale = 1.0f;
		const float modelScale = 1.35f;

		for (size_t i = 0; i < m_data.size(); i++)
		{
			if (m_data.at(i) != 0)
			{
				float x = left + i;
				float y = under + (m_size - m_rowNum);

				Mat4x4 matrix, mtxT, mtxR, mtxS;
				
				Quat q;
				q.rotationRollPitchYawFromVector(Vec3(0.0f, 0.0f, 0.0f));
				mtxR.rotation(q);
				mtxS.scale(Vec3(modelScale));
				mtxT.translation(Vec3(x, y, 0.0f));

				matrix = mtxS * mtxR * mtxT;
				ptrDraw->AddMatrix(matrix);
			}
		}
	}

	void InstanceSlope::OnCreate()
	{
		auto ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		ptrDraw->SetMeshResource(L"SLOPE");
		ptrDraw->SetTextureResource(m_textures.at(m_type));

		const float under = -98.0f;
		const float left = -49.0f;
		const Vec3 slopeScale = Vec3(1.37f);
		const Vec3 slopeULeft = Vec3(0.5f, 0.0f, 0.0f);
		const Vec3 slopeURight = Vec3(-0.5f, 0.0f, 0.0f);
		const Vec3 slopeDLeft = Vec3(0.5f, 1.0f, 0.0f);
		const Vec3 slopeDRight = Vec3(-0.5f, 1.0f, 0.0f);

		for (size_t i = 0; i < m_data.size(); i++)
		{
			if (m_data.at(i) != 0)
			{
				float x = left + i;
				float y = under + (m_size - m_rowNum);

				Mat4x4 matrix, mtxT, mtxR, mtxS;
				Quat q;

				switch (m_data.at(i))
				{
				case 1:
					q.rotationRollPitchYawFromVector(Utility::DegToRad(Vec3(0.0f, 0.0f, 45.0f)));
					mtxT.translation(Vec3(x, y, 0.0f) + slopeULeft);
					break;

				case 2:
					q.rotationRollPitchYawFromVector(Utility::DegToRad(Vec3(0.0f, 180.0f, 45.0f)));
					mtxT.translation(Vec3(x, y, 0.0f) + slopeURight);
					break;

				case 3:
					q.rotationRollPitchYawFromVector(Utility::DegToRad(Vec3(0.0f, 0.0f, 135.0f)));
					mtxT.translation(Vec3(x, y, 0.0f) + slopeDLeft);
					break;

				case 4:
					q.rotationRollPitchYawFromVector(Utility::DegToRad(Vec3(0.0f, 180.0f, 135.0f)));
					mtxT.translation(Vec3(x, y, 0.0f) + slopeDRight);
					break;

				default:
					break;
				}

				mtxR.rotation(q);
				mtxS.scale(slopeScale);

				matrix = mtxS * mtxR * mtxT;
				ptrDraw->AddMatrix(matrix);
			}
		}
	}
}