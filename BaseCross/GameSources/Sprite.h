/*!
@file Sprite.h
@brief �X�v���C�g�e�N���X
@author ���V���M
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Sprite : public GameObject �P���ȕ\�����s���X�v���C�g�e�N���X
	// ----------------------------------------------------------------------------------
	class Sprite : public GameObject
	{
	protected:

		Vec2 m_scale;					// �X�P�[��
		Vec3 m_position;				// �|�W�V����
		Col4 m_diffuseColor;			// �f�B�q���[�Y�F
		Col4 m_verticesColor;			// ���_�F
		Col4 m_emissiveColor;			// �G�~�b�V�u�F
		wstring m_texWstr;				// �e�N�X�`���L�[

		VertexData m_vertex;	// ���_�f�[�^

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		@param �����F�̕`��
		@param �X�P�[��
		@param �|�W�V����
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr,	const Vec2& scale, const Vec3& pos
		) :
			GameObject(stagePtr),
			m_texWstr(texWstr),
			m_scale(scale),
			m_position(pos)
		{
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Sprite() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;
		
		/*!
		@brief �F�͈̔͂�0.0f�`1.0f�̊ԂɂȂ��Ă邩�̏C���֐�
		@param �F
		*/
		void CorrectionColor(Col4& color);

		/*!
		@brief �f�B�t���[�Y�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetDiffuseColor(Col4& color);

		/*!
		@brief ���_�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetVerticesColor(Col4& color);

		/*!
		@brief �G�~�b�V�u�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetEmissiveColor(Col4& color);

		/*!
		@brief �t�F�[�h�C����������֐�
		@param �t�F�[�h�Ɋ|���鎞��
		@return �t�F�[�h�������������̐^�U
		*/
		bool FadeInColor(const float time);

		/*!
		@brief �t�F�[�h�C����������֐�
		@param �t�F�[�h�Ɋ|���鎞��
		@param �t�F�[�h�̏I���l
		@return �t�F�[�h�������������̐^�U
		*/
		bool FadeInColor(const float time, const float alpha);

		/*!
		@brief �t�F�[�h�C����������֐�
		@param �t�F�[�h�Ɋ|���鎞��
		@return �t�F�[�h�������������̐^�U
		*/
		bool FadeOutColor(const float time);

		/*!
		@brief �t�F�[�h�C����������֐�
		@param �t�F�[�h�Ɋ|���鎞��
		@param �t�F�[�h�̏I���l
		@return �t�F�[�h�������������̐^�U
		*/
		bool FadeOutColor(const float time, const float alpha);

		/*!
		@brief �X�P�[����ύX����֐�
		@param scale
		*/
		void SetScale(const Vec2& scale)
		{
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetScale(scale.x, scale.y, 1.0f);
			m_scale = scale;
		}

		/*!
		@brief �X�P�[����ύX����֐�
		@param x
		@param y
		*/
		void SetScale(const float x, const float y)
		{
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetScale(x, y, 1.0f);
			m_scale.x = x;
			m_scale.y = y;
		}

		/*!
		@brief �|�W�V������ύX����֐�
		@param �|�W�V����
		*/
		void SetPosition(const Vec3& position)
		{
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetPosition(position);
			m_position = position;
		}

		/*!
		@brief �e�N�X�`����ύX����֐�
		@param �e�N�X�`���L�[
		*/
		void SetTexture(const wstring& texWstr)
		{
			auto ptrDraw = GetComponent<PCTSpriteDraw>();
			ptrDraw->SetTextureResource(texWstr);
			m_texWstr = texWstr;
		}

		/*!
		@brief �X�P�[����Ԃ��֐�
		@return �X�P�[��
		*/
		Vec2 GetScale() const
		{
			return m_scale;
		}

		/*!
		@brief �|�W�V������Ԃ��֐�
		@return �|�W�V����
		*/
		Vec3 GetPosition() const
		{
			return m_position;
		}

		/*!
		@brief �f�B�t���[�Y�F��Ԃ��֐�
		@return �F
		*/
		Col4 GetDiffuseColor() const
		{
			return m_diffuseColor;
		}

		/*!
		@brief ���_�F��Ԃ��֐�
		@return �F
		*/
		Col4 GetVerticesColor() const
		{
			return m_verticesColor;
		}

		/*!
		@brief �G�~�b�V�u�F��Ԃ��֐�
		@return �F
		*/
		Col4 GetEmissiveColor() const
		{
			return m_emissiveColor;
		}

		/*!
		@brief �e�N�X�`���L�[��Ԃ��֐�
		@return �e�N�X�`���L�[
		*/
		wstring GetTextureKey() const
		{
			return m_texWstr;
		}
	};
}