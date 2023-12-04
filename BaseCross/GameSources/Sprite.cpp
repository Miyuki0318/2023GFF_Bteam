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
	@brief �F�͈̔͂�0.0f�`1.0f�̊ԂɂȂ��Ă邩�̏C���֐�
	@param �F
	*/
	void Sprite::CorrectionColor(Col4& color)
	{
		// �F�͈̔͂�0.0f�`1.0f�̊ԂɂȂ��Ă邩�̏C��
		for (int i = 0; i < 4; i++)
		{
			color.setElem(i, min(color.getElem(i), 1.0f));
			color.setElem(i, max(color.getElem(i), 0.0f));
		}
	}

	/*!
	@brief �f�B�t���[�Y�F���g���ĐF��ύX����֐�
	@param �F
	*/
	void Sprite::SetDiffuseColor(Col4& color)
	{
		// �F�̏C��
		CorrectionColor(color);

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
	void Sprite::SetEmissiveColor(Col4& color)
	{
		// �F�̏C��
		CorrectionColor(color);

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
	void Sprite::SetVerticesColor(Col4& color)
	{
		// �F�̏C��
		CorrectionColor(color);

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

	/*!
	@brief �t�F�[�h�C����������֐�
	@param �t�F�[�h�Ɋ|���鎞��
	@return �t�F�[�h�������������̐^�U
	*/
	bool Sprite::FadeInColor(const float time)
	{
		// ���ɓ����Ȃ�I��
		if (m_diffuseColor.w >= 1.0f) return true;

		// �F�̎擾
		Col4 color = m_diffuseColor;

		// �F�̒l���f���^�^�C���ŉ��Z����
		color.w += DELTA_TIME / time;

		// ���b�V���̃f�B�t���[�Y�F��ݒ�
		SetDiffuseColor(color);

		// �t�F�[�h�������������̐^�U��Ԃ�
		return color.w >= 1.0f;
	}

	/*!
	@brief �t�F�[�h�C����������֐�
	@param �t�F�[�h�̏I���l
	@param �t�F�[�h�Ɋ|���鎞��
	@return �t�F�[�h�������������̐^�U
	*/
	bool Sprite::FadeInColor(const float time, const float alpha)
	{
		// ���ɓ����Ȃ�I��
		if (m_diffuseColor.w >= alpha) return true;

		// �F�̎擾
		Col4 color = m_diffuseColor;

		// �F�̒l���f���^�^�C���ŉ��Z����
		color.w += DELTA_TIME / time;

		// ���b�V���̃f�B�t���[�Y�F��ݒ�
		SetDiffuseColor(color);

		// �t�F�[�h�������������̐^�U��Ԃ�
		return color.w >= alpha;
	}

	/*!
	@brief �t�F�[�h�C����������֐�
	@param �t�F�[�h�Ɋ|���鎞��
	@return �t�F�[�h�������������̐^�U
	*/
	bool Sprite::FadeOutColor(const float time)
	{
		// ���ɓ����Ȃ�I��
		if (m_diffuseColor.w <= 0.0f) return true;

		// �F�̎擾
		Col4 color = m_diffuseColor;

		// �F�̒l���f���^�^�C���ŉ��Z����
		color.w -= DELTA_TIME / time;

		// ���b�V���̃f�B�t���[�Y�F��ݒ�
		SetDiffuseColor(color);

		// �t�F�[�h�������������̐^�U��Ԃ�
		return color.w <= 0.0f;
	}

	/*!
	@brief �t�F�[�h�C����������֐�
	@param �t�F�[�h�̏I���l
	@param �t�F�[�h�Ɋ|���鎞��
	@return �t�F�[�h�������������̐^�U
	*/
	bool Sprite::FadeOutColor(const float time, const float alpha)
	{
		// ���ɓ����Ȃ�I��
		if (m_diffuseColor.w <= alpha) return true;

		// �F�̎擾
		Col4 color = m_diffuseColor;

		// �F�̒l���f���^�^�C���ŉ��Z����
		color.w -= DELTA_TIME / time;

		// ���b�V���̃f�B�t���[�Y�F��ݒ�
		SetDiffuseColor(color);

		// �t�F�[�h�������������̐^�U��Ԃ�
		return color.w <= alpha;
	}
}