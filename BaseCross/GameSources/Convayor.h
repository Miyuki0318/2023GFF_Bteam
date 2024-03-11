/*!
@file Convayor.h
@brief �X�e�[�W�M�~�b�N�̃x���g�R���x�A
*/

#pragma once
#include "Gimmick.h"
#include "Billboard.h"

namespace basecross
{
	/*!
	@brief �x���g�R���x�A
	*/
	class Convayor : public Gimmick
	{
	public:

		// �x���g�̃^�C�venum
		enum class eBeltType : int8_t
		{
			Middle, // ����
			Side,	// �O��
		};

		// ��]�^�C�venum
		enum class eRotType : int8_t
		{
			LeftRot,  // ����]
			RightRot, // �E��]
		};

	private:

		eRotType m_rotate; // ��]����
		const eBeltType m_type; // �x���g�^�C�v
		const float m_speed; // �x���g�R���x�A�̑��x
		const array<wstring, 2> m_animeKey; // �A�j���[�V�����L�[
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // �`��R���|�[�l���g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param ��]����
		@param �x���g�^�C�v
		@param ���x
		*/
		Convayor(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale,
			const eRotType& rotate, const eBeltType& type, float speed
		) :
			Gimmick(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(scale), GimmickAngle::eAngle::Up),
			m_type(type),
			m_rotate(rotate),
			m_speed(speed),
			m_animeKey({
				L"LEFT",
				L"RIGHT"
			})
		{
			m_modelMat.affineTransformation(
				Vec3(1.35f, 1.35f, 1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)			
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Convayor() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief ��]�����ݒ�֐�
		@param eRotType
		*/
		void SetRotate(const eRotType& rot)
		{
			// ���݂̉�]�����Ƒ���Ȃ�
			if (rot != m_rotate)
			{
				m_rotate = rot; // ��]�������㏑��

				// �A�j���[�V������ύX
				m_ptrDraw->ChangeCurrentAnimation(m_animeKey.at(static_cast<size_t>(rot)));
			}
		}

		/*!
		@brief ��]�����擾�֐�
		@return eRotType
		*/
		eRotType GetRotate() const
		{
			return m_rotate;
		}

		/*!
		@brief �x���g�̑��x�擾�֐�
		@return m_speed
		*/
		const float& GetConvayorSpeed() const
		{
			return m_speed;
		}
	};

	/*!
	@brief �x���g�R���x�A�̕����K�C�h
	*/
	class ConvayorGuide : public Billboard
	{
	private:

		Convayor::eRotType m_rotate; // ��]����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param ��]����
		*/
		ConvayorGuide(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale, const Convayor::eRotType& rotate
		) :
			Billboard(stagePtr, L"GUIDE_TX", Vec2(scale), Vec3(position.x, position.y, 0.0f)),
			m_rotate(rotate)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~ConvayorGuide() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;
	};
}