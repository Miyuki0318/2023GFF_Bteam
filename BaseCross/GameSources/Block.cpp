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

	void Block::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Block");

		auto ptrDraw = AddComponent<PNTStaticDraw>();

		// ���b�V���̐ݒ�
		switch (m_type)
		{
		case CubeObject::Normal:
		case CubeObject::Left:
		case CubeObject::Right:
			ptrDraw->SetMeshResource(L"BLOCK");
			ptrDraw->SetMeshToTransformMatrix(m_normalMat);
			break;

		case CubeObject::SlopeL:
		case CubeObject::SlopeR:
			ptrDraw->SetMeshResource(L"SLOPE");
			ptrDraw->SetMeshToTransformMatrix(m_slopeMat);
			break;

		default:
			break;
		}

		// �e�N�X�`���̐ݒ�
		switch (m_block)
		{
		case Block::Iron:
			ptrDraw->SetTextureResource(L"GRASS_TX");
			break;

		case Block::Metal:
			ptrDraw->SetTextureResource(L"DIRT_TX");
			break;

		case Block::DarkMetal:
			ptrDraw->SetTextureResource(L"ROCK_TX");
			break;

		default:
			break;
		}
	}
}