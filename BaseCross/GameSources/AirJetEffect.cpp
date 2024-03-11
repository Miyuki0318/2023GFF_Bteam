/*!
@file AirJetEffect.cpp
@brief �v���C���[�̃G�A�W�F�b�g�G�t�F�N�g
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void AirJetEffect::OnCreate()
	{
		// �p�����̐������̏��������s
		Billboard::OnCreate();

		// �`��D��x�̐ݒ�
		SetDrawLayer(3);
	}

	// �G�t�F�N�g�̍X�V����
	void AirJetEffect::UpdateEffect()
	{
		// �v���C���[�̃|�C���^�������
		if (m_player.lock())
		{
			// �v���C���[�^�Ɍ^�L���X�g
			const auto& player = dynamic_pointer_cast<Player>(m_player.lock());
			if (player)
			{
				const float& acsel = player->GetAcsel();  // �����x�̎擾
				const Vec2& velo = player->GetVelocity(); // �ړ��ʂ̎擾

				// �v���C���[�̍��W����ړ��ʂ̕����~�����x�ō��W��ݒ�
				Vec3 pos = player->GetPosition() + (Vec3(velo.x, velo.y, 0.0f).normalize() * (acsel * 1.15f));
				
				// �v���C���[���f���ւ̂߂荞�݂�h�~
				pos.y += 0.5f;
				pos.z = -1.0f;

				// ���W�ƃ��[�e�[�V�����ƃX�P�[���̍X�V
				SetPosition(pos);
				SetRotation(0.0f, 0.0f, (atan2f(velo.y, velo.x) - XM_PIDIV2));
				SetScale(Vec2((acsel - 1.0f) * 3.0f));
			}
		}
	}
}