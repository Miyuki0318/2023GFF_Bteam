/*!
@file Bumper.h
@brief �X�e�[�W�M�~�b�N�̓����ǂ̊J�p�{�^��
*/

#pragma once
#include "Gimmick.h"

namespace basecross
{
	/*!
	@brief �{�^��
	*/
	class Button : public Gimmick
	{
		// �`��R���|�[�l���g
		shared_ptr<PNTBoneModelDraw> m_ptrDraw;

		const int m_number;  // �����ǂ�ID

		bool m_active;		 // �{�^���������Ă邩�̐^�U
		bool m_current;		 // �O��܂ł̏��
		float m_reverseTime; // �A�j���[�V�����̍Đ�����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �����ǂƂ�ID
		*/
		Button(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, float scale, int number
		) :
			Gimmick(stagePtr, Vec3(position), Vec3(scale * 2.0f, scale, scale), GimmickAngle::eAngle::Up),
			m_number(number)
		{
			m_active = false;
			m_current = m_active;
			m_reverseTime = 0.0f;

			m_modelMat.affineTransformation(
				Vec3(1.35f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Button() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �{�^���Ƀ^�[�Q�b�g���ڐG���Ă��邩�擾����֐�(OnCollision�n�֐�����ɋ@�\���ĂȂ���)
		@return �Փ˂��Ă��邩�̐^�U
		*/
		bool GetHitButton() const 
		{
			for (const auto& objPtr : m_targetObj)
			{
				// �G���[�`�F�b�N
				if (!objPtr.lock()) continue;

				// �^�L���X�g
				const auto& targetPtr = dynamic_pointer_cast<TemplateObject>(objPtr.lock());
				if (!targetPtr) continue;

				// ���W�̒�`
				const Vec2 pos = Vec2(targetPtr->GetPosition());

				// �^�[�Q�b�g���W�Ƃ̋������X�P�[�����Z�����
				if ((pos - Vec2(GetPosition())).length() <= Vec2(GetScale()).length())
				{
					// ���E�[�̍��W
					const Vec2 left = Vec2(GetPosition() - GetScale() / 2.0f); 
					const Vec2 right = Vec2(GetPosition() + GetScale() / 2.0f);

					// �͈͓��Ȃ�return��Ԃ��ďI��
					if (Utility::GetBetween(pos, left, right))
					{
						return true;
					}

					// �X�P�[���̔������擾
					const Vec2 helfScale = Vec2(targetPtr->GetScale()) / 2.0f;
					
					// ���W����X�P�[�����̍��W�l�_�����߂�
					vector<Vec2> points = {
						(pos + Vec2(-helfScale.x, helfScale.y)),
						(pos - helfScale),
						(pos + helfScale),
						(pos + Vec2(helfScale.x, -helfScale.y))
					};

					// �l�_�̓��A�ǂꂩ��ł��͈͓��Ȃ�
					for (const auto& point : points)
					{
						// �͈͓��Ȃ�return��Ԃ��ďI��
						if (Utility::GetBetween(point, left, right))
						{
							return true;
						}
					}
				}
			}
			return false;
		}

		/*!
		@brief �{�^����ID�擾�֐�
		@return m_number
		*/
		int GetButtonNumber() const
		{
			return m_number;
		}

		/*!
		@brief �{�^���̓��͂����邩�̎擾�֐�
		@return m_active
		*/
		bool GetInput() const
		{
			return m_active;
		}
	};
}