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
			Middle,
			Side,
		};

	private:

		const eType m_type;
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

	public:

		Convayor(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eAngle& angle, const eType& type
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale), angle),
			m_type(type)
		{
			m_modelMat.affineTransformation(
				Vec3(1.35f, 1.35f, 1.35f),
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