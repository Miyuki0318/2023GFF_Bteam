#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Alpha::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Block");
	}

	void Grass::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Block");
		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		ptrDraw->SetMeshResource(L"GRASS");
		ptrDraw->SetMeshToTransformMatrix(m_bodyMat);
	}

	void Dirt::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Block");
		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		ptrDraw->SetMeshResource(L"DIRT");
		ptrDraw->SetMeshToTransformMatrix(m_bodyMat);
	}

	void Rock::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Block");
		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		ptrDraw->SetMeshResource(L"ROCK");
		ptrDraw->SetMeshToTransformMatrix(m_bodyMat);
	}

	void SandStone::OnCreate()
	{
		CubeObject::OnCreate();
		AddTag(L"Block");
		auto ptrDraw = AddComponent<PNTStaticModelDraw>();
		ptrDraw->SetMeshResource(L"SANDSTONE");
		ptrDraw->SetMeshToTransformMatrix(m_bodyMat);
	}
}