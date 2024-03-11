/*!
@file ShieldEffect.h
@brief プレイヤーのシールドエフェクト
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief シールドエフェクト
	*/
	class ShieldEffect : public TemplateObject
	{
		weak_ptr<TemplateObject> m_outLine;	 // アウトライン
		weak_ptr<TemplateObject> m_player;   // プレイヤーのポインタ
		shared_ptr<PNTStaticDraw> m_ptrDraw; // 描画コンポーネント

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param プレイヤーのポインタ
		*/
		ShieldEffect(const shared_ptr<Stage>& stagePtr, const shared_ptr<TemplateObject>& playerPtr) :
			TemplateObject(stagePtr, Vec3(0.0f), Vec3(0.0f), Vec3(2.0f)),
			m_player(playerPtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~ShieldEffect() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief エフェクトの更新処理関数
		*/
		void UpdateEffect();

		/*!
		@brief シールドを描画するかの設定関数
		@param bool
		*/
		void SetDrawShield(bool b);
	};

	/*!
	@brief シールドエフェクトのアウトライン
	*/
	class ShieldOutLine : public TemplateObject
	{
		shared_ptr<PNTStaticDraw> m_ptrDraw; // 描画コンポーネント

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		ShieldOutLine(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~ShieldOutLine() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 描画コンポーネント取得関数
		@return shared_ptr<PNTStaticDraw>
		*/
		const shared_ptr<PNTStaticDraw>& GetPtrDraw() const
		{
			return m_ptrDraw;
		}
	};
}