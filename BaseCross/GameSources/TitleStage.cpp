#include "stdafx.h"
#include "Project.h"

using namespace Utility;
namespace basecross
{
	void TitleStage::CreateResourses()
	{
		// 継承元の関数
		BaseStage::CreateResourses();

		// アプリケーションの取得
		const auto& app = App::GetApp();

		// メディアディレクトリパスの取得
		const wstring mediaPath = app->GetDataDirWString();

		// テクスチャディレクトリパスの取得
		const wstring texturePath = mediaPath + L"Textures/";

		// サウンドディレクトリパスの取得
		const wstring soundPath = mediaPath + L"Sounds/";

		app->RegisterWav(L"TITLE_BGM", soundPath + L"BGM/TitleBGM");
	}

	void TitleStage::CreateViewLight()
	{
		// ビューのカメラの設定
		auto ptrCamera = ObjectFactory::Create<Camera>();
		ptrCamera->SetEye(Vec3(-20.0f, 9.0f, -33.0f));
		ptrCamera->SetAt(Vec3(-20.0f, 9.0f, 0.0f));
		m_gameView = CreateView<SingleView>();
		m_gameView->SetCamera(ptrCamera);

		// マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		ptrMultiLight->SetDefaultLighting();

		// 背景の生成
		auto ptrBack = AddGameObject<DebugObject>();
		float x, y;
		x = 80.0f * 5;
		y = 45.0f * 5;
		ptrBack->SetPosition(Vec3(-90.0f, 0.0f, 50.0f));
		ptrBack->SetScale(Vec3(x, y, 5.0f));
		ptrBack->SetAlphaActive(true);

		VertexData vertex;
		SimpleVerticesIndices(vertex);
		vertex.vertices.at(1).textureCoordinate = Vec2(5.0f, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, 5.0f);
		vertex.vertices.at(3).textureCoordinate = Vec2(5.0f, 5.0f);
		auto backDraw = ptrBack->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	void TitleStage::CreateBGM()
	{
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		m_bgm = audioPtr->Start(L"TITLE_BGM", XAUDIO2_LOOP_INFINITE, 0.5f);
	}

	void TitleStage::OnCreate()
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

			// BGMの再生
			CreateBGM();

			// プレイヤーの作成
			CreatePlayer();

			// ステージ
			CreateStage("Title");
			CreateInstanceBlock("Title");
		}
		catch (...)
		{
			throw;
		}
	}

	void TitleStage::OnUpdate()
	{
		try
		{
			BaseStage::OnUpdate();
		}
		catch (...)
		{
			throw;
		}
	}
}