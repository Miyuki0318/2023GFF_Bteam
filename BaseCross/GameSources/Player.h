/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "DebugSphere.h"

namespace basecross
{
	class Player : public GameObject
	{
		Vec3 m_position;
		Vec3 m_rotation;
		Vec2 m_velocity;
		
		vector<weak_ptr<DebugSphere>> m_aligment;

		const float m_maxAcsel;

		float m_timeSpeed;
		float m_acsel;
		float m_speed;
		float m_gravity;
		bool m_isAir;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_maxAcsel(5.0f)
		{
			m_position.zero();
			m_rotation.zero();
			m_velocity = Vec2(0.0f, 1.5f);
			m_timeSpeed = 0.3f;
			m_acsel = 0.0f;
			m_speed = 10.0f;
			m_gravity = -5.0f;
			m_isAir = true;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Player() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief A�{�^���������ꂽ���ɌĂяo�����֐�
		*/
		void OnPushA();

		/*!
		@brief A�{�^����������Ȃ��Ȃ������ɌĂяo�����֐�
		*/
		void OnReleaseA();

		/*!
		@brief �Փ˂����u�ԂɌĂяo�����֐�
		*/
		void OnCollisionEnter(shared_ptr<GameObject>& other) override;

		/*!
		@brief �Փ˂��Ă���ԌĂяo�����֐�
		*/
		void OnCollisionExcute(shared_ptr<GameObject>& other) override;

		/*!
		@brief �v���C���[�̈ړ��֐�
		*/
		void MovePlayer();

		void AligmentRotate();
	};
}