/*!
@file Cannon.cpp
@brief �X�e�[�W�M�~�b�N�̑�C
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace GimmickAngle;
	using namespace CannonParam;
	using namespace Utility;

	// �������̏���
	void Cannon::OnCreate()
	{
		// �p�����̐������̏��������s
		Gimmick::OnCreate();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(m_fireType >= eFireType::NRotate ? L"R_CANNON" : L"N_CANNON");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"FIRE", 0, 60, false);
		m_ptrDraw->AddAnimation(L"RAPID", 30, 30, false);

		// OBB�R���W�����̐ݒ�
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetMakedSize(1.25f);
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		// �p�[�e�B�N���I�u�W�F�N�g�̐���
		m_particle = GetStage()->AddGameObject<MultiParticle>();

		// �^�O��ݒ�
		AddTag(L"Cannon");
	}

	// ���t���[���X�V����
	void Cannon::OnUpdate()
	{
		// ���˒����A��]���Ă��Ȃ����
		if (m_status(eStatus::IsFire) && !m_status(eStatus::IsRotate))
		{
			// ���ˎ��̃A�j���[�V�������X�V
			FireAnimation();
		}

		// ��]�^�C�v�Ȃ�
		if (m_fireType >= eFireType::NRotate)
		{
			// ��C����]������
			RotateCannon();
		}
	}

	// ���ˎ��̃A�j���[�V�����Đ�����
	void Cannon::FireAnimation()
	{
		// ���݂̃A�j���[�V�����̍Đ����Ԃ��擾
		const float animeTime = m_ptrDraw->GetCurrentAnimationTime();
		const float fireTime = GetFireTime(); // ���ˎ��Ԃ��擾

		// �A�j���[�V�������I����Ă邩�̐^�U
		bool animaEnd = m_ptrDraw->IsTargetAnimeEnd() && m_ptrDraw->GetCurrentAnimation() == GetAnimeKey(m_fireType);

		// �p�[�e�B�N���𐶐����邩�̐^�U
		bool fireEffect = animeTime > fireTime && animeTime < fireTime + m_particleTime;

		// �A�j���[�V�������I�������
		if (animaEnd)
		{
			// ���˒����̐^�U���I�t
			m_status.Set(eStatus::IsFire) = false;
		}

		// �p�[�e�B�N���𐶐�����Ȃ�
		if (fireEffect)
		{
			// �G�t�F�N�g�̍X�V���������s
			EffectUpdate();
		}

		// �A�j���[�V�����̍X�V
		m_ptrDraw->UpdateAnimation(DELTA_TIME * 1.5f);
	}

	// ��C�̉�]����
	void Cannon::RotateCannon()
	{
		// ���˒��ŉ�]���ĂȂ��Ȃ牽�������I��
		if (m_status(eStatus::IsFire) && !m_status(eStatus::IsRotate))
		{
			return;
		}

		// �����˂�����]�Ȃ�
		if (!m_status(eStatus::IsFire, eStatus::IsRotate))
		{
			// 1.5�b�̃^�C�}�[��ݒ�
			if (SetTimer(1.5f))
			{
				m_status.Set(eStatus::IsRotate) = true; // ��]���̐^�U���I���ɂ���
				m_currentRot = GetRotation().z; // ��]�pZ����ێ�
			}
		}

		// ��]���Ȃ�
		if (m_status(eStatus::IsRotate))
		{
			// 0.55�b�̃^�C�}�[��ݒ�
			SetTimer(0.55f);

			// ��]�p��Lerp�Őݒ�
			float time = GetTime(0.55f) * 2.0f; // Lerp�Ɏg������
			float end = m_currentRot - XM_PIDIV2; // �O��̉�]�p����90�x�������p�x
			float rad = Utility::Lerp(m_currentRot, end, time); // Lerp�ŉ�]�p�����߂�
			
			max(rad, end); // �I���p�x�ȏ�Ȃ�C��

			// �p�x�̍X�V
			SetRotation(Vec3(0.0f, 0.0f, rad));

			// ��]�p���I�����Ɠ���Ȃ��]���I������
			if (rad == end) m_status.Set(eStatus::IsRotate) = false;
		}
	}

	// �G�t�F�N�g�̍X�V����
	void Cannon::EffectUpdate()
	{
		// �΂̃G�t�F�N�g�����g�̈ʒu�ɐ���
		const auto stoneParticle = m_particle.lock()->InsertParticle(10);
		stoneParticle->SetEmitterPos(m_position);
		stoneParticle->SetTextureResource(L"STONE_TX");
		stoneParticle->SetMaxTime(1.0f);

		// ���������X�v���C�g��z��Ŏ擾
		vector<ParticleSprite>& stoneSpriteVec = stoneParticle->GetParticleSpriteVec();

		// �X�v���C�g�̐������[�v
		for (auto& sprite : stoneSpriteVec)
		{
			// �ړ������̐ݒ�
			float rad = GetRotation().z - XM_PIDIV2 + DegToRad(RangeRand(30.0f, -30.0f));
			sprite.m_Velocity = -Vec2(cos(rad), sin(rad)).normalize() * 20.0f;

			// �X�P�[���ƃ��[�e�[�V����Z���̐ݒ�
			sprite.m_LocalScale = Vec2(RangeRand(1.0f, 0.1f));
			sprite.m_LocalQt.rotationZ(DegToRad(RangeRand(360.0f, 0.0f)));
		}

		// ���̃G�t�F�N�g�����g�̈ʒu�ɐ���
		const auto fireParticle = m_particle.lock()->InsertParticle(20);
		fireParticle->SetEmitterPos(m_position);
		fireParticle->SetTextureResource(L"FIRE_TX");
		fireParticle->SetMaxTime(2.0f);

		// ���������X�v���C�g��z��Ŏ擾
		vector<ParticleSprite>& fireSpriteVec = fireParticle->GetParticleSpriteVec();

		// �X�v���C�g�̐������[�v
		for (auto& sprite : fireSpriteVec)
		{
			// �ړ�������ݒ�
			float rad = GetRotation().z - XM_PIDIV2 + DegToRad(RangeRand(45.0f, -45.0f));
			sprite.m_Velocity = -Vec2(cos(rad), sin(rad)).normalize() * 10.0f;

			// �X�P�[���ƃ��[�e�[�V����Z���̐ݒ�
			sprite.m_LocalScale = Vec2(RangeRand(5.0f, 2.5f));
			sprite.m_LocalQt.rotationZ(DegToRad(RangeRand(360.0f, 0.0f)));
		}
	}

	// �������̏���
	void GoalCannon::OnCreate()
	{
		// �p�����̐������̏��������s
		Gimmick::OnCreate();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"G_CANNON");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"FIRE", 0, 60, false);
		m_ptrDraw->AddAnimation(L"RAPID", 30, 30, false);

		// �R���W����OBB�R���|�[�l���g�̐ݒ�
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		// �p�[�e�B�N���I�u�W�F�N�g�̐���
		m_particle = GetStage()->AddGameObject<MultiParticle>();

		// �^�O��ݒ�
		AddTag(L"Goal");
	}

	// �G�t�F�N�g�̍X�V����
	void GoalCannon::EffectUpdate()
	{
		// ���̃G�t�F�N�g�����g�̈ʒu�ɐ���
		const auto paperParticle = m_particle.lock()->InsertParticle(100);
		paperParticle->SetEmitterPos(m_position);
		paperParticle->SetTextureResource(L"PAPER_TX");
		paperParticle->SetMaxTime(5.0f);
		paperParticle->SetDrawOption(Particle::Normal);

		// ���������X�v���C�g��z��Ŏ擾
		vector<ParticleSprite>& paperSpriteVec = paperParticle->GetParticleSpriteVec();

		// �X�v���C�g�̐������[�v
		for (auto& sprite : paperSpriteVec)
		{
			// �ړ������̐ݒ�
			float rad = GetRotation().z - XM_PIDIV2 + DegToRad(RangeRand(45.0f, -45.0f));
			sprite.m_Velocity = -Vec2(cos(rad), sin(rad)).normalize() * RangeRand(10.0f, 5.0f);
			
			// �X�P�[���ƃ��[�e�[�V����Z���̐ݒ�
			sprite.m_LocalScale = Vec2(RangeRand(2.0f, 0.5f));
			sprite.m_LocalQt.rotationZ(DegToRad(RangeRand(180.0f, 0.0f)));

			// �F�𗐐��ɉ����Đݒ�
			Col4 color;
			int random = RangeRand(4, 0);
			switch (random)
			{
			case 0:
				color = COL_RED;
				break;

			case 1:
				color = COL_BLUE;
				break;

			case 2:
				color = COL_GREAN;
				break;

			case 3:
				color = COL_YELOW;
				break;

			case 4:
				color = COL_BG;
				break;

			default:
				break;
			}

			sprite.m_Color = color;
		}
	}

	// �������̏���
	void TitleCannon::OnCreate()
	{
		// �`��R���|�[�l���g�̎擾
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();

		// ���b�V���L�[����ʏ��C�ɐݒ�
		wstring meshKey = L"N_CANNON";

		// ��Փx�ɉ����Ă��̓�Փx�p�̑�C�Ƀ��b�V���L�[��ύX
		switch (m_diffType)
		{
		case eDifficulty::Easy: // �C�[�W�[�Ȃ�
			meshKey = L"EASY_CANNON"; // �C�[�W�[�p�̑�C
			m_angle = eAngle::Uleft; // �p�x����������ɐݒ�
			break;

		case eDifficulty::Normal: // �m�[�}���Ȃ�
			meshKey = L"NORMAL_CANNON"; // �m�[�}���p�̑�C
			m_angle = eAngle::Up; // �p�x��������ɐݒ�
			break;

		case eDifficulty::Hard: // �n�[�h�Ȃ�
			meshKey = L"HARD_CANNON"; // �n�[�h�p�̑�C
			m_angle = eAngle::Uright; // �p�x���E������ɐݒ�
			break;

		default:
			break;
		}

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw->SetMeshResource(meshKey);
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"FIRE", 0, 60, false);

		// �p�����̐������̏��������s
		Gimmick::OnCreate();

		// �R���W�����̐ݒ�
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		// �p�[�e�B�N���p�I�u�W�F�N�g�̐���
		m_particle = GetStage()->AddGameObject<MultiParticle>();

		// �^�O��ݒ�
		AddTag(L"Cannon");
	}
}