/*!
@file block.h
@brief �u���b�N�̃R���W�����ƕ`��
*/

#pragma once
#include "CubeObject.h"

namespace basecross
{
	/*!
	@brief �����̃R���W�����I�u�W�F�N�g
	*/
	class Alpha : public CubeObject
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		*/
		Alpha(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), true)
		{
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �^�C�v
		*/
		Alpha(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const Vec3& scale, const CubeParam::eCubeType& type
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), scale, type, true)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Alpha() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;
	};

	/*!
	@brief �Փ˂����玀�S������p�̃R���W����
	*/
	class DeathColl : public CubeObject
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �R���W�����̗L��
		*/
		DeathColl(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), true)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~DeathColl() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;
	};

	/*!
	@brief ���o���͏Փˏ������s��Ȃ��R���W����
	*/
	class StagingColl : public CubeObject
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		*/
		StagingColl(const shared_ptr<Stage>& stagePtr,
			const Vec2& position, const float scale
		) :
			CubeObject(stagePtr, Vec3(position.x, position.y, 0.0f), Vec3(0.0f), Vec3(scale), true)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~StagingColl() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;
	};

	/*!
	@brief �u���b�N�̃C���X�^���X�`��
	*/
	class InstanceBlock : public GameObject
	{
	public:

		// �e�N�X�`���^�C�venum
		enum eTextureType : size_t
		{
			Iron,	// �S
			Metal,	// �|
			Dark,	// ���|
			Size,	// enum�̃T�C�Y�w��q
		};

	protected:

		const int m_size;			// CSV�̍s�̃T�C�Y
		const int m_rowNum;			// CSV�̍s�̐��l
		const eTextureType m_type;	// �e�N�X�`���^�C�v
		const vector<int> m_data;	// �u���b�N�̒l�̂ݕێ������z��

		// �e�N�X�`���z��
		const array<wstring, static_cast<size_t>(eTextureType::Size)> m_textures;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �u���b�N�̒l�̂ݕێ������z��
		@param �e�N�X�`���^�C�v
		@param CSV�̍s�̃T�C�Y
		@param CSV�̍s�̐��l
		*/
		InstanceBlock(const shared_ptr<Stage>& stagePtr,
			const vector<int>& data,
			const eTextureType type,
			const int size,
			const int rowNum
		) :
			GameObject(stagePtr),
			m_data(data),
			m_type(type),
			m_size(size),
			m_rowNum(rowNum),
			m_textures({ 
				L"GRASS_TX",
				L"DIRT_TX",
				L"ROCK_TX",
			})
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~InstanceBlock() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;
	};

	/*!
	@brief �X���[�v�̃C���X�^���X�`��
	*/
	class InstanceSlope : public InstanceBlock
	{
	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �u���b�N�̒l�̂ݕێ������z��
		@param �e�N�X�`���^�C�v
		@param CSV�̍s�̃T�C�Y
		@param CSV�̍s�̐��l
		*/
		InstanceSlope(const shared_ptr<Stage>& stagePtr,
			const vector<int>& data,
			const eTextureType type,
			const int size,
			const int rowNum
		) :
			InstanceBlock(stagePtr, data, type, size, rowNum)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~InstanceSlope() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;
	};
}