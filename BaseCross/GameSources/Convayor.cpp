#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Convayor::OnCreate()
	{
		Gimmick::OnCreate();
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		switch (m_type)
		{
		case Convayor::Middle:
			m_ptrDraw->SetMultiMeshResource(L"MCONVAYOR");
			break;

		case Convayor::Side:
			m_ptrDraw->SetMultiMeshResource(L"SCONVAYOR");
			break;

		default:
			break;
		}

		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(m_animeKey.at(LeftRot), 0, 60, true);
		m_ptrDraw->AddAnimation(m_animeKey.at(RightRot), 70, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(m_animeKey.at(m_rotate));

		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);

		AddTag(L"Convayor");
	}

	void Convayor::OnUpdate()
	{
		CubeObject::OnUpdate();
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	void ConvayorGuide::OnCreate()
	{
		Billboard::OnCreate();
		float HELF = 0.5f;

		VertexData vertex;
		vertex.vertices = {
			{Vec3(-HELF, HELF, 0.0f), COL_WHITE, Vec2(0.0f, 0.0f)},
			{Vec3(HELF, 0.0f, 0.0f), COL_WHITE, Vec2(1.0f, 0.5f)},
			{Vec3(-HELF, -HELF, 0.0f), COL_WHITE, Vec2(0.0f, 1.0f)},
		};

		vertex.indices = {
			0, 1, 2
		};

		m_ptrDraw->CreateOriginalMesh(vertex);
		SetVerticesColor(COL_RED);
	}

	void ConvayorGuide::OnUpdate()
	{

	}
}