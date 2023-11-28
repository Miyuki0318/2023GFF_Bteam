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

		// �T�E���h�f�B���N�g���p�X�̎擾
		const wstring soundPath = mediaPath + L"Sounds/";

		app->RegisterWav(L"TITLE_BGM", soundPath + L"BGM/TitleBGM");
	}

	void TitleStage::CreateViewLight()
	{
		// �r���[�̃J�����̐ݒ�
		auto ptrCamera = ObjectFactory::Create<Camera>();
		ptrCamera->SetEye(Vec3(-20.0f, 9.0f, -33.0f));
		ptrCamera->SetAt(Vec3(-20.0f, 9.0f, 0.0f));
		m_gameView = CreateView<SingleView>();
		m_gameView->SetCamera(ptrCamera);

		// �}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		ptrMultiLight->SetDefaultLighting();

		// �w�i�̐���
		auto ptrBack = AddGameObject<DebugObject>();
		float x, y;
		x = 80.0f * 5;
		y = 45.0f * 5;
		ptrBack->SetPosition(Vec3(-90.0f, 0.0f, 50.0f));
		ptrBack->SetScale(Vec3(x, y, 5.0f));
		ptrBack->SetAlphaActive(true);

		VertexData vertex;
		SimpleVerticesIndices(vertex);
		vertex.vertices.at(1).textureCoordinate = Vec2(5.0f, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, 5.0f);
		vertex.vertices.at(3).textureCoordinate = Vec2(5.0f, 5.0f);
		auto backDraw = ptrBack->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	void TitleStage::CreateBGM()
	{
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		m_bgm = audioPtr->Start(L"TITLE_BGM", XAUDIO2_LOOP_INFINITE, 0.5f);
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

			// BGM�̍Đ�
			CreateBGM();

			// �v���C���[�̍쐬
			CreatePlayer();

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
			BaseStage::OnUpdate();
		}
		catch (...)
		{
			throw;
		}
	}
}