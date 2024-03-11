/*!
@file NextStage.cpp
@brief 「次のステージへ」ステージ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;
	using namespace Utility;

	// リソースの読み込み
	void NextStage::CreateResourses()
	{
		// 継承元の関数
		BaseStage::CreateResourses();

		// アプリケーションの取得
		const auto& app = App::GetApp();

		// メディアディレクトリパスの取得
		const wstring mediaPath = app->GetDataDirWString();

		// テクスチャディレクトリパスの取得
		const wstring texturePath = mediaPath + L"Textures/";

		// 次への選択肢の読み込み
		app->RegisterTexture(L"GO_TX", texturePath + L"Go.png");
		app->RegisterTexture(L"NEXT_TX", texturePath + L"NextStage.png");

		// テクスチャの取得
		// サウンドディレクトリパス
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// サウンドの取得
		// ネクスト画面BGMの読み込み
		app->RegisterWav(L"NEXT_BGM", BGMPath + L"NextBGM");
	}

	// ビューとライトの生成
	void NextStage::CreateViewLight()
	{
		// ビューのカメラの設定
		auto ptrCamera = ObjectFactory::Create<Camera>();
		ptrCamera->SetEye(Vec3(0.0f, 0.0f, -33.0f));
		ptrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		m_gameView = CreateView<SingleView>();
		m_gameView->SetCamera(ptrCamera);

		// マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		ptrMultiLight->SetDefaultLighting();
	}

	// 背景の生成
	void NextStage::CreateBackGround()
	{
		// オブジェクトの生成
		m_backObj = AddGameObject<TemplateObject>();

		// パラメータの宣言
		float x, y, loop;
		loop = 2.0f; // ループ数
		x = 80.0f * loop;
		y = 45.0f * loop;

		const auto& backObj = m_backObj.lock();
		backObj->SetPosition(Vec3(0.0f, 0.0f, 50.0f)); // 座標の設定
		backObj->SetScale(Vec3(x, y, 5.0f)); // スケールの設定
		backObj->SetAlphaActive(true); // 透明色の描画を可能に

		VertexData vertex; // 頂点データの生成
		SimpleVerticesIndices(vertex); // 四角形で設定

		// UV座標をループ数で設定
		vertex.vertices.at(1).textureCoordinate = Vec2(loop, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, loop);
		vertex.vertices.at(3).textureCoordinate = Vec2(loop, loop);

		// 描画コンポーネントの取得
		auto backDraw = backObj->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	// プレイヤーの生成
	void NextStage::CreatePlayer()
	{
		// プレイヤーの生成を行う
		AddGameObject<NextPlayer>(Vec3(0.0f));
	}

	// スプライトの生成
	void NextStage::CreateSprites()
	{
		// フェード用スプライト
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));
		
		// ロゴと選択肢用スプライト
		m_logo = AddGameObject<Sprite>(L"NEXT_TX", WINDOW_SIZE, Vec3(0.0f, 200.0f, 0.2f));
		m_next = AddGameObject<Sprite>(L"GO_TX", WINDOW_SIZE * 0.55f, Vec3(-300.0f, 600.0f, 0.2f));
		m_back = AddGameObject<Sprite>(L"QUIT_TX", WINDOW_SIZE * 0.55f, Vec3(300.0f, 600.0f, 0.2f));

		// メタルウィンドウ用スプライト
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE, Vec3(-675.0f, 0.0f, 0.2f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE, Vec3(675.0f, 0.0f, 0.2f));
	}

	// 背景のスクロール
	void NextStage::SlideBackGround()
	{
		// 背景オブジェクトから描画コンポーネントを取得
		const auto& backObj = m_backObj.lock();
		const auto backDraw = backObj->GetComponent<PCTStaticDraw>();

		// 頂点データのUV座標をデルタタイムで加算してスクロール処理
		auto& vertices = backDraw->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
		for (auto& v : vertices) v.textureCoordinate += Vec2(1.0f, 0.0f) * DELTA_TIME;
		backDraw->UpdateVertices(vertices);
	}

	// 選択肢ステート
	void NextStage::SelectState()
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
			case NextStage::Next: // 次のステージへなら
				m_select = Back;  // タイトルに戻るに切り替える
				break;

			case NextStage::Back: // タイトルに戻るなら
				m_select = Next;  // 次のステージへに切り替える
				break;

			default:
				break;
			}
		}

		// 入力真偽を保持
		m_currentStickX = inputLStick;

		// 選択してるスプライトをサインカーブでバウンド処理
		m_next.lock()->SetScale(deffScale * (m_select == Next ? SinCurve(m_totalTime, 1.0f, 1.2f) : 1.0f));
		m_back.lock()->SetScale(deffScale * (m_select == Back ? SinCurve(m_totalTime, 1.0f, 1.2f) : 1.0f));
	}

	// メタルウィンドウステート
	void NextStage::MetalState()
	{
		// 各スプライトの座標を取得
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& nxPos = m_next.lock()->GetPosition();
		const Vec3& tbPos = m_back.lock()->GetPosition();

		// メタルウィンドウの左側が0.0より小さい(左側に居れば)なら
		if (mLPos.x < 0.0f)
		{
			// メタルウィンドウを中心位置に移動させる
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(DELTA_TIME * 750.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(-DELTA_TIME * 750.0f, 0.0f, 0.0f));
			
			// ゲームオーバー用のスプライトを画面外に移動させる
			m_next.lock()->SetPosition(nxPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
			m_back.lock()->SetPosition(tbPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
		}
		else
		{
			// メタルウィンドウの開閉時のSEを停止
			StopSE(L"METAL_SE");

			// メタルウィンドウ停止時のSEを再生
			CreateSE(L"METAL_STOP_SE", 1.5f);

			// 中心位置に修正
			m_metalLeft.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.2f));
			m_metalRight.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.2f));

			// フェードアウトステートへ切り替える
			m_stageState = FadeOut;
		}
	}

	// フェードインステート
	void NextStage::FadeInState()
	{
		// フェード用スプライトの取得
		const auto& fade = m_fade.lock();

		// スプライトの座標を取得
		const Vec3& nextPos = m_next.lock()->GetPosition();
		const Vec3& backPos = m_back.lock()->GetPosition();

		// フェードアウトを掛ける
		if (fade->FadeOutColor(2.0f))
		{
			m_stageState = Select; // ステートを選択肢ステートに切り替える
		}
		else
		{
			// LerpでY軸座標を求めて設定
			float y = Utility::Lerp(600.0f, -100.0f, 1.0f - fade->GetDiffuseColor().w);
			m_next.lock()->SetPosition(nextPos.x, y, nextPos.z);
			m_back.lock()->SetPosition(backPos.x, y, backPos.z);
		}
	}

	// フェードアウトステート
	void NextStage::FadeOutState()
	{
		// フェード用スプライトの取得
		const auto& fade = m_fade.lock();

		// シーンオブジェクトの取得
		const auto& scene = App::GetApp()->GetScene<Scene>();
		
		// スプライトの座標を取得
		const Vec3& nextPos = m_next.lock()->GetPosition();
		const Vec3& backPos = m_back.lock()->GetPosition();

		// フェードインを掛ける
		if (fade->FadeInColor(1.0f))
		{
			// 選択肢で分岐処理
			switch (m_select)
			{
			case NextStage::Next: // 次のステージへなら

				// シーンからゲームステージへ遷移させる
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
				break;

			case NextStage::Back: // タイトルに戻るなら

				// シーンからタイトルステージに遷移させる
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"TitleStage");
				break;

			default:
				break;
			}
		}
		else
		{
			// LerpでY軸座標を求めて設定する
			float y = Utility::Lerp(-100.0f, 600.0f, fade->GetDiffuseColor().w);
			m_next.lock()->SetPosition(nextPos.x, y, nextPos.z);
			m_back.lock()->SetPosition(backPos.x, y, backPos.z);
		}
	}

	// 生成時の処理
	void NextStage::OnCreate()
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
			CreateBGM(L"NEXT_BGM", 0.25f);

			// プレイヤーの作成
			CreatePlayer();

			// スプライトの作成
			CreateSprites();
		}
		catch (...)
		{
			throw;
		}
	}

	// 毎フレーム更新処理
	void NextStage::OnUpdate()
	{
		try
		{
			// 背景のスクロール
			SlideBackGround();

			// ステートに応じて処理
			switch (m_stageState)
			{
			case NextStage::FadeIn: // フェードインなら
				FadeInState(); // フェードインステート処理を実行
				break;

			case NextStage::Select:	// 選択肢なら
				SelectState(); // 選択肢ステート処理を実行
				break;

			case NextStage::MetalFade: // メタルフェードなら
				MetalState(); // メタルウィンドウステート処理を実行
				break;

			case NextStage::FadeOut: // フェードアウトなら
				FadeOutState(); // フェードアウトステート処理を実行
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