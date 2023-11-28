#pragma once
#include "stdafx.h"
#include "BaseStage.h"

namespace basecross
{
	class TitleStage : public BaseStage
	{
		/*!
		@brief リソース読み込み関数
		*/
		void CreateResourses() override;

		/*!
		@brief ビューとライトの生成関数
		*/
		void CreateViewLight() override;

		/*!
		@brief BGMの再生関数
		*/
		void CreateBGM() override;

	public:

		/*!
		@brief コンストラクタ
		*/
		TitleStage() : BaseStage() {}

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
	};
}