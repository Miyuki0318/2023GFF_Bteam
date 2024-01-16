/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"
#include "BaseStage.h"
#include "GameCamera.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public BaseStage 
	{
	public:

		// ステージステート定数
		enum eStageState
		{
			FadeIn,
			StartMove,
			GameNow,
			Goal,
			DeathDrop,
			Death,
			Select,
			Metal,
			Reset,
			FadeOut,
		};

		// セレクトタイプ
		enum eSelect
		{
			Continue,
			TitleBack,
		};

	private:

		float m_totalTime;		// 経過時間
		bool m_currentStickX;	// 前フレームのスティック入力X

		// ステージステート
		eStageState m_stageState;

		// セレクトタイプ
		eSelect m_select;

		// スプライト
		weak_ptr<Sprite> m_fade;
		weak_ptr<Sprite> m_metalLeft;
		weak_ptr<Sprite> m_metalRight;
		weak_ptr<Sprite> m_gameOver;
		weak_ptr<Sprite> m_continue;
		weak_ptr<Sprite> m_titleBack;

		// ステージパス
		string m_stagePath;

		// クリアしたかの真偽
		bool m_isClear;

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
		@brief UIの生成関数
		*/
		void CreateUI();

		/*!
		@brief 死亡時のフェードステート
		*/
		void DeathFadeState();

		/*!
		@brief セレクトステート
		*/
		void SelectState();

		/*!
		@brief メタルウィンドウステート
		*/
		void MetalState();

		/*!
		@brief コンティニューリセットステート
		*/
		void ResetState();

		/*!
		@brief フェードアウトステート
		@param フェード時間
		*/
		void FadeOutState(float fadeTime);

		/*!
		@brief ステージのリセット
		*/
		void ResetStage();

	public:

		/*!
		@brief コンストラクタ
		*/
		GameStage(const string& stageName) :BaseStage() 
		{
			m_select = Continue;
			m_stageState = FadeIn;
			m_stagePath = stageName;
			m_totalTime = 0.0f;
			m_isClear = false;
			m_currentStickX = false;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GameStage() {}

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
		@brief ゲームカメラの取得関数
		@return dynamic_pointer_cast<GameCamera>(m_gameView->GetCamera())
		*/
		const shared_ptr<GameCamera> GetGameCamera() const
		{
			return dynamic_pointer_cast<GameCamera>(m_gameView->GetCamera());
		}
	};
}