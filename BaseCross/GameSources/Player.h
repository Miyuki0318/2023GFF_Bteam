/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"
#include "DebugObject.h"
#include "ArrowEffect.h"
#include "AirJetEffect.h"
#include "Cannon.h"

namespace basecross
{
	class Player : public DebugObject
	{
	protected:

		shared_ptr<PNTBoneModelDraw> m_bodyDraw;
		shared_ptr<PNTBoneModelDraw> m_armDraw;

		Vec3 m_respawnPos;
		Vec2 m_velocity;
		Vec2 m_meddleVelo;
		const Vec2 m_deffVelo;
		Mat4x4 m_bodyMat;
		Mat4x4 m_armMat;

		weak_ptr<DebugObject> m_arm;
		weak_ptr<AirJetEffect> m_jetEffect;
		weak_ptr<ShieldEffect> m_shieldEffect;
		weak_ptr<Cannon> m_activeCannon;
		weak_ptr<MultiParticle> m_particle;
		weak_ptr<ArrowEffect> m_aligment;

		const int m_jumpLimit;
		const int m_shieldLimit;
		const float m_maxAcsel;
		const float m_veloSpeed;
		const float m_damageAcsel;
		const float m_slowTime;
		const float m_normalTime;
		const float m_invincibleTime;
		const float m_jumpRecoveryLimit;

		int m_jumpCount;
		int m_sRingCount;
		int m_shieldCount;
		float m_timeSpeed;
		float m_damageTime;
		float m_acsel;
		float m_speed;
		float m_gravity;
		float m_jumpRecoveryTime;
		
		bool m_isAir;
		bool m_isBlower;
		bool m_isInvincible;
		bool m_firePossible;
		bool m_cannonFire;
		bool m_cannonStandby;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Player(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			DebugObject(stagePtr),
			m_respawnPos(position),
			m_jumpLimit(3),
			m_shieldLimit(3),
			m_maxAcsel(4.5f),
			m_veloSpeed(2.5f),
			m_damageAcsel(2.5f),
			m_slowTime(0.1f),
			m_normalTime(1.5f),
			m_invincibleTime(0.5f),
			m_jumpRecoveryLimit(0.75f),
			m_deffVelo(0.0f, -1.0f)
		{
			m_position = position;
			m_rotation.zero();
			m_scale = Vec3(1.0f);
			m_velocity = m_deffVelo;
			m_meddleVelo.zero();
			m_timeSpeed = m_normalTime;
			m_jumpCount = 0;
			m_sRingCount = 0;
			m_shieldCount = 1;
			m_damageTime = 0.0f;
			m_acsel = 1.0f;
			m_speed = 4.0f;
			m_gravity = -5.0f;
			m_jumpRecoveryTime = 0.0f;
			m_isAir = true;
			m_isBlower = false;
			m_isInvincible = false;
			m_firePossible = true;
			m_cannonFire = false;
			m_cannonStandby = false;

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
		virtual void OnUpdate() override;

		/*!
		@brief A�{�^����������Ȃ��Ȃ������ɌĂяo�����֐�
		*/
		virtual void OnReleaseA();

		/*!
		@brief �Փ˂����u�ԂɌĂяo�����֐�
		*/
		void OnCollisionEnter(const CollisionPair& Pair) override;

		/*!
		@brief �Փ˂��Ă���ԌĂяo�����֐�
		*/
		void OnCollisionExcute(const CollisionPair& Pair) override;

		/*!
		@brief �Փ˂��Ă���ԌĂяo�����֐�
		*/
		void OnCollisionExit(const CollisionPair& Pair) override;

		/*!
		@brief �v���C���[�̈ړ��֐�
		*/
		void MovePlayer();

		/*!
		@brief �v���C���[�̈ړ��ʌ����֐�
		*/
		void MoveReduction();

		/*!
		@brief �G�A�V���b�N�̉񕜊֐�
		*/
		void RecoveryAirShock();

		/*!
		@brief �v���C���[�̉�]�֐�
		*/
		void RotatePlayer();

		/*!
		@brief �G�A�V���b�N�̋O���`��֐�
		*/
		virtual void RotateAligment();

		/*!
		@brief �A�j���[�V�����X�V�֐�
		*/
		void AnimationUpdate();

		/*!
		@brief �G�t�F�N�g�`��֐�
		*/
		void EffectUpdate();

		/*!
		@brief ��C�ҋ@�֐�
		*/
		virtual void CannonStandby(float acsel);

		/*!
		@brief ���G���Ԍo��
		*/
		void InvincibleTimer();

		void BlowerBetweenCheck();

		void UpMeddleVelocity();

		void DownMeddleVelocity();

		void LeftMeddleVelocity();

		void RightMeddleVelocity();

		void ReductionMeddleVelocity();

		void BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		void BlockUpperHit(const Vec3& objPos, const Vec3& helf);

		void BlockUnderHit(const Vec3& objPos, const Vec3& helf);

		void BlockLeftHit(const Vec3& objPos, const Vec3& helf);

		void BlockRightHit(const Vec3& objPos, const Vec3& helf);

		void BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		void SpikeEnter(const shared_ptr<GameObject>& spike, const Vec3& hitPos);

		void SpikeExcute(const shared_ptr<GameObject>& spike, const Vec3& hitPos);

		void CannonEnter(const shared_ptr<GameObject>& cannon);

		void ConvayorEnter(const shared_ptr<GameObject>& convayor, const Vec3& hitPos);

		void ConvayorExcute(const shared_ptr<GameObject>& convayor, const Vec3& hitPos);

		void DamageKnockBack(const Vec2& velocity);

		void RingCheck(const shared_ptr<GameObject>& ring);

		void AddShield()
		{
			StartSE(L"SHIELD_C_SE", 0.75f);
			m_shieldCount++;
			if (m_shieldCount > m_shieldLimit)
			{
				m_shieldCount = m_shieldLimit;
			}
		}

		const int& GetShieldCount() const
		{
			return m_shieldCount;
		}

		const float& GetAcsel() const
		{
			return m_acsel;
		}

		const Vec2& GetVelocity() const
		{
			return m_velocity;
		}
	};
}