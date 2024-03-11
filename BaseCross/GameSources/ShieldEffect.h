/*!
@file ShieldEffect.h
@brief �v���C���[�̃V�[���h�G�t�F�N�g
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �V�[���h�G�t�F�N�g
	*/
	class ShieldEffect : public TemplateObject
	{
		weak_ptr<TemplateObject> m_outLine;	 // �A�E�g���C��
		weak_ptr<TemplateObject> m_player;   // �v���C���[�̃|�C���^
		shared_ptr<PNTStaticDraw> m_ptrDraw; // �`��R���|�[�l���g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �v���C���[�̃|�C���^
		*/
		ShieldEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<TemplateObject>& playerPtr) :
			TemplateObject(stagePtr, Vec3(0.0f), Vec3(0.0f), Vec3(2.0f)),
			m_player(playerPtr)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~ShieldEffect() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �G�t�F�N�g�̍X�V�����֐�
		*/
		void UpdateEffect();

		/*!
		@brief �V�[���h��`�悷�邩�̐ݒ�֐�
		@param bool
		*/
		void SetDrawShield(bool b);
	};

	/*!
	@brief �V�[���h�G�t�F�N�g�̃A�E�g���C��
	*/
	class ShieldOutLine : public TemplateObject
	{
		shared_ptr<PNTStaticDraw> m_ptrDraw; // �`��R���|�[�l���g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		ShieldOutLine(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~ShieldOutLine() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �`��R���|�[�l���g�擾�֐�
		@return shared_ptr<PNTStaticDraw>
		*/
		const shared_ptr<PNTStaticDraw>& GetPtrDraw() const
		{
			return m_ptrDraw;
		}
	};
}