/*!
@file AirJetEffect.h
@brief �v���C���[�̃G�A�W�F�b�g�G�t�F�N�g
*/

#pragma once
#include "Billboard.h"

namespace basecross
{
	/*!
	@brief �G�A�W�F�b�g�G�t�F�N�g
	*/
	class AirJetEffect : public Billboard
	{
		// �v���C���[�I�u�W�F�N�g�̃|�C���^
		weak_ptr<TemplateObject> m_player;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �v���C���[�̃|�C���^
		*/
		AirJetEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<TemplateObject>& playerPtr) :
			Billboard(stagePtr, L"EFFECT_TX", Vec2(0.0f), Vec3(0.0f)),
			m_player(playerPtr)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~AirJetEffect() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �G�t�F�N�g�̍X�V�����֐�
		*/
		void UpdateEffect();
	};
}