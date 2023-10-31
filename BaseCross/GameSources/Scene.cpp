
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
		app->RegisterTexture(L"WHITE", texturePath + L"White.png");


		// ���f���f�B���N�g���p�X
		const wstring modelPath = mediaPath + L"Models/";

		// ���u���b�N���f��
		RegisterSingleMesh(L"GRASS", modelPath + L"Grass/", L"Grass", false);

		// �y�u���b�N���f��
		RegisterSingleMesh(L"DIRT", modelPath + L"Dirt/", L"Dirt", false);

		// ��u���b�N���f��
		RegisterSingleMesh(L"ROCK", modelPath + L"Rock/", L"Rock", false);

		// ����u���b�N���f��
		RegisterSingleMesh(L"SANDSTONE", modelPath + L"SandStone/", L"Sandstone", false);

		// �X�p�C�N���f��
		RegisterSingleMesh(L"SPIKE", modelPath, L"Spike", false);

		// ���{�b�g���f��
		RegisterSingleMesh(L"ROBOT_BODY", modelPath + L"Robot/", L"Robot_Body", true);
		RegisterMultiMesh(L"ROBOT_ARM", modelPath + L"Robot/", L"Robot_Arm", true);

		// �����f��
		RegisterSingleMesh(L"BIRD", modelPath + L"Bird/", L"Bird", true);
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
		if (event->m_MsgStr == L"GameStage")
		{
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
	}

}
//end basecross
