/*!
@file CubeObject.h
@brief �R���W����OBB���g�p����I�u�W�F�N�g�̌p����
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �L���[�u�p�����[�^�Q
	*/
	namespace CubeParam
	{
		//// C++17�Ȃ�\���̂̃����o�ϐ��Ƃ��Đ錾
		//// �p�x�z��
		//static inline const array<Vec3, 7> m_angles = {
		//	Vec3(0.0f),					// �ʏ�
		//	Vec3(0.0f, 0.0f, 45.0f),	// ��
		//	Vec3(0.0f, 0.0f, -45.0f),	// �E
		//	Vec3(0.0f, 0.0f, 45.0f),	// �X���[�v����
		//	Vec3(0.0f, 180.0f, 45.0f),	// �X���[�v�E��
		//	Vec3(0.0f, 0.0f, 135.0f),	// �X���[�v����
		//	Vec3(0.0f, 180.0f, 135.0f)	// �X���[�v�E��
		//};

		//// �X���[�v�̍��W����
		//static inline const array<Vec3, 4> m_slopeDiff = {
		//	Vec3(0.5f, -0.5f, 0.0f),	// �X���[�v����
		//	Vec3(-0.5f, -0.5f, 0.0f),	// �X���[�v�E��
		//	Vec3(0.5f, 0.5f, 0.0f),		// �X���[�v����
		//	Vec3(-0.5f, 0.5f, 0.0f)		// �X���[�v�E��
		//};

		// �L���[�u�^�C�venum
		enum class eCubeType : int8_t
		{
			Normal,		// �ʏ�
			Left,		// ��
			Right,		// �E
			SlopeUL,	// �X���[�v����
			SlopeUR,	// �X���[�v�E��
			SlopeDL,	// �X���[�v����
			SlopeDR,	// �X���[�v�E��
		};

		/*!
		@brief ���[�e�[�V�����擾�֐�
		@param eCubeType
		@return ���[�e�[�V����
		*/
		static const Vec3 GetAngle(const eCubeType& type)
		{
			switch (type)
			{
			case eCubeType::Left:
				return Vec3(0.0f, 0.0f, 45.0f);	// ���̃��[�e�[�V������Ԃ�
				break;

			case eCubeType::Right:
				return Vec3(0.0f, 0.0f, -45.0f); // �E�̃��[�e�[�V������Ԃ�
				break;

			case eCubeType::SlopeUL:
				return Vec3(0.0f, 0.0f, 45.0f); // �X���[�v����̃��[�e�[�V������Ԃ�
				break;

			case eCubeType::SlopeUR:
				return Vec3(0.0f, 180.0f, 45.0f); // �X���[�v�E��̃��[�e�[�V������Ԃ�
				break;

			case eCubeType::SlopeDL:
				return Vec3(0.0f, 0.0f, 135.0f); // �X���[�v�����̃��[�e�[�V������Ԃ�
				break;

			case eCubeType::SlopeDR:
				return Vec3(0.0f, 180.0f, 135.0f); // �X���[�v�E���̃��[�e�[�V������Ԃ�
				break;

			default:
				return Vec3(0.0f);
				break;
			}
		}

		/*!
		@brief �X���[�v�̍��W�����擾�֐�
		@param eCubeType
		@return m_slopeDiff
		*/
		static const Vec3 GetSlopeDiff(const eCubeType& type)
		{
			switch (type)
			{
			case eCubeType::SlopeUL:
				return Vec3(0.5f, -0.5f, 0.0f); // �X���[�v����̍��W������Ԃ�
				break;

			case eCubeType::SlopeUR:
				return Vec3(-0.5f, -0.5f, 0.0f); // �X���[�v�E��̍��W������Ԃ�
				break;

			case eCubeType::SlopeDL:
				return Vec3(0.5f, 0.55f, 0.0f); // �X���[�v�����̍��W������Ԃ�
				break;

			case eCubeType::SlopeDR:
				return Vec3(-0.5f, 0.55f, 0.0f); // �X���[�v�E���̍��W������Ԃ�
				break;

			default:

				// �G���[���b�Z�[�W�𑗂�
				throw BaseException(
					L"�X���[�v�ȊO��enum�񋓎q���w�肵�Ă��܂�",
					L"�����ɓ����eCubeType�̗񋓎q���m�F���Ă�������",
					L"CubeObject.h/GetSlopeDiff(const eCubeType& type)"
				);
				break;
			}
		}
	};

	/*!
	@brief �L���[�u�I�u�W�F�N�g(�R���W����OBB���g���I�u�W�F�N�g�̌p����)
	*/
	class CubeObject : public TemplateObject
	{
	protected:

		CubeParam::eCubeType m_type;				// �L���[�u�^�C�v
		const float m_collRange;					// �R���W�����A�N�e�B�u����
		const bool m_collActive;					// �R���W�������A�N�e�B�u�ɂ��邩
		shared_ptr<CollisionObb> m_ptrColl;			// �R���W����OBB�R���|�[�l���g
		vector<weak_ptr<GameObject>> m_targetObj;	// �R���W������������^�[�Q�b�g�z��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		CubeObject(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_collRange(2.0f),
			m_collActive(true)
		{
			m_type = CubeParam::eCubeType::Normal;
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param ���[�e�[�V����
		@param �X�P�[��
		@param �R���W�����A�N�e�B�u
		*/
		CubeObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& rotation, const Vec3& scale, const bool active
		) :
			TemplateObject(stagePtr, position, rotation, scale),
			m_collRange(2.0f),
			m_collActive(active)
		{
			m_type = CubeParam::eCubeType::Normal;
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		@param �X�P�[��
		@param �L���[�u�^�C�v
		@param �R���W�����A�N�e�B�u
		*/
		CubeObject(const shared_ptr<Stage>& stagePtr,
			const Vec3& position, const Vec3& scale, const CubeParam::eCubeType& type, const bool active
		) :
			TemplateObject(stagePtr, position, Vec3(0.0f), scale),
			m_type(type),
			m_collRange(2.0f),
			m_collActive(active)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~CubeObject() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �R���W�����̃p�t�H�[�}���X�Ǘ��֐�
		@param �R���W�������苗��
		*/
		void CollisionPerformance(const float range);

		/*!
		@brief �R���W�����A�N�e�B�u�ݒ�֐�
		@param �A�N�e�B�u���̐^�U
		*/
		void SetCollActive(bool b)
		{
			m_ptrColl->SetUpdateActive(b);
		}

		/*!
		@brief �L���[�u�^�C�v�擾�֐�
		@return m_type
		*/
		const CubeParam::eCubeType& GetCubeType() const
		{
			return m_type;
		}

		/*!
		@brief �R���W��������^�[�Q�b�g�ǉ��֐�
		@param �^�[�Q�b�g�̃|�C���^
		*/
		void AddTarget(const shared_ptr<GameObject>& objPtr)
		{
			m_targetObj.push_back(objPtr);
		}

		/*!
		@brief �R���W��������^�[�Q�b�g�ǉ��֐�
		@param �^�[�Q�b�g�̃|�C���^�z��
		*/
		void AddTarget(const vector<weak_ptr<GameObject>>& objVec)
		{
			for (const auto& ptr : objVec)
			{
				m_targetObj.push_back(ptr.lock());
			}
		}

		/*!
		@brief �R���W��������^�[�Q�b�g�z�񃊃Z�b�g�֐�
		*/
		void RemoveTarget()
		{
			m_targetObj.clear();
		}

		/*!
		@brief �R���W��������^�[�Q�b�g�z��擾��
		@return m_targetObj
		*/
		const vector<weak_ptr<GameObject>>& GetTargetVec() const
		{
			return m_targetObj;
		}

		/*!
		@brief �X���[�v�̍��W�擾�֐�
		@return 
		*/
		const Vec3 GetSlopePos() const
		{
			// �L���[�u�^�C�v���X���[�v�Ȃ̂�
			if (Utility::GetBetween(m_type, CubeParam::eCubeType::SlopeUL, CubeParam::eCubeType::SlopeDR))
			{
				// �X���[�v�̍��W�������擾
				Vec3 slope = CubeParam::GetSlopeDiff(m_type);

				// ���݂̍��W�ɃX���[�v�̍��W���������Z���ĕԂ�
				return (GetPosition() + slope);
			}
			else
			{
				// �X���[�v�ȊO�Ȃ猻�݂̍��W�����̂܂ܕԂ�
				return GetPosition();
			}
		}
	};
}