/*!
@file Rabbit.h
@brief ���J�E�T�M
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �G�L�����̃��J�E�T�M�p����
	*/
	class Rabbit : public TemplateObject
	{
	public:

		// �E�T�M�̍s���X�e�[�genum
		enum class eState : int8_t
		{
			Patrol,		// �p�g���[��
			Seek,		// �ǐ�
			LostSight,	// ������
			CannonJump,	// ��C�W�����v
			Death,		// ���S
		};

		enum class eStatus : uint8_t
		{
			IsAir,		// �󒆂�
			IsDeath,	// ���񂾂�
			IsCannon,	// ��C�ҋ@����
		};

	protected:

		const Vec2 m_jumpVelo; // �W�����v���̈ړ���
			
		eState m_state;    // �X�e�[�g
		Mat4x4 m_modelMat; // ���f���ƃg�����X�t�H�[���̍����s��
		Vec2 m_velocity;   // �ړ���

		weak_ptr<TemplateObject> m_targetObj;	// �^�[�Q�b�g�|�C���^
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // �`��R���|�[�l���g
		shared_ptr<CollisionObb> m_ptrColl;		// �R���W����OBB�R���|�[�l���g

		const float m_speed;	// ���x
		const float m_maxAcsel;	// �ő�����x
		const float m_range;	// �˒�

		float m_acsel;   // �����x
		float m_dir;	 // ����
		float m_gravity; // �d��

		Bool8_t<eStatus> m_status; // ��ԃX�e�[�^�X

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		*/
		Rabbit(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale
		) :
			TemplateObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f, XM_PI, 0.0f), Vec3(scale)),
			m_speed(3.0f),
			m_maxAcsel(5.0f),
			m_range(15.0f),
			m_jumpVelo(1.0f, -2.0f)
		{
			m_velocity = Vec2(-1.0f, 0.0f);
			m_acsel = 1.0f;
			m_dir = -1.0f;
			m_gravity = -5.0f;
			m_state = eState::Patrol;
			m_status.Set(eStatus::IsAir) = true;
			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Rabbit() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �E�T�M�̈ړ��֐�
		*/
		virtual void MoveRabbit();

		/*!
		@brief �E�T�M�̈ړ��ʌ����֐�
		*/
		void MoveReduction();

		/*!
		@brief �X�e�[�g�擾�֐�
		@return eState
		*/
		const eState& GetState() const
		{
			return m_state;
		}

		/*!
		@brief �X�e�[�g�ݒ�֐�
		@param eState
		*/
		void SetState(const eState& state)
		{
			m_state = state;
		}

		/*!
		@brief ���S�������̐^�U�擾�֐�
		@return m_status(eStatus::IsDeath)
		*/
		bool GetIsDeath() const
		{
			return m_status(eStatus::IsDeath);
		}

		/*!
		@brief �ړ��ݒ�֐�
		@param �ړ���
		@param �����x
		*/
		virtual void SetMoveValue(const Vec2& velocity, float acsel)
		{
			m_velocity = velocity;
			m_acsel = acsel < 1.0f ? 1.0f : acsel;
		}
	};
}