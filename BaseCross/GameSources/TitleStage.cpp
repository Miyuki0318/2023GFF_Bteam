/*!
@file TitleStage.cpp
@brief タイトル用ステージ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;
	using namespace Utility;

	// リソースの読み込み
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

		// テクスチャの取得
		// タイトルロゴの読み込み
		app->RegisterTexture(L"TITLE_LOGO", texturePath + L"TitleLogo.tga");

		// スタートボタンの読み込み
		app->RegisterTexture(L"PUSH_BUTTON", texturePath + L"PushButton.tga");

		// サウンドディレクトリパス
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// サウンドの取得
		// タイトル画面BGMの読み込み
		app->RegisterWav(L"TITLE_BGM", BGMPath + L"TitleBGM");
	}

	// ビューとライトの生成
	void TitleStage::CreateViewLight()
	{
		// ビューのカメラの設定
		auto ptrCamera = ObjectFactory::Create<Camera>();
		ptrCamera->SetEye(Vec3(-20.0f, -75.0f, -40.0f));
		ptrCamera->SetAt(Vec3(-20.0f, -75.0f, 0.0f));
		m_gameView = CreateView<SingleView>();
		m_gameView->SetCamera(ptrCamera);

		// マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		ptrMultiLight->SetDefaultLighting();
	}

	// 背景の生成
	void TitleStage::CreateBackGround()
	{
		// オブジェクトの生成
		m_backObj = AddGameObject<TemplateObject>();

		// パラメータの宣言
		float x, y, loop;
		loop = 5.0f; // ループ数
		x = 80.0f * loop;
		y = 45.0f * loop;

		const auto& backObj = m_backObj.lock(); 
		backObj->SetPosition(Vec3(-90.0f, 0.0f, 50.0f));// 座標の設定
		backObj->SetScale(Vec3(x, y, 5.0f)); // スケールの設定
		backObj->SetAlphaActive(true);// 透明色の描画を可能に

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
	void TitleStage::CreatePlayer()
	{
		// プレイヤーの生成を行う
		m_player = AddGameObject<TitlePlayer>(Vec3(-50.0f, -55.0f, 0.0f));

		// シェアオブジェクトとして登録
		SetSharedGameObject(L"Player", m_player.lock());
	}

	// スプライトの生成
	void TitleStage::CreateSprites()
	{
		// フェード用スプライト
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));

		// タイトルロゴとプッシュボタンスプライト
		m_titleLogo = AddGameObject<Sprite>(L"TITLE_LOGO", WINDOW_SIZE, Vec3(0.0f, 50.0f, 0.1f));
		m_pushButton = AddGameObject<Sprite>(L"PUSH_BUTTON", WINDOW_SIZE / 2.0f, Vec3(0.0f, -200.0f, 0.1f));
		
		// メタルウィンドウ用スプライト
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE, Vec3(0.0f, 0.0f, 0.2f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE, Vec3(0.0f, 0.0f, 0.2f));
	}

	// プッシュボタンステート
	void TitleStage::PushButtonState()
	{
		// Aボタン入力があれば
		if (GetPushA())
		{
			// ステートをメタルフェードに変更
			m_stageState = MetalFadeOut;

			// プッシュボタンとメタルウィンドウのSEを再生
			CreateSE(L"PUSH_SE", 1.5f);
			CreateSE(L"METAL_SE", 0.75f);
		}
	}

	// メタルウィンドウステート
	void TitleStage::MetalFadeState()
	{
		// 各スプライトの座標を取得
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& logoPos = m_titleLogo.lock()->GetPosition();
		const Vec3& pushPos = m_pushButton.lock()->GetPosition();

		// メタルウィンドウ左側が画面外に行くまで
		if (mLPos.x > -WINDOW_WIDTH / 1.9f)
		{
			// メタルウィンドウは左右で画面外に移動させる
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(-DELTA_TIME * 250.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(DELTA_TIME * 250.0f, 0.0f, 0.0f));

			// ロゴとプッシュボタンは上下で画面外に移動させる
			m_titleLogo.lock()->SetPosition(logoPos + Vec3(0.0f, DELTA_TIME * 200.0f, 0.0f));
			m_pushButton.lock()->SetPosition(pushPos + Vec3(0.0f, -DELTA_TIME * 100.0f, 0.0f));
		}
		else
		{
			// メタルウィンドウのSEを停止させ、停止時のSEを再生
			StopSE(L"METAL_SE");
			CreateSE(L"METAL_STOP_SE", 1.5f);

			// 開始時の移動演出ステートに変更
			m_stageState = StartMove;
		}
	}

	// 開始時の移動演出ステート
	void TitleStage::StartMoveState()
	{
		// プレイヤーの座標と着地先の範囲座標
		const Vec3& playerPos = m_player.lock()->GetPosition();
		const Vec3 leftUnder = Vec3(-22.0f, -89.0f, 0.0f);  // 着地先の左下
		const Vec3 rightUpper = Vec3(-20.0f, -88.0f, 0.0f); // 着地先の右上

		// プレイヤーの座標が範囲内なら
		if (GetBetween(playerPos, leftUnder, rightUpper))
		{
			// 難易度選択ステートに変更
			m_stageState = ModeSelect;
		}
	}

	// フェードアウトステート
	void TitleStage::FadeOutState()
	{
		// シーンオブジェクトの取得
		const auto& scene = App::GetApp()->GetScene<Scene>();
		if (m_fade.lock()->FadeInColor(1.0f))
		{
			// シーンからゲームステージに遷移
			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	// 生成時の処理
	void TitleStage::OnCreate()
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
			CreateBGM(L"TITLE_BGM", 0.25f);

			// プレイヤーの作成
			CreatePlayer();

			// スプライトの作成
			CreateSprites();

			// ステージの生成
			CreateStage("Title");
			CreateInstanceBlock("Title");

		}
		catch (...)
		{
			throw;
		}
	}

	// 毎フレーム更新処理
	void TitleStage::OnUpdate()
	{
		try
		{
			// ステートに応じて処理
			switch (m_stageState)
			{
			case TitleStage::FadeIn: // フェードインなら
				// フェードを掛けてフェードが終わったらプッシュボタンステートに変更
				if (m_fade.lock()->FadeOutColor(2.0f)) m_stageState = PushButton;
				break;

			case TitleStage::PushButton: // プッシュボタンなら
				PushButtonState(); // プッシュボタンステート処理を実行
				break;

			case TitleStage::MetalFadeOut: // メタルウィンドウフェードなら
				MetalFadeState(); // メタルウィンドウステート処理を実行
				break;

			case TitleStage::StartMove: // 開始時の移動演出なら
				StartMoveState(); // 開始時の移動演出処理を実行
				break;

			case TitleStage::FadeOut: // フェードアウトなら
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