#pragma once
#include "stdafx.h"
#include "CubeObject.h"

namespace basecross
{
	class Gimmick : public CubeObject
	{
	public:

		enum eAngle
		{
			Up,
			Down,
			Left,
			Right,
			All,
			Uleft,
			Uright,
			Dleft,
			Dright,
		};

	protected:

		Mat4x4 m_modelMat;
		eAngle m_angle;

	private:

		const vector<float> m_angles;

	public:


		Gimmick(const shared_ptr<Stage>& stagePtr) :
			m_angles{ 0.0f, 180.0f, 90.0f, -90.0f, 0.0f, 45.0f, -45.0f, 225.0f, 135.0f },
			CubeObject(stagePtr)
		{
			m_angle = Up;
		}

		Gimmick(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& scale, const eAngle& angle
		) :
			m_angle(angle),
			m_angles{ 0.0f, 180.0f, 90.0f, -90.0f, 0.0f, 45.0f, -45.0f, 225.0f, 135.0f},
			CubeObject(stagePtr, position, Vec3(0.0f, 0.0f, 0.0f), scale, true)
		{
		}

		virtual ~Gimmick() {}

		void OnCreate() override;
	};
}