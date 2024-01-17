#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Timer.h"
#include "TemplateObject.h"

namespace basecross
{
	class BaseStage : public Stage
	{
	protected:

		// SE�Ǘ��\����
		struct SE
		{
			weak_ptr<SoundItem> item; // �T�E���h�A�C�e��
			wstring seKey = L""; // �t�@�C���L�[

			// �R���X�g���N�^
			SE(const weak_ptr<SoundItem>& ptr, const wstring& key
			) :
				item(ptr),
				seKey(key)
			{
			}

			// �R���X�g���N�^
			SE(const wstring& key, const weak_ptr<SoundItem>& ptr
			) :
				item(ptr),
				seKey(key)
			{
			}

		public:

			// ���Z�b�g�֐�
			void Reset()
			{
				item.reset();
				seKey = L"";
			}
		};

		// CSV�f�[�^
		vector<vector<string>> m_csvData;

		// �r���[
		shared_ptr<SingleView> m_gameView;

		// �^�C�}�[�I�u�W�F�N�g
		shared_ptr<Timer> m_timer;

		// BGM�ESE
		weak_ptr<SoundItem> m_bgm;
		vector<SE> m_seList;

		// �w�i�I�u�W�F�N�g
		weak_ptr<TemplateObject> m_backObj;

		/*!
		@brief ���\�[�X�ǂݍ��݊֐�
		*/
		virtual void CreateResourses();

		/*!
		@brief �r���[�ƃ��C�g�̐����֐�
		*/
		virtual void CreateViewLight();

		/*!
		@brief �v���C���[�̐����֐�
		*/
		virtual void CreatePlayer();

		/*!
		@brief �X�e�[�W�̐����֐�
		@param �t�@�C���l�[��
		*/
		void CreateStage(const string& fileName);
		void CreateEnemy(const string& fileName);
		void CreateInstanceBlock(const string& fileName);

		/*!
		@brief �I�u�W�F�N�g�̃A�N�e�B�u�O���[�v�ɒǉ�����֐�
		@param �I�u�W�F�N�g�̃O���[�v�x�N�^�[�z��
		@param �v���C���[�̍��W
		@param �X�V�͈�
		*/
		void ObjectInToAvtiveGroup(const vector<weak_ptr<GameObject>>& groupVec, const shared_ptr<GameObjectGroup>& activeGroup, const Vec3& playerPos, float updateRange);

		/*!
		@brief �I�u�W�F�N�g�̃p�t�H�[�}���X�Ǘ��֐�
		@param �I�u�W�F�N�g�̃O���[�v�x�N�^�[�z��
		@param �X�V�͈�
		*/
		template<class T>
		void ObjectPerformance(const vector<weak_ptr<GameObject>>& groupVec, const Vec3& pos, float updateRange)
		{
			const Vec2 margin = Vec2(25.0f);
			const Vec3 pLeft = Vec3(WINDOW_SIZE + margin, 1.0f);
			const Vec3 pRight = Vec3(-WINDOW_SIZE - margin, 0.0f);

			for (const auto& weakObj : groupVec)
			{
				if (!weakObj.lock()) continue;

				const auto& sharedObj = dynamic_pointer_cast<T>(weakObj.lock());
				if (!sharedObj) continue;

				const Vec3& objPos = sharedObj->GetPosition();
				float length = (objPos - pos).length();
				sharedObj->SetUpdateActive(length <= updateRange);

				Vec3 point = Utility::ConvertToWorldPosition(m_gameView, objPos);
				sharedObj->SetDrawActive(Utility::GetBetween(point, pLeft, pRight));
			}
		}

		/*!
		@brief �I�u�W�F�N�g�O���[�v�̒��g������ۂɂ���֐�
		@param �I�u�W�F�N�g�̃O���[�v
		*/
		void GroupObjectRemove(const shared_ptr<GameObjectGroup>& group)
		{
			const auto& grouoVec = group->GetGroupVector();
			for (auto& weakObj : grouoVec)
			{
				if (!weakObj.lock()) continue;

				RemoveGameObject<GameObject>(weakObj.lock());
				group->OutoGroup(weakObj.lock());
			}
		}

	public:

		/*!
		@brief �R���X�g���N�^
		*/
		BaseStage() :Stage() {}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~BaseStage() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnDestroy() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief �`��X�V�֐�
		*/
		virtual void OnDraw() override;

		/*!
		@brief BGM�̍Đ��֐�
		*/
		virtual void CreateBGM(const wstring& bgmKey, float volume);

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void CreateSE(const wstring& seKey, float volume);

		/*!
		@brief SE�̍Đ��֐�
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief �Q�[���r���[�擾�֐�
		@return const shared_ptr<SingleView>
		*/
		const shared_ptr<SingleView>& GetGameView() const
		{
			return m_gameView;
		}

		/*!
		@brief �^�C�}�[�N���X�擾�֐�
		@return const shared_ptr<Timer>
		*/
		const shared_ptr<Timer>& GetTimer() const
		{
			return m_timer;
		}
	};
}