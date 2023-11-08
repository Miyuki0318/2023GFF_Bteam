
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::CreateResourses()
	{
		// アプリケーションの取得
		const auto& app = App::GetApp();

		// ディレクトリパスの設定
		const wstring mediaPath = app->GetDataDirWString();

		// テクスチャディレクトリパス
		const wstring texturePath = mediaPath + L"Textures/";

		// 白塗りテクスチャ
		app->RegisterTexture(L"WHITE", texturePath + L"White.png");

		// 煙テクスチャ
		app->RegisterTexture(L"SMOKE_TX", texturePath + L"Smoke.png");
		
		// 炎テクスチャ
		app->RegisterTexture(L"FIRE_TX", texturePath + L"Fire.png");

		// 石テクスチャ
		app->RegisterTexture(L"STONE_TX", texturePath + L"Stone.png");


		// モデルディレクトリパス
		const wstring modelPath = mediaPath + L"Models/";

		// ブロックモデル
		RegisterSingleMesh(L"BLOCK", modelPath + L"Block/", L"Block", false);
		RegisterSingleMesh(L"SLOPE", modelPath + L"Block/", L"Slope", false);
		app->RegisterTexture(L"GRASS_TX", modelPath + L"Block/Grass.png");
		app->RegisterTexture(L"DIRT_TX", modelPath + L"Block/Dirt.png");
		app->RegisterTexture(L"ROCK_TX", modelPath + L"Block/Rock.png");

		// スパイクモデル
		RegisterSingleMesh(L"SPIKE", modelPath, L"Spike", false);
		RegisterSingleMesh(L"SPIKE_BLOCK", modelPath, L"SpikeBlock", false);

		// ロボットモデル
		RegisterSingleMesh(L"ROBOT_BODY", modelPath + L"Robot/", L"Robot_Body", true);
		RegisterMultiMesh(L"ROBOT_ARM", modelPath + L"Robot/", L"Robot_Arm", true);

		// 鳥モデル
		RegisterSingleMesh(L"BIRD", modelPath + L"Bird/", L"Bird", true);

		// 大砲モデル
		RegisterSingleMesh(L"CANNON", modelPath + L"Cannon/", L"Cannon", true);


		// サウンドディレクトリパス
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// ゲーム用BGM
		app->RegisterWav(L"GAME_BGM", BGMPath + L"GameBGM");

		// 衝撃波SE
		app->RegisterWav(L"AIRSHOCK_SE", SEPath + L"AirShockSE");

		// 大砲発射SE
		app->RegisterWav(L"CANNON_SE", SEPath + L"CannonFireSE");
	}

	void Scene::RegisterSingleMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse)
	{
		shared_ptr<MeshResource> modelMesh;
		if (boneUse)
		{
			modelMesh = MeshResource::CreateBoneModelMesh(path, fileName + L".bmf");
		}
		else
		{
			modelMesh = MeshResource::CreateStaticModelMesh(path, fileName + L".bmf");
		}
		const auto& app = App::GetApp();
		app->RegisterResource(registerKey, modelMesh);
	}

	void Scene::RegisterMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse)
	{
		shared_ptr<MultiMeshResource> modelMesh;
		if (boneUse)
		{
			modelMesh = MultiMeshResource::CreateBoneModelMultiMesh(path, fileName + L".bmf");
		}
		else
		{
			modelMesh = MultiMeshResource::CreateStaticModelMultiMesh(path, fileName + L".bmf");
		}
		const auto& app = App::GetApp();
		app->RegisterResource(registerKey, modelMesh);
	}

	void Scene::OnCreate()
	{
		try 
		{
			// リソースの読み込み
			CreateResourses();

			// クリアする色を設定
			SetClearColor(Col4(160.0f / 256.0f, 216.0f / 256.0f, 239.0f / 256.0f, 1.0));

			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"GameStage");
		}
		catch (...) 
		{
			throw;
		}
	}

	Scene::~Scene(){}

	void Scene::OnEvent(const shared_ptr<Event>& event) 
	{
		if (event->m_MsgStr == L"GameStage")
		{
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
	}

}
//end basecross
