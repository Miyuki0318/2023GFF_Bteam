/*!
@file ArrowEffect.cpp
@brief �v���C���[�̃G�A�W�F�b�g�ł̗\���O��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void ArrowEffect::OnCreate()
	{
		// �p�����̐������̏��������s
		TemplateObject::OnCreate();

		// �v���C���[�̃|�C���^�������
		if (m_player.lock())
		{
			// �v���C���[�̍��W�Ɏ��g�̍��W��ݒ�
			SetPosition(m_player.lock()->GetPosition());
			SetScale(1.0f); // �X�P�[���̐ݒ�

			// ���_�f�[�^�̐ݒ�(���{���^���b�V��)
			VertexData tempVertex; // 
 			vector<Vec3> points; // ���W�Q
			points.resize(40); // �ߖڂ�40�ɐݒ肷���
			RibonVerticesIndices(points, tempVertex, Vec3(0.0, 0.0f, 1.0f), 0.25f, 1);

			// �`��R���|�[�l���g�̐ݒ�
			m_ptrDraw = AddComponent<PCTStaticDraw>();
			m_ptrDraw->SetOriginalMeshUse(true);
			m_ptrDraw->CreateOriginalMesh(tempVertex);
			m_ptrDraw->SetTextureResource(L"ARROW_TX");
			m_ptrDraw->SetSamplerState(SamplerState::LinearWrap);
			m_ptrDraw->SetEmissive(COL_WHITE);
			m_ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.5f));

			SetDrawActive(false); // �`����I�t
			SetAlphaActive(true); // �����F�̕`����\��
		}
	}

	// �G�t�F�N�g�̍X�V����
	void ArrowEffect::UpdateEffect(const vector<Vec3>& points)
	{
		// �v���C���[�̃|�C���^�������
		if (m_player.lock())
		{
			// �v���C���[�^�ɃL���X�g
			const auto& player = dynamic_pointer_cast<Player>(m_player.lock());
			if (player)
			{
				// �v���C���[�̍��W�Ɏ��g�̍��W��ݒ�
				SetPosition(player->GetPosition());

				// ���{���^���b�V���̍Đݒ���ł����
				if (RibonVerticesIndices(points, m_vertex, Vec3(0.0, 0.0f, 1.0f), 0.35f, 1))
				{
					SetDrawActive(true); // �`����I��
					m_ptrDraw->UpdateVertices(m_vertex.vertices); // ���_�f�[�^�̍X�V
				}
				else
				{
					SetDrawActive(false); // �`����I�t
				}
			}
		}
	}
}