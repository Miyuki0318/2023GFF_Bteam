/*!
@file Cannon.cpp
@brief ステージギミックの大砲
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace GimmickAngle;
	using namespace CannonParam;
	using namespace Utility;

	// 生成時の処理
	void Cannon::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Gimmick::OnCreate();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(m_fireType >= eFireType::NRotate ? L"R_CANNON" : L"N_CANNON");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"FIRE", 0, 60, false);
		m_ptrDraw->AddAnimation(L"RAPID", 30, 30, false);

		// OBBコリジョンの設定
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetMakedSize(1.25f);
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		// パーティクルオブジェクトの生成
		m_particle = GetStage()->AddGameObject<MultiParticle>();

		// タグを設定
		AddTag(L"Cannon");
	}

	// 毎フレーム更新処理
	void Cannon::OnUpdate()
	{
		// 発射中かつ、回転していなければ
		if (m_status(eStatus::IsFire) && !m_status(eStatus::IsRotate))
		{
			// 発射時のアニメーションを更新
			FireAnimation();
		}

		// 回転タイプなら
		if (m_fireType >= eFireType::NRotate)
		{
			// 大砲を回転させる
			RotateCannon();
		}
	}

	// 発射時のアニメーション再生処理
	void Cannon::FireAnimation()
	{
		// 現在のアニメーションの再生時間を取得
		const float animeTime = m_ptrDraw->GetCurrentAnimationTime();
		const float fireTime = GetFireTime(); // 発射時間を取得

		// アニメーションが終わってるかの真偽
		bool animaEnd = m_ptrDraw->IsTargetAnimeEnd() && m_ptrDraw->GetCurrentAnimation() == GetAnimeKey(m_fireType);

		// パーティクルを生成するかの真偽
		bool fireEffect = animeTime > fireTime && animeTime < fireTime + m_particleTime;

		// アニメーションが終わったら
		if (animaEnd)
		{
			// 発射中かの真偽をオフ
			m_status.Set(eStatus::IsFire) = false;
		}

		// パーティクルを生成するなら
		if (fireEffect)
		{
			// エフェクトの更新処理を実行
			EffectUpdate();
		}

		// アニメーションの更新
		m_ptrDraw->UpdateAnimation(DELTA_TIME * 1.5f);
	}

	// 大砲の回転処理
	void Cannon::RotateCannon()
	{
		// 発射中で回転してないなら何もせず終了
		if (m_status(eStatus::IsFire) && !m_status(eStatus::IsRotate))
		{
			return;
		}

		// 未発射かつ未回転なら
		if (!m_status(eStatus::IsFire, eStatus::IsRotate))
		{
			// 1.5秒のタイマーを設定
			if (SetTimer(1.5f))
			{
				m_status.Set(eStatus::IsRotate) = true; // 回転中の真偽をオンにする
				m_currentRot = GetRotation().z; // 回転角Z軸を保持
			}
		}

		// 回転中なら
		if (m_status(eStatus::IsRotate))
		{
			// 0.55秒のタイマーを設定
			SetTimer(0.55f);

			// 回転角をLerpで設定
			float time = GetTime(0.55f) * 2.0f; // Lerpに使う時間
			float end = m_currentRot - XM_PIDIV2; // 前回の回転角から90度引いた角度
			float rad = Utility::Lerp(m_currentRot, end, time); // Lerpで回転角を求める
			
			max(rad, end); // 終了角度以上なら修正

			// 角度の更新
			SetRotation(Vec3(0.0f, 0.0f, rad));

			// 回転角が終了時と同一なら回転を終了する
			if (rad == end) m_status.Set(eStatus::IsRotate) = false;
		}
	}

	// エフェクトの更新処理
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
			// 移動方向の設定
			float rad = GetRotation().z - XM_PIDIV2 + DegToRad(RangeRand(30.0f, -30.0f));
			sprite.m_Velocity = -Vec2(cos(rad), sin(rad)).normalize() * 20.0f;

			// スケールとローテーションZ軸の設定
			sprite.m_LocalScale = Vec2(RangeRand(1.0f, 0.1f));
			sprite.m_LocalQt.rotationZ(DegToRad(RangeRand(360.0f, 0.0f)));
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
			// 移動方向を設定
			float rad = GetRotation().z - XM_PIDIV2 + DegToRad(RangeRand(45.0f, -45.0f));
			sprite.m_Velocity = -Vec2(cos(rad), sin(rad)).normalize() * 10.0f;

			// スケールとローテーションZ軸の設定
			sprite.m_LocalScale = Vec2(RangeRand(5.0f, 2.5f));
			sprite.m_LocalQt.rotationZ(DegToRad(RangeRand(360.0f, 0.0f)));
		}
	}

	// 生成時の処理
	void GoalCannon::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Gimmick::OnCreate();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"G_CANNON");
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"FIRE", 0, 60, false);
		m_ptrDraw->AddAnimation(L"RAPID", 30, 30, false);

		// コリジョンOBBコンポーネントの設定
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		// パーティクルオブジェクトの生成
		m_particle = GetStage()->AddGameObject<MultiParticle>();

		// タグを設定
		AddTag(L"Goal");
	}

	// エフェクトの更新処理
	void GoalCannon::EffectUpdate()
	{
		// 紙のエフェクトを自身の位置に生成
		const auto paperParticle = m_particle.lock()->InsertParticle(100);
		paperParticle->SetEmitterPos(m_position);
		paperParticle->SetTextureResource(L"PAPER_TX");
		paperParticle->SetMaxTime(5.0f);
		paperParticle->SetDrawOption(Particle::Normal);

		// 生成したスプライトを配列で取得
		vector<ParticleSprite>& paperSpriteVec = paperParticle->GetParticleSpriteVec();

		// スプライトの数分ループ
		for (auto& sprite : paperSpriteVec)
		{
			// 移動方向の設定
			float rad = GetRotation().z - XM_PIDIV2 + DegToRad(RangeRand(45.0f, -45.0f));
			sprite.m_Velocity = -Vec2(cos(rad), sin(rad)).normalize() * RangeRand(10.0f, 5.0f);
			
			// スケールとローテーションZ軸の設定
			sprite.m_LocalScale = Vec2(RangeRand(2.0f, 0.5f));
			sprite.m_LocalQt.rotationZ(DegToRad(RangeRand(180.0f, 0.0f)));

			// 色を乱数に応じて設定
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

	// 生成時の処理
	void TitleCannon::OnCreate()
	{
		// 描画コンポーネントの取得
		m_ptrDraw = AddComponent<PNTBoneModelDraw>();

		// メッシュキーをを通常大砲に設定
		wstring meshKey = L"N_CANNON";

		// 難易度に応じてその難易度用の大砲にメッシュキーを変更
		switch (m_diffType)
		{
		case eDifficulty::Easy: // イージーなら
			meshKey = L"EASY_CANNON"; // イージー用の大砲
			m_angle = eAngle::Uleft; // 角度を左上方向に設定
			break;

		case eDifficulty::Normal: // ノーマルなら
			meshKey = L"NORMAL_CANNON"; // ノーマル用の大砲
			m_angle = eAngle::Up; // 角度を上方向に設定
			break;

		case eDifficulty::Hard: // ハードなら
			meshKey = L"HARD_CANNON"; // ハード用の大砲
			m_angle = eAngle::Uright; // 角度を右上方向に設定
			break;

		default:
			break;
		}

		// 描画コンポーネントの設定
		m_ptrDraw->SetMeshResource(meshKey);
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->AddAnimation(L"FIRE", 0, 60, false);

		// 継承元の生成時の処理を実行
		Gimmick::OnCreate();

		// コリジョンの設定
		auto ptrColl = GetComponent<CollisionObb>();
		ptrColl->SetUpdateActive(true);
		ptrColl->SetAfterCollision(AfterCollision::None);

		// パーティクル用オブジェクトの生成
		m_particle = GetStage()->AddGameObject<MultiParticle>();

		// タグを設定
		AddTag(L"Cannon");
	}
}