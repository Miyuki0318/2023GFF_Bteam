/*!
@file Player.h
@brief �v���C���[�{��
*/

#pragma once
#include "TemplateObject.h"
#include "ArrowEffect.h"
#include "AirJetEffect.h"
#include "Cannon.h"

namespace basecross
{
	/*!
	@brief �v���C���[�I�u�W�F�N�g�{��
	*/
	class Player : public TemplateObject
	{
	protected:

		// �X�e�[�^�X�Ǘ��penum
		enum class eStatus : uint8_t
		{
			IsAir,				// �󒆂��ǂ���
			IsBlower,			// �����@�ɓ������Ă��邩
			IsHighJump,			// ��������ł��邩
			IsInvincible,		// ���G����
			IsAliveMoveBlock,	// �����ǂ̏�ɋ��邩
			IsHitMoveBlock,		// �����ǂɏՓ˂��Ă��邩
			IsFirePossible,		// �G�A�V���b�N�g�p�\��
			IsCannonFire,		// ��C���ˌォ
			IsCannonStandby		// ��C�ҋ@����
		};

		shared_ptr<PNTBoneModelDraw> m_bodyDraw; // ���̕`��R���|�[�l���g
		shared_ptr<PNTBoneModelDraw> m_armDraw;  // �r�̕`��R���|�[�l���g
		shared_ptr<CollisionSphere> m_ptrColl;   // �X�t�B�A�R���W�����R���|�[�l���g

		const Vec2 m_deffVelo;	// �����͎��̃f�t�H���g�̈ړ���
		const Vec3 m_startPos;  // �J�n���̍��W

		Vec2 m_velocity;	// �ړ���
		Vec2 m_meddleVelo;	// ���Z�ړ���
		Mat4x4 m_bodyMat;	// ���̃��f���ƃg�����X�t�H�[���̍����s��
		Mat4x4 m_armMat;	// �r�̃��f���ƃg�����X�t�H�[���̍����s��

		weak_ptr<TemplateObject> m_arm;			// �r���f���p�I�u�W�F�N�g
		weak_ptr<AirJetEffect> m_airEffect;		// �G�A�V���b�N�G�t�F�N�g
		weak_ptr<ShieldEffect> m_shieldEffect;	// �V�[���h�G�t�F�N�g
		weak_ptr<Cannon> m_activeCannon;		// ��C�|�C���^�ꎞ�ێ��p
		weak_ptr<MultiParticle> m_particle;		// �p�[�e�B�N��
		weak_ptr<ArrowEffect> m_arrow;			// �O���`��
		weak_ptr<TemplateObject> m_currentWall; // �O��Փ˂���������

		const int m_jumpLimit;			  // �K��W�����v��
		const int m_shieldLimit;		  // �K��V�[���h����
		const float m_speed;			  // ���x
		const float m_gravity;			  // �d��
		const float m_maxAcsel;			  // �ő�����x
		const float m_veloSpeed;		  // �ړ��ʏ�Z���x
		const float m_damageAcsel;		  // �_���[�W�������x
		const float m_timeSpeed;		  // ���ԏ�Z���x
		const float m_invincibleTime;	  // �K�薳�G����
		const float m_jumpRecoveryLimit;  // �K��W�����v�񕜎���
		const array<int, 4> m_sRingLimit; // �X���[�������O�K���

		int m_jumpCount;			// �W�����v��
		int m_sRingCount;			// �X���[�������O�����
		int m_shieldCount;			// �V�[���h����
		float m_acsel;				// �����x
		float m_jumpRecoveryTime;	// �W�����v���̉񕜂܂ł̌o�ߎ���
		
		Bool16_t<eStatus> m_status; // ��ԃt���O

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		*/
		Player(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			TemplateObject(stagePtr),
			m_startPos(position),
			m_jumpLimit(2),
			m_shieldLimit(3),
			m_speed(4.0f),
			m_gravity(-5.0f),
			m_maxAcsel(4.5f),
			m_veloSpeed(2.5f),
			m_damageAcsel(2.5f),
			m_timeSpeed(1.5f),
			m_invincibleTime(1.0f),
			m_jumpRecoveryLimit(0.5f),
			m_deffVelo(0.0f, -1.0f),
			m_sRingLimit({ 5, 20, 25, 0	})
		{
			m_position = m_startPos;
			m_rotation.zero();
			m_scale = Vec3(1.0f);
			m_velocity = Vec2(-5.0f, 0.0f);
			m_meddleVelo.zero();
			m_jumpCount = 0;
			m_sRingCount = 0;
			m_shieldCount = 1;
			m_acsel = 7.5f;
			m_jumpRecoveryTime = 0.0f;
			m_status.Set(eStatus::IsAir, eStatus::IsFirePossible) = true;

			m_bodyMat.affineTransformation(
				Vec3(1.25f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -0.65f, 0.0f)
			);

			m_armMat.affineTransformation(
				Vec3(1.25f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -1.35f, 0.0f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Player() {}

		/*!
		@brief ���Z�b�g�֐�
		*/
		virtual void Reset();

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief A�{�^����������Ȃ��Ȃ������ɌĂяo�����֐�
		*/
		virtual void OnRushA();

		/*!
		@brief �Փ˂����u�ԂɌĂяo�����֐�
		@param �R���W�����y�A(�R���W�������)
		*/
		void OnCollisionEnter(const CollisionPair& Pair) override;

		/*!
		@brief �Փ˂��Ă���ԌĂяo�����֐�
		@param �R���W�����y�A(�R���W�������)
		*/
		void OnCollisionExcute(const CollisionPair& Pair) override;

		/*!
		@brief �Փ˂��Ă���ԌĂяo�����֐�
		@param �R���W�����y�A(�R���W�������)
		*/
		void OnCollisionExit(const CollisionPair& Pair) override;

	protected:

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
		virtual void RotatePlayer();

		/*!
		@brief �G�A�V���b�N�̋O���`��֐�
		*/
		virtual void UpdateArrow();

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
		@param �����x
		*/
		virtual void CannonStandby(float acsel);

		/*!
		@brief ��C���ˌ�֐�
		*/
		virtual void AftterCannon();

		/*!
		@brief ���S���̐ݒ�֐�
		*/
		virtual void DeathSetup();

		/*!
		@brief ���S���̗����֐�
		*/
		virtual void DeathDrop();

		/*!
		@brief ���G���Ԍo��
		*/
		void InvincibleTimer();

		/*!
		@brief �����@�͈̔͂ɓ����Ă��邩�̃`�F�b�N�֐�
		*/
		void BlowerBetweenCheck();

		/*!
		@brief ������̑����@�ɓ������Ă��鎞
		*/
		void UpMeddleVelocity();

		/*!
		@brief �������̑����@�ɓ������Ă��鎞
		*/
		void DownMeddleVelocity();

		/*!
		@brief �������̑����@�ɓ������Ă��鎞
		*/
		void LeftMeddleVelocity();

		/*!
		@brief �E�����̑����@�ɓ������Ă��鎞
		*/
		void RightMeddleVelocity();

		/*!
		@brief ���Z�ړ��ʂ̌����֐�
		*/
		void ReductionMeddleVelocity();

		/*!
		@brief �u���b�N�ɏՓ˂�����
		@param �u���b�N�̃|�C���^
		@param �Փˍ��W
		*/
		void BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		/*!
		@brief �u���b�N�ɏォ��Փ˂�����
		@param �u���b�N�̍��W
		@param �u���b�N�̔��a
		*/
		void BlockUpperHit(const Vec3& objPos, const Vec3& helf);

		/*!
		@brief �u���b�N�ɉ�����Փ˂�����
		@param �u���b�N�̍��W
		@param �u���b�N�̔��a
		*/
		void BlockUnderHit(const Vec3& objPos, const Vec3& helf);

		/*!
		@brief �u���b�N�ɍ�����Փ˂�����
		@param �u���b�N�̍��W
		@param �u���b�N�̔��a
		*/
		void BlockLeftHit(const Vec3& objPos, const Vec3& helf);

		/*!
		@brief �u���b�N�ɉE����Փ˂�����
		@param �u���b�N�̍��W
		@param �u���b�N�̔��a
		*/
		void BlockRightHit(const Vec3& objPos, const Vec3& helf);

		/*!
		@brief �u���b�N�o�E���h��������SE
		*/
		void BlockBoundSE();

		/*!
		@brief �u���b�N�ɏՓ˂���������
		@param �u���b�N�̃|�C���^
		@param �Փˍ��W
		*/
		void BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		/*!
		@brief ���ɏՓ˂�����
		@param ���̃|�C���^
		@param �Փˍ��W
		*/
		void SpikeEnter(const shared_ptr<GameObject>& spike, const Vec3& hitPos);

		/*!
		@brief ���ɏՓ˂���������
		@param ���̃|�C���^
		@param �Փˍ��W
		*/
		void SpikeExcute(const shared_ptr<GameObject>& spike, const Vec3& hitPos);

		/*!
		@brief ��C�ɏՓ˂�����
		@param ��C�̃|�C���^
		*/
		void CannonEnter(const shared_ptr<GameObject>& cannon);

		/*!
		@brief �x���g�R���x�A�ɏՓ˂�����
		@param �x���g�R���x�A�̃|�C���^
		@param �Փˍ��W
		*/
		void ConvayorEnter(const shared_ptr<GameObject>& convayor, const Vec3& hitPos);

		/*!
		@brief �x���g�R���x�A�ɏՓ˂���������
		@param �x���g�R���x�A�̃|�C���^
		@param �Փˍ��W
		*/
		void ConvayorExcute(const shared_ptr<GameObject>& convayor, const Vec3& hitPos);

		/*!
		@brief �o���p�[�ɏՓ˂�����
		@param �o���p�[�̃|�C���^
		@param �Փˍ��W
		*/
		void BumperEnter(const shared_ptr<GameObject>& bumper, const Vec3& hitPos);

		/*!
		@brief �G�̃E�T�M�ɏՓ˂�����
		@param �E�T�M�̃|�C���^
		*/
		void RabbitEnter(const shared_ptr<GameObject>& rabbit, const Vec3& hitPos);

		/*!
		@brief �G�̃E�T�M�ɏՓ˂���������
		@param �E�T�M�̃|�C���^
		*/
		void RabbitExcute(const shared_ptr<GameObject>& rabbit, const Vec3& hitPos);

		/*!
		@brief �����O�ɏՓ˂�����
		@param �����O�̃|�C���^
		*/
		void RingEnter(const shared_ptr<GameObject>& ring);

		/*!
		@brief �����ǂɏՓ˂�����
		@param �����ǂ̃|�C���^
		@param �Փˍ��W
		*/
		void MoveWallEnter(const shared_ptr<GameObject>& wall, const Vec3& hitPos);

		/*!
		@brief �����ǂ��痣�ꂽ��
		*/
		void MoveWallExit();

		/*!
		@brief �_���[�W�m�b�N�o�b�N����
		@param �ړ���
		*/
		void DamageKnockBack(const Vec2& velocity);

		/*!
		@brief �����ǂ̘A���擾�֐�
		@param �����ǃO���[�v�z��
		@param �J�n���W
		@param �������Z�x�N�g��
		@return �A�ԍŌ�̍��W
		*/
		Vec3 GetMoveWallPos(const vector<weak_ptr<GameObject>>& groupVec, const Vec3& pos, const Vec3& addVec)
		{
			Vec3 position = pos;

			while (ObjectCheck(groupVec, position))
			{
				position += addVec;
			}

			return position;
		}

		/*!
		@brief �����ǃO���[�v�̎擾�֐�
		@param gimmick�O���[�v�z��
		@return �����ǃO���[�v�z��
		*/
		const vector<weak_ptr<GameObject>> GetMoveWallGroup(const vector<weak_ptr<GameObject>>& groupVec)
		{
			vector<weak_ptr<GameObject>> moveWallVec;
			for (const auto& gimmick : groupVec)
			{
				if (!gimmick.lock()) continue;

				const auto& moveWall = dynamic_pointer_cast<MoveWall>(gimmick.lock());
				if (!moveWall) continue;

				moveWallVec.push_back(moveWall);
			}

			return moveWallVec;
		}

		/*!
		@brief �V�[���h�ǉ�����
		*/
		void AddShield()
		{
			// SE�̍Đ�
			StartSE(L"SHIELD_C_SE", 0.75f);

			// �V�[���h�𑝂₷
			m_shieldCount++;
			m_shieldCount = min(m_shieldLimit, m_shieldCount);
		}

		/*!
		@brief �������O�擾����
		*/
		void GetSmallRing()
		{
			m_sRingCount++; // �������O�J�E���^�𑝂₷

			// �W�����v�񐔂����炷
			m_jumpCount > 0 ? m_jumpCount-- : m_jumpCount = 0;
		}


		/*!
		@brief �㉺�ŉ����ׂ��ꂽ���̊֐�
		*/
		void UnderCompressedDeath()
		{
			if (m_status(eStatus::IsAliveMoveBlock))
			{
				// ���S���̐ݒ������
				m_shieldCount = 0;
				StartSE(L"SHIELD_D_SE", 1.5f);
				DeathSetup();
			}
		}

		/*!
		@brief ���E�ŉ����ׂ��ꂽ���̊֐�
		*/
		void LeftRightCompressedDeath()
		{
			if (m_status(eStatus::IsHitMoveBlock))
			{
				// ���S���̐ݒ������
				m_shieldCount = 0;
				StartSE(L"SHIELD_D_SE", 1.5f);
				DeathSetup();
			}
		}

	public:

		/*!
		@brief �V�[���h�̖����擾�֐�
		@return m_shieldCount
		*/
		const int& GetShieldCount() const
		{
			return m_shieldCount;
		}

		/*!
		@brief �V�[���h�̏�����擾�֐�
		@return m_shieldLimit
		*/
		const int& GetShieldLimit() const
		{
			return m_shieldLimit;
		}

		/*!
		@brief �X���[�������O�l�����擾�֐�
		@return m_sRingCount
		*/
		const int& GetSRingCount() const
		{
			return m_sRingCount;
		}

		/*!
		@brief �X���[�������O�l��������擾�֐�
		@return m_sRingLimit
		*/
		const array<int, 4>& GetSRingLimit() const
		{
			return m_sRingLimit;
		}

		/*!
		@brief �W�����v�񐔎擾�֐�
		@return m_jumpCount
		*/
		const int& GetJumpCount() const
		{
			return m_jumpCount;
		}

		/*!
		@brief �W�����v����l�擾�֐�
		@return m_jumpLimit
		*/
		const int& GetJumpLimit() const
		{
			return m_jumpLimit;
		}

		/*!
		@brief �W�����v�o�ߎ��Ԏ擾�֐�
		@return m_jumpRecoveryTime
		*/
		const float& GetJumpRecoveryTime() const
		{
			return m_jumpRecoveryTime;
		}

		/*!
		@brief �W�����v�񕜎��Ԏ擾�֐�
		@return m_jumpRecoveryLimit
		*/
		const float& GetJumpRecoveryLimit() const
		{
			return m_jumpRecoveryLimit;
		}

		/*!
		@brief �����x�̎擾�֐�
		@return m_acsel
		*/
		const float& GetAcsel() const
		{
			return m_acsel;
		}

		/*!
		@brief �ړ��ʂ̎擾�֐�
		@return m_velocity
		*/
		const Vec2& GetVelocity() const
		{
			return m_velocity;
		}
	};
}