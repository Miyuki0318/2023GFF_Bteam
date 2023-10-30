/*!
@file Billboard.h
@brief �r���{�[�h�I�u�W�F�N�g�̌p����
@author ���V���M
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Billboard : public GameObject �r���{�[�h�I�u�W�F�N�g�̌p����
	// ----------------------------------------------------------------------------------
	class Billboard : public GameObject
	{
	protected:

		Vec2 m_scale;					// �X�P�[��
		Vec3 m_position;				// �|�W�V����
		Vec3 m_rotation;				// ���[�e�[�V����
		wstring m_texWstr;				// �e�N�X�`���L�[
		Col4 m_diffuseColor;			// �f�B�q���[�Y�F
		Col4 m_verticesColor;			// ���_�F
		Col4 m_emissiveColor;			// �G�~�b�V�u�F
		VertexData m_vertex;	// ���_�f�[�^

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Billboard(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_scale.zero();
			m_position.zero();
			m_rotation.zero();
			m_texWstr = L"WHITE";
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		@param �X�P�[��
		@param �|�W�V����
		*/
		Billboard(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec2& scale,
			const Vec3& position
			) :
			GameObject(stagePtr),
			m_texWstr(texWstr),
			m_scale(scale),
			m_position(position)
		{
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Billboard() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �r���{�[�h�X�V�֐�
		*/
		void UpdateBillboard();

		/*!
		@brief �f�B�t���[�Y�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetDiffuseColor(const Col4& color);

		/*!
		@brief ���_�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetVerticesColor(const Col4& color);

		/*!
		@brief �G�~�b�V�u�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetEmissiveColor(const Col4& color);
		
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
		@brief �|�W�V������ύX����֐�
		@param �|�W�V����
		*/
		void SetPosition(float x, float y , float z)
		{
			SetPosition(Vec3(x, y, z));
		}

		/*!
		@brief ���[�e�[�V������ύX����֐�
		@param ���[�e�[�V����
		*/
		void SetRotation(const Vec3& rotation)
		{
			auto ptrTrans = GetComponent<Transform>();
			ptrTrans->SetRotation(rotation);
			m_rotation = rotation;
		}

		/*!
		@brief ���[�e�[�V������ύX����֐�
		@param ���[�e�[�V����
		*/
		void SetRotation(float x, float y, float z)
		{
			SetRotation(Vec3(x, y, z));
		}

		/*!
		@brief �e�N�X�`����ύX����֐�
		@param �e�N�X�`���L�[
		*/
		void SetTexture(const wstring& texWstr)
		{
			auto ptrDraw = GetComponent<PCTStaticDraw>();
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
		@brief ���[�e�[�V�����Ԃ��֐�
		@return ���[�e�[�V����
		*/
		Vec3 GetRotation() const
		{
			return m_rotation;
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