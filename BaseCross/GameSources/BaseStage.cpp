#include "stdafx.h"
#include "Project.h"

using namespace Utility;
namespace basecross
{
	void BaseStage::CreateResourses()
	{
		// アプリケーションの取得
		const auto& app = App::GetApp();

		// メディアディレクトリパスの取得
		const wstring mediaPath = app->GetDataDirWString();

		// テクスチャディレクトリパスの取得
		const wstring texturePath = mediaPath + L"Textures/";

		// 白塗りテクスチャ
		app->RegisterTexture(L"WHITE_TX", texturePath + L"White.png");

		// メタルの背景の読み込み
		app->RegisterTexture(L"METAL_LEFT", texturePath + L"MetalLeft.png");
		app->RegisterTexture(L"METAL_RIGHT", texturePath + L"MetalRight.png");

		// 炎のテクスチャの読み込み
		app->RegisterTexture(L"EFFECT_TX", texturePath + L"Effect.png");

		// シールドテクスチャの読み込み
		app->RegisterTexture(L"SHIELD_TX", texturePath + L"Shield.png");

		// 煙テクスチャ
		app->RegisterTexture(L"SMOKE_TX", texturePath + L"Smoke.png");

		// 炎テクスチャ
		app->RegisterTexture(L"FIRE_TX", texturePath + L"Fire.png");

		// 石テクスチャ
		app->RegisterTexture(L"STONE_TX", texturePath + L"Stone.png");

		// 紙テクスチャ
		app->RegisterTexture(L"PAPER_TX", texturePath + L"Paper.png");

		// 背景テクスチャ
		app->RegisterTexture(L"BACKGROUND_TX", texturePath + L"BackGround.png");
		app->RegisterTexture(L"NBACKGROUND_TX", texturePath + L"NewBackGround.png");

		// 矢印テクスチャ
		app->RegisterTexture(L"ARROW_TX", texturePath + L"Arrow.png");

		// サウンドディレクトリパス
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// メタルシャッターの開閉SE
		app->RegisterWav(L"METAL_SE", SEPath + L"MetalDoorSE");
		app->RegisterWav(L"METAL_STOP_SE", SEPath + L"MetalStopSE");

		// ボタンのSE
		app->RegisterWav(L"PUSH_SE", SEPath + L"PushButtonSE");

		// 衝撃波SE
		app->RegisterWav(L"AIRSHOCK_SE", SEPath + L"AirShockSE");

		// バウンド
		app->RegisterWav(L"ROBOT_BOUND_SE", SEPath + L"RobotBoundSE");

		// 大砲発射SE
		app->RegisterWav(L"CANNON_SE", SEPath + L"CannonFireSE");

		// リングSE
		app->RegisterWav(L"RING_SE", SEPath + L"RingGetSE");

		// シールドSE
		app->RegisterWav(L"SHIELD_C_SE", SEPath + L"ShieldCreateSE");
		app->RegisterWav(L"SHIELD_D_SE", SEPath + L"ShieldDestroySE");

		// ダメージSE
		app->RegisterWav(L"DAMAGE_SE", SEPath + L"DamageSE");

		// ウサギのSE
		app->RegisterWav(L"RABBIT_JUMP_SE", SEPath + L"RabbitJumpSE");
		app->RegisterWav(L"RABBIT_SEARCH_SE", SEPath + L"RabbitSearchSE");
	}

	void BaseStage::CreateViewLight() {}

	void BaseStage::CreateBGM(const wstring& bgmKey, float volume)
	{
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		m_bgm = audioPtr->Start(bgmKey, XAUDIO2_LOOP_INFINITE, volume);
	}

	void BaseStage::CreateSE(const wstring& seKey, float volume)
	{
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		m_seList.push_back(SE(audioPtr->Start(seKey, 0, volume), seKey));
	}

	void BaseStage::StopSE(const wstring& seKey)
	{
		if (seKey != L"")
		{
			for (size_t i = 0; i < m_seList.size(); i++)
			{
				if (m_seList.at(i).seKey == seKey)
				{
					const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
					audioPtr->Stop(m_seList.at(i).item.lock());
					m_seList.at(i).Reset();
					break;
				}
			}
		}
	}

	void BaseStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>(Vec3(-20.0f, -80.0f, 0.0f));
		SetSharedGameObject(L"Player", player);
	}

	void BaseStage::CreateEnemy(const string& fileName)
	{
		m_csvData = CSVLoader::LoadFile(fileName);
		const float under = -97.5f;
		const float left = -49.0f;
		const float scale = 1.0f;

		vector<weak_ptr<Enemy>> enemyVec;

		for (size_t i = 0; i < m_csvData.size(); i++)
		{
			for (size_t j = 0; j < m_csvData.at(i).size(); j++)
			{
				if (m_csvData.at(i).at(j) == "") continue;

				const int& num = stoi(m_csvData.at(i).at(j));
				if (GetBetween(num, 300, 301))
				{
					switch (num)
					{
					case 300:
						enemyVec.push_back(AddGameObject<Rabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Rabbit::Normal));
						break;

					case 301:
						AddGameObject<Rabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Rabbit::Wall);
						break;

					default:
						break;
					}
				}
			}
		}

		const auto& enemyGroup = CreateSharedObjectGroup(L"Enemy");

		for (const auto& enemy : enemyVec)
		{
			if (enemy.lock())
			{
				enemyGroup->IntoGroup(enemy.lock());
			}
		}
	}

	void BaseStage::CreateStage(const string& fileName)
	{
		CreateSharedObjectGroup(L"Active");
		const auto& stageGroup = CreateSharedObjectGroup(L"Stage");
		const auto& gimmickGroup = CreateSharedObjectGroup(L"Gimmick");
		const auto& updateGroup = CreateSharedObjectGroup(L"Update");
		vector<weak_ptr<GameObject>> enemyVec;
		if (GetSharedObjectGroup(L"Enemy", false))
		{
			enemyVec = GetSharedObjectGroup(L"Enemy")->GetGroupVector();
		}

		m_csvData = CSVLoader::LoadFile(fileName);

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
		const Vec3 slopeScale = Vec3(scale * 1.45f, scale * 1.45f, scale);
		const Vec2 slopeULeft = Vec2(0.5f, -0.5f);
		const Vec2 slopeURight = Vec2(-0.5f, -0.5f);
		const Vec2 slopeDLeft = Vec2(0.5f, 0.5f);
		const Vec2 slopeDRight = Vec2(-0.5f, 0.5f);

		Checker checker;
		for (size_t i = 0; i < m_csvData.size(); i++)
		{
			for (size_t j = 0; j < m_csvData.at(i).size(); j++)
			{
				if (m_csvData.at(i).at(j) == "") continue;

				shared_ptr<CubeObject> block = nullptr;
				shared_ptr<Gimmick> gimmick = nullptr;
				shared_ptr<Gimmick> update = nullptr;

				const int& num = stoi(m_csvData.at(i).at(j));

				switch (num)
				{
				case 0:
					checker.reset();
					break;

				case 1:
					block = AddGameObject<DeathColl>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, true);
					break;

				case 3:
					block = AddGameObject<StagingColl>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale);
					break;

				case 2:
				case 100:
				case 110:
				case 120:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, true);
					break;

				case 101:
				case 111:
				case 121:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)) + slopeULeft, slopeScale, CubeObject::SlopeUL, true);
					break;

				case 102:
				case 112:
				case 122:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)) + slopeURight, slopeScale, CubeObject::SlopeUR, true);
					break;

				case 103:
				case 113:
				case 123:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)) + slopeDLeft, slopeScale, CubeObject::SlopeDL, true);
					break;

				case 104:
				case 114:
				case 124:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)) + slopeDRight, slopeScale, CubeObject::SlopeDR, true);
					break;

				case 230:
					update = AddGameObject<Ring>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Ring::Big);
					break;

				case 231:
					update = AddGameObject<Ring>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Ring::Small);
					break;

				case 400:
					AddGameObject<GoalCannon>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale * 5.0f);

				default:
					break;
				}

				// 特殊なブロックやギミック用

				// 棘
				if (GetBetween(num, 200, 204))
				{
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&m_csvData.at(i).at(j).at(2)));
					update = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, angle);
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale * 0.9f, true);
				}

				// ベルトコンベア
				if (GetBetween(num, 2101, 2119))
				{
					checker.count++;
					checker.type = m_csvData.at(i).at(j);
					checker.check = m_csvData.at(i).at(j + 1) != checker.type || m_csvData.at(i).at(j - 1) != checker.type;
					const auto& rotate = static_cast<Convayor::eRotate>(atoi(&m_csvData.at(i).at(j).at(2)) / 10);
					const auto& beltType = static_cast<Convayor::eType>(checker.check);
					const float& speed = static_cast<float>(atof(&m_csvData.at(i).at(j).at(3)));
					update = AddGameObject<Convayor>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, rotate, beltType, speed);
				}

				// 大砲
				if (GetBetween(num, 2200, 2283))
				{
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&m_csvData.at(i).at(j).at(2)) / 10);
					const auto& fireType = static_cast<Cannon::eFireType> (atoi(&m_csvData.at(i).at(j).at(3)));
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale * 3.0f, angle, fireType);
				}

				// 送風機
				if (GetBetween(num, 240, 243))
				{
					const Vec3 blowerScale = Vec3(scale * 5.0f, scale, scale * 5.0f);
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&m_csvData.at(i).at(j).at(2)));
					update = AddGameObject<Blower>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), blowerScale, angle, 5.0f);
				}

				if (block)
				{
					block->AddTarget(GetSharedGameObject<Player>(L"Player"));
					block->AddTarget(enemyVec);
					stageGroup->IntoGroup(block);
				}

				if (gimmick)
				{
					gimmick->AddTarget(GetSharedGameObject<Player>(L"Player"));
					gimmickGroup->IntoGroup(gimmick);
				}

				if (update)
				{
					update->AddTarget(GetSharedGameObject<Player>(L"Player"));
					updateGroup->IntoGroup(update);
				}
			}
		}
	}

	void BaseStage::CreateInstanceBlock(const string& fileName)
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
		const auto& data = CSVLoader::LoadFile(fileName);

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

	void BaseStage::OnDestroy()
	{
		if (m_bgm.lock())
		{
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			audioPtr->Stop(m_bgm.lock());
		}
	}

	void BaseStage::OnUpdate()
	{
		try
		{
			const auto& fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
			Debug::Log(L"FPS : ", fps);

			const float range = 55.0f;
			const auto& playerPos = GetSharedGameObject<Player>(L"Player")->GetPosition();

			const auto& enemyVec = GetSharedObjectGroup(L"Enemy")->GetGroupVector();
			for (const auto& weakObj : enemyVec)
			{
				const auto& enemyObj = dynamic_pointer_cast<Enemy>(weakObj.lock());

				if (!enemyObj) continue;

				bool alive = false;
				const Vec3& pos = playerPos;
				float length = (enemyObj->GetPosition() - pos).length();
				enemyObj->SetDrawActive(length <= range);
				enemyObj->SetUpdateActive(length <= range);
			}

			const auto& cubeVec = GetSharedObjectGroup(L"Stage")->GetGroupVector();
			const auto& activeGroup = GetSharedObjectGroup(L"Active");
			activeGroup->AllClear();
			for (const auto& weakObj : cubeVec)
			{
				const auto& cubeObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());

				if (!cubeObj) continue;

				bool alive = false;
				const auto& vec = cubeObj->GetTargetVec();
				for (const auto& v : vec)
				{
					if (!v.lock()) continue;
					if (!v.lock()->GetUpdateActive()) continue;

					const Vec3& pos = v.lock()->GetComponent<Transform>()->GetPosition();
					float length = (cubeObj->GetPosition() - pos).length();
					if (length <= 4.0f)						
					{
						activeGroup->IntoGroup(cubeObj);
						alive = true;
						break;
					}
				}

				cubeObj->SetUpdateActive(alive);

				float length = (cubeObj->GetPosition() - playerPos).length();
				cubeObj->SetDrawActive(length <= range);
			}

			const auto& gimmickVec = GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
			for (const auto& weakObj : gimmickVec)
			{
				const auto& gimmickObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());

				if (!gimmickObj) continue;

				float length = (gimmickObj->GetPosition() - playerPos).length();
				gimmickObj->SetUpdateActive(length <= range);
				gimmickObj->SetDrawActive(length <= range);
			}

			const auto& updateVec = GetSharedObjectGroup(L"Update")->GetGroupVector();

			for (const auto& weakObj : updateVec)
			{
				const auto& gimmickObj = dynamic_pointer_cast<Gimmick>(weakObj.lock());

				if (!gimmickObj) continue;

				float length = (gimmickObj->GetPosition() - playerPos).length();

				if (gimmickObj->FindTag(L"Ring")) gimmickObj->SetUpdateActive(length <= range / 1.5f);
				else gimmickObj->SetUpdateActive(length <= range / 2.0f);
				gimmickObj->SetDrawActive(length <= range);
			}
		}
		catch (...)
		{
			throw;
		}
	}

	void BaseStage::OnDraw()
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