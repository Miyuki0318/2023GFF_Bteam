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

		// UI用テクスチャ
		app->RegisterTexture(L"JGAUGE_TX", texturePath + L"JumpUI.png");
		app->RegisterTexture(L"SGAUGE_TX", texturePath + L"ShieldUI.png");
		app->RegisterTexture(L"GAUGE_TX", texturePath + L"Gauge.png");
		app->RegisterTexture(L"N_EFFECT_TX", texturePath + L"WhiteEffect.png");
		app->RegisterTexture(L"GAMEOVER_TX", texturePath + L"GameOver.png");
		app->RegisterTexture(L"CONTINUE_TX", texturePath + L"Continue.png");

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
		m_backObj = AddGameObject<TemplateObject>();
		float x, y, loopX, loopY;
		loopX = 16;
		loopY = 12;
		x = 80.0f * loopX;
		y = 45.0f * loopY;
		const auto& backObj = m_backObj.lock();
		backObj->SetPosition(Vec3(500.0f, 135.0f, 50.0f));
		backObj->SetScale(Vec3(x, y, 5.0f));
		backObj->SetAlphaActive(true);

		VertexData vertex;
		SimpleVerticesIndices(vertex);
		vertex.vertices.at(1).textureCoordinate = Vec2(loopY, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, loopX);
		vertex.vertices.at(3).textureCoordinate = Vec2(loopY, loopX);
		auto backDraw = backObj->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	void GameStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>(Vec3(-48.5f, -75.0f, 0.0f));
		SetSharedGameObject(L"Player", player);
	}

	void GameStage::CreateSprites()
	{
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE * 1.25f, Vec3(-675.0f, 0.0f, 0.2f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE * 1.25f, Vec3(675.0f, 0.0f, 0.2f));
		m_gameOver = AddGameObject<Sprite>(L"GAMEOVER_TX", WINDOW_SIZE * 0.75f, Vec3(0.0f, 600.0f, 0.2f));
		m_continue = AddGameObject<Sprite>(L"CONTINUE_TX", WINDOW_SIZE * 0.55f, Vec3(-300.0f, 600.0f, 0.2f));
		m_titleBack = AddGameObject<Sprite>(L"QUIT_TX", WINDOW_SIZE * 0.55f, Vec3(300.0f, 600.0f, 0.2f));
	}

	void GameStage::CreateUI()
	{
		const Vec2 scale = Vec2(25.0f, 150.0f) * 1.25f;

		AddGameObject<JumpCountUI>(scale, Vec3(600.0f, -225.0f, 0.3f));
		AddGameObject<ShieldCountUI>(scale, Vec3(525.0f, -225.0f, 0.3f));
	}

	void GameStage::DeathFadeState()
	{
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& goPos = m_gameOver.lock()->GetPosition();
		const Vec3& coPos = m_continue.lock()->GetPosition();
		const Vec3& tbPos = m_titleBack.lock()->GetPosition();

		if (mLPos.x < 0.0f)
		{
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(DELTA_TIME * 750.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(-DELTA_TIME * 750.0f, 0.0f, 0.0f));
			m_gameOver.lock()->SetPosition(goPos + Vec3(0.0f, -DELTA_TIME * 545.0f, 0.0f));
			m_continue.lock()->SetPosition(coPos + Vec3(0.0f, -DELTA_TIME * 800.0f, 0.0f));
			m_titleBack.lock()->SetPosition(tbPos + Vec3(0.0f, -DELTA_TIME * 800.0f, 0.0f));
		}
		else
		{
			StopSE(L"METAL_SE");
			CreateSE(L"METAL_STOP_SE", 1.5f);
			m_metalLeft.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.2f));
			m_metalRight.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.2f));
			m_stageState = Select;
		}
	}
	
	void GameStage::SelectState()
	{
		const auto& player = GetSharedGameObject<Player>(L"Player");
		const bool inputLStick = Input::IsInputLStickX();
		const Vec2 deffScale = WINDOW_SIZE * 0.55f;
		m_totalTime += DELTA_TIME * 5.0f;

		if (inputLStick && !m_currentStickX)
		{
			m_totalTime = 0.0f;
			switch (m_select)
			{
			case GameStage::Continue:
				m_select = TitleBack;
				m_totalTime = 0.0f;
				break;

			case GameStage::TitleBack:
				m_select = Continue;
				m_totalTime = 0.0f;
				break;

			default:
				break;
			}
		}

		m_continue.lock()->SetScale(deffScale * (m_select == Continue ? SinCurve(m_totalTime ,1.0f, 1.2f) : 1.0f));
		m_titleBack.lock()->SetScale(deffScale * (m_select == TitleBack ? SinCurve(m_totalTime, 1.0f, 1.2f) : 1.0f));
		m_currentStickX = inputLStick;

		if (Input::GetPushA())
		{
			switch (m_select)
			{
			case GameStage::Continue:
				ResetStage();
				player->Reset();
				CreateSE(L"CANNON_SE", 1.0f);
				CreateSE(L"METAL_SE", 0.75f);
				m_totalTime = 0.0f;
				m_stageState = Reset;
				break;

			case GameStage::TitleBack:
				m_stageState = FadeOut;
				CreateSE(L"GAMEOVER_SE", 1.0f);
				break;

			default:
				break;
			}
		}
	}

	void GameStage::ResetState()
	{
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& goPos = m_gameOver.lock()->GetPosition();
		const Vec3& coPos = m_continue.lock()->GetPosition();
		const Vec3& tbPos = m_titleBack.lock()->GetPosition();

		if (mLPos.x > -WINDOW_WIDTH / 1.9f)
		{
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(-DELTA_TIME * 750.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(DELTA_TIME * 750.0f, 0.0f, 0.0f));
			m_gameOver.lock()->SetPosition(goPos + Vec3(0.0f, DELTA_TIME * 545.0f, 0.0f));
			m_continue.lock()->SetPosition(coPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
			m_titleBack.lock()->SetPosition(tbPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
		}
		else
		{
			StopSE(L"METAL_SE");
			CreateSE(L"METAL_STOP_SE", 1.5f);
			m_stageState = StartMove;
			m_metalLeft.lock()->SetPosition(Vec3(-675.0f, 0.0f, 0.2f));
			m_metalRight.lock()->SetPosition(Vec3(675.0f, 0.0f, 0.2f));
			m_gameOver.lock()->SetPosition(Vec3(0.0f, 600.0f, 0.2f));
			m_continue.lock()->SetPosition(Vec3(-300.0f, 600.0f, 0.2f));
			m_titleBack.lock()->SetPosition(Vec3(300.0f, 600.0f, 0.2f));
			m_continue.lock()->SetScale(WINDOW_SIZE * 0.55f);
			m_titleBack.lock()->SetScale(WINDOW_SIZE * 0.55f);
		}
	}

	void GameStage::FadeOutState(float fadeTime)
	{
		const auto& scene = App::GetApp()->GetScene<Scene>();
		if (m_fade.lock()->FadeInColor(fadeTime))
		{
			if (m_isClear)
			{
				scene->SetCurrentStage(m_stagePath);
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"NextStage");
			}
			else
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"TitleStage");
			}
		}
	}

	void GameStage::ResetStage()
	{
		const auto& collectGroup = GetSharedObjectGroup(L"Collect");
		const auto& enemyGroup = GetSharedObjectGroup(L"Enemy");
		const auto& wallGroup = GetSharedObjectGroup(L"Wall");
		const auto& stageVec = GetSharedObjectGroup(L"Stage")->GetGroupVector();
		const auto& updateVec = GetSharedObjectGroup(L"Update")->GetGroupVector();
		const auto& playerPtr = GetSharedGameObject<Player>(L"Player");

		GroupObjectRemove(collectGroup);
		GroupObjectRemove(enemyGroup);
		GroupObjectRemove(wallGroup);
		collectGroup->AllClear();
		enemyGroup->AllClear();
		wallGroup->AllClear();

		const float under = -97.5f;
		const float left = -49.0f;
		const float scale = 1.0f;

		vector<weak_ptr<Ring>> ringVec;
		vector<weak_ptr<Enemy>> enemyVec;
		vector<weak_ptr<Enemy>> wallVec;

		for (size_t i = 0; i < m_csvData.size(); i++)
		{
			for (size_t j = 0; j < m_csvData.at(i).size(); j++)
			{
				if (m_csvData.at(i).at(j) == "") continue;

				const int& num = stoi(m_csvData.at(i).at(j));

				if (GetBetween(num, 230, 301))
				{
					switch (num)
					{
					case 230:
						ringVec.push_back(AddGameObject<Ring>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Ring::Big));
						break;

					case 231:
						ringVec.push_back(AddGameObject<Ring>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Ring::Small));
						break;

					case 300:
						enemyVec.push_back(AddGameObject<Rabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Rabbit::Normal));
						break;

					case 301:
						wallVec.push_back(AddGameObject<Rabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Rabbit::Wall));
						break;

					default:
						break;
					}
				}
			}
		}

		for (const auto& ring : ringVec)
		{
			if (ring.lock())
			{
				ring.lock()->AddTarget(playerPtr);
				collectGroup->IntoGroup(ring.lock());
			}
		}

		for (const auto& enemy : enemyVec)
		{
			if (enemy.lock())
			{
				enemyGroup->IntoGroup(enemy.lock());
			}
		}

		for (const auto& wall : wallVec)
		{
			if (wall.lock())
			{
				wallGroup->IntoGroup(wall.lock());
			}
		}

		for (const auto& weakObj : stageVec)
		{
			if (!weakObj.lock()) continue;

			const auto& block = dynamic_pointer_cast<CubeObject>(weakObj.lock());
			if (!block) continue;

			block->RemoveTarget();
			block->AddTarget(enemyGroup->GetGroupVector());
			block->AddTarget(playerPtr);
		}

		for (const auto& weakObj : updateVec)
		{
			if (!weakObj.lock()) continue;

			const auto& update = dynamic_pointer_cast<CubeObject>(weakObj.lock());
			if (!update) continue;

			update->RemoveTarget();
			update->AddTarget(enemyGroup->GetGroupVector());
			update->AddTarget(playerPtr);
		}
	}

	void GameStage::OnCreate() 
	{
		try
		{
			BaseStage::OnCreate();

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

			// UIの作成
			CreateUI();

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

			Debug::Log(m_gameOver.lock()->GetPosition());

			switch (m_stageState)
			{
			case GameStage::FadeIn:
				if (m_fade.lock()->FadeOutColor(1.0f)) m_stageState = StartMove;
				break;

			case GameStage::Goal:
				m_isClear = true;
				FadeOutState(3.75f);
				break;

			case GameStage::Death:
				DeathFadeState();
				break;

			case GameStage::Select:
				SelectState();
				break;

			case GameStage::Reset:
				ResetState();
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