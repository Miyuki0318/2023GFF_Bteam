/*!
@file NextStage.h
@brief 「次のステージへ」ステージ
*/

#pragma once
#include "BaseStage.h"
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief 「次のステージへ」ステージ
	*/
	class NextStage : public BaseStage
	{
	public :

		// ステージステート定数
		enum eStageState
		{
			FadeIn,		// フェードイン
			Select,		// 選択肢
			StageMove,	// ステージへ移動
			TitleBack,	// タイトルに戻る
			MetalFade,	// タイトルに戻る用フェード
			FadeOut,	// フェードアウト
		};

		// セレクトタイプ
		enum eSelect
		{
			Next,	// 次のステージへ
			Back,	// タイトルに戻る
		};

	private:

		float m_totalTime;		// 経過時間
		bool m_currentStickX;	// 前フレームのスティック入力X

		// セレクトタイプ
		eSelect m_select;

		// ステージステート
		eStageState m_stageState;

		// スプライト
		weak_ptr<Sprite> m_fade;		// フェード用
		weak_ptr<Sprite> m_logo;		// ロゴ
		weak_ptr<Sprite> m_next;		// 次へ
		weak_ptr<Sprite> m_back;		// 戻る
		weak_ptr<Sprite> m_metalLeft;	// メタルウィンドウ左
		weak_ptr<Sprite> m_metalRight;	// メタルウィンドウ右

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
		@brief 背景スクロール処理関数
		*/
		void SlideBackGround();

		/*!
		@brief セレクトステート
		*/
		void SelectState();

		/*!
		@brief タイトルに戻るフェードステート
		*/
		void MetalState();

		/*!
		@brief フェードインステート
		*/
		void FadeInState();

		/*!
		@brief フェードアウトステート
		*/
		void FadeOutState();

	public:

		/*!
		@brief コンストラクタ
		*/
		NextStage() : BaseStage()
		{
			m_select = Next;
			m_stageState = eStageState::FadeIn;
			m_totalTime = 0.0f;
			m_currentStickX = false;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~NextStage() {}

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

		/*!
		@brief セレクトタイプ取得関数
		@return m_select
		*/
		const eSelect& GetSelect() const
		{
			return m_select;
		}
	};
}