/*!
@file Bumper.h
@brief �X�e�[�W�M�~�b�N�̃o���p�[
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �o���p�[
	*/
	class Bumper : public TemplateObject
	{
		vector<weak_ptr<GameObject>> m_targetObj; // �^�[�Q�b�g�z��
		shared_ptr<CollisionSphere> m_ptrColl;	  // �X�t�B�A�R���W�����R���|�[�l���g
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;   // �`��R���|�[�l���g
		Mat4x4 m_modelMat; // ���f���̃g�����X�t�H�[���Ƃ̍����s��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		*/
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
			StartSE(L"BUMPER_SE", 1.0f); // SE�̍Đ�
			m_ptrDraw->ChangeCurrentAnimation(L"PUSH"); // �A�j���[�V������ύX
		}
	};

	/*!
	@brief �����o���p�[
	*/
	class MoveBumper : public Bumper
	{
	public:

		// �����^�C�venum
		enum class eMoveType : int8_t
		{
			UpDown,
			LeftRight,
		};

		// �X�e�[�genum
		enum class eMoveState : int8_t
		{
			StandBy,
			StartPos,
			MoveA,
			MoveB,
		};

	private:

		eMoveType m_moveType;      // �^�C�v
		eMoveState m_moveState;	   // �X�e�[�g
		eMoveState m_currentState; // �O��̃X�e�[�g

		const Vec3 m_startPos; // �J�n�����W
		Vec3 m_movePointA;     // �ړ��ʒuA
		Vec3 m_movePointB;	   // �ړ��ʒuB

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
		MoveBumper(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eMoveType& type, const float speed, const float length
		) :
			Bumper(stagePtr, position, scale),
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
		virtual ~MoveBumper() {}

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