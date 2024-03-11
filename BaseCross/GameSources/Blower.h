/*!
@file Blower.h
@brief �X�e�[�W�M�~�b�N�̑����@
*/

#pragma once
#include "Gimmick.h"

namespace basecross
{
	/*!
	@brief �����@
	*/
	class Blower : public Gimmick
	{
		shared_ptr<PNTBoneModelDraw> m_ptrDraw; // �`��R���|�[�l���g
		const float m_range; // �����͈�

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �p�x
		@param �͈�
		*/
		Blower(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const Vec3& scale,
			const GimmickAngle::eAngle& angle, const float length
		) :
			Gimmick(stagePtr, Vec3(position), scale, angle),
			m_range(length)
		{
			m_modelMat.affineTransformation(
				Vec3(0.4f, 2.0f, 0.4f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Blower() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �����͈͂ɋ��邩�̐^�U�擾�֐�
		@param ���肷����W
		@return ���邩�̐^�U
		*/
		bool CheckBetween(const Vec3& targetPos) const;
	};
}