/*!
@file SEManager.h
@brief SE管理クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief SEマネージャー
	*/
	class SEManager
	{
	public:

		// SE管理構造体
		struct SE
		{
			weak_ptr<SoundItem> item; // サウンドアイテム
			wstring seKey = L"";	  // ファイルキー
			const void* objectPtr = nullptr; // オブジェクトのポインタ

			/*!
			@brief コンストラクタ
			@param サウンドアイテム
			@param サウンドキー
			@param 呼び出し元ポインタ
			*/
			SE(const weak_ptr<SoundItem>& ptr, const wstring& key, const void* objPtr
			) :
				item(ptr),
				seKey(key),
				objectPtr(objPtr)
			{
			}

		public:

			/*!
			@brief リセット関数
			*/
			void Reset()
			{
				item.reset();
				seKey = L"";
				objectPtr = nullptr;
			}
		};

	private:

		vector<SE> m_seList; // SE管理配列

	public:

		/*!
		@brief コンストラクタ
		*/
		SEManager() {}

		/*!
		@brief デストラクタ
		*/
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