/*!
@file Gimmick.h
@brief �X�e�[�W�M�~�b�N�̌p����
*/

#pragma once
#include "CubeObject.h"

namespace basecross
{
	/*!
	@brief �M�~�b�N�p�p�x
	*/
	namespace GimmickAngle
	{
		// �p�xenum
		enum class eAngle : int8_t
		{
			Up,		// ��
			Down,	// ��
			Left,	// ��
			Right,	// �E
			All,	// �S����
			Uleft,	// ����
			Uright,	// �E��
			Dleft,	// ����
			Dright,	// �E��
			Invers,	// ���]
		};

		/*!
		@brief �p�x�擾�֐�
		@param eAngle
		@return �p�x
		*/
		static const Vec3 GetAngle(const eAngle& angle)
		{
			switch (angle)
			{
			case eAngle::Down:
				return Vec3(0.0f, 0.0f, 180.0f); // ������̊p�x
				break;

			case eAngle::Left:
				return Vec3(0.0f, 0.0f, 90.0f); // �������̊p�x
				break;

			case eAngle::Right:
				return Vec3(0.0f, 0.0f, -90.0f); // �E�����̊p�x
				break;

			case eAngle::Uleft:
				return Vec3(0.0f, 0.0f, 45.0f); // ��������̊p�x
				break;

			case eAngle::Uright:
				return Vec3(0.0f, 0.0f, -45.0f); // �E������̊p�x
				break;

			case eAngle::Dleft:
				return Vec3(0.0f, 0.0f, 135.0f); // ���������̊p�x
				break;

			case eAngle::Dright:
				return Vec3(0.0f, 0.0f, 225.0f); // �E�������̊p�x
				break;

			case eAngle::Invers:
				return Vec3(0.0f, 180.0f, 0.0f); // ���Ό����̊p�x
				break;

			default:
				return Vec3(0.0f); // ������A�܂��͂���ȊO�̊p�x
				break;
			}
		}
	};

	/*!
	@brief �X�e�[�W�M�~�b�N�̌p����
	*/
	class Gimmick : public CubeObject
	{
	protected:

		Mat4x4 m_modelMat; // ���f���̃g�����X�t�H�[���Ƃ̍����s��
		GimmickAngle::eAngle m_angle; // �p�x

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Gimmick(const shared_ptr<Stage>& stagePtr) :
			CubeObject(stagePtr)
		{
			m_angle = GimmickAngle::eAngle::Up;
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �A���O��
		*/
		Gimmick(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& scale, const GimmickAngle::eAngle& angle
		) :
			CubeObject(stagePtr, position, Vec3(0.0f, 0.0f, 0.0f), scale, true),
			m_angle(angle)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Gimmick() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �p�x�擾�֐�
		@return m_angle
		*/
		const GimmickAngle::eAngle& GetAngle() const
		{
			return m_angle;
		}
	};
}