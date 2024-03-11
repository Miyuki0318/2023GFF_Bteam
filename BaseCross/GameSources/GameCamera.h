/*!
@file GameCamera.h
@brief �Q�[�����J����
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief �Q�[�����J����
	*/
	class GameCamera : public Camera
	{
		// �^�[�Q�b�g�I�u�W�F�N�g�|�C���^
		weak_ptr<GameObject> m_target; 

		const Vec3 m_startEye; // �J�n���̎��_�̍��W
		const Vec3 m_startAt;  // �J�n���̒����_�̍��W
		const Vec3 m_startTgt; // �J�n���̃^�[�Q�b�g�̍��W
		Vec3 m_targetPos;      // �^�[�Q�b�g�̍��W
		Vec3 m_eyePos;         // ���_�̍��W
		Vec3 m_atPos;		   // �����_�̍��W

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		GameCamera() :
			m_startEye(Vec3(-20.5f, -82.3f, -33.0f)),
			m_startAt(Vec3(-20.5f, -82.3f, 0.0f)),
			m_startTgt(Vec3(-35.0f, -88.5f, 0.0f))
		{
			m_targetPos = m_startTgt;
			m_eyePos = m_startEye;
			m_atPos = m_startAt;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GameCamera() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �^�[�Q�b�g�I�u�W�F�N�g�ݒ�֐�
		@param �^�[�Q�b�g�I�u�W�F�N�g�|�C���^
		*/
		void SetTargetObject(const shared_ptr<GameObject>& object)
		{
			// ���݂̃^�[�Q�b�g�ƈ������X�V
			if (m_target.lock() != object)
			{
				m_target = object;
			}
		}

		/*!
		@brief �^�[�Q�b�g�ݒ�����֐�
		*/
		void RemoveTarget()
		{
			// �^�[�Q�b�g�̍��W��ێ����ă|�C���^�̕ێ�������
			m_targetPos = m_target.lock()->GetComponent<Transform>()->GetPosition();
			m_target.reset();
		}

		/*!
		@brief ���Z�b�g�֐�
		*/
		void ResetCamera()
		{
			// �J�n���̎��_�����_�̍��W�ɍX�V
			m_eyePos = m_startEye;
			m_atPos = m_startAt;
			SetEye(m_eyePos);
			SetAt(m_atPos);

			// �^�[�Q�b�g�̍��W���J�n���̈ʒu�ɍX�V
			m_targetPos = m_startTgt;
		}

		/*!
		@brief �^�[�Q�b�g�̍��W�擾�֐�
		@return m_targetPos
		*/
		const Vec3& GetTargetPos() const
		{
			return m_targetPos;
		}
	};
}