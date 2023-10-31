
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


		// モデルディレクトリパス
		const wstring modelPath = mediaPath + L"Models/";

		// 草ブロックモデル
		RegisterSingleMesh(L"GRASS", modelPath + L"Grass/", L"Grass", false);

		// 土ブロックモデル
		RegisterSingleMesh(L"DIRT", modelPath + L"Dirt/", L"Dirt", false);

		// 岩ブロックモデル
		RegisterSingleMesh(L"ROCK", modelPath + L"Rock/", L"Rock", false);

		// 砂岩ブロックモデル
		RegisterSingleMesh(L"SANDSTONE", modelPath + L"SandStone/", L"Sandstone", false);

		// スパイクモデル
		RegisterSingleMesh(L"SPIKE", modelPath, L"Spike", false);

		// ロボットモデル
		RegisterSingleMesh(L"ROBOT_BODY", modelPath + L"Robot/", L"Robot_Body", true);
		RegisterMultiMesh(L"ROBOT_ARM", modelPath + L"Robot/", L"Robot_Arm", true);

		// 鳥モデル
		RegisterSingleMesh(L"BIRD", modelPath + L"Bird/", L"Bird", true);
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
