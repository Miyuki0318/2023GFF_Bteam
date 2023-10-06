/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage 
	{
		// リソースの読み込み
		void CreateResourses();

		// ビューの作成
		void CreateViewLight();

		// 地面の作成
		void CreateGround();

	public:

		GameStage() :Stage() {}

		/*!
		@brief デストラクタ
		*/
		virtual ~GameStage() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		///*!
		//@brief 毎フレーム度に呼び出される関数
		//*/
		//virtual void OnUpdate() override;

		/*!
		@brief 描画更新関数
		*/
		virtual void OnDraw() override;
	};
}