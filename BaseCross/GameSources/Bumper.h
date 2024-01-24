#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross
{
	class Bumper : public TemplateObject
	{
		vector<weak_ptr<GameObject>> m_targetObj;

		shared_ptr<CollisionSphere> m_ptrColl;
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;
		Mat4x4 m_modelMat;

	public:

		Bumper(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			TemplateObject(stagePtr, Vec3(position), Vec3(0.0f), Vec3(scale))
		{
			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(-XM_PIDIV2, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.5f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Bumper() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �Փˎ��ɃA�j���[�V���������Z�b�g����֐�
		*/
		void OnHit()
		{
			StartSE(L"BUMPER_SE", 1.0f);
			m_ptrDraw->ChangeCurrentAnimation(L"PUSH");
		}
	};

	class MoveBumper : public Bumper
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

		MoveBumper(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			eMoveType type, const float speed, const float length
		) :
			Bumper(stagePtr, position, scale),
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

		virtual ~MoveBumper() {}

		void OnCreate() override;

		void OnUpdate() override;

		void UpdateMove();

		void StandbyState();

		bool PointState(const Vec3& start, const Vec3& end, float time);
	};
}