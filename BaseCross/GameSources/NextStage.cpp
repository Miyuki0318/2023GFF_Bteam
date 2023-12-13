#include "stdafx.h"
#include "Project.h"

using namespace Utility;
namespace basecross
{
	void NextStage::CreateResourses()
	{
		// �p�����̊֐�
		BaseStage::CreateResourses();

		// �A�v���P�[�V�����̎擾
		const auto& app = App::GetApp();

		// ���f�B�A�f�B���N�g���p�X�̎擾
		const wstring mediaPath = app->GetDataDirWString();

		// �e�N�X�`���f�B���N�g���p�X�̎擾
		const wstring texturePath = mediaPath + L"Textures/";

		// ���ւ̑I�����̓ǂݍ���
		app->RegisterTexture(L"GO_TX", texturePath + L"Go.png");
		app->RegisterTexture(L"NEXT_TX", texturePath + L"NextStage.png");


		// �e�N�X�`���̎擾
		// �T�E���h�f�B���N�g���p�X
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// �T�E���h�̎擾
		// �l�N�X�g���BGM�̓ǂݍ���
		app->RegisterWav(L"NEXT_BGM", BGMPath + L"NextBGM");
	}

	void NextStage::CreateViewLight()
	{
		// �r���[�̃J�����̐ݒ�
		auto ptrCamera = ObjectFactory::Create<Camera>();
		ptrCamera->SetEye(Vec3(0.0f, 0.0f, -33.0f));
		ptrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		m_gameView = CreateView<SingleView>();
		m_gameView->SetCamera(ptrCamera);

		// �}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		ptrMultiLight->SetDefaultLighting();
	}

	// �w�i�̐���
	void NextStage::CreateBackGround()
	{
		// �I�u�W�F�N�g�̐����Ɣz�u
		m_backObj = AddGameObject<TemplateObject>();
		float loop, x, y;
		loop = 2.0f;
		x = 80.0f * loop;
		y = 45.0f * loop;
		const auto& backObj = m_backObj.lock();
		backObj->SetPosition(Vec3(0.0f, 0.0f, 50.0f));
		backObj->SetScale(Vec3(x, y, 5.0f));
		backObj->SetAlphaActive(true);

		// ���b�V���̐����Ɛݒ�
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
		m_logo = AddGameObject<Sprite>(L"NEXT_TX", WINDOW_SIZE, Vec3(0.0f, 200.0f, 0.2f));
		m_next = AddGameObject<Sprite>(L"GO_TX", WINDOW_SIZE * 0.55f, Vec3(-300.0f, 600.0f, 0.2f));
		m_back = AddGameObject<Sprite>(L"QUIT_TX", WINDOW_SIZE * 0.55f, Vec3(300.0f, 600.0f, 0.2f));
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
		const Vec2 deffScale = WINDOW_SIZE * 0.55f;
		const bool inputLStick = Input::IsInputLStickX();
		m_totalTime += DELTA_TIME * 5.0f;

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
		}
		m_currentStickX = inputLStick;
		m_next.lock()->SetScale(deffScale * (m_select == Next ? SinCurve(m_totalTime, 1.0f, 1.2f) : 1.0f));
		m_back.lock()->SetScale(deffScale * (m_select == Back ? SinCurve(m_totalTime, 1.0f, 1.2f) : 1.0f));
	}

	void NextStage::BackFadeState()
	{
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& nxPos = m_next.lock()->GetPosition();
		const Vec3& tbPos = m_back.lock()->GetPosition();
		if (mLPos.x < 0.0f)
		{
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(DELTA_TIME * 750.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(-DELTA_TIME * 750.0f, 0.0f, 0.0f));
			m_next.lock()->SetPosition(nxPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
			m_back.lock()->SetPosition(tbPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
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

	void NextStage::FadeInState()
	{
		const auto& fade = m_fade.lock();
		const Vec3& nextPos = m_next.lock()->GetPosition();
		const Vec3& backPos = m_back.lock()->GetPosition();

		if (fade->FadeOutColor(2.0f))
		{
			m_stageState = Select;
		}
		else
		{
			float y = Utility::Lerp(600.0f, -100.0f, 1.0f - fade->GetDiffuseColor().w);
			m_next.lock()->SetPosition(nextPos.x, y, nextPos.z);
			m_back.lock()->SetPosition(backPos.x, y, backPos.z);
		}
	}

	void NextStage::FadeOutState()
	{
		const auto& fade = m_fade.lock();
		const auto& scene = App::GetApp()->GetScene<Scene>();
		const Vec3& nextPos = m_next.lock()->GetPosition();
		const Vec3& backPos = m_back.lock()->GetPosition();

		if (fade->FadeInColor(1.0f))
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
		else
		{
			float y = Utility::Lerp(-100.0f, 600.0f, fade->GetDiffuseColor().w);
			m_next.lock()->SetPosition(nextPos.x, y, nextPos.z);
			m_back.lock()->SetPosition(backPos.x, y, backPos.z);
		}
	}

	void NextStage::OnCreate()
	{
		try
		{
			//�e�p�t�H�[�}���X�𓾂�
			SetCollisionPerformanceActive(true);
			SetUpdatePerformanceActive(true);
			SetDrawPerformanceActive(true);

			// ���\�[�X�̓ǂݍ���
			CreateResourses();

			// �r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			// �w�i�̍쐬
			CreateBackGround();

			// BGM�̍Đ�
			CreateBGM(L"NEXT_BGM", 0.25f);

			// �v���C���[�̍쐬
			CreatePlayer();

			// �X�v���C�g�̍쐬
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
				FadeInState();
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