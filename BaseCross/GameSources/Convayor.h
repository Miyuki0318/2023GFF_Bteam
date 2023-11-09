#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross
{
	class Convayor : public Gimmick
	{
	public:

		enum eType
		{
			Side = 1,
			Middle = 2,
		};

	private:

		const eType m_type;
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

	public:

		Convayor(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const Vec3& scale,
			const eAngle& angle, const eType& type
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), scale, angle),
			m_type(type)
		{
			m_modelMat.affineTransformation(
				Vec3(1.35f * (1.0f / m_type), 1.35f, 1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)			
			);
		}

		virtual ~Convayor() {}

		void OnCreate() override;

		void OnUpdate() override;
	};
}