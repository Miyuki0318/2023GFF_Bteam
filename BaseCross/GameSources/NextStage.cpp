#include "stdafx.h"
#include "Project.h"

using namespace Utility;
namespace basecross
{
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

		// テクスチャの取得
		// サウンドディレクトリパス
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// サウンドの取得
		// ネクスト画面BGMの読み込み
		app->RegisterWav(L"TITLE_BGM", BGMPath + L"TitleBGM");
	}

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
		// オブジェクトの生成と配置
		m_backObj = AddGameObject<TemplateObject>();
		float loop, x, y;
		loop = 2.0f;
		x = 80.0f * loop;
		y = 45.0f * loop;
		const auto& backObj = m_backObj.lock();
		backObj->SetPosition(Vec3(0.0f, 0.0f, 50.0f));
		backObj->SetScale(Vec3(x, y, 5.0f));
		backObj->SetAlphaActive(true);

		// メッシュの生成と設定
		VertexData vertex;
		SimpleVerticesIndices(vertex);
		vertex.vertices.at(1).textureCoordinate = Vec2(loop, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, loop);
		vertex.vertices.at(3).textureCoordinate = Vec2(loop, loop);
		auto backDraw = backObj->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	void NextStage::CreatePlayer()
	{
		AddGameObject<NextPlayer>(Vec3(0.0f));
	}

	void NextStage::CreateSprites()
	{
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE, Vec3(-675.0f, 0.0f, 0.2f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE, Vec3(675.0f, 0.0f, 0.2f));
	}

	void NextStage::SlideBackGround()
	{
		const auto& backObj = m_backObj.lock();
		auto backDraw = backObj->GetComponent<PCTStaticDraw>();
		auto& vertices = backDraw->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
		for (auto& v : vertices) v.textureCoordinate += Vec2(1.0f, 0.0f) * DELTA_TIME;
		backDraw->UpdateVertices(vertices);
	}

	void NextStage::SelectState()
	{
		const bool inputLStick = Input::IsInputLStickX();

		if (inputLStick && !m_currentStickX)
		{
			m_totalTime = 0.0f;
			switch (m_select)
			{
			case NextStage::Next:
				m_select = Back;
				break;

			case NextStage::Back:
				m_select = Next;
				break;

			default:
				break;
			}

			m_currentStickX = inputLStick;
		}
	}

	void NextStage::BackFadeState()
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

	void NextStage::FadeOutState()
	{
		const auto& scene = App::GetApp()->GetScene<Scene>();
		if (m_fade.lock()->FadeInColor(1.0f))
		{
			switch (m_select)
			{
			case NextStage::Next:
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
				break;

			case NextStage::Back:
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"TitleStage");
				break;

			default:
				break;
			}
		}
	}

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
			CreateBGM(L"TITLE_BGM", 0.25f);

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

	void NextStage::OnUpdate()
	{
		try
		{
			SlideBackGround();

			switch (m_stageState)
			{
			case NextStage::FadeIn:
				if (m_fade.lock()->FadeOutColor(2.0f)) m_stageState = Select;
				break;

			case NextStage::Select:
				SelectState();
				break;

			case NextStage::BackFade:
				BackFadeState();
				break;

			case NextStage::FadeOut:
				FadeOutState();
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