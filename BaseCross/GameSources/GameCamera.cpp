/*!
@file GameCamera.cpp
@brief ゲーム中カメラ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void GameCamera::OnCreate()
	{
		// 視点と注視点の座標を設定
		SetEye(m_startEye);
		SetAt(m_startAt);
	}

	// 毎フレーム更新処理
	void GameCamera::OnUpdate()
	{
		Vec3 targetPos; // 保存用変数

		// ターゲットのポインタがあるか
		if (m_target.lock())
		{
			// ターゲットの座標を取得して保持
			auto targetTrans = m_target.lock()->GetComponent<Transform>();
			targetPos = targetTrans->GetPosition();
			m_targetPos = targetPos;
		}
		else
		{
			// 前回までのターゲットの座標を代入
			targetPos = m_targetPos;
		}

		// パラメータの設定
		const float underDiff = 1.5f; // Y軸座標の差分
		const float followDistance = 0.2f; // 追尾距離
		const float speed = 4.0f; // 追尾速度
		const Vec3 atDiff = Vec3(10.0f, 7.5f, 0.0f); // 注視点からの差分
		bool down = (targetPos.y <= -87.5f); // 下限かの真偽
		bool left = (targetPos.x <= -30.5f); // 左限かの真偽

		targetPos.y -= underDiff;		// ターゲットの座標から差分を引く
		if (down) targetPos.y = -87.5f; // 下限なら下限座標に置き換え
		if (left) targetPos.x = -30.5f; // 左限なら左眼座標に置き換え

		// 座標の設定と距離の取得
		Vec3 vec = targetPos - (m_atPos - atDiff);
		float distance = vec.length();

		// 追尾距離より大きかったら追尾する
		if (distance > followDistance)
		{
			// 移動量を設定(距離の差分 × 速度)
			float move = (distance - followDistance) * (DELTA_TIME * speed);

			// 移動量分を加算
			m_eyePos += Vec3(vec.normalize().x * move, vec.normalize().y * move, 0.0f);
			m_atPos += Vec3(vec.normalize().x * move, vec.normalize().y * move, 0.0f);
		}

		// 視点と注視点の座標を設定
		SetEye(m_eyePos);
		SetAt(m_atPos);
	}
}