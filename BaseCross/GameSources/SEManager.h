#pragma once
#include "stdafx.h"

namespace basecross
{
	class SEManager
	{
	public:

		// SE�Ǘ��\����
		struct SE
		{
			weak_ptr<SoundItem> item; // �T�E���h�A�C�e��
			wstring seKey = L"";	  // �t�@�C���L�[
			const void* objectPtr = nullptr;   // �I�u�W�F�N�g�̃|�C���^


			// �R���X�g���N�^
			SE(const weak_ptr<SoundItem>& ptr, const wstring& key, const void* objPtr
			) :
				item(ptr),
				seKey(key),
				objectPtr(objPtr)
			{
			}

			// �R���X�g���N�^
			SE(const wstring& key, const weak_ptr<SoundItem>& ptr, const void* objPtr
			) :
				item(ptr),
				seKey(key),
				objectPtr(objPtr)
			{
			}

		public:

			// ���Z�b�g�֐�
			void Reset()
			{
				item.reset();
				seKey = L"";
				objectPtr = nullptr;
			}
		};

	private:

		vector<SE> m_seList;

	public:

		SEManager() {}

		~SEManager() 
		{
			m_seList.clear();
		}

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void StartSE(const wstring& seKey, float volume, const void* objPtr);

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void StopSE(const wstring& seKey, const void* objPtr);
	};
}