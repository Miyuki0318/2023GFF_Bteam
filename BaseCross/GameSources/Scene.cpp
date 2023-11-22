
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::CreateResourses()
	{
		// �A�v���P�[�V�����̎擾
		const auto& app = App::GetApp();

		// �f�B���N�g���p�X�̐ݒ�
		const wstring mediaPath = app->GetDataDirWString();

		// �e�N�X�`���f�B���N�g���p�X
		const wstring texturePath = mediaPath + L"Textures/";

		// ���h��e�N�X�`��
		app->RegisterTexture(L"WHITE_TX", texturePath + L"White.png");
		app->RegisterTexture(L"WHITE_BAR_TX", texturePath + L"WhiteBar.png");

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

		// �w�i�e�N�X�`��
		app->RegisterTexture(L"BACKGROUND_TX", texturePath + L"BackGround.png");

		// ���e�N�X�`��
		app->RegisterTexture(L"ARROW_TX", texturePath + L"Arrow.png");

		// ���f���f�B���N�g���p�X
		const wstring modelPath = mediaPath + L"Models/";

		// �u���b�N���f��
		RegisterSingleMesh(L"BLOCK", modelPath + L"Block/", L"Block", false);
		RegisterSingleMesh(L"SLOPE", modelPath + L"Block/", L"Slope", false);
		app->RegisterTexture(L"GRASS_TX", modelPath + L"Block/Grass.png");
		app->RegisterTexture(L"DIRT_TX", modelPath + L"Block/Dirt.png");
		app->RegisterTexture(L"ROCK_TX", modelPath + L"Block/Rock.png");

		// �X�p�C�N���f��
		RegisterSingleMesh(L"SPIKE", modelPath, L"Spike", false);
		RegisterSingleMesh(L"SPIKE_BLOCK", modelPath, L"SpikeBlock", false);

		// ���{�b�g���f��
		RegisterSingleMesh(L"ROBOT_BODY", modelPath + L"Robot/", L"Robot_Body", true);
		RegisterMultiMesh(L"ROBOT_ARM", modelPath + L"Robot/", L"Robot_Arm", true);

		// �����f��
		RegisterSingleMesh(L"BIRD", modelPath + L"Bird/", L"Bird", true);

		// ��C���f��
		RegisterSingleMesh(L"CANNON", modelPath + L"Cannon/", L"Cannon", true);

		// �x���g�R���x�A���f��
		RegisterMultiMesh(L"MCONVAYOR", modelPath + L"Convayor/", L"m_Convayor", true);
		RegisterMultiMesh(L"SCONVAYOR", modelPath + L"Convayor/", L"s_Convayor", true);

		// �����O���f��
		RegisterSingleMesh(L"RING", modelPath + L"Ring/", L"Ring", true);

		// �����@���f��
		RegisterMultiMesh(L"BLOWER", modelPath + L"Blower/", L"Blower", true);


		// �T�E���h�f�B���N�g���p�X
		const wstring BGMPath = mediaPath + L"Sounds/BGM/";
		const wstring SEPath = mediaPath + L"Sounds/SE/";

		// �Q�[���pBGM
		app->RegisterWav(L"GAME_BGM", BGMPath + L"GameBGM");

		// �Ռ��gSE
		app->RegisterWav(L"AIRSHOCK_SE", SEPath + L"AirShockSE");

		// ��C����SE
		app->RegisterWav(L"CANNON_SE", SEPath + L"CannonFireSE");

		// �V�[���hSE
		app->RegisterWav(L"SHIELD_C_SE", SEPath + L"ShieldCreateSE");
		app->RegisterWav(L"SHIELD_D_SE", SEPath + L"ShieldDestroySE");
	}

	void Scene::RegisterSingleMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse)
	{
		shared_ptr<MeshResource> modelMesh;
		if (boneUse)
		{
			modelMesh = MeshResource::CreateBoneModelMesh(path, fileName + L".bmf");
		}
		else
		{
			modelMesh = MeshResource::CreateStaticModelMesh(path, fileName + L".bmf");
		}
		const auto& app = App::GetApp();
		app->RegisterResource(registerKey, modelMesh);
	}

	void Scene::RegisterMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse)
	{
		shared_ptr<MultiMeshResource> modelMesh;
		if (boneUse)
		{
			modelMesh = MultiMeshResource::CreateBoneModelMultiMesh(path, fileName + L".bmf");
		}
		else
		{
			modelMesh = MultiMeshResource::CreateStaticModelMultiMesh(path, fileName + L".bmf");
		}
		const auto& app = App::GetApp();
		app->RegisterResource(registerKey, modelMesh);
	}

	void Scene::OnCreate()
	{
		try 
		{
			// ���\�[�X�̓ǂݍ���
			CreateResourses();

			// �N���A����F��ݒ�
			SetClearColor(Col4(160.0f / 256.0f, 216.0f / 256.0f, 239.0f / 256.0f, 1.0));

			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"GameStage");
		}
		catch (...) 
		{
			throw;
		}
	}

	Scene::~Scene(){}

	void Scene::OnEvent(const shared_ptr<Event>& event) 
	{
		if (event->m_MsgStr == L"TitleStage")
		{
			ResetActiveStage<TitleStage>();
		}
		if (event->m_MsgStr == L"GameStage")
		{
			ResetActiveStage<GameStage>();
		}
	}

}
//end basecross
