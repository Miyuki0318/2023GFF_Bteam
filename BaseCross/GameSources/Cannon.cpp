#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void Cannon::OnCreate()
	{
		Gimmick::OnCreate();
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"CANNON");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"FIRE", 0, 60, false);
		m_ptrDraw->AddAnimation(L"RAPID", 30, 30, false);

		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		m_particle = GetStage()->AddGameObject<MultiParticle>();

		SetAlphaActive(true);

		AddTag(L"Cannon");
	}

	void Cannon::OnUpdate()
	{
		float deltaTime = DELTA_TIME * 1.5f;

		if (m_fireType >= eFireType::NRotate)
		{
			Vec3 rot = GetRotation();
			rot.z -= deltaTime;
			SetRotation(rot);
		}

		if (m_isFire)
		{
			bool animaEnd = m_ptrDraw->IsTargetAnimeEnd() && m_ptrDraw->GetCurrentAnimation() == m_animeKey.at(m_select);
			bool fireEffect = m_ptrDraw->GetCurrentAnimationTime() > GetFireTime() && m_ptrDraw->GetCurrentAnimationTime() < GetFireTime() + m_particleTime;;

			if (animaEnd)
			{
				m_isFire = false;
			}
			if (fireEffect)
			{
				EffectUpdate();
			}

			m_ptrDraw->UpdateAnimation(deltaTime);
		}
	}

	void Cannon::EffectUpdate()
	{
		// 石のエフェクトを自身の位置に生成
		const auto stoneParticle = m_particle.lock()->InsertParticle(10);
		stoneParticle->SetEmitterPos(m_position);
		stoneParticle->SetTextureResource(L"STONE_TX");
		stoneParticle->SetMaxTime(1.0f);

		// 生成したスプライトを配列で取得
		vector<ParticleSprite>& stoneSpriteVec = stoneParticle->GetParticleSpriteVec();

		// スプライトの数分ループ
		for (auto& sprite : stoneSpriteVec)
		{
			float rad = GetRotation().z - XM_PIDIV2 + Utility::DegToRad(Utility::RangeRand(60.0f, 0.0f) - 30.0f);
			sprite.m_Velocity = -Vec2(cos(rad), sin(rad)).normalize() * 20.0f;

			sprite.m_LocalScale = Vec2(Utility::RangeRand(1.0f, 0.1f));
			sprite.m_LocalQt.rotationZ(Utility::DegToRad(Utility::RangeRand(360.0f, 0.0f)));
		}

		// 炎のエフェクトを自身の位置に生成
		const auto fireParticle = m_particle.lock()->InsertParticle(20);
		fireParticle->SetEmitterPos(m_position);
		fireParticle->SetTextureResource(L"FIRE_TX");
		fireParticle->SetMaxTime(2.0f);

		// 生成したスプライトを配列で取得
		vector<ParticleSprite>& fireSpriteVec = fireParticle->GetParticleSpriteVec();

		// スプライトの数分ループ
		for (auto& sprite : fireSpriteVec)
		{
			float rad = GetRotation().z - XM_PIDIV2 + Utility::DegToRad(Utility::RangeRand(90.0f, 0.0f) - 45.0f);
			sprite.m_Velocity = -Vec2(cos(rad), sin(rad)).normalize() * 10.0f;

			sprite.m_LocalScale = Vec2(Utility::RangeRand(5.0f, 2.5f));
			sprite.m_LocalQt.rotationZ(Utility::DegToRad(Utility::RangeRand(360.0f, 0.0f)));
		}
	}
}	