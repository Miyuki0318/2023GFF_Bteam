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

		// �e�N�X�`���̎擾
		// �T�E���h�f�B���N�g���p�X
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// �T�E���h�̎擾
		// �l�N�X�g���BGM�̓ǂݍ���
		app->RegisterWav(L"TITLE_BGM", BGMPath + L"TitleBGM");
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