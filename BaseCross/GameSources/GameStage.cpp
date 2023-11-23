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
		// アプリケーションの取得
		const auto& app = App::GetApp();

		// メディアディレクトリパスの取得
		const wstring mediaPath = app->GetDataDirWString();

		// テクスチャディレクトリパスの取得
		const wstring texturePath = mediaPath + L"Textures/";

		// サウンドディレクトリパスの取得
		const wstring soundPath = mediaPath + L"Sounds/";
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

		// 背景の生成
		for (size_t i = 0; i < 12; i++)
		{
			for (size_t j = 0; j < 18; j++)
			{
				auto ptrBack = AddGameObject<DebugObject>();
				float x, y;
				x = 80.0f;
				y = 45.0f;
				ptrBack->SetPosition(Vec3(-90.0f + (x * j), 135.0f - (y * i), 50.0f));
				ptrBack->SetScale(Vec3(x, y, 5.0f));
				ptrBack->SetAlphaActive(true);


				VertexData vertex;
				SimpleVerticesIndices(vertex);
				auto backDraw = ptrBack->AddComponent<PCTStaticDraw>();
				backDraw->SetOriginalMeshUse(true);
				backDraw->CreateOriginalMesh(vertex);
				backDraw->SetTextureResource(L"BACKGROUND_TX");
			}
		}
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

	// インスタンス描画オブジェクトの生成
	void GameStage::CreateInstanceBlock()
	{
		// テクスチャタイプ
		enum eTypes
		{
			Iron,
			Metal,
			Dark,
			Size, // enumのサイズ指定子
		};

		// インスタンス用構造体
		struct Instance
		{
			vector<int> num; // データ配列
			int count = 0;   // カウンタ
		};

		// CSVの読み込み(ストリング型の2次元配列)
		const auto& data = CSVLoader::LoadFile("Stage");

		// CSVの行の数
		const int rowSize = static_cast<int>(data.size());

		// 行の数でループ
		for (int i = 0; i < rowSize; i++)
		{
			// ブロックとスロープのインスタンス構造体
			vector<Instance> block;
			vector<Instance> slope;

			// タイプの数でリサイズ
			block.resize(eTypes::Size);
			slope.resize(eTypes::Size);

			// タイプの数ループ
			for (int j = 0; j < eTypes::Size; j++)
			{
				// i行目の列の数でリサイズ
				block.at(j).num.resize(data.at(i).size());
				slope.at(j).num.resize(data.at(i).size());
			}

			// 列の数でループ
			for (int j = 0; j < data.at(i).size(); j++)
			{
				// 空白が読み込まれた時用のエラーチェック
				if (data.at(i).at(j) == "") continue;

				// 文字列を整数に変換
				const int& num = stoi(data.at(i).at(j));

				// インスタンス描画している数値の範囲で指定(ID参照)
				if (GetBetween(num, 100, 125))
				{
					// 10の位を求める
					// 例：文字列の中身が121だった場合.at(1)で21を取得し、10で割る事で求められている
					int tensPlace = atoi(&data.at(i).at(j).at(1)) / 10;

					// 数値が5で割り切れる時(例：100,105)
					if (num % 5 == 0)
					{
						// 10の位をタイプの添え字としてカウンタを増やす
						block.at(tensPlace).count++;

						// タイプの数でループ
						for (int k = 0; k < eTypes::Size; k++)
						{
							// 10の位がタイプ(ループのK)と一致したら1、不一致なら0
							block.at(k).num.at(j) = static_cast<int>(tensPlace == k);
						}
					}
					else // 割り切れない時(例：101,102,103,104)
					{
						// 10の位をタイプの添え字としてカウンタを増やす
						slope.at(tensPlace).count++;

						// タイプの数でループ
						for (int k = 0; k < eTypes::Size; k++)
						{
							// 10の位がタイプ(ループのK)と一致したら数値の1の位の値を、不一致なら0
							slope.at(k).num.at(j) = tensPlace == k ? atoi(&data.at(i).at(j).at(2)) : 0;
						}
					}
				}
			}

			// タイプの数でループ
			for (int k = 0; k < eTypes::Size; k++)
			{
				// カウンタが0より大きかったら生成
				if (block.at(k).count > 0)
				{
					AddGameObject<InstanceBlock>(block.at(k).num, k, rowSize, i);
				}
				if (slope.at(k).count > 0)
				{
					AddGameObject<InstanceSlope>(slope.at(k).num, k, rowSize, i);
				}
			}
		}
	}

	void GameStage::CreateStage()
	{
		CreateSharedObjectGroup(L"Stage");
		CreateSharedObjectGroup(L"Gimmick");
		CreateSharedObjectGroup(L"Update");

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

		const float under = -97.5f;
		const float left = -49.0f;
		const float scale = 1.0f;
		const Vec3 slopeScale = Vec3(scale) * 1.4f;
		const Vec2 slopeULeft = Vec2(0.5f, -0.5f);
		const Vec2 slopeURight = Vec2(-0.5f, -0.5f);
		const Vec2 slopeDLeft = Vec2(0.5f, 0.5f);
		const Vec2 slopeDRight = Vec2(-0.5f, 0.5f);

		Checker checker;
		for (size_t i = 0; i < data.size(); i++)
		{
			for (size_t j = 0; j < data.at(i).size(); j++)
			{
				if (data.at(i).at(j) == "") continue;

				shared_ptr<CubeObject> block = nullptr;
				shared_ptr<Gimmick> gimmick = nullptr;
				shared_ptr<Gimmick> update = nullptr;

				const int& num = stoi(data.at(i).at(j));

				switch (num)
				{
				case 0:
					checker.reset();
					break;

				case 1:
					block = AddGameObject<DeathColl>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, true);
					break;

				case 2:
				case 100:
				case 110:
				case 120:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, true);
					break;

				case 101:
				case 111:
				case 121:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeULeft, slopeScale, true);
					break;

				case 102:
				case 112:
				case 122:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeURight, slopeScale, true);
					break;

				case 103:
				case 113:
				case 123:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeDLeft, slopeScale, true);
					break;

				case 104:
				case 114:
				case 124:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeDRight, slopeScale, true);
					break;

				case 230:
					gimmick = AddGameObject<Ring>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 5.0f);
					break;

				case 300:
					block = AddGameObject<Bird>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale);
					break;

				default:
					break;
				}

				// 特殊なブロックやギミック用

				// 棘
				if (GetBetween(num, 200, 204))
				{
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&data.at(i).at(j).at(2)));
					block = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, angle);
				}
				
				// ベルトコンベア
				if (GetBetween(num, 210, 211))
				{
					checker.count++;
					checker.type = data.at(i).at(j);
					checker.check = data.at(i).at(j + 1) != checker.type || data.at(i).at(j - 1) != checker.type;
					const auto& rotate = static_cast<Convayor::eRotate>(atoi(&data.at(i).at(j).at(2)));
					const auto& beltType = static_cast<Convayor::eType>(checker.check);
					gimmick = AddGameObject<Convayor>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, rotate, beltType);
				}

				// 大砲
				if (GetBetween(num, 2200, 2283))
				{
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&data.at(i).at(j).at(2)) / 10);
					const auto& fireType = static_cast<Cannon::eFireType> (atoi(&data.at(i).at(j).at(3)));
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, angle, fireType);
				}

				// 送風機
				if (GetBetween(num, 240, 243))
				{
					const Vec3 blowerScale = Vec3(scale * 5.0f, scale, scale * 5.0f);
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&data.at(i).at(j).at(2)));
					update = AddGameObject<Blower>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), blowerScale, angle, 15.0f);
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), update->GetRotation(), blowerScale, true);
				}

				if (block)
				{
					block->SetTarget(GetSharedGameObject<Player>(L"Player"));
					GetSharedObjectGroup(L"Stage")->IntoGroup(block);
				}

				if (gimmick)
				{
					gimmick->SetTarget(GetSharedGameObject<Player>(L"Player"));
					GetSharedObjectGroup(L"Gimmick")->IntoGroup(gimmick);
				}

				if (update)
				{
					update->SetTarget(GetSharedGameObject<Player>(L"Player"));
					GetSharedObjectGroup(L"Update")->IntoGroup(update);
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

			// プレイヤーの作成
			CreatePlayer();
			// ステージ
			CreateStage();
			CreateInstanceBlock();
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

				if (!cubeObj) continue;

				float length = (cubeObj->GetPosition() - pos).length();
				cubeObj->SetUpdateActive(length <= 10.0f);
				cubeObj->SetDrawActive(length <= 55.0f);
			}

			const auto& gimmickVec = GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
			for (const auto& weakObj : gimmickVec)
			{
				const auto& gimmickObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());

				if (!gimmickObj) continue;

				float length = (gimmickObj->GetPosition() - pos).length();
				gimmickObj->SetUpdateActive(length <= 55.0f);
				gimmickObj->SetDrawActive(length <= 55.0f);
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