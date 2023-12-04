/*!
@file Billboard.cpp
@brief �r���{�[�h�I�u�W�F�N�g�̌p����
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	/*!
	@brief �������Ɉ�x�����Ăяo�����֐�
	*/
	void Billboard::OnCreate()
	{
		// �g�����X�t�H�[���̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_position);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetScale(m_scale.x, m_scale.y, 1.0f);

		// �`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<PCTStaticDraw>();
		ptrDraw->SetOriginalMeshUse(true);
		ptrDraw->CreateOriginalMesh(m_vertex);
		ptrDraw->SetTextureResource(m_texWstr);

		// �����F�̕`����\��
		SetAlphaActive(true);
	}

	/*!
	@brief �r���{�[�h�X�V�֐�
	*/
	void Billboard::UpdateBillboard()
	{
		// �J���������Ƀr���{�[�h�֐��ŃN�H�[�^�j�I����ݒ�
		const auto& ptrCamera = GetStage()->GetView()->GetTargetCamera();
		Quat qt = Utility::GetBillboardQuat(ptrCamera->GetAt() - ptrCamera->GetEye());
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetQuaternion(qt);
	}

	/*!
	@brief �f�B�t���[�Y�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Billboard::SetDiffuseColor(const Col4& color)
	{
		// �`��R���|�[�l���g�̎擾���ăf�B�q���[�Y�F�ɐݒ�
		auto ptrDraw = GetComponent<PCTStaticDraw>();
		ptrDraw->SetDiffuse(color);

		// �F��ێ�
		m_diffuseColor = color;
	}

	/*!
	@brief �G�~�b�V�u�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Billboard::SetEmissiveColor(const Col4& color)
	{
		// �`��R���|�[�l���g�̎擾���ăG�~�b�V�u�F�ɐݒ�
		auto ptrDraw = GetComponent<PCTStaticDraw>();
		ptrDraw->SetEmissive(color);

		// �F��ێ�
		m_emissiveColor = color;
	}

	/*!
	@brief ���_�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Billboard::SetVerticesColor(const Col4& color)
	{
		// �F�𒸓_�F�ɐݒ�
		for (auto& v : m_vertex.vertices)
		{
			v.color = color;
		}

		// �`��R���|�[�l���g�̎擾���Ē��_�f�[�^�̍X�V
		auto ptrDraw = GetComponent<PCTStaticDraw>();
		ptrDraw->UpdateVertices(m_vertex.vertices);

		// �F��ێ�
		m_verticesColor = color;
	}
}