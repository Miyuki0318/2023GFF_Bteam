/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

	void GameStage::CreateResourses()
	{
		// �A�v���P�[�V�����̎擾
		const auto& app = App::GetApp();

		// ���f�B�A�f�B���N�g���p�X�̎擾
		const wstring mediaPath = app->GetDataDirWString();

		// �e�N�X�`���f�B���N�g���p�X�̎擾
		const wstring texturePath = mediaPath + L"Textures/";

		// �T�E���h�f�B���N�g���p�X�̎擾
		const wstring soundPath = mediaPath + L"Sounds/";

		// ���̃e�N�X�`���̓ǂݍ���
		app->RegisterTexture(L"EFFECT", texturePath + L"Effect.png");
	}

	void GameStage::CreateViewLight() 
	{
		// �r���[�̃J�����̐ݒ�
		auto ptrCamera = ObjectFactory::Create<GameCamera>();
		m_gameView = CreateView<SingleView>();
		m_gameView->SetCamera(ptrCamera);

		// �}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		ptrMultiLight->SetDefaultLighting();
	}

	void GameStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>();
		SetSharedGameObject(L"Player", player);
		
		const auto& gameCamera = dynamic_pointer_cast<GameCamera>(m_gameView->GetCamera());
		gameCamera->SetTargetObject(player);
	}

	void GameStage::CreateStage()
	{
		const auto& data = CSVLoader::LoadFile("Test");

		const float up = 22.5f;
		const float left = -22.0f;
		const float scale = 1.0f;

		for (size_t i = 0; i < data.size(); i++)
		{
			for (size_t j = 0; j < data.at(i).size(); j++)
			{
				shared_ptr<CubeObject> ptr;
				if (data.at(i).at(j) == "-1")
				{
					ptr = AddGameObject<Alpha>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "1")
				{
					ptr = AddGameObject<Grass>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "2")
				{
					ptr = AddGameObject<Dirt>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "3")
				{
					ptr = AddGameObject<Rock>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "4")
				{
					ptr = AddGameObject<SandStone>(Vec2(left + (j * scale), up - (i * scale)), scale);
				}
				if (data.at(i).at(j) == "20")
				{
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), up - (i * scale)), scale, Gimmick::Up);
				}
				if (data.at(i).at(j) == "21")
				{
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), up - (i * scale)), scale, Gimmick::Down);
				}
				if (data.at(i).at(j) == "22")
				{
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), up - (i * scale)), scale, Gimmick::Left);
				}
				if (data.at(i).at(j) == "23")
				{
					ptr = AddGameObject<Spike>(Vec2(left + (j * scale), up - (i * scale)), scale, Gimmick::Right);
				}

				if (ptr)
				{
					ptr->SetTarget(GetSharedGameObject<Player>(L"Player"));
				}
			}
		}
	}

	void GameStage::OnCreate() 
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

			// �n�ʂ̍쐬
			CreatePlayer();

			// �X�e�[�W
			CreateStage();
		}
		catch (...) 
		{
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		const auto& fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		Debug::Log(L"FPS : ", fps);
	}

	void GameStage::OnDraw()
	{
		// �f�o�b�O������������I�ɋ�ɂ���
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// �p�����̕`�掞�̊֐������s����
		Stage::OnDraw();

		// �f�o�b�N�p������̕\����\���؂�ւ�
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(true);
	}
}