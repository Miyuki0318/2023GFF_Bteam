/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

using namespace Utility;
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
				SimpleVerticesIndices(vertex);
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

	// �C���X�^���X�`��I�u�W�F�N�g�̐���
	void GameStage::CreateInstanceBlock()
	{
		// �e�N�X�`���^�C�v
		enum eTypes
		{
			Iron,
			Metal,
			Dark,
			Size, // enum�̃T�C�Y�w��q
		};

		// �C���X�^���X�p�\����
		struct Instance
		{
			vector<int> num; // �f�[�^�z��
			int count = 0;   // �J�E���^
		};

		// CSV�̓ǂݍ���(�X�g�����O�^��2�����z��)
		const auto& data = CSVLoader::LoadFile("Stage");

		// CSV�̍s�̐�
		const int rowSize = static_cast<int>(data.size());

		// �s�̐��Ń��[�v
		for (int i = 0; i < rowSize; i++)
		{
			// �u���b�N�ƃX���[�v�̃C���X�^���X�\����
			vector<Instance> block;
			vector<Instance> slope;

			// �^�C�v�̐��Ń��T�C�Y
			block.resize(eTypes::Size);
			slope.resize(eTypes::Size);

			// �^�C�v�̐����[�v
			for (int j = 0; j < eTypes::Size; j++)
			{
				// i�s�ڂ̗�̐��Ń��T�C�Y
				block.at(j).num.resize(data.at(i).size());
				slope.at(j).num.resize(data.at(i).size());
			}

			// ��̐��Ń��[�v
			for (int j = 0; j < data.at(i).size(); j++)
			{
				// �󔒂��ǂݍ��܂ꂽ���p�̃G���[�`�F�b�N
				if (data.at(i).at(j) == "") continue;

				// ������𐮐��ɕϊ�
				const int& num = stoi(data.at(i).at(j));

				// �C���X�^���X�`�悵�Ă��鐔�l�͈̔͂Ŏw��(ID�Q��)
				if (GetBetween(num, 100, 125))
				{
					// 10�̈ʂ����߂�
					// ��F������̒��g��121�������ꍇ.at(1)��21���擾���A10�Ŋ��鎖�ŋ��߂��Ă���
					int tensPlace = atoi(&data.at(i).at(j).at(1)) / 10;

					// ���l��5�Ŋ���؂�鎞(��F100,105)
					if (num % 5 == 0)
					{
						// 10�̈ʂ��^�C�v�̓Y�����Ƃ��ăJ�E���^�𑝂₷
						block.at(tensPlace).count++;

						// �^�C�v�̐��Ń��[�v
						for (int k = 0; k < eTypes::Size; k++)
						{
							// 10�̈ʂ��^�C�v(���[�v��K)�ƈ�v������1�A�s��v�Ȃ�0
							block.at(k).num.at(j) = static_cast<int>(tensPlace == k);
						}
					}
					else // ����؂�Ȃ���(��F101,102,103,104)
					{
						// 10�̈ʂ��^�C�v�̓Y�����Ƃ��ăJ�E���^�𑝂₷
						slope.at(tensPlace).count++;

						// �^�C�v�̐��Ń��[�v
						for (int k = 0; k < eTypes::Size; k++)
						{
							// 10�̈ʂ��^�C�v(���[�v��K)�ƈ�v�����琔�l��1�̈ʂ̒l���A�s��v�Ȃ�0
							slope.at(k).num.at(j) = tensPlace == k ? atoi(&data.at(i).at(j).at(2)) : 0;
						}
					}
				}
			}

			// �^�C�v�̐��Ń��[�v
			for (int k = 0; k < eTypes::Size; k++)
			{
				// �J�E���^��0���傫�������琶��
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

				const int& num = stoi(data.at(i).at(j));

				switch (num)
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

				case 230:
					gimmick = AddGameObject<Ring>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 5.0f);
					break;

				case 300:
					block = AddGameObject<Bird>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale);
					break;

				default:
					break;
				}

				// ����ȃu���b�N��M�~�b�N�p

				// ��
				if (GetBetween(num, 200, 204))
				{
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&data.at(i).at(j).at(2)));
					block = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, angle);
				}
				
				// �x���g�R���x�A
				if (GetBetween(num, 210, 211))
				{
					checker.count++;
					checker.type = data.at(i).at(j);
					checker.check = data.at(i).at(j + 1) != checker.type || data.at(i).at(j - 1) != checker.type;
					const auto& rotate = static_cast<Convayor::eRotate>(atoi(&data.at(i).at(j).at(2)));
					const auto& beltType = static_cast<Convayor::eType>(checker.check);
					gimmick = AddGameObject<Convayor>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale, rotate, beltType);
				}

				// ��C
				if (GetBetween(num, 2200, 2283))
				{
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&data.at(i).at(j).at(2)) / 10);
					const auto& fireType = static_cast<Cannon::eFireType> (atoi(&data.at(i).at(j).at(3)));
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), scale * 3.0f, angle, fireType);
				}

				// �����@
				if (GetBetween(num, 240, 243))
				{
					const Vec3 blowerScale = Vec3(scale * 5.0f, scale, scale * 5.0f);
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&data.at(i).at(j).at(2)));
					update = AddGameObject<Blower>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), blowerScale, angle, 15.0f);
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((data.size() - i) * scale)), update->GetRotation(), blowerScale, true);
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