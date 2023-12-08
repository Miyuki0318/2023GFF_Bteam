#include "stdafx.h"
#include "Project.h"

using namespace Utility;
namespace basecross
{
	void BaseStage::CreateResourses()
	{
		// �A�v���P�[�V�����̎擾
		const auto& app = App::GetApp();

		// ���f�B�A�f�B���N�g���p�X�̎擾
		const wstring mediaPath = app->GetDataDirWString();

		// �e�N�X�`���f�B���N�g���p�X�̎擾
		const wstring texturePath = mediaPath + L"Textures/";

		// ���h��e�N�X�`��
		app->RegisterTexture(L"WHITE_TX", texturePath + L"White.png");

		// ���^���̔w�i�̓ǂݍ���
		app->RegisterTexture(L"METAL_LEFT", texturePath + L"MetalLeft.png");
		app->RegisterTexture(L"METAL_RIGHT", texturePath + L"MetalRight.png");

		// ���̃e�N�X�`���̓ǂݍ���
		app->RegisterTexture(L"EFFECT_TX", texturePath + L"Effect.png");

		// �V�[���h�e�N�X�`���̓ǂݍ���
		app->RegisterTexture(L"SHIELD_TX", texturePath + L"Shield.png");

		// ���e�N�X�`��
		app->RegisterTexture(L"SMOKE_TX", texturePath + L"Smoke.png");

		// ���e�N�X�`��
		app->RegisterTexture(L"FIRE_TX", texturePath + L"Fire.png");

		// �΃e�N�X�`��
		app->RegisterTexture(L"STONE_TX", texturePath + L"Stone.png");

		// ���e�N�X�`��
		app->RegisterTexture(L"PAPER_TX", texturePath + L"Paper.png");

		// �w�i�e�N�X�`��
		app->RegisterTexture(L"BACKGROUND_TX", texturePath + L"BackGround.png");
		app->RegisterTexture(L"NBACKGROUND_TX", texturePath + L"NewBackGround.png");

		// ���e�N�X�`��
		app->RegisterTexture(L"ARROW_TX", texturePath + L"Arrow.png");

		// �T�E���h�f�B���N�g���p�X
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// ���^���V���b�^�[�̊J��SE
		app->RegisterWav(L"METAL_SE", SEPath + L"MetalDoorSE");
		app->RegisterWav(L"METAL_STOP_SE", SEPath + L"MetalStopSE");

		// �{�^����SE
		app->RegisterWav(L"PUSH_SE", SEPath + L"PushButtonSE");

		// �Ռ��gSE
		app->RegisterWav(L"AIRSHOCK_SE", SEPath + L"AirShockSE");

		// �o�E���h
		app->RegisterWav(L"ROBOT_BOUND_SE", SEPath + L"RobotBoundSE");

		// ��C����SE
		app->RegisterWav(L"CANNON_SE", SEPath + L"CannonFireSE");

		// �����OSE
		app->RegisterWav(L"RING_SE", SEPath + L"RingGetSE");

		// �V�[���hSE
		app->RegisterWav(L"SHIELD_C_SE", SEPath + L"ShieldCreateSE");
		app->RegisterWav(L"SHIELD_D_SE", SEPath + L"ShieldDestroySE");

		// �_���[�WSE
		app->RegisterWav(L"DAMAGE_SE", SEPath + L"DamageSE");

		// �E�T�M��SE
		app->RegisterWav(L"RABBIT_JUMP_SE", SEPath + L"RabbitJumpSE");
		app->RegisterWav(L"RABBIT_SEARCH_SE", SEPath + L"RabbitSearchSE");
	}

	void BaseStage::CreateViewLight() {}

	void BaseStage::CreateBGM(const wstring& bgmKey, float volume)
	{
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		m_bgm = audioPtr->Start(bgmKey, XAUDIO2_LOOP_INFINITE, volume);
	}

	void BaseStage::CreateSE(const wstring& seKey, float volume)
	{
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
		m_seList.push_back(SE(audioPtr->Start(seKey, 0, volume), seKey));
	}

	void BaseStage::StopSE(const wstring& seKey)
	{
		if (seKey != L"")
		{
			for (size_t i = 0; i < m_seList.size(); i++)
			{
				if (m_seList.at(i).seKey == seKey)
				{
					const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
					audioPtr->Stop(m_seList.at(i).item.lock());
					m_seList.at(i).Reset();
					break;
				}
			}
		}
	}

	void BaseStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>(Vec3(-20.0f, -80.0f, 0.0f));
		SetSharedGameObject(L"Player", player);
	}

	void BaseStage::CreateEnemy(const string& fileName)
	{
		m_csvData = CSVLoader::LoadFile(fileName);
		const float under = -97.5f;
		const float left = -49.0f;
		const float scale = 1.0f;

		vector<weak_ptr<Enemy>> enemyVec;

		for (size_t i = 0; i < m_csvData.size(); i++)
		{
			for (size_t j = 0; j < m_csvData.at(i).size(); j++)
			{
				if (m_csvData.at(i).at(j) == "") continue;

				const int& num = stoi(m_csvData.at(i).at(j));
				if (GetBetween(num, 300, 301))
				{
					switch (num)
					{
					case 300:
						enemyVec.push_back(AddGameObject<Rabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Rabbit::Normal));
						break;

					case 301:
						AddGameObject<Rabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Rabbit::Wall);
						break;

					default:
						break;
					}
				}
			}
		}

		const auto& enemyGroup = CreateSharedObjectGroup(L"Enemy");

		for (const auto& enemy : enemyVec)
		{
			if (enemy.lock())
			{
				enemyGroup->IntoGroup(enemy.lock());
			}
		}
	}

	void BaseStage::CreateStage(const string& fileName)
	{
		CreateSharedObjectGroup(L"Active");
		const auto& stageGroup = CreateSharedObjectGroup(L"Stage");
		const auto& gimmickGroup = CreateSharedObjectGroup(L"Gimmick");
		const auto& updateGroup = CreateSharedObjectGroup(L"Update");
		vector<weak_ptr<GameObject>> enemyVec;
		if (GetSharedObjectGroup(L"Enemy", false))
		{
			enemyVec = GetSharedObjectGroup(L"Enemy")->GetGroupVector();
		}

		m_csvData = CSVLoader::LoadFile(fileName);

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
		const Vec3 slopeScale = Vec3(scale * 1.45f, scale * 1.45f, scale);
		const Vec2 slopeULeft = Vec2(0.5f, -0.5f);
		const Vec2 slopeURight = Vec2(-0.5f, -0.5f);
		const Vec2 slopeDLeft = Vec2(0.5f, 0.5f);
		const Vec2 slopeDRight = Vec2(-0.5f, 0.5f);

		Checker checker;
		for (size_t i = 0; i < m_csvData.size(); i++)
		{
			for (size_t j = 0; j < m_csvData.at(i).size(); j++)
			{
				if (m_csvData.at(i).at(j) == "") continue;

				shared_ptr<CubeObject> block = nullptr;
				shared_ptr<Gimmick> gimmick = nullptr;
				shared_ptr<Gimmick> update = nullptr;

				const int& num = stoi(m_csvData.at(i).at(j));

				switch (num)
				{
				case 0:
					checker.reset();
					break;

				case 1:
					block = AddGameObject<DeathColl>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, true);
					break;

				case 3:
					block = AddGameObject<StagingColl>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale);
					break;

				case 2:
				case 100:
				case 110:
				case 120:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, true);
					break;

				case 101:
				case 111:
				case 121:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)) + slopeULeft, slopeScale, CubeObject::SlopeUL, true);
					break;

				case 102:
				case 112:
				case 122:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)) + slopeURight, slopeScale, CubeObject::SlopeUR, true);
					break;

				case 103:
				case 113:
				case 123:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)) + slopeDLeft, slopeScale, CubeObject::SlopeDL, true);
					break;

				case 104:
				case 114:
				case 124:
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)) + slopeDRight, slopeScale, CubeObject::SlopeDR, true);
					break;

				case 230:
					update = AddGameObject<Ring>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Ring::Big);
					break;

				case 231:
					update = AddGameObject<Ring>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, Ring::Small);
					break;

				case 400:
					AddGameObject<GoalCannon>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale * 5.0f);

				default:
					break;
				}

				// ����ȃu���b�N��M�~�b�N�p

				// ��
				if (GetBetween(num, 200, 204))
				{
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&m_csvData.at(i).at(j).at(2)));
					update = AddGameObject<Spike>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, angle);
					block = AddGameObject<Alpha>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale * 0.9f, true);
				}

				// �x���g�R���x�A
				if (GetBetween(num, 2101, 2119))
				{
					checker.count++;
					checker.type = m_csvData.at(i).at(j);
					checker.check = m_csvData.at(i).at(j + 1) != checker.type || m_csvData.at(i).at(j - 1) != checker.type;
					const auto& rotate = static_cast<Convayor::eRotate>(atoi(&m_csvData.at(i).at(j).at(2)) / 10);
					const auto& beltType = static_cast<Convayor::eType>(checker.check);
					const float& speed = static_cast<float>(atof(&m_csvData.at(i).at(j).at(3)));
					update = AddGameObject<Convayor>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale, rotate, beltType, speed);
				}

				// ��C
				if (GetBetween(num, 2200, 2283))
				{
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&m_csvData.at(i).at(j).at(2)) / 10);
					const auto& fireType = static_cast<Cannon::eFireType> (atoi(&m_csvData.at(i).at(j).at(3)));
					gimmick = AddGameObject<Cannon>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale * 3.0f, angle, fireType);
				}

				// �����@
				if (GetBetween(num, 240, 243))
				{
					const Vec3 blowerScale = Vec3(scale * 5.0f, scale, scale * 5.0f);
					const auto& angle = static_cast<Gimmick::eAngle>(atoi(&m_csvData.at(i).at(j).at(2)));
					update = AddGameObject<Blower>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), blowerScale, angle, 5.0f);
				}

				if (block)
				{
					block->AddTarget(GetSharedGameObject<Player>(L"Player"));
					block->AddTarget(enemyVec);
					stageGroup->IntoGroup(block);
				}

				if (gimmick)
				{
					gimmick->AddTarget(GetSharedGameObject<Player>(L"Player"));
					gimmickGroup->IntoGroup(gimmick);
				}

				if (update)
				{
					update->AddTarget(GetSharedGameObject<Player>(L"Player"));
					updateGroup->IntoGroup(update);
				}
			}
		}
	}

	void BaseStage::CreateInstanceBlock(const string& fileName)
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
		const auto& data = CSVLoader::LoadFile(fileName);

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

	void BaseStage::OnDestroy()
	{
		if (m_bgm.lock())
		{
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			audioPtr->Stop(m_bgm.lock());
		}
	}

	void BaseStage::OnUpdate()
	{
		try
		{
			const auto& fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
			Debug::Log(L"FPS : ", fps);

			const float range = 55.0f;
			const auto& playerPos = GetSharedGameObject<Player>(L"Player")->GetPosition();

			const auto& enemyVec = GetSharedObjectGroup(L"Enemy")->GetGroupVector();
			for (const auto& weakObj : enemyVec)
			{
				const auto& enemyObj = dynamic_pointer_cast<Enemy>(weakObj.lock());

				if (!enemyObj) continue;

				bool alive = false;
				const Vec3& pos = playerPos;
				float length = (enemyObj->GetPosition() - pos).length();
				enemyObj->SetDrawActive(length <= range);
				enemyObj->SetUpdateActive(length <= range);
			}

			const auto& cubeVec = GetSharedObjectGroup(L"Stage")->GetGroupVector();
			const auto& activeGroup = GetSharedObjectGroup(L"Active");
			activeGroup->AllClear();
			for (const auto& weakObj : cubeVec)
			{
				const auto& cubeObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());

				if (!cubeObj) continue;

				bool alive = false;
				const auto& vec = cubeObj->GetTargetVec();
				for (const auto& v : vec)
				{
					if (!v.lock()) continue;
					if (!v.lock()->GetUpdateActive()) continue;

					const Vec3& pos = v.lock()->GetComponent<Transform>()->GetPosition();
					float length = (cubeObj->GetPosition() - pos).length();
					if (length <= 4.0f)						
					{
						activeGroup->IntoGroup(cubeObj);
						alive = true;
						break;
					}
				}

				cubeObj->SetUpdateActive(alive);

				float length = (cubeObj->GetPosition() - playerPos).length();
				cubeObj->SetDrawActive(length <= range);
			}

			const auto& gimmickVec = GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
			for (const auto& weakObj : gimmickVec)
			{
				const auto& gimmickObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());

				if (!gimmickObj) continue;

				float length = (gimmickObj->GetPosition() - playerPos).length();
				gimmickObj->SetUpdateActive(length <= range);
				gimmickObj->SetDrawActive(length <= range);
			}

			const auto& updateVec = GetSharedObjectGroup(L"Update")->GetGroupVector();

			for (const auto& weakObj : updateVec)
			{
				const auto& gimmickObj = dynamic_pointer_cast<Gimmick>(weakObj.lock());

				if (!gimmickObj) continue;

				float length = (gimmickObj->GetPosition() - playerPos).length();

				if (gimmickObj->FindTag(L"Ring")) gimmickObj->SetUpdateActive(length <= range / 1.5f);
				else gimmickObj->SetUpdateActive(length <= range / 2.0f);
				gimmickObj->SetDrawActive(length <= range);
			}
		}
		catch (...)
		{
			throw;
		}
	}

	void BaseStage::OnDraw()
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