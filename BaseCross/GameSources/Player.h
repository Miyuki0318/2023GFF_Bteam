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
		shared_ptr<Transform> m_ptrTrans;
		shared_ptr<PNTBoneModelDraw> m_bodyDraw;
		shared_ptr<PNTBoneModelDraw> m_armDraw;

		Vec3 m_position;
		Vec3 m_rotation;
		Vec2 m_velocity;
		const Vec2 m_deffVelo;
		Mat4x4 m_bodyMat;
		Mat4x4 m_armMat;

		weak_ptr<DebugObject> m_arm;
		weak_ptr<Billboard> m_effect;
		vector<weak_ptr<DebugSphere>> m_aligment;

		const float m_maxAcsel;
		const float m_damageAcsel;
		const float m_slowTime;
		const float m_normalTime;

		float m_timeSpeed;
		float m_acsel;
		float m_speed;
		float m_gravity;
		bool m_isAir;
		bool m_firePossible;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_maxAcsel(5.0f),
			m_damageAcsel(3.0f),
			m_slowTime(0.1f),
			m_normalTime(2.0f),
			m_deffVelo(0.0f, -1.0f)
		{
			m_position.zero();
			m_rotation.zero();
			m_velocity = m_deffVelo;
			m_timeSpeed = m_normalTime;
			m_acsel = 1.0f;
			m_speed = 3.0f;
			m_gravity = -5.0f;
			m_isAir = true;
			m_firePossible = true;

			m_bodyMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -0.65f, 0.0f)
			);
			m_armMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -1.05f, 0.0f)
			);
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
		@brief �Փ˂���Ȃ��Ȃ�����Ăяo�����֐�
		*/
		void OnCollisionExit(shared_ptr<GameObject>& other) override;

		/*!
		@brief �v���C���[�̈ړ��֐�
		*/
		void MovePlayer();

		/*!
		@brief �v���C���[�̈ړ��ʌ����֐�
		*/
		void MoveReduction();

		/*!
		@brief �v���C���[�̉�]�֐�
		*/
		void RotatePlayer();

		/*!
		@brief �G�A�V���b�N�̋O���`��֐�
		*/
		void RotateAligment();

		/*!
		@brief �A�j���[�V�����X�V�֐�
		*/
		void AnimationUpdate();

		/*!
		@brief �G�t�F�N�g�`��֐�
		*/
		void EffectUpdate();

		void BlockEnter(shared_ptr<GameObject>& block);

		void BlockExcute(shared_ptr<GameObject>& block);

		void BlockExit(shared_ptr<GameObject>& block);

		void SpikeEnter(shared_ptr<GameObject>& spike);

		void SpikeExcute(shared_ptr<GameObject>& spike);

		void DamageKnockBack(const Vec2& velocity);

		bool CollHitUpper(const Vec3& position, const Vec3& helfScale);

		bool CollHitUnder(const Vec3& position, const Vec3& helfScale);

		bool CollHitLeft(const Vec3& position, const Vec3& helfScale);

		bool CollHitRight(const Vec3& position, const Vec3& helfScale);
	};
}