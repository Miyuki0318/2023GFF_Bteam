/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

using namespace Utility;
namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------

	void GameStage::CreateResourses()
	{
		BaseStage::CreateResourses();

		// アプリケーションの取得
		const auto& app = App::GetApp();

		// メディアディレクトリパスの取得
		const wstring mediaPath = app->GetDataDirWString();

		// テクスチャディレクトリパスの取得
		const wstring texturePath = mediaPath + L"Textures/";

		// サウンドディレクトリパスの取得
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// ゲーム用BGM
		app->RegisterWav(L"GAME_BGM", BGMPath + L"GameBGM");
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

	void GameStage::CreateBackGround()
	{
		// 背景の生成
		auto ptrBack = AddGameObject<DebugObject>();
		float x, y, loopX, loopY;
		loopX = 16;
		loopY = 12;
		x = 80.0f * loopX;
		y = 45.0f * loopY;
		ptrBack->SetPosition(Vec3(500.0f, 135.0f, 50.0f));
		ptrBack->SetScale(Vec3(x, y, 5.0f));
		ptrBack->SetAlphaActive(true);

		VertexData vertex;
		SimpleVerticesIndices(vertex);
		vertex.vertices.at(1).textureCoordinate = Vec2(loopY, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, loopX);
		vertex.vertices.at(3).textureCoordinate = Vec2(loopY, loopX);
		auto backDraw = ptrBack->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	void GameStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>(Vec3(-48.5f, -70.0f, 0.0f));
		SetSharedGameObject(L"Player", player);
	}

	void GameStage::CreateSprites()
	{
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE, Vec3(-675.0f, 0.0f, 0.2f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE, Vec3(675.0f, 0.0f, 0.2f));
	}

	void GameStage::DeathFadeState()
	{
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		if (mLPos.x < 0.0f)
		{
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(DELTA_TIME * 750.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(-DELTA_TIME * 750.0f, 0.0f, 0.0f));
		}
		else
		{
			StopSE(L"METAL_SE");
			CreateSE(L"METAL_STOP_SE", 1.5f);
			m_metalLeft.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.2f));
			m_metalRight.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.2f));
			m_stageState = FadeOut;
		}

	}

	void GameStage::FadeOutState(float fadeTime)
	{
		const auto& scene = App::GetApp()->GetScene<Scene>();
		if (m_fade.lock()->FadeInColor(fadeTime))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"TitleStage");
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

			// 背景の作成
			CreateBackGround();

			// BGMの再生
			CreateBGM(L"GAME_BGM", 0.3f);

			// プレイヤーの作成
			CreatePlayer();

			// スプライトの作成
			CreateSprites();

			// ステージ
			CreateEnemy(m_stagePath);
			CreateStage(m_stagePath);
			CreateInstanceBlock(m_stagePath);
		}
		catch (...) 
		{
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		try
		{
			BaseStage::OnUpdate();
			Debug::Log(m_gameView->GetCamera()->GetAt());

			switch (m_stageState)
			{
			case GameStage::FadeIn:
				if (m_fade.lock()->FadeOutColor(1.0f)) m_stageState = StartMove;
				break;

			case GameStage::Goal:
				FadeOutState(3.75f);
				break;

			case GameStage::Death:
				DeathFadeState();
				break;

			case GameStage::Continue:
				break;

			case GameStage::FadeOut:
				FadeOutState(3.0f);
				break;

			default:
				break;
			}
		}
		catch (...)
		{
			throw;
		}
	}
}