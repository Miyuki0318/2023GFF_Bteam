#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TitleStage::CreateResourses()
	{
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
		Utility::SimpleVerticesIndices(vertex);
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

	void TitleStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>();
		SetSharedGameObject(L"Player", player);
	}

	void TitleStage::CreateInstanceBlock()
	{
		struct Instance
		{
			vector<int> num;
			int count = 0;
		};

		const auto& data = CSVLoader::LoadFile("Title");
		const int size = static_cast<int>(data.size());

		for (int i = 0; i < data.size(); i++)
		{
			Instance iron;
			Instance metal;
			Instance darkMetal;

			iron.num.resize(data.at(i).size());
			metal.num.resize(data.at(i).size());
			darkMetal.num.resize(data.at(i).size());

			for (int j = 0; j < data.at(i).size(); j++)
			{
				if (data.at(i).at(j) == "") continue;

				switch (stoi(data.at(i).at(j)))
				{
				case 100:
				case 105:
					iron.count++;
					iron.num.at(j) = 1;
					metal.num.at(j) = 0;
					darkMetal.num.at(j) = 0;
					break;

				case 110:
				case 115:
					metal.count++;
					iron.num.at(j) = 0;
					metal.num.at(j) = 1;
					darkMetal.num.at(j) = 0;
					break;

				case 120:
				case 125:
					darkMetal.count++;
					iron.num.at(j) = 0;
					metal.num.at(j) = 0;
					darkMetal.num.at(j) = 1;
					break;

				default:
					iron.num.at(j) = 0;
					metal.num.at(j) = 0;
					darkMetal.num.at(j) = 0;
					break;
				}
			}

			if (iron.count > 0)
			{
				AddGameObject<InstanceBlock>(iron.num, L"GRASS_TX", size, i);
			}
			if (metal.count > 0)
			{
				AddGameObject<InstanceBlock>(metal.num, L"DIRT_TX", size, i);
			}
			if (darkMetal.count > 0)
			{
				AddGameObject<InstanceBlock>(darkMetal.num, L"ROCK_TX", size, i);
			}
		}
	}

	void TitleStage::CreateStage()
	{
		CreateSharedObjectGroup(L"Stage");
		CreateSharedObjectGroup(L"Gimmick");

		const auto& data = CSVLoader::LoadFile("Title");

		struct Checker
		{
			string type = "";
			int count = 0;
			bool check = false;

			void reset()
			{
				type = "";
				count = 0;
				check = false;
			}
		};

		const float under = -7.5f;
		const float left = -49.0f;
		const float scale = 1.0f;
		const Vec3 slopeScale = Vec3(scale) * 1.4f;
		const Vec2 slopeLeft = Vec2(0.5f, -0.5f);
		const Vec2 slopeRight = Vec2(-0.5f, -0.5f);

		Checker checker;
		for (size_t i = 0; i < data.size(); i++)
		{
			for (size_t j = 0; j < data.at(i).size(); j++)
			{
				if (data.at(i).at(j) == "") continue;

				shared_ptr<CubeObject> block = nullptr;
				shared_ptr<Gimmick> gimmick = nullptr;

				switch (stoi(data.at(i).at(j)))
				{
				case 0:
					checker.reset();
					break;

				case 1:
					block = AddGameObject<DeathColl>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, true);
					break;

				case 2:
				case 100:
				case 110:
				case 120:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, true);
					break;

				case 200:
					block = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::All);
					break;

				case 201:
					block = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Up);
					break;

				case 202:
					block = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Down);
					break;

				case 203:
					block = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Left);
					break;

				case 204:
					block = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Gimmick::Right);
					break;

				case 210:
					checker.count++;
					checker.type = data.at(i).at(j);
					checker.check = data.at(i).at(j + 1) != checker.type || data.at(i).at(j - 1) != checker.type;
					gimmick = AddGameObject<Convayor>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Convayor::LeftRot, static_cast<Convayor::eType>(checker.check));
					break;

				case 211:
					checker.count++;
					checker.type = data.at(i).at(j);
					checker.check = data.at(i).at(j + 1) != checker.type || data.at(i).at(j - 1) != checker.type;
					gimmick = AddGameObject<Convayor>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, Convayor::RightRot, static_cast<Convayor::eType>(checker.check));
					break;

				case 220:
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Up, Cannon::Normal);
					break;

				case 221:
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Down, Cannon::Normal);
					break;

				case 222:
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Left, Cannon::Normal);
					break;

				case 223:
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Uright, Cannon::Normal);
					break;

				case 224:
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Uleft, Cannon::Normal);
					break;

				case 225:
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Dleft, Cannon::Normal);
					break;

				case 226:
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Uright, Cannon::Normal);
					break;

				case 227:
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Dright, Cannon::Normal);
					break;

				case 230:
					gimmick = AddGameObject<Ring>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 5.0f);
					break;

				default:
					break;
				}

				if (block)
				{
					block->SetTarget(GetSharedGameObject<Player>(L"Player"));
					GetSharedObjectGroup(L"Stage")->IntoGroup(block);
				}

				if (gimmick)
				{
					gimmick->SetTarget(GetSharedGameObject<Player>(L"Player"));
					GetSharedObjectGroup(L"Gimmick")->IntoGroup(gimmick);
				}
			}
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

			// BGM�̍Đ�
			CreateBGM();

			// �v���C���[�̍쐬
			CreatePlayer();

			// �X�e�[�W
			CreateStage();
			CreateInstanceBlock();
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
			const auto& fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
			Debug::Log(L"FPS : ", fps);
		}
		catch (...)
		{
			throw;
		}
	}

	void TitleStage::OnDraw()
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