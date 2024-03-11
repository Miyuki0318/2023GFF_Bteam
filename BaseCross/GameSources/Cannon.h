/*!
@file Cannon.h
@brief �X�e�[�W�M�~�b�N�̑�C
*/

#pragma once
#include "Gimmick.h"

namespace basecross
{
	/*!
	@brief ��C�̃p�����[�^
	*/
	namespace CannonParam
	{
		// ��C�̎��enum
		enum class eFireType : int8_t
		{
			Normal,	 // �ʏ�
			Rapid,	 // ����
			NRotate, // �ʏ�+��]
			RRotate, // ����+��]
		};

		// ��Փx�I��p��C�^�C�v
		enum class eDifficulty : int8_t
		{
			Easy,	// �C�[�W�[
			Normal, // �m�[�}��
			Hard,   // �n�[�h
		};

		// ��C�̏��enum
		enum class eStatus : uint8_t
		{
			IsFire,	  // ���˂�����
			IsRotate, // ��]����
		};

		/*!
		@brief ��C�̔��ˎ��Ԏ擾�֐�
		@param eFireType
		@return ���ˎ���
		*/
		static float GetFireTime(const eFireType fireType)
		{
			// ��C�̎�ނŕ���
			switch (fireType)
			{
			case eFireType::Normal:  // �ʏ�^�C�v
			case eFireType::NRotate: // �ʏ�+��]�^�C�v
				return 1.4f; // �ʏ�^�C�v�̔��ˎ��Ԃ�Ԃ�
				break;

			case eFireType::Rapid:   // ���˃^�C�v
			case eFireType::RRotate: // ����+��]�^�C�v
				return 0.4f; // ���˃^�C�v�̔��ˎ��Ԃ�Ԃ�
				break;

			default:
				return 0.0f;
				break;
			}
		}

		/*!
		@brief ��C�̃A�j���[�V�����L�[�擾�֐�
		@param eFireType
		@return �A�j���[�V�����L�[
		*/
		static wstring GetAnimeKey(const eFireType fireType)
		{
			// ��C�̎�ނŕ���
			switch (fireType)
			{
			case eFireType::Normal:  // �ʏ�^�C�v
			case eFireType::NRotate: // �ʏ�+��]�^�C�v
				return L"FIRE"; // �ʏ�^�C�v�̃A�j���[�V�����L�[��Ԃ�
				break;

			case eFireType::Rapid:   // ���˃^�C�v
			case eFireType::RRotate: // ����+��]�^�C�v
				return  L"RAPID"; // ���˃^�C�v�̃A�j���[�V�����L�[��Ԃ�
				break;

			default:
				return L"";
				break;
			}
		}
	}

	/*!
	@brief ��C
	*/
	class Cannon : public Gimmick
	{
	protected:

		float m_currentRot; // �O�t���[���܂ł̉�]�p
		Bool8_t<CannonParam::eStatus> m_status; // ��Ԑ^�U

		weak_ptr<MultiParticle> m_particle; // �p�[�e�B�N���I�u�W�F�N�g
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // �`��R���|�[�l���g

		const float m_particleTime; // �p�[�e�B�N���̍Đ�����
		const CannonParam::eFireType m_fireType; // ��C�̎��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �p�x
		@param ��C�^�C�v
		*/
		Cannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, 
			const GimmickAngle::eAngle& angle,
			const CannonParam::eFireType& fire
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale), angle),
			m_fireType(fire),
			m_particleTime(0.04f)
		{
			m_currentRot = 0.0f;
			m_modelMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, -1.0f, 0.0f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Cannon() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief ��C�̉�]�֐�
		*/
		void RotateCannon();

		/*!
		@brief ��C���ˎ��̃A�j���[�V�������s�֐�
		*/
		void FireAnimation();

		/*!
		@brief �G�t�F�N�g�X�V�֐�
		*/
		virtual void EffectUpdate();

		/*!
		@brief ��C���ˊ֐�
		*/
		void OnFire()
		{
			m_status.Set(CannonParam::eStatus::IsFire) = true; // ���˂��Ă邩�̐^�U���I��

			// �A�j���[�V������ݒ�
			m_ptrDraw->ChangeCurrentAnimation(CannonParam::GetAnimeKey(m_fireType), 0.0f);
		}

		/*!
		@brief ��C�����˂��Ă邩�̐^�U�擾�֐�
		@return ��C�����˒����̐^�U
		*/
		bool GetFire()
		{
			return m_status(CannonParam::eStatus::IsFire);
		}

		/*!
		@brief ��C�̔��ˎ��Ԏ擾�֐�
		@return ��C�̔��ˎ���(s)
		*/
		const float GetFireTime() const
		{
			return CannonParam::GetFireTime(m_fireType);
		}

		/*!
		@brief ��C�̃A�j���[�V�����o�ߎ��Ԏ擾�֐�
		@return �A�j���[�V�����o�ߎ���
		*/
		const float GetAnimationTime() const
		{
			return m_ptrDraw->GetCurrentAnimationTime();
		}
	};

	/*!
	@brief �S�[���p��C
	*/
	class GoalCannon : public Cannon
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		*/
		GoalCannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			Cannon(stagePtr, position, scale, GimmickAngle::eAngle::Uright, CannonParam::eFireType::Normal)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GoalCannon() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �G�t�F�N�g�X�V�֐�
		*/
		void EffectUpdate() override;
	};

	/*!
	@brief �^�C�g����ʗp��C
	*/
	class TitleCannon : public Cannon
	{
	private:

		// ��C�̓�Փx�^�C�v
		const CannonParam::eDifficulty m_diffType;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param ��Փx
		*/
		TitleCannon(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const CannonParam::eDifficulty& diffType
		) :
			Cannon(stagePtr, position, scale, GimmickAngle::eAngle::Up, CannonParam::eFireType::Normal),
			m_diffType(diffType)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~TitleCannon() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;
	};
}