/*!
@file Number.h
@brief 数字を表示するスプライト
@author 小澤博貴
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Number : public Sprite 数字を表示するスプライトの継承元
	// ----------------------------------------------------------------------------------
	class Number : public Sprite
	{
		int m_number; // 数字

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param スケール
		@param ポジション
		*/
		Number(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position
		) :
			Sprite(stagePtr, L"NUMBER", scale, position)
		{
			m_number = 0;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Number() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief 表示する数字を変更する関数
		@param 数字
		*/
		void SetNumber(const int number);
	};
}