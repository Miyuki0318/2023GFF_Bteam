/*!
@file BaseStage.h
@brief �X�e�[�W�̌p����
*/

#pragma once
#include "Timer.h"
#include "Sprite.h"
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �X�e�[�W�̌p����
	*/
	class BaseStage : public Stage
	{
	protected:

		// CSV�f�[�^
		vector<vector<string>> m_csvData;

		// �r���[
		shared_ptr<SingleView> m_gameView;

		// �^�C�}�[�I�u�W�F�N�g
		weak_ptr<Timer> m_timer;

		// BGM�ESE
		weak_ptr<SoundItem> m_bgm;
		shared_ptr<SEManager> m_seManager;

		// �w�i�I�u�W�F�N�g
		weak_ptr<TemplateObject> m_backObj;

		/*!
		@brief ���\�[�X�ǂݍ��݊֐�
		*/
		virtual void CreateResourses();

		/*!
		@brief �r���[�ƃ��C�g�̐����֐�
		*/
		virtual void CreateViewLight() = 0;

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
		@brief �I�u�W�F�N�g���A�N�e�B�u�O���[�v�ɒǉ�����֐�
		@param �I�u�W�F�N�g�̃O���[�v�x�N�^�[�z��
		@param �A�N�e�B�u�O���[�v
		@param �v���C���[�̍��W
		@param �X�V�͈�
		*/
		void ObjectInToAvtiveGroup(const vector<weak_ptr<GameObject>>& groupVec, const shared_ptr<GameObjectGroup>& activeGroup, const Vec3& playerPos, float updateRange);

		/*!
		@brief �I�u�W�F�N�g�̃p�t�H�[�}���X�Ǘ��֐�
		@param �I�u�W�F�N�g�̃O���[�v�x�N�^�[�z��
		@param ���W
		@param �X�V�͈�
		*/
		template<class T>
		void ObjectPerformance(const vector<weak_ptr<GameObject>>& groupVec, const Vec3& pos, float updateRange)
		{
			const Vec2 margin = Vec2(25.0f); // �]��
			const Vec3 pLeft = Vec3(WINDOW_SIZE + margin, 1.0f); // ���[
			const Vec3 pRight = Vec3(-WINDOW_SIZE - margin, 0.0f); // �E�[

			// �I�u�W�F�N�g�̐����[�v
			for (const auto& weakObj : groupVec)
			{
				// �G���[�`�F�b�N
				if (!weakObj.lock()) continue;

				// �^�L���X�g
				const auto& sharedObj = dynamic_pointer_cast<T>(weakObj.lock());
				if (!sharedObj) continue;

				// ���W�ƃI�u�W�F�N�g�̍��W�̋������X�V�͈͓��ł��邩
				const Vec3& objPos = sharedObj->GetPosition();
				float length = (objPos - pos).length();

				// �͈͓����ōX�V�������A�N�e�B�u�ɂ��邩�ݒ�
				sharedObj->SetUpdateActive(length <= updateRange);

				// 2D���W�ɕϊ����A��ʓ��ɃI�u�W�F�N�g������Ε`����s��
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
			// �O���[�v����I�u�W�F�N�g�z����擾
			const auto& grouoVec = group->GetGroupVector();

			// �I�u�W�F�N�g�̐����[�v
			for (auto& weakObj : grouoVec)
			{
				// �G���[�`�F�b�N
				if (!weakObj.lock()) continue;

				// �X�e�[�W���ɔj�����߂𑗂�
				RemoveGameObject<GameObject>(weakObj.lock());

				// �O���[�v���珜�O����
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
		@param BGM�L�[
		@param ����
		*/
		virtual void CreateBGM(const wstring& bgmKey, float volume);

		/*!
		@brief SE�}�l�[�W���[�̐����֐�
		*/
		virtual void CreateSEManager();

		/*!
		@brief SE�̍Đ��֐�
		@param SE�L�[
		@param ����
		*/
		virtual void CreateSE(const wstring& seKey, float volume);

		/*!
		@brief SE�̍Đ��֐�
		@param SE�L�[
		@param ����
		@param �I�u�W�F�N�g�̃|�C���^
		*/
		virtual void CreateSE(const wstring& seKey, float volume, const void* objPtr);

		/*!
		@brief SE�̒�~�֐�
		@param SE�L�[
		*/
		virtual void StopSE(const wstring& seKey);

		/*!
		@brief SE�̒�~�֐�
		@param SE�L�[
		@param �I�u�W�F�N�g�̃|�C���^
		*/
		virtual void StopSE(const wstring& seKey, const void* objPtr);

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
		const shared_ptr<Timer> GetTimer() const
		{
			return m_timer.lock();
		}
	};
}