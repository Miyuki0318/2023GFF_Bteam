#pragma once
#include "stdafx.h"

namespace basecross
{
	class BaseStage : public Stage
	{
	protected:

		// ビュー
		shared_ptr<SingleView> m_gameView;

		// BGM
		weak_ptr<SoundItem> m_bgm;

		/*!
		@brief リソース読み込み関数
		*/
		virtual void CreateResourses();

		/*!
		@brief ビューとライトの生成関数
		*/
		virtual void CreateViewLight();

		/*!
		@brief BGMの再生関数
		*/
		virtual void CreateBGM();

		/*!
		@brief プレイヤーの生成関数
		*/
		virtual void CreatePlayer();

		/*!
		@brief ステージブロックとギミックの生成関数
		*/
		void CreateStage(const string& fileName);
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
		@brief ゲームビュー取得関数
		@return const shared_ptr<SingleView>
		*/
		const shared_ptr<SingleView>& GetGameView() const
		{
			return m_gameView;
		}
	};
}