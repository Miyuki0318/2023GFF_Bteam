#pragma once
#include "stdafx.h"

namespace basecross
{
	class TitleStage : public Stage
	{
		// ビュー
		shared_ptr<SingleView> m_gameView;

		// BGM
		weak_ptr<SoundItem> m_bgm;

		// リソースの読み込み
		void CreateResourses();

		// ビューの作成
		void CreateViewLight();

		// BGMの再生
		void CreateBGM();

		// 地面の作成
		void CreatePlayer();

		// ステージの作成
		void CreateStage();
		void CreateInstanceBlock();

	public:

		/*!
		@brief コンストラクタ
		*/
		TitleStage() : Stage() {}

		/*!
		@brief デストラクタ
		*/
		virtual ~TitleStage() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief 描画更新関数
		*/
		virtual void OnDraw() override;
	};
}