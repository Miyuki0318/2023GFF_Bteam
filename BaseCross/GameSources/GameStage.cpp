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

		// �w�i�̐���
		for (size_t i = 0; i < 12; i++)
		{
			for (size_t j = 0; j < 18; j++)
			{
				auto ptrBack = AddGameObject<DebugObject>();
				float x, y;
				x = 80.0f;
				y = 45.0f;
				ptrBack->SetPosition(Vec3(-90.0f + (x * j), 135.0f - (y * i), 50.0f));
				ptrBack->SetScale(Vec3(x, y, 5.0f));
				ptrBack->SetAlphaActive(true);


				VertexData vertex;
				Utility::SimpleVerticesIndices(vertex);
				auto backDraw = ptrBack->AddComponent<PCTStaticDraw>();
				backDraw->SetOriginalMeshUse(true);
				backDraw->CreateOriginalMesh(vertex);
				backDraw->SetTextureResource(L"BACKGROUND_TX");
			}
		}
	}

	// BGM�̍Đ�
	void GameStage::CreateBGM()
	{
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		m_bgm = audioPtr->Start(L"GAME_BGM", XAUDIO2_LOOP_INFINITE, 0.3f);
	}

	void GameStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>();
		SetSharedGameObject(L"Player", player);
		
		const auto& gameCamera = dynamic_pointer_cast<GameCamera>(m_gameView->GetCamera());
		gameCamera->SetTargetObject(player);
	}

	void GameStage::CreateInstanceBlock()
	{
		enum eTypes
		{
			Iron,
			Metal,
			Dark,
			Size,
		};

		struct Instance
		{
			vector<int> num;
			int count = 0;
		};

		const auto& data = CSVLoader::LoadFile("Stage");
		const int rowSize = static_cast<int>(data.size());

		for (int i = 0; i < data.size(); i++)
		{
			vector<Instance> block;
			vector<Instance> slope;

			block.resize(eTypes::Size);
			slope.resize(eTypes::Size);

			for (int j = 0; j < eTypes::Size; j++)
			{
				block.at(j).num.resize(data.at(i).size());
				slope.at(j).num.resize(data.at(i).size());
			}

			for (int j = 0; j < data.at(i).size(); j++)
			{
				if (data.at(i).at(j) == "") continue;

				switch (stoi(data.at(i).at(j)))
				{
				case 100:
				case 105:
					block.at(Iron).count++;
					block.at(Iron).num.at(j) = 1;
					block.at(Metal).num.at(j) = 0;
					block.at(Dark).num.at(j) = 0;
					break;

				case 110:
				case 115:
					block.at(Metal).count++;
					block.at(Iron).num.at(j) = 0;
					block.at(Metal).num.at(j) = 1;
					block.at(Dark).num.at(j) = 0;
					break;

				case 120:
				case 125:
					block.at(Dark).count++;
					block.at(Iron).num.at(j) = 0;
					block.at(Metal).num.at(j) = 0;
					block.at(Dark).num.at(j) = 1;
					break;

				default:
					block.at(Iron).num.at(j) = 0;
					block.at(Metal).num.at(j) = 0;
					block.at(Dark).num.at(j) = 0;
					break;
				}

				switch (stoi(data.at(i).at(j)))
				{
				case 101:
				case 102:
				case 103:
				case 104:
					slope.at(Iron).count++;
					slope.at(Iron).num.at(j) = atoi(&data.at(i).at(j).at(2));
					slope.at(Metal).num.at(j) = 0;
					slope.at(Dark).num.at(j) = 0;
					break;

				case 111:
				case 112:
				case 113:
				case 114:
					slope.at(Metal).count++;
					slope.at(Iron).num.at(j) = 0;
					slope.at(Metal).num.at(j) = atoi(&data.at(i).at(j).at(2));
					slope.at(Dark).num.at(j) = 0;
					break;

				case 121:
				case 122:
				case 123:
				case 124:
					slope.at(Dark).count++;
					slope.at(Iron).num.at(j) = 0;
					slope.at(Metal).num.at(j) = 0;
					slope.at(Dark).num.at(j) = atoi(&data.at(i).at(j).at(2));
					break;

				default:
					slope.at(Iron).num.at(j) = 0;
					slope.at(Metal).num.at(j) = 0;
					slope.at(Dark).num.at(j) = 0;
					break;
				}

			}

			for (int k = 0; k < eTypes::Size; k++)
			{
				if (block.at(k).count > 0)
				{
					AddGameObject<InstanceBlock>(block.at(k).num, k, rowSize, i);
				}
				if (slope.at(k).count > 0)
				{
					AddGameObject<InstanceSlope>(slope.at(k).num, k, rowSize, i);
				}
			}
		}
	}

	void GameStage::CreateStage()
	{
		CreateSharedObjectGroup(L"Stage");
		CreateSharedObjectGroup(L"Gimmick");
		CreateSharedObjectGroup(L"Update");

		const auto& data = CSVLoader::LoadFile("Stage");

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

		const float under = -97.5f;
		const float left = -49.0f;
		const float scale = 1.0f;
		const Vec3 slopeScale = Vec3(scale) * 1.4f;
		const Vec2 slopeULeft = Vec2(0.5f, -0.5f);
		const Vec2 slopeURight = Vec2(-0.5f, -0.5f);
		const Vec2 slopeDLeft = Vec2(0.5f, 0.5f);
		const Vec2 slopeDRight = Vec2(-0.5f, 0.5f);

		Checker checker;
		for (size_t i = 0; i < data.size(); i++)
		{
			for (size_t j = 0; j < data.at(i).size(); j++)
			{
				if (data.at(i).at(j) == "") continue;

				shared_ptr<CubeObject> block = nullptr;
				shared_ptr<Gimmick> gimmick = nullptr;
				shared_ptr<Gimmick> update = nullptr;

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

				case 101:
				case 111:
				case 121:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeULeft, slopeScale, true);
					break;

				case 102:
				case 112:
				case 122:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeURight, slopeScale, true);
					break;

				case 103:
				case 113:
				case 123:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeDLeft, slopeScale, true);
					break;

				case 104:
				case 114:
				case 124:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)) + slopeDRight, slopeScale, true);
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
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, Gimmick::Right, Cannon::Normal);
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

				case 240:
					update = AddGameObject<Blower>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale * 5.0f, scale, scale * 5.0f), Gimmick::Up, 30.0f);
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, true);
					break;

				case 241:
					update = AddGameObject<Blower>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale * 5.0f, scale, scale * 5.0f), Gimmick::Down, 30.0f);
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale * 5.0f, scale, scale * 5.0f), true);
					break;

				case 242:
					update = AddGameObject<Blower>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale * 5.0f, scale, scale * 5.0f), Gimmick::Left, 30.0f);
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale * 5.0f, scale, scale * 5.0f), true);
					break;

				case 243:
					update = AddGameObject<Blower>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale * 5.0f, scale, scale * 5.0f), Gimmick::Right, 30.0f);
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), Vec3(scale * 5.0f, scale, scale * 5.0f), true);
					break;

				case 300:
					block = AddGameObject<Bird>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale);
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

				if (update)
				{
					update->SetTarget(GetSharedGameObject<Player>(L"Player"));
					GetSharedObjectGroup(L"Update")->IntoGroup(update);
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

	void GameStage::OnUpdate()
	{
		try
		{
			const auto& fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
			Debug::Log(L"FPS : ", fps);

			const auto& player = GetSharedGameObject<Player>(L"Player");
			Vec3 pos = player->GetComponent<Transform>()->GetPosition();

			const auto& cubeVec = GetSharedObjectGroup(L"Stage")->GetGroupVector();
			for (const auto& weakObj : cubeVec)
			{
				const auto& cubeObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());

				if (!cubeObj) continue;

				float length = (cubeObj->GetPosition() - pos).length();
				cubeObj->SetUpdateActive(length <= 10.0f);
				cubeObj->SetDrawActive(length <= 55.0f);
			}

			const auto& gimmickVec = GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
			for (const auto& weakObj : gimmickVec)
			{
				const auto& gimmickObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());

				if (!gimmickObj) continue;

				float length = (gimmickObj->GetPosition() - pos).length();
				gimmickObj->SetUpdateActive(length <= 55.0f);
				gimmickObj->SetDrawActive(length <= 55.0f);
			}
		}
		catch (...)
		{
			throw;
		}
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