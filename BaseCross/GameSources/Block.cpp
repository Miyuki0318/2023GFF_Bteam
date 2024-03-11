/*!
@file block.cpp
@brief �u���b�N�̃R���W�����ƕ`��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void Alpha::OnCreate()
	{
		// �p�����̊J�n���̏����𑗂�
		CubeObject::OnCreate();

		// �^�O��ǉ�
		AddTag(L"Block");
	}

	// �������̏���
	void DeathColl::OnCreate()
	{
		// �p�����̊J�n���̏����𑗂�
		CubeObject::OnCreate();

		// �^�O��ǉ�
		AddTag(L"Death");
	}

	// �������̏���
	void StagingColl::OnCreate()
	{
		// �p�����̊J�n���̏����𑗂�
		CubeObject::OnCreate();

		// �^�O��ǉ�
		AddTag(L"Block");
	}

	// ���t���[���X�V����
	void StagingColl::OnUpdate()
	{
		// �X�e�[�W�X�e�[�g���擾
		const auto& stageState = GetStageState<GameStage>();

		// �R���W�������J�n���̉��o�ɉ����ăA�N�e�B�u����؂�ւ���
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(stageState != GameStage::StartMove);
	}

	// �������̏���
	void InstanceBlock::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		ptrDraw->SetMeshResource(L"BLOCK");
		ptrDraw->SetTextureResource(m_textures.at(toSize_t(m_type)));

		// �p�����[�^�̐ݒ�
		const float under = -98.0f;		// �X�e�[�W�̉���
		const float left = -49.0f;		// �X�e�[�W�̍���
		const float scale = 1.0f;		// �X�P�[��
		const float modelScale = 1.35f;	// ���f���̃X�P�[��

		// �z��̃T�C�Y�����[�v
		for (size_t i = 0; i < m_data.size(); i++)
		{
			// ���l��0�ȊO�Ȃ�
			if (m_data.at(i) != 0)
			{
				// ���W�̐ݒ�
				float x = left + i;	// �������烋�[�v���𑫂������l
				float y = under + (m_size - m_rowNum); // ��������s�T�C�Y - �s���𑫂������l

				Mat4x4 matrix, mtxT, mtxR, mtxS; // �s��̐錾
				
				// �N�H�[�^�j�I�����烍�[�e�[�V������ݒ�
				Quat quat;
				quat.rotationRollPitchYawFromVector(Vec3(0.0f, 0.0f, 0.0f));
				mtxR.rotation(quat);
				mtxS.scale(Vec3(modelScale)); // �X�P�[���̐ݒ�
				mtxT.translation(Vec3(x, y, 0.0f)); // ���W�̐ݒ�

				// �s��̐ݒ�ƒǉ�
				matrix = mtxS * mtxR * mtxT;
				ptrDraw->AddMatrix(matrix);
			}
		}
	}

	// �������̏���
	void InstanceSlope::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		ptrDraw->SetMeshResource(L"SLOPE");
		ptrDraw->SetTextureResource(m_textures.at(toSize_t(m_type)));
		
		// �p�����[�^�̐ݒ�
		const float under = -97.5f;	// �X�e�[�W�̉���
		const float left = -49.0f;  // �X�e�[�W�̍���
		const Vec3 slopeScale = Vec3(1.375f); // �X���[�v�̃X�P�[��

		// �z��̃T�C�Y�����[�v
		for (size_t i = 0; i < m_data.size(); i++)
		{
			// ���l��0�ȊO�Ȃ�
			if (m_data.at(i) != 0)
			{
				// ���W�̐ݒ�
				float x = left + i;	// �������烋�[�v���𑫂������l
				float y = under + (m_size - m_rowNum); // ��������s�T�C�Y - �s���𑫂������l

				// �X���[�v�̃^�C�v���擾
				CubeParam::eCubeType slope = static_cast<CubeParam::eCubeType>(m_data.at(i) + 2);

				Mat4x4 matrix, mtxT, mtxR, mtxS; // �s��̐錾
				Quat quat; // �N�H�[�^�j�I���̐錾

				// �N�H�[�^�j�I�����烍�[�e�[�V�����̐ݒ�
				quat.rotationRollPitchYawFromVector(DegToRad(CubeParam::GetAngle(slope)));

				mtxR.rotation(quat); // ���[�e�[�V����
				mtxS.scale(slopeScale); // �X�P�[��
				mtxT.translation(Vec3(x, y, 0.0f) + CubeParam::GetSlopeDiff(slope)); // ���W

				// �s��̐ݒ�ƒǉ�
				matrix = mtxS * mtxR * mtxT;
				ptrDraw->AddMatrix(matrix);
			}
		}
	}
}