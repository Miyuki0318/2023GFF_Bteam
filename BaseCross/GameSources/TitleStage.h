/*!
@file TitleStage.h
@brief タイトル用ステージ
*/

#pragma once
#include "BaseStage.h"
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief タイトル用のステージ
	*/
	class TitleStage : public BaseStage
	{
	public:

		// ステージステート定数
		enum eStageState
		{
			FadeIn,		  // フェードイン
			PushButton,	  // プッシュボタン
			MetalFadeOut, // メタルウィンドウ
			StartMove,	  // 開始時の移動演出
			ModeSelect,   // 難易度選択
			CannonStanby, // 大砲待機
			FadeOut,      // フェードアウト
		};

	private:

		// ステージステート
		eStageState m_stageState;

		// プレイヤー
		weak_ptr<TemplateObject> m_player;

		// スプライト
		weak_ptr<Sprite> m_fade;		// フェード用スプライト
		weak_ptr<Sprite> m_titleLogo;	// タイトルロゴ
		weak_ptr<Sprite> m_metalLeft;	// メタルウィンドウ左
		weak_ptr<Sprite> m_metalRight;	// メタルウィンドウ右
		weak_ptr<Sprite> m_pushButton;	// プッシュボタン

		/*!
		@brief リソース読み込み関数
		*/
		void CreateResourses() override;

		/*!
		@brief ビューとライトの生成関数
		*/
		void CreateViewLight() override;

		/*!
		@brief ビューとライトの生成関数
		*/
		void CreateBackGround();

		/*!
		@brief プレイヤーの生成関数
		*/
		void CreatePlayer() override;

		/*!
		@brief スプライトの生成関数
		*/
		void CreateSprites();

		/*!
		@brief メタルウィンドウステート
		*/
		void MetalFadeState();

		/*!
		@brief プッシュボタンステート
		*/
		void PushButtonState();

		/*!
		@brief 開始時の移動演出ステート
		*/
		void StartMoveState();

		/*!
		@brief フェードアウトステート
		*/
		void FadeOutState();

	public:

		/*!
		@brief コンストラクタ
		*/
		TitleStage() : BaseStage() 
		{
			m_stageState = eStageState::FadeIn;
		}

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
		@brief ステージステート設定関数
		@param eStageState
		*/
		void SetStageState(const eStageState& state)
		{
			m_stageState = state;
		}

		/*!
		@brief ステージステート取得関数
		@return m_stageState
		*/
		const eStageState& GetStageState() const
		{
			return m_stageState;
		}
	};
}