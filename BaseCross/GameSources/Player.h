/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Player : public GameObject
	{
		Vec3 m_position;
		Vec3 m_rotation;
		Vec2 m_velocity;
		
		float m_acsel;
		float m_speed;
		float m_gravity;
		bool m_isAir;

	public:

		Player(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_position.zero();
			m_rotation.zero();
			m_velocity.zero();
			m_acsel = 0.0f;
			m_speed = 10.0f;
			m_gravity = -5.0f;
			m_isAir = false;
		}

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
		@brief �Փ˔��肪�N����x�ɌĂяo�����֐�
		*/
		void OnCollisionExcute(shared_ptr<GameObject>& other) override;
	};
}