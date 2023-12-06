
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

		// ���f���f�B���N�g���p�X
		const wstring modelPath = mediaPath + L"Models/";

		// �u���b�N���f��
		RegisterSingleMesh(L"BLOCK", modelPath + L"Block/", L"Block", false);
		RegisterSingleMesh(L"SLOPE", modelPath + L"Block/", L"Slope", false);
		app->RegisterTexture(L"GRASS_TX", modelPath + L"Block/Grass.png");
		app->RegisterTexture(L"DIRT_TX", modelPath + L"Block/Dirt.png");
		app->RegisterTexture(L"ROCK_TX", modelPath + L"Block/Rock.png");

		// �X�p�C�N���f��
		RegisterMultiMesh(L"SPIKE", modelPath, L"Spike", true);
		RegisterMultiMesh(L"SPIKE_BLOCK", modelPath, L"SpikeBlock", true);

		// ���{�b�g���f��
		RegisterSingleMesh(L"ROBOT_BODY", modelPath + L"Robot/", L"Robot_Body", true);
		RegisterMultiMesh(L"ROBOT_ARM", modelPath + L"Robot/", L"Robot_Arm", true);

		// �G���f��
		RegisterSingleMesh(L"BIRD", modelPath + L"Bird/", L"Bird", true);
		RegisterMultiMesh(L"RABBIT", modelPath + L"Rabbit/", L"rabbitEnemy", true);

		// ��C���f��
		RegisterSingleMesh(L"N_CANNON", modelPath + L"Cannon/", L"Cannon", true);
		RegisterSingleMesh(L"G_CANNON", modelPath + L"Cannon/", L"GoalCannon", true);

		// �x���g�R���x�A���f��
		RegisterMultiMesh(L"MCONVAYOR", modelPath + L"Convayor/", L"m_Convayor", true);
		RegisterMultiMesh(L"SCONVAYOR", modelPath + L"Convayor/", L"s_Convayor", true);

		// �����O���f��
		RegisterSingleMesh(L"RING", modelPath + L"Ring/", L"Ring", true);

		// �����@���f��
		RegisterMultiMesh(L"BLOWER", modelPath + L"Blower/", L"Blower", true);
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
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"TitleStage");
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
