#pragma once
#include "stdafx.h"
#include "Gimmick.h"
#include "Button.h"

namespace basecross
{
	class MoveWall : public Gimmick
	{
	public:

		enum eMoveType
		{
			Down,
			Up,
			Left,
			Right
		};

	private:

		shared_ptr<PNTStaticDraw> m_ptrDraw;
		vector<weak_ptr<Button>> m_buttons;

		const eMoveType m_moveType;
		const int m_number;
		const float m_moveSpeed;
		const float m_moveLength;
		float m_moveRatio;
		
		const Vec3 m_startPos;
		Vec3 m_movePoint;

	public:

		MoveWall(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eMoveType& type, const float speed, 
			const float length, const int number
		) :
			Gimmick(stagePtr, Vec3(position), Vec3(scale), eAngle::All),
			m_moveType(type),
			m_moveSpeed(speed),
			m_moveLength(length),
			m_number(number),
			m_startPos(position)
		{
			m_movePoint = position;
			m_moveRatio = 0.0f;

			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		virtual ~MoveWall() {}

		void OnCreate() override;

		void OnUpdate() override;

		void SetTargetButton();

		void MoveWallBlock(const Vec3& start, const Vec3& end);
	};
}