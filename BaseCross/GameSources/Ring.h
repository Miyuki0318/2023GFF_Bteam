/*!
@file Ring.h
@brief �X�e�[�W�M�~�b�N�̃����O
*/

#pragma once
#include "Gimmick.h"

namespace basecross
{
	/*!
	@brief �����O
	*/
	class Ring : public Gimmick
	{
	public:

		// �����O�̃T�C�Yenum
		enum class eRingSize : int8_t
		{
			Big,	// ��
			Small,	// ��
		};

	private:

		// �`��R���|�[�l���g
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

		const eRingSize m_ringSize; // �����O�T�C�Y
		bool m_isGet;			// �擾�������̐^�U
		float m_rotSpeed;		// ��]���x
		float m_totalTime;		// �o�ߎ���
		float m_deleteTime;		// �폜����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �����O�T�C�Y
		*/
		Ring(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale, eRingSize ring
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale, scale, scale), GimmickAngle::eAngle::Up),
			m_ringSize(ring)
		{
			m_isGet = false;
			m_rotSpeed = 3.0f;
			m_totalTime = 0.0f;
			m_deleteTime = 1.0f;
			m_modelMat.affineTransformation(
				Vec3(0.475f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.75f, 0.0f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Ring() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �擾�������̊֐�
		*/
		void IsGetRing()
		{
			m_isGet = true;
			m_rotSpeed = 12.0f;
			m_ptrColl->SetUpdateActive(false);
		}

		/*!
		@brief �����O�T�C�Y�擾�֐�
		@return eRingSize
		*/
		const eRingSize& GetRingSize() const
		{
			return m_ringSize;
		}
	};
}