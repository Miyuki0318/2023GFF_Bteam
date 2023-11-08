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

	// BGMの再生
	void GameStage::CreateBGM()
	{
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		m_bgm = audioPtr->Start(L"GAME_BGM", XAUDIO2_LOOP_INFINITE, 0.3f);
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
		CreateSharedObjectGroup(L"Stage");

		const auto& data = CSVLoader::LoadFile("Stage");

		struct Checker
		{
			string type = "";
			int count = 0;
			bool check = false;

			void reset()
			{
				type = "";
				count = 0;
				check = false;
			}
		};

		Checker checker;
		const float under = -7.5f;
		const float left = -49.0f;
		const float scale = 1.0f;
		const Vec3 slopeScale = Vec3(scale) * 1.4f;
		const Vec2 slopeLeft = Vec2(0.5f, -0.5f);
		const Vec2 slopeRight = Vec2(-0.5f, -0.5f);

		for (size_t i = 0; i < data.size(); i++)
		{
			for (size_t j = 0; j < data.at(i).size(); j++)
			{
				if (data.at(i).at(j) == "") continue;

				shared_ptr<CubeObject> ptr;

				switch (stoi(data.at(i).at(j)))
				{
				case 0:
					checker.reset();
					break;

				case 1:
					ptr = AddGameObject<DeathColl>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, true);
					break;

				case 2:
					ptr = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, true);
					break;

				case 100:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), Block::Iron, CubeObject::Normal, true);
					break;

				case 101:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeLeft, slopeScale, Block::Iron, CubeObject::SlopeL, true);
					break;

				case 102:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeRight, slopeScale, Block::Iron, CubeObject::SlopeR, true);
					break;

				case 105:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), Block::Iron, CubeObject::Normal, false);
					break;

				case 110:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), Block::Metal, CubeObject::Normal, true);
					break;

				case 111:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeLeft, slopeScale, Block::Metal, CubeObject::SlopeL, true);
					break;

				case 112:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeRight, slopeScale, Block::Metal, CubeObject::SlopeR, true);
					break;

				case 115:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), Block::Metal, CubeObject::Normal, false);
					break;

				case 120:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), Block::DarkMetal, CubeObject::Normal, true);
					break;

				case 121:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeLeft, slopeScale, Block::DarkMetal, CubeObject::SlopeL, true);
					break;

				case 122:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeRight, slopeScale, Block::DarkMetal, CubeObject::SlopeR, true);
					break;

				case 125:
					ptr = AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), Block::DarkMetal, CubeObject::Normal, false);
					break;

				case 200:
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::All);
					break;

				case 201:
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Up);
					break;

				case 202:
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Down);
					break;

				case 203:
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Left);
					break;

				case 204:
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Right);
					break;

				case 210:
					checker.count++;
					checker.type = data.at(i).at(j);
					checker.check = data.at(i).at(j + 1) != checker.type || data.at(i).at(j - 1) != checker.type;

					if (checker.check)
					{
						auto p = AddGameObject<DebugSphere>(Vec3(left + (j * scale), under + ((data.size() - i) * scale), 0.0f), Vec3(0.0f), Vec3(scale));
					}
					else
					{
						AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), Block::DarkMetal, CubeObject::Normal, false);
					}
					break;

				case 211:
					checker.count++;
					checker.type = data.at(i).at(j);
					checker.check = data.at(i).at(j + 1) != checker.type || data.at(i).at(j - 1) != checker.type;

					if (checker.check)
					{
						auto p = AddGameObject<DebugSphere>(Vec3(left + (j * scale), under + ((data.size() - i) * scale), 0.0f), Vec3(0.0f), Vec3(scale));
					}
					else
					{
						AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), Block::DarkMetal, CubeObject::Normal, false);
					}
					break;

				case 220:
					ptr = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Up, Cannon::Rotate);
					break;

				case 221:
					ptr = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Down, Cannon::Normal);
					break;

				case 222:
					ptr = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Left, Cannon::Normal);
					break;

				case 223:
					ptr = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Right, Cannon::Normal);
					break;

				case 224:
					ptr = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Uleft, Cannon::Normal);
					break;

				case 225:
					ptr = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Dleft, Cannon::Normal);
					break;

				case 226:
					ptr = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Uright, Cannon::Normal);
					break;

				case 227:
					ptr = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Dright, Cannon::Normal);
					break;

				case 300:
					ptr = AddGameObject<Bird>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale);
					break;

				case 30:
					checker.count++;
					checker.type = data.at(i).at(j);
					checker.check = data.at(i).at(j + 1) != checker.type || data.at(i).at(j - 1) != checker.type;

					if (checker.check)
					{
						auto p = AddGameObject<DebugSphere>(Vec3(left + (j * scale), under + ((data.size() - i) * scale), 0.0f), Vec3(0.0f), Vec3(scale));
						p->GetComponent<PNTStaticDraw>()->SetEmissive(checker.count > 1 ? COL_RED : COL_BLUE);
					}
					else
					{
						AddGameObject<Block>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), Block::DarkMetal, CubeObject::Normal, false);
					}
					break;

				default:
					break;
				}

				if (ptr)
				{
					ptr->SetTarget(GetSharedGameObject<Player>(L"Player"));
					GetSharedObjectGroup(L"Stage")->IntoGroup(ptr);
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

			// BGMの再生
			CreateBGM();

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
		try
		{
			const auto& fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
			Debug::Log(L"FPS : ", fps);

			const auto& player = GetSharedGameObject<Player>(L"Player");
			Vec3 pos = player->GetComponent<Transform>()->GetPosition();

			const auto& cubeVec = GetSharedObjectGroup(L"Stage")->GetGroupVector();
			for (const auto& weakObj : cubeVec)
			{
				const auto& cubeObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());

				float length = (cubeObj->GetPosition() - pos).length();
				cubeObj->SetUpdateActive(length <= 55.0f);
			}
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
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(true);
	}
}