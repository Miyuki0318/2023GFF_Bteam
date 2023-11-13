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

		enum eRotate
		{
			LeftRot,
			RightRot,
		};

	private:

		eRotate m_rotate;
		const eType m_type;
		vector<wstring> m_animeKey;
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

	public:

		Convayor(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eRotate& rotate, const eType& type
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale), Up),
			m_type(type),
			m_rotate(rotate)
		{
			m_animeKey = {
				L"LEFT",
				L"RIGHT"
			};

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

		void SetRotate(const eRotate& rot)
		{
			if (rot != m_rotate)
			{
				m_rotate = rot;
				m_ptrDraw->ChangeCurrentAnimation(m_animeKey.at(rot));
			}
		}

		eRotate GetRotate() const
		{
			return m_rotate;
		}
	};
}