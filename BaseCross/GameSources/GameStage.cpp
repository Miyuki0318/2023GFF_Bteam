/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------

	void GameStage::CreateResourses()
	{
		// アプリケーションの取得
		const auto& app = App::GetApp();

		// メディアディレクトリパスの取得
		const wstring mediaPath = app->GetDataDirWString();

		// テクスチャディレクトリパスの取得
		const wstring texturePath = mediaPath + L"Textures/";

		// サウンドディレクトリパスの取得
		const wstring soundPath = mediaPath + L"Sounds/";

		// 炎のテクスチャの読み込み
		app->RegisterTexture(L"EFFECT", texturePath + L"Effect.png");
	}

	void GameStage::CreateViewLight() 
	{
		// ビューのカメラの設定
		auto ptrCamera = ObjectFactory::Create<GameCamera>();
		m_gameView = CreateView<SingleView>();
		m_gameView->SetCamera(ptrCamera);

		// マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		ptrMultiLight->SetDefaultLighting();
	}

	void GameStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>();
		SetSharedGameObject(L"Player", player);
		
		const auto& gameCamera = dynamic_pointer_cast<GameCamera>(m_gameView->GetCamera());
		gameCamera->SetTargetObject(player);
	}

	void GameStage::CreateStage()
	{
		const auto& data = CSVLoader::LoadFile("Test");

		const float up = 22.5f;
		const float left = -22.0f;
		const float scale = 1.0f;

		for (size_t i = 0; i < data.size(); i++)
		{
			for (size_t j = 0; j < data.at(i).size(); j++)
			{
				shared_ptr<CubeObject> ptr;
				if (data.at(i).at(j) == "-1")
				{
					ptr = AddGameObject<Alpha>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "1")
				{
					ptr = AddGameObject<Grass>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "2")
				{
					ptr = AddGameObject<Dirt>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "3")
				{
					ptr = AddGameObject<Rock>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "4")
				{
					ptr = AddGameObject<SandStone>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "20")
				{
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), up - (i * scale)), scale, Gimmick::Up);
				}
				if (data.at(i).at(j) == "21")
				{
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), up - (i * scale)), scale, Gimmick::Down);
				}
				if (data.at(i).at(j) == "22")
				{
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), up - (i * scale)), scale, Gimmick::Left);
				}
				if (data.at(i).at(j) == "23")
				{
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), up - (i * scale)), scale, Gimmick::Right);
				}

				if (ptr)
				{
					ptr->SetTarget(GetSharedGameObject<Player>(L"Player"));
				}
			}
		}
	}

	void GameStage::OnCreate() 
	{
		try
		{
			//各パフォーマンスを得る
			SetCollisionPerformanceActive(true);
			SetUpdatePerformanceActive(true);
			SetDrawPerformanceActive(true);

			// リソースの読み込み
			CreateResourses();

			// ビューとライトの作成
			CreateViewLight();

			// 地面の作成
			CreatePlayer();

			// ステージ
			CreateStage();
		}
		catch (...) 
		{
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		const auto& fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		Debug::Log(L"FPS : ", fps);
	}

	void GameStage::OnDraw()
	{
		// デバッグ文字列を強制的に空にする
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// 継承元の描画時の関数を実行する
		Stage::OnDraw();

		// デバック用文字列の表示非表示切り替え
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(true);
	}
}