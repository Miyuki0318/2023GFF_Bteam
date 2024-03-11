/*!
@file BaseStage.cpp
@brief ステージの継承元
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;
	using namespace CubeParam;
	using namespace GimmickAngle;
	using namespace CannonParam;

	// リソースの読み込み
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

		// 矢印テクスチャ
		app->RegisterTexture(L"ARROW_TX", texturePath + L"Arrow.png");

		// タイトルに戻るテクスチャ
		app->RegisterTexture(L"QUIT_TX", texturePath + L"Quit.tga");

		// メタルブロックテクスチャ
		app->RegisterTexture(L"METAL_TX", texturePath + L"Metal.png");

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
		app->RegisterWav(L"RABBIT_DEATH_SE", SEPath + L"RabbitDeathSE");

		// バンパーのSE
		app->RegisterWav(L"BUMPER_SE", SEPath + L"BumperSE");

		// ゲームオーバーSE
		app->RegisterWav(L"GAMEOVER_SE", SEPath + L"GameOverSE");
	}

	// BGMの再生
	void BaseStage::CreateBGM(const wstring& bgmKey, float volume)
	{
		// オーディオマネージャーを取得
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();

		// オーディオマネージャーでBGMを再生
		m_bgm = audioPtr->Start(bgmKey, XAUDIO2_LOOP_INFINITE, volume);
	}

	// SEマネージャーの生成
	void BaseStage::CreateSEManager()
	{
		// SEマネージャーがNULLなら
		if (!m_seManager)
		{
			// SEマネージャーをmake_sharedで生成
			m_seManager = make_shared<SEManager>();
		}
	}

	// SEの再生
	void BaseStage::CreateSE(const wstring& seKey, float volume)
	{
		// SEマネージャーがNULLなら
		if (!m_seManager)
		{
			// SEマネージャーの生成を行う
			CreateSEManager();
		}

		// SEマネージャーからSEの再生を送る
		m_seManager->StartSE(seKey, volume, ThisPtr);
	}

	// SEの再生
	void BaseStage::CreateSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// SEマネージャーがNULL
		if (!m_seManager)
		{
			// SEマネージャーの生成を行う
			CreateSEManager();
		}

		// SEマネージャーからSEの再生を送る
		m_seManager->StartSE(seKey, volume, objPtr);
	}

	// SEの停止
	void BaseStage::StopSE(const wstring& seKey)
	{
		// SEマネージャーがあるなら
		if (m_seManager)
		{
			// SEマネージャーからSEの停止を送る
			m_seManager->StopSE(seKey, ThisPtr);
		}
	}

	// SEの停止
	void BaseStage::StopSE(const wstring& seKey, const void* objPtr)
	{
		// SEマネージャーがあるなら
		if (m_seManager)
		{
			// SEマネージャーからSEの停止を送る
			m_seManager->StopSE(seKey, objPtr);
		}
	}

	// プレイヤーの生成
	void BaseStage::CreatePlayer()
	{
		// プレイヤーの生成を行う
		const auto& player = AddGameObject<Player>(Vec3(-20.0f, -80.0f, 0.0f));

		// シェアオブジェクトとして登録
		SetSharedGameObject(L"Player", player);
	}

	// 敵キャラの生成
	void BaseStage::CreateEnemy(const string& fileName)
	{
		// CSVの読み込み
		m_csvData = CSVLoader::LoadFile(fileName);

		// パラメータの宣言
		const float under = -97.5f; // 下限
		const float left = -49.0f;  // 左限
		const float scale = 1.0f;   // スケール

		vector<weak_ptr<JumpRabbit>> jumpVec; // ジャンプウサギ配列
		vector<weak_ptr<WallRabbit>> wallVec; // 壁ウサギ配列

		// 行数分ループ
		for (size_t i = 0; i < m_csvData.size(); i++)
		{
			// 列数分ループ
			for (size_t j = 0; j < m_csvData.at(i).size(); j++)
			{
				// 中身が空なら何もしない
				if (m_csvData.at(i).at(j) == "") continue;

				// int型に変換
				const int& num = stoi(m_csvData.at(i).at(j));

				// 数値がジャンプウサギ(300)と壁ウサギ(301)の範囲内なら
				if (GetBetween(num, 300, 301))
				{
					switch (num)
					{
					case 300: // ジャンプウサギ
						jumpVec.push_back(AddGameObject<JumpRabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale));
						break;

					case 301: // 壁ウサギ
						wallVec.push_back(AddGameObject<WallRabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale));
						break;

					default:
						break;
					}
				}
			}
		}

		// シェアドグループを取得
		const auto& enemyGroup = GetSharedObjectGroup(L"Enemy");
		const auto& wallGroup = GetSharedObjectGroup(L"Wall");

		// オブジェクトの数ループ
		for (const auto& jump : jumpVec)
		{
			// エラーチェック
			if (jump.lock())
			{
				// グループに追加
				enemyGroup->IntoGroup(jump.lock());
			}
		}

		// オブジェクトの数ループ
		for (const auto& wall : wallVec)
		{
			// エラーチェック
			if (wall.lock())
			{
				// グループに追加
				wallGroup->IntoGroup(wall.lock());
			}
		}
	}

	// ゲーム中のステージの生成
	void BaseStage::CreateStage(const string& fileName)
	{
		// プレイヤーとオブジェクトグループの取得
		const auto& playerPtr = GetSharedGameObject<Player>(L"Player");
		const auto& stageGroup = GetSharedObjectGroup(L"Stage");
		const auto& gimmickGroup = GetSharedObjectGroup(L"Gimmick");
		const auto& updateGroup = GetSharedObjectGroup(L"Update");
		const auto& collectGroup = GetSharedObjectGroup(L"Collect");
		const auto& enemyVec = GetSharedObjectGroup(L"Enemy")->GetGroupVector();

		// CSVの読み込み
		m_csvData = CSVLoader::LoadFile(fileName);

		// ベルトコンベア用のチェッカ
		struct Checker
		{
			string type = "";   // タイプ保存用
			int count = 0;		// 連番カウンタ
			bool check = false; // 側面か

			// リセット関数
			void reset()
			{
				type = "";
				count = 0;
				check = false;
			}
		};

		// パラメータの宣言
		const float under = -97.5f; // 下限
		const float left = -49.0f;  // 左限
		const float scale = 1.0f;   // スケール

		// スロープ用
		const Vec3 slopeScale = Vec3(scale * 1.45f, scale * 1.45f, scale); 
		const Vec2 slopeULeft = Vec2(GetSlopeDiff(eCubeType::SlopeUL));
		const Vec2 slopeURight = Vec2(GetSlopeDiff(eCubeType::SlopeUR));
		const Vec2 slopeDLeft = Vec2(GetSlopeDiff(eCubeType::SlopeDL));
		const Vec2 slopeDRight = Vec2(GetSlopeDiff(eCubeType::SlopeDR));

		// 送風機用
		const Vec3 blowerScale = Vec3(scale * 5.0f, scale, scale * 5.0f);

		Checker checker; // チェッカ

		// 行数分ループ
		for (size_t i = 0; i < m_csvData.size(); i++)
		{
			// 列数分ループ
			for (size_t j = 0; j < m_csvData.at(i).size(); j++)
			{
				// 中身が空なら何もしない
				if (m_csvData.at(i).at(j) == "") continue;

				// オブジェクトポインタ
				shared_ptr<CubeObject> block = nullptr;
				shared_ptr<Gimmick> gimmick = nullptr;
				shared_ptr<Gimmick> update = nullptr;
				shared_ptr<Gimmick> collect = nullptr;

				// 座標(左限+列番号×スケール,下限+行数-行番号×スケール)
				Vec2 position = Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale));

				// int型にキャスト
				const int& num = stoi(m_csvData.at(i).at(j));

				// 数値で分岐
				switch (num)
				{
				case 0: // 空白
					checker.reset();
					break;

				case 1: // 死亡用コリジョン
					block = AddGameObject<DeathColl>(position, scale);
					break;

				case 3: // 演出時用コリジョン
					block = AddGameObject<StagingColl>(position, scale);
					break;

				case 2:   // 透明ブロック
				case 100: // Ironブロック
				case 110: // Metalブロック
				case 120: // Darkブロック
					block = AddGameObject<Alpha>(position, scale);
					break;

				case 101: // Iron左上スロープ
				case 111: // Metal左上スロープ
				case 121: // Dark左上スロープ
					block = AddGameObject<Alpha>(position + slopeULeft, slopeScale, CubeParam::eCubeType::SlopeUL);
					break;

				case 102: // Iron右上スロープ
				case 112: // Metal右上スロープ
				case 122: // Dark右上スロープ
					block = AddGameObject<Alpha>(position + slopeURight, slopeScale, CubeParam::eCubeType::SlopeUR);
					break;

				case 103: // Iron左下スロープ
				case 113: // Metal左下スロープ
				case 123: // Dark左下スロープ
					block = AddGameObject<Alpha>(position + slopeDLeft, slopeScale, CubeParam::eCubeType::SlopeDL);
					break;

				case 104: // Iron右下スロープ
				case 114: // Metal右下スロープ
				case 124: // Dark右下スロープ
					block = AddGameObject<Alpha>(position + slopeDRight, slopeScale, CubeParam::eCubeType::SlopeDR);
					break;

				case 400: // ゴール用大砲
					AddGameObject<GoalCannon>(position, scale * 5.0f);
					break;

				default:
					break;
				}

				// 特殊なブロックやギミック用

				// 棘
				if (GetBetween(num, 200, 204))
				{
					// 数値の末尾を角度用enumにキャスト
					const auto angle = static_cast<eAngle>(atoi(&m_csvData.at(i).at(j).at(2)));
					
					// 棘オブジェクトの生成
					update = AddGameObject<Spike>(position, scale, angle);

					// ジャンプウサギグループをコリジョンターゲットに追加
					update->AddTarget(enemyVec);
				}

				// 動く棘
				if (GetBetween(num, 20000, 20200))
				{
					// 数値の100の位を挙動タイプenumにキャスト
					const auto type = static_cast<MoveSpike::eMoveType>(atoi(&m_csvData.at(i).at(j).at(2)) / 100);
					
					// 数値の10と1の位を距離と速度にキャスト
					const float length = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(3)) / 10);
					const float speed = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(4)));
					
					// 動く棘オブジェクトの生成
					gimmick = AddGameObject<MoveSpike>(position, scale, type, speed, length);
					
					// ジャンプウサギグループをコリジョンターゲットに追加
					gimmick->AddTarget(enemyVec);
				}

				// ベルトコンベア
				if (GetBetween(num, 2101, 2119))
				{
					checker.count++; // 連番カウンタを加算
					checker.type = m_csvData.at(i).at(j); // ベルトコンベアタイプを登録

					// 現在の列番号の1つ先、または前が違うベルトコンベアタイプなら
					checker.check = m_csvData.at(i).at(j + 1) != checker.type || m_csvData.at(i).at(j - 1) != checker.type;
					
					// boolの0か1でベルトコンベアタイプを設定
					const auto beltType = static_cast<Convayor::eBeltType>(checker.check);

					// 数値の10と1の位で回転タイプenumと回転速度にキャスト
					const auto rotate = static_cast<Convayor::eRotType>(atoi(&m_csvData.at(i).at(j).at(2)) / 10);
					const float speed = static_cast<float>(atof(&m_csvData.at(i).at(j).at(3)));
					
					// ベルトコンベアオブジェクトの生成
					update = AddGameObject<Convayor>(position, scale, rotate, beltType, speed);
					
					// ジャンプウサギグループをコリジョンターゲットに追加
					update->AddTarget(enemyVec);
				}

				// ベルトコンベアのガイド
				if (num == 310)
				{
					// 自身の1つ下のベルトコンベアIDを取得
					const int convayorID = stoi(m_csvData.at(i + 1).at(j));

					// ベルトコンベアIDの10の位で回転タイプenumにキャスト
					const auto rotate = static_cast<Convayor::eRotType>((convayorID - 2100) / 10);

					// ベルトコンベアガイドオブジェクトの生成
					AddGameObject<ConvayorGuide>(Vec2(left + (j * scale), under + ((m_csvData.size() - i - 1) * scale)), scale, rotate);
				}

				// 大砲
				if (GetBetween(num, 2200, 2283))
				{
					// 数値の10と1の位で角度enumと発射タイプenumにキャスト
					const auto angle = static_cast<eAngle>(atoi(&m_csvData.at(i).at(j).at(2)) / 10);
					const auto fireType = static_cast<eFireType>(atoi(&m_csvData.at(i).at(j).at(3)));

					// 大砲オブジェクトの生成
					gimmick = AddGameObject<Cannon>(position, scale * 3.0f, angle, fireType);
				}

				// タイトル用大砲
				if (GetBetween(num, 2290, 2292))
				{
					// 数値の1の位で難易度enumにキャスト
					const auto diffType = static_cast<eDifficulty>(atoi(&m_csvData.at(i).at(j).at(3)));

					// タイトル用大砲オブジェクトの生成
					gimmick = AddGameObject<TitleCannon>(position, scale * 3.0f, diffType);
				}

				// リング
				if (GetBetween(num, 230, 231))
				{
					// 数値の1の位でリングのサイズenumにキャスト
					const auto size = static_cast<Ring::eRingSize>(atoi(&m_csvData.at(i).at(j).at(2)));
					
					// リングオブジェクトの生成
					collect = AddGameObject<Ring>(position, scale, size);
				}

				// 送風機
				if (GetBetween(num, 240, 243))
				{
					// 数値の1の位で角度enumにキャスト
					const auto angle = static_cast<eAngle>(atoi(&m_csvData.at(i).at(j).at(2)));

					// 送風機オブジェクトの生成
					gimmick = AddGameObject<Blower>(position, blowerScale, angle, 5.0f);
				}

				// 動く壁開閉用ボタン
				if (GetBetween(num, 3110, 3119))
				{
					// 数値の1の位で識別用数値を設定
					const int number = atoi(&m_csvData.at(i).at(j).at(3));

					// ボタンオブジェクトの生成
					gimmick = AddGameObject<Button>(position, scale, number);

					// ジャンプウサギグループをコリジョンターゲットに追加
					gimmick->AddTarget(enemyVec);
				}

				// 動く壁
				if (GetBetween(num, 50000, 57999))
				{
					// 数値の1000と100と10と1の位で挙動タイプenumと開閉距離と開閉速度と識別用数値を設定
					const auto type = static_cast<MoveWall::eMoveType>(atoi(&m_csvData.at(i).at(j).at(1)) / 1000);
					const float length = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(2)) / 100);
					const float speed = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(3)) / 10);
					const int number = atoi(&m_csvData.at(i).at(j).at(4));

					// 動く壁オブジェクトの生成
					gimmick = AddGameObject<MoveWall>(position, scale, type, speed, length, number);

					// ジャンプウサギグループをコリジョンターゲットに追加
					gimmick->AddTarget(enemyVec);
				}

				// バンパー
				if (num == 250)
				{
					// バンパーオブジェクトの生成
					const auto& bumper = AddGameObject<Bumper>(position, scale * 3.0f);

					// ギミックグループに自身を追加
					gimmickGroup->IntoGroup(bumper);
				}

				// 動くバンパー
				if (GetBetween(num, 25000, 25199))
				{
					// 数値の100と10と1の位で挙動タイプenumと移動距離と移動速度を設定
					const auto type = static_cast<MoveBumper::eMoveType>(atoi(&m_csvData.at(i).at(j).at(2)) / 100);
					const float length = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(3)) / 10);
					const float speed = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(4)));

					// 動くバンパーオブジェクトの生成
					const auto& bumper = AddGameObject<MoveBumper>(position, scale * 3.0f, type, speed, length);
					
					// ギミックグループに自身を追加
					gimmickGroup->IntoGroup(bumper);
				}

				// ブロック用ポインタに中身があるなら
				if (block)
				{
					// プレイヤーとジャンプウサギグループをコリジョンターゲットに追加
					block->AddTarget(playerPtr);
					block->AddTarget(enemyVec);

					// ブロック用グループに自身を追加
					stageGroup->IntoGroup(block);
				}

				// ギミック用ポインタに中身があるなら
				if (gimmick)
				{
					// プレイヤーをコリジョンターゲットに追加
					gimmick->AddTarget(playerPtr);

					// ギミック用グループに自身を追加
					gimmickGroup->IntoGroup(gimmick);
				}

				// 動的処理が多いギミック用ポインタに中身があるなら
				if (update)
				{
					// プレイヤーをコリジョンターゲットに追加
					update->AddTarget(playerPtr);

					// 動的処理が多いギミック用グループに自身を追加
					updateGroup->IntoGroup(update);
				}

				// 収集物用ポインタに中身があるなら
				if (collect)
				{
					// プレイヤーをコリジョンターゲットに追加
					collect->AddTarget(playerPtr);

					// 収集物用グループに自身を追加
					collectGroup->IntoGroup(collect);
				}
			}
		}
	}

	// ブロックのインスタンス描画
	void BaseStage::CreateInstanceBlock(const string& fileName)
	{
		// enumの別名
		using eTypes = InstanceBlock::eTextureType;

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
				const auto& type = static_cast<eTypes>(k);
				if (block.at(k).count > 0)
				{
					// ブロック用のインスタンス描画オブジェクトの生成
					AddGameObject<InstanceBlock>(block.at(k).num, type, rowSize, i);
				}
				if (slope.at(k).count > 0)
				{
					// スロープ用のインスタンス描画オブジェクトの生成
					AddGameObject<InstanceSlope>(slope.at(k).num, type, rowSize, i);
				}
			}
		}
	}

	// コリジョンアクティブグループに追加
	void BaseStage::ObjectInToAvtiveGroup(const vector<weak_ptr<GameObject>>& groupVec, const shared_ptr<GameObjectGroup>& activeGroup, const Vec3& playerPos, float updateRange)
	{
		// パラメータの宣言
		const Vec2 margin = Vec2(25.0f); // 余剰
		const Vec3 pLeft = Vec3(WINDOW_SIZE + margin, 1.0f); // 左端
		const Vec3 pRight = Vec3(-WINDOW_SIZE - margin, 0.0f); // 右端

		// オブジェクトの数ループ
		for (const auto& weakObj : groupVec)
		{
			// エラーチェック
			if (!weakObj.lock()) continue;

			// 型キャストとエラーチェック
			const auto& cubeObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());
			if (!cubeObj) continue;

			// アクティブかの真偽
			bool active = false;

			// 距離を比較するターゲット配列を取得
			const auto& vec = cubeObj->GetTargetVec();
			if (vec.empty()) continue;

			// コリジョンターゲットの数ループ
			for (const auto& v : vec)
			{
				// エラーチェックとアクティブかのチェック
				if (!v.lock()) continue;
				if (!v.lock()->GetUpdateActive()) continue;

				// ターゲットの座標との距離を求める
				const Vec3& pos = v.lock()->GetComponent<Transform>()->GetPosition();
				float length = (cubeObj->GetPosition() - pos).length();

				// 更新距離内なら
				if (length <= updateRange)
				{
					// アクティブグループに追加
					activeGroup->IntoGroup(cubeObj);
					active = true;
					break;
				}
			}

			// 更新するかはターゲットとの距離で行う
			cubeObj->SetUpdateActive(active);

			// 描画するかは画面内かで行う
			Vec3 point = Utility::ConvertToWorldPosition(m_gameView, cubeObj->GetPosition());
			cubeObj->SetDrawActive(Utility::GetBetween(point, pLeft, pRight));
		}
	}

	// 生成時の処理
	void BaseStage::OnCreate()
	{
		try
		{
			//各パフォーマンスを得る
			SetCollisionPerformanceActive(true);
			SetUpdatePerformanceActive(true);
			SetDrawPerformanceActive(true);

			// シェアグループの作成
			CreateSharedObjectGroup(L"Stage");
			CreateSharedObjectGroup(L"Active");
			CreateSharedObjectGroup(L"Gimmick");
			CreateSharedObjectGroup(L"Update");
			CreateSharedObjectGroup(L"Collect");
			CreateSharedObjectGroup(L"Enemy");
			CreateSharedObjectGroup(L"Wall");

			// タイマーオブジェクトの生成
			m_timer = AddGameObject<Timer>();
		}
		catch (...)
		{
			throw;
		}
	}

	// 破棄時の処理
	void BaseStage::OnDestroy()
	{
		// bgmが設定されてるなら
		if (m_bgm.lock())
		{
			// オーディオマネージャーからbgmの停止を送る
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			audioPtr->Stop(m_bgm.lock());
		}
	}

	// 毎フレーム更新処理
	void BaseStage::OnUpdate()
	{
		try
		{
			//// FPSの描画
			//const auto& fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
			//Debug::Log(L"FPS : ", fps);
			
			// 範囲
			const float range = 55.0f;
			const float cubeRange = 4.0f;

			// プレイヤーの座標
			const auto& playerPos = GetSharedGameObject<Player>(L"Player")->GetPosition();

			// オブジェクトグループの取得とその配列
			const auto& enemyVec = GetSharedObjectGroup(L"Enemy")->GetGroupVector();
			const auto& wallVec = GetSharedObjectGroup(L"Wall")->GetGroupVector();
			const auto& gimmickVec = GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
			const auto& updateVec = GetSharedObjectGroup(L"Update")->GetGroupVector();
			const auto& collectVec = GetSharedObjectGroup(L"Collect")->GetGroupVector();
			const auto& stageVec = GetSharedObjectGroup(L"Stage")->GetGroupVector();
			const auto& activeGroup = GetSharedObjectGroup(L"Active");

			// パフォーマンス管理関数を実行
			ObjectPerformance<JumpRabbit>(enemyVec, playerPos, range / 1.5f);
			ObjectPerformance<WallRabbit>(wallVec, playerPos, range / 1.5f);
			ObjectPerformance<Gimmick>(gimmickVec, playerPos, range);
			ObjectPerformance<Gimmick>(updateVec, playerPos, range / 2.0f);
			ObjectPerformance<Gimmick>(collectVec, playerPos, range / 1.5f);

			// ステージオブジェクトグループだけ特殊
			// アクティブになっているオブジェクトのグループをリセット
			activeGroup->AllClear();
			ObjectInToAvtiveGroup(stageVec, activeGroup, playerPos, cubeRange);
			ObjectInToAvtiveGroup(updateVec, activeGroup, playerPos, range / 2.0f);
		}
		catch (...)
		{
			throw;
		}
	}

	// 描画処理
	void BaseStage::OnDraw()
	{
		// デバッグ文字列を強制的に空にする
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// 継承元の描画時の関数を実行する
		Stage::OnDraw();

		// デバック用文字列の表示非表示切り替え
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(false);
	}
}