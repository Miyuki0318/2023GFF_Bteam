#pragma once
#include "stdafx.h"
#include "Sprite.h"

namespace basecross
{
	class BaseStage : public Stage
	{
	protected:

		// SE�Ǘ��\����
		struct SE
		{
			weak_ptr<SoundItem> item; // �T�E���h�A�C�e��
			wstring seKey = L""; // �t�@�C���L�[

			// �R���X�g���N�^
			SE(const weak_ptr<SoundItem>& ptr, const wstring& key
			) :
				item(ptr),
				seKey(key)
			{
			}

			// �R���X�g���N�^
			SE(const wstring& key, const weak_ptr<SoundItem>& ptr
			) :
				item(ptr),
				seKey(key)
			{
			}

		public:

			// ���Z�b�g�֐�
			void Reset()
			{
				item.reset();
				seKey = L"";
			}
		};

		// �r���[
		shared_ptr<SingleView> m_gameView;

		// BGM�ESE
		weak_ptr<SoundItem> m_bgm;
		vector<SE> m_seList;

		/*!
		@brief ���\�[�X�ǂݍ��݊֐�
		*/
		virtual void CreateResourses();

		/*!
		@brief �r���[�ƃ��C�g�̐����֐�
		*/
		virtual void CreateViewLight();

		/*!
		@brief �v���C���[�̐����֐�
		*/
		virtual void CreatePlayer();

		void CreateEnemy(const string& fileName);

		/*!
		@brief �X�e�[�W�u���b�N�ƃM�~�b�N�̐����֐�
		*/
		void CreateStage(const string& fileName);
		void CreateInstanceBlock(const string& fileName);

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		BaseStage() :Stage() {}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~BaseStage() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnDestroy() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;

		/*!
		@brief BGM�̍Đ��֐�
		*/
		virtual void CreateBGM(const wstring& bgmKey, float volume);

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void CreateSE(const wstring& seKey, float volume);

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief �Q�[���r���[�擾�֐�
		@return const shared_ptr<SingleView>
		*/
		const shared_ptr<SingleView>& GetGameView() const
		{
			return m_gameView;
		}
	};
}