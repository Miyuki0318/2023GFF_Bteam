/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase
	{
	public:

		enum eGameStage
		{
			None,
			Easy,
			Normal,
			Hard,
		};

	private:

		eGameStage m_gameStage;
		string m_currentStage;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() : SceneBase() 
		{
			m_gameStage = None;
			m_currentStage = "Tutorial";
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X�̓ǂݍ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void CreateResourses();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���O�����f�����b�V���̓o�^
		@param[in]  �o�^����L�[
		@param[in]	�f�B���N�g���p�X
		@param[in]	�t�@�C���l�[��
		@param[in]  �{�[�������邩�̐^�U
		*/
		virtual void RegisterSingleMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���f�����b�V���̓o�^
		@param[in]  �o�^����L�[
		@param[in]	�f�B���N�g���p�X
		@param[in]	�t�@�C���l�[��
		@param[in]  �{�[�������邩�̐^�U
		*/
		virtual void RegisterMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃X�e�[�W�l�[���̕ێ�
		@param[in]  ���݂̃X�e�[�W�l�[��
		*/
		void SetCurrentStage(const string& stageName)
		{
			m_currentStage = stageName;
			if (stageName == "Easy") m_gameStage = Easy;
			if (stageName == "Normal") m_gameStage = Normal;
			if (stageName == "Hard") m_gameStage = Hard;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�O��̃X�e�[�W�l�[���̎擾
		@return  �O��̃X�e�[�W�l�[��
		*/
		const string& GetCurrentStage() const
		{
			return m_currentStage;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���̃X�e�[�W�̎擾
		@return  ���̃X�e�[�W��
		*/
		const string GetNextStage() const
		{
			string stageName = "Easy";

			switch (m_gameStage)
			{
			case Scene::Easy:
				stageName = "Normal";
				break;

			case Scene::Normal:
				stageName = "Hard";
				break;

			default:
				break;
			}

			return stageName;
		}
	};
}