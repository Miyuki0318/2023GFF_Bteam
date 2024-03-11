/*!
@file GameCamera.cpp
@brief �Q�[�����J����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void GameCamera::OnCreate()
	{
		// ���_�ƒ����_�̍��W��ݒ�
		SetEye(m_startEye);
		SetAt(m_startAt);
	}

	// ���t���[���X�V����
	void GameCamera::OnUpdate()
	{
		Vec3 targetPos; // �ۑ��p�ϐ�

		// �^�[�Q�b�g�̃|�C���^�����邩
		if (m_target.lock())
		{
			// �^�[�Q�b�g�̍��W���擾���ĕێ�
			auto targetTrans = m_target.lock()->GetComponent<Transform>();
			targetPos = targetTrans->GetPosition();
			m_targetPos = targetPos;
		}
		else
		{
			// �O��܂ł̃^�[�Q�b�g�̍��W����
			targetPos = m_targetPos;
		}

		// �p�����[�^�̐ݒ�
		const float underDiff = 1.5f; // Y�����W�̍���
		const float followDistance = 0.2f; // �ǔ�����
		const float speed = 4.0f; // �ǔ����x
		const Vec3 atDiff = Vec3(10.0f, 7.5f, 0.0f); // �����_����̍���
		bool down = (targetPos.y <= -87.5f); // �������̐^�U
		bool left = (targetPos.x <= -30.5f); // �������̐^�U

		targetPos.y -= underDiff;		// �^�[�Q�b�g�̍��W���獷��������
		if (down) targetPos.y = -87.5f; // �����Ȃ牺�����W�ɒu������
		if (left) targetPos.x = -30.5f; // �����Ȃ獶����W�ɒu������

		// ���W�̐ݒ�Ƌ����̎擾
		Vec3 vec = targetPos - (m_atPos - atDiff);
		float distance = vec.length();

		// �ǔ��������傫��������ǔ�����
		if (distance > followDistance)
		{
			// �ړ��ʂ�ݒ�(�����̍��� �~ ���x)
			float move = (distance - followDistance) * (DELTA_TIME * speed);

			// �ړ��ʕ������Z
			m_eyePos += Vec3(vec.normalize().x * move, vec.normalize().y * move, 0.0f);
			m_atPos += Vec3(vec.normalize().x * move, vec.normalize().y * move, 0.0f);
		}

		// ���_�ƒ����_�̍��W��ݒ�
		SetEye(m_eyePos);
		SetAt(m_atPos);
	}
}