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


		// 地面のテクスチャの読み込み
		app->RegisterTexture(L"GROUND", texturePath + L"Ground.png");
	}

	void GameStage::CreateViewLight() 
	{
		const Vec3 eye(0.0f, 5.0f, -75.0f);
		const Vec3 at(0.0f, 5.0f, 0.0f);
		auto PtrView = CreateView<SingleView>();
		// ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		// マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		// デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void GameStage::CreateGround()
	{
		AddGameObject<Player>();
		AddGameObject<Ground>(Vec3(0.0f, -0.5f, 0.0f), Vec3(1000.0f, 1.0f, 10.0f));
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
			CreateGround();
		}
		catch (...) 
		{
			throw;
		}
	}

	void GameStage::OnDraw()
	{
		// デバッグ文字列を強制的に空にする
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// 継承元の描画時の関数を実行する
		Stage::OnDraw();

		// デバック用文字列の表示非表示切り替え
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawActive(true);
	}
}