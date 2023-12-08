/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase
	{
	public:

		enum eGameStage
		{
			None,
			Easy,
			Normal,
			Hard,
		};

	private:

		eGameStage m_gameStage;
		string m_currentStage;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() : SceneBase() 
		{
			m_gameStage = None;
			m_currentStage = "Tutorial";
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースの読み込み
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CreateResourses();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シングルモデルメッシュの登録
		@param[in]  登録するキー
		@param[in]	ディレクトリパス
		@param[in]	ファイルネーム
		@param[in]  ボーンがあるかの真偽
		*/
		virtual void RegisterSingleMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチモデルメッシュの登録
		@param[in]  登録するキー
		@param[in]	ディレクトリパス
		@param[in]	ファイルネーム
		@param[in]  ボーンがあるかの真偽
		*/
		virtual void RegisterMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のステージネームの保持
		@param[in]  現在のステージネーム
		*/
		void SetCurrentStage(const string& stageName)
		{
			m_currentStage = stageName;
			if (stageName == "Easy") m_gameStage = Easy;
			if (stageName == "Normal") m_gameStage = Normal;
			if (stageName == "Hard") m_gameStage = Hard;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	前回のステージネームの取得
		@return  前回のステージネーム
		*/
		const string& GetCurrentStage() const
		{
			return m_currentStage;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	次のステージの取得
		@return  次のステージ名
		*/
		const string GetNextStage() const
		{
			string stageName = "Easy";

			switch (m_gameStage)
			{
			case Scene::Easy:
				stageName = "Normal";
				break;

			case Scene::Normal:
				stageName = "Hard";
				break;

			default:
				break;
			}

			return stageName;
		}
	};
}