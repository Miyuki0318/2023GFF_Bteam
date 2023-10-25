/*!
@file Sprite.cpp
@brief �X�v���C�g�e�N���X
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	/*!
	@brief �������Ɉ�x�����Ăяo�����֐�
	*/
	void Sprite::OnCreate()
	{
		// �g�����X�t�H�[���̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_scale.x, m_scale.y, 1.0f);
		ptrTrans->SetRotation(Vec3(0.0f));
		ptrTrans->SetPosition(m_position);

		// �`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<PCTSpriteDraw>(m_vertex);
		ptrDraw->SetTextureResource(m_texWstr);
		m_verticesColor = COL_WHITE;
		m_diffuseColor = ptrDraw->GetDiffuse();
		m_emissiveColor = ptrDraw->GetEmissive();

		// �����F�̕`��ݒ�
		SetAlphaActive(true);
	}

	/*!
	@brief �f�B�t���[�Y�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Sprite::SetDiffuseColor(const Col4& color)
	{
		// �`��R���|�[�l���g�̎擾���ăf�B�q���[�Y�F�ɐݒ�
		auto ptrDraw = GetComponent<PCTSpriteDraw>();
		ptrDraw->SetDiffuse(color);

		// �F��ێ�
		m_diffuseColor = color;
	}

	/*!
	@brief �G�~�b�V�u�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Sprite::SetEmissiveColor(const Col4& color)
	{
		// �`��R���|�[�l���g�̎擾���ăG�~�b�V�u�F�ɐݒ�
		auto ptrDraw = GetComponent<PCTSpriteDraw>();
		ptrDraw->SetEmissive(color);

		// �F��ێ�
		m_emissiveColor = color;
	}

	/*!
	@brief ���_�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Sprite::SetVerticesColor(const Col4& color)
	{
		// �F�𒸓_�F�ɐݒ�
		for (auto& v : m_vertex.vertices)
		{
			v.color = color;
		}

		// �`��R���|�[�l���g�̎擾���Ē��_�f�[�^�̍X�V
		auto ptrDraw = GetComponent<PCTSpriteDraw>();
		ptrDraw->UpdateVertices(m_vertex.vertices);

		// �F��ێ�
		m_verticesColor = color;
	}
}