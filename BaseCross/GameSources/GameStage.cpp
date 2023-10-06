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


		// �n�ʂ̃e�N�X�`���̓ǂݍ���
		app->RegisterTexture(L"GROUND", texturePath + L"Ground.png");
	}

	void GameStage::CreateViewLight() 
	{
		const Vec3 eye(0.0f, 5.0f, -75.0f);
		const Vec3 at(0.0f, 5.0f, 0.0f);
		auto PtrView = CreateView<SingleView>();
		// �r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		// �}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		// �f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	void GameStage::CreateGround()
	{
		AddGameObject<Player>();
		AddGameObject<Ground>(Vec3(0.0f, -0.5f, 0.0f), Vec3(1000.0f, 1.0f, 10.0f));
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
			CreateGround();
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
		debugStr->SetDrawActive(true);
	}
}