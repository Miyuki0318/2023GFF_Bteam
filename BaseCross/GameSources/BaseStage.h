#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "DebugObject.h"

namespace basecross
{
	class BaseStage : public Stage
	{
	protected:

		// SE管理構造体
		struct SE
		{
			weak_ptr<SoundItem> item; // サウンドアイテム
			wstring seKey = L""; // ファイルキー

			// コンストラクタ
			SE(const weak_ptr<SoundItem>& ptr, const wstring& key
			) :
				item(ptr),
				seKey(key)
			{
			}

			// コンストラクタ
			SE(const wstring& key, const weak_ptr<SoundItem>& ptr
			) :
				item(ptr),
				seKey(key)
			{
			}

		public:

			// リセット関数
			void Reset()
			{
				item.reset();
				seKey = L"";
			}
		};

		// CSVデータ
		vector<vector<string>> m_csvData;

		// ビュー
		shared_ptr<SingleView> m_gameView;

		// BGM・SE
		weak_ptr<SoundItem> m_bgm;
		vector<SE> m_seList;

		// 背景オブジェクト
		weak_ptr<DebugObject> m_backObj;

		/*!
		@brief リソース読み込み関数
		*/
		virtual void CreateResourses();

		/*!
		@brief ビューとライトの生成関数
		*/
		virtual void CreateViewLight();

		/*!
		@brief プレイヤーの生成関数
		*/
		virtual void CreatePlayer();

		/*!
		@brief ステージブロックとギミックの生成関数
		*/
		void CreateStage(const string& fileName);
		void CreateEnemy(const string& fileName);
		void CreateInstanceBlock(const string& fileName);

	public:

		/*!
		@brief コンストラクタ
		*/
		BaseStage() :Stage() {}

		/*!
		@brief デストラクタ
		*/
		virtual ~BaseStage() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnDestroy() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief 描画更新関数
		*/
		virtual void OnDraw() override;

		/*!
		@brief BGMの再生関数
		*/
		virtual void CreateBGM(const wstring& bgmKey, float volume);

		/*!
		@brief SEの再生関数
		*/
		virtual void CreateSE(const wstring& seKey, float volume);

		/*!
		@brief SEの再生関数
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief ゲームビュー取得関数
		@return const shared_ptr<SingleView>
		*/
		const shared_ptr<SingleView>& GetGameView() const
		{
			return m_gameView;
		}
	};
}