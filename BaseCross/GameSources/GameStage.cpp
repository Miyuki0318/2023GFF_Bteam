/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;
	using namespace Utility;

	// ���\�[�X�̓ǂݍ���
	void GameStage::CreateResourses()
	{
		// �p�����̃��\�[�X�̓ǂݍ��݂����s����
		BaseStage::CreateResourses();

		// �A�v���P�[�V�����̎擾
		const auto& app = App::GetApp();

		// ���f�B�A�f�B���N�g���p�X�̎擾
		const wstring mediaPath = app->GetDataDirWString();

		// �e�N�X�`���f�B���N�g���p�X�̎擾
		const wstring texturePath = mediaPath + L"Textures/";

		// �������̃e�N�X�`��
		app->RegisterTexture(L"DISCOVER_TX", texturePath + L"Discovered.png");

		// UI�p�e�N�X�`��
		app->RegisterTexture(L"JGAUGE_TX", texturePath + L"JumpUI.png");
		app->RegisterTexture(L"SGAUGE_TX", texturePath + L"ShieldUI.png");
		app->RegisterTexture(L"GAUGE_TX", texturePath + L"Gauge.png");
		app->RegisterTexture(L"N_EFFECT_TX", texturePath + L"WhiteEffect.png");
		app->RegisterTexture(L"GAMEOVER_TX", texturePath + L"GameOver.png");
		app->RegisterTexture(L"CONTINUE_TX", texturePath + L"Continue.png");
		app->RegisterTexture(L"GUIDE_TX", texturePath + L"ConvayorGuide.png");

		// �T�E���h�f�B���N�g���p�X�̎擾
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// �Q�[���pBGM
		app->RegisterWav(L"GAME_BGM", BGMPath + L"GameBGM");

		// �N���A����SE
		app->RegisterWav(L"CLEAR_SE", SEPath + L"GameClearSE");
	}

	// �r���[�ƃ��C�g�̐���
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

	// �w�i�̐���
	void GameStage::CreateBackGround()
	{
		// �I�u�W�F�N�g�̐���
		m_backObj = AddGameObject<TemplateObject>();

		// �p�����[�^�̐錾
		float x, y, loopX, loopY;
		loopX = 16; // X���̃��[�v��
		loopY = 12; // Y���̃��[�v��
		x = 80.0f * loopX;
		y = 45.0f * loopY;

		const auto& backObj = m_backObj.lock();
		backObj->SetPosition(Vec3(500.0f, 135.0f, 50.0f)); // ���W�̐ݒ�
		backObj->SetScale(Vec3(x, y, 5.0f)); // �X�P�[���̐ݒ�
		backObj->SetAlphaActive(true); // �����F�̕`����\��

		VertexData vertex; // ���_�f�[�^�̐���
		SimpleVerticesIndices(vertex); // �l�p�`�Őݒ�

		// UV���W�����[�v���Őݒ�
		vertex.vertices.at(1).textureCoordinate = Vec2(loopY, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, loopX);
		vertex.vertices.at(3).textureCoordinate = Vec2(loopY, loopX);

		// �`��R���|�[�l���g�̎擾
		auto backDraw = backObj->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	// �v���C���[�̐���
	void GameStage::CreatePlayer()
	{
		// �v���C���[�̐������s��
		auto player = AddGameObject<Player>(Vec3(-48.5f, -75.0f, 0.0f));

		// �V�F�A�I�u�W�F�N�g�Ƃ��ēo�^
		SetSharedGameObject(L"Player", player);
	}

	// �X�v���C�g�̐���
	void GameStage::CreateSprites()
	{
		// �t�F�[�h�p�X�v���C�g
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));
		
		// ���^���E�B���h�E�p�X�v���C�g
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE * 1.25f, Vec3(-675.0f, 0.0f, 0.1f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE * 1.25f, Vec3(675.0f, 0.0f, 0.1f));
		
		// �Q�[���I�[�o�[���̃X�v���C�g
		m_gameOver = AddGameObject<Sprite>(L"GAMEOVER_TX", WINDOW_SIZE, Vec3(0.0f, 600.0f, 0.2f));
		m_continue = AddGameObject<Sprite>(L"CONTINUE_TX", WINDOW_SIZE * 0.55f, Vec3(-300.0f, 600.0f, 0.2f));
		m_titleBack = AddGameObject<Sprite>(L"QUIT_TX", WINDOW_SIZE * 0.55f, Vec3(300.0f, 600.0f, 0.2f));
	}

	// UI�̐���
	void GameStage::CreateUI()
	{
		// �X�P�[���̐ݒ�
		const Vec2 scale = Vec2(25.0f, 150.0f) * 1.25f;

		// �W�����v���ƃV�[���h����UI�𐶐�
		AddGameObject<JumpCountUI>(scale, Vec3(600.0f, -225.0f, 0.3f));
		AddGameObject<ShieldCountUI>(scale, Vec3(525.0f, -225.0f, 0.3f));
	}

	// ���S���̃t�F�[�h�X�e�[�g
	void GameStage::DeathFadeState()
	{
		// �m�X�v���C�g�̍��W
		const Vec3& goPos = m_gameOver.lock()->GetPosition();
		const Vec3& coPos = m_continue.lock()->GetPosition();
		const Vec3& tbPos = m_titleBack.lock()->GetPosition();

		// �Q�[���I�[�o�[�X�v���C�g�̍��������ȏ�Ȃ�
		if (goPos.y >= 51.0f)
		{
			// �m�X�v���C�g�����Ɉړ�������
			m_gameOver.lock()->SetPosition(goPos + Vec3(0.0f, -DELTA_TIME * 600.0f, 0.0f));
			m_continue.lock()->SetPosition(coPos + Vec3(0.0f, -DELTA_TIME * 800.0f, 0.0f));
			m_titleBack.lock()->SetPosition(tbPos + Vec3(0.0f, -DELTA_TIME * 800.0f, 0.0f));
		}
		else
		{
			// �I�����X�e�[�g�ɕύX
			m_stageState = Select;
		}
	}
	
	// �I�����X�e�[�g
	void GameStage::SelectState()
	{
		// L�X�e�B�b�N�̓��͐^�U
		const bool inputLStick = IsInputLStickX();

		// �X�P�[��
		const Vec2 deffScale = WINDOW_SIZE * 0.55f;

		// �o�ߎ���
		m_totalTime += DELTA_TIME * 5.0f;

		// ���͂�����A�O�t���[���ɓ��͂��������
		if (inputLStick && !m_currentStickX)
		{
			// �o�ߎ��Ԃ����Z�b�g
			m_totalTime = 0.0f;

			// �I�����ɉ����ď���
			switch (m_select)
			{
			case GameStage::Continue: // �R���e�B�j���[��������
				m_select = TitleBack; // �؂�ւ���
				break;

			case GameStage::TitleBack: // �^�C�g���ɖ߂邾������
				m_select = Continue;   // �؂�ւ���
				break;

			default:
				break;
			}
		}

		// �I�����Ă�X�v���C�g���T�C���J�[�u�Ńo�E���h����
		m_continue.lock()->SetScale(deffScale * (m_select == Continue ? SinCurve(m_totalTime ,1.0f, 1.2f) : 1.0f));
		m_titleBack.lock()->SetScale(deffScale * (m_select == TitleBack ? SinCurve(m_totalTime, 1.0f, 1.2f) : 1.0f));
		
		// ���͐^�U��ێ�
		m_currentStickX = inputLStick;

		// A�{�^�����͂������
		if (GetPushA())
		{
			// ���^���E�B���h�E�X�e�[�g�ɕύX
			m_stageState = Metal;

			// ���^���E�B���h�E�̊J����SE���Đ�
			CreateSE(L"METAL_SE", 0.75f);
		}
	}

	// ���^���E�B���h�E�X�e�[�g
	void GameStage::MetalState()
	{
		// �e�X�v���C�g�̍��W���擾
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& goPos = m_gameOver.lock()->GetPosition();
		const Vec3& coPos = m_continue.lock()->GetPosition();
		const Vec3& tbPos = m_titleBack.lock()->GetPosition();

		// ���^���E�B���h�E�̍�����0.0��菬����(�����ɋ����)�Ȃ�
		if (mLPos.x <= 0.0f)
		{
			// ���^���E�B���h�E�𒆐S�ʒu�Ɉړ�������
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(DELTA_TIME * 850.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(-DELTA_TIME * 850.0f, 0.0f, 0.0f));
			
			// �Q�[���I�[�o�[�p�̃X�v���C�g����ʊO�Ɉړ�������
			m_gameOver.lock()->SetPosition(goPos + Vec3(0.0f, DELTA_TIME * 545.0f, 0.0f));
			m_continue.lock()->SetPosition(coPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
			m_titleBack.lock()->SetPosition(tbPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
		}
		else
		{
			// ���^���E�B���h�E�̊J����SE���~
			StopSE(L"METAL_SE");

			// ���^���E�B���h�E��~����SE���Đ�
			CreateSE(L"METAL_STOP_SE", 1.5f);

			// ���S�ʒu�ɏC��
			m_metalLeft.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.1f));
			m_metalRight.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.1f));

			// ��ʊO�ɏC��
			m_gameOver.lock()->SetPosition(Vec3(0.0f, 600.0f, 0.2f));
			m_continue.lock()->SetPosition(Vec3(-300.0f, 600.0f, 0.2f));
			m_titleBack.lock()->SetPosition(Vec3(300.0f, 600.0f, 0.2f));

			// �X�P�[���̏C��
			m_continue.lock()->SetScale(WINDOW_SIZE * 0.55f);
			m_titleBack.lock()->SetScale(WINDOW_SIZE * 0.55f);

			// �v���C���[�̎擾
			const auto& player = GetSharedGameObject<Player>(L"Player");

			// �I�����ŕ���
			switch (m_select)
			{
			case GameStage::Continue: // �R���e�B�j���[�Ȃ�
				ResetStage(); // �X�e�[�W�̃��Z�b�g�����s
				player->Reset(); // �v���C���[�̃��Z�b�g�����s
				CreateSE(L"METAL_SE", 0.75f); // ���^���E�B���h�E�̊J��SE���Đ�
				m_totalTime = 0.0f; // �o�ߎ��Ԃ�������
				m_stageState = Reset; // �X�e�[�W�X�e�[�g�����Z�b�g��
				break;

			case GameStage::TitleBack: // �^�C�g���ɖ߂�Ȃ�
				m_stageState = FadeOut; // �t�F�[�h�A�E�g�X�e�[�g��
				CreateSE(L"GAMEOVER_SE", 1.0f); // �Q�[���I�[�o�[����SE���Đ�
				break;

			default:
				break;
			}
		}
	}

	// ���Z�b�g�X�e�[�g
	void GameStage::ResetState()
	{
		// ���^���E�B���h�E�̍��W���擾
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();

		// �����̃��^���E�B���h�E�̍��W����ʓ��Ȃ�
		if (mLPos.x > -WINDOW_WIDTH / 1.9f)
		{
			// ��ʊO�Ɉړ�������
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(-DELTA_TIME * 850.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(DELTA_TIME * 850.0f, 0.0f, 0.0f));
		}
		else
		{
			// ���^���E�B���h�E�̊J����SE���~
			StopSE(L"METAL_SE");

			// ���^���E�B���h�E�̒�~����SE�Ƒ�C���ˎ���SE���Đ�
			CreateSE(L"METAL_STOP_SE", 1.5f);
			CreateSE(L"CANNON_SE", 1.0f);

			// �J���̈ړ����o�X�e�[�g�ɕύX
			m_stageState = StartMove;

			// ���^���E�B���h�E�̍��W���C��
			m_metalLeft.lock()->SetPosition(Vec3(-675.0f, 0.0f, 0.1f));
			m_metalRight.lock()->SetPosition(Vec3(675.0f, 0.0f, 0.1f));
		}
	}

	// �t�F�[�h�A�E�g�X�e�[�g
	void GameStage::FadeOutState(float fadeTime)
	{
		// �V�[�����擾
		const auto& scene = App::GetApp()->GetScene<Scene>();

		// �t�F�[�h������������
		if (m_fade.lock()->FadeInColor(fadeTime))
		{
			// �Q�[�����N���A���Ă邩
			if (m_isClear)
			{
				// �V�[���Ɍ��݂̃X�e�[�W���𑗂�
				scene->SetCurrentStage(m_stagePath);

				// �u���̃X�e�[�W�ցv�X�e�[�W�ɑJ��
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"NextStage");
			}
			else
			{
				// �u�^�C�g���v�X�e�[�W�ɑJ��
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"TitleStage");
			}
		}
	}

	// �X�e�[�W�̃��Z�b�g
	void GameStage::ResetStage()
	{
		// �v���C���[�ƃI�u�W�F�N�g�O���[�v�̎擾
		const auto& collectGroup = GetSharedObjectGroup(L"Collect");
		const auto& enemyGroup = GetSharedObjectGroup(L"Enemy");
		const auto& wallGroup = GetSharedObjectGroup(L"Wall");
		const auto& stageVec = GetSharedObjectGroup(L"Stage")->GetGroupVector();
		const auto& updateVec = GetSharedObjectGroup(L"Update")->GetGroupVector();
		const auto& playerPtr = GetSharedGameObject<Player>(L"Player");

		// �O���[�v���̃I�u�W�F�N�g���폜
		GroupObjectRemove(collectGroup);
		GroupObjectRemove(enemyGroup);
		GroupObjectRemove(wallGroup);

		// �O���[�v�̏�����
		collectGroup->AllClear();
		enemyGroup->AllClear();
		wallGroup->AllClear();

		// �p�����[�^�̐錾
		const float under = -97.5f; // ����
		const float left = -49.0f;  // ����
		const float scale = 1.0f;   // �X�P�[��

		// �I�u�W�F�N�g�z��
		vector<weak_ptr<Ring>> ringVec;
		vector<weak_ptr<JumpRabbit>> jumpVec;
		vector<weak_ptr<WallRabbit>> wallVec;

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

				// ���l�������O(230or231)�ƕǃE�T�M(301)�͈͓̔��Ȃ�
				if (GetBetween(num, 230, 301))
				{
					Vec2 position = Vec2(left + (j * scale), under + ((m_csvData.size() - i) * scale));

					switch (num)
					{
					case 230: // �傫�������O�Ȃ�

						// �傫�������O�I�u�W�F�N�g�𐶐����Ĕz��ɒǉ�
						ringVec.push_back(AddGameObject<Ring>(position, scale, Ring::eRingSize::Big));
						break;

					case 231: // �����������O�Ȃ�

						// �����������O�I�u�W�F�N�g�𐶐����Ĕz��ɒǉ�
						ringVec.push_back(AddGameObject<Ring>(position, scale, Ring::eRingSize::Small));
						break;

					case 300: // �W�����v�E�T�M�Ȃ�

						// �W�����v�E�T�M�I�u�W�F�N�g�𐶐����Ĕz��ɒǉ�
						jumpVec.push_back(AddGameObject<JumpRabbit>(position, scale));
						break;

					case 301: // �ǃE�T�M�Ȃ�

						// �ǃE�T�M�I�u�W�F�N�g�𐶐����Ĕz��ɒǉ�
						wallVec.push_back(AddGameObject<WallRabbit>(position, scale));
						break;

					default:
						break;
					}
				}
			}
		}

		// �����O�̐����[�v
		for (const auto& ring : ringVec)
		{
			// �G���[�`�F�b�N
			if (ring.lock())
			{
				// �R���W�����^�[�Q�b�g�Ƀv���C���[��ǉ�
				ring.lock()->AddTarget(playerPtr);

				// ���W���O���[�v�Ɏ��g��ǉ�
				collectGroup->IntoGroup(ring.lock());
			}
		}

		// �W�����v�E�T�M�̐����[�v
		for (const auto& jump : jumpVec)
		{
			// �G���[�`�F�b�N
			if (jump.lock())
			{
				// �W�����v�E�T�M�O���[�v�Ɏ��g��ǉ�
				enemyGroup->IntoGroup(jump.lock());
			}
		}

		// �ǃE�T�M�̐����[�v
		for (const auto& wall : wallVec)
		{
			// �G���[�`�F�b�N
			if (wall.lock())
			{
				// �ǃE�T�M�O���[�v�Ɏ��g��ǉ�
				wallGroup->IntoGroup(wall.lock());
			}
		}

		// �X�e�[�W�I�u�W�F�N�g�̐����[�v
		for (const auto& weakObj : stageVec)
		{
			// �G���[�`�F�b�N
			if (!weakObj.lock()) continue;

			// �^�L���X�g
			const auto& block = dynamic_pointer_cast<CubeObject>(weakObj.lock());
			if (!block) continue;

			// �R���W�����^�[�Q�b�g���폜���čĐݒ�
			block->RemoveTarget();
			block->AddTarget(enemyGroup->GetGroupVector());
			block->AddTarget(playerPtr);
		}

		// ���I�����������M�~�b�N�I�u�W�F�N�g�̐����[�v
		for (const auto& weakObj : updateVec)
		{
			// �G���[�`�F�b�N
			if (!weakObj.lock()) continue;

			// �^�L���X�g
			const auto& update = dynamic_pointer_cast<CubeObject>(weakObj.lock());
			if (!update) continue;

			// �R���W�����^�[�Q�b�g���폜���čĐݒ�
			update->RemoveTarget();
			update->AddTarget(enemyGroup->GetGroupVector());
			update->AddTarget(playerPtr);
		}
	}

	// �������̏���
	void GameStage::OnCreate() 
	{
		try
		{
			// �p�����̐������̏��������s
			BaseStage::OnCreate();

			// ���\�[�X�̓ǂݍ���
			CreateResourses();

			// �r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			// �w�i�̍쐬
			CreateBackGround();

			// BGM�̍Đ�
			CreateBGM(L"GAME_BGM", 0.3f);

			// �v���C���[�̍쐬
			CreatePlayer();

			// �X�v���C�g�̍쐬
			CreateSprites();

			// UI�̍쐬
			CreateUI();

			// �X�e�[�W����
			CreateEnemy(m_stagePath);
			CreateStage(m_stagePath);
			CreateInstanceBlock(m_stagePath);
		}
		catch (...) 
		{
			throw;
		}
	}

	// ���t���[���X�V����
	void GameStage::OnUpdate()
	{
		try
		{
			// �p�����̖��t���[���X�V���������s
			BaseStage::OnUpdate();

			// �X�e�[�g�ŕ��򏈗�
			switch (m_stageState)
			{
			case GameStage::FadeIn: // �t�F�[�h�C���Ȃ�

				// �t�F�[�h���I�������J�n���̈ړ����o�X�e�[�g�ɕύX
				if (m_fade.lock()->FadeOutColor(1.0f)) m_stageState = StartMove;
				break;

			case GameStage::Goal: // �S�[�����Ȃ�
				m_isClear = true; // �N���A�^�U�����
				FadeOutState(3.75f); // �t�F�[�h�A�E�g����
				break;

			case GameStage::Death: // ���S��Ȃ�
				DeathFadeState(); // ���S���̃t�F�[�h�X�e�[�g
				break;

			case GameStage::Select: // �I�����Ȃ�
				SelectState(); // �I�����X�e�[�g
				break;

			case GameStage::Metal: // ���^���E�B���h�E�Ȃ�
				MetalState(); // ���^���E�B���h�E�X�e�[�g
				break;

			case GameStage::Reset: // �R���e�B�j���[���Z�b�g�Ȃ�
				ResetState(); // �R���e�B�j���[���Z�b�g�X�e�[�g
				break;

			case GameStage::FadeOut: // �t�F�[�h�A�E�g�Ȃ�
				FadeOutState(3.0f); // �t�F�[�h�A�E�g�X�e�[�g
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