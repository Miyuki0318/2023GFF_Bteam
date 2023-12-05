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
			Death,
			Continue,
			FadeOut,
		};

	private:

		// ステージステート
		eStageState m_stageState;

		// スプライト
		weak_ptr<Sprite> m_fade;
		weak_ptr<Sprite> m_metalLeft;
		weak_ptr<Sprite> m_metalRight;

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

		void DeathFadeState();

		void FadeOutState(float fadeTime);

	public:

		/*!
		@brief コンストラクタ
		*/
		GameStage() :BaseStage() 
		{
			m_stageState = FadeIn;
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