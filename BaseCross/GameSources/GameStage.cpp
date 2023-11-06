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
		CreateSharedObjectGroup(L"Stage");

		const auto& data = CSVLoader::LoadFile("Test");

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
		const Vec3 slopeScale = Vec3(scale * 1.5f, scale, scale);

		for (size_t i = 0; i < data.size(); i++)
		{
			for (size_t j = 0; j < data.at(i).size(); j++)
			{
				vector<shared_ptr<CubeObject>> ptr;

				switch (stoi(data.at(i).at(j)))
				{
				case 0:
					checker.reset();
					break;

				case 2:
					ptr.push_back(AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, true));
					break;

				case 100:
					ptr.push_back(AddGameObject<Grass>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, true));
					break;

				case 101:
					ptr.push_back(AddGameObject<Grass>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Left, true));
					break;

				case 102:
					ptr.push_back(AddGameObject<Grass>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Right, true));
					break;

				case 103:
					ptr.push_back(AddGameObject<Grass>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, true));
					ptr.push_back(AddGameObject<Grass>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Left, true));
					break;

				case 104:
					ptr.push_back(AddGameObject<Grass>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, true));
					ptr.push_back(AddGameObject<Grass>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Right, true));
					break;

				case 105:
					ptr.push_back(AddGameObject<Grass>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, false));
					break;

				case 110:
					ptr.push_back(AddGameObject<Dirt>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, true));
					break;

				case 111:
					ptr.push_back(AddGameObject<Dirt>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Left, true));
					break;

				case 112:
					ptr.push_back(AddGameObject<Dirt>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Right, true));
					break;

				case 113:
					ptr.push_back(AddGameObject<Dirt>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, true));
					ptr.push_back(AddGameObject<Dirt>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Left, true));
					break;

				case 114:
					ptr.push_back(AddGameObject<Dirt>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, true));
					ptr.push_back(AddGameObject<Dirt>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Right, true));
					break;

				case 115:
					ptr.push_back(AddGameObject<Dirt>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, false));
					break;

				case 120:
					ptr.push_back(AddGameObject<Rock>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, true));
					break;

				case 121:
					ptr.push_back(AddGameObject<Rock>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Left, true));
					break;

				case 122:
					ptr.push_back(AddGameObject<Rock>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Right, true));
					break;

				case 123:
					ptr.push_back(AddGameObject<Rock>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, true));
					ptr.push_back(AddGameObject<Rock>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Left, true));
					break;

				case 124:
					ptr.push_back(AddGameObject<Rock>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, true));
					ptr.push_back(AddGameObject<Rock>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Right, true));
					break;

				case 125:
					ptr.push_back(AddGameObject<Rock>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale), (float)CubeObject::Normal, false));
					break;

				case 200:
					ptr.push_back(AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, true));
					break;

				case 201:
					ptr.push_back(AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Up));
					break;

				case 202:
					ptr.push_back(AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Down));
					break;

				case 203:
					ptr.push_back(AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Left));
					break;

				case 204:
					ptr.push_back(AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Right));
					break;

				case 300:
					ptr.push_back(AddGameObject<Bird>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale));
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
						AddGameObject<Rock>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), slopeScale, (float)CubeObject::Normal, false);
					}
					break;

				default:
					break;
				}

				for (const auto& p : ptr)
				{
					p->SetTarget(GetSharedGameObject<Player>(L"Player"));
					GetSharedObjectGroup(L"Stage")->IntoGroup(p);
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