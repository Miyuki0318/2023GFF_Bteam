/*!
@file ArrowEffect.h
@brief �v���C���[�̃G�A�W�F�b�g�ł̗\���O��
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �G�A�W�F�b�g�ł̗\���O��
	*/
	class ArrowEffect : public TemplateObject
	{
		VertexData m_vertex; // ���_�f�[�^�\����
		weak_ptr<TemplateObject> m_player; // �v���C���[�̃|�C���^
		shared_ptr<PCTStaticDraw> m_ptrDraw; // �`��R���|�[�l���g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �v���C���[�̃|�C���^
		*/
		ArrowEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<TemplateObject>& playerPtr) :
			TemplateObject(stagePtr),
			m_player(playerPtr)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~ArrowEffect() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �G�t�F�N�g�̍X�V�����֐�
		@param �O���̃G�t�F�N�g�ɕK�v�Ȑ߂̍��W
		*/
		void UpdateEffect(const vector<Vec3>& points);
	};
}