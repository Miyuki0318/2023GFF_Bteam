/*!
@file CubeObject.cpp
@brief �R���W����OBB���g�p����I�u�W�F�N�g�̌p����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;
	using namespace CubeParam;

	// �������̏���
	void CubeObject::OnCreate()
	{
		// �p�����̐������̊֐������s����
		TemplateObject::OnCreate();

		// ���W�ƃX�P�[���͂��̂܂ܐݒ�
		SetPosition(m_position);
		SetScale(m_scale);

		// ���[�e�[�V�����̓m�[�}���^�C�v�Ȃ�m_rotation���A����ȊO�Ȃ�CubeParam�̃A���O�����Q��
		SetRotation(m_type != eCubeType::Normal ? DegToRad(GetAngle(m_type)) : m_rotation);

		// �R���W�������A�N�e�B�u�Ȃ�
		if (m_collActive)
		{
			// �R���W����OBB�R���|�[�l���g��ǉ�
			m_ptrColl = AddComponent<CollisionObb>();
			m_ptrColl->SetFixed(true);
			m_ptrColl->SetUpdateActive(false);
		}

		// �����F�̕`����\�ɂ���
		SetAlphaActive(true);
	}

	// ���t���[���X�V����
	void CubeObject::OnUpdate()
	{
		// �R���W�����p�t�H�[�}���X�֐������s
		CollisionPerformance(m_collRange);
	}

	// �R���W�����p�t�H�[�}���X
	void CubeObject::CollisionPerformance(const float range)
	{
		// �R���W�������A�N�e�B�u��
		if (m_collActive)
		{
			bool achieve = false; // �������B���������̐^�U

			// �^�[�Q�b�g�z��̕����[�v
			for (const auto& obj : m_targetObj)
			{
				// �^�[�Q�b�g�̎擾�ƃG���[�`�F�b�N
				const auto& target = obj.lock();
				if (!target) continue;

				// �^�[�Q�b�g�̍X�V�������A�N�e�B�u��
				if (target->GetUpdateActive())
				{
					// �^�[�Q�b�g�̃g�����X�t�H�[���R���|�[�l���g���擾���č��W���擾
					const auto& targetTrans = target->GetComponent<Transform>();
					Vec3 targetPos = targetTrans->GetPosition();

					// ���g�ƃ^�[�Q�b�g�̍��W�̋��������߂�
					float length = (targetPos - m_position).length();

					// �R���W�������苗�����߂���Ώ����B���Ƃ��ďI��
					achieve = (length <= range);
					if (achieve) break;
				}
			}

			// ���������ɃR���W�������A�N�e�B�u����A�N�e�B�u���ɂ���
			m_ptrColl->SetUpdateActive(achieve);
		}
	}
}