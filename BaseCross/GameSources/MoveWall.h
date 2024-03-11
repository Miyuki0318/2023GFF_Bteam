/*!
@file MoveWall.h
@brief �X�e�[�W�M�~�b�N�̓�����
*/

#pragma once
#include "Gimmick.h"
#include "Button.h"

namespace basecross
{
	/*!
	@brief ������
	*/
	class MoveWall : public Gimmick
	{
	public:

		// ���������^�C�venum
		enum class eMoveType : int8_t
		{
			Down,   // ������
			Up,		// �����
			Left,	// ������
			Right,	// �E����
			DownSE,	// SE�t��������
			UpSE,	// SE�t�������
			LeftSE, // SE�t��������
			RightSE	// SE�t���E����
		};

	private:

		shared_ptr<PNTStaticDraw> m_ptrDraw; // �`��R���|�[�l���g
		vector<weak_ptr<Button>> m_buttons;  // �J�p�̃{�^��

		const eMoveType m_moveType; // ���������^�C�v
		const Vec3 m_startPos;		// �J�n���W
		const int m_number;			// �{�^�����ʃi���o�[
		const float m_moveSpeed;	// �J���x
		const float m_moveLength;	// �J����

		float m_moveRatio;		// ��������
		float m_currentRatio;	// �O��̓�������
		bool m_currentInput;	// �O�񓮂�����
		Vec3 m_movePoint;		// �J���W

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param ���������^�C�v
		@param �J���x
		@param �J����
		@param ���ʃi���o�[
		*/
		MoveWall(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eMoveType& type, const float speed, 
			const float length, const int number
		) :
			Gimmick(stagePtr, Vec3(position), Vec3(scale), GimmickAngle::eAngle::All),
			m_moveType(type),
			m_moveSpeed(speed),
			m_moveLength(length),
			m_number(number),
			m_startPos(position)
		{
			m_movePoint = position;
			m_moveRatio = 0.0f;
			m_currentRatio = 0.0f;
			m_currentInput = false;

			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~MoveWall() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief ���t���[���X�V�������s�����̐ݒ�֐�
		*/
		void SetUpdateActive(bool b) override;

		/*!
		@brief �J�p�{�^���ݒ�֐�
		*/
		void SetTargetButton();

		/*!
		@brief �����ǈړ��֐�
		@param �J�n���W
		@param �I�����W
		*/
		void MoveWallBlock(const Vec3& start, const Vec3& end);

		/*!
		@brief �����ǂ̈ړ��ʎ擾�֐�
		@return m_moveRatio
		*/
		const float GetMoveRatio() const
		{
			return m_moveRatio;
		}
	};
}