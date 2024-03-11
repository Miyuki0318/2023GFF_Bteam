/*!
@file BaseStage.cpp
@brief �X�e�[�W�̌p����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;
	using namespace CubeParam;
	using namespace GimmickAngle;
	using namespace CannonParam;

	// ���\�[�X�̓ǂݍ���
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

		// ���e�N�X�`��
		app->RegisterTexture(L"ARROW_TX", texturePath + L"Arrow.png");

		// �^�C�g���ɖ߂�e�N�X�`��
		app->RegisterTexture(L"QUIT_TX", texturePath + L"Quit.tga");

		// ���^���u���b�N�e�N�X�`��
		app->RegisterTexture(L"METAL_TX", texturePath + L"Metal.png");

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
		app->RegisterWav(L"RABBIT_DEATH_SE", SEPath + L"RabbitDeathSE");

		// �o���p�[��SE
		app->RegisterWav(L"BUMPER_SE", SEPath + L"BumperSE");

		// �Q�[���I�[�o�[SE
		app->RegisterWav(L"GAMEOVER_SE", SEPath + L"GameOverSE");
	}

	// BGM�̍Đ�
	void BaseStage::CreateBGM(const wstring& bgmKey, float volume)
	{
		// �I�[�f�B�I�}�l�[�W���[���擾
		const auto& audioPtr = App::GetApp()->GetXAudio2Manager();

		// �I�[�f�B�I�}�l�[�W���[��BGM���Đ�
		m_bgm = audioPtr->Start(bgmKey, XAUDIO2_LOOP_INFINITE, volume);
	}

	// SE�}�l�[�W���[�̐���
	void BaseStage::CreateSEManager()
	{
		// SE�}�l�[�W���[��NULL�Ȃ�
		if (!m_seManager)
		{
			// SE�}�l�[�W���[��make_shared�Ő���
			m_seManager = make_shared<SEManager>();
		}
	}

	// SE�̍Đ�
	void BaseStage::CreateSE(const wstring& seKey, float volume)
	{
		// SE�}�l�[�W���[��NULL�Ȃ�
		if (!m_seManager)
		{
			// SE�}�l�[�W���[�̐������s��
			CreateSEManager();
		}

		// SE�}�l�[�W���[����SE�̍Đ��𑗂�
		m_seManager->StartSE(seKey, volume, ThisPtr);
	}

	// SE�̍Đ�
	void BaseStage::CreateSE(const wstring& seKey, float volume, const void* objPtr)
	{
		// SE�}�l�[�W���[��NULL
		if (!m_seManager)
		{
			// SE�}�l�[�W���[�̐������s��
			CreateSEManager();
		}

		// SE�}�l�[�W���[����SE�̍Đ��𑗂�
		m_seManager->StartSE(seKey, volume, objPtr);
	}

	// SE�̒�~
	void BaseStage::StopSE(const wstring& seKey)
	{
		// SE�}�l�[�W���[������Ȃ�
		if (m_seManager)
		{
			// SE�}�l�[�W���[����SE�̒�~�𑗂�
			m_seManager->StopSE(seKey, ThisPtr);
		}
	}

	// SE�̒�~
	void BaseStage::StopSE(const wstring& seKey, const void* objPtr)
	{
		// SE�}�l�[�W���[������Ȃ�
		if (m_seManager)
		{
			// SE�}�l�[�W���[����SE�̒�~�𑗂�
			m_seManager->StopSE(seKey, objPtr);
		}
	}

	// �v���C���[�̐���
	void BaseStage::CreatePlayer()
	{
		// �v���C���[�̐������s��
		const auto& player = AddGameObject<Player>(Vec3(-20.0f, -80.0f, 0.0f));

		// �V�F�A�I�u�W�F�N�g�Ƃ��ēo�^
		SetSharedGameObject(L"Player", player);
	}

	// �G�L�����̐���
	void BaseStage::CreateEnemy(const string& fileName)
	{
		// CSV�̓ǂݍ���
		m_csvData = CSVLoader::LoadFile(fileName);

		// �p�����[�^�̐錾
		const float under = -97.5f; // ����
		const float left = -49.0f;  // ����
		const float scale = 1.0f;   // �X�P�[��

		vector<weak_ptr<JumpRabbit>> jumpVec; // �W�����v�E�T�M�z��
		vector<weak_ptr<WallRabbit>> wallVec; // �ǃE�T�M�z��

		// �s�������[�v
		for (size_t i = 0; i < m_csvData.size(); i++)
		{
			// �񐔕����[�v
			for (size_t j = 0; j < m_csvData.at(i).size(); j++)
			{
				// ���g����Ȃ牽�����Ȃ�
				if (m_csvData.at(i).at(j) == "") continue;

				// int�^�ɕϊ�
				const int& num = stoi(m_csvData.at(i).at(j));

				// ���l���W�����v�E�T�M(300)�ƕǃE�T�M(301)�͈͓̔��Ȃ�
				if (GetBetween(num, 300, 301))
				{
					switch (num)
					{
					case 300: // �W�����v�E�T�M
						jumpVec.push_back(AddGameObject<JumpRabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale));
						break;

					case 301: // �ǃE�T�M
						wallVec.push_back(AddGameObject<WallRabbit>(Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale)), scale));
						break;

					default:
						break;
					}
				}
			}
		}

		// �V�F�A�h�O���[�v���擾
		const auto& enemyGroup = GetSharedObjectGroup(L"Enemy");
		const auto& wallGroup = GetSharedObjectGroup(L"Wall");

		// �I�u�W�F�N�g�̐����[�v
		for (const auto& jump : jumpVec)
		{
			// �G���[�`�F�b�N
			if (jump.lock())
			{
				// �O���[�v�ɒǉ�
				enemyGroup->IntoGroup(jump.lock());
			}
		}

		// �I�u�W�F�N�g�̐����[�v
		for (const auto& wall : wallVec)
		{
			// �G���[�`�F�b�N
			if (wall.lock())
			{
				// �O���[�v�ɒǉ�
				wallGroup->IntoGroup(wall.lock());
			}
		}
	}

	// �Q�[�����̃X�e�[�W�̐���
	void BaseStage::CreateStage(const string& fileName)
	{
		// �v���C���[�ƃI�u�W�F�N�g�O���[�v�̎擾
		const auto& playerPtr = GetSharedGameObject<Player>(L"Player");
		const auto& stageGroup = GetSharedObjectGroup(L"Stage");
		const auto& gimmickGroup = GetSharedObjectGroup(L"Gimmick");
		const auto& updateGroup = GetSharedObjectGroup(L"Update");
		const auto& collectGroup = GetSharedObjectGroup(L"Collect");
		const auto& enemyVec = GetSharedObjectGroup(L"Enemy")->GetGroupVector();

		// CSV�̓ǂݍ���
		m_csvData = CSVLoader::LoadFile(fileName);

		// �x���g�R���x�A�p�̃`�F�b�J
		struct Checker
		{
			string type = "";   // �^�C�v�ۑ��p
			int count = 0;		// �A�ԃJ�E���^
			bool check = false; // ���ʂ�

			// ���Z�b�g�֐�
			void reset()
			{
				type = "";
				count = 0;
				check = false;
			}
		};

		// �p�����[�^�̐錾
		const float under = -97.5f; // ����
		const float left = -49.0f;  // ����
		const float scale = 1.0f;   // �X�P�[��

		// �X���[�v�p
		const Vec3 slopeScale = Vec3(scale * 1.45f, scale * 1.45f, scale); 
		const Vec2 slopeULeft = Vec2(GetSlopeDiff(eCubeType::SlopeUL));
		const Vec2 slopeURight = Vec2(GetSlopeDiff(eCubeType::SlopeUR));
		const Vec2 slopeDLeft = Vec2(GetSlopeDiff(eCubeType::SlopeDL));
		const Vec2 slopeDRight = Vec2(GetSlopeDiff(eCubeType::SlopeDR));

		// �����@�p
		const Vec3 blowerScale = Vec3(scale * 5.0f, scale, scale * 5.0f);

		Checker checker; // �`�F�b�J

		// �s�������[�v
		for (size_t i = 0; i < m_csvData.size(); i++)
		{
			// �񐔕����[�v
			for (size_t j = 0; j < m_csvData.at(i).size(); j++)
			{
				// ���g����Ȃ牽�����Ȃ�
				if (m_csvData.at(i).at(j) == "") continue;

				// �I�u�W�F�N�g�|�C���^
				shared_ptr<CubeObject> block = nullptr;
				shared_ptr<Gimmick> gimmick = nullptr;
				shared_ptr<Gimmick> update = nullptr;
				shared_ptr<Gimmick> collect = nullptr;

				// ���W(����+��ԍ��~�X�P�[��,����+�s��-�s�ԍ��~�X�P�[��)
				Vec2 position = Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale));

				// int�^�ɃL���X�g
				const int& num = stoi(m_csvData.at(i).at(j));

				// ���l�ŕ���
				switch (num)
				{
				case 0: // ��
					checker.reset();
					break;

				case 1: // ���S�p�R���W����
					block = AddGameObject<DeathColl>(position, scale);
					break;

				case 3: // ���o���p�R���W����
					block = AddGameObject<StagingColl>(position, scale);
					break;

				case 2:   // �����u���b�N
				case 100: // Iron�u���b�N
				case 110: // Metal�u���b�N
				case 120: // Dark�u���b�N
					block = AddGameObject<Alpha>(position, scale);
					break;

				case 101: // Iron����X���[�v
				case 111: // Metal����X���[�v
				case 121: // Dark����X���[�v
					block = AddGameObject<Alpha>(position + slopeULeft, slopeScale, CubeParam::eCubeType::SlopeUL);
					break;

				case 102: // Iron�E��X���[�v
				case 112: // Metal�E��X���[�v
				case 122: // Dark�E��X���[�v
					block = AddGameObject<Alpha>(position + slopeURight, slopeScale, CubeParam::eCubeType::SlopeUR);
					break;

				case 103: // Iron�����X���[�v
				case 113: // Metal�����X���[�v
				case 123: // Dark�����X���[�v
					block = AddGameObject<Alpha>(position + slopeDLeft, slopeScale, CubeParam::eCubeType::SlopeDL);
					break;

				case 104: // Iron�E���X���[�v
				case 114: // Metal�E���X���[�v
				case 124: // Dark�E���X���[�v
					block = AddGameObject<Alpha>(position + slopeDRight, slopeScale, CubeParam::eCubeType::SlopeDR);
					break;

				case 400: // �S�[���p��C
					AddGameObject<GoalCannon>(position, scale * 5.0f);
					break;

				default:
					break;
				}

				// ����ȃu���b�N��M�~�b�N�p

				// ��
				if (GetBetween(num, 200, 204))
				{
					// ���l�̖������p�x�penum�ɃL���X�g
					const auto angle = static_cast<eAngle>(atoi(&m_csvData.at(i).at(j).at(2)));
					
					// ���I�u�W�F�N�g�̐���
					update = AddGameObject<Spike>(position, scale, angle);

					// �W�����v�E�T�M�O���[�v���R���W�����^�[�Q�b�g�ɒǉ�
					update->AddTarget(enemyVec);
				}

				// ������
				if (GetBetween(num, 20000, 20200))
				{
					// ���l��100�̈ʂ������^�C�venum�ɃL���X�g
					const auto type = static_cast<MoveSpike::eMoveType>(atoi(&m_csvData.at(i).at(j).at(2)) / 100);
					
					// ���l��10��1�̈ʂ������Ƒ��x�ɃL���X�g
					const float length = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(3)) / 10);
					const float speed = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(4)));
					
					// �������I�u�W�F�N�g�̐���
					gimmick = AddGameObject<MoveSpike>(position, scale, type, speed, length);
					
					// �W�����v�E�T�M�O���[�v���R���W�����^�[�Q�b�g�ɒǉ�
					gimmick->AddTarget(enemyVec);
				}

				// �x���g�R���x�A
				if (GetBetween(num, 2101, 2119))
				{
					checker.count++; // �A�ԃJ�E���^�����Z
					checker.type = m_csvData.at(i).at(j); // �x���g�R���x�A�^�C�v��o�^

					// ���݂̗�ԍ���1��A�܂��͑O���Ⴄ�x���g�R���x�A�^�C�v�Ȃ�
					checker.check = m_csvData.at(i).at(j + 1) != checker.type || m_csvData.at(i).at(j - 1) != checker.type;
					
					// bool��0��1�Ńx���g�R���x�A�^�C�v��ݒ�
					const auto beltType = static_cast<Convayor::eBeltType>(checker.check);

					// ���l��10��1�̈ʂŉ�]�^�C�venum�Ɖ�]���x�ɃL���X�g
					const auto rotate = static_cast<Convayor::eRotType>(atoi(&m_csvData.at(i).at(j).at(2)) / 10);
					const float speed = static_cast<float>(atof(&m_csvData.at(i).at(j).at(3)));
					
					// �x���g�R���x�A�I�u�W�F�N�g�̐���
					update = AddGameObject<Convayor>(position, scale, rotate, beltType, speed);
					
					// �W�����v�E�T�M�O���[�v���R���W�����^�[�Q�b�g�ɒǉ�
					update->AddTarget(enemyVec);
				}

				// �x���g�R���x�A�̃K�C�h
				if (num == 310)
				{
					// ���g��1���̃x���g�R���x�AID���擾
					const int convayorID = stoi(m_csvData.at(i + 1).at(j));

					// �x���g�R���x�AID��10�̈ʂŉ�]�^�C�venum�ɃL���X�g
					const auto rotate = static_cast<Convayor::eRotType>((convayorID - 2100) / 10);

					// �x���g�R���x�A�K�C�h�I�u�W�F�N�g�̐���
					AddGameObject<ConvayorGuide>(Vec2(left + (j * scale), under + ((m_csvData.size() - i - 1) * scale)), scale, rotate);
				}

				// ��C
				if (GetBetween(num, 2200, 2283))
				{
					// ���l��10��1�̈ʂŊp�xenum�Ɣ��˃^�C�venum�ɃL���X�g
					const auto angle = static_cast<eAngle>(atoi(&m_csvData.at(i).at(j).at(2)) / 10);
					const auto fireType = static_cast<eFireType>(atoi(&m_csvData.at(i).at(j).at(3)));

					// ��C�I�u�W�F�N�g�̐���
					gimmick = AddGameObject<Cannon>(position, scale * 3.0f, angle, fireType);
				}

				// �^�C�g���p��C
				if (GetBetween(num, 2290, 2292))
				{
					// ���l��1�̈ʂœ�Փxenum�ɃL���X�g
					const auto diffType = static_cast<eDifficulty>(atoi(&m_csvData.at(i).at(j).at(3)));

					// �^�C�g���p��C�I�u�W�F�N�g�̐���
					gimmick = AddGameObject<TitleCannon>(position, scale * 3.0f, diffType);
				}

				// �����O
				if (GetBetween(num, 230, 231))
				{
					// ���l��1�̈ʂŃ����O�̃T�C�Yenum�ɃL���X�g
					const auto size = static_cast<Ring::eRingSize>(atoi(&m_csvData.at(i).at(j).at(2)));
					
					// �����O�I�u�W�F�N�g�̐���
					collect = AddGameObject<Ring>(position, scale, size);
				}

				// �����@
				if (GetBetween(num, 240, 243))
				{
					// ���l��1�̈ʂŊp�xenum�ɃL���X�g
					const auto angle = static_cast<eAngle>(atoi(&m_csvData.at(i).at(j).at(2)));

					// �����@�I�u�W�F�N�g�̐���
					gimmick = AddGameObject<Blower>(position, blowerScale, angle, 5.0f);
				}

				// �����ǊJ�p�{�^��
				if (GetBetween(num, 3110, 3119))
				{
					// ���l��1�̈ʂŎ��ʗp���l��ݒ�
					const int number = atoi(&m_csvData.at(i).at(j).at(3));

					// �{�^���I�u�W�F�N�g�̐���
					gimmick = AddGameObject<Button>(position, scale, number);

					// �W�����v�E�T�M�O���[�v���R���W�����^�[�Q�b�g�ɒǉ�
					gimmick->AddTarget(enemyVec);
				}

				// ������
				if (GetBetween(num, 50000, 57999))
				{
					// ���l��1000��100��10��1�̈ʂŋ����^�C�venum�ƊJ�����ƊJ���x�Ǝ��ʗp���l��ݒ�
					const auto type = static_cast<MoveWall::eMoveType>(atoi(&m_csvData.at(i).at(j).at(1)) / 1000);
					const float length = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(2)) / 100);
					const float speed = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(3)) / 10);
					const int number = atoi(&m_csvData.at(i).at(j).at(4));

					// �����ǃI�u�W�F�N�g�̐���
					gimmick = AddGameObject<MoveWall>(position, scale, type, speed, length, number);

					// �W�����v�E�T�M�O���[�v���R���W�����^�[�Q�b�g�ɒǉ�
					gimmick->AddTarget(enemyVec);
				}

				// �o���p�[
				if (num == 250)
				{
					// �o���p�[�I�u�W�F�N�g�̐���
					const auto& bumper = AddGameObject<Bumper>(position, scale * 3.0f);

					// �M�~�b�N�O���[�v�Ɏ��g��ǉ�
					gimmickGroup->IntoGroup(bumper);
				}

				// �����o���p�[
				if (GetBetween(num, 25000, 25199))
				{
					// ���l��100��10��1�̈ʂŋ����^�C�venum�ƈړ������ƈړ����x��ݒ�
					const auto type = static_cast<MoveBumper::eMoveType>(atoi(&m_csvData.at(i).at(j).at(2)) / 100);
					const float length = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(3)) / 10);
					const float speed = static_cast<float>(atoi(&m_csvData.at(i).at(j).at(4)));

					// �����o���p�[�I�u�W�F�N�g�̐���
					const auto& bumper = AddGameObject<MoveBumper>(position, scale * 3.0f, type, speed, length);
					
					// �M�~�b�N�O���[�v�Ɏ��g��ǉ�
					gimmickGroup->IntoGroup(bumper);
				}

				// �u���b�N�p�|�C���^�ɒ��g������Ȃ�
				if (block)
				{
					// �v���C���[�ƃW�����v�E�T�M�O���[�v���R���W�����^�[�Q�b�g�ɒǉ�
					block->AddTarget(playerPtr);
					block->AddTarget(enemyVec);

					// �u���b�N�p�O���[�v�Ɏ��g��ǉ�
					stageGroup->IntoGroup(block);
				}

				// �M�~�b�N�p�|�C���^�ɒ��g������Ȃ�
				if (gimmick)
				{
					// �v���C���[���R���W�����^�[�Q�b�g�ɒǉ�
					gimmick->AddTarget(playerPtr);

					// �M�~�b�N�p�O���[�v�Ɏ��g��ǉ�
					gimmickGroup->IntoGroup(gimmick);
				}

				// ���I�����������M�~�b�N�p�|�C���^�ɒ��g������Ȃ�
				if (update)
				{
					// �v���C���[���R���W�����^�[�Q�b�g�ɒǉ�
					update->AddTarget(playerPtr);

					// ���I�����������M�~�b�N�p�O���[�v�Ɏ��g��ǉ�
					updateGroup->IntoGroup(update);
				}

				// ���W���p�|�C���^�ɒ��g������Ȃ�
				if (collect)
				{
					// �v���C���[���R���W�����^�[�Q�b�g�ɒǉ�
					collect->AddTarget(playerPtr);

					// ���W���p�O���[�v�Ɏ��g��ǉ�
					collectGroup->IntoGroup(collect);
				}
			}
		}
	}

	// �u���b�N�̃C���X�^���X�`��
	void BaseStage::CreateInstanceBlock(const string& fileName)
	{
		// enum�̕ʖ�
		using eTypes = InstanceBlock::eTextureType;

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
				const auto& type = static_cast<eTypes>(k);
				if (block.at(k).count > 0)
				{
					// �u���b�N�p�̃C���X�^���X�`��I�u�W�F�N�g�̐���
					AddGameObject<InstanceBlock>(block.at(k).num, type, rowSize, i);
				}
				if (slope.at(k).count > 0)
				{
					// �X���[�v�p�̃C���X�^���X�`��I�u�W�F�N�g�̐���
					AddGameObject<InstanceSlope>(slope.at(k).num, type, rowSize, i);
				}
			}
		}
	}

	// �R���W�����A�N�e�B�u�O���[�v�ɒǉ�
	void BaseStage::ObjectInToAvtiveGroup(const vector<weak_ptr<GameObject>>& groupVec, const shared_ptr<GameObjectGroup>& activeGroup, const Vec3& playerPos, float updateRange)
	{
		// �p�����[�^�̐錾
		const Vec2 margin = Vec2(25.0f); // �]��
		const Vec3 pLeft = Vec3(WINDOW_SIZE + margin, 1.0f); // ���[
		const Vec3 pRight = Vec3(-WINDOW_SIZE - margin, 0.0f); // �E�[

		// �I�u�W�F�N�g�̐����[�v
		for (const auto& weakObj : groupVec)
		{
			// �G���[�`�F�b�N
			if (!weakObj.lock()) continue;

			// �^�L���X�g�ƃG���[�`�F�b�N
			const auto& cubeObj = dynamic_pointer_cast<CubeObject>(weakObj.lock());
			if (!cubeObj) continue;

			// �A�N�e�B�u���̐^�U
			bool active = false;

			// �������r����^�[�Q�b�g�z����擾
			const auto& vec = cubeObj->GetTargetVec();
			if (vec.empty()) continue;

			// �R���W�����^�[�Q�b�g�̐����[�v
			for (const auto& v : vec)
			{
				// �G���[�`�F�b�N�ƃA�N�e�B�u���̃`�F�b�N
				if (!v.lock()) continue;
				if (!v.lock()->GetUpdateActive()) continue;

				// �^�[�Q�b�g�̍��W�Ƃ̋��������߂�
				const Vec3& pos = v.lock()->GetComponent<Transform>()->GetPosition();
				float length = (cubeObj->GetPosition() - pos).length();

				// �X�V�������Ȃ�
				if (length <= updateRange)
				{
					// �A�N�e�B�u�O���[�v�ɒǉ�
					activeGroup->IntoGroup(cubeObj);
					active = true;
					break;
				}
			}

			// �X�V���邩�̓^�[�Q�b�g�Ƃ̋����ōs��
			cubeObj->SetUpdateActive(active);

			// �`�悷�邩�͉�ʓ����ōs��
			Vec3 point = Utility::ConvertToWorldPosition(m_gameView, cubeObj->GetPosition());
			cubeObj->SetDrawActive(Utility::GetBetween(point, pLeft, pRight));
		}
	}

	// �������̏���
	void BaseStage::OnCreate()
	{
		try
		{
			//�e�p�t�H�[�}���X�𓾂�
			SetCollisionPerformanceActive(true);
			SetUpdatePerformanceActive(true);
			SetDrawPerformanceActive(true);

			// �V�F�A�O���[�v�̍쐬
			CreateSharedObjectGroup(L"Stage");
			CreateSharedObjectGroup(L"Active");
			CreateSharedObjectGroup(L"Gimmick");
			CreateSharedObjectGroup(L"Update");
			CreateSharedObjectGroup(L"Collect");
			CreateSharedObjectGroup(L"Enemy");
			CreateSharedObjectGroup(L"Wall");

			// �^�C�}�[�I�u�W�F�N�g�̐���
			m_timer = AddGameObject<Timer>();
		}
		catch (...)
		{
			throw;
		}
	}

	// �j�����̏���
	void BaseStage::OnDestroy()
	{
		// bgm���ݒ肳��Ă�Ȃ�
		if (m_bgm.lock())
		{
			// �I�[�f�B�I�}�l�[�W���[����bgm�̒�~�𑗂�
			const auto& audioPtr = App::GetApp()->GetXAudio2Manager();
			audioPtr->Stop(m_bgm.lock());
		}
	}

	// ���t���[���X�V����
	void BaseStage::OnUpdate()
	{
		try
		{
			//// FPS�̕`��
			//const auto& fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
			//Debug::Log(L"FPS : ", fps);
			
			// �͈�
			const float range = 55.0f;
			const float cubeRange = 4.0f;

			// �v���C���[�̍��W
			const auto& playerPos = GetSharedGameObject<Player>(L"Player")->GetPosition();

			// �I�u�W�F�N�g�O���[�v�̎擾�Ƃ��̔z��
			const auto& enemyVec = GetSharedObjectGroup(L"Enemy")->GetGroupVector();
			const auto& wallVec = GetSharedObjectGroup(L"Wall")->GetGroupVector();
			const auto& gimmickVec = GetSharedObjectGroup(L"Gimmick")->GetGroupVector();
			const auto& updateVec = GetSharedObjectGroup(L"Update")->GetGroupVector();
			const auto& collectVec = GetSharedObjectGroup(L"Collect")->GetGroupVector();
			const auto& stageVec = GetSharedObjectGroup(L"Stage")->GetGroupVector();
			const auto& activeGroup = GetSharedObjectGroup(L"Active");

			// �p�t�H�[�}���X�Ǘ��֐������s
			ObjectPerformance<JumpRabbit>(enemyVec, playerPos, range / 1.5f);
			ObjectPerformance<WallRabbit>(wallVec, playerPos, range / 1.5f);
			ObjectPerformance<Gimmick>(gimmickVec, playerPos, range);
			ObjectPerformance<Gimmick>(updateVec, playerPos, range / 2.0f);
			ObjectPerformance<Gimmick>(collectVec, playerPos, range / 1.5f);

			// �X�e�[�W�I�u�W�F�N�g�O���[�v��������
			// �A�N�e�B�u�ɂȂ��Ă���I�u�W�F�N�g�̃O���[�v�����Z�b�g
			activeGroup->AllClear();
			ObjectInToAvtiveGroup(stageVec, activeGroup, playerPos, cubeRange);
			ObjectInToAvtiveGroup(updateVec, activeGroup, playerPos, range / 2.0f);
		}
		catch (...)
		{
			throw;
		}
	}

	// �`�揈��
	void BaseStage::OnDraw()
	{
		// �f�o�b�O������������I�ɋ�ɂ���
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// �p�����̕`�掞�̊֐������s����
		Stage::OnDraw();

		// �f�o�b�N�p������̕\����\���؂�ւ�
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawLayer(10);
		debugStr->SetDrawActive(false);
	}
}