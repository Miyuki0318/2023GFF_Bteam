/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "DebugSphere.h"

namespace basecross
{
	class Player : public GameObject
	{
		shared_ptr<Transform> m_ptrTrans;
		shared_ptr<PNTBoneModelDraw> m_bodyDraw;
		shared_ptr<PNTBoneModelDraw> m_armDraw;

		Vec3 m_position;
		Vec3 m_respawnPos;
		Vec3 m_rotation;
		Vec3 m_scale;
		Vec2 m_velocity;
		const Vec2 m_deffVelo;
		Mat4x4 m_bodyMat;
		Mat4x4 m_armMat;

		weak_ptr<DebugObject> m_arm;
		weak_ptr<Billboard> m_effect;
		weak_ptr<Cannon> m_activeCannon;
		vector<weak_ptr<DebugSphere>> m_aligment;

		const float m_maxAcsel;
		const float m_damageAcsel;
		const float m_slowTime;
		const float m_normalTime;

		float m_timeSpeed;
		float m_acsel;
		float m_speed;
		float m_gravity;
		bool m_isAir;
		bool m_firePossible;
		bool m_cannonStandby;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_maxAcsel(4.5f),
			m_damageAcsel(3.0f),
			m_slowTime(0.1f),
			m_normalTime(1.5f),
			m_deffVelo(0.0f, -1.0f)
		{
			m_position = Vec3(10.0f, 15.0f, 0.0f);
			m_respawnPos = Vec3(10.0f, 15.0f, 0.0f);
			m_rotation.zero();
			m_scale = Vec3(1.0f);
			m_velocity = m_deffVelo;
			m_timeSpeed = m_normalTime;
			m_acsel = 1.0f;
			m_speed = 4.0f;
			m_gravity = -5.0f;
			m_isAir = true;
			m_firePossible = true;
			m_cannonStandby = false;

			m_bodyMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -0.65f, 0.0f)
			);

			m_armMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -1.05f, 0.0f)
			);
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~Player() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief Aボタンが押された時に呼び出される関数
		*/
		void OnPushA();

		/*!
		@brief Aボタンが押されなくなった時に呼び出される関数
		*/
		void OnReleaseA();

		/*!
		@brief 衝突した瞬間に呼び出される関数
		*/
		void OnCollisionEnter(const CollisionPair& Pair) override;

		/*!
		@brief 衝突している間呼び出される関数
		*/
		void OnCollisionExcute(const CollisionPair& Pair) override;

		/*!
		@brief 衝突されなくなったら呼び出される関数
		*/
		void OnCollisionExit(const CollisionPair& Pair) override;

		/*!
		@brief プレイヤーの移動関数
		*/
		void MovePlayer();

		/*!
		@brief プレイヤーの移動量減少関数
		*/
		void MoveReduction();

		/*!
		@brief プレイヤーの回転関数
		*/
		void RotatePlayer();

		/*!
		@brief エアショックの軌道描画関数
		*/
		void RotateAligment();

		/*!
		@brief アニメーション更新関数
		*/
		void AnimationUpdate();

		/*!
		@brief エフェクト描画関数
		*/
		void EffectUpdate();

		/*!
		@brief 大砲待機関数
		*/
		void CannonStandby();

		void BlockEnter(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		void BlockExcute(const shared_ptr<GameObject>& block, const Vec3& hitPos);

		void BlockExit(const shared_ptr<GameObject>& block);

		bool BlockUpperCheck(const Vec3& upperPos);

		bool BlockUnderCheck(const Vec3& underPos);

		void SpikeEnter(const shared_ptr<GameObject>& spike, const Vec3& hitPos);

		void SpikeExcute(const shared_ptr<GameObject>& spike, const Vec3& hitPos);

		void BirdEnter(const shared_ptr<GameObject>& enemy, const Vec3& hitPos);

		void CannonEnter(const shared_ptr<GameObject>& cannon);

		void DamageKnockBack(const Vec2& velocity);

		bool CollHitUpper(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale);

		bool CollHitUnder(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale);

		bool CollHitLeft(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale);

		bool CollHitRight(const Vec3& hitPos, const Vec3& hitObjPos, const Vec3& helfScale);
	};
}