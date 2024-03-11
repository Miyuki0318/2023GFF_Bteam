/*!
@file GameStage.cpp
@brief ゲームステージ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	// ネームスペースの省略
	using namespace Input;
	using namespace Utility;

	// リソースの読み込み
	void GameStage::CreateResourses()
	{
		// 継承元のリソースの読み込みを実行する
		BaseStage::CreateResourses();

		// アプリケーションの取得
		const auto& app = App::GetApp();

		// メディアディレクトリパスの取得
		const wstring mediaPath = app->GetDataDirWString();

		// テクスチャディレクトリパスの取得
		const wstring texturePath = mediaPath + L"Textures/";

		// 発見時のテクスチャ
		app->RegisterTexture(L"DISCOVER_TX", texturePath + L"Discovered.png");

		// UI用テクスチャ
		app->RegisterTexture(L"JGAUGE_TX", texturePath + L"JumpUI.png");
		app->RegisterTexture(L"SGAUGE_TX", texturePath + L"ShieldUI.png");
		app->RegisterTexture(L"GAUGE_TX", texturePath + L"Gauge.png");
		app->RegisterTexture(L"N_EFFECT_TX", texturePath + L"WhiteEffect.png");
		app->RegisterTexture(L"GAMEOVER_TX", texturePath + L"GameOver.png");
		app->RegisterTexture(L"CONTINUE_TX", texturePath + L"Continue.png");
		app->RegisterTexture(L"GUIDE_TX", texturePath + L"ConvayorGuide.png");

		// サウンドディレクトリパスの取得
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// ゲーム用BGM
		app->RegisterWav(L"GAME_BGM", BGMPath + L"GameBGM");

		// クリア時のSE
		app->RegisterWav(L"CLEAR_SE", SEPath + L"GameClearSE");
	}

	// ビューとライトの生成
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

	// 背景の生成
	void GameStage::CreateBackGround()
	{
		// オブジェクトの生成
		m_backObj = AddGameObject<TemplateObject>();

		// パラメータの宣言
		float x, y, loopX, loopY;
		loopX = 16; // X軸のループ数
		loopY = 12; // Y軸のループ数
		x = 80.0f * loopX;
		y = 45.0f * loopY;

		const auto& backObj = m_backObj.lock();
		backObj->SetPosition(Vec3(500.0f, 135.0f, 50.0f)); // 座標の設定
		backObj->SetScale(Vec3(x, y, 5.0f)); // スケールの設定
		backObj->SetAlphaActive(true); // 透明色の描画を可能に

		VertexData vertex; // 頂点データの生成
		SimpleVerticesIndices(vertex); // 四角形で設定

		// UV座標をループ数で設定
		vertex.vertices.at(1).textureCoordinate = Vec2(loopY, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, loopX);
		vertex.vertices.at(3).textureCoordinate = Vec2(loopY, loopX);

		// 描画コンポーネントの取得
		auto backDraw = backObj->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	// プレイヤーの生成
	void GameStage::CreatePlayer()
	{
		// プレイヤーの生成を行う
		auto player = AddGameObject<Player>(Vec3(-48.5f, -75.0f, 0.0f));

		// シェアオブジェクトとして登録
		SetSharedGameObject(L"Player", player);
	}

	// スプライトの生成
	void GameStage::CreateSprites()
	{
		// フェード用スプライト
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));
		
		// メタルウィンドウ用スプライト
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE * 1.25f, Vec3(-675.0f, 0.0f, 0.1f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE * 1.25f, Vec3(675.0f, 0.0f, 0.1f));
		
		// ゲームオーバー時のスプライト
		m_gameOver = AddGameObject<Sprite>(L"GAMEOVER_TX", WINDOW_SIZE, Vec3(0.0f, 600.0f, 0.2f));
		m_continue = AddGameObject<Sprite>(L"CONTINUE_TX", WINDOW_SIZE * 0.55f, Vec3(-300.0f, 600.0f, 0.2f));
		m_titleBack = AddGameObject<Sprite>(L"QUIT_TX", WINDOW_SIZE * 0.55f, Vec3(300.0f, 600.0f, 0.2f));
	}

	// UIの生成
	void GameStage::CreateUI()
	{
		// スケールの設定
		const Vec2 scale = Vec2(25.0f, 150.0f) * 1.25f;

		// ジャンプ数とシールド数のUIを生成
		AddGameObject<JumpCountUI>(scale, Vec3(600.0f, -225.0f, 0.3f));
		AddGameObject<ShieldCountUI>(scale, Vec3(525.0f, -225.0f, 0.3f));
	}

	// 死亡時のフェードステート
	void GameStage::DeathFadeState()
	{
		// 確スプライトの座標
		const Vec3& goPos = m_gameOver.lock()->GetPosition();
		const Vec3& coPos = m_continue.lock()->GetPosition();
		const Vec3& tbPos = m_titleBack.lock()->GetPosition();

		// ゲームオーバースプライトの高さが一定以上なら
		if (goPos.y >= 51.0f)
		{
			// 確スプライトを下に移動させる
			m_gameOver.lock()->SetPosition(goPos + Vec3(0.0f, -DELTA_TIME * 600.0f, 0.0f));
			m_continue.lock()->SetPosition(coPos + Vec3(0.0f, -DELTA_TIME * 800.0f, 0.0f));
			m_titleBack.lock()->SetPosition(tbPos + Vec3(0.0f, -DELTA_TIME * 800.0f, 0.0f));
		}
		else
		{
			// 選択肢ステートに変更
			m_stageState = Select;
		}
	}
	
	// 選択肢ステート
	void GameStage::SelectState()
	{
		// Lスティックの入力真偽
		const bool inputLStick = IsInputLStickX();

		// スケール
		const Vec2 deffScale = WINDOW_SIZE * 0.55f;

		// 経過時間
		m_totalTime += DELTA_TIME * 5.0f;

		// 入力があり、前フレームに入力が無ければ
		if (inputLStick && !m_currentStickX)
		{
			// 経過時間をリセット
			m_totalTime = 0.0f;

			// 選択肢に応じて処理
			switch (m_select)
			{
			case GameStage::Continue: // コンティニューだったら
				m_select = TitleBack; // 切り替える
				break;

			case GameStage::TitleBack: // タイトルに戻るだったら
				m_select = Continue;   // 切り替える
				break;

			default:
				break;
			}
		}

		// 選択してるスプライトをサインカーブでバウンド処理
		m_continue.lock()->SetScale(deffScale * (m_select == Continue ? SinCurve(m_totalTime ,1.0f, 1.2f) : 1.0f));
		m_titleBack.lock()->SetScale(deffScale * (m_select == TitleBack ? SinCurve(m_totalTime, 1.0f, 1.2f) : 1.0f));
		
		// 入力真偽を保持
		m_currentStickX = inputLStick;

		// Aボタン入力があれば
		if (GetPushA())
		{
			// メタルウィンドウステートに変更
			m_stageState = Metal;

			// メタルウィンドウの開閉時のSEを再生
			CreateSE(L"METAL_SE", 0.75f);
		}
	}

	// メタルウィンドウステート
	void GameStage::MetalState()
	{
		// 各スプライトの座標を取得
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& goPos = m_gameOver.lock()->GetPosition();
		const Vec3& coPos = m_continue.lock()->GetPosition();
		const Vec3& tbPos = m_titleBack.lock()->GetPosition();

		// メタルウィンドウの左側が0.0より小さい(左側に居れば)なら
		if (mLPos.x <= 0.0f)
		{
			// メタルウィンドウを中心位置に移動させる
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(DELTA_TIME * 850.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(-DELTA_TIME * 850.0f, 0.0f, 0.0f));
			
			// ゲームオーバー用のスプライトを画面外に移動させる
			m_gameOver.lock()->SetPosition(goPos + Vec3(0.0f, DELTA_TIME * 545.0f, 0.0f));
			m_continue.lock()->SetPosition(coPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
			m_titleBack.lock()->SetPosition(tbPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
		}
		else
		{
			// メタルウィンドウの開閉時のSEを停止
			StopSE(L"METAL_SE");

			// メタルウィンドウ停止時のSEを再生
			CreateSE(L"METAL_STOP_SE", 1.5f);

			// 中心位置に修正
			m_metalLeft.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.1f));
			m_metalRight.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.1f));

			// 画面外に修正
			m_gameOver.lock()->SetPosition(Vec3(0.0f, 600.0f, 0.2f));
			m_continue.lock()->SetPosition(Vec3(-300.0f, 600.0f, 0.2f));
			m_titleBack.lock()->SetPosition(Vec3(300.0f, 600.0f, 0.2f));

			// スケールの修正
			m_continue.lock()->SetScale(WINDOW_SIZE * 0.55f);
			m_titleBack.lock()->SetScale(WINDOW_SIZE * 0.55f);

			// プレイヤーの取得
			const auto& player = GetSharedGameObject<Player>(L"Player");

			// 選択肢で分岐
			switch (m_select)
			{
			case GameStage::Continue: // コンティニューなら
				ResetStage(); // ステージのリセットを実行
				player->Reset(); // プレイヤーのリセットを実行
				CreateSE(L"METAL_SE", 0.75f); // メタルウィンドウの開閉SEを再生
				m_totalTime = 0.0f; // 経過時間を初期化
				m_stageState = Reset; // ステージステートをリセットに
				break;

			case GameStage::TitleBack: // タイトルに戻るなら
				m_stageState = FadeOut; // フェードアウトステートに
				CreateSE(L"GAMEOVER_SE", 1.0f); // ゲームオーバー時のSEを再生
				break;

			default:
				break;
			}
		}
	}

	// リセットステート
	void GameStage::ResetState()
	{
		// メタルウィンドウの座標を取得
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();

		// 左側のメタルウィンドウの座標が画面内なら
		if (mLPos.x > -WINDOW_WIDTH / 1.9f)
		{
			// 画面外に移動させる
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(-DELTA_TIME * 850.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(DELTA_TIME * 850.0f, 0.0f, 0.0f));
		}
		else
		{
			// メタルウィンドウの開閉時のSEを停止
			StopSE(L"METAL_SE");

			// メタルウィンドウの停止時のSEと大砲発射時のSEを再生
			CreateSE(L"METAL_STOP_SE", 1.5f);
			CreateSE(L"CANNON_SE", 1.0f);

			// 開閉時の移動演出ステートに変更
			m_stageState = StartMove;

			// メタルウィンドウの座標を修正
			m_metalLeft.lock()->SetPosition(Vec3(-675.0f, 0.0f, 0.1f));
			m_metalRight.lock()->SetPosition(Vec3(675.0f, 0.0f, 0.1f));
		}
	}

	// フェードアウトステート
	void GameStage::FadeOutState(float fadeTime)
	{
		// シーンを取得
		const auto& scene = App::GetApp()->GetScene<Scene>();

		// フェードが完了したら
		if (m_fade.lock()->FadeInColor(fadeTime))
		{
			// ゲームをクリアしてるか
			if (m_isClear)
			{
				// シーンに現在のステージ名を送る
				scene->SetCurrentStage(m_stagePath);

				// 「次のステージへ」ステージに遷移
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"NextStage");
			}
			else
			{
				// 「タイトル」ステージに遷移
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"TitleStage");
			}
		}
	}

	// ステージのリセット
	void GameStage::ResetStage()
	{
		// プレイヤーとオブジェクトグループの取得
		const auto& collectGroup = GetSharedObjectGroup(L"Collect");
		const auto& enemyGroup = GetSharedObjectGroup(L"Enemy");
		const auto& wallGroup = GetSharedObjectGroup(L"Wall");
		const auto& stageVec = GetSharedObjectGroup(L"Stage")->GetGroupVector();
		const auto& updateVec = GetSharedObjectGroup(L"Update")->GetGroupVector();
		const auto& playerPtr = GetSharedGameObject<Player>(L"Player");

		// グループ内のオブジェクトを削除
		GroupObjectRemove(collectGroup);
		GroupObjectRemove(enemyGroup);
		GroupObjectRemove(wallGroup);

		// グループの初期化
		collectGroup->AllClear();
		enemyGroup->AllClear();
		wallGroup->AllClear();

		// パラメータの宣言
		const float under = -97.5f; // 下限
		const float left = -49.0f;  // 左限
		const float scale = 1.0f;   // スケール

		// オブジェクト配列
		vector<weak_ptr<Ring>> ringVec;
		vector<weak_ptr<JumpRabbit>> jumpVec;
		vector<weak_ptr<WallRabbit>> wallVec;

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

				// 数値がリング(230or231)と壁ウサギ(301)の範囲内なら
				if (GetBetween(num, 230, 301))
				{
					Vec2 position = Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale));

					switch (num)
					{
					case 230: // 大きいリングなら

						// 大きいリングオブジェクトを生成して配列に追加
						ringVec.push_back(AddGameObject<Ring>(position, scale, Ring::eRingSize::Big));
						break;

					case 231: // 小さいリングなら

						// 小さいリングオブジェクトを生成して配列に追加
						ringVec.push_back(AddGameObject<Ring>(position, scale, Ring::eRingSize::Small));
						break;

					case 300: // ジャンプウサギなら

						// ジャンプウサギオブジェクトを生成して配列に追加
						jumpVec.push_back(AddGameObject<JumpRabbit>(position, scale));
						break;

					case 301: // 壁ウサギなら

						// 壁ウサギオブジェクトを生成して配列に追加
						wallVec.push_back(AddGameObject<WallRabbit>(position, scale));
						break;

					default:
						break;
					}
				}
			}
		}

		// リングの数ループ
		for (const auto& ring : ringVec)
		{
			// エラーチェック
			if (ring.lock())
			{
				// コリジョンターゲットにプレイヤーを追加
				ring.lock()->AddTarget(playerPtr);

				// 収集物グループに自身を追加
				collectGroup->IntoGroup(ring.lock());
			}
		}

		// ジャンプウサギの数ループ
		for (const auto& jump : jumpVec)
		{
			// エラーチェック
			if (jump.lock())
			{
				// ジャンプウサギグループに自身を追加
				enemyGroup->IntoGroup(jump.lock());
			}
		}

		// 壁ウサギの数ループ
		for (const auto& wall : wallVec)
		{
			// エラーチェック
			if (wall.lock())
			{
				// 壁ウサギグループに自身を追加
				wallGroup->IntoGroup(wall.lock());
			}
		}

		// ステージオブジェクトの数ループ
		for (const auto& weakObj : stageVec)
		{
			// エラーチェック
			if (!weakObj.lock()) continue;

			// 型キャスト
			const auto& block = dynamic_pointer_cast<CubeObject>(weakObj.lock());
			if (!block) continue;

			// コリジョンターゲットを削除して再設定
			block->RemoveTarget();
			block->AddTarget(enemyGroup->GetGroupVector());
			block->AddTarget(playerPtr);
		}

		// 動的処理が多いギミックオブジェクトの数ループ
		for (const auto& weakObj : updateVec)
		{
			// エラーチェック
			if (!weakObj.lock()) continue;

			// 型キャスト
			const auto& update = dynamic_pointer_cast<CubeObject>(weakObj.lock());
			if (!update) continue;

			// コリジョンターゲットを削除して再設定
			update->RemoveTarget();
			update->AddTarget(enemyGroup->GetGroupVector());
			update->AddTarget(playerPtr);
		}
	}

	// 生成時の処理
	void GameStage::OnCreate() 
	{
		try
		{
			// 継承元の生成時の処理を実行
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

			// ステージ生成
			CreateEnemy(m_stagePath);
			CreateStage(m_stagePath);
			CreateInstanceBlock(m_stagePath);
		}
		catch (...) 
		{
			throw;
		}
	}

	// 毎フレーム更新処理
	void GameStage::OnUpdate()
	{
		try
		{
			// 継承元の毎フレーム更新処理を実行
			BaseStage::OnUpdate();

			// ステートで分岐処理
			switch (m_stageState)
			{
			case GameStage::FadeIn: // フェードインなら

				// フェードが終わったら開始時の移動演出ステートに変更
				if (m_fade.lock()->FadeOutColor(1.0f)) m_stageState = StartMove;
				break;

			case GameStage::Goal: // ゴール時なら
				m_isClear = true; // クリア真偽を取る
				FadeOutState(3.75f); // フェードアウト処理
				break;

			case GameStage::Death: // 死亡後なら
				DeathFadeState(); // 死亡時のフェードステート
				break;

			case GameStage::Select: // 選択肢なら
				SelectState(); // 選択肢ステート
				break;

			case GameStage::Metal: // メタルウィンドウなら
				MetalState(); // メタルウィンドウステート
				break;

			case GameStage::Reset: // コンティニューリセットなら
				ResetState(); // コンティニューリセットステート
				break;

			case GameStage::FadeOut: // フェードアウトなら
				FadeOutState(3.0f); // フェードアウトステート
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