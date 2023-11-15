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

	void Slope::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Block");

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"SLOPE");
		ptrDraw->SetMeshToTransformMatrix(m_modelMat);

		// テクスチャの設定
		switch (m_block)
		{
		case Slope::Iron:
			ptrDraw->SetTextureResource(L"GRASS_TX");
			break;

		case Slope::Metal:
			ptrDraw->SetTextureResource(L"DIRT_TX");
			break;

		case Slope::DarkMetal:
			ptrDraw->SetTextureResource(L"ROCK_TX");
			break;

		default:
			break;
		}
	}

	void InstanceBlock::OnCreate()
	{
		auto ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		ptrDraw->SetMeshResource(L"BLOCK");
		ptrDraw->SetTextureResource(m_texture);

		const float under = -8.0f;
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
				mtxR.translation(Vec3(x, y, 0.0f));

				matrix = mtxS * mtxR * mtxT;
				ptrDraw->AddMatrix(matrix);
			}
		}
	}
}