/*!
@file Blower.cpp
@brief �X�e�[�W�M�~�b�N�̑����@
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;
	using namespace GimmickAngle;

	// �������̏���
	void Blower::OnCreate()
	{
		// �p�����̐������̊֐������s����
		Gimmick::OnCreate();
		
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMultiMeshResource(L"BLOWER");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");

		// �^�O�̐ݒ�
		AddTag(L"Blower");
	}

	// ���t���[���X�V����
	void Blower::OnUpdate()
	{
		// �A�j���[�V�����̍X�V
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// �^�[�Q�b�g���͈͓��ɋ��邩�̎擾�֐�
	bool Blower::CheckBetween(const Vec3& targetPos) const
	{
		// ���W�ƃX�P�[���̎擾
		Vec3 pos = GetPosition();
		Vec3 scale = GetScale();
		Vec3 helf = GetScale() / 2.0f;

		Vec3 left, right; // �͈͑���p�̕ϐ�

		// �p�x�ɉ����Ĕ͈͂����߂�
		switch (m_angle)
		{
		case eAngle::Up: // ������Ȃ�
			left = Vec3(pos.x - scale.x, pos.y - helf.y, 0.0f); // ���[�̍��W��ݒ�
			right = Vec3(pos.x + scale.x, pos.y + helf.y + m_range, 0.0f); // �E�[�̍��W��ݒ�
			break;
		
		case eAngle::Down: // �������Ȃ�
			left = Vec3(pos.x - scale.x, pos.y - helf.y - m_range, 0.0f); // ���[�̍��W��ݒ�
			right = Vec3(pos.x + scale.x, pos.y + helf.y, 0.0f); // �E�[�̍��W��ݒ�
			break;
		
		case eAngle::Left: // �������Ȃ�
			left = Vec3(pos.x - helf.x - m_range, pos.y - scale.y, 0.0f); // ���[�̍��W��ݒ�
			right = Vec3(pos.x + helf.x, pos.y + scale.y, 0.0f); // �E�[�̍��W��ݒ�
			break;
		
		case eAngle::Right: // �E�����Ȃ�
			left = Vec3(pos.x - helf.x, pos.y - scale.y, 0.0f); // ���[�̍��W��ݒ�
			right = Vec3(pos.x + helf.x + m_range, pos.y + scale.y, 0.0f); // �E�[�̍��W��ݒ�
			break;

		default:
			break;
		}

		// �E�[�ƍ��[����^�[�Q�b�g���͈͓��ɋ��邩��^�U
		return GetBetween(targetPos, left, right);
	}
}