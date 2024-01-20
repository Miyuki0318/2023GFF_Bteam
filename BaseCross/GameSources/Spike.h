#pragma once
#include "stdafx.h"
#include "Gimmick.h"

namespace basecross 
{
	class Spike : public Gimmick
	{
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;
		Mat4x4 m_blockMat;

	public:

		Spike(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, const eAngle& angle
		) :
			Gimmick(stagePtr, Vec3(position), Vec3(scale), angle)
		{
			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f, -0.65f, 0.0f)
			);
			m_blockMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.65f, 0.0f)
			);
		}

		virtual ~Spike() {}

		virtual void OnCreate() override;

		virtual void OnUpdate() override;
	};

	class MoveSpike : public Spike
	{
	public:

		enum eMoveType
		{
			UpDown,
			LeftRight,
		};

		enum eMoveState
		{
			StandBy,
			StartPos,
			MoveA,
			MoveB,
		};

	private:

		eMoveType m_moveType;
		eMoveState m_moveState;
		eMoveState m_currentState;

		const Vec3 m_startPos;
		Vec3 m_movePointA;
		Vec3 m_movePointB;

		const float m_moveSpeed;
		const float m_moveLength;

	public:

		MoveSpike(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			eMoveType type, const float speed, const float length
		) :
			Spike(stagePtr, position, scale, eAngle::All),
			m_moveType(type),
			m_moveSpeed(speed),
			m_moveLength(length),
			m_startPos(position)
		{
			m_moveState = StartPos;
			m_currentState = StandBy;
			m_movePointA = position;
			m_movePointB = position;
		}

		virtual ~MoveSpike() {}

		void OnCreate() override;

		void OnUpdate() override;

		void UpdateMove();

		void StandbyState();

		bool PointState(const Vec3& start, const Vec3& end, float time);
	};
}