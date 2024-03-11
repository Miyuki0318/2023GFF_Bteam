/*!
@file TitleStage.cpp
@brief �^�C�g���p�X�e�[�W
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Input;
	using namespace Utility;

	// ���\�[�X�̓ǂݍ���
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

		// �e�N�X�`���̎擾
		// �^�C�g�����S�̓ǂݍ���
		app->RegisterTexture(L"TITLE_LOGO", texturePath + L"TitleLogo.tga");

		// �X�^�[�g�{�^���̓ǂݍ���
		app->RegisterTexture(L"PUSH_BUTTON", texturePath + L"PushButton.tga");

		// �T�E���h�f�B���N�g���p�X
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// �T�E���h�̎擾
		// �^�C�g�����BGM�̓ǂݍ���
		app->RegisterWav(L"TITLE_BGM", BGMPath + L"TitleBGM");
	}

	// �r���[�ƃ��C�g�̐���
	void TitleStage::CreateViewLight()
	{
		// �r���[�̃J�����̐ݒ�
		auto ptrCamera = ObjectFactory::Create<Camera>();
		ptrCamera->SetEye(Vec3(-20.0f, -75.0f, -40.0f));
		ptrCamera->SetAt(Vec3(-20.0f, -75.0f, 0.0f));
		m_gameView = CreateView<SingleView>();
		m_gameView->SetCamera(ptrCamera);

		// �}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		ptrMultiLight->SetDefaultLighting();
	}

	// �w�i�̐���
	void TitleStage::CreateBackGround()
	{
		// �I�u�W�F�N�g�̐���
		m_backObj = AddGameObject<TemplateObject>();

		// �p�����[�^�̐錾
		float x, y, loop;
		loop = 5.0f; // ���[�v��
		x = 80.0f * loop;
		y = 45.0f * loop;

		const auto& backObj = m_backObj.lock(); 
		backObj->SetPosition(Vec3(-90.0f, 0.0f, 50.0f));// ���W�̐ݒ�
		backObj->SetScale(Vec3(x, y, 5.0f)); // �X�P�[���̐ݒ�
		backObj->SetAlphaActive(true);// �����F�̕`����\��

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
	void TitleStage::CreatePlayer()
	{
		// �v���C���[�̐������s��
		m_player = AddGameObject<TitlePlayer>(Vec3(-50.0f, -55.0f, 0.0f));

		// �V�F�A�I�u�W�F�N�g�Ƃ��ēo�^
		SetSharedGameObject(L"Player", m_player.lock());
	}

	// �X�v���C�g�̐���
	void TitleStage::CreateSprites()
	{
		// �t�F�[�h�p�X�v���C�g
		m_fade = AddGameObject<Sprite>(L"WHITE_TX", WINDOW_SIZE, Vec3(0.0f));

		// �^�C�g�����S�ƃv�b�V���{�^���X�v���C�g
		m_titleLogo = AddGameObject<Sprite>(L"TITLE_LOGO", WINDOW_SIZE, Vec3(0.0f, 50.0f, 0.1f));
		m_pushButton = AddGameObject<Sprite>(L"PUSH_BUTTON", WINDOW_SIZE / 2.0f, Vec3(0.0f, -200.0f, 0.1f));
		
		// ���^���E�B���h�E�p�X�v���C�g
		m_metalLeft = AddGameObject<Sprite>(L"METAL_LEFT", WINDOW_SIZE, Vec3(0.0f, 0.0f, 0.2f));
		m_metalRight = AddGameObject<Sprite>(L"METAL_RIGHT", WINDOW_SIZE, Vec3(0.0f, 0.0f, 0.2f));
	}

	// �v�b�V���{�^���X�e�[�g
	void TitleStage::PushButtonState()
	{
		// A�{�^�����͂������
		if (GetPushA())
		{
			// �X�e�[�g�����^���t�F�[�h�ɕύX
			m_stageState = MetalFadeOut;

			// �v�b�V���{�^���ƃ��^���E�B���h�E��SE���Đ�
			CreateSE(L"PUSH_SE", 1.5f);
			CreateSE(L"METAL_SE", 0.75f);
		}
	}

	// ���^���E�B���h�E�X�e�[�g
	void TitleStage::MetalFadeState()
	{
		// �e�X�v���C�g�̍��W���擾
		const Vec3& mLPos = m_metalLeft.lock()->GetPosition();
		const Vec3& mRPos = m_metalRight.lock()->GetPosition();
		const Vec3& logoPos = m_titleLogo.lock()->GetPosition();
		const Vec3& pushPos = m_pushButton.lock()->GetPosition();

		// ���^���E�B���h�E��������ʊO�ɍs���܂�
		if (mLPos.x > -WINDOW_WIDTH / 1.9f)
		{
			// ���^���E�B���h�E�͍��E�ŉ�ʊO�Ɉړ�������
			m_metalLeft.lock()->SetPosition(mLPos + Vec3(-DELTA_TIME * 250.0f, 0.0f, 0.0f));
			m_metalRight.lock()->SetPosition(mRPos + Vec3(DELTA_TIME * 250.0f, 0.0f, 0.0f));

			// ���S�ƃv�b�V���{�^���͏㉺�ŉ�ʊO�Ɉړ�������
			m_titleLogo.lock()->SetPosition(logoPos + Vec3(0.0f, DELTA_TIME * 200.0f, 0.0f));
			m_pushButton.lock()->SetPosition(pushPos + Vec3(0.0f, -DELTA_TIME * 100.0f, 0.0f));
		}
		else
		{
			// ���^���E�B���h�E��SE���~�����A��~����SE���Đ�
			StopSE(L"METAL_SE");
			CreateSE(L"METAL_STOP_SE", 1.5f);

			// �J�n���̈ړ����o�X�e�[�g�ɕύX
			m_stageState = StartMove;
		}
	}

	// �J�n���̈ړ����o�X�e�[�g
	void TitleStage::StartMoveState()
	{
		// �v���C���[�̍��W�ƒ��n��͈͍̔��W
		const Vec3& playerPos = m_player.lock()->GetPosition();
		const Vec3 leftUnder = Vec3(-22.0f, -89.0f, 0.0f);  // ���n��̍���
		const Vec3 rightUpper = Vec3(-20.0f, -88.0f, 0.0f); // ���n��̉E��

		// �v���C���[�̍��W���͈͓��Ȃ�
		if (GetBetween(playerPos, leftUnder, rightUpper))
		{
			// ��Փx�I���X�e�[�g�ɕύX
			m_stageState = ModeSelect;
		}
	}

	// �t�F�[�h�A�E�g�X�e�[�g
	void TitleStage::FadeOutState()
	{
		// �V�[���I�u�W�F�N�g�̎擾
		const auto& scene = App::GetApp()->GetScene<Scene>();
		if (m_fade.lock()->FadeInColor(1.0f))
		{
			// �V�[������Q�[���X�e�[�W�ɑJ��
			PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"GameStage");
		}
	}

	// �������̏���
	void TitleStage::OnCreate()
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
			CreateBGM(L"TITLE_BGM", 0.25f);

			// �v���C���[�̍쐬
			CreatePlayer();

			// �X�v���C�g�̍쐬
			CreateSprites();

			// �X�e�[�W�̐���
			CreateStage("Title");
			CreateInstanceBlock("Title");

		}
		catch (...)
		{
			throw;
		}
	}

	// ���t���[���X�V����
	void TitleStage::OnUpdate()
	{
		try
		{
			// �X�e�[�g�ɉ����ď���
			switch (m_stageState)
			{
			case TitleStage::FadeIn: // �t�F�[�h�C���Ȃ�
				// �t�F�[�h���|���ăt�F�[�h���I�������v�b�V���{�^���X�e�[�g�ɕύX
				if (m_fade.lock()->FadeOutColor(2.0f)) m_stageState = PushButton;
				break;

			case TitleStage::PushButton: // �v�b�V���{�^���Ȃ�
				PushButtonState(); // �v�b�V���{�^���X�e�[�g���������s
				break;

			case TitleStage::MetalFadeOut: // ���^���E�B���h�E�t�F�[�h�Ȃ�
				MetalFadeState(); // ���^���E�B���h�E�X�e�[�g���������s
				break;

			case TitleStage::StartMove: // �J�n���̈ړ����o�Ȃ�
				StartMoveState(); // �J�n���̈ړ����o���������s
				break;

			case TitleStage::FadeOut: // �t�F�[�h�A�E�g�Ȃ�
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