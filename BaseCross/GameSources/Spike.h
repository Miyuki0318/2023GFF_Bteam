/*!
@file Spike.h
@brief �X�e�[�W�M�~�b�N�̞�
*/

#pragma once
#include "Gimmick.h"

namespace basecross 
{
	/*!
	@brief ��
	*/
	class Spike : public Gimmick
	{
		// �`��R���|�[�l���g
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; 

		// ���f���ƃg�����X�t�H�[���̍����s��
		Mat4x4 m_blockMat;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �p�x
		*/
		Spike(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, GimmickAngle::eAngle angle
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

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Spike() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;
	};

	/*!
	@brief ������
	*/
	class MoveSpike : public Spike
	{
	public:

		// ���������^�C�venum
		enum class eMoveType : int8_t
		{
			UpDown,		// �㉺
			LeftRight,	// ���E
		};

		// �����X�e�[�genum
		enum class eMoveState : int8_t
		{
			StandBy,	// �ҋ@��
			StartPos,	// �J�n���̍��W
			MoveA,		// �|�C���gA�Ɉړ�
			MoveB,		// �|�C���gB�Ɉړ�
		};

	private:

		eMoveType m_moveType;	   // �����^�C�v
		eMoveState m_moveState;	   // �X�e�[�g
		eMoveState m_currentState; // �O��̃X�e�[�g

		const Vec3 m_startPos; // �J�n���̍��W
		Vec3 m_movePointA;	   // �|�C���gA
		Vec3 m_movePointB;	   // �|�C���gB

		const float m_moveSpeed;  // �ړ����x
		const float m_moveLength; // �ړ�����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �����^�C�v
		@param �ړ����x
		@param �ړ�����
		*/
		MoveSpike(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			eMoveType type, const float speed, const float length
		) :
			Spike(stagePtr, position, scale, GimmickAngle::eAngle::All),
			m_moveType(type),
			m_moveSpeed(speed),
			m_moveLength(length),
			m_startPos(position)
		{
			m_moveState = eMoveState::StartPos;
			m_currentState = eMoveState::StandBy;
			m_movePointA = position;
			m_movePointB = position;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~MoveSpike() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �X�e�[�g�̍X�V�����֐�
		*/
		void UpdateState();

		/*!
		@brief �ҋ@���̃X�e�[�g�֐�
		*/
		void StandbyState();

		/*!
		@brief �|�C���g�Ɉړ����̃X�e�[�g�֐�
		@param �J�n���W
		@param �I�����W
		@param ����
		@return �ړ��I���������̐^�U
		*/
		bool PointState(const Vec3& start, const Vec3& end, float time);
	};
}