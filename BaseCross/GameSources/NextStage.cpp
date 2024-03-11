/*!
@file NextStage.cpp
@brief �u���̃X�e�[�W�ցv�X�e�[�W
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;
	using namespace Utility;

	// ���\�[�X�̓ǂݍ���
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

		// ���ւ̑I�����̓ǂݍ���
		app->RegisterTexture(L"GO_TX", texturePath + L"Go.png");
		app->RegisterTexture(L"NEXT_TX", texturePath + L"NextStage.png");

		// �e�N�X�`���̎擾
		// �T�E���h�f�B���N�g���p�X
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// �T�E���h�̎擾
		// �l�N�X�g���BGM�̓ǂݍ���
		app->RegisterWav(L"NEXT_BGM", BGMPath + L"NextBGM");
	}

	// �r���[�ƃ��C�g�̐���
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
		// �I�u�W�F�N�g�̐���
		m_backObj = AddGameObject<TemplateObject>();

		// �p�����[�^�̐錾
		float x, y, loop;
		loop = 2.0f; // ���[�v��
		x = 80.0f * loop;
		y = 45.0f * loop;

		const auto& backObj = m_backObj.lock();
		backObj->SetPosition(Vec3(0.0f, 0.0f, 50.0f)); // ���W�̐ݒ�
		backObj->SetScale(Vec3(x, y, 5.0f)); // �X�P�[���̐ݒ�
		backObj->SetAlphaActive(true); // �����F�̕`����\��

		VertexData vertex; // ���_�f�[�^�̐���
		SimpleVerticesIndices(vertex); // �l�p�`�Őݒ�

		// UV���W�����[�v���Őݒ�
		vertex.vertices.at(1).textureCoordinate = Vec2(loop, 0.0f);
		vertex.vertices.at(2).textureCoordinate = Vec2(0.0f, loop);
		vertex.vertices.at(3).textureCoordinate = Vec2(loop, loop);

		// �`��R���|�[�l���g�̎擾
		auto backDraw = backObj->AddComponent<PCTStaticDraw>();
		backDraw->SetOriginalMeshUse(true);
		backDraw->CreateOriginalMesh(vertex);
		backDraw->SetTextureResource(L"BACKGROUND_TX");
		backDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	// �v���C���[�̐���
	void NextStage::CreatePlayer()
	{
		// �v���C���[�̐������s��
		AddGameObject<NextPlayer>(Vec3(0.0f));
	}

	// �X�v���C�g�̐���
	void NextStage::CreateSprites()
	{
		// �t�F�[�h�p�X�v���C�g
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));
		
		// ���S�ƑI�����p�X�v���C�g
		m_logo = AddGameObject<Sprite>(L"NEXT_TX", WINDOW_SIZE, Vec3(0.0f, 200.0f, 0.2f));
		m_next = AddGameObject<Sprite>(L"GO_TX", WINDOW_SIZE * 0.55f, Vec3(-300.0f, 600.0f, 0.2f));
		m_back = AddGameObject<Sprite>(L"QUIT_TX", WINDOW_SIZE * 0.55f, Vec3(300.0f, 600.0f, 0.2f));

		// ���^���E�B���h�E�p�X�v���C�g
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE, Vec3(-675.0f, 0.0f, 0.2f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE, Vec3(675.0f, 0.0f, 0.2f));
	}

	// �w�i�̃X�N���[��
	void NextStage::SlideBackGround()
	{
		// �w�i�I�u�W�F�N�g����`��R���|�[�l���g���擾
		const auto& backObj = m_backObj.lock();
		const auto backDraw = backObj->GetComponent<PCTStaticDraw>();

		// ���_�f�[�^��UV���W���f���^�^�C���ŉ��Z���ăX�N���[������
		auto& vertices = backDraw->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
		for (auto& v : vertices) v.textureCoordinate += Vec2(1.0f, 0.0f) * DELTA_TIME;
		backDraw->UpdateVertices(vertices);
	}

	// �I�����X�e�[�g
	void NextStage::SelectState()
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
			case NextStage::Next: // ���̃X�e�[�W�ւȂ�
				m_select = Back;  // �^�C�g���ɖ߂�ɐ؂�ւ���
				break;

			case NextStage::Back: // �^�C�g���ɖ߂�Ȃ�
				m_select = Next;  // ���̃X�e�[�W�ւɐ؂�ւ���
				break;

			default:
				break;
			}
		}

		// ���͐^�U��ێ�
		m_currentStickX = inputLStick;

		// �I�����Ă�X�v���C�g���T�C���J�[�u�Ńo�E���h����
		m_next.lock()->SetScale(deffScale * (m_select == Next ? SinCurve(m_totalTime, 1.0f, 1.2f) : 1.0f));
		m_back.lock()->SetScale(deffScale * (m_select == Back ? SinCurve(m_totalTime, 1.0f, 1.2f) : 1.0f));
	}

	// ���^���E�B���h�E�X�e�[�g
	void NextStage::MetalState()
	{
		// �e�X�v���C�g�̍��W���擾
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& nxPos = m_next.lock()->GetPosition();
		const Vec3& tbPos = m_back.lock()->GetPosition();

		// ���^���E�B���h�E�̍�����0.0��菬����(�����ɋ����)�Ȃ�
		if (mLPos.x < 0.0f)
		{
			// ���^���E�B���h�E�𒆐S�ʒu�Ɉړ�������
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(DELTA_TIME * 750.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(-DELTA_TIME * 750.0f, 0.0f, 0.0f));
			
			// �Q�[���I�[�o�[�p�̃X�v���C�g����ʊO�Ɉړ�������
			m_next.lock()->SetPosition(nxPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
			m_back.lock()->SetPosition(tbPos + Vec3(0.0f, DELTA_TIME * 800.0f, 0.0f));
		}
		else
		{
			// ���^���E�B���h�E�̊J����SE���~
			StopSE(L"METAL_SE");

			// ���^���E�B���h�E��~����SE���Đ�
			CreateSE(L"METAL_STOP_SE", 1.5f);

			// ���S�ʒu�ɏC��
			m_metalLeft.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.2f));
			m_metalRight.lock()->SetPosition(Vec3(0.0f, 0.0f, 0.2f));

			// �t�F�[�h�A�E�g�X�e�[�g�֐؂�ւ���
			m_stageState = FadeOut;
		}
	}

	// �t�F�[�h�C���X�e�[�g
	void NextStage::FadeInState()
	{
		// �t�F�[�h�p�X�v���C�g�̎擾
		const auto& fade = m_fade.lock();

		// �X�v���C�g�̍��W���擾
		const Vec3& nextPos = m_next.lock()->GetPosition();
		const Vec3& backPos = m_back.lock()->GetPosition();

		// �t�F�[�h�A�E�g���|����
		if (fade->FadeOutColor(2.0f))
		{
			m_stageState = Select; // �X�e�[�g��I�����X�e�[�g�ɐ؂�ւ���
		}
		else
		{
			// Lerp��Y�����W�����߂Đݒ�
			float y = Utility::Lerp(600.0f, -100.0f, 1.0f - fade->GetDiffuseColor().w);
			m_next.lock()->SetPosition(nextPos.x, y, nextPos.z);
			m_back.lock()->SetPosition(backPos.x, y, backPos.z);
		}
	}

	// �t�F�[�h�A�E�g�X�e�[�g
	void NextStage::FadeOutState()
	{
		// �t�F�[�h�p�X�v���C�g�̎擾
		const auto& fade = m_fade.lock();

		// �V�[���I�u�W�F�N�g�̎擾
		const auto& scene = App::GetApp()->GetScene<Scene>();
		
		// �X�v���C�g�̍��W���擾
		const Vec3& nextPos = m_next.lock()->GetPosition();
		const Vec3& backPos = m_back.lock()->GetPosition();

		// �t�F�[�h�C�����|����
		if (fade->FadeInColor(1.0f))
		{
			// �I�����ŕ��򏈗�
			switch (m_select)
			{
			case NextStage::Next: // ���̃X�e�[�W�ւȂ�

				// �V�[������Q�[���X�e�[�W�֑J�ڂ�����
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
				break;

			case NextStage::Back: // �^�C�g���ɖ߂�Ȃ�

				// �V�[������^�C�g���X�e�[�W�ɑJ�ڂ�����
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"TitleStage");
				break;

			default:
				break;
			}
		}
		else
		{
			// Lerp��Y�����W�����߂Đݒ肷��
			float y = Utility::Lerp(-100.0f, 600.0f, fade->GetDiffuseColor().w);
			m_next.lock()->SetPosition(nextPos.x, y, nextPos.z);
			m_back.lock()->SetPosition(backPos.x, y, backPos.z);
		}
	}

	// �������̏���
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
			CreateBGM(L"NEXT_BGM", 0.25f);

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

	// ���t���[���X�V����
	void NextStage::OnUpdate()
	{
		try
		{
			// �w�i�̃X�N���[��
			SlideBackGround();

			// �X�e�[�g�ɉ����ď���
			switch (m_stageState)
			{
			case NextStage::FadeIn: // �t�F�[�h�C���Ȃ�
				FadeInState(); // �t�F�[�h�C���X�e�[�g���������s
				break;

			case NextStage::Select:	// �I�����Ȃ�
				SelectState(); // �I�����X�e�[�g���������s
				break;

			case NextStage::MetalFade: // ���^���t�F�[�h�Ȃ�
				MetalState(); // ���^���E�B���h�E�X�e�[�g���������s
				break;

			case NextStage::FadeOut: // �t�F�[�h�A�E�g�Ȃ�
				FadeOutState(); // �t�F�[�h�A�E�g�X�e�[�g���������s
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