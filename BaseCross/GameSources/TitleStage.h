#pragma once
#include "stdafx.h"
#include "BaseStage.h"
#include "DebugObject.h"
#include "Sprite.h"

namespace basecross
{
	class TitleStage : public BaseStage
	{
	public:

		// ステージステート定数
		enum eStageState
		{
			FadeIn,
			PushButton,
			BackFadeOut,
			StartMove,
			ModeSelect,
			CannonStanby,
			FadeOut,
		};

	public:

		// ステージステート
		eStageState m_stageState;

		// プレイヤー
		weak_ptr<DebugObject> m_player;

		// スプライト
		weak_ptr<Sprite> m_fade;
		weak_ptr<Sprite> m_titleLogo;
		weak_ptr<Sprite> m_metalLeft;
		weak_ptr<Sprite> m_metalRight;
		weak_ptr<Sprite> m_pushButton;
		//weak_ptr<Sprite> m_easy;
		//weak_ptr<Sprite> m_normal;
		//weak_ptr<Sprite> m_hard;

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

		void BackFadeState();

		void PushButtonState();

		void StartMoveState();

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