/*!
@file Gimmick.cpp
@brief �X�e�[�W�M�~�b�N�̌p����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void Gimmick::OnCreate()
	{
		// �p�����̐������̊֐������s����
		CubeObject::OnCreate();

		// ���[�e�[�V�����̐ݒ�
		SetRotation(Utility::DegToRad(GimmickAngle::GetAngle(m_angle)));

		// �^�O��ݒ�
		AddTag(L"Gimmick");
	}
}