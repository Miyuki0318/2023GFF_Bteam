#pragma once
#include "stdafx.h"

namespace basecross
{
	class SEManager
	{
	public:

		// SE管理構造体
		struct SE
		{
			weak_ptr<SoundItem> item; // サウンドアイテム
			wstring seKey = L"";	  // ファイルキー
			const void* objectPtr = nullptr;   // オブジェクトのポインタ


			// コンストラクタ
			SE(const weak_ptr<SoundItem>& ptr, const wstring& key, const void* objPtr
			) :
				item(ptr),
				seKey(key),
				objectPtr(objPtr)
			{
			}

			// コンストラクタ
			SE(const wstring& key, const weak_ptr<SoundItem>& ptr, const void* objPtr
			) :
				item(ptr),
				seKey(key),
				objectPtr(objPtr)
			{
			}

		public:

			// リセット関数
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
		@brief SEの再生関数
		*/
		virtual void StartSE(const wstring& seKey, float volume, const void* objPtr);

		/*!
		@brief SEの再生関数
		*/
		virtual void StopSE(const wstring& seKey, const void* objPtr);
	};
}