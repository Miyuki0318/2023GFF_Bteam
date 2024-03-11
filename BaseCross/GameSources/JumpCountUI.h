/*!
@file JumpCountUI.h
@brief ジャンプ可能回数UI
*/

#pragma once
#include "Sprite.h"
#include "Player.h"

namespace basecross
{
	/*!
	@brief ジャンプ回数UI
	*/
	class JumpCountUI : public Sprite
	{
		weak_ptr<Player> m_player;   // プレイヤーポインタ
		weak_ptr<Sprite> m_gauge;    // ゲージ用スプライト
		weak_ptr<Sprite> m_airShock; // エアショックアイコンスプライト

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param スケール
		@param ポジション
		*/
		JumpCountUI(const shared_ptr<Stage>& stagePtr,
			const Vec2& scale, const Vec3& position
		) :
			Sprite(stagePtr, L"JGAUGE_TX", scale, position)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~JumpCountUI() {}

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