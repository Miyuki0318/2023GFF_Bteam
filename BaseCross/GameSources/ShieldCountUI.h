/*!
@file ShieldCountUI.h
@brief シールド枚数UI
*/

#pragma once
#include "Sprite.h"
#include "Player.h"

namespace basecross 
{
	/*!
	@brief シールドのUI
	*/
	class ShieldCountUI: public Sprite
	{
		weak_ptr<Player> m_player; // プレイヤーのポインタ
		weak_ptr<Sprite> m_gauge;  // シールドゲージ
		weak_ptr<Sprite> m_ring;   // リング取得数ゲージ
		weak_ptr<Sprite> m_shield; // シールドアイコン
		weak_ptr<Sprite> m_circle; // シールドアイコンエフェクト

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param スケール
		@param ポジション
		*/
		ShieldCountUI(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position
		) :
			Sprite(stagePtr, L"SGAUGE_TX", scale, position)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~ShieldCountUI() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief UIの更新処理関数
		*/
		void UpdateUI();
	};
}