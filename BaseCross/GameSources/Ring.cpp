/*!
@file Ring.cpp
@brief �X�e�[�W�M�~�b�N�̃����O
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void Ring::OnCreate()
	{
		// �T�C�Y�{���̐ݒ�
		float size = 1.0f;
		if (m_ringSize == eRingSize::Big) size = 5.0f;
		m_scale *= size;

		// �p�����̐������̏��������s
		Gimmick::OnCreate();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"RING");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 30, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");
		m_ptrDraw->SetEmissive(COL_WHITE);

		// �R���W�����R���|�[�l���g�̐ݒ�
		m_ptrColl->SetMakedSize(m_ringSize == eRingSize::Big ? 1.0f : 1.75f);
		m_ptrColl->SetAfterCollision(AfterCollision::None);

		// �^�O��ݒ�
		AddTag(L"Ring");
	}

	// ���t���[���X�V����
	void Ring::OnUpdate()
	{
		// �f���^�^�C���Ǝ擾���̃A�j���[�V�����X�P�[���̍ő�l��ݒ�
		const float& deltaTime = DELTA_TIME;
		const float maxSize = m_ringSize == eRingSize::Big ? 7.5f : 1.5f;

		// ���[�e�[�V����Z���ŉ�]
		Vec3 rot = GetRotation();
		rot.y -= deltaTime * m_rotSpeed;
		SetRotation(rot);

		// �A�j���[�V�����̍X�V����
		m_ptrDraw->UpdateAnimation(deltaTime);

		// �擾�����Ȃ�
		if (m_isGet)
		{
			// �o�ߎ��Ԃ��f���^�^�C���ŉ��Z
			m_totalTime += deltaTime;

			// �o�ߎ��Ԃƍ폜���Ԃ̊���
			float ratio = m_totalTime / m_deleteTime;

			// �X�P�[�����T�C���J�[�u�Őݒ�
			SetScale(SinCurve(ratio * DegToRad(235.0f), -2.5f, maxSize));

			// �f�B�t���[�Y�F��Lerp�Őݒ�
			m_ptrDraw->SetDiffuse(Utility::Lerp(COL_WHITE, COL_ALPHA, ratio));

			// �o�ߎ��Ԃ��폜���Ԏ��Ԉȏ�ɂȂ�����
			if (m_deleteTime <= m_totalTime)
			{
				// �X�e�[�W�Ǝ��g�̃|�C���^���擾
				const auto& stage = GetStage();
				const auto& thisPtr = GetThis<Ring>();

				// ���W���O���[�v���珜�O���ăI�u�W�F�N�g��j��
				stage->GetSharedObjectGroup(L"Collect")->OutoGroup(thisPtr);
				stage->RemoveGameObject<Ring>(thisPtr);
			}
		}
		else
		{
			// �R���W�����̃p�t�H�[�}���X�֐������s
			CollisionPerformance(maxSize);
		}
	}
}