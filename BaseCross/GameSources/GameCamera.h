/*!
@file GameCamera.h
@brief ゲーム中カメラ
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief ゲーム中カメラ
	*/
	class GameCamera : public Camera
	{
		// ターゲットオブジェクトポインタ
		weak_ptr<GameObject> m_target; 

		const Vec3 m_startEye; // 開始時の視点の座標
		const Vec3 m_startAt;  // 開始時の注視点の座標
		const Vec3 m_startTgt; // 開始時のターゲットの座標
		Vec3 m_targetPos;      // ターゲットの座標
		Vec3 m_eyePos;         // 視点の座標
		Vec3 m_atPos;		   // 注視点の座標

	public:

		/*!
		@brief コンストラクタ
		*/
		GameCamera() :
			m_startEye(Vec3(-20.5f, -82.3f, -33.0f)),
			m_startAt(Vec3(-20.5f, -82.3f, 0.0f)),
			m_startTgt(Vec3(-35.0f, -88.5f, 0.0f))
		{
			m_targetPos = m_startTgt;
			m_eyePos = m_startEye;
			m_atPos = m_startAt;
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~GameCamera() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief ターゲットオブジェクト設定関数
		@param ターゲットオブジェクトポインタ
		*/
		void SetTargetObject(const shared_ptr<GameObject>& object)
		{
			// 現在のターゲットと違ったら更新
			if (m_target.lock() != object)
			{
				m_target = object;
			}
		}

		/*!
		@brief ターゲット設定解除関数
		*/
		void RemoveTarget()
		{
			// ターゲットの座標を保持してポインタの保持を解除
			m_targetPos = m_target.lock()->GetComponent<Transform>()->GetPosition();
			m_target.reset();
		}

		/*!
		@brief リセット関数
		*/
		void ResetCamera()
		{
			// 開始時の視点注視点の座標に更新
			m_eyePos = m_startEye;
			m_atPos = m_startAt;
			SetEye(m_eyePos);
			SetAt(m_atPos);

			// ターゲットの座標を開始時の位置に更新
			m_targetPos = m_startTgt;
		}

		/*!
		@brief ターゲットの座標取得関数
		@return m_targetPos
		*/
		const Vec3& GetTargetPos() const
		{
			return m_targetPos;
		}
	};
}