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
		Vec3 m_position;
		Vec3 m_rotation;
		Vec2 m_velocity;
		
		vector<weak_ptr<DebugSphere>> m_aligment;

		const float m_maxAcsel;

		float m_timeSpeed;
		float m_acsel;
		float m_speed;
		float m_gravity;
		bool m_isAir;

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_maxAcsel(5.0f)
		{
			m_position.zero();
			m_rotation.zero();
			m_velocity = Vec2(0.0f, 1.5f);
			m_timeSpeed = 0.3f;
			m_acsel = 0.0f;
			m_speed = 10.0f;
			m_gravity = -5.0f;
			m_isAir = true;
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
		void OnCollisionEnter(shared_ptr<GameObject>& other) override;

		/*!
		@brief 衝突している間呼び出される関数
		*/
		void OnCollisionExcute(shared_ptr<GameObject>& other) override;

		/*!
		@brief プレイヤーの移動関数
		*/
		void MovePlayer();

		void AligmentRotate();
	};
}