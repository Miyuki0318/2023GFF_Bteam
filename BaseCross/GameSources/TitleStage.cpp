#include "stdafx.h"
#include "Project.h"

using namespace Utility;
namespace basecross
{
	void TitleStage::CreateResourses()
	{
		// �p�����̊֐�
		BaseStage::CreateResourses();

		// �A�v���P�[�V�����̎擾
		const auto& app = App::GetApp();

		// ���f�B�A�f�B���N�g���p�X�̎擾
		const wstring mediaPath = app->GetDataDirWString();

		// �e�N�X�`���f�B���N�g���p�X�̎擾
		const wstring texturePath = mediaPath + L"Textures/";

		// �e�N�X�`���̎擾
		// �^�C�g�����S�̓ǂݍ���
		app->RegisterTexture(L"TITLE_LOGO", texturePath + L"TitleLogo.tga");

		// �X�^�[�g�{�^���̓ǂݍ���
		app->RegisterTexture(L"PUSH_BUTTON", texturePath + L"PushButton.png");

		//// �C�[�W�[�{�^���̓ǂݍ���
		//app->RegisterTexture(L"EASY_BUTTON", texturePath + L"Easy.png");

		//// �m�[�}���{�^���̓ǂݍ���
		//app->RegisterTexture(L"NORMAL_BUTTON", texturePath + L"Normal.png");

		//// �n�[�h�{�^���̓ǂݍ���
		//app->RegisterTexture(L"HARD_BUTTON", texturePath + L"Hard.png");


		// �T�E���h�f�B���N�g���p�X
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// �T�E���h�̎擾
		// �^�C�g�����BGM�̓ǂݍ���
		app->RegisterWav(L"TITLE_BGM", BGMPath + L"TitleBGM");
	}

	void TitleStage::CreateViewLight()
	{
		// �r���[�̃J�����̐ݒ�
		auto ptrCamera = ObjectFactory::Create<Camera>();
		ptrCamera->SetEye(Vec3(-20.0f, -75.0f, -40.0f));
		ptrCamera->SetAt(Vec3(-20.0f, -75.0f, 0.0f));
		m_gameView = CreateView<SingleView>();
		m_gameView->SetCamera(ptrCamera);

		// �}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		ptrMultiLight->SetDefaultLighting();
	}

	// �w�i�̐���
	void TitleStage::CreateBackGround()
	{
		// �I�u�W�F�N�g�̐����Ɣz�u
		auto ptrBack = AddGameObject<DebugObject>();
		float loop, x, y;
		loop = 5.0f;
		x = 80.0f * loop;
		y = 45.0f * loop;
		ptrBack->SetPosition(Vec3(-90.0f, 0.0f, 50.0f));
		ptrBack->SetScale(Vec3(x, y, 5.0f));
		ptrBack->SetAlphaActive(true);

		// ���b�V���̐����Ɛݒ�
		VertexData vertex;
		SimpleVerticesIndices(vertex);
		vertex.vertices.at(1).textureCoordinate = Vec2(loop, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, loop);
		vertex.vertices.at(3).textureCoordinate = Vec2(5.0f, loop);
		auto backDraw = ptrBack->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	void TitleStage::CreatePlayer()
	{
		m_player = AddGameObject<StagingPlayer>(Vec3(-50.0f, -45.0f, 0.0f));
		SetSharedGameObject(L"Player", m_player.lock());
	}

	void TitleStage::CreateSprites()
	{
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));
		m_titleLogo = AddGameObject<Sprite>(L"TITLE_LOGO", WINDOW_SIZE, Vec3(0.0f, 50.0f, 0.1f));
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE, Vec3(0.0f, 0.0f, 0.2f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE, Vec3(0.0f, 0.0f, 0.2f));
		m_pushButton = AddGameObject<Sprite>(L"PUSH_BUTTON", WINDOW_SIZE / 2.0f, Vec3(0.0f, -200.0f, 0.2f));
	}

	void TitleStage::PushButtonState()
	{
		if (Input::GetPushA())
		{
			m_stageState = BackFadeOut;
			CreateSE(L"METAL_SE", 0.75f);
		}
	}

	void TitleStage::BackFadeState()
	{
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& logoPos = m_titleLogo.lock()->GetPosition();
		const Vec3& pushPos = m_pushButton.lock()->GetPosition();
		if (mLPos.x > -WINDOW_WIDTH / 1.9f)
		{
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(-DELTA_TIME * 250.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(DELTA_TIME * 250.0f, 0.0f, 0.0f));
			m_titleLogo.lock()->SetPosition(logoPos + Vec3(0.0f, DELTA_TIME * 200.0f, 0.0f));
			m_pushButton.lock()->SetPosition(pushPos + Vec3(0.0f, -DELTA_TIME * 100.0f, 0.0f));
		}
		else
		{
			StopSE(L"METAL_SE");
			CreateSE(L"METAL_STOP_SE", 1.5f);
			m_stageState = StartMove;
		}
	}

	void TitleStage::StartMoveState()
	{
		const Vec3& playerPos = m_player.lock()->GetPosition();
		const Vec3 leftUnder = Vec3(-22.0f, -89.0f, 0.0f);
		const Vec3 rightUpper = Vec3(-20.0f, -88.0f, 0.0f);
		if (GetBetween(playerPos, leftUnder, rightUpper))
		{
			m_stageState = ModeSelect;
		}
	}

	void TitleStage::FadeOutState()
	{
		const auto& scene = App::GetApp()->GetScene<Scene>();
		if (m_fade.lock()->FadeInColor(2.0f))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	void TitleStage::OnCreate()
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
			CreateBGM(L"TITLE_BGM", 0.25f);

			// �v���C���[�̍쐬
			CreatePlayer();

			// �X�v���C�g�̍쐬
			CreateSprites();

			// �X�e�[�W
			CreateStage("Title");
			CreateInstanceBlock("Title");

		}
		catch (...)
		{
			throw;
		}
	}

	void TitleStage::OnUpdate()
	{
		try
		{
			switch (m_stageState)
			{
			case TitleStage::FadeIn:
				Debug::Log(L"�t�F�[�h�C���X�e�[�g");
				if (m_fade.lock()->FadeOutColor(2.0f)) m_stageState = PushButton;
				break;

			case TitleStage::PushButton:
				Debug::Log(L"�v�b�V���{�^���X�e�[�g");
				PushButtonState();
				break;

			case TitleStage::BackFadeOut:
				Debug::Log(L"�o�b�N�t�F�[�h�A�E�g�X�e�[�g");
				BackFadeState();
				break;

			case TitleStage::StartMove:
				Debug::Log(L"�X�^�[�g���[�u�X�e�[�g");
				StartMoveState();
				break;

			case TitleStage::ModeSelect:
				Debug::Log(L"���[�h�Z���N�g�X�e�[�g");
				break;

			case TitleStage::CannonStanby:
				Debug::Log(L"�L���m���X�^���o�C�X�e�[�g");
				break;

			case TitleStage::FadeOut:
				Debug::Log(L"�t�F�[�h�A�E�g�X�e�[�g");
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